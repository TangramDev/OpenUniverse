/********************************************************************************
 *					DOM Plus for Application - Version 1.1.5.30
 **
 *********************************************************************************
 * Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.
 **
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

#include "../stdafx.h"
#include "../UniverseApp.h"
#include "../Grid.h"
#include "../GridHelper.h"
#include "../Galaxy.h"
#include "WebPage.h"
#include "Browser.h"

namespace Browser {
	CBrowser::CBrowser() {
		m_hDrawWnd = 0;
		m_heightfix = 0;
		m_hOldTab = NULL;
		m_bTabChange = false;
		m_pRemoteGrid = nullptr;
		m_pBrowser = nullptr;
		m_pParentGrid = nullptr;
		m_fdevice_scale_factor = 1.0f;
		m_strCurKey = _T("");
		m_pVisibleWebWnd = nullptr;
		m_pOmniboxViewViews = nullptr;
		if (g_pHubble->m_pCreatingOmniboxViewViews) {
			m_pOmniboxViewViews = g_pHubble->m_pCreatingOmniboxViewViews;
			g_pHubble->m_pCreatingOmniboxViewViews = nullptr;
		}
	}

	CBrowser::~CBrowser() {}

	void CBrowser::ActiveChromeTab(HWND hActive, HWND hOldWnd)
	{
		m_bTabChange = true;
		if (g_pHubble->m_bChromeNeedClosed == false && m_pBrowser)
		{
			if (::IsWindow(hOldWnd))
			{
				m_hOldTab = hOldWnd;
			}
		}
	}

	LRESULT CBrowser::OnChromeTabChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		g_pHubble->m_pActiveHtmlWnd = m_pVisibleWebWnd;
		if (m_pVisibleWebWnd && g_pHubble->m_pActiveHtmlWnd->m_pChromeRenderFrameHost)
		{
			g_pHubble->m_pGalaxy = nullptr;
			g_pHubble->m_bWinFormActived = false;
		}
		m_pBrowser->LayoutBrowser();
		//::SendMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 4);
		return lRes;
	}

	void CBrowser::UpdateContentRect(HWND hWnd, RECT& rc, int nTopFix) {
		if (hWnd == 0 || ::IsWindowVisible(m_hWnd) == false || g_pHubble->m_bChromeNeedClosed == TRUE || g_pHubble->m_bOMNIBOXPOPUPVISIBLE) {
			return;
		}
		if (m_hOldTab)
		{
			RECT rc;
			::GetWindowRect(m_hOldTab, &rc);
			ScreenToClient(&rc);
			::SetWindowPos(m_hOldTab, HWND_BOTTOM, rc.left, rc.top, 1, 1, /*SWP_NOREDRAW |*/ SWP_NOACTIVATE);
			m_hOldTab = NULL;
		}

		BrowserLayout();
		if (m_bTabChange == true || ::IsWindowVisible(hWnd) == FALSE ||
			(m_pVisibleWebWnd && m_pVisibleWebWnd->m_hWnd != hWnd))
		{
			auto it = g_pHubble->m_mapHtmlWnd.find(hWnd);
			if (it != g_pHubble->m_mapHtmlWnd.end())
			{
				m_pVisibleWebWnd = (CWebPage*)it->second;
				if (m_pVisibleWebWnd->m_pChromeRenderFrameHost)
					m_pVisibleWebWnd->m_pChromeRenderFrameHost->ShowWebPage(true);
				if (m_bTabChange)
					::PostMessage(m_hWnd, WM_COSMOSMSG, 20200205, 1);
				return;
			}
		}

		if (m_pVisibleWebWnd)
		{
			if (m_pVisibleWebWnd->m_hExtendWnd == nullptr)
			{
				m_pVisibleWebWnd->m_hExtendWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, m_hWnd, NULL, theApp.m_hInstance, NULL);
				m_pVisibleWebWnd->m_hChildWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, m_pVisibleWebWnd->m_hExtendWnd, (HMENU)2, theApp.m_hInstance, NULL);

				::SetWindowLongPtr(m_pVisibleWebWnd->m_hExtendWnd, GWLP_USERDATA, (LONG_PTR)m_pVisibleWebWnd->m_hChildWnd);
				::SetWindowLongPtr(m_pVisibleWebWnd->m_hChildWnd, GWLP_USERDATA, (LONG_PTR)m_pVisibleWebWnd);
			}
			HWND hExtendWnd = m_pVisibleWebWnd->m_hExtendWnd;
			if (::IsChild(hWnd, hExtendWnd))
				::SetParent(hExtendWnd, m_hWnd);

			::SetWindowPos(hExtendWnd, m_hDrawWnd,
				rc.left,
				nTopFix * m_fdevice_scale_factor,
				rc.right * m_fdevice_scale_factor,
				(rc.bottom - rc.top) * m_fdevice_scale_factor,
				SWP_SHOWWINDOW /*| SWP_NOREDRAW*/ | SWP_NOACTIVATE);
			HWND hWebHostWnd = m_pVisibleWebWnd->m_hWebHostWnd;
			if (hWebHostWnd == NULL)
				hWebHostWnd = m_pVisibleWebWnd->m_hChildWnd;
			if (::IsWindowVisible(hWebHostWnd) == false) {
				m_pVisibleWebWnd->m_bWebContentVisible = false;
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

					rc.left += rc2.left / m_fdevice_scale_factor;
					rc.right -= (rect.right - rc2.right) / m_fdevice_scale_factor;
					rc.top += (rc2.top - rect.top) / m_fdevice_scale_factor;
					rc.bottom -= (rect.bottom - rc2.bottom) / m_fdevice_scale_factor;
					m_pVisibleWebWnd->m_bWebContentVisible = true;
					if (rc.right <= rc.left || rc.bottom <= rc.top) {
						m_pVisibleWebWnd->m_bWebContentVisible = false;
						rc.right = rc.left + 1;
						rc.bottom = rc.top + 1;
					}
				}
			}
		}
	};

	LRESULT CBrowser::BrowserLayout() {
		if (m_pVisibleWebWnd == nullptr || m_bTabChange ||
			!::IsWindowVisible(m_hWnd) ||
			g_pHubble->m_bChromeNeedClosed == TRUE)
			return 0;
		if (!::IsWindow(m_hWnd))
			return 0;
		RECT rcBrowser;
		GetClientRect(&rcBrowser);
		if (m_pVisibleWebWnd->m_pGalaxy == nullptr) {
			if (rcBrowser.right * rcBrowser.left)
				::SetWindowPos(m_pVisibleWebWnd->m_hExtendWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_SHOWWINDOW);
			::SetWindowRgn(m_hDrawWnd, NULL, true);
			return 0;
		}

		if (!::IsWindowVisible(m_pVisibleWebWnd->m_hWnd))
		{
			if (m_pVisibleWebWnd->m_hExtendWnd)
				::SetParent(m_pVisibleWebWnd->m_hExtendWnd, m_pVisibleWebWnd->m_hWnd);
			auto it = g_pHubble->m_mapHtmlWnd.find(m_pBrowser->GetActiveWebContentWnd());
			if (it != g_pHubble->m_mapHtmlWnd.end())
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
		HRGN hWebPage = ::CreateRectRgn(rcWebPage.left, rcWebPage.top, rcWebPage.right, rcWebPage.bottom);
		::CombineRgn(hWebExtendWndRgn, hWebExtendWndRgn, hWebPage, RGN_DIFF);
		::CombineRgn(hGPUWndRgn, hGPUWndRgn, hWebExtendWndRgn, RGN_DIFF);
		::DeleteObject(hWebPage);
		::DeleteObject(hWebExtendWndRgn);
		::SetWindowRgn(m_hDrawWnd, hGPUWndRgn, false);

		return 0;
	}

	LRESULT CBrowser::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		if (LOWORD(wParam) != WA_INACTIVE) {
			if (m_pBrowser) {
				g_pHubble->m_pActiveBrowser = m_pBrowser;
				g_pHubble->m_pActiveBrowser->m_pProxy = this;
				//m_pBrowser->LayoutBrowser();
				//BrowserLayout();
				//::InvalidateRect(m_hWnd, nullptr, true);
			}
		}
		else
		{
			if (g_pHubble->m_pCLRProxy)
				g_pHubble->m_pCLRProxy->HideMenuStripPopup();
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
		if (g_pHubble->m_pCLRProxy)
			g_pHubble->m_pCLRProxy->HideMenuStripPopup();
		if (m_pVisibleWebWnd)
		{
			g_pHubble->m_pActiveHtmlWnd = m_pVisibleWebWnd;
			if (g_pHubble->m_pActiveHtmlWnd && g_pHubble->m_pActiveHtmlWnd->m_pChromeRenderFrameHost)
			{
				g_pHubble->m_pGalaxy = nullptr;
				g_pHubble->m_bWinFormActived = false;
			}
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CBrowser::OnHubbleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		HWND hWnd = (HWND)lParam;
		switch (wParam) {
		case 0: {
			g_pHubble->m_pHtmlWndCreated = new CComObject<CWebPage>;
			g_pHubble->m_pHtmlWndCreated->SubclassWindow(hWnd);
			if (g_pHubble->m_pCLRProxy)
				g_pHubble->m_pCLRProxy->OnWebPageCreated(hWnd, (CWebPageImpl*)g_pHubble->m_pHtmlWndCreated, (IWebPage*)g_pHubble->m_pHtmlWndCreated, 0);
			HWND hPWnd = ::GetParent(m_hWnd);
			if (g_pHubble->m_bCreatingDevTool == false)
			{
				g_pHubble->m_pHtmlWndCreated->m_bDevToolWnd = false;
				g_pHubble->m_mapHtmlWnd[hWnd] = g_pHubble->m_pHtmlWndCreated;
				if (m_pBrowser&&hWnd == m_pBrowser->GetActiveWebContentWnd())
					m_pVisibleWebWnd = g_pHubble->m_pHtmlWndCreated;
#ifdef WIN32	
				if (::IsWindow(hPWnd))
				{
					DWORD dwID = 0;
					::GetWindowThreadProcessId(hPWnd, &dwID);
					if (dwID != ::GetCurrentProcessId())
					{
						auto it = g_pHubble->m_mapRemoteTangramApp.find(dwID);
						if (it != g_pHubble->m_mapRemoteTangramApp.end())
						{
							g_pHubble->m_pHtmlWndCreated->m_pRemoteHubble = it->second;
						}
					}
				}
#endif
			}
			else
			{
				g_pHubble->m_bCreatingDevTool = false;
				g_pHubble->m_pHtmlWndCreated->m_bDevToolWnd = true;
				if (m_pVisibleWebWnd) {
					m_pVisibleWebWnd->m_pDevToolWnd = g_pHubble->m_pHtmlWndCreated;
					g_pHubble->m_pHtmlWndCreated->m_pWebWnd = m_pVisibleWebWnd;
				}
			}
			if (hPWnd)
			{
				g_pHubble->m_pActiveHtmlWnd = m_pVisibleWebWnd;// g_pHubble->m_pHtmlWndCreated;
				g_pHubble->m_pGalaxy = nullptr;
				g_pHubble->m_bWinFormActived = false;
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
			::PostMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 4);
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
		case 20200214:
		{
			auto t = create_task([this]()
				{
					m_heightfix = 0;
					SleepEx(500, true);
					try
					{
						if (m_pVisibleWebWnd)
						{
							::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW);
							BrowserLayout();
							::SetFocus(m_pVisibleWebWnd->m_hWnd);
							::SetForegroundWindow(m_pVisibleWebWnd->m_hWnd);
						}
					}
					catch (...)
					{
						ATLASSERT(false);
						return 0;
					}
					return 1;
				});
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
					CGrid* pGrid = pGalaxy->m_pWorkGrid;
					CGridHelper* pWnd = (CGridHelper*)(pGrid->m_pHostWnd);
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
		if (g_pHubble->m_pCLRProxy)
		{
			IBrowser* pIBrowser = nullptr;
			QueryInterface(__uuidof(IBrowser), (void**)&pIBrowser);
			g_pHubble->m_pCLRProxy->OnDestroyChromeBrowser(pIBrowser);
		}

		m_pVisibleWebWnd = nullptr;
		auto it = g_pHubble->m_mapBrowserWnd.find(m_hWnd);
		if (it != g_pHubble->m_mapBrowserWnd.end()) {
			g_pHubble->m_mapBrowserWnd.erase(it);
		}

		if ((g_pHubble->m_hMainWnd == g_pHubble->m_hHostWnd && g_pHubble->m_mapBrowserWnd.size() == 1) ||
			g_pHubble->m_hHostBrowserWnd == m_hWnd)
		{
			if (g_pHubble->m_hHostBrowserWnd == m_hWnd)
			{
				g_pHubble->m_bChromeNeedClosed = true;
				for (auto it : g_pHubble->m_mapBrowserWnd)
				{
					if (((CBrowser*)it.second)->m_hWnd != m_hWnd)
						((CBrowser*)it.second)->PostMessageW(WM_CLOSE, 0, 0);
				}
				g_pHubble->m_mapBrowserWnd.erase(g_pHubble->m_mapBrowserWnd.begin(), g_pHubble->m_mapBrowserWnd.end());
			}
		}

		if ((g_pHubble->m_hMainWnd == NULL && g_pHubble->m_mapBrowserWnd.size() == 0) ||
			g_pHubble->m_hHostBrowserWnd == m_hWnd) {
			if (g_pHubble->m_hHostBrowserWnd == m_hWnd)
				g_pHubble->m_hHostBrowserWnd = NULL;
			if (g_pHubble->m_pCLRProxy)
			{
				if (g_pHubble->m_pCosmosAppProxy)
					g_pHubble->m_pCosmosAppProxy->OnHubbleClose();
			}

			if (g_pHubble->m_hCBTHook) {
				UnhookWindowsHookEx(g_pHubble->m_hCBTHook);
				g_pHubble->m_hCBTHook = nullptr;
			}
		}

		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);

		if (g_pHubble->m_hMainWnd == NULL &&
			g_pHubble->m_bChromeNeedClosed == false &&
			g_pHubble->m_mapBrowserWnd.size() == 1 &&
			::IsWindow(g_pHubble->m_hHostBrowserWnd) &&
			g_pHubble->m_nAppID == 10000 &&
			g_pHubble->m_bEclipse == false)
		{
			::SendMessageW(g_pHubble->m_hHostBrowserWnd, WM_CLOSE, 0, 0);
		}
		return lRes;
	}

	LRESULT CBrowser::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
		if (g_pHubble->m_pCLRProxy)
		{
			g_pHubble->m_pCLRProxy->PreWindowPosChanging(m_hWnd, lpwndpos, 0);
		}
		HWND hPWnd = ::GetParent(m_hWnd);
		if (::IsWindow(hPWnd))
		{
			WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
			RECT rc;
			::GetClientRect(hPWnd, &rc);
			lpwndpos->x = -12;
			lpwndpos->y = -6 - m_heightfix;
			lpwndpos->cx = rc.right + 24;
			lpwndpos->cy = rc.bottom + 18 + 3 + m_heightfix;
		}
		else if (g_pHubble->m_bOMNIBOXPOPUPVISIBLE)
			::SendMessage(m_hWnd, WM_BROWSERLAYOUT, 0, 2);
		return lRes;
	}

	LRESULT CBrowser::OnBrowserLayout(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		DefWindowProc(uMsg, wParam, lParam);
		if (g_pHubble->m_bChromeNeedClosed == false && m_pVisibleWebWnd)
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
					if (g_pHubble->m_bOMNIBOXPOPUPVISIBLE)
					{
						BrowserLayout();
					}
					m_pBrowser->LayoutBrowser();
				}
				else if (m_pBrowser)
				{
					if (!::IsWindowVisible(m_hWnd))
						::ShowWindow(m_hWnd, SW_SHOW);
					if (g_pHubble->m_bOMNIBOXPOPUPVISIBLE)
					{
						BrowserLayout();
						m_pBrowser->LayoutBrowser();
					}
					m_bTabChange = false;
				}
			}
			break;
			}
		}
		return 0;
	}

	void CBrowser::OnFinalMessage(HWND hWnd) {
		CWindowImpl::OnFinalMessage(hWnd);
		delete this;
	}

	STDMETHODIMP CBrowser::get_RemoteGrid(IGrid** pVal)
	{
		if (m_pRemoteGrid != nullptr)
		{
			*pVal = m_pRemoteGrid;
		}
		return S_OK;
	}

	STDMETHODIMP CBrowser::put_RemoteGrid(IGrid* newVal)
	{
		CComQIPtr<IGrid>pGrid(newVal);
		if (pGrid)
			m_pRemoteGrid = pGrid.Detach();
		//if (m_pRemoteNode == nullptr)
		//{
		//	IStream* pStream = 0;
		//	HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(IID_IGrid, (IGrid*)newVal.pdispVal, &pStream);
		//	if (hr == S_OK)
		//	{
		//		IDispatch* pTarget = nullptr;
		//		hr = ::CoGetInterfaceAndReleaseStream(pStream, IID_IDispatch, (LPVOID*)&pTarget);
		//		if (hr == S_OK && pTarget)
		//			hr = pTarget->QueryInterface(IID_IGrid, (void**)m_pRemoteNode);
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
				m_pVisibleWebWnd->m_pChromeRenderFrameHost->SendHubbleMessage(&msg);
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
				m_pVisibleWebWnd->m_pChromeRenderFrameHost->SendHubbleMessage(&msg);
			}
		}
		return S_OK;
	}
}  // namespace Browser
