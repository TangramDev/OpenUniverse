/********************************************************************************
*					Open Universe - version 0.9.9999								*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
********************************************************************************/

// TangramView.cpp : implementation file
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "Hubble.h"
#include "GridHelper.h"
#include "grid.h"
#include "Quasar.h"
#include "GridWnd.h"
#include "Wormhole.h"

#include "chromium/WebPage.h"

// CGridHelper

IMPLEMENT_DYNCREATE(CGridHelper, CWnd)

CGridHelper::CGridHelper()
{
	m_hFormWnd = NULL;
	m_bNoMove = false;
	m_bBKWnd = false;
	m_bCreateExternal = false;
	m_bEraseBkgnd = true;
	m_pGrid = nullptr;
	m_pParentGrid = nullptr;
	m_pOleInPlaceActiveObject = nullptr;
	m_strKey = m_strXml = _T("");
}

CGridHelper::~CGridHelper()
{
}

BEGIN_MESSAGE_MAP(CGridHelper, CWnd)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_TABCHANGE, OnTabChange)
	ON_MESSAGE(WM_COSMOSMSG, OnHubbleMsg)
	ON_MESSAGE(WM_TANGRAMGETNODE, OnGetHubbleObj)
	ON_MESSAGE(WM_TGM_SETACTIVEPAGE, OnActiveTangramObj)
	ON_MESSAGE(WM_SPLITTERREPOSITION, OnSplitterReposition)
END_MESSAGE_MAP()

// CGridHelper diagnostics

