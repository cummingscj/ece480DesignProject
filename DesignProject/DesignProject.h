
// DesignProject.h : main header file for the DesignProject application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDesignProjectApp:
// See DesignProject.cpp for the implementation of this class
//

class CDesignProjectApp : public CWinApp
{
public:
	CDesignProjectApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDesignProjectApp theApp;
