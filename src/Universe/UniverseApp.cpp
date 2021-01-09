/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101070006           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * 
 * Web Runtime is a new, subversive and Internet Oriented Development Technology 
 * for Windows Desktop Software. Its core idea is that, relying on the open-source 
 * project of Chromium, developers are allowed to use their familiar software 
 * development technology to realize the Main Process of their application system 
 * as the Browser Process of Modern Web Browser. Because the Main Process is realized 
 * by the developers themselves, the application system developed in this way is 
 * completely different from modern web browser. This kind of application will 
 * integrate the infrastructure of modern web browser as a part of the system model, 
 * the application system contains not only the browser window inherent in the browser, 
 * but also other desktop elements oriented to the application, such as WinForms, Java
 * and other mature software elements.
 * 
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
 *
 * Built-in Full-Featured Modern Web Browser Subsystem
 *		Since the main process of the application system is essentially a Browser 
 * Process, the application system can naturally create a new browser window. The 
 * difference with standard browsers is that we allow the browser window to be a child 
 * window of other windows. in sub-window model, we see a brand-new scene: a multi-tab 
 * browser sub-window, we think this structure is more suitable for organizing specific 
 * applications. Of course, the application system fully supports the creation of an 
 * independent browser window as a part of the complete application system, and the browser 
 * window serves as a natural extension of the ecological chain to ensure the richness 
 * and diversity of the application system content.
 * DOM Plus:
 *		Because a large number of mature binary components that conform to industry 
 * standards have formed a wealth of software asset accumulation, web runtime allows 
 * developers to regard these existing accumulation as new DOM elements and then become 
 * a part of web pages, this starting point eventually forms "DOM Plus". DOM plus is a 
 * natural extension of standard DOM for desktop applications, similar to the extension 
 * from "real number system" to "complex number system" in mathematical theory. Undoubtedly, 
 * for the development of mathematics, from "real number system" to "complex number system" 
 * is a huge leap, which can be said to be a decisive change in the development of modern 
 * mathematics. We hope that the evolution from "DOM" to "DOM plus" will have a similar 
 * effect, which is bound to be a huge change from desktop software to Internet desktop software.
 *		Different from earlier IE, based on the sandbox mechanism of Modern Web Browser, 
 * we let these binary elements run in the Browser Process, completely isolated from the 
 * Browser's Renderer Process. This makes full use of the security mechanisms of modern 
 * browsers and also takes into account the accumulation of mature software component 
 * technologies. Based on such considerations, existing development technologies such as 
 * .NET, COM, Java, etc. can be fully applied to Internet software development.
 * 
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

// UniverseApp.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "UniverseApp.h" 
#include "Cosmos.h" 
#include <VersionHelpers.h> 
#include <shellscalingapi.h>

#include "XobjHelper.h"
#include "Xobj.h"
#include "Galaxy.h"
#include <io.h>
#include <stdio.h>

#include "Psapi.h"

#include "chromium\Browser.h"
#include "chromium\WebPage.h"

// Description  : the unique App object
CUniverse theUniverse;
CCosmos* g_pCosmos = nullptr;

CUniverse::CUniverse()
{
	m_bHostCLR = false;
}

CUniverse::~CUniverse()
{
	if (g_pCosmos)
		delete g_pCosmos;

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
		new CComObject < CCosmos >;
		g_pCosmos->m_strExeName = strExeName;
		g_pCosmos->m_dwThreadID = ::GetCurrentThreadId();
		if (g_pCosmos->m_hCBTHook == nullptr)
			g_pCosmos->m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, g_pCosmos->m_dwThreadID);
		theUniverse.SetHook(g_pCosmos->m_dwThreadID);
#ifndef _WIN64
#else
		g_pCosmos->m_bEnableProcessFormTabKey = true;
		if (g_pCosmos->m_hForegroundIdleHook == NULL)
			g_pCosmos->m_hForegroundIdleHook = SetWindowsHookEx(WH_FOREGROUNDIDLE, CUniverse::ForegroundIdleProc, NULL, ::GetCurrentThreadId());
