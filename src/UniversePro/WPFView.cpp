/********************************************************************************
*					DOM Plus for Application - Version 1.1.6.35                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE MIT LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
********************************************************************************/
// WPFView.cpp : implementation file
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "WPFView.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "GridWnd.h"


// CWPFView

IMPLEMENT_DYNCREATE(CWPFView, CView)

CWPFView::CWPFView()
{
	m_RespondMouseActive = false;
	m_pXobj = nullptr;
	m_pCosmosWPFObj = nullptr;
}

CWPFView::~CWPFView()
{
	if (m_pXobj)
		delete m_pXobj;
}

BEGIN_MESSAGE_MAP(CWPFView, CView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CWPFView drawing

void CWPFView::OnDraw(CDC* pDC)
{
}

// CWPFView diagnostics

#ifdef _DEBUG
void CWPFView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CWPFView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWPFView message handlers

BOOL CWPFView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_pXobj = g_pCosmos->m_pActiveXobj;
	m_pXobj->m_pHostWnd = this;
	m_pXobj->m_nViewType = TangramWPFCtrl;
	m_pXobj->m_nID = nID;

	g_pCosmos->LoadCLR();
	m_pCosmosWPFObj = g_pCosmos->m_pCLRProxy->CreateWPFControl(m_pXobj, pParentWnd->m_hWnd, nID);
	if (m_pCosmosWPFObj&&::IsWindow(m_pCosmosWPFObj->m_hwndWPF))
	{
		BOOL bSubclass =  SubclassWindow(m_pCosmosWPFObj->m_hwndWPF);
		if (bSubclass)
		{
			m_pXobj->m_pDisp = m_pCosmosWPFObj->m_pDisp;
			m_pXobj->m_pXobjShareData->m_pGalaxy->m_mapWPFView[m_hWnd] = this;
			UINT nIndex = ::GetWindowLongPtr(::GetParent(m_hWnd), GWLP_USERDATA);
			if (nIndex == 1992)
				m_RespondMouseActive = true;
		}
		return bSubclass;
	}
	return false;
}

int CWPFView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pCosmos->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = g_pCosmos->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	g_pCosmos->m_pActiveXobj = m_pXobj;
	::SetFocus(m_hWnd);
	g_pCosmos->m_bWinFormActived = false;

	g_pCosmos->m_pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
	g_pCosmos->m_pGalaxy->HostPosChanged();
	g_pCosmos->m_pGalaxy->UpdateVisualWPFMap(::GetParent(m_hWnd),false);

	long nIndex = (long)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	if (m_RespondMouseActive||nIndex == 1963||m_pCosmosWPFObj->IsVisible()==false)
	{
		::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0);
		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		CWnd* pParent = this->GetParent();
		pParent->ScreenToClient(&rc);
		::SetWindowPos(m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + 1, SWP_DRAWFRAME);
		::SetWindowPos(m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_DRAWFRAME);
	}

	return MA_ACTIVATE;
}

void CWPFView::OnDestroy()
{
	CGalaxy* pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
	auto it = pGalaxy->m_mapWPFView.find(m_hWnd);
	if (it != pGalaxy->m_mapWPFView.end())
		pGalaxy->m_mapWPFView.erase(it);
	it = pGalaxy->m_mapVisibleWPFView.find(m_hWnd);
	if (it != pGalaxy->m_mapVisibleWPFView.end())
		pGalaxy->m_mapVisibleWPFView.erase(it);

	g_pCosmos->m_pCLRProxy->WindowDestroy(m_hWnd);
	CView::OnDestroy();
}

BOOL CWPFView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_TAB)
		{
			//CView::PreTranslateMessage(pMsg);
			return true;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}

void CWPFView::OnMouseMove(UINT nFlags, CPoint point)
{
	long nIndex = (long)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	if (nIndex == 1963)
	{
		g_pCosmos->m_pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
		g_pCosmos->m_pGalaxy->HostPosChanged();
		g_pCosmos->m_pGalaxy->UpdateVisualWPFMap(::GetParent(m_hWnd), false);
		::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0);

		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		CWnd* pParent = this->GetParent();
		pParent->ScreenToClient(&rc);
		::SetWindowPos(m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + 1, SWP_DRAWFRAME);
		::SetWindowPos(m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_DRAWFRAME);
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CWPFView::OnEraseBkgnd(CDC* pDC)
{
	long nIndex = (long)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	if (nIndex!=1963)
	{
		CGalaxy* pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
		pGalaxy->UpdateVisualWPFMap(::GetParent(pGalaxy->m_hWnd), false);
	}
	//pGalaxy->HostPosChanged();

	return true;
	//return CView::OnEraseBkgnd(pDC);
}
