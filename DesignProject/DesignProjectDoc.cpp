
// DesignProjectDoc.cpp : implementation of the CDesignProjectDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DesignProject.h"
#endif

#include "DesignProjectDoc.h"
#include "CommunicationDlg.h"
#include "CommandDlg.h"
#include "EyeTrackFilterSetupDlg.h"

#include <propkey.h>
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDesignProjectDoc

IMPLEMENT_DYNCREATE(CDesignProjectDoc, CDocument)

BEGIN_MESSAGE_MAP(CDesignProjectDoc, CDocument)
	ON_COMMAND(ID_COMMUNICATION_SETUP, &CDesignProjectDoc::OnCommunicationSetup)
	ON_COMMAND(ID_COMMUNICATION_CONNECT, &CDesignProjectDoc::OnCommunicationConnect)
	ON_COMMAND(ID_EYETRACKING_STARTCLASSIFICATIONS, &CDesignProjectDoc::OnEyetrackingStartclassifications)
	ON_COMMAND(ID_EYETRACKING_STOPCLASSIFICATIONS, &CDesignProjectDoc::OnEyetrackingStopclassifications)
	ON_COMMAND(ID_COMMUNICATION_SENDTESTCOMMAND, &CDesignProjectDoc::OnCommunicationSendtestcommand)
	ON_UPDATE_COMMAND_UI(ID_COMMUNICATION_ENABLE, &CDesignProjectDoc::OnUpdateCommunicationEnable)
	ON_COMMAND(ID_COMMUNICATION_ENABLE, &CDesignProjectDoc::OnCommunicationEnable)
	ON_COMMAND(ID_ROBOT_FORWARD, &CDesignProjectDoc::OnCameraRight)
	ON_COMMAND(ID_MOVEMENT_LEFT, &CDesignProjectDoc::OnCameraLeft)
	ON_COMMAND(ID_MOVEMENT_UP, &CDesignProjectDoc::OnCameraUp)
	ON_COMMAND(ID_MOVEMENT_DOWN, &CDesignProjectDoc::OnCameraDown)
	ON_COMMAND(ID_COMMUNICATION_DISCONNECT, &CDesignProjectDoc::OnCommunicationDisconnect)
	ON_COMMAND(ID_COMMUNICATION_CONNECTCAMERA, &CDesignProjectDoc::OnCommunicationConnectcamera)
	ON_COMMAND(ID_COMMUNICATION_DISCONNECTCAMERA, &CDesignProjectDoc::OnCommunicationDisconnectcamera)
	ON_COMMAND(ID_EYETRACKING_CLEAR, &CDesignProjectDoc::OnEyetrackingClear)
	ON_COMMAND(ID_COMMUNICATION_CLEAR, &CDesignProjectDoc::OnCommunicationClear)
	ON_COMMAND(ID_FILE_CLEAR, &CDesignProjectDoc::OnFileClear)
	ON_COMMAND(ID_MOVEMENT_SETUP, &CDesignProjectDoc::OnMovementSetup)
	ON_COMMAND(ID_EYETRACKING_SETUP, &CDesignProjectDoc::OnEyetrackingSetup)
END_MESSAGE_MAP()


// CDesignProjectDoc construction/destruction

