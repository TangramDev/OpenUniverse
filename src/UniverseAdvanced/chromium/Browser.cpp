/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202104110061           *
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
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

#include "../stdafx.h"
#include "../UniverseApp.h"
#include "../Xobj.h"
#include "../XobjWnd.h"
#include "../Galaxy.h"
#include "WebPage.h"
#include "Browser.h"

namespace Browser {
	CBrowser::CBrowser() {
		m_hDrawWnd = 0;
		m_hOldTab = NULL;
		m_bTabChange = false;
		m_pRemoteXobj = nullptr;
		m_pBrowser = nullptr;
		m_pParentXobj = nullptr;
		m_fdevice_scale_factor = 1.0f;
		m_strCurKey = _T("");
		m_pVisibleWebView = nullptr;
		m_pOmniboxViewViews = nullptr;
		if (g_pCosmos->m_pCreatingOmniboxViewViews) {
			m_pOmniboxViewViews = g_pCosmos->m_pCreatingOmniboxViewViews;
			g_pCosmos->m_pCreatingOmniboxViewViews = nullptr;
		}
	}

	CBrowser::~CBrowser() {}

	void CBrowser::BeforeActiveChromeTab(HWND hOldWnd)
	{
		m_bSZMode = true;
		m_hBeforeChange = hOldWnd;
		m_bTabChange = true;
		g_pCosmos->m_mapSizingBrowser[m_hWnd] = this;
		//if (::GetParent(m_hWnd))
		m_bInTabChange = true;
	}

	void CBrowser::EndActiveChromeTab(HWND hActive)
	{
		if (m_bDestroy == false)
		{
			auto it = g_pCosmos->m_mapWebView.find(hActive);
			if (it != g_pCosmos->m_mapWebView.end())
			{
				m_bInTabChange = true;
				((CWebView*)it->second)->m_bCanShow = false;
			}
			::PostMessage(m_hWnd, WM_COSMOSMSG, 20210314, (LPARAM)hActive);
		}
	}

	//void CBrowser::ActiveChromeTab(HWND hActive, HWND hOldWnd)
	//{
	//	m_bTabChange = true;
	//	m_bSZMode = true;
	//}

	void CBrowser::UpdateContentRect(HWND hWnd, RECT& rc, int nTopFix) {
		if (m_hBeforeChange && hWnd == m_hBeforeChange)
		{
			return;
		}
		else
		{
			m_hBeforeChange = nullptr;
			if (m_bInTabChange)
			{
				::PostMessageW(m_hWnd, WM_BROWSERLAYOUT, 0, 1000);
				m_bInTabChange = false;
				return;
			}
			m_bInTabChange = false;
		}
		if (m_bDestroy || m_bInTabChange || hWnd == 0 || g_pCosmos->m_bChromeNeedClosed == TRUE || g_pCosmos->m_bOMNIBOXPOPUPVISIBLE)
		{
			return;
		}

		auto it = g_pCosmos->m_mapWebView.find(hWnd);
		if (it != g_pCosmos->m_mapWebView.end())
		{
			m_pVisibleWebView = (CWebView*)it->second;
		}
		else
		{
			m_pVisibleWebView = new CComObject<CWebView>();
			m_pVisibleWebView->SubclassWindow(hWnd);
			g_pCosmos->m_mapWebView[hWnd] = m_pVisibleWebView;
		}

		if (::IsWindowVisible(hWnd) == false)
		{
			if (::IsWindowVisible(m_hWnd))
			{
				if (m_pBrowser->GetActiveWebContentWnd() == hWnd)
				{
					m_pVisibleWebView->m_pChromeRenderFrameHost->ShowWebPage(true);
					if (m_pVisibleWebView->m_hExtendWnd)
						::SetParent(m_pVisibleWebView->m_hExtendWnd, m_hWnd);
					::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 7);
					return;
				}
				else
					return;
			}
			else
				return;
		}

		if (m_hOldTab)
		{
			RECT rc;
			::GetWindowRect(m_hOldTab, &rc);
			ScreenToClient(&rc);
			::SetWindowPos(m_hOldTab, HWND_BOTTOM, rc.left, rc.top, 1, 1, SWP_NOREDRAW | SWP_NOACTIVATE);
			m_hOldTab = NULL;
		}

