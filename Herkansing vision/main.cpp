/*  Vision solution of gasket inspection  */


/*
Assignment title	|	Gasket Inspection WinnerParts
Duncan Kikkert		|	2171399
Florian Buren		|	2172850
*/


/*
This C++ program uses the OpenCV library for image processing and comprehensive quality control for real-time inspection of three different types of gaskets.
It performs object detection, measurements, and error checking, providing valuable information about the provided gaskets.
It follows these main steps:

Initialization:
	The program begins by including necessary libraries, declaring namespaces, and initializing variables and data structures.

Image Processing Setup:
	It sets up components for image processing, including structuring elements and an empty image for visualization.

Control File Initialization:
	The program initializes control files and handles potential errors associated with file loading.

Camera Initialization:
	Video capture is set up from camera index 3, with checks to ensure a successful setup.

Main Processing Loop:
	The program enters a continuous loop for real-time processing of camera frames. Within this loop:

Image Processing:
	Utilizes the Processor class for operations such as binarization, contour detection, and bounding box calculations.

Gasket Calculations:
	Uses the Calculate class to determine gasket type, dimensions, and area.

Hole Analysis:
	Extracts information about holes, including their positions and diameters.

Gasket Control and Error Handling:
	The Control class manages gasket-related parameters and tracks errors in hole positions and diameters.

Time Measurements:
	The program measures the time taken for each cycle and displays it.

Turn-off Button:
	Checks for the 'e' or 'E' keypress, enabling manual intervention to reset the system. The total runtime is displayed before exiting.

Timer:
	There's an option (commented out) to introduce a delay between processing cycles using this_thread::sleep_for.
*/

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <chrono>
#include <fstream>

#include "Init.h"
#include "Processor.h"
#include "Calculate.h"
#include "Control.h"

using namespace std;
using namespace cv;
using namespace std::chrono;