#ifdef _DEBUG
void CGridHelper::AssertValid() const
{
	///CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGridHelper::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif
#endif //_DEBUG

//CGridHelper message handlers
BOOL CGridHelper::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_pGrid = g_pHubble->m_pActiveGrid;
	m_pGrid->m_nID = nID;
	m_pGrid->m_pHostWnd = this;

	if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		CQuasar* pQuasar = m_pGrid->m_pGridCommonData->m_pQuasar;
		pQuasar->m_pBindingGrid = m_pGrid;

		m_pGrid->m_pGridCommonData->m_pHostClientView = this;
		CGalaxyCluster* pGalaxyCluster = pQuasar->m_pGalaxyCluster;
		HWND hWnd = CreateWindow(L"Hubble Grid Class", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);
		BOOL bRet = SubclassWindow(hWnd);
		if (m_pGrid->m_pParentObj)
		{
			if (m_pGrid->m_pParentObj->m_nViewType == Grid)
			{
				::PostMessage(::GetParent(m_hWnd), WM_HOSTNODEFORSPLITTERCREATED, m_pGrid->m_nRow, m_pGrid->m_nCol);
				ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
		}
		m_pGrid->NodeCreated();
		return bRet;
	}
	return m_pGrid->Create(dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CGridHelper::OnSplitterReposition(WPARAM wParam, LPARAM lParam)
{
	switch (m_pGrid->m_nViewType)
	{
	case CLRCtrl:
	case ActiveX:
	case TabGrid:
		m_pGrid->m_pGridCommonData->m_pQuasar->HostPosChanged();
		break;
	default:
		break;
	}

	return CWnd::DefWindowProc(WM_SPLITTERREPOSITION, wParam, lParam);
}

int CGridHelper::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pHubble->m_pCLRProxy)
		g_pHubble->m_pCLRProxy->HideMenuStripPopup();
	if (g_pHubble->m_pActiveHtmlWnd)
	{
		::PostMessage(g_pHubble->m_pActiveHtmlWnd->m_hWnd, WM_COSMOSMSG, 20190331, 0);
		g_pHubble->m_pActiveHtmlWnd = nullptr;
	}

	CQuasar* pQuasar = m_pGrid->m_pRootObj->m_pGridCommonData->m_pQuasar;
	if (pQuasar->m_pGalaxyCluster->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = pQuasar->m_pGalaxyCluster->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	else if (g_pHubble->m_pActiveAppProxy)
	{
		HWND hMenuWnd = g_pHubble->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	BOOL b = pQuasar->m_bDesignerState;
	if (m_pGrid->m_nViewType == BlankView && m_pGrid->m_strCnnID == _T(""))
		b = true;

	if ((m_pGrid->m_nViewType == TabGrid || m_pGrid->m_nViewType == Grid))
	{
		if (g_pHubble->m_pQuasar != m_pGrid->m_pGridCommonData->m_pQuasar)
			::SetFocus(m_hWnd);
		g_pHubble->m_pActiveGrid = m_pGrid;
		g_pHubble->m_bWinFormActived = false;
		return MA_ACTIVATE;
	}

	if (!m_bCreateExternal)
		Invalidate(true);
	else
	{
		if ((m_pGrid->m_nViewType != TabGrid && m_pGrid->m_nViewType != Grid))
		{
			if (g_pHubble->m_pQuasar != m_pGrid->m_pGridCommonData->m_pQuasar || g_pHubble->m_pActiveGrid != m_pGrid)
				::SetFocus(m_hWnd);
		}
	}
	g_pHubble->m_pActiveGrid = m_pGrid;
	g_pHubble->m_bWinFormActived = false;
	if (m_pGrid->m_pParentObj)
	{
		if (m_pGrid->m_pParentObj->m_nViewType & TabGrid)
			m_pGrid->m_pParentObj->m_pVisibleXMLObj = m_pGrid;
	}
	g_pHubble->m_pQuasar = m_pGrid->m_pGridCommonData->m_pQuasar;

	CWebPage* pHtmlWnd = g_pHubble->m_pQuasar->m_pWebPageWnd;
	CString strID = m_pGrid->m_strName;

	if (m_pGrid->m_nViewType == CLRCtrl)
	{
		if (pHtmlWnd)
		{
			CWormhole* pSession = m_pGrid->m_pHubbleCloudSession;
			if (pSession)
			{
				pSession->InsertString(_T("msgID"), IPC_NODE_ONMOUSEACTIVATE_ID);
				pHtmlWnd->m_pChromeRenderFrameHost->SendHubbleMessage(pSession->m_pSession);
			}
		}
		return MA_NOACTIVATE;
	}

	if (b && m_bCreateExternal == false 
		&& m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS)
		&&m_pGrid->m_pDisp == NULL)
	{
		if (g_pHubble->m_pDesignGrid && g_pHubble->m_pDesignGrid != m_pGrid)
		{
			CGridHelper* pWnd = ((CGridHelper*)g_pHubble->m_pDesignGrid->m_pHostWnd);
			if (pWnd && ::IsWindow(pWnd->m_hWnd))
			{
				pWnd->Invalidate(true);
			}
		}
		g_pHubble->m_pDesignGrid = m_pGrid;
		Invalidate(true);
	}

	if (m_bCreateExternal == false)
	{
		if (pHtmlWnd)
		{
			IPCMsg pIPCInfo;
			pIPCInfo.m_strId = IPC_NODE_ONMOUSEACTIVATE_ID;
			pIPCInfo.m_strParam1 = strID;
			CString strHandle = _T("");
			strHandle.Format(_T("%d"), m_hWnd);
			pIPCInfo.m_strParam2 = strHandle;
			strHandle.Format(_T("%d"), m_pGrid->m_nViewType);
			pIPCInfo.m_strParam3 = strHandle;
			strHandle.Format(_T("%d"), pQuasar->m_hWnd);
			pIPCInfo.m_strParam4 = strHandle;
			//strHandle.Format(_T("%d"), m_pRootObj->m_pHostWnd->m_hWnd);
			pIPCInfo.m_strParam5 = _T("wndnode");
			pHtmlWnd->m_pChromeRenderFrameHost->SendHubbleMessage(&pIPCInfo);
		}
		return MA_ACTIVATEANDEAT;
	}
	else
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CGridHelper::OnEraseBkgnd(CDC* pDC)
{
	if (m_pGrid->m_nViewType != BlankView)
		return true;
	CQuasar* pQuasar = m_pGrid->m_pGridCommonData->m_pQuasar;
	BOOL bInDesignState = pQuasar->m_bDesignerState;
	CBitmap bit;
	RECT rt;
	GetClientRect(&rt);
	if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		HWND hWnd = pQuasar->m_hWnd;
		if (::IsWindow(hWnd) && !::IsWindowVisible(hWnd))
		{
			//pQuasar->HostPosChanged();
			return false;
		}
	}

	if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pGrid->m_pDisp == NULL) && m_bEraseBkgnd)
	{
		CString strText = _T("");
		bit.LoadBitmap(IDB_BITMAP_DESIGNER);
		CBrush br(&bit);
		pDC->FillRect(&rt, &br);
		if (bInDesignState && g_pHubble->m_pDesignGrid == m_pGrid)
		{
			pDC->SetTextColor(RGB(255, 0, 255));
			strText = _T("\n\n  ") + g_pHubble->m_strGridSelectedText;
		}
		else
		{
			CComBSTR bstrCaption(L"");
			m_pGrid->get_Attribute(CComBSTR(L"caption"), &bstrCaption);
			CString strInfo = _T("\n\n  ");
			if (bInDesignState)
			{
				strInfo = strInfo + g_pHubble->m_strDesignerTip1;
			}
			strInfo = strInfo + _T("\n  ") + g_pHubble->m_strDesignerTip2;
			strText.Format(strInfo, m_pGrid->m_strName, CString(OLE2T(bstrCaption)));
			pDC->SetTextColor(RGB(255, 255, 255));
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strText, &rt, DT_LEFT);
	}
	return true;
}