		if (m_pCosmosFrameWndInfo && m_pCosmosFrameWndInfo->m_nFrameType == 2)
		{
			if (m_pVisibleWebView == m_pCosmosFrameWndInfo->m_pWebPage)
				if (m_pVisibleWebView->m_bCanShow == false)
					return;
		}

		if (m_bTabChange == true)
		{
			::PostMessage(m_hWnd, WM_COSMOSMSG, 20200205, 1);
			return;
		}

		BrowserLayout();
		if (m_pVisibleWebView)
		{
			HWND hExtendWnd = m_pVisibleWebView->m_hExtendWnd;
			if (hExtendWnd == nullptr)
			{
				hExtendWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, m_hWnd, NULL, theApp.m_hInstance, NULL);
				m_pVisibleWebView->m_hExtendWnd = hExtendWnd;
				m_pVisibleWebView->m_hChildWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hExtendWnd, (HMENU)2, theApp.m_hInstance, NULL);
				CExtendWnd* pExtendWnd = new CExtendWnd();
				pExtendWnd->m_pHostPage = m_pVisibleWebView;
				pExtendWnd->SubclassWindow(m_pVisibleWebView->m_hChildWnd);
				::SetWindowLongPtr(hExtendWnd, GWLP_USERDATA, (LONG_PTR)m_pVisibleWebView->m_hChildWnd);
				::SetWindowLongPtr(m_pVisibleWebView->m_hChildWnd, GWLP_USERDATA, (LONG_PTR)m_pVisibleWebView);
			}
			if (::IsChild(hWnd, hExtendWnd))
				::SetParent(hExtendWnd, m_hWnd);

			if (m_pVisibleWebView->m_strCurKey == _T(""))
			{
				::SetWindowPos(hExtendWnd, m_hDrawWnd,
					rc.left,
					nTopFix * m_fdevice_scale_factor,
					0,
					0,
					SWP_SHOWWINDOW | SWP_NOREDRAW | SWP_NOACTIVATE);
				return;
			}
			::SetWindowPos(hExtendWnd, m_hDrawWnd,
				rc.left,
				nTopFix * m_fdevice_scale_factor,
				rc.right * m_fdevice_scale_factor,
				(rc.bottom - rc.top) * m_fdevice_scale_factor,
				SWP_SHOWWINDOW | SWP_NOREDRAW | SWP_NOACTIVATE);
			if (theApp.m_bAppStarting)
				return;
			HWND hWebHostWnd = m_pVisibleWebView->m_hWebHostWnd;
			if (hWebHostWnd == NULL)
				hWebHostWnd = m_pVisibleWebView->m_hChildWnd;
			if (::IsWindowVisible(hWebHostWnd) == false) {
				rc.right = rc.left + 1;
				rc.bottom = rc.top + 1;
			}
			else {
				RECT rc2;
				::GetWindowRect(hWebHostWnd, &rc2);
				if (::ScreenToClient(hExtendWnd, (LPPOINT)&rc2))
				{
					::ScreenToClient(hExtendWnd, ((LPPOINT)&rc2) + 1);
					RECT rect;
					::GetClientRect(hExtendWnd, &rect);
					if (m_bSZMode)
					{
						rc.right = rc.left + 1;
						rc.bottom = rc.top + 1;
					}
					else
					{
						rc.left += rc2.left / m_fdevice_scale_factor;
						rc.right -= (rect.right - rc2.right) / m_fdevice_scale_factor;
						rc.top += (rc2.top - rect.top) / m_fdevice_scale_factor;
						rc.bottom -= (rect.bottom - rc2.bottom) / m_fdevice_scale_factor;
						if (rc.right <= rc.left || rc.bottom <= rc.top)
						{
							rc.right = rc.left + 1;
							rc.bottom = rc.top + 1;
						}
						if (m_pParentXobj && g_pCosmos->m_bChromeNeedClosed == false && m_bDestroy == false)
						{
							HWND hPWnd = m_pParentXobj->m_pXobjShareData->m_pGalaxy->m_hWnd;
							if (m_pCosmosFrameWndInfo && m_pCosmosFrameWndInfo->m_nFrameType == 1)
							{
								if (m_OldRect.left != rc.left || m_OldRect.top != rc.top || m_OldRect.right != rc.right || m_OldRect.bottom != rc.bottom)
								{
									g_pCosmos->m_mapSizingBrowser[m_hWnd] = this;
									::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)hPWnd, 20210309);
								}
							}
						}
						m_OldRect = rc;
					}
				}
			}
		}
	};

	LRESULT CBrowser::BrowserLayout() {
		if (theApp.m_bAppStarting == false)
		{
			TRACE(_T(""));
		}
		if (m_bInTabChange || m_bDestroy || m_pVisibleWebView == nullptr || !::IsWindowVisible(m_hWnd) ||
			g_pCosmos->m_bChromeNeedClosed == TRUE)
			return 0;
		if (!::IsWindow(m_hWnd))
			return 0;
		if (m_pCosmosFrameWndInfo && m_pCosmosFrameWndInfo->m_nFrameType == 2)
		{
			auto it = g_pCosmos->m_mapMDIParent.find(::GetParent(m_pCosmosFrameWndInfo->m_hClient));
			if (it != g_pCosmos->m_mapMDIParent.end())
			{
				if (it->second->m_bCreateNewDoc)
				{
					m_bInTabChange = true;
					return 0;
				}
			}
			if (m_pVisibleWebView == m_pCosmosFrameWndInfo->m_pWebPage)
			{
				if (m_pVisibleWebView->m_bCanShow == false)
					return 0;
			}
		}
		RECT rcBrowser;
		GetClientRect(&rcBrowser);
		if (theApp.m_bAppStarting || m_pVisibleWebView->m_pGalaxy == nullptr || m_pVisibleWebView->m_strCurKey == _T("")) {
			if (rcBrowser.right * rcBrowser.left)
				::SetWindowPos(m_pVisibleWebView->m_hExtendWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_SHOWWINDOW);
			::SetWindowRgn(m_hDrawWnd, NULL, true);
			return 0;
		}
		if (!::IsWindowVisible(m_pVisibleWebView->m_hWnd))
		{
			if (m_pVisibleWebView->m_hExtendWnd)
				::SetParent(m_pVisibleWebView->m_hExtendWnd, m_pVisibleWebView->m_hWnd);
			auto it = g_pCosmos->m_mapWebView.find(m_pBrowser->GetActiveWebContentWnd());
			if (it != g_pCosmos->m_mapWebView.end())
			{
				m_pVisibleWebView = (CWebView*)it->second;
				if (m_pVisibleWebView->m_hExtendWnd)
					::SetParent(m_pVisibleWebView->m_hExtendWnd, m_hWnd);
			}
		}

		if (::GetParent(m_pVisibleWebView->m_hExtendWnd) != m_hWnd) {
			::SetParent(m_pVisibleWebView->m_hExtendWnd, m_hWnd);
		}

		RECT rcWebPage;
		RECT rcExtendWnd;
		::GetWindowRect(m_pVisibleWebView->m_hExtendWnd, &rcExtendWnd);
		::ScreenToClient(m_hWnd, (LPPOINT)&rcExtendWnd);
		::ScreenToClient(m_hWnd, ((LPPOINT)&rcExtendWnd) + 1);
		HWND _hWebPage = m_pVisibleWebView->m_hWnd;
		if (m_pVisibleWebView->m_pDevToolWnd)
		{
			if (::GetParent(m_pVisibleWebView->m_hWnd) == ::GetParent(m_pVisibleWebView->m_pDevToolWnd->m_hWnd))
			{
				_hWebPage = m_pVisibleWebView->m_pDevToolWnd->m_hWnd;
				::ShowWindow(_hWebPage, SW_SHOW);
			}
		}
		::GetWindowRect(_hWebPage, &rcWebPage);
		::ScreenToClient(m_hWnd, (LPPOINT)&rcWebPage);
		::ScreenToClient(m_hWnd, ((LPPOINT)&rcWebPage) + 1);
		//浏览器窗口区域：
		HRGN hGPUWndRgn = ::CreateRectRgn(rcBrowser.left, rcBrowser.top, rcBrowser.right, rcBrowser.bottom);
		//浏览器页面扩展窗口区域：
		HRGN hWebExtendWndRgn = ::CreateRectRgn(rcExtendWnd.left, rcExtendWnd.top, rcExtendWnd.right, rcExtendWnd.bottom);
		//浏览器页面窗口区域：
		HRGN hWebPage = NULL;
		if (m_bSZMode == false)
			hWebPage = ::CreateRectRgn(rcWebPage.left, rcWebPage.top, rcWebPage.right, rcWebPage.bottom);
		if (hWebPage)
			::CombineRgn(hWebExtendWndRgn, hWebExtendWndRgn, hWebPage, RGN_DIFF);
		::CombineRgn(hGPUWndRgn, hGPUWndRgn, hWebExtendWndRgn, RGN_DIFF);
		if (hWebPage)
			::DeleteObject(hWebPage);
		::DeleteObject(hWebExtendWndRgn);
		::SetWindowRgn(m_hDrawWnd, hGPUWndRgn, false);

		return 0;
	}

	LRESULT CBrowser::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		if (m_bDestroy)
			return lRes;
		if (LOWORD(wParam) != WA_INACTIVE) {
			if (m_pBrowser) {
				g_pCosmos->m_pActiveBrowser = m_pBrowser;
				g_pCosmos->m_pActiveBrowser->m_pProxy = this;
			}
		}
		return lRes;
	}

	LRESULT CBrowser::OnDeviceScaleFactorChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		m_fdevice_scale_factor = (float)lParam / 100;
		return lRes;
	}

	LRESULT CBrowser::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		if (m_pVisibleWebView)
		{
			g_pCosmos->m_pActiveHtmlWnd = m_pVisibleWebView;
			if (g_pCosmos->m_pActiveHtmlWnd && g_pCosmos->m_pActiveHtmlWnd->m_pChromeRenderFrameHost)
			{
				g_pCosmos->m_pGalaxy = nullptr;
				g_pCosmos->m_bWinFormActived = false;
			}
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CBrowser::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		if (m_bDestroy)
			return lRes;
		HWND hWnd = (HWND)lParam;
		switch (wParam) {
		case 0: {
			if (g_pCosmos->m_bCreatingDevTool)
			{
				g_pCosmos->m_pHtmlWndCreated = new CComObject<CWebView>;
				g_pCosmos->m_pHtmlWndCreated->SubclassWindow(hWnd);
				if (g_pCosmos->m_pCLRProxy)
					g_pCosmos->m_pCLRProxy->OnWebPageCreated(hWnd, (CWebPageImpl*)g_pCosmos->m_pHtmlWndCreated, (IWebPage*)g_pCosmos->m_pHtmlWndCreated, 0);
				g_pCosmos->m_bCreatingDevTool = false;
				g_pCosmos->m_pHtmlWndCreated->m_bDevToolWnd = true;
				if (m_pVisibleWebView) {
					m_pVisibleWebView->m_pDevToolWnd = g_pCosmos->m_pHtmlWndCreated;
					g_pCosmos->m_pHtmlWndCreated->m_pWebWnd = m_pVisibleWebView;
				}
				g_pCosmos->m_pActiveHtmlWnd = m_pVisibleWebView;
				g_pCosmos->m_pGalaxy = nullptr;
				g_pCosmos->m_bWinFormActived = false;
				g_pCosmos->m_pHtmlWndCreated->m_bCanShow = true;
				m_mapChildPage[hWnd] = g_pCosmos->m_pHtmlWndCreated;
				::PostMessage(hWnd, WM_COSMOSMSG, 20190331, 1);
			}
		} break;
		case 1:
		{
			if (lParam == 20200115)
			{
				::InvalidateRect(m_hWnd, nullptr, true);
				m_pBrowser->LayoutBrowser();
				::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 2);
				::InvalidateRect(m_hWnd, nullptr, true);
			}
		}
		break;
		case 20201101:
		{
			m_hDrawWnd = (HWND)lParam;
			::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 5);
			return 0;
		}
		break;
		case 20210314://TabChange
		{
			m_pVisibleWebView->m_bCanShow = false;
			if (theApp.m_bAppStarting == false && m_pParentXobj)
			{
				m_bSZMode = true;
				g_pCosmos->m_mapSizingBrowser[m_hWnd] = this;

				if (m_pCosmosFrameWndInfo)
				{
					switch (m_pCosmosFrameWndInfo->m_nFrameType)
					{
					case 1:
					{
						if (!::IsChild(m_pParentXobj->m_pHostWnd->m_hWnd, m_hWnd))
							::SetParent(m_hWnd, m_pParentXobj->m_pHostWnd->m_hWnd);
					}
					break;
					case 2:
						if (m_pVisibleWebView->m_pGalaxy)
						{
							m_pVisibleWebView->m_bCanShow = true;
							IXobj* pObj = nullptr;
							m_pVisibleWebView->Observe(CComBSTR(m_pVisibleWebView->m_pGalaxy->m_strCurrentKey), CComBSTR(""), &pObj);
							::PostMessage(m_pParentXobj->m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20210315);
						}
						break;
					}
				}
				if (m_pParentXobj->m_pParentWinFormWnd)
				{
					CGalaxy* pGalaxy = m_pParentXobj->m_pParentWinFormWnd->m_pClientGalaxy;
					if (pGalaxy && pGalaxy->m_pWorkXobj)
					{
						pGalaxy->m_pBindingXobj = pGalaxy->m_pWorkXobj->GetVisibleChildByName(_T("mdiclient"));
					}
				}
			}
			g_pCosmos->m_mapSizingBrowser[m_hWnd] = this;
			if (m_pParentXobj == nullptr)
				::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 7, 7);
			else
			{
				if (m_pVisibleWebView->m_strLoadingURLs == _T("") && g_pCosmos->m_hWaitTabWebPageWnd == NULL && g_pCosmos->m_nWaitTabCounts == 0)
				{
					::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 7, 7);
				}
				else
					::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 7);
			}
		}
		break;
		case 20190527:
		{
			return (LRESULT)((IBrowser*)this);
		}
		break;
		case 20200205:
		{
			if (lParam == 1)
			{
				m_bTabChange = false;
			}
			return 1;
		}
		break;
		case 1992:
		{
			return (LRESULT)m_pBrowser->GetBrowser();
		}
		break;
		case 20190629:
		{
			switch (lParam)
			{
			case 1:
				return m_pVisibleWebView && ::IsWindow(m_pVisibleWebView->m_hExtendWnd);
				break;
			}
		}
		break;
		}
		return lRes;
	}

	LRESULT CBrowser::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		m_bDestroy = true;
		if (g_pCosmos->m_pCLRProxy)
		{
			IBrowser* pIBrowser = nullptr;
			QueryInterface(__uuidof(IBrowser), (void**)&pIBrowser);
			g_pCosmos->m_pCLRProxy->OnDestroyChromeBrowser(pIBrowser);
		}

		m_pVisibleWebView = nullptr;
		auto it = g_pCosmos->m_mapBrowserWnd.find(m_hWnd);
		if (it != g_pCosmos->m_mapBrowserWnd.end()) {
			g_pCosmos->m_mapBrowserWnd.erase(it);
		}
		if (m_pParentXobj && m_pParentXobj->m_pParentWinFormWnd)
		{
			m_pParentXobj->m_pParentWinFormWnd->m_pOwnerHtmlWnd = nullptr;
		}
		if ((g_pCosmos->m_hMainWnd == g_pCosmos->m_hCosmosWnd && g_pCosmos->m_mapBrowserWnd.size() == 1) ||
			g_pCosmos->m_hHostBrowserWnd == m_hWnd)
		{
			if (g_pCosmos->m_hHostBrowserWnd == m_hWnd)
			{
				g_pCosmos->m_bChromeNeedClosed = true;
				for (auto it : g_pCosmos->m_mapBrowserWnd)
				{
					if (((CBrowser*)it.second)->m_hWnd != m_hWnd)
						((CBrowser*)it.second)->PostMessageW(WM_CLOSE, 0, 0);
				}
				g_pCosmos->m_mapBrowserWnd.erase(g_pCosmos->m_mapBrowserWnd.begin(), g_pCosmos->m_mapBrowserWnd.end());
			}
		}

		if ((g_pCosmos->m_hMainWnd == NULL && g_pCosmos->m_mapBrowserWnd.size() == 0) ||
			g_pCosmos->m_hHostBrowserWnd == m_hWnd) {
			if (g_pCosmos->m_hHostBrowserWnd == m_hWnd)
				g_pCosmos->m_hHostBrowserWnd = NULL;
			if (g_pCosmos->m_pCLRProxy)
			{
				if (g_pCosmos->m_pCosmosAppProxy)
					g_pCosmos->m_pCosmosAppProxy->OnCosmosClose();
			}

			if (g_pCosmos->m_hCBTHook) {
				UnhookWindowsHookEx(g_pCosmos->m_hCBTHook);
				g_pCosmos->m_hCBTHook = nullptr;
			}
		}

		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);

		if (g_pCosmos->m_hMainWnd == NULL &&
			g_pCosmos->m_bChromeNeedClosed == false &&
			g_pCosmos->m_mapBrowserWnd.size() == 1 &&
			::IsWindow(g_pCosmos->m_hHostBrowserWnd) &&
			g_pCosmos->m_nAppID == 10000 &&
			g_pCosmos->m_bEclipse == false)
		{
			::SendMessageW(g_pCosmos->m_hHostBrowserWnd, WM_CLOSE, 0, 0);
		}
		return lRes;
	}

	LRESULT CBrowser::OnChromeTabChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		if (m_pVisibleWebView && m_pVisibleWebView->m_pGalaxy)
		{
		}
		return lRes;
	}

	LRESULT CBrowser::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
		if (m_bInTabChange || m_bDestroy || !::IsWindowVisible(m_hWnd) || lpwndpos->flags == (SWP_NOSIZE | SWP_NOMOVE))
			return DefWindowProc(uMsg, wParam, lParam);
		HWND hPWnd = ::GetParent(m_hWnd);
		if (m_bSZMode)
		{
			g_pCosmos->m_mapSizingBrowser[m_hWnd] = this;
		}
		if (::IsWindow(hPWnd))
		{
			WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
			RECT rc;
			::GetClientRect(hPWnd, &rc);
			lpwndpos->x = -12;
			lpwndpos->y = -6;
			lpwndpos->cx = rc.right + 24;
			lpwndpos->cy = rc.bottom + 18;
			lpwndpos->flags |= SWP_NOREDRAW | SWP_NOACTIVATE;
		}
		else if (g_pCosmos->m_bOMNIBOXPOPUPVISIBLE)
			::SendMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 2);
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CBrowser::OnExitSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		m_bSZMode = false;
		for (auto& it : g_pCosmos->m_mapSizingBrowser)
		{
			if (::IsWindow(it.first))
			{
				it.second->m_bSZMode = false;
				it.second->m_pBrowser->LayoutBrowser();
			}
		}
		g_pCosmos->m_mapSizingBrowser.erase(g_pCosmos->m_mapSizingBrowser.begin(), g_pCosmos->m_mapSizingBrowser.end());
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CBrowser::OnEnterSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		m_bSZMode = true;
		g_pCosmos->m_mapSizingBrowser[m_hWnd] = this;
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CBrowser::OnBrowserLayout(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		if (m_bInTabChange || m_bDestroy)
		{
			if (m_pParentXobj)
			{
				if (wParam == 7 && m_pMDIParent == nullptr)
				{
					m_bInTabChange = false;
					::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 1, 7);
				}
				else
					return lRes;
			}
		}
		if (g_pCosmos->m_bChromeNeedClosed == false && m_pVisibleWebView)
		{
			switch (lParam)
			{
			case 1000:
			{
				m_bTabChange = false;
				if (m_pParentXobj)
				{
					g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(QueryType::RecalcLayout, m_pParentXobj->m_pXobjShareData->m_pGalaxy->m_hWnd);
					m_pBrowser->LayoutBrowser();
				}
				if (::GetParent(m_hWnd) == nullptr)
				{
					HWND hWnd = m_pBrowser->GetActiveWebContentWnd();
					if (hWnd == m_pVisibleWebView->m_hWnd)
					{
						if (m_pVisibleWebView->m_pGalaxy)
						{
							m_pBrowser->LayoutBrowser();
							BrowserLayout();
						}
					}
					else
					{
						auto it = g_pCosmos->m_mapWebView.find(hWnd);
						if (it != g_pCosmos->m_mapWebView.end())
						{
							m_pVisibleWebView = (CWebView*)it->second;
						}
						else
						{
							m_pVisibleWebView = new CComObject<CWebView>();
							m_pVisibleWebView->SubclassWindow(hWnd);
							g_pCosmos->m_mapWebView[hWnd] = m_pVisibleWebView;
						}
					}
				}
				//if (m_pVisibleWebView->m_pGalaxy)
				//{
				//	::SendMessage(m_pVisibleWebView->m_hExtendWnd, WM_BROWSERLAYOUT, (WPARAM)m_pVisibleWebView->m_hChildWnd, 0);
				//	if (::GetParent(m_hWnd) == nullptr)
				//	{
				//		CXobj* pObj = m_pVisibleWebView->m_pGalaxy->m_pWorkXobj;
				//		if (pObj && pObj->m_nViewType == Grid)
				//		{
				//			CSplitterWnd* pWnd = (CSplitterWnd*)pObj->m_pHostWnd;
				//			pWnd->RecalcLayout();
				//		}
				//	}
				//}
				//m_bSZMode = false;
				if (theApp.m_bAppStarting == true)
				{
					theApp.m_bAppStarting = false;
					m_pVisibleWebView->m_bCanShow = true;
				}
			}
			break;
			case 8:
			{
				RECT rc;
				::GetClientRect(m_hWnd, &rc);
				if ((rc.right < rc.left) || (rc.bottom < rc.top))
				{
					::GetClientRect(::GetParent(m_hWnd), &rc);
					::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rc.right, rc.bottom, SWP_NOREDRAW | SWP_NOACTIVATE);
				}
			}
			break;
			case 1:
			{
				if (m_pBrowser)
				{
					m_pBrowser->LayoutBrowser();
					BrowserLayout();
				}
			}
			break;
			case 2:
			case 3:
			case 4:
			{
				if (lParam == 4)
				{
					BrowserLayout();
					m_pBrowser->LayoutBrowser();
				}
				else if (m_pBrowser)
				{
					if (!::IsWindowVisible(m_hWnd))
						::ShowWindow(m_hWnd, SW_SHOW);
					if (g_pCosmos->m_bOMNIBOXPOPUPVISIBLE)
					{
						BrowserLayout();
						m_pBrowser->LayoutBrowser();
					}
					m_bTabChange = false;
				}
			}
			break;
			case 5:
			{
				if (m_pBrowser)
				{
					m_bTabChange = false;
					m_pBrowser->LayoutBrowser();
				}
			}
			break;
			case 7:
			{
				if (m_pVisibleWebView == nullptr)
					break;
				switch (wParam)
				{
				case 7:
				{
					m_bInTabChange = false;
					if (m_pVisibleWebView)
						m_pVisibleWebView->m_bCanShow = true;
					::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 7);
					if (m_pParentXobj && m_pParentXobj->m_pParentWinFormWnd && m_pParentXobj->m_pParentWinFormWnd->m_hMDIClient)
					{
						::SendMessage(m_pParentXobj->m_pParentWinFormWnd->m_hMDIClient, WM_COSMOSMSG, 3, 20180115);
					}
				}
				break;
				case 3:
				{
					if (theApp.m_bAppStarting == false)
						m_pVisibleWebView->m_bCanShow = true;
				}
				break;
				case 2:
				{
					m_bTabChange = false;
					::SendMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 7);
					m_bSZMode = false;
					::PostAppMessage(::GetCurrentThreadId(), WM_BROWSERLAYOUT, (WPARAM)m_hWnd, 7);
				}
				break;
				case 1:
				{
					m_bTabChange = false;
					if (m_pMDIParent)
						m_pMDIParent->m_bCreateNewDoc = false;
					theApp.m_bAppStarting = false;
					m_pVisibleWebView->m_bCanShow = true;
					::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 7);
					m_bSZMode = false;
				}
				break;
				default:
				{
					if ((m_pMDIParent && m_pMDIParent->m_bCreateNewDoc) || !::IsWindowVisible(m_hWnd))
						break;
					if (/*m_pVisibleWebView->m_bCanShow == false ||*/ m_bTabChange || m_bInTabChange)
						break;
					HWND hWnd = m_pBrowser->GetActiveWebContentWnd();
					for (auto& it : m_mapChildPage)
					{
						if (::IsWindow(it.first))
						{
							if (it.first != hWnd)
							{
								if (it.second->m_pChromeRenderFrameHost)
									it.second->m_pChromeRenderFrameHost->ShowWebPage(false);
							}
							else
							{
								m_pVisibleWebView = it.second;
							}
						}
					}

					m_bSZMode = false;
					m_pBrowser->LayoutBrowser();
					BrowserLayout();
					if (m_pParentXobj)
					{
						HWND hWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(QueryType::RecalcLayout, m_pParentXobj->m_pXobjShareData->m_pGalaxy->m_hWnd);
					}

					if (m_pVisibleWebView && m_pVisibleWebView->m_pGalaxy)
					{
						::SendMessage(m_pVisibleWebView->m_hExtendWnd, WM_BROWSERLAYOUT, (WPARAM)m_pVisibleWebView->m_hChildWnd, 0);
						m_pVisibleWebView->m_pGalaxy->HostPosChanged();
						if (::GetParent(m_hWnd) == nullptr)
						{
							CXobj* pObj = m_pVisibleWebView->m_pGalaxy->m_pWorkXobj;
							if (pObj && pObj->m_nViewType == Grid)
							{
								CSplitterWnd* pWnd = (CSplitterWnd*)pObj->m_pHostWnd;
								pWnd->RecalcLayout();
							}
						}
					}
					if (m_pMDIParent)
					{
						::PostMessage(m_pMDIParent->m_hWnd, WM_QUERYAPPPROXY, 0, 19651965);
					}
				}
				break;
				}
			}
			break;
			}
		}
		return lRes;
	}

	void CBrowser::OnFinalMessage(HWND hWnd) {
		CWindowImpl::OnFinalMessage(hWnd);
		delete this;
	}

	STDMETHODIMP CBrowser::get_RemoteXobj(IXobj** pVal)
	{
		if (m_pRemoteXobj != nullptr)
		{
			*pVal = m_pRemoteXobj;
		}
		return S_OK;
	}

	STDMETHODIMP CBrowser::put_RemoteXobj(IXobj* newVal)
	{
		CComQIPtr<IXobj>pXobj(newVal);
		if (pXobj)
			m_pRemoteXobj = pXobj.Detach();
		//if (m_pRemoteNode == nullptr)
		//{
		//	IStream* pStream = 0;
		//	HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(IID_IXobj, (IXobj*)newVal.pdispVal, &pStream);
		//	if (hr == S_OK)
		//	{
		//		IDispatch* pTarget = nullptr;
		//		hr = ::CoGetInterfaceAndReleaseStream(pStream, IID_IDispatch, (LPVOID*)&pTarget);
		//		if (hr == S_OK && pTarget)
		//			hr = pTarget->QueryInterface(IID_IXobj, (void**)m_pRemoteNode);
		//	}
		//}
		return S_OK;
	}

	STDMETHODIMP CBrowser::AddURLs(BSTR bstrURLs)
	{
		if (m_pVisibleWebView)
		{
			CString strDisposition = _T("");
			strDisposition.Format(_T("%d"), NEW_BACKGROUND_TAB);
			if (m_pVisibleWebView->m_pChromeRenderFrameHost)
			{
				IPCMsg msg;
				msg.m_strId = L"ADD_URL";
				msg.m_strParam1 = OLE2T(bstrURLs);
				msg.m_strParam2 = strDisposition;
				m_pVisibleWebView->m_pChromeRenderFrameHost->SendCosmosMessage(&msg);
			}
		}
		return S_OK;
	}

	STDMETHODIMP CBrowser::OpenURL(BSTR bstrURL, BrowserWndOpenDisposition nDisposition, BSTR bstrKey, BSTR bstrXml)
	{
		if (m_pVisibleWebView)
		{
			CString strDisposition = _T("");
			strDisposition.Format(_T("%d"), nDisposition);
			if (m_pVisibleWebView->m_pChromeRenderFrameHost)
			{
				IPCMsg msg;
				msg.m_strId = L"OPEN_URL";
				msg.m_strParam1 = OLE2T(bstrURL);
				msg.m_strParam2 = strDisposition;
				m_pVisibleWebView->m_pChromeRenderFrameHost->SendCosmosMessage(&msg);
			}
		}
		return S_OK;
	}
}  // namespace Browser
