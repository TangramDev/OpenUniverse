/********************************************************************************
*					Open Universe - version 1.0.1.8								*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
********************************************************************************/

// Grid.cpp : implementation file

#include "stdafx.h"
#include "UniverseApp.h"
#include "Hubble.h"
#include "grid.h"
#include "Galaxy.h"
#include "GridHelper.h"
#include "GridWnd.h"
#include "chromium/Browser.h"
#include "chromium/WebPage.h"

struct AUX_DATA
{
	// system metrics
	int cxVScroll, cyHScroll;
	int cxIcon, cyIcon;

	int cxBorder2, cyBorder2;

	// device metrics for screen
	int cxPixelsPerInch, cyPixelsPerInch;

	// convenient system color
	HBRUSH hbrWindowFrame;
	HBRUSH hbrBtnFace;

	// color values of system colors used for CToolBar
	COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
	COLORREF clrBtnText, clrWindowFrame;

	// standard cursors
	HCURSOR hcurWait;
	HCURSOR hcurArrow;
	HCURSOR hcurHelp;       // cursor used in Shift+F1 help

	// special GDI objects allocated on demand
	HFONT   hStatusFont;
	HFONT   hToolTipsFont;
	HBITMAP hbmMenuDot;

	// Implementation
	AUX_DATA();
	~AUX_DATA();
	void UpdateSysColors();
	void UpdateSysMetrics();
};

extern AFX_DATA AUX_DATA afxData;
#ifndef AFX_CX_BORDER
#define AFX_CX_BORDER CX_BORDER 
#endif

#ifndef AFX_CY_BORDER
#define AFX_CY_BORDER CY_BORDER 
#endif
#define CX_BORDER   1
#define CY_BORDER   1

// CGridWnd
enum HitTestValue
{
	noHit = 0,
	vSplitterBox = 1,
	hSplitterBox = 2,
	bothSplitterBox = 3,        // just for keyboard
	vSplitterBar1 = 101,
	vSplitterBar15 = 115,
	hSplitterBar1 = 201,
	hSplitterBar15 = 215,
	splitterIntersection1 = 301,
	splitterIntersection225 = 525
};

IMPLEMENT_DYNCREATE(CGridWnd, CSplitterWnd)

CGridWnd::CGridWnd()
{
	bInited = false;
	m_bCreated = false;
	m_pGrid = nullptr;
	m_pHostGrid = nullptr;
	m_nHostWidth = m_nHostHeight = 0;
	m_nMasterRow = m_nMasterCol = -1;
	m_Vmin = m_Vmax = m_Hmin = m_Hmax = 0;
}

CGridWnd::~CGridWnd()
{
}

BEGIN_MESSAGE_MAP(CGridWnd, CSplitterWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	ON_MESSAGE(WM_TABCHANGE, OnActivePage)
	ON_MESSAGE(WM_HUBBLE_GETNODE, OnGetHubbleObj)
	ON_MESSAGE(WM_COSMOSMSG, OnSplitterNodeAdd)
	ON_MESSAGE(WM_TGM_SETACTIVEPAGE, OnActiveTangramObj)
	ON_MESSAGE(WM_HOSTNODEFORSPLITTERCREATED, OnSplitterCreated)
END_MESSAGE_MAP()

// CGridWnd diagnostics

#ifdef _DEBUG
void CGridWnd::AssertValid() const
{
	//CSplitterWnd::AssertValid();
}

#ifndef _WIN32_WCE
void CGridWnd::Dump(CDumpContext& dc) const
{
	CSplitterWnd::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CGridWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (CWnd::OnNotify(wParam, lParam, pResult))
		return TRUE;

	// route commands to the splitter to the parent frame window
	//*pResult = EnsureParentFrame()->SendMessage(WM_NOTIFY, wParam, lParam);
	return true;
}

void CGridWnd::TrackRowSize(int y, int row)
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 1);

	CPoint pt(0, y);
	ClientToScreen(&pt);
	GetPane(row, 0)->ScreenToClient(&pt);
	m_pRowInfo[row].nIdealSize = pt.y;      // new size
	if (pt.y < m_pRowInfo[row].nMinSize)
	{
		// resized too small
		m_pRowInfo[row].nIdealSize = 0; // make it go away
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
			DeleteRow(row);
	}
	else if (m_pRowInfo[row].nCurSize + m_pRowInfo[row + 1].nCurSize
		< pt.y + m_pRowInfo[row + 1].nMinSize)
	{
		// not enough room for other pane
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
			DeleteRow(row + 1);
	}
	if (m_pHostGrid && row != m_nRows - 1)
	{
		ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

		CRect rectInside;
		GetInsideRect(rectInside);

		int i;
		int _nSize = rectInside.Height();
		CSplitterWnd::CRowColInfo* pInfo;

		if (_nSize < 0)
			_nSize = 0;  // if really too small, layout as zero size

		for (i = 0, pInfo = m_pRowInfo; i < m_nRows - 1; i++, pInfo++)
		{
			if (pInfo->nIdealSize < pInfo->nMinSize)
				pInfo->nIdealSize = 0;      // too small to see
			_nSize -= pInfo->nIdealSize;
		}
		_nSize -= (m_nRows - 1) * m_cxSplitterGap;
		pInfo->nCurSize = _nSize;
		pInfo->nIdealSize = _nSize;
	}
}

