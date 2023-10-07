#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class Processor
{
public:
	Mat src2bin(Mat src, Mat eKernell, Mat dKernel);
	vector<vector<Point>> drawAllContours(Mat src, Mat drawing);
	vector<RotatedRect> bBox(vector<vector<Point>> contours, Mat drawing);
};

