#pragma once

#include <string>

// CCommunicationDlg dialog

class CCommunicationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCommunicationDlg)

public:
	CCommunicationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommunicationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Test command for the robot server
	CString testCommand;
	// UDP port number
	int udpPort;
	// Camera input frames port number
	int cameraInPort;
};