CDesignProjectDoc::CDesignProjectDoc()
{
	mThreadsActive = 1;
	mTrackEnv.SetDocument(this); // Only call once!

	mImage.SetSize(740, 480);
	mImage.Fill(0, 0, 0);

	mRobotImage.SetSize(740, 480);
	mRobotImage.Fill(0, 0, 0);

	mTestCommand = TEST_COMMAND;
	mCommunicationEnabled = true;
	mCommandsEnabled = true;
	mProcessDissable = 0;
	mTotalUp = 0; mTotalDown = 0; mTotalRight = 0; mTotalLeft = 0;
	mPercentUp = 0; mPercentDown = 0; mPercentRight = 0; mPercentLeft = 0;
	mUpCommand = L"u";
	mDownCommand = L"d";
	mRightCommand = L"r";
	mLeftCommand = L"l";

	mRobotReviecePort = UDPROBOTRECIEVEPORTNUMBER;
	mImagesRecieved = 0;
	StartCameraFeedThread();

	// Eye track init
	mEyeTrackXPosThreshold = 14;
	mEyeTrackYPosThreshold = 14;
	mEyeTrackXNegThreshold = -14;
	mEyeTrackYNegThreshold = -14;
	mEyeTrackXPosBias = .6; // Right
	mEyeTrackXNegBias = .6; // Left
	mEyeTrackYPosBias = .8; // Up
	mEyeTrackYNegBias = .2; // Down
	mEyeTrackXConst = 0;
	mEyeTrackYConst = 1;
	mEyeTrackGraphDRange = 40;
	mEyeTrackGraphBRange = 10;
	mLastXFval = 0;
	mLastYFval = 0;
}

CDesignProjectDoc::~CDesignProjectDoc()
{
	if (mTrackEnv.IsTracking())
	{
		mTrackEnv.StopTracking();
	}
	mRobotRecieveConnected = false;
	closesocket(mUdpRobotRecieveSock);
}

BOOL CDesignProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CDesignProjectDoc serialization

void CDesignProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CDesignProjectDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CDesignProjectDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDesignProjectDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDesignProjectDoc diagnostics

#ifdef _DEBUG
void CDesignProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDesignProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDesignProjectDoc commands

/*******************************************************************
 * Communication
 *******************************************************************/

/**
 * On communication setup.
 */
void CDesignProjectDoc::OnCommunicationSetup()
{
	CCommunicationDlg dlg;

	dlg.testCommand = CString(mTestCommand.c_str());
	dlg.udpPort = GetPort();
	dlg.cameraInPort = GetRPort();

	if (dlg.DoModal() == IDOK)
	{
		CT2CA convC(dlg.testCommand);
		mTestCommand = string(convC);
		mCommunication.SetPort(dlg.udpPort);
		mRobotReviecePort = dlg.cameraInPort;

		UpdateAllViews(NULL);
	}
}


/**
 * Connect to the server to start sending commands via udp.
 */
void CDesignProjectDoc::OnCommunicationConnect()
{
	mCommunication.InitializeSockData();
	UpdateAllViews(NULL);
}


/**
 * Disconnect and exit the server.
 * ENDS THE SERVER APPLICATION.
 */
void CDesignProjectDoc::OnCommunicationDisconnect()
{
	mCommunication.Disconnect();
	UpdateAllViews(NULL);
}


/**
 * Send a test command.
 */
void CDesignProjectDoc::OnCommunicationSendtestcommand()
{
	if (IsConnected())
		SendCommand(mTestCommand);
}


/**
 * Enabled checked UI update.
 */
void CDesignProjectDoc::OnUpdateCommunicationEnable(CCmdUI *pCmdUI)
{
	if (mCommunicationEnabled)
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}


/**
 * On communication enable click handle.
 */
void CDesignProjectDoc::OnCommunicationEnable()
{
	mCommunicationEnabled = !mCommunicationEnabled;
	UpdateAllViews(NULL);
}


/**
 * Send movement.
 * Given a step range, repeat calls.
 */
