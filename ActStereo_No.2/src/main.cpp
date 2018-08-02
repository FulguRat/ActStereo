#include <iostream>
#include <opencv2/opencv.hpp>
#include "shuttlecock_recognizer.h"
#include "timer.h"

using namespace std;
using namespace cv;

int keyStatus;
Timer periodTimer;

ConnectedComponent shuttlecockL;
ConnectedComponent shuttlecockR;

bool isPointsFound = false;

Mat tmpLeft;
Mat tmpRight;

Vec4d testVec;
//
//const int g_xTrackbarMax = 640;
//const int g_yTrackbarMax = 480;
//int g_xLeftSlider = 300;
//int g_yLeftSlider = 226;
//int g_xRightSlider = 320;
//int g_yRightSlider = 226;
//void trackbarCallback(int, void*) {}

int main(int argc, char* argv[])
{
	ShuttleRecognizer actStereoNo_1("CameraLeft", CAMERA_ARGS_LEFT, "CameraRight", CAMERA_ARGS_RIGHT);

	actStereoNo_1.stereoInit();

	//cv::namedWindow("TKB");
	//createTrackbar("xLeft", "TKB", &g_xLeftSlider, g_xTrackbarMax, trackbarCallback);
	//createTrackbar("yLeft", "TKB", &g_yLeftSlider, g_yTrackbarMax, trackbarCallback);
	//createTrackbar("xRight", "TKB", &g_xRightSlider, g_xTrackbarMax, trackbarCallback);
	//createTrackbar("yRight", "TKB", &g_yRightSlider, g_yTrackbarMax, trackbarCallback);

	while (!actStereoNo_1.m_bExit)
	{
		actStereoNo_1.syncUpdate();

		//testVec = actStereoNo_1.simulatedLocating(g_yLeftSlider, g_xLeftSlider, g_yRightSlider, g_xRightSlider);
		//cout << "x = " << testVec[0] << "\ty = " << testVec[1] << "\tz = " << testVec[2] << "\td = " << testVec[3] << endl;

		actStereoNo_1.backgroundSubtract();

		actStereoNo_1.preProcessing();
		isPointsFound = actStereoNo_1.findMatchedPoints(actStereoNo_1.getDstLeft(), shuttlecockL, actStereoNo_1.getDstRight(), shuttlecockR);

		cvtColor(actStereoNo_1.getDstLeft(), tmpLeft, CV_GRAY2BGR);
		cvtColor(actStereoNo_1.getDstRight(), tmpRight, CV_GRAY2BGR);

		if (isPointsFound == true)
		{
			//cout << "yes" << endl;
			isPointsFound = false;

			circle(tmpLeft, shuttlecockL.core, 3, Scalar(0, 0, 255), 2);
			circle(tmpRight, shuttlecockR.core, 3, Scalar(0, 0, 255), 2);

			testVec = actStereoNo_1.simulatedLocating(shuttlecockL.core.y, shuttlecockL.core.x, shuttlecockR.core.y, shuttlecockR.core.x);
			cout << "x = " << testVec[0] << "\ty = " << testVec[1] << "\tz = " << testVec[2] << "\td = " << testVec[3] << endl;
		}

		shuttlecockL.size = 0;
		shuttlecockR.size = 0;
		shuttlecockL.core = Point(-1, -1);
		shuttlecockR.core = Point(-1, -1);

		imshow("Left", tmpLeft);
		imshow("Right", tmpRight);

		//cout << periodTimer.end() << endl;
		//periodTimer.begin();

		//if push down Esc, kill the progress
		keyStatus = waitKey(1);
		if (keyStatus == 0x1B)
		{
			break;
		}
	}

	return 0;
}