int main () {
	/*  Initialize Timer  */
	auto programStart = high_resolution_clock::now();

	/*  Initialize Variables  */
	char key;
	bool resetButton;
	unsigned int type, gasket_length, gasket_width, gasket_area;
	unsigned int gasket_counter = 0;
	unsigned int type1_counter = 0;
	unsigned int type2_counter = 0;
	unsigned int type3_counter = 0;
	unsigned int type0_counter = 0;
	unsigned int type1_rej = 0;
	unsigned int type2_rej = 0;
	unsigned int type3_rej = 0;
	Mat src, bin, cont, inv_bin;
	vector<float> hole_diameter;
	vector<Point2f> holes_pos;
	vector<vector<Point>> contours, inv_contours;
	vector<RotatedRect> boundingBox, inv_boundingBox;

	/*  Initialize Image Processing  */
	Mat eKernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat dKernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);

	/*  Initialize Camera  */
	VideoCapture cap(0);
	cap.set(CAP_PROP_FRAME_WIDTH, 2048);
	cap.set(CAP_PROP_FRAME_HEIGHT, 1536);

	string path = "Resources/Gasket_Test_Type1.jpg";
	Mat img = imread(path);
	//imshow("image", img);
	while (true) {
		auto cycleStart = high_resolution_clock::now();	
		cap.read(src);
		//imshow("Source", src);
		waitKey(1);

		if (!cap.isOpened()) {
			cout << "No camera detected." << endl;
			break;
		}
		
		/* Processing image */
		Processor imProcessor;
		//bin = imProcessor.src2bin(src, eKernel, dKernel);
		bin = imProcessor.src2bin(img, eKernel, dKernel);
		//imshow("Image", bin);
		contours = imProcessor.drawAllContours(bin, drawing);
		boundingBox = imProcessor.bBox(contours, drawing);

		bitwise_not(bin, inv_bin);
		inv_contours = imProcessor.drawAllContours(inv_bin, drawing);
		inv_boundingBox = imProcessor.bBox(inv_contours, drawing);
		imshow("Image",inv_bin);

		/*  Gasket calculations  */
		Calculate gasket;
		type = gasket.get_type(inv_boundingBox[0]);
		gasket_length = gasket.size.height / gasket.length_scale;
		gasket_width = gasket.size.width / gasket.width_scale;
		gasket_area = gasket.get_main_area(inv_contours[0]);
		hole_diameter = gasket.get_hole_diameter(contours);
		holes_pos = gasket.get_pos(boundingBox, contours, inv_boundingBox[0]);
		
		/*  Gasket control and send to screen  */
		Control gasket_control(type);
		bool dim_control = gasket_control.control_dim(gasket_length, gasket_width);
		bool area_control = gasket_control.control_area(gasket_area);
		bool diameter_control = gasket_control.control_diameter(hole_diameter); 
		bool pos_control = gasket_control.control_position(holes_pos);

		cout << "Current gasket is of type: " << type << endl;
		if (!dim_control) {
			cout << "Dimensions of current gasket are: " << gasket_length << "mm x " << gasket_width << "mm." << endl;
			cout << "Dimensions are outside of tolerance" << endl;
		}
		else {
			cout << "Dimensions of current gasket are: " << gasket_length << "mm x " << gasket_width << "mm." << endl;
			cout << "Dimensions are correct." << endl;
		}
		if (!area_control) {
			cout << "Area of current gasket is: " << gasket_area << "mm^2" << endl;
			cout << "Area is outside of tolerance." << endl;
		}
		else {
			cout << "Area of current gasket is: " << gasket_area << "mm^2" << endl;
			cout << "Area is correct." << endl;
		}
		if (!diameter_control) {
			cout << "Hole diameter(s) is outside of tolerance." << endl;
			for (int i = 0; i < hole_diameter.size(); i++) {
				cout << "Diameter[" << i << "] = " << hole_diameter[i] << endl;
			}
		}
		else {
			cout << "Hole diameters are correct." << endl;
		}
		if (!pos_control) {
			cout << "Hole position(s) is outside of tolerance" << endl;
			for (int i = 1; i < holes_pos.size(); i++) {
				cout << "Position[" << i << "] = " << holes_pos[i] << endl;
			}
		}
		else {
			cout << "Hole positions are correct" << endl;
		}

		switch (type) {
		case 0:
			type0_counter++;
			cout << "Unknown object detected" << endl;
			break;
		case 1:
			type1_counter++;
			if (!dim_control || !area_control || !diameter_control || !pos_control) {
				type1_rej++;
			}
			break;
		case 2:
			type2_counter++;
			if (!dim_control || !area_control || !diameter_control || !pos_control) {
				type2_rej++;
			}
			break;
		case 3:
			type3_counter++;
			if (!dim_control || !area_control || !diameter_control || !pos_control) {
				type3_rej++;
			}
			break;
		case 4:
			cout << "No object detected." << endl;
		}

		if (dim_control && area_control && diameter_control && pos_control) {
			cout << "No abnormalities in Area, Diameter or Hole Positions detected." << endl;
		}
		
		/* Time measurements */
		auto cycleEnd = high_resolution_clock::now();
		auto cycleDuration = duration_cast<milliseconds>(cycleEnd - cycleStart);
		cout << "Cycle time is: " << cycleDuration.count() << "ms." << endl;
		
		/*  Counters  */
		gasket_counter = type1_counter + type2_counter + type3_counter + type0_counter;
		cout << "Number of type 1 gasket encountered: " << type1_counter << ", of which rejected: " << type1_rej << endl;
		cout << "Number of type 2 gasket encountered: " << type2_counter << ", of which rejected: " << type2_rej << endl;
		cout << "Number of type 3 gasket encountered: " << type3_counter << ", of which rejected: " << type3_rej << endl;
		cout << "Number of type 0 gasket encountered: " << type0_counter << endl << endl;

		/* Turn off button */
		if (_kbhit()) {
			key = _getch();
			if (key == 'e' || key == 'E') {
				resetButton = true;
				cout << "Reset key has been pressed; Resetting system...";
				auto programEnd = high_resolution_clock::now();
				auto runTime = duration_cast<seconds>(programEnd - programStart);
				cout << "Total runtime is: " << runTime.count() << "s" << endl;
				cout << "Total amount of gaskets inspected: " << gasket_counter << endl;
				break;
			}
		}
		waitKey(1);

		/* Time between cycles */
		this_thread::sleep_for(chrono::milliseconds(5000));
	}
}