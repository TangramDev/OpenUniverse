/********************************************************************************
*					Open Universe - version 0.2.0								*
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

// UniverseApp.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "UniverseApp.h" 
#include "Hubble.h" 
#include <VersionHelpers.h> 
#include <shellscalingapi.h>

#include "StarWnd.h"
#include "Star.h"
#include "Quasar.h"
#include <io.h>
#include <stdio.h>

#include "Psapi.h"

#include "chromium\BrowserWnd.h"
#include "chromium\HtmlWnd.h"

// Description  : the unique App object
CUniverse theUniverse;
CHubble* g_pHubble = nullptr;

CUniverse::CUniverse()
{
	m_bHostCLR = false;
}

CUniverse::~CUniverse()
{
	if (g_pHubble)
		delete g_pHubble;

	OutputDebugString(_T("******************************Exit CUniverse******************************\n"));
}

BOOL CUniverse::InitInstance()
{
	OutputDebugString(_T("******* Start CUniverse::InitInstance()*************\n"));
	TCHAR m_szBuffer[MAX_PATH];
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	::GetModuleFileName(NULL, m_szBuffer, MAX_PATH);
	CString path(m_szBuffer);
	int nPos = path.ReverseFind('\\');
	CString strName = path.Mid(nPos + 1);
	nPos = strName.Find(_T("."));
	CString strExeName = strName.Left(nPos);
	strExeName.MakeLower();
	if (strExeName == _T("regsvr32"))
		return true;

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(824);

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	::OleInitialize(NULL);
	BOOL bOfficeApp = false;

	m_bHostCLR = (BOOL)::GetModuleHandle(_T("mscoreei.dll"));

	{
		new CComObject < CHubble >;
		g_pHubble->m_strExeName = strExeName;
		g_pHubble->m_dwThreadID = ::GetCurrentThreadId();
		if (g_pHubble->m_hCBTHook == nullptr)
			g_pHubble->m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, g_pHubble->m_dwThreadID);
		theUniverse.SetHook(g_pHubble->m_dwThreadID);
#ifndef _WIN64
#else
		g_pHubble->m_bEnableProcessFormTabKey = true;
		if (g_pHubble->m_hForegroundIdleHook == NULL)
			g_pHubble->m_hForegroundIdleHook = SetWindowsHookEx(WH_FOREGROUNDIDLE, CUniverse::ForegroundIdleProc, NULL, ::GetCurrentThreadId());
#endif	
	}
	if (g_pHubble)
	{
		WNDCLASS wndClass;
		wndClass.style = CS_DBLCLKS;
		wndClass.lpfnWndProc = ::DefWindowProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = AfxGetInstanceHandle();
		wndClass.hIcon = 0;
		wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = 0;
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = _T("Tangram Splitter Class");

		RegisterClass(&wndClass);

		g_pHubble->m_lpszSplitterClass = wndClass.lpszClassName;

		wndClass.lpfnWndProc = HubbleWndProc;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpszClassName = L"Tangram Node Class";

		RegisterClass(&wndClass);

		wndClass.lpfnWndProc = HubbleMsgWndProc;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpszClassName = L"Tangram Message Window Class";

		RegisterClass(&wndClass);

		wndClass.lpfnWndProc = HubbleExtendedWndProc;
		wndClass.lpszClassName = L"Chrome Extended Window Class";

		RegisterClass(&wndClass);

		g_pHubble->m_strExeName = strExeName;
		g_pHubble->m_dwThreadID = ::GetCurrentThreadId();
		g_pHubble->HubbleLoad();
		SetHook(g_pHubble->m_dwThreadID);
		if (bOfficeApp || ::GetModuleHandle(_T("msenv.dll")))
			g_pHubble->Init();
		else
		{
			//#ifdef _WIN64
			g_pHubble->m_strStartupURL = _T("");
			int nPos = path.ReverseFind('.');
			int nPos2 = path.ReverseFind('\\');
			CString strPath = path.Left(nPos + 1);
			CString strPath2 = path.Left(nPos2 + 1) + _T("chrome_rt.dll");
			BOOL bHasChromeRT = ::PathFileExists(strPath2) && !::PathIsDirectory(strPath2);
			CString strInitWebPage = strPath + _T("index.html");
			CString strInitEclipse = strPath + _T("eclipse");
			bool bSupportBrowser = (::PathFileExists(strInitWebPage) && !::PathIsDirectory(strInitWebPage));
			if (g_pHubble->m_strStartupURL == _T(""))
			{
				if (bSupportBrowser)
				{
					if (bHasChromeRT)
					{
						g_pHubble->m_strStartupURL = strInitWebPage;
						g_pHubble->m_nAppType = APP_BROWSERAPP;
					}
				}
				else
				{
					strPath = strPath + _T("win32");
					if (::PathFileExists(strPath) && !::PathIsDirectory(strPath))
					{
						g_pHubble->m_nAppType = APP_WIN32;
						g_pHubble->m_strStartupURL = strPath;
					}
					else if (bHasChromeRT)
					{
						g_pHubble->m_nAppType = APP_BROWSER;
						g_pHubble->m_strStartupURL = _T("");
					}
				}
			}
			::PostAppMessage(g_pHubble->m_dwThreadID, WM_TANGRAMINIT, 20191005, 0);
		}
	}

	return true;
}

int CUniverse::ExitInstance()
{
	ATLTRACE(_T("Begin Tangram ExitInstance :%p\n"), this);
	if (g_pHubble) {
		g_pHubble->ExitInstance();
	}
	AfxOleTerm(FALSE);
	ATLTRACE(_T("End Tangram ExitInstance :%p\n"), this);

	return CWinApp::ExitInstance();
}

LRESULT CALLBACK CUniverse::HubbleWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		::ShowWindow(g_pHubble->m_hHostWnd, SW_HIDE);
	}
	return 0;
	case WM_DESTROY:
	{
		if (hWnd == g_pHubble->m_hHostWnd)
		{
			g_pHubble->m_pActiveAppProxy = nullptr;

			if (g_pHubble->m_pCLRProxy)
			{
				if (g_pHubble->m_pCosmosAppProxy)
					g_pHubble->m_pCosmosAppProxy->OnHubbleClose();
			}

			if (::IsWindow(g_pHubble->m_hHostBrowserWnd))
			{
				::SendMessage(g_pHubble->m_hHostBrowserWnd, WM_CLOSE, 0, 0);
			}

			if (g_pHubble->m_hForegroundIdleHook)
				UnhookWindowsHookEx(g_pHubble->m_hForegroundIdleHook);
		}
		break;
	}
	break;
	case WM_POWERBROADCAST:
	{
		if (wParam == PBT_APMRESUMEAUTOMATIC)
		{
			for (auto it : g_pHubble->m_mapThreadInfo)
			{
				if (it.second)
				{
					for (auto it2 : it.second->m_mapQuasar)
					{
						it2.second->HostPosChanged();
					}
				}
			}
		}
	}
	break;
	case WM_WINDOWPOSCHANGED:
		if (hWnd == g_pHubble->m_hHostWnd)
		{
			RECT rc;
			::GetClientRect(g_pHubble->m_hHostWnd, &rc);
			::SetWindowPos(g_pHubble->m_hChildHostWnd, NULL, 0, 0, rc.right, rc.bottom, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
		break;
	case WM_COSMOSMSG:
		switch (lParam)
		{
		case TANGRAM_CHROME_APP_INIT:
		{
			if (g_pHubble->m_hMainWnd == NULL && g_pHubble->m_pUniverseAppProxy)
				g_pHubble->m_hMainWnd = g_pHubble->m_pUniverseAppProxy->InitHubbleApp();
		}
		break;
		case 20200120:
		{
			HWND h = (HWND)wParam;
			if (g_pHubble->m_pCLRProxy)
			{
				g_pHubble->m_pCLRProxy->OnWinFormActivate(h, 4);
			}
		}
		break;
		}
		break;
	case WM_TABCHANGE:
	{
		HWND hwnd = (HWND)wParam;
		IQuasar* pQuasar = nullptr;
		g_pHubble->GetQuasar((LONGLONG)hwnd, &pQuasar);
		if (pQuasar)
		{
			IStar* pNode = nullptr;
			pQuasar->Observe(CComBSTR(L""), CComBSTR(L""), &pNode);
			LONGLONG h = 0;
			pNode->get_Handle(&h);
			HWND hWnd = (HWND)h;
			::InvalidateRect(hWnd, nullptr, true);
		}
	}
	break;
	case WM_TANGRAMAPPQUIT:
	{
		::PostAppMessage(::GetCurrentThreadId(), WM_QUIT, 0, 0);
	}
	break;
	default:
		break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CUniverse::ForegroundIdleProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	for (auto it : g_pHubble->m_mapBrowserWnd)
	{
		CBrowser* pWnd = (CBrowser*)it.second;
		if (pWnd && ::IsWindowVisible(it.first) && pWnd->m_pVisibleWebWnd)
		{
			HWND hWnd = pWnd->m_pBrowser->GetActiveWebContentWnd();
			if (pWnd->m_pVisibleWebWnd)
			{
				if (pWnd->m_pVisibleWebWnd->m_hWnd != hWnd)
				{
					auto it = g_pHubble->m_mapHtmlWnd.find(hWnd);
					if (it != g_pHubble->m_mapHtmlWnd.end())
					{
						pWnd->m_pVisibleWebWnd = (CWebPage*)it->second;
					}
				}
			}
			if (!::IsChild(it.first, pWnd->m_pVisibleWebWnd->m_hWnd))
				::PostMessage(pWnd->m_pVisibleWebWnd->m_hWnd, WM_COSMOSMSG, 20200131, 0);
		}
	}

	if (g_pHubble->m_pHubbleDelegate)
	{
		g_pHubble->m_pHubbleDelegate->ForegroundIdleProc();
	}
	for (auto it : g_pHubble->m_mapHubbleAppProxy)
		it.second->OnForegroundIdleProc();
	return CallNextHookEx(g_pHubble->m_hForegroundIdleHook, nCode, wParam, lParam);
}

LRESULT CALLBACK CUniverse::HubbleMsgWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		if (g_pHubble->m_hTangramWnd == NULL)
		{
			g_pHubble->m_hTangramWnd = hWnd;
			g_pHubble->HubbleInit();
		}
	}
	break;
	case WM_WINFORMCREATED:
	{
		LRESULT l = ::SendMessage((HWND)wParam, WM_TANGRAMDATA, 0, 20190214);
		if (l == 0)
		{
			CWinForm* pWnd = new CWinForm();
			g_pHubble->m_hFormNodeWnd = NULL;
			g_pHubble->m_hFormNodeWnd = (HWND)wParam;
			pWnd->SubclassWindow((HWND)wParam);
			::PostMessage(g_pHubble->m_hFormNodeWnd, WM_WINFORMCREATED, 0, 0);
		}
	}
	break;
	switch (lParam)
	{
	break;
	case 20170907:
		if (g_pHubble->m_pHubblePackageProxy->m_hVSGridView)
		{
			::SetParent(g_pHubble->m_pHubblePackageProxy->m_hPropertyWnd, hWnd);
			::SetWindowPos(g_pHubble->m_pHubblePackageProxy->m_hPropertyWnd, HWND_TOP, -1000, -1000, 0, 0, SWP_NOACTIVATE);
			::PostMessage(hWnd, WM_COSMOSMSG, 1, 20170907);
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	default:
		break;
	}
	return 1;
	break;
	case WM_TANGRAMINIT:
		ATLTRACE(_T("Tangram Message Window WM_TANGRAMINIT :%p\n"), g_pHubble->m_hTangramWnd);
		if (lParam == 20002000)
		{
			g_pHubble->HubbleInit();
		}
		break;
		case WM_TANGRAMAPPQUIT:
		{
			::PostAppMessage(::GetCurrentThreadId(), WM_QUIT, 0, 0);
		}
		break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CUniverse::HubbleExtendedWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_WINDOWPOSCHANGED:
	{
		WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
		LRESULT lRes = ::DefWindowProc(hWnd, msg, wParam, lParam);
		HWND m_hChildWnd = (HWND)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (::IsWindow(m_hChildWnd)) {
			if (::IsWindowVisible(hWnd))
			{
				RECT rc;
				::GetClientRect(m_hChildWnd, &rc);
				if (rc.right != lpwndpos->cx || rc.bottom != lpwndpos->cy)
					::SetWindowPos(m_hChildWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, /*SWP_NOREDRAW |*/ SWP_FRAMECHANGED | SWP_NOACTIVATE);
			}
		}
		return lRes;
	}
	break;
	//case WM_COSMOSMSG:
	//{
	//	switch (lParam)
	//	{
	//	case 20200627:
	//	{
	//		CBrowser* pWnd = (CBrowser*)wParam;
	//		pWnd->BrowserLayout();
	//	}
	//	break;
	//	}
	//	break;
	//}
	//break;
	case WM_TANGRAMDATA:
	{
		switch (lParam)
		{
		case 20200202:
		{
			LRESULT lRes = ::DefWindowProc(hWnd, msg, wParam, lParam);
			CWebPage* m_pHtmlWnd = (CWebPage*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			BindWebObj* pObj = (BindWebObj*)wParam;
			auto it = m_pHtmlWnd->m_mapBindWebObj.find(pObj->m_strBindObjName);
			if (it != m_pHtmlWnd->m_mapBindWebObj.end())
			{
				delete it->second;
				m_pHtmlWnd->m_mapBindWebObj.erase(it);
			}
			m_pHtmlWnd->m_mapBindWebObj[pObj->m_strBindObjName] = pObj;
		}
		break;
		case 20200203:
		{
			LRESULT lRes = ::DefWindowProc(hWnd, msg, wParam, lParam);
			CWebPage* m_pHtmlWnd = (CWebPage*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (m_pHtmlWnd->m_pChromeRenderFrameHost)
			{
				IPCMsg* pMsg = (IPCMsg*)wParam;
				m_pHtmlWnd->m_pChromeRenderFrameHost->SendHubbleMessage(pMsg);
				g_pHubble->m_pCurrentIPCMsg = nullptr;
			}
		}
		break;
		default:
			break;
		}
	}
	break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CUniverse::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (g_pHubble == nullptr)
		return 0;
	LRESULT hr = CallNextHookEx(g_pHubble->m_hCBTHook, nCode, wParam, lParam);
	//TangramCBT(nCode, wParam, lParam);
	HWND hWnd = (HWND)wParam;
	switch (nCode)
	{
	case HCBT_CREATEWND:
	{
		CBT_CREATEWND* pCreateWnd = (CBT_CREATEWND*)lParam;
		LPCTSTR lpszName = pCreateWnd->lpcs->lpszName;
		HWND hPWnd = pCreateWnd->lpcs->hwndParent;
		DWORD dwID = (DWORD)pCreateWnd->lpcs->hMenu;
		::GetClassName(hWnd, g_pHubble->m_szBuffer, MAX_PATH);
		CString strClassName = CString(g_pHubble->m_szBuffer);
		::GetClassName(hPWnd, g_pHubble->m_szBuffer, MAX_PATH);
		CString strPClassName = CString(g_pHubble->m_szBuffer);

		if (strClassName == _T("Chrome_RenderWidgetHostHWND"))
		{
			if ((::GetWindowLong(hPWnd, GWL_STYLE) & WS_POPUP) == 0)
			{
				auto it = g_pHubble->m_mapBrowserWnd.find(hPWnd);
				if (it == g_pHubble->m_mapBrowserWnd.end()) {
					CBrowser* pChromeBrowserWnd = new CComObject<CBrowser>();
					pChromeBrowserWnd->SubclassWindow(hPWnd);
					g_pHubble->m_mapBrowserWnd[hPWnd] = pChromeBrowserWnd;
					pChromeBrowserWnd->m_pBrowser = g_pHubble->m_pActiveBrowser;
					if (pChromeBrowserWnd->m_pBrowser)
						pChromeBrowserWnd->m_pBrowser->m_pProxy = pChromeBrowserWnd;
					if (g_pHubble->m_pHubbleDelegate)
						g_pHubble->m_pHubbleDelegate->m_bBrowserWndCreated = true;
				}
				::PostMessage(hPWnd, WM_COSMOSMSG, 0, (LPARAM)hWnd);
			}
		}

		if (HIWORD(pCreateWnd->lpcs->lpszClass))
		{
			if (g_pHubble->m_pCLRProxy)
			{
				g_pHubble->m_pCLRProxy->WindowCreated(strClassName, lpszName, hPWnd, hWnd);
			}
		}
		if ((pCreateWnd->lpcs->style & WS_CHILD) == 0)
		{
			LRESULT lRes = ::SendMessage(hWnd, WM_QUERYAPPPROXY, 0, 0);
			if (lRes > 0)
			{
				g_pHubble->m_pActiveAppProxy = (IHubbleAppProxy*)lRes;
			}
		}
	}
	break;
	case HCBT_DESTROYWND:
	{
		if (g_pHubble->m_pCLRProxy)
			g_pHubble->m_pCLRProxy->WindowDestroy(hWnd);

		auto it = g_pHubble->m_mapFramePage.find(hWnd);
		if (it != g_pHubble->m_mapFramePage.end())
			g_pHubble->m_mapFramePage.erase(it);

		if (hWnd == g_pHubble->m_hMainWnd)
		{
			if (theUniverse.m_bHostCLR && g_pHubble->m_nAppType == APP_BROWSERAPP)
				g_pHubble->m_nAppType = APP_BROWSER;
			::DestroyWindow(g_pHubble->m_hHostWnd);
			if (theUniverse.m_bHostCLR && g_pHubble->m_nAppType == 0)
				::PostQuitMessage(20191116);
		}
	}
	break;
	case HCBT_MINMAX:
		switch (lParam)
		{
		case SW_MINIMIZE:
		{
			if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
				::PostMessage(hWnd, WM_MDICHILDMIN, 0, 0);
		}
		break;
		case SW_RESTORE:
		case SW_MAXIMIZE:
		{
			::PostMessage(hWnd, WM_COSMOSMSG, 1, 20200115);
		}
		break;
		}
		break;
	case HCBT_ACTIVATE:
	{
		g_pHubble->m_hActiveWnd = hWnd;
		if (g_pHubble->m_pCLRProxy)
		{
			g_pHubble->m_bWinFormActived = g_pHubble->m_pCLRProxy->IsWinForm(hWnd);
			if (g_pHubble->m_bWinFormActived)
			{
				g_pHubble->m_pQuasar = nullptr;
			}
		}
		LRESULT lRes = ::SendMessage(hWnd, WM_QUERYAPPPROXY, 0, 0);
		if (lRes > 0)
		{
			g_pHubble->m_pActiveAppProxy = (IHubbleAppProxy*)lRes;
		}
		else
			g_pHubble->m_pActiveAppProxy = nullptr;
	}
	break;
	}
	return hr;
}

static bool g_bRecturnPressed = false;

LRESULT CALLBACK CUniverse::GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSG lpMsg = (LPMSG)lParam;
	DWORD dwID = ::GetCurrentThreadId();
	CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);
	if (lpMsg->message == WM_TIMER)
		return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
	if (nCode >= 0)
	{
		switch (wParam)
		{
		case PM_NOREMOVE:
		{
			switch (lpMsg->message)
			{
			case WM_QUIT:
			{
				if (::GetCurrentThreadId() == g_pHubble->m_dwThreadID)
				{
					if (::IsWindow(g_pHubble->m_hHostWnd))
						::DestroyWindow(g_pHubble->m_hHostWnd);
					if (::IsWindow(g_pHubble->m_hTangramWnd))
						::DestroyWindow(g_pHubble->m_hTangramWnd);
				}
				if (g_pHubble->m_pCosmosAppProxy)
					g_pHubble->m_pCosmosAppProxy->OnHubbleClose();
			}
			break;
			default:
				break;
			}
		}
		break;
		case PM_REMOVE:
		{
			switch (lpMsg->message)
			{
			case WM_SYSKEYDOWN:
			{
				if (::GetAsyncKeyState(VK_MENU) < 0 &&
					g_pHubble->m_pHubbleDelegate &&
					g_pHubble->m_pHubbleDelegate->OnUniversePreTranslateMessage(lpMsg))
				{
					return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
				}
			}
			break;
			case WM_KEYDOWN:
			{
				CStarWnd* pWnd = nullptr;
				if (g_pHubble->m_bOMNIBOXPOPUPVISIBLE && lpMsg->wParam == VK_RETURN)
				{
					g_bRecturnPressed = true;
				}
				if (g_pHubble->m_pActiveStar)
				{
					if (g_pHubble->m_pActiveStar->m_nViewType != Splitter)
					{
						pWnd = (CStarWnd*)g_pHubble->m_pActiveStar->m_pHostWnd;
						if (pWnd && ::IsChild(pWnd->m_hWnd, lpMsg->hwnd) == false)
						{
							g_pHubble->m_pActiveStar = nullptr;
							if (lpMsg->wParam != VK_TAB)
								break;
							else if (g_pHubble->m_bWinFormActived == false)
							{
								if (pWnd->m_bBKWnd)
								{
									if (pWnd->PreTranslateMessage(lpMsg))
									{
										lpMsg->hwnd = NULL;
										lpMsg->lParam = 0;
										lpMsg->wParam = 0;
										lpMsg->message = 0;
										break;
									}
								}
								else
									g_pHubble->m_pQuasar = nullptr;
							}
						}
					}
				}

				switch (lpMsg->wParam)
				{
				case VK_TAB:
					if (g_pHubble->m_bWinFormActived && g_pHubble->m_bEnableProcessFormTabKey && g_pHubble->m_pCLRProxy->ProcessFormMsg(g_pHubble->m_hActiveWnd, lpMsg, 0))
					{
						break;
					}
					if (g_pHubble->m_pQuasar && g_pHubble->m_pActiveStar && pWnd && pWnd->PreTranslateMessage(lpMsg))
					{
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
					}
					else
					{
						HWND hwnd = lpMsg->hwnd;
						if (::GetModuleHandle(_T("chrome.dll")))
						{
							TRACE(_T("======== CUniverse=========:%x,MSG:%x\n"), lpMsg->hwnd, lpMsg->message);
							if (g_pHubble->m_pActiveHtmlWnd)
							{
								HWND hWnd = ::GetParent(::GetParent(g_pHubble->m_pActiveHtmlWnd->m_hWnd));
								if (hWnd)
								{
									TranslateMessage(lpMsg);
									::DispatchMessage(lpMsg);
									lpMsg->hwnd = NULL;
									lpMsg->lParam = 0;
									lpMsg->wParam = 0;
									lpMsg->message = 0;
									return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
								}
							}
						}
						if (pWnd && pWnd->PreTranslateMessage(lpMsg))
						{
							::DispatchMessage(lpMsg);
							TranslateMessage(lpMsg);
							lpMsg->hwnd = NULL;
							lpMsg->lParam = 0;
							lpMsg->wParam = 0;
							lpMsg->message = 0;
						}
						if (((__int64)g_pHubble->m_pActiveAppProxy) > 1)
							g_pHubble->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
						//else if (((__int64)g_pHubble->m_pUniverseAppProxy) > 1)
						//{
						//	g_pHubble->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
						//}
					}
					break;
				case VK_PRIOR:
				case VK_NEXT:
				case VK_HOME:
				case VK_END:
				case VK_LEFT:
				case VK_UP:
				case VK_RIGHT:
				case VK_DOWN:
				case VK_BACK:
					if (g_pHubble->m_bWinFormActived && g_pHubble->m_bEnableProcessFormTabKey && g_pHubble->m_pCLRProxy->ProcessFormMsg(g_pHubble->m_hActiveWnd, lpMsg, 0))
					{
						TranslateMessage(lpMsg);
						::DispatchMessage(lpMsg);
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
						break;
					}
					if (g_pHubble->m_pQuasar && g_pHubble->m_pActiveStar && pWnd && pWnd->PreTranslateMessage(lpMsg))
					{
						if (g_pHubble->m_pCLRProxy && g_pHubble->m_pCLRProxy->IsWinForm(pWnd->m_hWnd))
						{
							//TranslateMessage(lpMsg);
							::DispatchMessage(lpMsg);
							//lpMsg->hwnd = NULL;
							//lpMsg->lParam = 0;
							//lpMsg->wParam = 0;
							//lpMsg->message = 0;
							return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
						}
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
					}
					if (g_pHubble->m_pHubbleDelegate)
					{
						if (g_pHubble->m_pHubbleDelegate->OnUniversePreTranslateMessage(lpMsg))
							break;
					}
					break;
				case VK_DELETE:
					if (g_pHubble->m_pActiveStar)
					{
						if (g_pHubble->m_pActiveStar->m_nViewType == ActiveX)
						{
							pWnd->PreTranslateMessage(lpMsg);
							lpMsg->hwnd = NULL;
							lpMsg->wParam = 0;
							break;
						}
						DispatchMessage(lpMsg);
						lpMsg->hwnd = NULL;
						lpMsg->wParam = 0;
					}

					break;
				case VK_RETURN:
				{
					if (g_pHubble->m_pHubbleDelegate)
					{
						if (g_pHubble->m_pHubbleDelegate->OnUniversePreTranslateMessage(lpMsg))
							break;
					}
					if (g_pHubble->m_pQuasar && g_pHubble->m_pActiveStar)
					{
						if (pWnd && ::IsChild(pWnd->m_hWnd, lpMsg->hwnd) == false)
						{
							g_pHubble->m_pActiveStar = nullptr;
							g_pHubble->m_pQuasar = nullptr;
						}
						else if (pWnd)
						{
							TranslateMessage(lpMsg);
							lpMsg->hwnd = NULL;
							lpMsg->lParam = 0;
							lpMsg->wParam = 0;
							lpMsg->message = 0;
							break;
						}
					}
					if (g_pHubble->m_bOfficeApp)
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
					TranslateMessage(lpMsg);
					if (g_pHubble->m_strExeName != _T("devenv"))
					{
						DispatchMessage(lpMsg);
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
					}
				}
				break;
				case 0x41://Ctrl+A
				case 0x43://Ctrl+C
				case 0x56://Ctrl+V
				case 0x58://Ctrl+X
				case 0x5a://Ctrl+Z
					if (::GetKeyState(VK_CONTROL) < 0)  // control pressed
					{
						if (g_pHubble->m_pActiveStar && pWnd && !::IsWindow(pWnd->m_hWnd))
						{
							g_pHubble->m_pActiveStar = nullptr;
						}
						if (g_pHubble->m_pActiveStar)
						{
							HWND hWnd = nullptr;
							if (pWnd)
								hWnd = pWnd->m_hWnd;
							if ((g_pHubble->m_pActiveStar->m_nViewType == ActiveX || g_pHubble->m_pActiveStar->m_strID.CompareNoCase(TGM_NUCLEUS) == 0))
							{
								if (pWnd)
									pWnd->PreTranslateMessage(lpMsg);
								lpMsg->hwnd = NULL;
								lpMsg->wParam = 0;
								break;
							}
							if (g_pHubble->m_pActiveHtmlWnd)
							{
								HWND hwnd = lpMsg->hwnd;
								HWND hWnd = ::GetParent(::GetParent(g_pHubble->m_pActiveHtmlWnd->m_hWnd));
								if (hWnd)
								{
									TranslateMessage(lpMsg);
									::DispatchMessage(lpMsg);
									lpMsg->hwnd = NULL;
									lpMsg->lParam = 0;
									lpMsg->wParam = 0;
									lpMsg->message = 0;
									return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
								}
							}
							TranslateMessage(lpMsg);
							lpMsg->wParam = 0;
						}
					}
					break;
				}
			}
			break;
			case WM_TANGRAMINIT:
			{
				if (lpMsg->wParam == 20191005)
					g_pHubble->Init();
			}
			break;
			case WM_MOUSEMOVE:
			{
				if ((long)(g_pHubble->m_pActiveAppProxy) > 1)
				{
					g_pHubble->m_pActiveAppProxy->MouseMoveProxy(lpMsg->hwnd);
				}
				else if (g_pHubble->m_pUniverseAppProxy)
				{
					g_pHubble->m_pUniverseAppProxy->MouseMoveProxy(lpMsg->hwnd);
				}
				if (g_pHubble->m_pHubbleDelegate)
				{
					if (g_pHubble->m_pHubbleDelegate->OnUniversePreTranslateMessage(lpMsg))
						break;
				}
				if ((long)(g_pHubble->m_pActiveAppProxy) > 1)
					g_pHubble->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
				else if (g_pHubble->m_pUniverseAppProxy)
					g_pHubble->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
			}
			break;
			case WM_NCLBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONUP:
				//case WM_POINTERDOWN:
				//case WM_POINTERUP:
			case WM_SETWNDFOCUSE:
			{
				g_pHubble->ProcessMsg(lpMsg);
				//::DispatchMessage(lpMsg);
				//for m_strStartupCLRObj support
				//if (g_pHubble->m_pActiveWinFormWnd && g_pHubble->m_bEnableProcessFormTabKey && g_pHubble->m_pCLRProxy->ProcessFormMsg(g_pHubble->m_pActiveWinFormWnd->m_hWnd, lpMsg, 0))
				//{
				//	TranslateMessage(lpMsg);
				//	::DispatchMessage(lpMsg);
				//	//lpMsg->hwnd = NULL;
				//	//lpMsg->lParam = 0;
				//	//lpMsg->wParam = 0;
				//	//lpMsg->message = 0;
				//	return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
				//	break;
				//}
				if (lpMsg->message == WM_LBUTTONDOWN || lpMsg->message == WM_LBUTTONUP)
				{
					if (g_pHubble->m_pActiveWinFormWnd)
					{
						//::SendMessage(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
						g_pHubble->m_pCLRProxy->ProcessFormMsg(g_pHubble->m_pActiveWinFormWnd->m_hWnd, lpMsg, 0);
					}
				}
				if ((long)(g_pHubble->m_pActiveAppProxy) > 1)
					g_pHubble->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
				else if (g_pHubble->m_pUniverseAppProxy)
					g_pHubble->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
				if (g_pHubble->m_pHubbleDelegate)
				{
					if (g_pHubble->m_pHubbleDelegate->OnUniversePreTranslateMessage(lpMsg))
						break;
				}
				return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
			}
			break;
			case WM_MDICHILDMIN:
				::BringWindowToTop(lpMsg->hwnd);
				break;
			case WM_CHROMEAPPINIT:
			{
				if (lpMsg->lParam == APP_WIN32 ||
					lpMsg->lParam == APP_ECLIPSE)
					break;
				typedef int(__stdcall* _InitApp)(bool bSupportCrashReporting);
				_InitApp FuncInitApp;
				HMODULE hModule = ::GetModuleHandle(L"chrome_rt.dll");
				if (hModule == nullptr)
					hModule = ::LoadLibrary(L"chrome_rt.dll");
				if (hModule) {
					FuncInitApp = (_InitApp)GetProcAddress(hModule, "InitApp");
					if (FuncInitApp != NULL) {
						HWND hWnd = g_pHubble->m_pHubbleDelegate->GetMainWnd();
						if (::IsWindow(hWnd))
						{
							g_pHubble->m_hMainWnd = hWnd;
						}

						FuncInitApp(false);

						::PostQuitMessage(0);
					}
				}
			}
			break;
			case WM_NAVIXTML:
			{
				RECT rc;
				HWND hWnd = ::GetParent(lpMsg->hwnd);
				::GetClientRect(hWnd, &rc);
				::SetWindowPos(lpMsg->hwnd, HWND_BOTTOM, rc.left, rc.top, rc.right + 1, rc.bottom, SWP_NOZORDER | SWP_FRAMECHANGED);
				::SetWindowPos(lpMsg->hwnd, HWND_BOTTOM, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			break;
			case WM_TANGRAM_WEBNODEDOCCOMPLETE:
			{
				auto it = g_pHubble->m_mapWindowPage.find((HWND)lpMsg->wParam);
				if (it != g_pHubble->m_mapWindowPage.end())
					((CGalaxyCluster*)it->second)->OnNodeDocComplete(lpMsg->wParam);
			}
			break;
			case WM_CHROMEDEVTOOLMSG:
			{
				switch (lpMsg->wParam)
				{
				case 1:
				{
					g_pHubble->m_bCreatingDevTool = true;
					auto it = g_pHubble->m_mapBrowserWnd.find(::GetActiveWindow());
					if (it != g_pHubble->m_mapBrowserWnd.end())
					{
						ATLTRACE(_T("\n"));
					}
				}
				break;
				}
			}
			break;
			case WM_COSMOSMSG:
			{
				switch (lpMsg->lParam)
				{
				case 20191004:
				{
					if (g_pHubble->m_pCLRProxy)
					{
						g_pHubble->m_pCLRProxy->HubbleAction(CComBSTR("setmainform"), nullptr);
					}
				}
				break;
				case 20190511:
				{
					::DestroyWindow(g_pHubble->m_hHostWnd);
				}
				break;
				case 2019111701:
				{
					if (g_pHubble->m_mapNodeForHtml.size())
					{
						for (auto it : g_pHubble->m_mapNodeForHtml)
						{
							it.first->put_URL(CComBSTR(it.second));
						}
						g_pHubble->m_mapNodeForHtml.erase(g_pHubble->m_mapNodeForHtml.begin(), g_pHubble->m_mapNodeForHtml.end());
					}
				}
				break;
				case 20191022:
				{
					if (g_pHubble->m_pCLRProxy)
					{
						g_pHubble->m_pCLRProxy->HubbleAction(CComBSTR("startclrapp"), nullptr);
					}
				}
				break;
				}
			}
			break;
			case WM_TANGRAMAPPQUIT:
			{
				TRACE(_T("======== WM_TANGRAMAPPQUIT=========\n"));
				if (g_pHubble->m_mapBrowserWnd.size())
				{
					g_pHubble->m_bChromeNeedClosed = true;
					auto it = g_pHubble->m_mapBrowserWnd.begin();
					((CBrowser*)it->second)->SendMessageW(WM_CLOSE, 0, 0);
				}
			}
			break;
			case WM_CHROMEOMNIBOXPOPUPVISIBLE:
			{
				g_pHubble->m_bOMNIBOXPOPUPVISIBLE = lpMsg->lParam ? true : false;
				auto it = g_pHubble->m_mapBrowserWnd.find((HWND)lpMsg->wParam);
				if (it != g_pHubble->m_mapBrowserWnd.end())
				{
					CWebPage* pWnd = ((CBrowser*)it->second)->m_pVisibleWebWnd;
					if (pWnd && ::IsWindow(pWnd->m_hWnd) && pWnd->m_pQuasar)
					{
						IStar* pNode = nullptr;
						if (g_bRecturnPressed == false)
						{
							pWnd->m_pQuasar->Observe(CComBSTR(lpMsg->lParam ? _T("__default__key__for__chrome__") : pWnd->m_strCurKey), CComBSTR(lpMsg->lParam ? g_pHubble->m_strDefaultXml : _T("")), &pNode);
							::SendMessage(it->first, WM_BROWSERLAYOUT, 0, 2);
						}
						g_bRecturnPressed = false;
					}
				}
			}
			break;
			}
		}
		break;
		default:
			break;
		}
	}

	return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
}

void CUniverse::SetHook(DWORD ThreadID)
{
	CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(ThreadID);
	if (pThreadInfo && pThreadInfo->m_hGetMessageHook == NULL)
	{
		pThreadInfo->m_hGetMessageHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, NULL, ThreadID);
	}
}
