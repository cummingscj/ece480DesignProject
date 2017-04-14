#pragma once


// CEyeTrackFilterSetupDlg dialog

class CEyeTrackFilterSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEyeTrackFilterSetupDlg)

public:
	CEyeTrackFilterSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEyeTrackFilterSetupDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EYETRACKFILTERSETUPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// x direction filter
	int xDirectionThreshold;
	// y direction filter
	int yDirectionThreshold;
	// x positive bias %
	double xPosBias;
	// x negative bias %
	double xNegBias;
	// y positive bias %
	double yPosBias;
	// y negative bias %
	double yNegBias;
	// negative x threshold
	int xNegDirectionThreshold;
	// y negative threshold
	int yNegDirectionThreshold;
	// x constant
	double xConst;
	// y constant
	double yConst;
	// Derivative range for graph
	int derivativeRange;
	// Bias graph range
	int biasRange;
	afx_msg void OnBnClickedGraphfilterbutton();
private:
	// Make the filter input file
	void MakeFilterInput();
};
