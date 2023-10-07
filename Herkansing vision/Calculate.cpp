#include "Calculate.h"

using namespace std;
using namespace cv;

/*  
	get_type function calculates the length, width and the amount of holes the contour has. It uses those values to determine what gasket type it is  
	Input:	RotatedRect	bound_box	| A bounding box
	Output:	int type				| Returns type of the gasket
*/
int Calculate::get_type(RotatedRect bound_box) {
	int type;
	Point2f rectPoints[4];
	bound_box.points(rectPoints);
	size = bound_box.size;

	if (size.height < size.width) {
		float temp_length = size.width;
		float temp_width = size.height;
		size.height = temp_length;
		size.width = temp_width;
	}

	if (size.height / length_scale >= 314 && size.height / length_scale <= 320) {
		type = 1;
	}
	else if (size.height / length_scale >= 367 && size.height / length_scale <= 373) {
		type = 2;
	}
	else if (size.height / length_scale >= 352 && size.height / length_scale <= 358) {
		type = 3;
	}
	else {
		type = 0;
	}
	return type;
}

/*
	get_main_area function calculates the area of the gasket itself, excluding the holes.
	Input:	vector<Point> contour	| A vector of the detected area
	Output:	float contour_area		| The area of the detected contour
*/
float Calculate::get_main_area(vector<Point> contour) {
	float contour_area = contourArea(contour) / area_scale;
	return contour_area;
}

/*
	get_hole_diameter function calculates the diameters of the holes. It detects holes apart from other shapes.
	Input:	vector<vector<Point>> contours	| A vector of all detected areas
	Output:	vector<float> diameter			| Vector of the diameter of all holes
*/
vector<float> Calculate::get_hole_diameter(vector<vector<Point>> contours) {
	vector<float> diameter;

	diameter.resize(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		float contour_area = contourArea(contours[i]) / area_scale;
		float peri = arcLength(contours[i], true);

		float circularity = (peri * peri) / (4 * M_PI * contour_area);
		
		/*cout << "Area: " << contour_area << endl;
		cout << "Circularity: " << circularity << endl;
		cout << "Perimeter: " << peri << endl;*/
		
		Rect boundingBox = boundingRect(contours[i]);
		double aspectRatio = static_cast<double>(boundingBox.width) / boundingBox.height;

		if (circularity > 0.8 && aspectRatio < 1.2) {
			diameter[i] = sqrt(contour_area / M_PI) * 2;
		}
	}
	diameter.erase(remove(diameter.begin(), diameter.end(), 0.0), diameter.end());
	return diameter;
}

/*
	get_pos function calculates all positions of the contours in polarcoordinates
	Input:	vector<RotatedRect>	bound_box		| A vector of bounding boxes
			vector<vector<Point>> contours		| A vector of the detected areas
	Output:	vector<Point> rotated_contour_pos	| Returns the positions of the holes 
*/
vector<Point2f> Calculate::get_pos(vector<RotatedRect> bound_box, vector<vector<Point>> contours, RotatedRect reference) {
	vector<Point2f> rotated_contour_pos(contours.size());
	for (int i = 1; i < contours.size(); i++) {
		bound_box[i] = minAreaRect(contours[i]);
		Point2f rectPoints[4];
		bound_box[i].points(rectPoints);
		float dy = bound_box[i].center.y - reference.center.y;
		float dx = bound_box[i].center.x - reference.center.x;
		float angle = atan2(dy, dx) * 180 / M_PI;
		float arm = abs(sqrt(pow(dy, 2) + pow(dx, 2)) / diagonal_scale);

		rotated_contour_pos[i].x = arm;
		rotated_contour_pos[i].y = angle - reference.angle;
		//cout << arm << " " << angle << endl;
	}
	return rotated_contour_pos;
}