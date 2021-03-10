/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103100048
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser: Independent Browser Window and Browser Window
 *    as sub windows of other windows are supported in the application process;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

// XobjHelper.cpp : implementation file
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "Cosmos.h"
#include "XobjWnd.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "GridWnd.h"

// CXobjWnd

IMPLEMENT_DYNCREATE(CXobjWnd, CWnd)

CXobjWnd::CXobjWnd()
{
	m_hFormWnd = NULL;
	m_bNoMove = false;
	m_bCreateExternal = false;
	m_bEraseBkgnd = true;
	m_pXobj = nullptr;
	m_pParentXobj = nullptr;
	m_pOleInPlaceActiveObject = nullptr;
	m_strKey = m_strXml = _T("");
}

CXobjWnd::~CXobjWnd()
{
}

BEGIN_MESSAGE_MAP(CXobjWnd, CWnd)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_TABCHANGE, OnTabChange)
	ON_MESSAGE(WM_COSMOSMSG, OnCosmosMsg)
	ON_MESSAGE(WM_HUBBLE_GETNODE, OnGetCosmosObj)
	ON_MESSAGE(WM_TGM_SETACTIVEPAGE, OnActiveTangramObj)
	ON_MESSAGE(WM_SPLITTERREPOSITION, OnSplitterReposition)
END_MESSAGE_MAP()

// CXobjWnd diagnostics

#ifdef _DEBUG
void CXobjWnd::AssertValid() const
{
	///CView::AssertValid();
}

#ifndef _WIN32_WCE
void CXobjWnd::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif
#endif //_DEBUG

//CXobjWnd message handlers
BOOL CXobjWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_pXobj = g_pCosmos->m_pActiveXobj;
	m_pXobj->m_nID = nID;
	m_pXobj->m_pHostWnd = this;

	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		CGalaxy* pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
		pGalaxy->m_pBindingXobj = m_pXobj;

		m_pXobj->m_pXobjShareData->m_pHostClientView = this;
		CGalaxyCluster* pGalaxyCluster = pGalaxy->m_pGalaxyCluster;
		HWND hWnd = CreateWindow(L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);
		BOOL bRet = SubclassWindow(hWnd);
		if (m_pXobj->m_pParentObj)
		{
			if (m_pXobj->m_pParentObj->m_nViewType == Grid)
			{
				::PostMessage(::GetParent(m_hWnd), WM_HOSTNODEFORSPLITTERCREATED, m_pXobj->m_nRow, m_pXobj->m_nCol);
				ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
		}
		return bRet;
	}
	return m_pXobj->Create(dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CXobjWnd::OnSplitterReposition(WPARAM wParam, LPARAM lParam)
{
	switch (m_pXobj->m_nViewType)
	{
	case CLRCtrl:
	case ActiveX:
	case TabGrid:
		m_pXobj->m_pXobjShareData->m_pGalaxy->HostPosChanged();
		break;
	default:
		break;
	}

	return CWnd::DefWindowProc(WM_SPLITTERREPOSITION, wParam, lParam);
}

int CXobjWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pCosmos->m_pCLRProxy)
		g_pCosmos->m_pCLRProxy->HideMenuStripPopup();

	CGalaxy* pGalaxy = m_pXobj->m_pRootObj->m_pXobjShareData->m_pGalaxy;
	HWND hWnd = pGalaxy->m_pGalaxyCluster->m_hWnd;
	if (((::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)) || ::GetParent(hWnd) == NULL)
		::BringWindowToTop(hWnd);
	if (pGalaxy->m_pGalaxyCluster->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = pGalaxy->m_pGalaxyCluster->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	else if (g_pCosmos->m_pActiveAppProxy)
	{
		HWND hMenuWnd = g_pCosmos->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}

	if ((m_pXobj->m_nViewType == TabGrid || m_pXobj->m_nViewType == Grid))
	{
		if (g_pCosmos->m_pGalaxy != m_pXobj->m_pXobjShareData->m_pGalaxy)
			::SetFocus(m_hWnd);
		g_pCosmos->m_pActiveXobj = m_pXobj;
		g_pCosmos->m_bWinFormActived = false;
		return MA_ACTIVATE;
	}

	if (!m_bCreateExternal)
		Invalidate(true);
	else
	{
		if ((m_pXobj->m_nViewType != TabGrid && m_pXobj->m_nViewType != Grid))
		{
			if (g_pCosmos->m_pGalaxy != m_pXobj->m_pXobjShareData->m_pGalaxy || g_pCosmos->m_pActiveXobj != m_pXobj)
				::SetFocus(m_hWnd);
		}
	}
	g_pCosmos->m_pActiveXobj = m_pXobj;
	g_pCosmos->m_bWinFormActived = false;
	g_pCosmos->m_pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;

	if (m_pXobj->m_nViewType == CLRCtrl)
	{
		return MA_NOACTIVATE;
	}

	if (m_bCreateExternal == false)
	{
		return MA_ACTIVATEANDEAT;
	}
	else
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CXobjWnd::OnEraseBkgnd(CDC* pDC)
{
	if (m_pXobj->m_nViewType != BlankView)
		return true;
	CGalaxy* pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
	CBitmap bit;
	RECT rt;
	GetClientRect(&rt);
	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		HWND hWnd = pGalaxy->m_hWnd;
		if (::IsWindow(hWnd) && !::IsWindowVisible(hWnd))
		{
			//pGalaxy->HostPosChanged();
			return false;
		}
	}

	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pXobj->m_pDisp == NULL) && m_bEraseBkgnd)
	{
		CString strText = _T("");
		bit.LoadBitmap(IDB_BITMAP_DESIGNER);
		CBrush br(&bit);
		pDC->FillRect(&rt, &br);
		{
			CComBSTR bstrCaption(L"");
			m_pXobj->get_Attribute(CComBSTR(L"caption"), &bstrCaption);
			CString strInfo = _T("\n\n  ");
			strText.Format(strInfo, m_pXobj->m_strName, CString(OLE2T(bstrCaption)));
			pDC->SetTextColor(RGB(255, 255, 255));
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strText, &rt, DT_LEFT);
	}
	return true;
}

