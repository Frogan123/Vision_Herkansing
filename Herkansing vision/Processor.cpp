#include "Processor.h"

/*
	src2bin converts a colored source image, grayscales it, blurs it, binarize it and enhances the binarized image
	Input:	Mat src			| The source image
			Mat eKernel		| Kernel to use for eroding the binary image
			Mat dKernel		| Kernel to use for dilating the binary image
	Output:	Mat bin			| Returns the binarized image
*/
Mat Processor::src2bin(Mat src, Mat eKernel, Mat dKernel) {
	Mat grey, blur, bin;
	cvtColor(src, grey, COLOR_BGR2GRAY);
	//imshow("gray", grey);
	GaussianBlur(grey, blur, Size(7, 7), 0, 0);
	threshold(blur, bin, 132, 255, THRESH_BINARY);
	dilate(bin, bin, dKernel);
	erode(bin, bin, eKernel);
	return bin;
}

/*
	drawAllContours function draws all contours in a source image
	Input:	Mat src							| Source image
			Mat drawing						| Colorless template to make a drawing on
	Output: vector<vector<Point>> contours	| Returns all drawn contours
*/
vector<vector<Point>> Processor::drawAllContours(Mat src, Mat drawing) {
	RNG rng(12345);
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;
	findContours(src, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);
	for (size_t i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(src, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}
	//imshow("contours", src);
	return contours;
}

/*
	bBox function creates bounding boxes for all given contours
	Input:	vector<vector<Point>> contours	| All contours of an image
			Mat drawing						| Colorless template to make a drawing on
	Output: vector<RotatedRect> minRect		| Returns a vector containing the bounding boxes of all contours
*/
vector<RotatedRect> Processor::bBox(vector<vector<Point>> contours, Mat drawing) {
	RNG rng(12345);
	vector<RotatedRect> minRect(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		minRect[i] = minAreaRect(contours[i]);
		Point2f rectPoints[4];
		minRect[i].points(rectPoints);
		for (int j = 0; j < 4; j++) {
			Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
			line(drawing, rectPoints[j], rectPoints[(j + 1) % 4], color);
		}
	}
	return minRect;
}