BOOL CGridHelper::PreTranslateMessage(MSG* pMsg)
{
	if (m_pOleInPlaceActiveObject)
	{
		LRESULT hr = m_pOleInPlaceActiveObject->TranslateAccelerator((LPMSG)pMsg);
		if (hr == S_OK)
			return true;
		else
		{
			if (m_pGrid->m_nViewType == CLRCtrl)
			{
				VARIANT_BOOL bShiftKey = false;
				if (::GetKeyState(VK_SHIFT) < 0)  // shift pressed
					bShiftKey = true;
				hr = g_pHubble->m_pCLRProxy->ProcessCtrlMsg(::GetWindow(m_hWnd, GW_CHILD), bShiftKey ? true : false);
				if (hr == S_OK)
					return true;
				return false;
			}
			else
			{
				pMsg->hwnd = 0;
				return true;
			}
		}
	}
	else
	{
		if (m_pGrid->m_nViewType == CLRCtrl)
		{
			if (g_pHubble->m_pCLRProxy->ProcessFormMsg(m_hWnd, pMsg, 0))
				return true;
			return false;
		}
	}
	if (IsDialogMessage(pMsg))
		return true;
	return CWnd::PreTranslateMessage(pMsg);
}

void CGridHelper::OnDestroy()
{
	if (g_pHubble->m_pDesignGrid == m_pGrid)
	{
		g_pHubble->m_pDesignGrid = NULL;
	}

	m_pGrid->Fire_Destroy();
	CWnd::OnDestroy();
}

void CGridHelper::PostNcDestroy()
{
	delete m_pGrid;
	CWnd::PostNcDestroy();
	delete this;
}

LRESULT CGridHelper::OnTabChange(WPARAM wParam, LPARAM lParam)
{
	int nOldPage = m_pGrid->m_nActivePage;
	m_pGrid->m_nActivePage = (int)wParam;
	IGrid* pGrid = nullptr;
	m_pGrid->GetGrid(0, wParam, &pGrid);

	CQuasar* pQuasar = m_pGrid->m_pGridCommonData->m_pQuasar;
	if (pGrid)
	{
		CGrid* _pGrid = (CGrid*)pGrid;
		if (_pGrid->m_nViewType == Grid)
		{
			((CSplitterWnd*)_pGrid->m_pHostWnd)->RecalcLayout();
		}
		else
		{
			pQuasar->HostPosChanged();
		}
		if (_pGrid->m_pWebBrowser) {
			g_pHubble->m_pActiveHtmlWnd = _pGrid->m_pWebBrowser->m_pVisibleWebWnd;
		}
		if (nOldPage != wParam)
		{
			::PostMessage(pQuasar->m_hWnd, WM_TANGRAMACTIVEPAGE, wParam, lParam);
			::SendMessage(_pGrid->m_pHostWnd->m_hWnd, WM_TANGRAMACTIVEPAGE, wParam, lParam);
		}
		if (m_pGrid->m_nViewType == TabGrid)
		{
			for (auto it : m_pGrid->m_vChildNodes)
			{
				CGrid* pGrid = it;
				if (pGrid->m_nCol != m_pGrid->m_nActivePage)
				{
					pGrid->m_pHostWnd->ShowWindow(SW_HIDE);
				}
			}
		}
		HWND h = ::GetParent(m_hWnd);
		if (h)
		{
			LRESULT lRes = ::SendMessage(h, WM_TANGRAMGETNODE, 0, 0);
			if (lRes)
			{
				CGrid* pRetNode = (CGrid*)lRes;
				if (pRetNode && pRetNode->m_nViewType == Grid)
				{
					pRetNode->m_pGridCommonData->m_pQuasar->HostPosChanged();
				}
			}
		}
	}
	if (nOldPage != wParam)
	{
		m_pGrid->Fire_TabChange(wParam, lParam);
		if (pQuasar->m_pWebPageWnd)
		{
			::SendMessage(::GetParent(pQuasar->m_pWebPageWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
		}
	}

	LRESULT lRes = CWnd::DefWindowProc(WM_TABCHANGE, wParam, lParam);
	return lRes;
}

LRESULT CGridHelper::OnHubbleMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0 && lParam)//Create CLRCtrl Node
	{
		switch (lParam)
		{
		case 19920612:
		case 20200609:
		case 20200606:
		case 20200531:
		case 19631222:
		case 20191031:
		case 20180115:
			return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
			break;
		case 20200128:
		{
			if (m_pGrid && m_pGrid->m_pWebBrowser)
			{
				::SetWindowPos(m_pGrid->m_pWebBrowser->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW);
			}
		}
		break;
		default:
		break;
		}
		return 0;
	}
	if (lParam == 20191031 || lParam == 20200609|| lParam == 20200606)
		return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
	if (lParam == 20200208)
		return 0;
	return -1;
}