#endif	
	}
	if (g_pCosmos)
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
		wndClass.lpszClassName = _T("Universe GridWindow Class");

		RegisterClass(&wndClass);

		g_pCosmos->m_lpszSplitterClass = wndClass.lpszClassName;

		wndClass.lpfnWndProc = CosmosWndProc;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpszClassName = L"Cosmos Xobj Class";

		RegisterClass(&wndClass);

		wndClass.lpfnWndProc = CosmosMsgWndProc;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpszClassName = L"Tangram Message Window Class";

		RegisterClass(&wndClass);

		wndClass.lpfnWndProc = CosmosExtendedWndProc;
		wndClass.lpszClassName = L"Chrome Extended Window Class";

		RegisterClass(&wndClass);

		g_pCosmos->m_strExeName = strExeName;
		g_pCosmos->m_dwThreadID = ::GetCurrentThreadId();
		g_pCosmos->CosmosLoad();
		SetHook(g_pCosmos->m_dwThreadID);
		if (bOfficeApp || ::GetModuleHandle(_T("msenv.dll")))
			g_pCosmos->Init();
		else
		{
			//#ifdef _WIN64
			g_pCosmos->m_strStartupURL = _T("");
			int nPos = path.ReverseFind('.');
			int nPos2 = path.ReverseFind('\\');
			CString strPath = path.Left(nPos + 1);
			CString strPath2 = path.Left(nPos2 + 1) + _T("chrome_rt.dll");
			BOOL bHasChromeRT = ::PathFileExists(strPath2) && !::PathIsDirectory(strPath2);
			CString strInitWebPage = strPath + _T("index.html");
			CString strInitEclipse = strPath + _T("eclipse");
			bool bSupportBrowser = (::PathFileExists(strInitWebPage) && !::PathIsDirectory(strInitWebPage));
			if (g_pCosmos->m_strStartupURL == _T(""))
			{
				if (bSupportBrowser)
				{
					if (bHasChromeRT)
					{
						g_pCosmos->m_strStartupURL = strInitWebPage;
						g_pCosmos->m_nAppType = APP_BROWSERAPP;
					}
				}
				else
				{
					strPath = strPath + _T("win32");
					if (::PathFileExists(strPath) && !::PathIsDirectory(strPath))
					{
						g_pCosmos->m_nAppType = APP_WIN32;
						g_pCosmos->m_strStartupURL = strPath;
					}
					else if (bHasChromeRT)
					{
						g_pCosmos->m_nAppType = APP_BROWSER;
						g_pCosmos->m_strStartupURL = _T("");
					}
				}
			}
			::PostAppMessage(g_pCosmos->m_dwThreadID, WM_HUBBLE_INIT, 20191005, 0);
		}
	}

	return true;
}

int CUniverse::ExitInstance()
{
	ATLTRACE(_T("Begin Tangram ExitInstance :%p\n"), this);
	if (g_pCosmos) {
		g_pCosmos->ExitInstance();
	}
	AfxOleTerm(FALSE);
	ATLTRACE(_T("End Tangram ExitInstance :%p\n"), this);

	return CWinApp::ExitInstance();
}

