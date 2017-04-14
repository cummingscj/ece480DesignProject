#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "graphics\GrImage.h"

#include <vector>
#include <ctime>
#include <math.h>

#include "constants.h"
#include "findEyeCenter.h"
#include "findEyeCorner.h"

#define BASELINE_TL 10

class CDesignProjectDoc;

class CEyeTrackEnv
{
public:
	CEyeTrackEnv();
	~CEyeTrackEnv();

	/// Tracking eyes?
	bool IsTracking() const { return this->mTracking; }

	/// Start tracking
	void StartTracking();

	/// In baseline mode?
	bool EstablishingBaseline() const { return mBaselineTime < mEstablishedIn; }

	/// Classify
	void Classify();

	/// Apply filter and display
	void ApplyFilterAndDisplay();

	/// Find eyes method
	void FindEyes(cv::Mat frame_grey, cv::Rect face);

	/// Process points and error correction
	void ProcessPointsCorrect(cv::Mat&, cv::Point, cv::Point, cv::Rect, cv::Rect);
	void ShiftLeftHist(cv::Point left);
	void ShiftRightHist(cv::Point right);

	/// Stop tracking
	void StopTracking();

	/// Clear metrics and such
	void Clear();

	/// Set document
	void SetDocument(CDesignProjectDoc* doc) {
		this->mDocument = doc;
	}

	/// Get document
	CDesignProjectDoc* GetDocument() const { return mDocument; }

	/// Get metrics
	int TotalClassificationCalls() const { return mClassificationCalls; }
	int EmptyFrames() const { return mEmptyFrames; }
	int TotalFacesClassified() const { return mTotalFacesClassified; }
	int TotalEyesClassified() const { return mTotalEyesClassified; }
	double ClassificationsPerSec() const { return mClassificationsPerSec; }

private:
	/// Tracking
	bool mTracking;
	/// Classifications
	int mClassificationCalls;
	int mEmptyFrames;
	int mTotalFacesClassified;
	int mTotalEyesClassified;
	/// Start time in miliseconds
	std::clock_t mStartTime;
	double mClassificationsPerSec;
	/// The document
	CDesignProjectDoc *mDocument;
	/// Video capture
	cv::VideoCapture mCapture;
	/// Frame matrix
	cv::Mat mFrame;
	/// Cascade name
	cv::String mFaceCascadeXML;
	/// Classifier
	cv::CascadeClassifier mClassifier;
	/// RNG
	cv::RNG mRng;
	/// Debug image
	cv::Mat mDebugImage;
	/// Skin hist
	cv::Mat mSkinCrCbHist;
	/// Left Eye History
	//std::vector<cv::Point> mLeftHist;
	cv::Point mLeftHist[3];
	/// Right Eye History
	//std::vector<cv::Point> mRightHist;
	cv::Point mRightHist[3];
	/// Int baseline reading
	int mBaselineTime;
	/// Int established in
	int mEstablishedIn;
};