void CDesignProjectDoc::SendCameraMovement(MovementType movement, bool delayOverride)
{
	if (mProcessDissable == 5) mProcessDissable = 0;
	// If delay override, short circuit and process dissable does not incroment...
	//if (!delayOverride && mProcessDissable++ != 0) return;

	CString command = L"";
	switch (movement) {
	case (MovementType::right):
		command += mRightCommand;
		mTotalRight++;
		break;
	case (MovementType::left):
		command += mLeftCommand;
		mTotalLeft++;
		break;
	case (MovementType::up):
		command += mUpCommand;
		mTotalUp++;
		break;
	case (MovementType::down):
		command += mDownCommand;
		mTotalDown++;
		break;
	}

	double commands = mCommunication.CommandsSent() + 1;
	if (commands != 0) {
		mPercentRight = (mTotalRight / (double)commands)*100.;
		mPercentLeft = (mTotalLeft / (double)commands)*100.;
		mPercentUp = (mTotalUp / (double)commands)*100.;
		mPercentDown = (mTotalDown / (double)commands)*100.;
	}
	CString temp = mCSTRLastCommand;
	mCSTRLastCommand = command;
	// If the command is opposite, return as we are recentering the eyes
	if ((temp == mRightCommand && command == mLeftCommand) ||
		(temp == mUpCommand && command == mDownCommand)) return;
	CT2CA c(command);
	string toSend(c);
	SendCommand(toSend);
}


/**
 * Send command.
 * \param exit: Set command_msg.exit -> exits robot server.
 */
bool CDesignProjectDoc::SendCommand(string command, bool exit)
{
	bool valid = false;
	if (mCommunicationEnabled)
		valid = mCommunication.SendCommand(command, exit);
	if (valid) mLastCommand = command;
	UpdateAllViews(NULL);
	return valid;
}


/*************************************************************
 * Move camera commands menu options
 *************************************************************/

/**
 * Move camera right menu.
 */
void CDesignProjectDoc::OnCameraRight()
{
	SendCameraMovement(MovementType::right, true);
}


/**
 * Move camera left menu.
 */
void CDesignProjectDoc::OnCameraLeft()
{
	SendCameraMovement(MovementType::left, true);
}


/**
 * Move camera up menu.
 */
void CDesignProjectDoc::OnCameraUp()
{
	SendCameraMovement(MovementType::up, true);
}


/**
 * Move camera down menu.
 */
void CDesignProjectDoc::OnCameraDown()
{
	SendCameraMovement(MovementType::down, true);
}


/*************************************************************
 * Eye Tracking
 *************************************************************/

/**
 * Start eye tracking classifications.
 */
void CDesignProjectDoc::OnEyetrackingStartclassifications()
{
	// Start the thread if one doesnt already exist
	if (!mTrackEnv.IsTracking())
	{
		mEyeTrackingThread = thread(EyeTrackingThreadedCall, &mTrackEnv);
		mEyeTrackingThread.detach();
		mThreadsActive++;
	}
	UpdateAllViews(NULL);
}


/**
 * Stop eye tracking clasifications.
 */
void CDesignProjectDoc::OnEyetrackingStopclassifications()
{
	// Stop the tracking and join
	mTrackEnv.StopTracking();
	mThreadsActive--;
}


/**
 * Clear the eye tracking image.
 */
void CDesignProjectDoc::OnEyetrackingClear()
{
	if (!IsTracking()) {
		mImage.Fill(0, 0, 0); // Set to black
		mPercentDown = 0;
		mPercentLeft = 0;
		mPercentRight = 0;
		mPercentUp = 0;
		mTotalDown = 0;
		mTotalLeft = 0;
		mTotalRight = 0;
		mTotalUp = 0;
		mImagesRecieved = 0;
		mTrackEnv.Clear();
		mCommunication.Clear();
		UpdateAllViews(NULL);
	}
}


/**
 * Called from within the enviornment when the tracking goes bad.
 */
void CDesignProjectDoc::EndedEyeTracking()
{
}


/**
 * Display the cascade mat frame.
 */
void CDesignProjectDoc::DisplayCascadeMatFrame(cv::Mat &destination)
{
	cv::Size size(740, 480);//the dst image size,e.g.100x100
	cv::Mat dst;//dst image
	resize(destination, dst, size);//resize image
	imwrite("matFrameTemp.png", dst);
	mImage.LoadFile(L"matFrameTemp.png");

	UpdateAllViews(NULL);
}


/**
 * Threaded eye tracking call.
 */
