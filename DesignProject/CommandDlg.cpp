// CommandDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DesignProject.h"
#include "CommandDlg.h"
#include "afxdialogex.h"


// CCommandDlg dialog

IMPLEMENT_DYNAMIC(CCommandDlg, CDialogEx)

CCommandDlg::CCommandDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMANDSETUP, pParent)
	, up(_T(""))
	, down(_T(""))
	, left(_T(""))
	, right(_T(""))
{

}

CCommandDlg::~CCommandDlg()
{
}

void CCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, up);
	DDX_Text(pDX, IDC_EDIT2, down);
	DDX_Text(pDX, IDC_EDIT3, left);
	DDX_Text(pDX, IDC_EDIT4, right);
}


BEGIN_MESSAGE_MAP(CCommandDlg, CDialogEx)
END_MESSAGE_MAP()


// CCommandDlg message handlers
