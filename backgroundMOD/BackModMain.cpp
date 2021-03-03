// Vedio_detect_human.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"  
// �˶������⡪����������
#include "opencv2/opencv.hpp"  
#include <highgui.hpp>
#include <opencv2\imgproc\types_c.h>
using namespace cv;
#include <iostream>  
using namespace std;
// �˶������⺯������  

Mat MoveDetect(Mat background, Mat frame);

int staticMOD_main()
{

	VideoCapture video("fisheye.wmv");//����VideoCapture��video  
	if (!video.isOpened())  //��video�����쳣���  
	{
		cout << "video open error!" << endl;
		return 0;
	}
	// ��ȡ֡��
	int frameCount = video.get(CAP_PROP_FRAME_COUNT);
	// ��ȡFPS 
	double FPS = video.get(CAP_PROP_FPS);
	// �洢֡
	Mat frame;
	// �洢����ͼ��
	Mat background;
	// �洢���ͼ��

	Mat result;
	for (int i = 0; i < frameCount; i++)
	{
		// ��֡��frame
		video >> frame;
		imshow("frame", frame);
		// ��֡�����쳣���
		if (frame.empty())
		{
			cout << "frame is empty!" << endl;
			break;
		}
		// ��ȡ֡λ��(�ڼ�֡)
		int framePosition = video.get(CAP_PROP_POS_FRAMES);
		cout << "framePosition: " << framePosition << endl;
		// ����һ֡��Ϊ����ͼ��
		if (framePosition == 2||framePosition==1)
			background = frame.clone();
		
		// ����MoveDetect()�����˶������⣬����ֵ����result
		result = MoveDetect(background, frame);
		imshow("result", result);
		// ��ԭFPS��ʾ
		if (waitKey(1000.0 / FPS) == 27)
		{
			cout << "ESC�˳�!" << endl;
			break;
		}
	}
	system("pause");
	return 0;
}
Mat MoveDetect(Mat background, Mat frame)
{
	Mat result = frame.clone();
	// 1.��background��frameתΪ�Ҷ�ͼ  
	Mat gray1, gray2;

	cvtColor(background, gray1, CV_BGR2GRAY);
	cvtColor(frame, gray2, CV_BGR2GRAY);
	// 2.��background��frame����  
	Mat diff;
	absdiff(gray1, gray2, diff);
	imshow("diff", diff);
	// 3.�Բ�ֵͼdiff_thresh������ֵ������  
	Mat diff_thresh;
	threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
	imshow("diff_thresh", diff_thresh);
	// 4.��ʴ  
	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
	erode(diff_thresh, diff_thresh, kernel_erode);
	imshow("erode", diff_thresh);
	// 5.����  
	dilate(diff_thresh, diff_thresh, kernel_dilate);
	imshow("dilate", diff_thresh);
	// 6.������������������  
	vector<vector<Point>> contours;
	findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	// ��result�ϻ�������
	drawContours(result, contours, -1, Scalar(0, 0, 255), 2);
	// 7.��������Ӿ���  
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = boundingRect(contours[i]);
		// ��result�ϻ�������Ӿ���
		rectangle(result, boundRect[i], Scalar(255, 255, 0), 2);
	}
	// ����result
	return result;
}

