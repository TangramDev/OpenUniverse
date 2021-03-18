/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103160051           *
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
		m_pVisibleWebWnd = nullptr;
		m_pOmniboxViewViews = nullptr;
		if (g_pCosmos->m_pCreatingOmniboxViewViews) {
			m_pOmniboxViewViews = g_pCosmos->m_pCreatingOmniboxViewViews;
			g_pCosmos->m_pCreatingOmniboxViewViews = nullptr;
		}
	}

	CBrowser::~CBrowser() {}

	void CBrowser::BeforeActiveChromeTab(HWND hOldWnd)
	{

	}

	void CBrowser::EndActiveChromeTab(HWND hActive)
	{

	}

	void CBrowser::ActiveChromeTab(HWND hActive, HWND hOldWnd)
	{
		m_bTabChange = true;
		if (g_pCosmos->m_bChromeNeedClosed == false && m_pBrowser)
		{
			m_bSZMode = true;
			g_pCosmos->m_mapSizingBrowser[m_hWnd] = this;
			if (::IsWindow(hOldWnd))
			{
				m_hOldTab = hOldWnd;
			}

			if (m_pCosmosFrameWndInfo && m_pCosmosFrameWndInfo->m_nFrameType == 2)
			{
				auto it = g_pCosmos->m_mapHtmlWnd.find(hActive);
				if (it != g_pCosmos->m_mapHtmlWnd.end())
				{
					CWebPage* pPage = (CWebPage*)it->second;
					if (pPage->m_pGalaxy)
					{
						IXobj* pObj = nullptr;
						pPage->Observe(CComBSTR(pPage->m_pGalaxy->m_strCurrentKey), CComBSTR(""), &pObj);
					}
				}
			}
			else
			{
				if (m_pClientGalaxy)
				{
					m_pClientGalaxy->ModifyStyle(WS_CLIPCHILDREN, 0);
					g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(RecalcLayout, m_pClientGalaxy->m_hWnd);
					m_pClientGalaxy->ModifyStyle(0, WS_CLIPCHILDREN);
				}
			}
			::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 7);
		}
	}

	LRESULT CBrowser::OnChromeTabChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		if (m_bDestroy)
			return lRes;
		if (g_pCosmos->m_bChromeNeedClosed == false && m_pBrowser)
		{
			HWND hActive = m_pBrowser->GetActiveWebContentWnd();
			if (::GetParent(m_hWnd) && m_pVisibleWebWnd->m_hWnd != hActive)
			{
				auto it = m_mapChildPage.find(hActive);
				if (it != m_mapChildPage.end())
					m_pVisibleWebWnd = it->second;
			}
			g_pCosmos->m_pActiveHtmlWnd = m_pVisibleWebWnd;
			if (m_pVisibleWebWnd && g_pCosmos->m_pActiveHtmlWnd->m_pChromeRenderFrameHost)
			{
				g_pCosmos->m_pGalaxy = nullptr;
				g_pCosmos->m_bWinFormActived = false;
			}
			if (!m_pClientGalaxy)
				m_pBrowser->LayoutBrowser();
			::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 1, 7);
		}
		return lRes;
	}

	void CBrowser::UpdateContentRect(HWND hWnd, RECT& rc, int nTopFix) {
		if (m_bDestroy || hWnd == 0 || g_pCosmos->m_bChromeNeedClosed == TRUE || g_pCosmos->m_bOMNIBOXPOPUPVISIBLE)
		{
			return;
		}

		auto it = g_pCosmos->m_mapHtmlWnd.find(hWnd);
		if (it != g_pCosmos->m_mapHtmlWnd.end())
		{
			m_pVisibleWebWnd = (CWebPage*)it->second;
		}
		else
			return;

		if (::IsWindowVisible(hWnd) == false)
		{
			if (::IsWindowVisible(m_hWnd))
			{
				if (m_pBrowser->GetActiveWebContentWnd() == hWnd)
				{
					m_pVisibleWebWnd->m_pChromeRenderFrameHost->ShowWebPage(true);
					if (m_pVisibleWebWnd->m_hExtendWnd)
						::SetParent(m_pVisibleWebWnd->m_hExtendWnd, m_hWnd);
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

		if (m_bTabChange == true)
		{
			if (m_pVisibleWebWnd->m_pChromeRenderFrameHost)
			{
				m_pVisibleWebWnd->m_pChromeRenderFrameHost->ShowWebPage(true);
			}
			::PostMessage(m_hWnd, WM_COSMOSMSG, 20200205, 1);
			return;
		}
		if (m_pVisibleWebWnd && m_pVisibleWebWnd->m_pChromeRenderFrameHost && ::IsWindowVisible(hWnd))
		{
			m_pVisibleWebWnd->m_pChromeRenderFrameHost->ShowWebPage(true);
		}

		BrowserLayout();
		if (m_pVisibleWebWnd)
		{
			HWND hExtendWnd = m_pVisibleWebWnd->m_hExtendWnd;
			if (hExtendWnd == nullptr)
			{
				hExtendWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, m_hWnd, NULL, theApp.m_hInstance, NULL);
				m_pVisibleWebWnd->m_hExtendWnd = hExtendWnd;
				m_pVisibleWebWnd->m_hChildWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hExtendWnd, (HMENU)2, theApp.m_hInstance, NULL);
				CExtendWnd* pExtendWnd = new CExtendWnd();
				pExtendWnd->m_pHostPage = m_pVisibleWebWnd;
				pExtendWnd->SubclassWindow(m_pVisibleWebWnd->m_hChildWnd);
				::SetWindowLongPtr(hExtendWnd, GWLP_USERDATA, (LONG_PTR)m_pVisibleWebWnd->m_hChildWnd);
				::SetWindowLongPtr(m_pVisibleWebWnd->m_hChildWnd, GWLP_USERDATA, (LONG_PTR)m_pVisibleWebWnd);
			}
			if (::IsChild(hWnd, hExtendWnd))
				::SetParent(hExtendWnd, m_hWnd);

			if (m_pVisibleWebWnd->m_strCurKey == _T(""))
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
			HWND hWebHostWnd = m_pVisibleWebWnd->m_hWebHostWnd;
			if (hWebHostWnd == NULL)
				hWebHostWnd = m_pVisibleWebWnd->m_hChildWnd;
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
							if (m_pCosmosFrameWndInfo == nullptr)
							{
								HWND _hPWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(DocView, hPWnd);
								if (_hPWnd)
								{
									m_pCosmosFrameWndInfo = (CosmosFrameWndInfo*)::GetProp(_hPWnd, _T("CosmosFrameWndInfo"));;
								}
							}
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
		if (m_bDestroy || m_pVisibleWebWnd == nullptr ||
			!::IsWindowVisible(m_hWnd) ||
			g_pCosmos->m_bChromeNeedClosed == TRUE)
			return 0;
		if (!::IsWindow(m_hWnd))
			return 0;
		RECT rcBrowser;
		GetClientRect(&rcBrowser);
		if (m_pVisibleWebWnd->m_pGalaxy == nullptr || m_pVisibleWebWnd->m_strCurKey == _T("")) {
			if (rcBrowser.right * rcBrowser.left)
				::SetWindowPos(m_pVisibleWebWnd->m_hExtendWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_SHOWWINDOW);
			::SetWindowRgn(m_hDrawWnd, NULL, true);
			return 0;
		}
		if (!::IsWindowVisible(m_pVisibleWebWnd->m_hWnd))
		{
			if (m_pVisibleWebWnd->m_hExtendWnd)
				::SetParent(m_pVisibleWebWnd->m_hExtendWnd, m_pVisibleWebWnd->m_hWnd);
			auto it = g_pCosmos->m_mapHtmlWnd.find(m_pBrowser->GetActiveWebContentWnd());
			if (it != g_pCosmos->m_mapHtmlWnd.end())
			{
				m_pVisibleWebWnd = (CWebPage*)it->second;
				if (m_pVisibleWebWnd->m_hExtendWnd)
					::SetParent(m_pVisibleWebWnd->m_hExtendWnd, m_hWnd);
			}
		}

		if (::GetParent(m_pVisibleWebWnd->m_hExtendWnd) != m_hWnd) {
			::SetParent(m_pVisibleWebWnd->m_hExtendWnd, m_hWnd);
		}

		RECT rcWebPage;
		RECT rcExtendWnd;
		::GetWindowRect(m_pVisibleWebWnd->m_hExtendWnd, &rcExtendWnd);
		::ScreenToClient(m_hWnd, (LPPOINT)&rcExtendWnd);
		::ScreenToClient(m_hWnd, ((LPPOINT)&rcExtendWnd) + 1);
		HWND _hWebPage = m_pVisibleWebWnd->m_hWnd;
		if (m_pVisibleWebWnd->m_pDevToolWnd)
		{
			if (::GetParent(m_pVisibleWebWnd->m_hWnd) == ::GetParent(m_pVisibleWebWnd->m_pDevToolWnd->m_hWnd))
			{
				_hWebPage = m_pVisibleWebWnd->m_pDevToolWnd->m_hWnd;
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
		else
		{
			if (g_pCosmos->m_pCLRProxy)
				g_pCosmos->m_pCLRProxy->HideMenuStripPopup();
		}
		::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 4);
		return lRes;
	}

	LRESULT CBrowser::OnDeviceScaleFactorChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		m_fdevice_scale_factor = (float)lParam / 100;
		return lRes;
	}

	LRESULT CBrowser::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		if (g_pCosmos->m_pCLRProxy)
			g_pCosmos->m_pCLRProxy->HideMenuStripPopup();
		if (m_pVisibleWebWnd)
		{
			g_pCosmos->m_pActiveHtmlWnd = m_pVisibleWebWnd;
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
			g_pCosmos->m_pHtmlWndCreated = new CComObject<CWebPage>;
			g_pCosmos->m_pHtmlWndCreated->SubclassWindow(hWnd);
			if (g_pCosmos->m_pCLRProxy)
				g_pCosmos->m_pCLRProxy->OnWebPageCreated(hWnd, (CWebPageImpl*)g_pCosmos->m_pHtmlWndCreated, (IWebPage*)g_pCosmos->m_pHtmlWndCreated, 0);
			HWND hPWnd = ::GetParent(m_hWnd);
			if (g_pCosmos->m_bCreatingDevTool == false)
			{
				g_pCosmos->m_pHtmlWndCreated->m_bDevToolWnd = false;
				g_pCosmos->m_mapHtmlWnd[hWnd] = g_pCosmos->m_pHtmlWndCreated;
				if (m_pBrowser && hWnd == m_pBrowser->GetActiveWebContentWnd())
					m_pVisibleWebWnd = g_pCosmos->m_pHtmlWndCreated;
#ifdef WIN32	
				if (::IsWindow(hPWnd))
				{
					DWORD dwID = 0;
					::GetWindowThreadProcessId(hPWnd, &dwID);
					if (dwID != ::GetCurrentProcessId())
					{
						auto it = g_pCosmos->m_mapRemoteTangramApp.find(dwID);
						if (it != g_pCosmos->m_mapRemoteTangramApp.end())
						{
							g_pCosmos->m_pHtmlWndCreated->m_pRemoteCosmos = it->second;
						}
					}
				}
#endif
			}
			else
			{
				g_pCosmos->m_bCreatingDevTool = false;
				g_pCosmos->m_pHtmlWndCreated->m_bDevToolWnd = true;
				if (m_pVisibleWebWnd) {
					m_pVisibleWebWnd->m_pDevToolWnd = g_pCosmos->m_pHtmlWndCreated;
					g_pCosmos->m_pHtmlWndCreated->m_pWebWnd = m_pVisibleWebWnd;
				}
			}
			if (hPWnd)
			{
				g_pCosmos->m_pActiveHtmlWnd = m_pVisibleWebWnd;
				g_pCosmos->m_pGalaxy = nullptr;
				g_pCosmos->m_bWinFormActived = false;
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
		}
		return 1;
		break;
		case 20200128:
		{
			if (m_pVisibleWebWnd)
			{
				CGalaxy* pGalaxy = m_pVisibleWebWnd->m_pGalaxy;
				if (pGalaxy)
				{
					CXobj* pXobj = pGalaxy->m_pWorkXobj;
					CXobjWnd* pWnd = (CXobjWnd*)(pXobj->m_pHostWnd);
					return (LRESULT)(pWnd->m_hWnd);
				}
			}
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
				return m_pVisibleWebWnd && ::IsWindow(m_pVisibleWebWnd->m_hExtendWnd);
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

		m_pVisibleWebWnd = nullptr;
		auto it = g_pCosmos->m_mapBrowserWnd.find(m_hWnd);
		if (it != g_pCosmos->m_mapBrowserWnd.end()) {
			g_pCosmos->m_mapBrowserWnd.erase(it);
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

	LRESULT CBrowser::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
		if (m_bDestroy || !::IsWindowVisible(m_hWnd) || lpwndpos->flags == (SWP_NOSIZE | SWP_NOMOVE))
			return DefWindowProc(uMsg, wParam, lParam);
		if (g_pCosmos->m_pCLRProxy)
		{
			g_pCosmos->m_pCLRProxy->PreWindowPosChanging(m_hWnd, lpwndpos, 0);
		}
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
		if (m_bDestroy)
			return lRes;
		if (g_pCosmos->m_bChromeNeedClosed == false && m_pVisibleWebWnd)
		{
			switch (lParam)
			{
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
					//if (g_pCosmos->m_bOMNIBOXPOPUPVISIBLE)
					{
						BrowserLayout();
					}
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
								m_pVisibleWebWnd = it.second;
							}
						}
					}
				}
			}
			break;
			case 7:
			{
				switch (wParam)
				{
				case 0:
				case 2:
				{
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
								it.second->m_pChromeRenderFrameHost->ShowWebPage(true);
								m_pVisibleWebWnd = it.second;
							}
						}
					}

					m_pBrowser->LayoutBrowser();
					if (::GetParent(m_hWnd) == nullptr)
						BrowserLayout();
					if (m_pParentXobj)
					{
						HWND hWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(QueryType::RecalcLayout, m_pParentXobj->m_pXobjShareData->m_pGalaxy->m_hWnd);
					}

					if (m_pVisibleWebWnd->m_pGalaxy)
					{
						::SendMessage(m_pVisibleWebWnd->m_hExtendWnd, WM_BROWSERLAYOUT, (WPARAM)m_pVisibleWebWnd->m_hChildWnd, 0);
						if (::GetParent(m_hWnd) == nullptr)
						{
							CXobj* pObj = m_pVisibleWebWnd->m_pGalaxy->m_pWorkXobj;
							if (pObj->m_nViewType == Grid)
							{
								CSplitterWnd* pWnd = (CSplitterWnd*)pObj->m_pHostWnd;
								pWnd->RecalcLayout();
							}
						}
					}
					m_bSZMode = false;
					if (wParam == 2)
					{
						HWND hPWnd = ::GetWindow(m_hWnd, GA_ROOT);
						::RedrawWindow(hPWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
					}
				}
				break;
				case 1:
				{
					m_bTabChange = false;
					::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 7);
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
		if (m_pVisibleWebWnd)
		{
			CString strDisposition = _T("");
			strDisposition.Format(_T("%d"), NEW_BACKGROUND_TAB);
			if (m_pVisibleWebWnd->m_pChromeRenderFrameHost)
			{
				IPCMsg msg;
				msg.m_strId = L"ADD_URL";
				msg.m_strParam1 = OLE2T(bstrURLs);
				msg.m_strParam2 = strDisposition;
				m_pVisibleWebWnd->m_pChromeRenderFrameHost->SendCosmosMessage(&msg);
			}
		}
		return S_OK;
	}

	STDMETHODIMP CBrowser::OpenURL(BSTR bstrURL, BrowserWndOpenDisposition nDisposition, BSTR bstrKey, BSTR bstrXml)
	{
		if (m_pVisibleWebWnd)
		{
			CString strDisposition = _T("");
			strDisposition.Format(_T("%d"), nDisposition);
			if (m_pVisibleWebWnd->m_pChromeRenderFrameHost)
			{
				IPCMsg msg;
				msg.m_strId = L"OPEN_URL";
				msg.m_strParam1 = OLE2T(bstrURL);
				msg.m_strParam2 = strDisposition;
				m_pVisibleWebWnd->m_pChromeRenderFrameHost->SendCosmosMessage(&msg);
			}
		}
		return S_OK;
	}
}  // namespace Browser
