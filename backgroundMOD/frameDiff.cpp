// Vedio_detect_human.cpp : �������̨Ӧ�ó������ڵ㡣
//
// �˶������⡪��֡�  
#include "opencv2/opencv.hpp"  
#include <opencv2\imgproc\types_c.h>
using namespace cv;
#include <iostream>  
using namespace std;
// �˶������⺯������  
Mat MoveDetect_frameDiff(Mat temp, Mat frame);

int main()
{
	// ����VideoCapture��video
	VideoCapture video("fisheye.wmv");
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
	// �洢ǰһ֡ͼ��
	Mat temp;
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
		// ���Ϊ��һ֡��temp��Ϊ�գ�
		if (i == 0)
		{
			// ����MoveDetect()�����˶������⣬����ֵ����result
			result = MoveDetect_frameDiff(frame, frame);
		}
		//�����ǵ�һ֡��temp��ֵ�ˣ�
		else
		{
			// ����MoveDetect()�����˶������⣬����ֵ����result
			result = MoveDetect_frameDiff(temp, frame);
		}
		imshow("result", result);
		// ��ԭFPS��ʾ
		if (waitKey(1000.0 / FPS) == 27)
		{
			cout << "ESC�˳�!" << endl;
			break;
		}
		temp = frame.clone();
	}
	return 0;


}
Mat MoveDetect_frameDiff(Mat temp, Mat frame)
{
	Mat result = frame.clone();
	// 1.��background��frameתΪ�Ҷ�ͼ  
	Mat gray1, gray2;
	cvtColor(temp, gray1, CV_BGR2GRAY);
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
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
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
		rectangle(result, boundRect[i], Scalar(0, 255, 0), 2);
	}
	// ����result
	return result;
}