#include "stdafx.h"
#include "EyeTrackEnv.h"
#include "DesignProjectDoc.h"

using namespace std;

CEyeTrackEnv::CEyeTrackEnv()
{
	// Set tracking to false
	mTracking = false;
	// Clear all to start
	Clear();
	// Get start classification time
	mStartTime = clock();
	// Setup the cascade filter
	mRng(12345);
	mSkinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);
	mFaceCascadeXML = "haarcascade_frontalface_alt.xml";
	// Initialize baseline
	mBaselineTime = 0;
	mEstablishedIn = BASELINE_TL;
}


CEyeTrackEnv::~CEyeTrackEnv()
{
	//StopTracking();
}


/**
 * Start eye tracking.
 * Called from a thread.
 */
void CEyeTrackEnv::StartTracking()
{
	// Load face cascade
	if (!mClassifier.load(mFaceCascadeXML)) { printf("--(!)Error loading face cascade, please change face_cascade_name in source code.\n"); StopTracking(); return; };
	
	createCornerKernels();
	ellipse(mSkinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2),
		43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);

	// Open the webcam stream
	mCapture.open(0);
	if (!mCapture.isOpened()) { printf("--(!)Error opening video capture\n"); StopTracking(); return; }
	
	// Identify that we are now tracking
	mTracking = true;
	mBaselineTime = 0;
}


/**
 * Classify the capture if tracking.
 * TODO:// Add Metrics
 */
void CEyeTrackEnv::Classify()
{
	if (!mTracking) return;
	if (!mCapture.read(mFrame)) { StopTracking(); return; }

	if (mFrame.empty()) { mEmptyFrames++; return; }// No frame to process

	cv::flip(mFrame, mFrame, 1);

	mClassificationCalls++;
	if (mClassificationCalls % 10 == 0) {
		clock_t newStartTime = clock();
		mClassificationsPerSec = 10. / ((newStartTime - mStartTime) / (double)CLOCKS_PER_SEC);
		mStartTime = newStartTime;
	}

	mFrame.copyTo(mDebugImage);

	// Filter and display
	ApplyFilterAndDisplay();
}


/**
 * Apply the filters and display.
 */
void CEyeTrackEnv::ApplyFilterAndDisplay()
{
	std::vector<cv::Rect> faces;

	std::vector<cv::Mat> rgbChannels(3);
	cv::split(mFrame, rgbChannels);
	cv::Mat frame_gray = rgbChannels[2];

	// Detect faces
	mClassifier.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150));

	for (int i = 0; i < faces.size(); i++)
	{
		mTotalFacesClassified++;
		rectangle(mDebugImage, faces[i], 1234);
	}
	// Find eyes and show in frame
	if (faces.size() > 0) {
		FindEyes(frame_gray, faces[0]);
	}
}


/**
 * Stop eye tracking.
 */
void CEyeTrackEnv::StopTracking()
{
	// No more looping for tracking
	mTracking = false;
	// End video capture
	mCapture.release();
	// End the thread on the document side
	mDocument->EndedEyeTracking();
	// Release the corner cornals
	releaseCornerKernels();
}


/**
 * Clear.
 */
void CEyeTrackEnv::Clear()
{
	mClassificationCalls = 0;
	mEmptyFrames = 0;
	mTotalEyesClassified = 0;
	mTotalFacesClassified = 0;
	mClassificationsPerSec = 0;
}



