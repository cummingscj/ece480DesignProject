
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

using namespace std;

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
	mStatusXRightInit = 0;
	mMetricsXLeftInit = 0;
}

CDesignProjectView::~CDesignProjectView()
{
}

BOOL CDesignProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs)) return FALSE;
	return TRUE;
}

// CDesignProjectView drawing

void CDesignProjectView::OnDraw(CDC* pDC)
{
	CDesignProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Draw any background we need around the image
	CRect imgRect;
	GetClientRect(&imgRect);
	BYTE bgGray = 128;
	CBrush bgBrush(RGB(bgGray, bgGray, bgGray));
	pDC->FillRect(imgRect, &bgBrush);

	// Get the images to draw
	const CGrImage &image = pDoc->GetImage();
	const CGrImage &rImage = pDoc->GetRImage();

	CSize imgSize(image.GetWidth(), image.GetHeight()); // Left side
	CSize rImgSize(rImage.GetWidth(), rImage.GetHeight()); // Right side

	image.Draw(pDC, 80, 20);
	rImage.Draw(pDC, imgRect.Width() - rImage.GetWidth() - 80, 20);

	// Get dims for metrics view and status view
	CBrush whiteB(RGB(255, 255, 255));

	// Status Rect
	if (mStatusXRightInit == 0) mStatusXRightInit = imgSize.cx + 120;
	CRect statusRect(imgRect);
	statusRect.top = imgSize.cy + 40;
	statusRect.right = mStatusXRightInit;
	statusRect.left = 20;
	statusRect.bottom -= 20;
	pDC->FillRect(statusRect, &whiteB);

	// Metrics Rect
	if (mMetricsXLeftInit == 0) mMetricsXLeftInit = imgRect.right - rImgSize.cx - 120;
	CRect metricsRect(imgRect);
	metricsRect.top = statusRect.top;
	metricsRect.bottom = statusRect.bottom;
	metricsRect.left = mMetricsXLeftInit;
	metricsRect.right = imgRect.right - 20;
	pDC->FillRect(metricsRect, &whiteB);

	// Status displays
	UpdateStatusDisplays();
	int count = 0;
	bool first = true;
	int statStartY = statusRect.top + 20;
	int statStartX = statusRect.left + 20;
	pDC->TextOutW((statusRect.right - statusRect.left) / 2. - 50, statStartY, L"Program Status");
	for (auto p : mStatusV)
	{
		if (count > 9 && first) {
			statStartX += 400; statStartY = statusRect.top + 20; first = false;
		}
		statStartY += 30;
		pDC->TextOutW(statStartX, statStartY, p.first);
		pDC->TextOutW(statStartX + p.first.GetLength()*10 + 20, statStartY, p.second);
		count++;
	}

	// Metrics displays
	UpdateMetricsDisplays();
	count = 0;
	first = true;
	statStartY = metricsRect.top + 20;
	statStartX = metricsRect.left + 20;
	pDC->TextOutW(metricsRect.left + (metricsRect.right - metricsRect.left) / 2. - 50, statStartY,
		L"Program Metrics");
	for (auto p : mMetricsV)
	{
		if (count > 9 && first) {
			statStartX += 450; statStartY = metricsRect.top + 20; first = false;
		}
		statStartY += 30;
		pDC->TextOutW(statStartX, statStartY, p.first);
		pDC->TextOutW(statStartX + p.first.GetLength() * 9 + 20, statStartY, p.second);
		count++;
	}
}


void CDesignProjectView::UpdateStatusDisplays()
{
	mStatusV.clear();

	CString enabled = GetDocument()->CommunicationEnabled() ? L"True" : L"False";
	CString cEnabled = GetDocument()->CommandsEnabled() ? L"True" : L"False";
	CString connected = GetDocument()->IsConnected() ? L"True" : L"False";
	CString port(to_string(GetDocument()->GetPort()).c_str());
	CString rConnected = GetDocument()->CameraConnected() ? L"True" : L"False";
	CString rPort(to_string(GetDocument()->GetRPort()).c_str());
	CString testC(GetDocument()->TestCommand().c_str());
	CString lastC(GetDocument()->LastCommandSent().c_str());
	CString tracking = GetDocument()->IsTracking() ? L"True" : L"False";
	CString threads(to_string(GetDocument()->ThreadsActive()).c_str());
	mStatusV.push_back(make_pair(L"Communication Enabled    ", enabled));
	mStatusV.push_back(make_pair(L"Commands Enabled         ", cEnabled));
	mStatusV.push_back(make_pair(L"Robot Commands Connected:", connected));
	mStatusV.push_back(make_pair(L"Robot Commands Port:     ", port));
	mStatusV.push_back(make_pair(L"", L""));
	mStatusV.push_back(make_pair(L"Robot Camera Listening:  ", rConnected));
	mStatusV.push_back(make_pair(L"Robot Camera Port:       ", rPort));
	mStatusV.push_back(make_pair(L"", L""));
	mStatusV.push_back(make_pair(L"Last Command Sent        ", lastC));
	mStatusV.push_back(make_pair(L"Test Command             ", testC));
	mStatusV.push_back(make_pair(L"Tracking Eyes            ", tracking));
	mStatusV.push_back(make_pair(L"", L""));
	mStatusV.push_back(make_pair(L"Threads Active           ", threads));
}

