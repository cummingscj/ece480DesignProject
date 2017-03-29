
// DesignProjectView.h : interface of the CDesignProjectView class
//

#pragma once


class CDesignProjectView : public CView
{
protected: // create from serialization only
	CDesignProjectView();
	DECLARE_DYNCREATE(CDesignProjectView)

// Attributes
public:
	CDesignProjectDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDesignProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DesignProjectView.cpp
inline CDesignProjectDoc* CDesignProjectView::GetDocument() const
   { return reinterpret_cast<CDesignProjectDoc*>(m_pDocument); }
#endif