void CEyeTrackEnv::FindEyes(cv::Mat frame_gray, cv::Rect face) {
	cv::Mat faceROI = frame_gray(face);
	cv::Mat debugFace = faceROI;

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(faceROI, faceROI, cv::Size(0, 0), sigma);
	}
	//-- Find eye regions and draw them
	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
	int eye_region_top = face.height * (kEyePercentTop / 100.0);
	cv::Rect leftEyeRegion(face.width*(kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);
	cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);

	//-- Find Eye Centers
	cv::Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");
	cv::Point rightPupil = findEyeCenter(faceROI, rightEyeRegion, "Right Eye");
	// get corner regions
	cv::Rect leftRightCornerRegion(leftEyeRegion);
	leftRightCornerRegion.width -= leftPupil.x;
	leftRightCornerRegion.x += leftPupil.x;
	leftRightCornerRegion.height /= 2;
	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
	cv::Rect leftLeftCornerRegion(leftEyeRegion);
	leftLeftCornerRegion.width = leftPupil.x;
	leftLeftCornerRegion.height /= 2;
	leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
	cv::Rect rightLeftCornerRegion(rightEyeRegion);
	rightLeftCornerRegion.width = rightPupil.x;
	rightLeftCornerRegion.height /= 2;
	rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
	cv::Rect rightRightCornerRegion(rightEyeRegion);
	rightRightCornerRegion.width -= rightPupil.x;
	rightRightCornerRegion.x += rightPupil.x;
	rightRightCornerRegion.height /= 2;
	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
	//rectangle(debugFace, leftRightCornerRegion, 200);
	//rectangle(debugFace, leftLeftCornerRegion, 200);
	//rectangle(debugFace, rightLeftCornerRegion, 200);
	//rectangle(debugFace, rightRightCornerRegion, 200);
	// change eye centers to face coordinates
	rightPupil.x += rightEyeRegion.x;
	rightPupil.y += rightEyeRegion.y;
	leftPupil.x += leftEyeRegion.x;
	leftPupil.y += leftEyeRegion.y;
	// draw eye centers
	circle(debugFace, rightPupil, 3, 1234);
	circle(debugFace, leftPupil, 3, 1234);

	// Find Eye Corners -- Not utilized
	if (kEnableEyeCorner) {
		cv::Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);
		leftRightCorner.x += leftRightCornerRegion.x;
		leftRightCorner.y += leftRightCornerRegion.y;
		cv::Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
		leftLeftCorner.x += leftLeftCornerRegion.x;
		leftLeftCorner.y += leftLeftCornerRegion.y;
		cv::Point2f rightLeftCorner = findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
		rightLeftCorner.x += rightLeftCornerRegion.x;
		rightLeftCorner.y += rightLeftCornerRegion.y;
		cv::Point2f rightRightCorner = findEyeCorner(faceROI(rightRightCornerRegion), false, false);
		rightRightCorner.x += rightRightCornerRegion.x;
		rightRightCorner.y += rightRightCornerRegion.y;
		circle(faceROI, leftRightCorner, 3, 200);
		circle(faceROI, leftLeftCorner, 3, 200);
		circle(faceROI, rightLeftCorner, 3, 200);
		circle(faceROI, rightRightCorner, 3, 200);
	}

	// Process results
	mTotalEyesClassified++; // Clasified one set of eyes
	// Process and display the resulting mat
	ProcessPointsCorrect(faceROI, leftPupil, rightPupil, leftEyeRegion, rightEyeRegion);
	mBaselineTime++; // Incroment the baseline time for gathering initial data
}


/**
 * Process points and correct for error.
 */
void CEyeTrackEnv::ProcessPointsCorrect(cv::Mat& display, cv::Point left, cv::Point right,
	cv::Rect leftEyeRect, cv::Rect rightEyeRect)
{
	// Display the resulting mat in the Document View window
	mDocument->DisplayCascadeMatFrame(display);

	// Shift rows
	ShiftLeftHist(left);
	ShiftRightHist(right);

	// Process first points as baseline -- Get to steady state
	if (EstablishingBaseline()) return;

	// Baseline data points are in -- take derivitives
	// Right eye derivitive
	cv::Point derivitive(0, 0);
	derivitive += mRightHist[0] * -1;
	derivitive += mRightHist[2] * 1;
	
	// Left eye derivitive
	derivitive += mLeftHist[0] * -1;
	derivitive += mLeftHist[2] * 1;

	// Calculate bias zones
	int xBias = left.x - (leftEyeRect.x + leftEyeRect.width/2);
	xBias += right.x - (rightEyeRect.x + rightEyeRect.width/2);
	xBias /= 2;
	int yBias = left.y - (leftEyeRect.y + leftEyeRect.height/2);
	yBias += right.y - (rightEyeRect.y + rightEyeRect.height/2);
	yBias /= 2;

	// Process calculated derivitives plus bias.. X most probable
	if (!mDocument->ProcessXDerivitive(derivitive.x, xBias))
		mDocument->ProcessYDerivitive(derivitive.y, yBias);
}


/**
 * Shift left rows.
 */
void CEyeTrackEnv::ShiftLeftHist(cv::Point left)
{
	mLeftHist[0] = mLeftHist[1];
	mLeftHist[1] = mLeftHist[2];
	mLeftHist[2] = left;
}


/**
 * Shift right rows.
 */
void CEyeTrackEnv::ShiftRightHist(cv::Point right)
{
	mRightHist[0] = mRightHist[1];
	mRightHist[1] = mRightHist[2];
	mRightHist[2] = right;
}