void EyeTrackingThreadedCall(CEyeTrackEnv* env)
{
	env->StartTracking();
	while (env->IsTracking())
	{
		// Do neat eye tracking mumbo-jumbo
		env->Classify();
		env->GetDocument()->UpdateAllViews(NULL);
	}
}


/**
 * Process X derivitive.
 * \param dx = dx/dt -- over 3 "frame times".
 * \param xBias = pos(x) - pos(center.x) = amount "off-center".
 *		Positive and Negative bias determins how much of this off-center to add in.
 */
bool CDesignProjectDoc::ProcessXDerivitive(int dx, int xBias)
{
	float f = dx + mEyeTrackXConst + (xBias > 0 ? (xBias*mEyeTrackXPosBias) : (xBias*mEyeTrackXNegBias));
	
	// Error correction
	if ((f > mEyeTrackXPosThreshold - 10 || f < mEyeTrackXNegThreshold + 10) &&
		(dx*xBias < 0 || xBias == 0)) {
		f = 0;
	}

	mLastXFval = f;
	
	if (f > mEyeTrackXPosThreshold) {
		SendCameraMovement(MovementType::right); return true;
	}
	else if (f < mEyeTrackXNegThreshold) {
		SendCameraMovement(MovementType::left); return true;
	}
	return false;
}


/**
 * Process Y derivitive.
 * \param dy = dy/dt -- over 3 "frame times".
 * \param yBias = pos(y) - pos(center.y) = amount "off-center".
 *		Positive and Negative bias determins how much of this off-center to add in.
 */
bool CDesignProjectDoc::ProcessYDerivitive(int dy, int yBias)
{
	float f = dy + mEyeTrackYConst + (yBias > 0 ? (yBias*mEyeTrackYPosBias) : (yBias*mEyeTrackYNegBias));
	
	// Error correction
	if ((f > mEyeTrackYPosThreshold - 10 || f < mEyeTrackYNegThreshold + 10) &&
		(dy*yBias < 0 || yBias == 0)) {
		f = 0;
	}

	mLastYFval = f;
	
	if (f > mEyeTrackYPosThreshold) {
		SendCameraMovement(MovementType::up); return true;
	}
	else if (f < mEyeTrackYNegThreshold) {
		SendCameraMovement(MovementType::down); return true;
	}
	return false;
}


/**
 * Eye tracking setup.
 */
void CDesignProjectDoc::OnEyetrackingSetup()
{
	CEyeTrackFilterSetupDlg dlg;

	dlg.xDirectionThreshold = mEyeTrackXPosThreshold;
	dlg.xNegDirectionThreshold = mEyeTrackXNegThreshold;
	dlg.yDirectionThreshold = mEyeTrackYPosThreshold;
	dlg.yNegDirectionThreshold = mEyeTrackYNegThreshold;
	dlg.xPosBias = mEyeTrackXPosBias;
	dlg.xNegBias = mEyeTrackXNegBias;
	dlg.yPosBias = mEyeTrackYPosBias;
	dlg.yNegBias = mEyeTrackYNegBias;
	dlg.yConst = mEyeTrackYConst;
	dlg.xConst = mEyeTrackXConst;
	dlg.derivativeRange = mEyeTrackGraphDRange;
	dlg.biasRange = mEyeTrackGraphBRange;

	if (dlg.DoModal() == IDOK)
	{
		mEyeTrackXPosThreshold = dlg.xDirectionThreshold;
		mEyeTrackYPosThreshold = dlg.yDirectionThreshold;
		mEyeTrackXNegThreshold = dlg.xNegDirectionThreshold;
		mEyeTrackYNegThreshold = dlg.yNegDirectionThreshold;
		mEyeTrackXPosBias = dlg.xPosBias;
		mEyeTrackXNegBias = dlg.xNegBias;
		mEyeTrackYPosBias = dlg.yPosBias;
		mEyeTrackYNegBias = dlg.yNegBias;
		mEyeTrackYConst = dlg.yConst;
		mEyeTrackXConst = dlg.xConst;
		mEyeTrackGraphBRange = dlg.biasRange;
		mEyeTrackGraphDRange = dlg.derivativeRange;

		UpdateAllViews(NULL);
	}
}


