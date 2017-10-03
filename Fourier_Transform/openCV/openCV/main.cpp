#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <highgui.h>
#include "cv.h"
#include <cmath>
#include <complex>
#include <algorithm>
using namespace std;
using namespace cv;
#define pi 3.141592653
complex <double> temp[1000][1000] = { (0,0) };
complex <double> re[1000][1000] = { (0,0) };
int he[1000][1000] = { 0 };
int main(){

	// Read input images
	// Fig3.tif is in openCV\bin\Release
	Mat SrcImg = imread("Q4.tif", CV_LOAD_IMAGE_GRAYSCALE);

	// Create a grayscale output temp image matrix (0,0)
	Mat DstImgtmp = Mat(SrcImg.rows, SrcImg.cols, CV_8UC1);

	// Create a grayscale output image matrix  (center point)
	Mat DstImg = Mat(SrcImg.rows, SrcImg.cols, CV_8UC1);

	// Perform 1D-DFT  (image[x][y]->temp[x][v])

	double N = (2 * pi) / SrcImg.cols;
	for (int i = 0; i < SrcImg.rows; ++i)
	for (int a = 0; a < SrcImg.cols; ++a)
	{
		for (int b = 0; b < SrcImg.cols; ++b)
		{
			complex<double> s = (int)SrcImg.at<uchar>(i, b);
			temp[i][a] += s / complex<double>(cos(N*a*b), sin(N*a*b));
		}
	}

	// Perform 1D-DFT (temp[x][v]->result[u][v])
	double M = (2 * pi) / SrcImg.rows;
	for (int i = 0; i < SrcImg.cols; ++i)
	for (int a = 0; a < SrcImg.rows; ++a)
	{
		for (int b = 0; b < SrcImg.rows; ++b)
		{

			re[a][i] += temp[b][i] / complex<double>(cos(M*a*b), sin(M*a*b));
		}
	}
	//Calculate the complex number's value(distance)
	for (int i = 0; i<SrcImg.rows; ++i)
	for (int j = 0; j<SrcImg.cols; ++j)
	{
		he[i][j] = sqrt(re[i][j].real()*re[i][j].real() + re[i][j].imag()*re[i][j].imag());
	}
	//Map the value to 0~255 
	for (int i = 0; i<SrcImg.rows; ++i)
	for (int j = 0; j<SrcImg.cols; ++j)
	{
		he[i][j] /= (SrcImg.rows / 2); //devided by a constant
		if (he[i][j] >= 255)
			he[i][j] = 255;
		DstImgtmp.at<uchar>(i, j) = he[i][j];
	}
	//Map (0,0) to the center point and rearrange the image
	int cx = DstImgtmp.cols / 2;
	int cy = DstImgtmp.rows / 2;

	Mat q0(DstImgtmp, Rect(0, 0, cx, cy));   // Top Left
	Mat q1(DstImgtmp, Rect(cx, 0, cx, cy));  // Top Right
	Mat q2(DstImgtmp, Rect(0, cy, cx, cy));  // Bottom Left
	Mat q3(DstImgtmp, Rect(cx, cy, cx, cy)); // Bottom Right

	Mat p3(DstImg, Rect(0, 0, cx, cy));   // Top Left
	Mat p2(DstImg, Rect(cx, 0, cx, cy));  // Top Right
	Mat p1(DstImg, Rect(0, cy, cx, cy));  // Bottom Left
	Mat p0(DstImg, Rect(cx, cy, cx, cy)); // Bottom Right
	                            
	q0.copyTo(p0);
	q1.copyTo(p1);
	q2.copyTo(p2);
	q3.copyTo(p3);
	
	// Show images
	imshow("Input Image", SrcImg);
	imshow("Output Image", DstImg);

	// Write output images
	imwrite("Q4_output.tif", DstImg);

	waitKey(0);
	return 0;
}