BOOL CXobjWnd::PreTranslateMessage(MSG* pMsg)
{
	if (m_pOleInPlaceActiveObject)
	{
		LRESULT hr = m_pOleInPlaceActiveObject->TranslateAccelerator((LPMSG)pMsg);
		if (hr == S_OK)
			return true;
		else
		{
			if (m_pXobj->m_nViewType == CLRCtrl)
			{
				VARIANT_BOOL bShiftKey = false;
				if (::GetKeyState(VK_SHIFT) < 0)  // shift pressed
					bShiftKey = true;
				hr = g_pCosmos->m_pCLRProxy->ProcessCtrlMsg(::GetWindow(m_hWnd, GW_CHILD), bShiftKey ? true : false);
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
		if (m_pXobj->m_nViewType == CLRCtrl)
		{
			if (g_pCosmos->m_pCLRProxy->ProcessFormMsg(m_hWnd, pMsg, 0))
				return true;
			return false;
		}
	}
	if (IsDialogMessage(pMsg))
		return true;
	return CWnd::PreTranslateMessage(pMsg);
}

void CXobjWnd::OnDestroy()
{
	m_pXobj->Fire_Destroy();
	CWnd::OnDestroy();
}

void CXobjWnd::PostNcDestroy()
{
	delete m_pXobj;
	CWnd::PostNcDestroy();
	delete this;
}

LRESULT CXobjWnd::OnTabChange(WPARAM wParam, LPARAM lParam)
{
	int nOldPage = m_pXobj->m_nActivePage;
	m_pXobj->m_nActivePage = (int)wParam;
	IXobj* pXobj = nullptr;
	m_pXobj->GetXobj(0, wParam, &pXobj);

	CGalaxy* pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
	if (pXobj)
	{
		CXobj* _pXobj = (CXobj*)pXobj;
		if (_pXobj->m_nViewType == Grid)
		{
			((CSplitterWnd*)_pXobj->m_pHostWnd)->RecalcLayout();
		}
		else
		{
			pGalaxy->HostPosChanged();
		}
		if (nOldPage != wParam)
		{
			::PostMessage(pGalaxy->m_hWnd, WM_HUBBLE_ACTIVEPAGE, wParam, lParam);
			::SendMessage(_pXobj->m_pHostWnd->m_hWnd, WM_HUBBLE_ACTIVEPAGE, wParam, lParam);
		}
		if (m_pXobj->m_nViewType == TabGrid)
		{
			for (auto it : m_pXobj->m_vChildNodes)
			{
				CXobj* pXobj = it;
				if (pXobj->m_nCol != m_pXobj->m_nActivePage)
				{
					pXobj->m_pHostWnd->ShowWindow(SW_HIDE);
				}
			}
		}
		HWND h = ::GetParent(m_hWnd);
		if (h)
		{
			LRESULT lRes = ::SendMessage(h, WM_HUBBLE_GETNODE, 0, 0);
			if (lRes)
			{
				CXobj* pRetNode = (CXobj*)lRes;
				if (pRetNode && pRetNode->m_nViewType == Grid)
				{
					pRetNode->m_pXobjShareData->m_pGalaxy->HostPosChanged();
				}
			}
		}
	}
	if (nOldPage != wParam)
	{
		m_pXobj->Fire_TabChange(wParam, lParam);
	}

	LRESULT lRes = CWnd::DefWindowProc(WM_TABCHANGE, wParam, lParam);
	return lRes;
}

LRESULT CXobjWnd::OnCosmosMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0 && lParam)//Create CLRCtrl Node
	{
		switch (lParam)
		{
		case 19631222:
		case 20180115:
			return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
			break;
		case 20200128:
		{
		}
		break;
		default:
		break;
		}
		return 0;
	}
	return -1;
}

