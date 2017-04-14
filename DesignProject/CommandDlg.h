#pragma once


// CCommandDlg dialog

class CCommandDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCommandDlg)

public:
	CCommandDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommandDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMANDSETUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Up command
	CString up;
	// Down command
	CString down;
	// Left command
	CString left;
	// Right command
	CString right;
};
