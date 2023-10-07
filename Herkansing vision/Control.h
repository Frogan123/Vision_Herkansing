#pragma once
#include <vector>
#include <opencv2/imgcodecs.hpp>

#include "init.h"

using namespace std;
using namespace cv;

class Control
{
public: 
	Control(int type);
	bool control_position(vector<Point2f> holes_pos);
	bool control_diameter(vector<float> diameters);
	bool control_area(float area);
	bool control_dim(float length, float width);
	double distance_tolerance = 2;
	double angle_tolerance = 1;
	double area_tolerance = 500;

private:
	int type;
};