#include<iostream>
//#include"highgui.h"
//#include"cv.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

using namespace std;
using namespace cv;

int main_mask()
{
	IplImage* img1 = cvCreateImage(cvSize(320, 180), IPL_DEPTH_8U, 3);
	//uchar r1, g1, b1;
	for (int i = 0; i < img1->height; i++)
	{
		uchar *ptrImage = (uchar*)(img1->imageData + i * img1->widthStep);
		//uchar *ptrDst = (uchar*)(img->imageData + i * img->widthStep);

		for (int j = 0; j < img1->width; j++)
		{
			//b1 = ptrImage[3 * j + 0];
			//g1 = ptrImage[3 * j + 1];
			//r1 = ptrImage[3 * j + 2];

			//ptrDst[3 * (j + 400) + 0] = 0;
			//ptrDst[3 * (j + 400) + 1] = 0;
			//ptrDst[3 * (j + 400) + 2] = 0;
			ptrImage[3 * j + 0] = 0;
			ptrImage[3 * j + 1] = 0;
			ptrImage[3 * j + 2] = 0;
		}
	}
	cvShowImage("ceshi", img1);
	cvWaitKey(0);
	system("pause");
	return 0;
}