void CGridWnd::TrackColumnSize(int x, int col)
{
	ASSERT_VALID(this);
	ASSERT(m_nCols > 1);

	CPoint pt(x, 0);
	ClientToScreen(&pt);
	GetPane(0, col)->ScreenToClient(&pt);
	m_pColInfo[col].nIdealSize = pt.x;      // new size
	if (pt.x < m_pColInfo[col].nMinSize)
	{
		// resized too small
		m_pColInfo[col].nIdealSize = 0; // make it go away
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
			DeleteColumn(col);
	}
	else if (m_pColInfo[col].nCurSize + m_pColInfo[col + 1].nCurSize
		< pt.x + m_pColInfo[col + 1].nMinSize)
	{
		// not enough room for other pane
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
			DeleteColumn(col + 1);
	}
	if (m_pHostGrid && col != m_nCols - 1)
	{
		ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

		CRect rectInside;
		GetInsideRect(rectInside);

		int i;
		int _nSize = rectInside.Width();
		CSplitterWnd::CRowColInfo* pInfo;

		if (_nSize < 0)
			_nSize = 0;  // if really too small, layout as zero size

		for (i = 0, pInfo = m_pColInfo; i < m_nCols - 1; i++, pInfo++)
		{
			if (pInfo->nIdealSize < pInfo->nMinSize)
				pInfo->nIdealSize = 0;      // too small to see
			_nSize -= pInfo->nIdealSize;
		}
		_nSize -= (m_nCols - 1) * m_cxSplitterGap;
		pInfo->nCurSize = _nSize;
		pInfo->nIdealSize = _nSize;
	}
}

LRESULT CGridWnd::OnSplitterNodeAdd(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 1992 || wParam == 0x01000 || wParam == 0)
	{
		return 0;
	}
	if (lParam == 1993)
	{
		//fix bug for .net Control or Window Form
		switch (wParam)
		{
		case 1:
		{
			bInited = true;
			::PostMessage(m_hWnd, WM_COSMOSMSG, 2, 1993);
			return 0;
		}
		break;
		case 2:
		{
			m_pGrid->m_pGridShareData->m_pGalaxy->HostPosChanged();
			return 0;
		}
		break;
		case 3:
		{
			RecalcLayout();
			return 0;
		}
		break;
		}
		return 0;
	}
	IGrid* _pGrid = nullptr;
	CString str = (LPCTSTR)lParam;
	CGrid* pOldNode = (CGrid*)g_pHubble->m_pDesignGrid;
	CTangramXmlParse* pOld = pOldNode->m_pHostParse;

	long	m_nRow;
	g_pHubble->m_pDesignGrid->get_Row(&m_nRow);
	long	m_nCol;
	g_pHubble->m_pDesignGrid->get_Col(&m_nCol);
	IGrid* _pOldNode = nullptr;
	m_pGrid->GetGrid(m_nRow, m_nCol, &_pOldNode);
	CGrid* _pOldNode2 = (CGrid*)_pOldNode;
	CTangramXmlParse* _pOldParse = _pOldNode2->m_pHostParse;
	m_pGrid->ObserveEx(m_nRow, m_nCol, CComBSTR(L""), str.AllocSysString(), &_pGrid);
	CGrid* pGrid = (CGrid*)_pGrid;
	if (pGrid && pOldNode)
	{
		CGalaxy* pGalaxy = m_pGrid->m_pGridShareData->m_pGalaxy;
		pGrid->m_pGridShareData->m_pGalaxy->m_bDesignerState = true;
		CGridVector::iterator it;
		it = find(m_pGrid->m_vChildNodes.begin(), m_pGrid->m_vChildNodes.end(), pOldNode);

		if (it != m_pGrid->m_vChildNodes.end())
		{
			pGrid->m_pRootObj = m_pGrid->m_pRootObj;
			pGrid->m_pParentObj = m_pGrid;
			CGridVector vec = pGrid->m_vChildNodes;
			CGrid* pChildNode = nullptr;
			for (auto it2 : pGrid->m_vChildNodes)
			{
				pChildNode = it2;
				pChildNode->m_pRootObj = m_pGrid->m_pRootObj;
			}
			CTangramXmlParse* pNew = pGrid->m_pHostParse;
			CTangramXmlParse* pRet = m_pGrid->m_pHostParse->ReplaceNode(pOld, pNew, _T(""));
			m_pGrid->m_vChildNodes.erase(it);
			m_pGrid->m_vChildNodes.push_back(pGrid);
			pOldNode->m_pHostWnd->DestroyWindow();
			g_pHubble->m_pDesignGrid = nullptr;
			RecalcLayout();
		}
	}
	return -1;
}

LRESULT CGridWnd::OnActiveTangramObj(WPARAM wParam, LPARAM lParam)
{
	//RecalcLayout();
	//m_pGrid->m_pGridShareData->m_pGalaxy->HostPosChanged();
	//::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return -1;
}

