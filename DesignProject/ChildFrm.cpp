
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "DesignProject.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}


void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Add your specialized code here and/or call the base class
	if (nCmdShow == -1)
	{
		// Show the window in maximized mode 
		// if and only if it is the very first frame
		// Otherwise, do default for user conveniences
		// The user may prefer to keep a previously created view and 
		// frame in restored (not maximized) mode
		// So, it is advisable to keep the newly created view and frame 
		// in the same state as already created views
		if (GetNextWindow() == NULL && GetNextWindow(GW_HWNDPREV) == NULL)
		{
			nCmdShow = SW_SHOWMAXIMIZED;
		}
	}
	CMDIChildWnd::ActivateFrame(nCmdShow);
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	// Create a window without min/max buttons or sizable border 
	//cs.style = WS_OVERLAPPED | WS_SYSMENU | WS_BORDER;

	// Size the window to 1/3 screen size and center it 
	cs.cy = ::GetSystemMetrics(SM_CYSCREEN) / 3;
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN) / 3;
	//cs.y = ((cs.cy * 3) - cs.cy) / 2;
	//cs.x = ((cs.cx * 3) - cs.cx) / 2;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers
