// EyeTrackFilterSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DesignProject.h"
#include "EyeTrackFilterSetupDlg.h"
#include "afxdialogex.h"

#include <fstream>
#include <string>

using namespace std;

// CEyeTrackFilterSetupDlg dialog

IMPLEMENT_DYNAMIC(CEyeTrackFilterSetupDlg, CDialogEx)

CEyeTrackFilterSetupDlg::CEyeTrackFilterSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EYETRACKFILTERSETUPDLG, pParent)
	, xDirectionThreshold(0)
	, yDirectionThreshold(0)
	, xPosBias(0)
	, xNegBias(0)
	, yPosBias(0)
	, yNegBias(0)
	, xNegDirectionThreshold(0)
	, yNegDirectionThreshold(0)
	, xConst(0)
	, yConst(0)
	, derivativeRange(0)
	, biasRange(0)
{

}

CEyeTrackFilterSetupDlg::~CEyeTrackFilterSetupDlg()
{
}

void CEyeTrackFilterSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, xDirectionThreshold);
	DDX_Text(pDX, IDC_EDIT4, yDirectionThreshold);
	DDX_Text(pDX, IDC_EDIT2, xPosBias);
	DDV_MinMaxDouble(pDX, xPosBias, 0, 1);
	DDX_Text(pDX, IDC_EDIT5, xNegBias);
	DDV_MinMaxDouble(pDX, xNegBias, 0, 1);
	DDX_Text(pDX, IDC_EDIT3, yPosBias);
	DDV_MinMaxDouble(pDX, yPosBias, 0, 1);
	DDX_Text(pDX, IDC_EDIT6, yNegBias);
	DDV_MinMaxDouble(pDX, yNegBias, 0, 1);
	DDX_Text(pDX, IDC_EDIT7, xNegDirectionThreshold);
	DDX_Text(pDX, IDC_EDIT8, yNegDirectionThreshold);
	DDX_Text(pDX, IDC_EDIT9, xConst);
	DDX_Text(pDX, IDC_EDIT10, yConst);
	DDX_Text(pDX, IDC_EDIT12, derivativeRange);
	DDX_Text(pDX, IDC_EDIT11, biasRange);
}


BEGIN_MESSAGE_MAP(CEyeTrackFilterSetupDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GRAPHFILTERBUTTON, &CEyeTrackFilterSetupDlg::OnBnClickedGraphfilterbutton)
END_MESSAGE_MAP()


// CEyeTrackFilterSetupDlg message handlers


void CEyeTrackFilterSetupDlg::OnBnClickedGraphfilterbutton()
{
	MakeFilterInput();

	CString params = L"derivitiveDocProcessAnalysis.py ";
	params.Append(CString(to_string(derivativeRange).c_str()));
	params.Append(L" ");
	params.Append(CString(to_string(biasRange).c_str()));
	params.Append(L" filterInputTemp.fin");

	SHELLEXECUTEINFO shExecInfo;
	shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExecInfo.fMask = NULL;
	shExecInfo.hwnd = NULL;
	shExecInfo.lpVerb = L"runas";
	shExecInfo.lpFile = L"python.exe";
	shExecInfo.lpParameters = params;
	shExecInfo.lpDirectory = NULL;
	shExecInfo.nShow = SW_HIDE;
	shExecInfo.hInstApp = NULL;
	ShellExecuteEx(&shExecInfo);
}


void CEyeTrackFilterSetupDlg::MakeFilterInput()
{
	UpdateData(true);
	ofstream myfile;
	myfile.open("filterInputTemp.fin");
	myfile << "xPT " << xDirectionThreshold << "\n";
	myfile << "xNT " << xNegDirectionThreshold << "\n";
	myfile << "xP " << xPosBias << "\n";
	myfile << "xN " << xNegBias << "\n";
	myfile << "xC " << xConst << "\n";
	myfile << "yPT " << yDirectionThreshold << "\n";
	myfile << "yNT " << yNegDirectionThreshold << "\n";
	myfile << "yP " << yPosBias << "\n";
	myfile << "yN " << yNegBias << "\n";
	myfile << "yC " << yConst << "\n";
	myfile.close();
}
