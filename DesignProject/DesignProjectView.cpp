
// DesignProjectView.cpp : implementation of the CDesignProjectView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DesignProject.h"
#endif

#include "DesignProjectDoc.h"
#include "DesignProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDesignProjectView

IMPLEMENT_DYNCREATE(CDesignProjectView, CView)

BEGIN_MESSAGE_MAP(CDesignProjectView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDesignProjectView construction/destruction

CDesignProjectView::CDesignProjectView()
{
	// TODO: add construction code here

}

CDesignProjectView::~CDesignProjectView()
{
}

BOOL CDesignProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDesignProjectView drawing

void CDesignProjectView::OnDraw(CDC* /*pDC*/)
{
	CDesignProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDesignProjectView printing

BOOL CDesignProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDesignProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDesignProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CDesignProjectView diagnostics

#ifdef _DEBUG
void CDesignProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CDesignProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDesignProjectDoc* CDesignProjectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDesignProjectDoc)));
	return (CDesignProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CDesignProjectView message handlers
