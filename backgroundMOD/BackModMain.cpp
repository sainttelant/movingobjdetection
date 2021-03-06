// Vedio_detect_human.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"  
// 运动物体检测——背景减法
#include "opencv2/opencv.hpp"  
#include <highgui.hpp>
#include <opencv2\imgproc\types_c.h>
using namespace cv;
#include <iostream>  
using namespace std;
// 运动物体检测函数声明  

Mat MoveDetect(Mat background, Mat frame);

int staticMOD_main()
{

	VideoCapture video("fisheye.wmv");//定义VideoCapture类video  
	if (!video.isOpened())  //对video进行异常检测  
	{
		cout << "video open error!" << endl;
		return 0;
	}
	// 获取帧数
	int frameCount = video.get(CAP_PROP_FRAME_COUNT);
	// 获取FPS 
	double FPS = video.get(CAP_PROP_FPS);
	// 存储帧
	Mat frame;
	// 存储背景图像
	Mat background;
	// 存储结果图像

	Mat result;
	for (int i = 0; i < frameCount; i++)
	{
		// 读帧进frame
		video >> frame;
		imshow("frame", frame);
		// 对帧进行异常检测
		if (frame.empty())
		{
			cout << "frame is empty!" << endl;
			break;
		}
		// 获取帧位置(第几帧)
		int framePosition = video.get(CAP_PROP_POS_FRAMES);
		cout << "framePosition: " << framePosition << endl;
		// 将第一帧作为背景图像
		if (framePosition == 2||framePosition==1)
			background = frame.clone();
		
		// 调用MoveDetect()进行运动物体检测，返回值存入result
		result = MoveDetect(background, frame);
		imshow("result", result);
		// 按原FPS显示
		if (waitKey(1000.0 / FPS) == 27)
		{
			cout << "ESC退出!" << endl;
			break;
		}
	}
	system("pause");
	return 0;
}
Mat MoveDetect(Mat background, Mat frame)
{
	Mat result = frame.clone();
	// 1.将background和frame转为灰度图  
	Mat gray1, gray2;

	cvtColor(background, gray1, CV_BGR2GRAY);
	cvtColor(frame, gray2, CV_BGR2GRAY);
	// 2.将background和frame做差  
	Mat diff;
	absdiff(gray1, gray2, diff);
	imshow("diff", diff);
	// 3.对差值图diff_thresh进行阈值化处理  
	Mat diff_thresh;
	threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
	imshow("diff_thresh", diff_thresh);
	// 4.腐蚀  
	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
	erode(diff_thresh, diff_thresh, kernel_erode);
	imshow("erode", diff_thresh);
	// 5.膨胀  
	dilate(diff_thresh, diff_thresh, kernel_dilate);
	imshow("dilate", diff_thresh);
	// 6.查找轮廓并绘制轮廓  
	vector<vector<Point>> contours;
	findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	// 在result上绘制轮廓
	drawContours(result, contours, -1, Scalar(0, 0, 255), 2);
	// 7.查找正外接矩形  
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = boundingRect(contours[i]);
		// 在result上绘制正外接矩形
		rectangle(result, boundRect[i], Scalar(255, 255, 0), 2);
	}
	// 返回result
	return result;
}