LRESULT CGridWnd::OnSplitterCreated(WPARAM wParam, LPARAM lParam)
{
	int _nWidth = 0;
	SetColumnInfo(lParam, m_nHostWidth >= 0 ? m_nHostWidth : 0, _nWidth);
	SetRowInfo(wParam, m_nHostHeight, _nWidth);
	//SetColumnInfo(lParam, (m_nHostWidth>=0)? m_nHostWidth:0, _nWidth);
	//SetRowInfo(wParam, (m_nHostHeight>=0)? m_nHostHeight:0, _nWidth);
	return 0;
}

LRESULT CGridWnd::OnActivePage(WPARAM wParam, LPARAM lParam)
{
	return CWnd::DefWindowProc(WM_TABCHANGE, wParam, lParam);;
}

void CGridWnd::StartTracking(int ht)
{
	ASSERT_VALID(this);
	if (ht == noHit)
		return;

	CGrid* pGrid = m_pGrid->m_pGridShareData->m_pGalaxy->m_pWorkGrid;
	if (pGrid && pGrid->m_pGridShareData->m_pHostClientView)
	{
		pGrid->m_pHostWnd->ModifyStyle(WS_CLIPSIBLINGS, 0);
	}

	GetInsideRect(m_rectLimit);

	if (ht >= splitterIntersection1 && ht <= splitterIntersection225)
	{
		// split two directions (two tracking rectangles)
		int row = (ht - splitterIntersection1) / 15;
		int col = (ht - splitterIntersection1) % 15;

		GetHitRect(row + vSplitterBar1, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = true;
		GetHitRect(col + hSplitterBar1, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;
	}
	else if (ht == bothSplitterBox)
	{
		// hit on splitter boxes (for keyboard)
		GetHitRect(vSplitterBox, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = true;
		GetHitRect(hSplitterBox, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;

		// center it
		m_rectTracker.OffsetRect(0, m_rectLimit.Height() / 2);
		m_rectTracker2.OffsetRect(m_rectLimit.Width() / 2, 0);
	}
	else
	{
		// only hit one bar
		GetHitRect(ht, m_rectTracker);
	}

	// steal focus and capture
	SetCapture();
	SetFocus();

	// make sure no updates are pending
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

	// set tracking state and appropriate cursor
	m_bTracking = true;
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);
	m_htTrack = ht;
}

void CGridWnd::StopTracking(BOOL bAccept)
{
	if (!m_bTracking)
		return;
	CGalaxy* pGalaxy = m_pGrid->m_pGridShareData->m_pGalaxy;
	CGrid* pGrid = pGalaxy->m_pWorkGrid;
	if (pGrid && pGrid->m_pGridShareData->m_pHostClientView)
	{
		pGrid->m_pHostWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		::InvalidateRect(pGalaxy->m_hWnd, NULL, false);
		pGrid->m_pHostWnd->Invalidate();
	}

	CSplitterWnd::StopTracking(bAccept);

	if (bAccept)
	{
		::InvalidateRect(pGalaxy->m_hWnd, nullptr, true);

		CWebPage* pWebWnd = nullptr;
		if (pGalaxy->m_pWebPageWnd)
		{
			pWebWnd = pGalaxy->m_pWebPageWnd;
		}
		else if (g_pHubble->m_pDesignGrid && g_pHubble->m_pDesignGrid->m_pGridShareData->m_pGalaxy->m_pWebPageWnd)
			pWebWnd = g_pHubble->m_pDesignGrid->m_pGridShareData->m_pGalaxy->m_pWebPageWnd;
		pGalaxy->HostPosChanged();
		HWND h = ::GetParent(m_hWnd);
		if (h)
		{
			LRESULT lRes = ::SendMessage(h, WM_HUBBLE_GETNODE, 0, 0);
			if (lRes)
			{
				CGrid* pRetNode = (CGrid*)lRes;
				if (pRetNode && pRetNode->m_nViewType == Grid)
				{
					pRetNode->m_pGridShareData->m_pGalaxy->HostPosChanged();
				}
			}
		}
		RecalcLayout();
		if (pWebWnd)
		{
			::SendMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
			::PostMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
		}
	}
}

void CGridWnd::PostNcDestroy()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	delete m_pGrid;
	CSplitterWnd::PostNcDestroy();
	delete this;
}

// Generic routine:
//  for X direction: pInfo = m_pColInfo, nMax = m_nMaxCols, nSize = cx
//  for Y direction: pInfo = m_pRowInfo, nMax = m_nMaxRows, nSize = cy
void CGridWnd::_LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray, int nMax, int nSize, int nSizeSplitter, CGrid* pHostNode, bool bCol)
{
	ASSERT(pInfoArray != NULL);
	ASSERT(nMax > 0);
	if (nSizeSplitter < 0)
		nSizeSplitter = 0;
	//ASSERT(nSizeSplitter > 0);

	CSplitterWnd::CRowColInfo* pInfo;
	CSplitterWnd::CRowColInfo* pInfoHost = nullptr;
	int i;

	if (nSize < 0)
		nSize = 0;  // if really too small, layout as zero size

					// start with ideal sizes
	int _indexHost = -1;
	int _nSize = nSize;
	if (pHostNode)
	{
		if (bCol)
			_indexHost = pHostNode->m_nCol;
		else
			_indexHost = pHostNode->m_nRow;
	}
	if (_indexHost != -1)
		pInfoHost = &pInfoArray[_indexHost];

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		if (pInfo->nIdealSize < pInfo->nMinSize)
			pInfo->nIdealSize = 0;      // too small to see
		pInfo->nCurSize = pInfo->nIdealSize;
		if (_indexHost != -1)
		{
			if (_indexHost != i)
			{
				_nSize -= pInfo->nIdealSize;
			}
		}
		if (i == nMax - 1)
		{
			if (_indexHost != -1 && pInfoHost)
			{
				if (_indexHost != nMax - 1)
				{
					_nSize -= (nMax - 1) * nSizeSplitter;
					if (_nSize < 0)
						_nSize = 0;
					pInfoHost->nCurSize = _nSize;
				}
				else
					pInfoHost->nCurSize = INT_MAX;  // last row/column takes the rest
				if (bCol)
					m_nHostWidth = _nSize;
				else
					m_nHostHeight = _nSize;
			}
			else
				pInfo->nCurSize = INT_MAX;  // last row/column takes the rest
		}
	}

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		ASSERT(nSize >= 0);
		if (nSize == 0)
		{
			// no more room (set pane to be invisible)
			pInfo->nCurSize = 0;
			continue;       // don't worry about splitters
		}
		else if (nSize < pInfo->nMinSize && i != 0)
		{
			// additional panes below the recommended minimum size
			//   aren't shown and the size goes to the previous pane
			pInfo->nCurSize = 0;

			// previous pane already has room for splitter + border
			//   add remaining size and remove the extra border
			ASSERT(afxData.cxBorder2 == afxData.cyBorder2);
			(pInfo - 1)->nCurSize += nSize + afxData.cxBorder2;
			nSize = 0;
		}
		else
		{
			// otherwise we can add the second pane
			ASSERT(nSize > 0);
			if (pInfo->nCurSize == 0)
			{
				// too small to see
				if (i != 0)
					pInfo->nCurSize = 0;
			}
			else if (nSize < pInfo->nCurSize)
			{
				// this row/col won't fit completely - make as small as possible
				pInfo->nCurSize = nSize;
				nSize = 0;
			}
			else
			{
				// can fit everything
				nSize -= pInfo->nCurSize;
			}
		}

		// see if we should add a splitter
		ASSERT(nSize >= 0);
		if (i != nMax - 1)
		{
			// should have a splitter
			if (nSize > nSizeSplitter)
			{
				nSize -= nSizeSplitter; // leave room for splitter + border
				ASSERT(nSize > 0);
			}
			else
			{
				// not enough room - add left over less splitter size
				ASSERT(afxData.cxBorder2 == afxData.cyBorder2);
				pInfo->nCurSize += nSize;
				if (pInfo->nCurSize > (nSizeSplitter - afxData.cxBorder2))
					pInfo->nCurSize -= (nSizeSplitter - afxData.cyBorder2);
				nSize = 0;
			}
		}
	}
	//ASSERT(nSize == 0); // all space should be allocated
}

