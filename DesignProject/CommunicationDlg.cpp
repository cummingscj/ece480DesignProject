// CommunicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DesignProject.h"
#include "CommunicationDlg.h"
#include "afxdialogex.h"


// CCommunicationDlg dialog

IMPLEMENT_DYNAMIC(CCommunicationDlg, CDialogEx)

CCommunicationDlg::CCommunicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMDLG, pParent)
	, testCommand(_T(""))
	, udpPort(0)
	, cameraInPort(0)
{

}

CCommunicationDlg::~CCommunicationDlg()
{
}

void CCommunicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, testCommand);
	DDX_Text(pDX, IDC_EDIT4, udpPort);
	DDV_MinMaxInt(pDX, udpPort, 0, 10000);
	DDX_Text(pDX, IDC_EDIT1, cameraInPort);
	DDV_MinMaxInt(pDX, cameraInPort, 0, 10000);
}


BEGIN_MESSAGE_MAP(CCommunicationDlg, CDialogEx)
END_MESSAGE_MAP()


// CCommunicationDlg message handlers