LRESULT CXobjWnd::OnActiveTangramObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pXobj->m_nViewType == CLRCtrl)
		::SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	//m_pXobj->m_pXobjShareData->m_pGalaxy->HostPosChanged();
	//::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return CWnd::DefWindowProc(WM_TGM_SETACTIVEPAGE, wParam, lParam);
}

LRESULT CXobjWnd::OnGetCosmosObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pXobj)
	{
		return (LRESULT)m_pXobj;
	}
	return CWnd::DefWindowProc(WM_HUBBLE_GETNODE, wParam, lParam);;
}

LRESULT CXobjWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CXobjWnd::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
	if (m_pXobj && m_pXobj->m_nViewType == CLRCtrl && m_pXobj->m_hHostWnd)
	{
		if (m_bNoMove)
		{
			lpwndpos->flags |= SWP_NOMOVE;
			CXobjWnd* pXobjWnd = (CXobjWnd*)m_pXobj->m_pHostWnd;
			if (m_pXobj->m_pParentObj->m_nViewType == Grid)
			{
				CGridWnd* pWnd = (CGridWnd*)m_pXobj->m_pParentObj->m_pHostWnd;
				::PostMessage(pWnd->m_hWnd, WM_COSMOSMSG, 3, 1993);
			}
		}
	}
	if (m_pXobj && m_pXobj->m_hHostWnd)
	{
		if (m_pXobj->m_nViewType == CLRCtrl)
		{
			if (m_bNoMove)
			{
				m_bNoMove = false;
			}
			return;
		}
		else if (m_pXobj->m_nViewType == BlankView)
		{
			::SetWindowPos(m_pXobj->m_hHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
			if (m_pXobj->m_hChildHostWnd)
				::SetWindowPos(m_pXobj->m_hChildHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
	}

	{
		if (m_hFormWnd)
			::SetWindowPos(m_hFormWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		else if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
		{
			m_pXobj->m_pXobjShareData->m_pGalaxy->HostPosChanged();
		}
	}
	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pXobj->m_pDisp == NULL) && m_pXobj != m_pXobj->m_pRootObj)
	{
		return;
	}

	if (m_bCreateExternal)// || m_pXobj == m_pXobj->m_pRootObj)
	{
		Invalidate(true);
	}
}
