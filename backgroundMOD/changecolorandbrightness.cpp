#include "opencv2/opencv.hpp"  
#include <opencv2\imgproc\types_c.h>
using namespace cv;
#include <iostream>  
using namespace std;


double alpha; /*< Simple contrast control */
int beta;  /*< Simple brightness control */
int main_change()
{

	Mat image = imread("C:\\Users\\XUEWEI\\Pictures\\Saved Pictures\\safepro.jpg");
	Mat new_image = Mat::zeros(image.size(), image.type()); //��image ��С(h*w*c) ����һ��
	std::cout << " Basic Linear Transforms " << std::endl;
	std::cout << "-------------------------" << std::endl;
	std::cout << "* Enter the alpha value [1.0-3.0]: "; std::cin >> alpha;
	std::cout << "* Enter the beta value [0-100]: "; std::cin >> beta;
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < 3; c++) {
				new_image.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(alpha*(image.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}

	namedWindow("Original Image", 1);
	namedWindow("New Image", 1);
	imshow("Original Image", image);
	imshow("New Image", new_image);
	imwrite("C:\\Users\\XUEWEI\\Pictures\\Saved Pictures\\safepro_new.jpg", new_image);
	waitKey();

	system("pause");
	return 0;
}