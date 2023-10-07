#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <corecrt_math_defines.h>
#include <cmath>


using namespace std;
using namespace cv;

class Calculate
{
private:

public:
	float length_scale = 2048 / 392 + 0.35;
	//float length_scale = 1.542586751;
	float width_scale = 1536 / 294 + 0.35;
	//float width_scale = 1.575757576;
	float area_scale = length_scale * width_scale;
	float diagonal_scale = sqrt(pow(length_scale, 2) + pow(width_scale, 2));
	Size2f size;

	int get_type(RotatedRect bound_box);
	float get_main_area(vector<Point> contour);
	vector<Point2f> get_pos(vector<RotatedRect> bound_box, vector<vector<Point>> contours, RotatedRect reference);
	vector<float> get_hole_diameter(vector<vector<Point>> contours);
};