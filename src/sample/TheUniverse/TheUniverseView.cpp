
// TheUniverseView.cpp : implementation of the CTheUniverseView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TheUniverse.h"
#endif

#include "TheUniverseDoc.h"
#include "TheUniverseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTheUniverseView

IMPLEMENT_DYNCREATE(CTheUniverseView, CFormView)

BEGIN_MESSAGE_MAP(CTheUniverseView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTheUniverseView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTheUniverseView construction/destruction

CTheUniverseView::CTheUniverseView() noexcept
	: CFormView(IDD_THEUNIVERSE_FORM)
{
	// TODO: add construction code here

}

CTheUniverseView::~CTheUniverseView()
{
}

void CTheUniverseView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CTheUniverseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTheUniverseView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//ResizeParentToFit();
	if (m_pDocument)
	{
		theApp.SetFrameInfo(m_hWnd);
	}
}


// CTheUniverseView printing


void CTheUniverseView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTheUniverseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTheUniverseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTheUniverseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTheUniverseView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void CTheUniverseView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTheUniverseView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTheUniverseView diagnostics

#ifdef _DEBUG
void CTheUniverseView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTheUniverseView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTheUniverseDoc* CTheUniverseView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTheUniverseDoc)));
	return (CTheUniverseDoc*)m_pDocument;
}
#endif //_DEBUG


// CTheUniverseView message handlers