// repositions client area of specified window
// assumes everything has WS_BORDER or is inset like it does
//  (includes scroll bars)
void CGridWnd::_DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout, CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
{
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd != NULL);
	CRect rect(x, y, x + cx, y + cy);

	// adjust for 3d border (splitter windows have implied border)
	if ((pWnd->GetExStyle() & WS_EX_CLIENTEDGE) ||
		pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
		rect.InflateRect(afxData.cxBorder2, afxData.cyBorder2);

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	pWnd->GetWindowRect(rectOld);
	pWnd->GetParent()->ScreenToClient(&rectOld);
	if (rect != rectOld)
	{
		ASSERT(pWnd->m_hWnd != NULL);
		ASSERT(rect != NULL);
		HWND hWndParent = ::GetParent(pWnd->m_hWnd);
		ASSERT(hWndParent != NULL);

		if (lpLayout != NULL && lpLayout->hDWP == NULL)
			return;

		// first check if the new rectangle is the same as the current
		CRect rectOld;
		::GetWindowRect(pWnd->m_hWnd, rectOld);
		::ScreenToClient(hWndParent, &rectOld.TopLeft());
		::ScreenToClient(hWndParent, &rectOld.BottomRight());
		if (::EqualRect(rectOld, rect))
			return;     // nothing to do

		// try to use DeferWindowPos for speed, otherwise use SetWindowPos
		if (lpLayout != NULL)
		{
			lpLayout->hDWP = ::DeferWindowPos(lpLayout->hDWP, pWnd->m_hWnd, NULL,
				rect.left, rect.top, rect.right - rect.left,
				rect.bottom - rect.top, SWP_NOACTIVATE | SWP_NOZORDER);
		}
		else
		{
			::SetWindowPos(pWnd->m_hWnd, NULL, rect.left, rect.top, rect.right - rect.left,
				rect.bottom - rect.top,
				SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}
	else
	{
		::PostMessage(pWnd->m_hWnd, WM_SPLITTERREPOSITION, 0, 0);
	}
}

void CGridWnd::RecalcLayout()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane||::IsWindowVisible(m_hWnd) == FALSE
	if (m_bCreated == false || GetDlgItem(IdFromRowCol(0, 0)) == NULL)
		return;
	_RecalcLayout();
}

void CGridWnd::_RecalcLayout()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

	if (m_nMaxCols >= 2)
	{
		int LimitWidth = 0;
		int LimitWndCount = 0;
		int Width = 0;
		RECT SplitterWndRect;
		GetWindowRect(&SplitterWndRect);
		Width = SplitterWndRect.right - SplitterWndRect.left - m_nMaxCols * m_cxSplitterGap - LimitWidth + m_cxBorder * m_nMaxCols;
		if (Width < 0)
			return;
	}
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);

	CRect rectInside;
	GetInsideRect(rectInside);

	// layout columns (restrict to possible sizes)
	_LayoutRowCol(m_pColInfo, m_nCols, rectInside.Width(), m_cxSplitterGap, m_pHostGrid, true);
	_LayoutRowCol(m_pRowInfo, m_nRows, rectInside.Height(), m_cySplitterGap, m_pHostGrid, false);

	// give the hint for the maximum number of HWNDs
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);

	//BLOCK: Reposition all the panes
	{
		int x = rectClient.left;
		for (int col = 0; col < m_nCols; col++)
		{
			int cxCol = m_pColInfo[col].nCurSize;
			int y = rectClient.top;
			for (int row = 0; row < m_nRows; row++)
			{
				int cyRow = m_pRowInfo[row].nCurSize;
				CWnd* pWnd = GetPane(row, col);
				if (pWnd)
					_DeferClientPos(&layout, pWnd, x, y, cxCol, cyRow, FALSE);
				y += cyRow + m_cySplitterGap;
			}
			x += cxCol + m_cxSplitterGap;
		}
	}

	// move and resize all the windows at once!
	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
		TRACE(traceAppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");

	// invalidate all the splitter bars (with NULL pDC)
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
	//::InvalidateRect(m_hWnd, nullptr, false);
}