LRESULT CALLBACK CUniverse::CosmosWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		::ShowWindow(g_pCosmos->m_hHostWnd, SW_HIDE);
	}
	return 0;
	case WM_DESTROY:
	{
		if (hWnd == g_pCosmos->m_hHostWnd)
		{
			g_pCosmos->m_pActiveAppProxy = nullptr;

			if (g_pCosmos->m_pCLRProxy)
			{
				if (g_pCosmos->m_pCosmosAppProxy)
					g_pCosmos->m_pCosmosAppProxy->OnCosmosClose();
			}

			if (::IsWindow(g_pCosmos->m_hHostBrowserWnd))
			{
				::SendMessage(g_pCosmos->m_hHostBrowserWnd, WM_CLOSE, 0, 0);
			}

			if (g_pCosmos->m_hForegroundIdleHook)
				UnhookWindowsHookEx(g_pCosmos->m_hForegroundIdleHook);
		}
		break;
	}
	break;
	case WM_POWERBROADCAST:
	{
		switch (wParam)
		{
		case PBT_APMRESUMEAUTOMATIC:
		case PBT_APMPOWERSTATUSCHANGE:
		{
			for (auto it : g_pCosmos->m_mapThreadInfo)
			{
				if (it.second)
				{
					for (auto it2 : it.second->m_mapGalaxy)
					{
						it2.second->HostPosChanged();
						//for (auto it3 : it2.second->m_mapWPFView)
						//{
						//	ATLTRACE(_T("HWND %x, WM_POWERBROADCAST\n"), it3.second->m_hWnd);
						//	::SetWindowLongPtr(it3.second->m_hWnd, GWLP_USERDATA, 1963);
						//}
					}
				}
			}
			for (auto it : g_pCosmos->m_mapBrowserWnd)
			{
				if (::IsWindowVisible(it.first))
				{
					CBrowser* pWnd = (CBrowser*)it.second;
					if (pWnd && pWnd->m_pVisibleWebWnd)
					{
						HWND hWnd = pWnd->m_pBrowser->GetActiveWebContentWnd();
						if (hWnd)
						{
							auto it = g_pCosmos->m_mapHtmlWnd.find(hWnd);
							if (it != g_pCosmos->m_mapHtmlWnd.end())
							{
								pWnd->m_pVisibleWebWnd = (CWebPage*)it->second;
								it->second->m_pChromeRenderFrameHost->ShowWebPage(true);
							}
						}
						::PostMessage(hWnd, WM_COSMOSMSG, 20200131, 0);
					}
					::PostMessage(it.first, WM_BROWSERLAYOUT, 0, 4);
				}
			}
		}
		break;
		}
	}
	break;
	case WM_WINDOWPOSCHANGED:
		if (hWnd == g_pCosmos->m_hHostWnd)
		{
			RECT rc;
			::GetClientRect(g_pCosmos->m_hHostWnd, &rc);
			::SetWindowPos(g_pCosmos->m_hChildHostWnd, NULL, 0, 0, rc.right, rc.bottom, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
		break;
	case WM_COSMOSMSG:
		switch (lParam)
		{
		case TANGRAM_CHROME_APP_INIT:
		{
			if (g_pCosmos->m_hMainWnd == NULL && g_pCosmos->m_pUniverseAppProxy)
				g_pCosmos->m_hMainWnd = g_pCosmos->m_pUniverseAppProxy->InitCosmosApp();
		}
		break;
		case 20200120:
		{
			HWND h = (HWND)wParam;
			if (g_pCosmos->m_pCLRProxy)
			{
				g_pCosmos->m_pCLRProxy->OnWinFormActivate(h, 4);
			}
		}
		break;
		}
		break;
	case WM_TABCHANGE:
	{
		HWND hwnd = (HWND)wParam;
		IGalaxy* pGalaxy = nullptr;
		g_pCosmos->GetGalaxy((LONGLONG)hwnd, &pGalaxy);
		if (pGalaxy)
		{
			IXobj* pXobj = nullptr;
			pGalaxy->Observe(CComBSTR(L""), CComBSTR(L""), &pXobj);
			LONGLONG h = 0;
			pXobj->get_Handle(&h);
			HWND hWnd = (HWND)h;
			//::InvalidateRect(hWnd, nullptr, true);
		}
	}
	break;
	case WM_HUBBLE_APPQUIT:
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
	if (g_pCosmos->m_pCosmosDelegate)
	{
		g_pCosmos->m_pCosmosDelegate->ForegroundIdleProc();
	}
	for (auto it : g_pCosmos->m_mapCosmosAppProxy)
		it.second->OnForegroundIdleProc();
	return CallNextHookEx(g_pCosmos->m_hForegroundIdleHook, nCode, wParam, lParam);
}

LRESULT CALLBACK CUniverse::CosmosMsgWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		if (g_pCosmos->m_hCosmosWnd == NULL)
		{
			g_pCosmos->m_hCosmosWnd = hWnd;
			g_pCosmos->CosmosInit();
		}
	}
	break;
	case WM_WINFORMCREATED:
	{
		LRESULT l = ::SendMessage((HWND)wParam, WM_HUBBLE_DATA, 0, 20190214);
		if (l == 0)
		{
			CWinForm* pWnd = new CWinForm();
			g_pCosmos->m_hFormNodeWnd = NULL;
			g_pCosmos->m_hFormNodeWnd = (HWND)wParam;
			pWnd->SubclassWindow((HWND)wParam);
			::PostMessage(g_pCosmos->m_hFormNodeWnd, WM_WINFORMCREATED, 0, 0);
		}
	}
	break;
	switch (lParam)
	{
	break;
	default:
		break;
	}
	return 1;
	break;
	case WM_HUBBLE_INIT:
		//if (lParam == 20002000)
		//{
		//	g_pCosmos->CosmosInit();
		//}
		//break;
		case WM_HUBBLE_APPQUIT:
		{
			::PostAppMessage(::GetCurrentThreadId(), WM_QUIT, 0, 0);
		}
		break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CUniverse::CosmosExtendedWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_WINDOWPOSCHANGED:
	{
		HWND m_hChildWnd = (HWND)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (::IsWindow(m_hChildWnd) && ::IsWindowVisible(hWnd)) {
			RECT rc;
			::GetClientRect(m_hChildWnd, &rc);
			WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
			if (rc.right != lpwndpos->cx || rc.bottom != lpwndpos->cy)
				::SetWindowPos(m_hChildWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_FRAMECHANGED | SWP_NOREDRAW | SWP_NOACTIVATE);
		}
	}
	break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CUniverse::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (g_pCosmos == nullptr)
		return 0;
	LRESULT hr = CallNextHookEx(g_pCosmos->m_hCBTHook, nCode, wParam, lParam);
	HWND hWnd = (HWND)wParam;
	switch (nCode)
	{
	case HCBT_CREATEWND:
	{
		CBT_CREATEWND* pCreateWnd = (CBT_CREATEWND*)lParam;
		LPCTSTR lpszName = pCreateWnd->lpcs->lpszName;
		HWND hPWnd = pCreateWnd->lpcs->hwndParent;
		DWORD dwID = (DWORD)pCreateWnd->lpcs->hMenu;

		::GetClassName(hWnd, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strClassName = g_pCosmos->m_szBuffer;
		memset(g_pCosmos->m_szBuffer, 0, sizeof(g_pCosmos->m_szBuffer));
		::GetClassName(hPWnd, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strPClassName = g_pCosmos->m_szBuffer;
		memset(g_pCosmos->m_szBuffer, 0, sizeof(g_pCosmos->m_szBuffer));

		CosmosFrameWndInfo* pCosmosFrameWndInfo = nullptr;
		if (strClassName == _T("MDIClient"))
		{
			HANDLE hHandle = ::GetProp(hPWnd, _T("CosmosFrameWndInfo"));
			if (hHandle == 0)
			{
				pCosmosFrameWndInfo = new CosmosFrameWndInfo();
				::SetProp(hPWnd, _T("CosmosFrameWndInfo"), pCosmosFrameWndInfo);
				g_pCosmos->m_mapCosmosFrameWndInfo[hPWnd] = pCosmosFrameWndInfo;
			}
			else
			{
				pCosmosFrameWndInfo = (CosmosFrameWndInfo*)hHandle;
			}
			pCosmosFrameWndInfo->m_hClient = hWnd;
			if (g_pCosmos->m_pCosmosDelegate)
				g_pCosmos->m_pCosmosDelegate->AppWindowCreated(_T("MDIClient"),hPWnd, hWnd);
		}
		else if (strClassName.Find(_T("Afx:ControlBar:")) == 0)
		{
			HANDLE hHandle = ::GetProp(hPWnd, _T("CosmosFrameWndInfo"));
			if (hHandle == 0)
			{
				pCosmosFrameWndInfo = new CosmosFrameWndInfo();
				::SetProp(hPWnd, _T("CosmosFrameWndInfo"), pCosmosFrameWndInfo);
				g_pCosmos->m_mapCosmosFrameWndInfo[hPWnd] = pCosmosFrameWndInfo;
			}
			else
			{
				pCosmosFrameWndInfo = (CosmosFrameWndInfo*)hHandle;
			}
			::SetProp(hWnd, _T("CosmosFrameWndInfo"), pCosmosFrameWndInfo);
			::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)hWnd, 20210105);
			if (g_pCosmos->m_pCosmosDelegate)
				g_pCosmos->m_pCosmosDelegate->AppWindowCreated(_T("Afx:ControlBar"), hPWnd, hWnd);
		}
		else if (strClassName.Find(_T("Afx:MiniFrame:")) == 0)
		{
			if (g_pCosmos->m_pCosmosDelegate)
				g_pCosmos->m_pCosmosDelegate->AppWindowCreated(_T("Afx:MiniFrame"), hPWnd, hWnd);
		}
		else if (strClassName.Find(_T("Afx:RibbonBar:")) == 0)
		{
			if (g_pCosmos->m_pCosmosDelegate)
				g_pCosmos->m_pCosmosDelegate->AppWindowCreated(_T("Afx:RibbonBar"), hPWnd, hWnd);
		}
		else if (strClassName == _T("Chrome_RenderWidgetHostHWND"))
		{
			if ((::GetWindowLong(hPWnd, GWL_STYLE) & WS_POPUP) == 0)
			{
				auto it = g_pCosmos->m_mapBrowserWnd.find(hPWnd);
				if (it == g_pCosmos->m_mapBrowserWnd.end()) {
					CBrowser* pChromeBrowserWnd = new CComObject<CBrowser>();
					pChromeBrowserWnd->SubclassWindow(hPWnd);
					g_pCosmos->m_mapBrowserWnd[hPWnd] = pChromeBrowserWnd;
					pChromeBrowserWnd->m_pBrowser = g_pCosmos->m_pActiveBrowser;
					if (pChromeBrowserWnd->m_pBrowser)
						pChromeBrowserWnd->m_pBrowser->m_pProxy = pChromeBrowserWnd;
					if (g_pCosmos->m_pCosmosDelegate)
						g_pCosmos->m_pCosmosDelegate->m_bBrowserWndCreated = true;
				}
				::PostMessage(hPWnd, WM_COSMOSMSG, 0, (LPARAM)hWnd);
			}
			if (g_pCosmos->m_pCosmosDelegate)
				g_pCosmos->m_pCosmosDelegate->AppWindowCreated(_T("Chrome_RenderWidgetHostHWND"), hPWnd, hWnd);
		}
		else if (strClassName.Find(_T("SysTreeView32")) == 0 || strClassName.Find(_T("SysTabControl32")) == 0 || strClassName.Find(_T("SysListView32")) == 0)
		{
			::PostMessage(hWnd, WM_XOBJCREATED, 0, 20210108);
			if (g_pCosmos->m_pCosmosDelegate)
				g_pCosmos->m_pCosmosDelegate->AppWindowCreated(strClassName, hPWnd, hWnd);
		}

		if (HIWORD(pCreateWnd->lpcs->lpszClass))
		{
			if (g_pCosmos->m_pCLRProxy)
			{
				g_pCosmos->m_pCLRProxy->WindowCreated(strClassName, lpszName, hPWnd, hWnd);
			}
		}
		if ((pCreateWnd->lpcs->style & WS_CHILD) == 0)
		{
			LRESULT lRes = ::SendMessage(hWnd, WM_QUERYAPPPROXY, 0, 0);
			if (lRes > 0)
			{
				g_pCosmos->m_pActiveAppProxy = (IUniverseAppProxy*)lRes;
			}
		}
	}
	break;
	case HCBT_DESTROYWND:
	{
		if (g_pCosmos->m_pCLRProxy)
			g_pCosmos->m_pCLRProxy->WindowDestroy(hWnd);

		auto it = g_pCosmos->m_mapGalaxy2GalaxyCluster.find(hWnd);
		if (it != g_pCosmos->m_mapGalaxy2GalaxyCluster.end())
			g_pCosmos->m_mapGalaxy2GalaxyCluster.erase(it);

		auto itXobj = g_pCosmos->m_mapXobj.find(hWnd);
		if (itXobj != g_pCosmos->m_mapXobj.end())
		{
			g_pCosmos->m_mapXobj.erase(itXobj);
		}
		auto it1 = g_pCosmos->m_mapUIData.find(hWnd);
		if (it1 != g_pCosmos->m_mapUIData.end())
			g_pCosmos->m_mapUIData.erase(it1);
		it1 = g_pCosmos->m_mapCtrlTag.find(hWnd);
		if (it1 != g_pCosmos->m_mapCtrlTag.end())
			g_pCosmos->m_mapCtrlTag.erase(it1);
		auto it4 = g_pCosmos->m_mapCosmosFrameWndInfo.find(hWnd);
		if (it4 != g_pCosmos->m_mapCosmosFrameWndInfo.end())
		{
			delete it4->second;
			g_pCosmos->m_mapCosmosFrameWndInfo.erase(it4);
		}

		if (hWnd == g_pCosmos->m_hMainWnd)
		{
			if (theUniverse.m_bHostCLR && g_pCosmos->m_nAppType == APP_BROWSERAPP)
				g_pCosmos->m_nAppType = APP_BROWSER;
			::DestroyWindow(g_pCosmos->m_hHostWnd);
			if (theUniverse.m_bHostCLR && g_pCosmos->m_nAppType == 0)
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
		g_pCosmos->m_hActiveWnd = hWnd;
		if (g_pCosmos->m_pCLRProxy)
		{
			g_pCosmos->m_bWinFormActived = g_pCosmos->m_pCLRProxy->IsWinForm(hWnd);
			if (g_pCosmos->m_bWinFormActived)
			{
				g_pCosmos->m_pGalaxy = nullptr;
			}
		}
		LRESULT lRes = ::SendMessage(hWnd, WM_QUERYAPPPROXY, 0, 0);
		if (lRes > 0)
		{
			g_pCosmos->m_pActiveAppProxy = (IUniverseAppProxy*)lRes;
		}
		else
			g_pCosmos->m_pActiveAppProxy = nullptr;
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
	CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
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
				if (::GetCurrentThreadId() == g_pCosmos->m_dwThreadID)
				{
					if (::IsWindow(g_pCosmos->m_hHostWnd))
						::DestroyWindow(g_pCosmos->m_hHostWnd);
					if (::IsWindow(g_pCosmos->m_hCosmosWnd))
						::DestroyWindow(g_pCosmos->m_hCosmosWnd);
				}
				if (g_pCosmos->m_pCosmosAppProxy)
					g_pCosmos->m_pCosmosAppProxy->OnCosmosClose();
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
					g_pCosmos->m_pCosmosDelegate &&
					g_pCosmos->m_pCosmosDelegate->OnUniversePreTranslateMessage(lpMsg))
				{
					return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
				}
			}
			break;
			case WM_KEYDOWN:
			{
				CXobjHelper* pWnd = nullptr;
				if (g_pCosmos->m_bOMNIBOXPOPUPVISIBLE && lpMsg->wParam == VK_RETURN)
				{
					g_bRecturnPressed = true;
				}
				if (g_pCosmos->m_pActiveXobj)
				{
					if (g_pCosmos->m_pActiveXobj->m_nViewType != Grid)
					{
						pWnd = (CXobjHelper*)g_pCosmos->m_pActiveXobj->m_pHostWnd;
						if (pWnd && ::IsChild(pWnd->m_hWnd, lpMsg->hwnd) == false)
						{
							g_pCosmos->m_pActiveXobj = nullptr;
							if (lpMsg->wParam != VK_TAB)
								break;
							else if (g_pCosmos->m_bWinFormActived == false)
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
									g_pCosmos->m_pGalaxy = nullptr;
							}
						}
					}
				}

				switch (lpMsg->wParam)
				{
				case VK_TAB:
					if (g_pCosmos->m_bWinFormActived && g_pCosmos->m_bEnableProcessFormTabKey && g_pCosmos->m_pCLRProxy->ProcessFormMsg(g_pCosmos->m_hActiveWnd, lpMsg, 0))
					{
						break;
					}
					if (g_pCosmos->m_pGalaxy && g_pCosmos->m_pActiveXobj && pWnd && pWnd->PreTranslateMessage(lpMsg))
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
							if (g_pCosmos->m_pActiveHtmlWnd)
							{
								HWND hWnd = ::GetParent(::GetParent(g_pCosmos->m_pActiveHtmlWnd->m_hWnd));
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
						if (((__int64)g_pCosmos->m_pActiveAppProxy) > 1)
							g_pCosmos->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
						//else if (((__int64)g_pCosmos->m_pUniverseAppProxy) > 1)
						//{
						//	g_pCosmos->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
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
					if (g_pCosmos->m_bWinFormActived && g_pCosmos->m_bEnableProcessFormTabKey && g_pCosmos->m_pCLRProxy->ProcessFormMsg(g_pCosmos->m_hActiveWnd, lpMsg, 0))
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
					if (g_pCosmos->m_pGalaxy && g_pCosmos->m_pActiveXobj && pWnd && pWnd->PreTranslateMessage(lpMsg))
					{
						if (g_pCosmos->m_pCLRProxy && g_pCosmos->m_pCLRProxy->IsWinForm(pWnd->m_hWnd))
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
					if (g_pCosmos->m_pCosmosDelegate)
					{
						if (g_pCosmos->m_pCosmosDelegate->OnUniversePreTranslateMessage(lpMsg))
							break;
					}
					break;
				case VK_DELETE:
					if (g_pCosmos->m_pActiveXobj)
					{
						if (g_pCosmos->m_pActiveXobj->m_nViewType == ActiveX)
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
					if (g_pCosmos->m_pCosmosDelegate)
					{
						if (g_pCosmos->m_pCosmosDelegate->OnUniversePreTranslateMessage(lpMsg))
							break;
					}
					if (g_pCosmos->m_pGalaxy && g_pCosmos->m_pActiveXobj)
					{
						if (pWnd && ::IsChild(pWnd->m_hWnd, lpMsg->hwnd) == false)
						{
							g_pCosmos->m_pActiveXobj = nullptr;
							g_pCosmos->m_pGalaxy = nullptr;
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
					if (g_pCosmos->m_bOfficeApp)
						return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
					TranslateMessage(lpMsg);
					if (g_pCosmos->m_strExeName != _T("devenv"))
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
						if (g_pCosmos->m_pActiveXobj && pWnd && !::IsWindow(pWnd->m_hWnd))
						{
							g_pCosmos->m_pActiveXobj = nullptr;
						}
						if (g_pCosmos->m_pActiveXobj)
						{
							HWND hWnd = nullptr;
							if (pWnd)
								hWnd = pWnd->m_hWnd;
							if (g_pCosmos->m_pActiveXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
							{
								if (pWnd)
									pWnd->PreTranslateMessage(lpMsg);
								lpMsg->hwnd = NULL;
								lpMsg->wParam = 0;
								break;
							}
							if (g_pCosmos->m_pActiveHtmlWnd)
							{
								HWND hwnd = lpMsg->hwnd;
								HWND hWnd = ::GetParent(::GetParent(g_pCosmos->m_pActiveHtmlWnd->m_hWnd));
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
			case WM_HUBBLE_INIT:
			{
				if (lpMsg->wParam == 20191005)
					g_pCosmos->Init();
			}
			break;
			case WM_MOUSEMOVE:
			{
				if ((long)(g_pCosmos->m_pActiveAppProxy) > 1)
				{
					g_pCosmos->m_pActiveAppProxy->MouseMoveProxy(lpMsg->hwnd);
				}
				else if (g_pCosmos->m_pUniverseAppProxy)
				{
					g_pCosmos->m_pUniverseAppProxy->MouseMoveProxy(lpMsg->hwnd);
				}
				if (g_pCosmos->m_pCosmosDelegate)
				{
					if (g_pCosmos->m_pCosmosDelegate->OnUniversePreTranslateMessage(lpMsg))
						break;
				}
				if ((long)(g_pCosmos->m_pActiveAppProxy) > 1)
					g_pCosmos->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
				else if (g_pCosmos->m_pUniverseAppProxy)
					g_pCosmos->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
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
				g_pCosmos->ProcessMsg(lpMsg);
				//::DispatchMessage(lpMsg);
				//for m_strStartupCLRObj support
				//if (g_pCosmos->m_pActiveWinFormWnd && g_pCosmos->m_bEnableProcessFormTabKey && g_pCosmos->m_pCLRProxy->ProcessFormMsg(g_pCosmos->m_pActiveWinFormWnd->m_hWnd, lpMsg, 0))
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
					if (g_pCosmos->m_pActiveWinFormWnd)
					{
						//::SendMessage(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
						g_pCosmos->m_pCLRProxy->ProcessFormMsg(g_pCosmos->m_pActiveWinFormWnd->m_hWnd, lpMsg, 0);
					}
				}
				if ((long)(g_pCosmos->m_pActiveAppProxy) > 1)
					g_pCosmos->m_pActiveAppProxy->UniversePreTranslateMessage(lpMsg);
				else if (g_pCosmos->m_pUniverseAppProxy)
					g_pCosmos->m_pUniverseAppProxy->UniversePreTranslateMessage(lpMsg);
				if (g_pCosmos->m_pCosmosDelegate)
				{
					if (g_pCosmos->m_pCosmosDelegate->OnUniversePreTranslateMessage(lpMsg))
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
						HWND hWnd = g_pCosmos->m_pCosmosDelegate->GetMainWnd();
						if (::IsWindow(hWnd))
						{
							g_pCosmos->m_hMainWnd = hWnd;
						}

						FuncInitApp(false);

						::PostQuitMessage(0);
					}
				}
			}
			break;
			case WM_CHROMEDEVTOOLMSG:
			{
				switch (lpMsg->wParam)
				{
				case 1:
				{
					g_pCosmos->m_bCreatingDevTool = true;
					auto it = g_pCosmos->m_mapBrowserWnd.find(::GetActiveWindow());
					if (it != g_pCosmos->m_mapBrowserWnd.end())
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
				case 20210105:
				{
					HWND hWnd = (HWND)lpMsg->wParam;
					if (::IsWindow(hWnd))
					{
						HANDLE nHandle = ::RemoveProp(hWnd, _T("CosmosFrameWndInfo"));
						if (nHandle)
						{
							CosmosFrameWndInfo* pInfo = (CosmosFrameWndInfo*)nHandle;
							TCHAR szBuffer[MAX_PATH];
							memset(szBuffer, 0, sizeof(szBuffer));
							::GetWindowText(hWnd, szBuffer, 256);
							CString strCaption = szBuffer;
							if (strCaption != _T(""))
							{
								auto it = pInfo->m_mapCtrlBar.find(strCaption);
								if (it != pInfo->m_mapCtrlBar.end())
								{
									HWND h = it->second;
									if (::IsChild(hWnd, h) == false)
										pInfo->m_mapCtrlBar[strCaption] = hWnd;
								}
								else
									pInfo->m_mapCtrlBar[strCaption] = hWnd;
							}
						}
					}
				}
				break;
				case 20201028:
				{
					if (g_pCosmos->m_hTempBrowserWnd)
					{
						g_pCosmos->m_hTempBrowserWnd=NULL;
					}
				}
				break;
				case 20191004:
				{
					if (g_pCosmos->m_pCLRProxy)
					{
						g_pCosmos->m_pCLRProxy->CosmosAction(CComBSTR("setmainform"), nullptr);
					}
				}
				break;
				case 20190511:
				{
					::DestroyWindow(g_pCosmos->m_hHostWnd);
				}
				break;
				case 2019111701:
				{
					if (g_pCosmos->m_mapXobjForHtml.size())
					{
						for (auto it : g_pCosmos->m_mapXobjForHtml)
						{
							it.first->put_URL(CComBSTR(it.second));
						}
						g_pCosmos->m_mapXobjForHtml.erase(g_pCosmos->m_mapXobjForHtml.begin(), g_pCosmos->m_mapXobjForHtml.end());
					}
				}
				break;
				case 20191022:
				{
					if (g_pCosmos->m_pCLRProxy)
					{
						g_pCosmos->m_pCLRProxy->CosmosAction(CComBSTR("startclrapp"), nullptr);
					}
				}
				break;
				}
			}
			break;
			case WM_HUBBLE_APPQUIT:
			{
				TRACE(_T("======== WM_HUBBLE_APPQUIT=========\n"));
				if (g_pCosmos->m_mapBrowserWnd.size())
				{
					g_pCosmos->m_bChromeNeedClosed = true;
					auto it = g_pCosmos->m_mapBrowserWnd.begin();
					((CBrowser*)it->second)->SendMessageW(WM_CLOSE, 0, 0);
				}
			}
			break;
			case WM_CHROMEOMNIBOXPOPUPVISIBLE:
			{
				g_pCosmos->m_bOMNIBOXPOPUPVISIBLE = lpMsg->lParam ? true : false;
				auto it = g_pCosmos->m_mapBrowserWnd.find((HWND)lpMsg->wParam);
				if (it != g_pCosmos->m_mapBrowserWnd.end())
				{
					CWebPage* pWnd = ((CBrowser*)it->second)->m_pVisibleWebWnd;
					if (pWnd && ::IsWindow(pWnd->m_hWnd) && pWnd->m_pGalaxy)
					{
						IXobj* pXobj = nullptr;
						if (g_bRecturnPressed == false)
						{
							pWnd->m_pGalaxy->Observe(CComBSTR(lpMsg->lParam ? _T("__default__key__for__chrome__") : pWnd->m_strCurKey), CComBSTR(lpMsg->lParam ? g_pCosmos->m_strDefaultXml : _T("")), &pXobj);
							::SendMessage(it->first, WM_BROWSERLAYOUT, 0, 4);
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
	CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(ThreadID);
	if (pThreadInfo && pThreadInfo->m_hGetMessageHook == NULL)
	{
		pThreadInfo->m_hGetMessageHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, NULL, ThreadID);
	}
}
