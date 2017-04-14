
// DesignProjectDoc.h : interface of the CDesignProjectDoc class
//


#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "Communication.h"
#include "EyeTrackEnv.h"
#include "graphics\GrImage.h"

#include <thread>

enum MovementType {
	right,
	left,
	up,
	down
};

class CDesignProjectDoc : public CDocument
{
protected: // create from serialization only
	CDesignProjectDoc();
	DECLARE_DYNCREATE(CDesignProjectDoc)

// Attributes
public:
	/// Get image
	CGrImage GetImage() { return mImage; }
	/// Get robot image
	CGrImage GetRImage() { return mRobotImage; }
	/// Start the camera feed
	void StartCameraFeedThread();
	void CameraDisconnect();
	bool CameraConnected() const { return mRobotRecieveConnected; }
	SOCKET CameraSock() const { return mUdpRobotRecieveSock; }
	sockaddr_in CamerasockIn() const { return mUdp_Robot_remote; }

	/// test command
	std::string TestCommand() const { return mTestCommand; }

	/// Last command send
	std::string LastCommandSent() const { return mLastCommand; }

	/// Communication enabled
	bool CommunicationEnabled() const { return mCommunicationEnabled; }
	bool CommandsEnabled() const { return mCommandsEnabled; }

	/// Is tracking
	bool IsTracking() const { return mTrackEnv.IsTracking(); }

	/// Threads active
	int ThreadsActive() const { return mThreadsActive; }

	/// Get eye tracking enviornment
	CEyeTrackEnv* GetEyeTrackEnv() { return &mTrackEnv; }

	///Percentages
	double PercentUp() const { return mPercentUp; }
	double PercentDown() const { return mPercentDown; }
	double PercentRight() const { return mPercentRight; }
	double PercentLeft() const { return mPercentLeft; }
	int NumUp() const { return mTotalUp; }
	int NumDown() const { return mTotalDown; }
	int NumLeft() const { return mTotalLeft; }
	int NumRight() const { return mTotalRight; }
	float LastXFval() const { return mLastXFval; }
	float LastYFval() const { return mLastYFval; }

	/// Number of images recieved
	int NumberOfImagesRecieved() const { return mImagesRecieved; }

	/// Process deriviteves to send commands
	bool ProcessXDerivitive(int dx, int xBias);
	bool ProcessYDerivitive(int dy, int yBias);
	int mProcessDissable;

private:
	/// Communication interface
	CCommunication mCommunication;
	int mTotalUp, mTotalDown, mTotalRight, mTotalLeft;
	double mPercentUp;
	double mPercentDown;
	double mPercentRight;
	double mPercentLeft;
	/// Images recieved from the robot
	int mImagesRecieved;
	/// Test command
	std::string mTestCommand;
	/// Last command
	std::string mLastCommand;
	CString mCSTRLastCommand;
	/// Commands
	CString mUpCommand;
	CString mDownCommand;
	CString mRightCommand;
	CString mLeftCommand;
	/// Communication enabled
	bool mCommunicationEnabled;
	bool mCommandsEnabled;
	/// Eye tracking enviornment
	CEyeTrackEnv mTrackEnv;
	/// Eye track filter thresholds
	int mEyeTrackXPosThreshold;
	int mEyeTrackXNegThreshold;
	int mEyeTrackYPosThreshold;
	int mEyeTrackYNegThreshold;
	/// X Bias
	double mEyeTrackXPosBias;
	double mEyeTrackXNegBias;
	/// Y Bias
	double mEyeTrackYPosBias;
	double mEyeTrackYNegBias;
	/// Constants
	double mEyeTrackXConst;
	double mEyeTrackYConst;
	/// Graph steps
	int mEyeTrackGraphDRange;
	int mEyeTrackGraphBRange;
	/// Display
	float mLastXFval;
	float mLastYFval;
	/// Eye tracking thread
	std::thread mEyeTrackingThread;
	/// Camera feed thread
	std::thread mCameraFeedThread;
	/// Image for manipulation
	CGrImage mImage;
	/// Image from robot
	CGrImage mRobotImage;

	/// UDP recieve socket
	SOCKET mUdpRobotRecieveSock;
	/// UDP sockaddr in
	sockaddr_in mUdp_Robot_remote;
	/// robot recieve connected
	bool mRobotRecieveConnected;
	/// Robot recieve port
	int mRobotReviecePort;
	/// Number of threads active
	int mThreadsActive;

// Communication Operations
public:
	/// Get communication
	CCommunication* GetCommunication() { return &mCommunication; }
	
	/// Get the port number
	int GetPort() const { return mCommunication.GetPort(); }
	int GetRPort() const { return mRobotReviecePort; }

	/// Connected?
	bool IsConnected() const { return mCommunication.Connected(); }

	/// Send command
	bool SendCommand(std::string command, bool exit = false);

	/// Send camera movement
	void SendCameraMovement(MovementType movement, bool dOverride=false);

	/// Ended Eye Tracking
	void EndedEyeTracking();

	/// Display classifier mat frame
	void DisplayCascadeMatFrame(cv::Mat&);

	/// Display robot image frame
	void DisplayRobotImageFrame(cv::Mat&);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CDesignProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnCommunicationSetup();
	afx_msg void OnCommunicationConnect();
	afx_msg void OnEyetrackingStartclassifications();
	afx_msg void OnEyetrackingStopclassifications();
	afx_msg void OnCommunicationSendtestcommand();
	afx_msg void OnUpdateCommunicationEnable(CCmdUI *pCmdUI);
	afx_msg void OnCommunicationEnable();
	afx_msg void OnCameraRight();
	afx_msg void OnCameraLeft();
	afx_msg void OnCameraUp();
	afx_msg void OnCameraDown();
	afx_msg void OnCommunicationDisconnect();
	afx_msg void OnCommunicationConnectcamera();
	afx_msg void OnCommunicationDisconnectcamera();
	afx_msg void OnEyetrackingClear();
	afx_msg void OnCommunicationClear();
	afx_msg void OnFileClear();
	afx_msg void OnMovementSetup();
	afx_msg void OnEyetrackingSetup();
};


void EyeTrackingThreadedCall(CEyeTrackEnv* env);
void CameraVideoFeedCall(CDesignProjectDoc* doc);

/* Mat message type. */
typedef struct Mat_MSG_type
{
	cv::Mat mat;
}Mat_MSG;