BOOL CGridWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_pGrid = g_pHubble->m_pActiveGrid;
	m_pGrid->m_pHostWnd = this;
	m_pGrid->m_nViewType = Grid;
	m_pGrid->m_nID = nID;
	m_pGrid->m_pDisp = nullptr;

	int r, g, b;
	CComBSTR bstrVal(L"");
	m_pGrid->get_Attribute(CComBSTR("lefttopcolor"), &bstrVal);
	if (!CString(bstrVal).IsEmpty())
	{
		_stscanf_s(CString(bstrVal), _T("RGB(%d,%d,%d)"), &r, &g, &b);
		rgbLeftTop = RGB(r, g, b);
	}
	else
	{
		rgbLeftTop = RGB(240, 240, 240);
	}

	bstrVal.Empty();
	m_pGrid->get_Attribute(CComBSTR("rightbottomcolor"), &bstrVal);
	if (!CString(bstrVal).IsEmpty())
	{
		_stscanf_s(CString(bstrVal), _T("RGB(%d,%d,%d)"), &r, &g, &b);
		rgbRightBottom = RGB(r, g, b);
	}
	else
	{
		rgbRightBottom = RGB(240, 240, 240);
	}
	bstrVal.Empty();
	m_pGrid->get_Attribute(CComBSTR(L"middlecolor"), &bstrVal);
	if (!CString(bstrVal).IsEmpty())
	{
		_stscanf_s(CString(bstrVal), _T("RGB(%d,%d,%d)"), &r, &g, &b);
		rgbMiddle = RGB(r, g, b);
	}
	else
	{
		rgbMiddle = RGB(240, 240, 240);
	}

	bstrVal.Empty();
	m_pGrid->get_Attribute(CComBSTR(L"splitterwidth"), &bstrVal);
	m_cxSplitterGap = m_cySplitterGap = m_cxSplitter = m_cySplitter = !CString(bstrVal).IsEmpty() ? _ttoi(CString(bstrVal)) : 7;

	bstrVal.Empty();
	m_pGrid->get_Attribute(CComBSTR(L"borderwidth"), &bstrVal);
	m_cxBorder = m_cyBorder = !CString(bstrVal).IsEmpty() ? _ttoi(CString(bstrVal)) : 2;

	bstrVal.Empty();
	m_pGrid->get_Attribute(CComBSTR(L"vmin"), &bstrVal);
	m_Vmin = !CString(bstrVal).IsEmpty() ? _ttoi(CString(bstrVal)) : 0;

	bstrVal.Empty();
	m_pGrid->get_Attribute(CComBSTR(L"vmax"), &bstrVal);
	if (!CString(bstrVal).IsEmpty())
	{
		m_Vmax = _ttoi(CString(bstrVal));
		if (m_Vmax <= 0)
			m_Vmax = 65535;
	}
	else
		m_Vmax = 65535;
	bstrVal.Empty();
	m_pGrid->get_Attribute(CComBSTR(L"hmin"), &bstrVal);
	m_Hmin = !CString(bstrVal).IsEmpty() ? _ttoi(CString(bstrVal)) : 0;

	bstrVal.Empty();
	m_pGrid->get_Attribute(CComBSTR(L"hmax"), &bstrVal);
	if (!CString(bstrVal).IsEmpty())
	{
		m_Hmax = _ttoi(CString(bstrVal));
		if (m_Hmax <= 0)
			m_Hmax = 65535;
	}
	else
		m_Hmax = 65535;

	m_nMasterRow = m_pGrid->m_pHostParse->attrInt(L"masterrow", -1);
	m_nMasterCol = m_pGrid->m_pHostParse->attrInt(L"mastercol", -1);

	if (nID == 0)
		nID = 1;

	if (CreateStatic(pParentWnd, m_pGrid->m_nRows, m_pGrid->m_nCols, dwStyle, nID))
	{
		m_pGrid->NodeCreated();
		CString strWidth = m_pGrid->m_pHostParse->attr(TGM_WIDTH, _T(""));
		strWidth += _T(",");
		CString strHeight = m_pGrid->m_pHostParse->attr(TGM_HEIGHT, _T(""));
		strHeight += _T(",");

		int nWidth, nHeight, nPos;
		CString strW = _T(""), strH = _T(""), strOldWidth = _T(""), strName = _T("");

		strOldWidth = strWidth;
		long nSize = m_pGrid->m_pHostParse->GetCount();
		int nIndex = 0;
		CTangramXmlParse* pSubItem = m_pGrid->m_pHostParse->GetChild(nIndex);
		if (pSubItem == nullptr)
		{
			strName.Format(_T("%s_splitterchild_%i"), m_pGrid->m_strName, 0);
			pSubItem = m_pGrid->m_pHostParse->AddNode(strName);
		}
		for (int i = 0; i < m_pGrid->m_nRows; i++)
		{
			nPos = strHeight.Find(_T(","));
			strH = strHeight.Left(nPos);
			strHeight = strHeight.Mid(nPos + 1);
			nHeight = _ttoi(strH);

			strWidth = strOldWidth;
			for (int j = 0; j < m_pGrid->m_nCols; j++)
			{
				nPos = strWidth.Find(_T(","));
				strW = strWidth.Left(nPos);
				strWidth = strWidth.Mid(nPos + 1);
				nWidth = _ttoi(strW);
				CGrid* pObj = new CComObject<CGrid>;
				pObj->m_pRootObj = m_pGrid->m_pRootObj;
				pObj->m_pHostParse = pSubItem;
				pObj->m_pParentObj = m_pGrid;

				m_pGrid->AddChildNode(pObj);
				pObj->InitWndGrid();

				if (pObj->m_pObjClsInfo)
				{
					pObj->m_nRow = i;
					pObj->m_nCol = j;

					pObj->m_nWidth = nWidth;
					pObj->m_nHeigh = nHeight;
					if (pContext->m_pNewViewClass == nullptr)
						pContext->m_pNewViewClass = RUNTIME_CLASS(CGridHelper);
					CreateView(pObj->m_nRow, pObj->m_nCol, pObj->m_pObjClsInfo, CSize(max(pObj->m_nWidth, m_Hmin), max(pObj->m_nHeigh, m_Vmin)), pContext);
				}
				if (m_nMasterRow == i && m_nMasterCol == j)
				{
					m_pHostGrid = pObj;
				}
				nIndex++;
				if (nIndex < nSize)
					pSubItem = m_pGrid->m_pHostParse->GetChild(nIndex);
				else if (nIndex < m_pGrid->m_nCols * m_pGrid->m_nRows)
				{
					strName.Format(_T("%s_splitterchild_%i"), m_pGrid->m_strName, nIndex);
					pSubItem = m_pGrid->m_pHostParse->AddNode(strName);
				}
			}
		}
		SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOREDRAW);

		SetWindowText(m_pGrid->m_strNodeName);
		m_bCreated = true;
		CGrid* pHostNode = nullptr;
		CGrid* pParent = nullptr;
		CGalaxy* pGalaxy = m_pGrid->m_pGridShareData->m_pGalaxy;
		bool bHasHostView = false;
		if (pGalaxy->m_pBindingGrid)
		{
			pHostNode = pGalaxy->m_pBindingGrid;
			if (::IsChild(m_hWnd, pHostNode->m_pHostWnd->m_hWnd))
			{
				bHasHostView = true;
				pParent = pHostNode->m_pParentObj;
				while (pParent != m_pGrid)
				{
					pHostNode = pParent;
					pParent = pHostNode->m_pParentObj;
				}
			}
		}
		if (pHostNode && ::IsChild(m_hWnd, pHostNode->m_pHostWnd->m_hWnd))
			m_pHostGrid = pHostNode;
		//if (m_pHostGrid == nullptr)
		//{
		//	if (m_nMasterCol != -1 && m_nMasterRow != -1)
		//	{
		//		IGrid* pGrid = nullptr;
		//		m_pGrid->GetGrid(m_nMasterRow, m_nMasterCol, &pGrid);
		//		if (pGrid)
		//		{
		//			m_pHostGrid = (CGrid*)pGrid;
		//		}
		//	}
		//}
		_RecalcLayout();

		return true;
	}
	return false;
}

