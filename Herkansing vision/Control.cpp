#include "Control.h"

/*
	control_gasket function checks every hole position and diameter
	Input:	int type						| Type of gasket
			vector<float> hole_diameter		| Diameter of the holes
			vector<Point> holes_pos			| Position of the holes
	Output: vector<vector<int>> hole_check	| Vector containing the vectors of which holes do, and which holes don't, meet the requirements
*/

Control::Control(int type)
	: type(type) {}

bool isApproximatelyEqual(double a, double b, double tolerance) {
	return fabs(a - b) <= tolerance;
}

bool Control::control_position(vector<Point2f> holes_pos)
{
	Init gasket1_pos("Holes_type_1.txt", 2, 41);
	gasket1_pos.files_initialize();
	vector<vector<int>> array1 = gasket1_pos.get_array();

	Init gasket2_pos("Holes_type_2.txt", 2, 37);
	gasket2_pos.files_initialize();
	vector<vector<int>> array2 = gasket2_pos.get_array();

	Init gasket3_pos("Holes_type_3.txt", 2, 34);
	gasket3_pos.files_initialize();
	vector<vector<int>> array3 = gasket3_pos.get_array();

	if (type != 0) {
		switch (type) {
		case 1:
			if (holes_pos.size() == array1.size()) {
				for (const auto& subArray : array1) {
					for (int value : subArray) {
						// Convert the integer value to a Point2f for comparison
						Point2f point(static_cast<float>(value), static_cast<float>(value));
						if (find(holes_pos.begin(), holes_pos.end(), point) == holes_pos.end()) {
							return false; // Found a point in array1 that is not in points
						}
					}
				}
				return true; // All points in array1 are in points
			}
			else {
				return false;
			}
		case 2:
			if (holes_pos.size() == array2.size()) {
				for (const auto& subArray : array1) {
					for (int value : subArray) {
						// Convert the integer value to a Point2f for comparison
						Point2f point(static_cast<float>(value), static_cast<float>(value));
						if (find(holes_pos.begin(), holes_pos.end(), point) == holes_pos.end()) {
							return false; // Found a point in array1 that is not in points
						}
					}
				}
				return true; // All points in array1 are in points
			}
			else {
				return false;
			}
		case 3:
			if (holes_pos.size() == array3.size()) {
				for (const auto& subArray : array3) {
					for (int value : subArray) {
						// Convert the integer value to a Point2f for comparison
						Point2f point(static_cast<float>(value), static_cast<float>(value));
						if (find(holes_pos.begin(), holes_pos.end(), point) == holes_pos.end()) {
							return false; // Found a point in array1 that is not in points
						}
					}
				}
				return true; // All points in array1 are in points
			}
			else {
				return false;
			}
		}
	}
	else {
		return false;
	}
}

bool Control::control_diameter(vector<float> diameters) {
	Init gasket1_diameter("Holes_type_1.txt", 1, 41);
	gasket1_diameter.files_initialize();
	vector<vector<int>> array1 = gasket1_diameter.get_array();

	Init gasket2_diameter("Holes_type_2.txt", 1, 37);
	gasket2_diameter.files_initialize();
	vector<vector<int>> array2 = gasket2_diameter.get_array();

	Init gasket3_diameter("Holes_type_3.txt", 1, 34);
	gasket3_diameter.files_initialize();
	vector<vector<int>> array3 = gasket3_diameter.get_array();

	if (type != 0) {
		switch (type) {
		case 1:
			if (diameters.size() == array1.size()) {
				for (int row = 0; row < diameters.size(); row++) {
					for (const auto& subArray : array1) {
						for (int value : subArray) {
							if (find(diameters.begin(), diameters.end(), value) == diameters.end()) {
								return false; // Found a value in array1 that is not in diameters
							}
						}
					}
				}			
				return true; // All values in array1 are in diameters
			}
			else {
				return false;
			}
		case 2:
			if (diameters.size() == array2.size()) {
				for (int row = 0; row < diameters.size(); row++) {
					for (const auto& subArray : array2) {
						for (int value : subArray) {
							if (find(diameters.begin(), diameters.end(), value) == diameters.end()) {
								return false; // Found a value in array1 that is not in diameters
							}
						}
					}
				}
				return true; // All values in array1 are in diameters
			}
			else {
				return false;
			}
		case 3:
			if (diameters.size() == array3.size()) {
				for (int row = 0; row < diameters.size(); row++) {
					for (const auto& subArray : array3) {
						for (int value : subArray) {
							if (find(diameters.begin(), diameters.end(), value) == diameters.end()) {
								return false; // Found a value in array1 that is not in diameters
							}
						}
					}
				}
				return true; // All values in array1 are in diameters
			}
			else {
				return false;
			}
		}
	}
	else {
		return false;
	}
}

bool Control::control_area(float area) {
	switch (type) {
	case 1:
		return isApproximatelyEqual(area, 44000, area_tolerance);
	case 2:
		return isApproximatelyEqual(area, 54000, area_tolerance);
	case 3:
		return isApproximatelyEqual(area, 47000, area_tolerance);
	}
}

bool Control::control_dim(float length, float width) {
	switch (type) {
	case 1:
		if (isApproximatelyEqual(length, 317, distance_tolerance) && isApproximatelyEqual(width, 165, distance_tolerance)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 2:
		if (isApproximatelyEqual(length, 370, distance_tolerance) && isApproximatelyEqual(width, 160, distance_tolerance)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 3:
		if (isApproximatelyEqual(length, 355, distance_tolerance) && isApproximatelyEqual(width, 140, distance_tolerance)) {
			return true;
		}
		else {
			return false;
		}
		break;
	}
}