LRESULT CGridHelper::OnActiveTangramObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrid->m_nViewType == CLRCtrl)
		::SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	//m_pGrid->m_pGridCommonData->m_pQuasar->HostPosChanged();
	//::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return CWnd::DefWindowProc(WM_TGM_SETACTIVEPAGE, wParam, lParam);
}

LRESULT CGridHelper::OnGetHubbleObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrid)
	{
		HWND hBrowser = (HWND)lParam;
		if (::IsWindow(hBrowser))
		{
			auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
			if (it != g_pHubble->m_mapBrowserWnd.end())
			{
				m_pGrid->m_pWebBrowser = (CBrowser*)it->second;
			}
		}
		return (LRESULT)m_pGrid;
	}
	return CWnd::DefWindowProc(WM_TANGRAMGETNODE, wParam, lParam);;
}

LRESULT CGridHelper::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_bCreateExternal)
	{
		switch (message)
		{
		case WM_ACTIVATE:
		case WM_ERASEBKGND:
		case WM_SETFOCUS:
		case WM_CONTEXTMENU:
		case WM_LBUTTONDOWN:
			//case WM_NOTIFY:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONDBLCLK:
		{
			WNDPROC* lplpfn = GetSuperWndProcAddr();
			return CallWindowProc(*lplpfn, m_hWnd, message, wParam, lParam);
		}
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CGridHelper::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
	if (m_pGrid && m_pGrid->m_nViewType == CLRCtrl && m_pGrid->m_hHostWnd)
	{
		if (m_bNoMove)
		{
			lpwndpos->flags |= SWP_NOMOVE;
			CGridHelper* pGridWnd = (CGridHelper*)m_pGrid->m_pHostWnd;
			if (m_pGrid->m_pParentObj->m_nViewType == Grid)
			{
				CGridWnd* pWnd = (CGridWnd*)m_pGrid->m_pParentObj->m_pHostWnd;
				::PostMessage(pWnd->m_hWnd, WM_COSMOSMSG, 3, 1993);
			}
		}
	}
	if (m_pGrid && m_pGrid->m_hHostWnd)
	{
		if (m_pGrid->m_nViewType == CLRCtrl)
		{
			if (m_bNoMove)
			{
				m_bNoMove = false;
			}
			return;
		}
		else if (m_pGrid->m_nViewType == BlankView)
		{
			::SetWindowPos(m_pGrid->m_hHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
			if (m_pGrid->m_hChildHostWnd)
				::SetWindowPos(m_pGrid->m_hChildHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
	}

	if (m_pGrid->m_pWebBrowser)
	{
		::SetWindowPos(m_pGrid->m_pWebBrowser->m_hWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE);// | SWP_NOREDRAW);
	}
	else {
		if (m_hFormWnd)
			::SetWindowPos(m_hFormWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		else if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
		{
			m_pGrid->m_pGridCommonData->m_pQuasar->HostPosChanged();
		}
	}
	if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pGrid->m_pDisp == NULL) && m_pGrid != m_pGrid->m_pRootObj)
	{
		return;
	}

	if (m_bCreateExternal)// || m_pGrid == m_pGrid->m_pRootObj)
	{
		Invalidate(true);
	}
}