LRESULT CGridWnd::OnGetHubbleObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrid)
		return (LRESULT)m_pGrid;
	return (long)CWnd::DefWindowProc(WM_HUBBLE_GETNODE, wParam, lParam);;
}

void CGridWnd::OnPaint()
{
	ASSERT_VALID(this);
	CPaintDC dc(this);

	CRect rectClient;
	GetClientRect(&rectClient);

	CRect rectInside;
	GetInsideRect(rectInside);
	rectInside.InflateRect(4, 4);


	// draw the splitter boxes
	if (m_bHasVScroll && m_nRows < m_nMaxRows)
	{
		OnDrawSplitter(&dc, splitBox, CRect(rectInside.right, rectClient.top, rectClient.right, rectClient.top + m_cySplitter));
	}

	if (m_bHasHScroll && m_nCols < m_nMaxCols)
	{
		OnDrawSplitter(&dc, splitBox,
			CRect(rectClient.left, rectInside.bottom,
				rectClient.left + m_cxSplitter, rectClient.bottom));
	}

	// extend split bars to window border (past margins)
	DrawAllSplitBars(&dc, rectInside.right, rectInside.bottom);
	// draw splitter intersections (inside only)
	GetInsideRect(rectInside);
	dc.IntersectClipRect(rectInside);
	CRect rect;
	rect.top = rectInside.top;
	for (int row = 0; row < m_nRows - 1; row++)
	{
		rect.top += m_pRowInfo[row].nCurSize + m_cyBorderShare;
		rect.bottom = rect.top + m_cySplitter;
		rect.left = rectInside.left;
		for (int col = 0; col < m_nCols - 1; col++)
		{
			rect.left += m_pColInfo[col].nCurSize + m_cxBorderShare;
			rect.right = rect.left + m_cxSplitter;
			OnDrawSplitter(&dc, splitIntersection, rect);
			rect.left = rect.right + m_cxBorderShare;
		}
		rect.top = rect.bottom + m_cxBorderShare;
	}
}

void CGridWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	if (pDC == nullptr)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);
	;

	// otherwise, actually draw
	CRect rect = rectArg;
	switch (nType)
	{
	case splitBorder:
		//ASSERT(afxData.bWin4);
		pDC->Draw3dRect(rect, rgbLeftTop, rgbRightBottom);
		rect.InflateRect(-AFX_CX_BORDER, -AFX_CY_BORDER);
		pDC->Draw3dRect(rect, rgbLeftTop, rgbRightBottom);

		return;

	case splitIntersection:

		//ASSERT(afxData.bWin4);
		break;

	case splitBox:
		//if (afxData.bWin4)
	{
		pDC->Draw3dRect(rect, rgbLeftTop, rgbRightBottom);
		rect.InflateRect(-AFX_CX_BORDER, -AFX_CY_BORDER);
		pDC->Draw3dRect(rect, rgbLeftTop, rgbRightBottom);
		rect.InflateRect(-AFX_CX_BORDER, -AFX_CY_BORDER);
		break;
	}
	// fall through...
	case splitBar:
	{
		pDC->FillSolidRect(rect, rgbMiddle);
		//pDC->FillSolidRect(rect, rgbMiddle);
		if ((rect.bottom - rect.top) > (rect.right - rect.left))
		{
			rect.bottom -= 1;
			rect.top += 1;
		}
		else
		{
			rect.right -= 1;
			rect.left += 1;
		}
	}
	break;

	default:
		ASSERT(false);  // unknown splitter type
	}
}

BOOL CGridWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = g_pHubble->m_lpszSplitterClass;
	cs.style |= WS_CLIPSIBLINGS;
	return CSplitterWnd::PreCreateWindow(cs);
}

void CGridWnd::DrawAllSplitBars(CDC* pDC, int cxInside, int cyInside)
{
	//ColRect.clear();
	//RowRect.clear();
	ASSERT_VALID(this);

	int col;
	int row;
	CRect rect;

	// draw pane borders
	GetClientRect(rect);
	int x = rect.left;
	for (col = 0; col < m_nCols; col++)
	{
		int cx = m_pColInfo[col].nCurSize + 2 * m_cxBorder;
		if (col == m_nCols - 1 && m_bHasVScroll)
			cx += afxData.cxVScroll - CX_BORDER;
		int y = rect.top;
		for (row = 0; row < m_nRows; row++)
		{
			int cy = m_pRowInfo[row].nCurSize + 2 * m_cyBorder;
			if (row == m_nRows - 1 && m_bHasHScroll)
				cy += afxData.cyHScroll - CX_BORDER;
			OnDrawSplitter(pDC, splitBorder, CRect(x, y, x + cx, y + cy));
			y += cy + m_cySplitterGap - 2 * m_cyBorder;
		}
		x += cx + m_cxSplitterGap - 2 * m_cxBorder;
	}


	// draw column split bars
	GetClientRect(rect);
	rect.left += m_cxBorder;
	for (col = 0; col < m_nCols - 1; col++)
	{
		rect.left += m_pColInfo[col].nCurSize + m_cxBorderShare;
		rect.right = rect.left + m_cxSplitter;
		if (rect.left > cxInside)
			break;      // stop if not fully visible
		//ColumnsplitBar = rect;
		//ColRect.push_back(rect);
		OnDrawSplitter(pDC, splitBar, rect);

		rect.left = rect.right + m_cxBorderShare;
	}

	// draw row split bars
	GetClientRect(rect);
	rect.top += m_cyBorder;
	for (row = 0; row < m_nRows - 1; row++)
	{
		rect.top += m_pRowInfo[row].nCurSize + m_cyBorderShare;
		rect.bottom = rect.top + m_cySplitter;
		if (rect.top > cyInside)
			break;      // stop if not fully visible
		//RowsplitBar = rect;
		//RowRect.push_back(rect);
		OnDrawSplitter(pDC, splitBar, rect);

		rect.top = rect.bottom + m_cyBorderShare;
	}
}