void CDesignProjectView::UpdateMetricsDisplays()
{
	mMetricsV.clear();

	int classificationAttempts = GetDocument()->GetEyeTrackEnv()->TotalClassificationCalls();
	int faceClassifications = GetDocument()->GetEyeTrackEnv()->TotalFacesClassified();
	int errorsClassifying = classificationAttempts - faceClassifications;

	CString totClassify(to_string(classificationAttempts).c_str());
	CString totError(to_string(errorsClassifying).c_str());
	CString totFaceClassify(to_string(faceClassifications).c_str());
	CString totEyeClassify(to_string(GetDocument()->GetEyeTrackEnv()->TotalEyesClassified()).c_str());
	CString totClassifyPerSec(to_string(GetDocument()->GetEyeTrackEnv()->ClassificationsPerSec()).c_str());
	mMetricsV.push_back(make_pair(L"Total Classification Attempts", totClassify));
	mMetricsV.push_back(make_pair(L"Classification Errors        ", totError));
	mMetricsV.push_back(make_pair(L"Total Face Classifications   ", totFaceClassify));
	mMetricsV.push_back(make_pair(L"Total Eye Classifications    ", totEyeClassify));
	mMetricsV.push_back(make_pair(L"Classifications / Second     ", totClassifyPerSec));
	CString totCommands(to_string(GetDocument()->GetCommunication()->CommandsSent()).c_str());
	CString percUp(to_string(GetDocument()->PercentUp()).c_str());
	CString percDown(to_string(GetDocument()->PercentDown()).c_str());
	CString percRight(to_string(GetDocument()->PercentRight()).c_str());
	CString percLeft(to_string(GetDocument()->PercentLeft()).c_str());
	CString imagesRec(to_string(GetDocument()->NumberOfImagesRecieved()).c_str());
	CString numUp(to_string(GetDocument()->NumUp()).c_str());
	CString numDown(to_string(GetDocument()->NumDown()).c_str());
	CString numRight(to_string(GetDocument()->NumRight()).c_str());
	CString numLeft(to_string(GetDocument()->NumLeft()).c_str());
	CString xFval(to_string(GetDocument()->LastXFval()).c_str());
	CString yFval(to_string(GetDocument()->LastYFval()).c_str());
	mMetricsV.push_back(make_pair(L"", L""));
	mMetricsV.push_back(make_pair(L"Percent Up                   ", percUp));
	mMetricsV.push_back(make_pair(L"Percent Down                 ", percDown));
	mMetricsV.push_back(make_pair(L"Percent Right                ", percRight));
	mMetricsV.push_back(make_pair(L"Percent Left                 ", percLeft));
	mMetricsV.push_back(make_pair(L"Images Recieved From Robot   ", imagesRec));
	mMetricsV.push_back(make_pair(L"", L""));
	mMetricsV.push_back(make_pair(L"Last X Thresh. Val           ", xFval));
	mMetricsV.push_back(make_pair(L"Last Y Thresh. Val           ", yFval));
	mMetricsV.push_back(make_pair(L"", L""));
	mMetricsV.push_back(make_pair(L"Number of Commands Sent      ", totCommands));
	mMetricsV.push_back(make_pair(L"Number of Up                 ", numUp));
	mMetricsV.push_back(make_pair(L"Number of Down               ", numDown));
	mMetricsV.push_back(make_pair(L"Number of Right              ", numRight));
	mMetricsV.push_back(make_pair(L"Number of Left               ", numLeft));
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
	//CView::AssertValid();
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