/***************************************************************
 * Camera feed from robot.
 ***************************************************************/

/**
 * Start the camera feed thread.
 */
void CDesignProjectDoc::StartCameraFeedThread()
{
	mRobotRecieveConnected = false;
	mUdpRobotRecieveSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	int broadcast = 1;
	if ((setsockopt(mUdpRobotRecieveSock, SOL_SOCKET, SO_BROADCAST,
		(char*)&broadcast, sizeof(broadcast))) == -1) {
		AfxMessageBox(L"Error setting sock opt udp.");
		exit(6);
	}

	memset(&mUdp_Robot_remote, 0, sizeof(mUdp_Robot_remote));
	mUdp_Robot_remote.sin_family = AF_INET;
	mUdp_Robot_remote.sin_port = mRobotReviecePort;
	mUdp_Robot_remote.sin_addr.s_addr = INADDR_ANY;

	mRobotRecieveConnected = true;
	mCameraFeedThread = thread(CameraVideoFeedCall, this);
	mCameraFeedThread.detach();
	mThreadsActive++;
}


/**
 * Camera video feed from robot call.
 */
void CameraVideoFeedCall(CDesignProjectDoc* doc)
{
	Mat_MSG* msg = new Mat_MSG();
	char temp[sizeof(Mat_MSG)];
	sockaddr_in* sockin = &doc->CamerasockIn();
	int slen = sizeof(sockaddr);
	sockaddr_in other;
	SOCKET sock = doc->CameraSock();
	//Mat img(Size(320, 240), CV_8UC3);

	while (doc->CameraConnected() && doc->CommunicationEnabled())
	{
		recvfrom(sock, temp, sizeof(temp) - 1, 0,
			(sockaddr*)&other, &slen);
		//AfxMessageBox((CString)temp);
		memcpy(msg, &temp[0], sizeof(Mat_MSG));
		if (msg != NULL)
			doc->DisplayRobotImageFrame(msg->mat);
	}

	doc->CameraDisconnect();
	doc->UpdateAllViews(NULL);
	//delete msg;
}


/**
 * Camera disconnect.
 */
void CDesignProjectDoc::CameraDisconnect()
{
	mRobotRecieveConnected = false;
	mThreadsActive--;
	closesocket(mUdpRobotRecieveSock);
}


/**
 * Display the robot image frame.
 */
void CDesignProjectDoc::DisplayRobotImageFrame(cv::Mat &destination)
{

	if (destination.rows > 0 && destination.cols > 0) {
		AfxMessageBox(L"Got an image");
		imwrite("robotFrameTemp.png", destination);
		mRobotImage.LoadFile(L"robotFrameTemp.png");
		mImagesRecieved++;
		UpdateAllViews(NULL);
	}
}

void CDesignProjectDoc::OnCommunicationConnectcamera()
{
	if (!CameraConnected()) StartCameraFeedThread();
	UpdateAllViews(NULL);
}


void CDesignProjectDoc::OnCommunicationDisconnectcamera()
{
	mRobotRecieveConnected = false;
}


void CDesignProjectDoc::OnCommunicationClear()
{
	OnEyetrackingClear();
}


void CDesignProjectDoc::OnFileClear()
{
	OnEyetrackingClear();
}


void CDesignProjectDoc::OnMovementSetup()
{
	CCommandDlg dlg;

	dlg.right = mRightCommand;
	dlg.left = mLeftCommand;
	dlg.up = mUpCommand;
	dlg.down = mDownCommand;

	if (dlg.DoModal() == IDOK)
	{
		mDownCommand = dlg.down;
		mUpCommand = dlg.up;
		mRightCommand = dlg.right;
		mLeftCommand = dlg.left;

		UpdateAllViews(NULL);
	}
}