CWnd* CGridWnd::GetActivePane(int* pRow, int* pCol)
{
	CWnd* pView = nullptr;
	pView = GetFocus();

	// make sure the pane is a child pane of the splitter
	if (pView != nullptr && !IsChildPane(pView, pRow, pCol))
		pView = nullptr;


	return pView;
}

int CGridWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pHubble->m_pActiveHtmlWnd)
	{
		g_pHubble->m_pActiveHtmlWnd = nullptr;
	}

	CGalaxy* pGalaxy = m_pGrid->m_pGridShareData->m_pGalaxy;

	if (pGalaxy->m_pGalaxyCluster->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = pGalaxy->m_pGalaxyCluster->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	else if ((long)(g_pHubble->m_pActiveAppProxy) > 1)
	{
		HWND hMenuWnd = g_pHubble->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}

	return MA_ACTIVATE;// CSplitterWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CGridWnd::Save()
{
	CString strWidth = _T("");
	CString strHeight = _T("");

	int minCx, minCy;

	for (int i = 0; i < m_pGrid->m_nRows; i++)
	{
		int iHeight;
		CString strH;
		GetRowInfo(i, iHeight, minCy);
		strH.Format(_T("%d,"), iHeight);

		strHeight += strH;
	}

	for (int j = 0; j < m_pGrid->m_nCols; j++)
	{
		int iWidth;
		CString strW;
		GetColumnInfo(j, iWidth, minCx);
		strW.Format(_T("%d,"), iWidth);

		strWidth += strW;
	}

	m_pGrid->put_Attribute(CComBSTR(TGM_HEIGHT), CComBSTR(strHeight));
	m_pGrid->put_Attribute(CComBSTR(TGM_WIDTH), CComBSTR(strWidth));
}

void CGridWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (point.x < m_Hmin && point.y < m_Vmin)
	{
		CSplitterWnd::OnMouseMove(nFlags, CPoint(m_Hmin, m_Vmin));
	}

	else if (point.x < m_Hmin && point.y > m_Vmin && point.y < m_Vmax)
	{
		CSplitterWnd::OnMouseMove(nFlags, CPoint(m_Hmin, point.y));
	}

	else if (point.x < m_Hmin && point.y > m_Vmax)
	{
		CSplitterWnd::OnMouseMove(nFlags, CPoint(m_Hmin, m_Vmax));
	}

	else if (point.x > m_Hmax && point.y < m_Vmin)
	{
		CSplitterWnd::OnMouseMove(nFlags, CPoint(m_Hmax, m_Vmin));
	}

	else if (point.x > m_Hmax && point.y > m_Vmin && point.y < m_Vmax)
	{
		CSplitterWnd::OnMouseMove(nFlags, CPoint(m_Hmax, point.y));
	}

	else if (point.x > m_Hmax && point.y > m_Vmax)
	{
		CSplitterWnd::OnMouseMove(nFlags, CPoint(m_Hmax, m_Vmax));
	}

	else if (point.x > m_Hmin && point.x < m_Hmax && point.y > m_Vmax)
	{
		CSplitterWnd::OnMouseMove(nFlags, CPoint(point.x, m_Vmax));
	}
	else if (point.x > m_Hmin && point.x < m_Hmax && point.y < m_Vmin)
	{
		CSplitterWnd::OnMouseMove(nFlags, CPoint(point.x, m_Vmin));
	}
	else
	{
		CSplitterWnd::OnMouseMove(nFlags, point);
	}

	//CDC *pDC = GetDC();
	//for (int col = 0; col < m_nCols - 1; col++)
	//{
	//	if(PtInRect( &ColRect.at(col),point) && bColMoving == 0)
	//	{
	//		pDC->FillSolidRect(&(ColRect.at(col)),rgbMiddle);
	//	}
	//}

	//for (int row = 0; row < m_nRows - 1; row++)
	//{
	//	if(PtInRect( &RowRect.at(row),point))
	//	{
	//		pDC->FillSolidRect(&(RowRect.at(row)), rgbMiddle);
	//	}
	//}
}

void CGridWnd::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (m_pColInfo != nullptr)
		RecalcLayout();
}

void CGridWnd::OnDestroy()
{
	m_pGrid->Fire_Destroy();
	__super::OnDestroy();
}

