/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103120049           *
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

 // TangramApp.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "UniverseApp.h" 
#include <VersionHelpers.h> 
#include <shellscalingapi.h>

#include "fm20.h"
#include "EclipsePlus\EclipseAddin.h"

#include "OfficePlus\WordPlus\WordAddin.h"
#include "OfficePlus\ExcelPlus\ExcelAddin.h"
#include "OfficePlus\OutLookPlus\OutLookAddin.h"
#include "OfficePlus\ProjectPlus\ProjectAddin.h"
#include "OfficePlus\PowerPointPlus\PowerPointAddin.h"

#include "XobjWnd.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "WPFView.h"
#include <io.h>
#include <stdio.h>

#include "TangramListView.h"
#include "Psapi.h"

#include "chromium\Browser.h"
#include "chromium\WebPage.h"

using namespace OfficePlus;
using namespace OfficePlus::WordPlus;
using namespace OfficePlus::ExcelPlus;

// Description  : the unique App object
CUniverse theApp;
CCosmos* g_pCosmos = nullptr;

void CHelperWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	auto it = g_pCosmos->m_mapRemoteCosmos.find(m_strID);
	if (it != g_pCosmos->m_mapRemoteCosmos.end())
	{
		ULONG dw = it->second->Release();
		while (dw)
			dw = it->second->Release();
		g_pCosmos->m_mapRemoteCosmos.erase(m_strID);
	}

	delete this;
}

void CWebHelperWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CWebHelperWnd::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
	if (lpwndpos->flags & SWP_HIDEWINDOW)
	{
		ATLTRACE(_T("1\n"));
	}
	else if (lpwndpos->flags & SWP_SHOWWINDOW)
	{
		ATLTRACE(_T("2\n"));
	}
	return lRes;
}

LRESULT CWebHelperWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	if (wParam) {
	}
	else
	{
	}
	return lRes;
}

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
	CString path = m_szBuffer;
	int nPos = path.ReverseFind('\\');
	CString strName = path.Mid(nPos + 1);
	nPos = strName.Find(_T("."));
	CString strExeName = strName.Left(nPos);
	strExeName.MakeLower();
	if (strExeName == _T("regsvr32"))
		return true;

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(3888);

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	::OleInitialize(NULL);
	BOOL bOfficeApp = false;

	m_bHostCLR = (BOOL)::GetModuleHandle(_T("mscoreei.dll"));
	CString strExes = _T("");
	HMODULE hModule = ::GetModuleHandle(_T("mso.dll"));
	if (hModule)
	{
		strExes = _T("winword,excel,powerpnt,outlook,msaccess,infopath,winproj,onenote,visio,");
		bOfficeApp = true;
	}
	if (bOfficeApp)
	{
		COfficeAddin* pOfficeAddin = (COfficeAddin*)this;
		nPos = strExes.Find(strExeName);
		if (nPos != -1)
		{
			int nAppID = strExes.Left(nPos).Replace(_T(","), _T(""));
			switch (nAppID)
			{
			case 0:
				new CComObject < WordPlus::CWordAddin >;
				break;
			case 1:
				new CComObject < ExcelPlus::CExcelAddin >;
				break;
			case 2:
				new CComObject < PowerPointPlus::CPowerPntAddin >;
				break;
			case 3:
				new CComObject < OutLookPlus::COutLookAddin >;
				break;
			case 4:
				break;
			case 6:
				new CComObject < ProjectPlus::CProjectAddin >;
				break;
				//case 5:
				//	m_pCosmos = new CComObject < InfoPathPlus::CInfoPathCloudAddin >;
				//	break;
			case 7:
				new CComObject < OfficePlus::COfficeAddin >;
				break;
			case 8:
				break;
			}
			g_pCosmos->m_nAppID = nAppID;
		}
	}
	else
	{
#ifndef _WIN64
		{
			new CComObject < CCosmos >;
			g_pCosmos->m_strExeName = strExeName;
			g_pCosmos->m_dwThreadID = ::GetCurrentThreadId();
			if (g_pCosmos->m_hCBTHook == nullptr)
				g_pCosmos->m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, g_pCosmos->m_dwThreadID);
			theApp.SetHook(g_pCosmos->m_dwThreadID);
		}
#else
		new CComObject < CCosmos >;
		g_pCosmos->m_strExeName = strExeName;
		g_pCosmos->m_dwThreadID = ::GetCurrentThreadId();
		if (g_pCosmos->m_hCBTHook == nullptr)
			g_pCosmos->m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, g_pCosmos->m_dwThreadID);
		g_pCosmos->m_bEnableProcessFormTabKey = true;
		theApp.SetHook(g_pCosmos->m_dwThreadID);
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
		theApp.SetHook(g_pCosmos->m_dwThreadID);
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
			bool bSupportEclipse = (::PathFileExists(strInitEclipse) && !::PathIsDirectory(strInitEclipse));
			if (bSupportEclipse)
			{
				g_pCosmos->m_bEclipse = false;
				CString strplugins = g_pCosmos->m_strAppPath + _T("plugins\\");
				if (::PathIsDirectory(strplugins)) {
					CString strPath = strplugins + _T("*.jar");
					_wfinddata_t fd;
					fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
					intptr_t pf = _wfindfirst(strPath, &fd);
					if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
						g_pCosmos->m_bEclipse = true;
					}
					else {
						while (!_wfindnext(pf, &fd)) {
							if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
								g_pCosmos->m_bEclipse = true;
								break;
							}
						}
					}
					_findclose(pf);
				}
				bSupportEclipse = g_pCosmos->m_bEclipse;
			}
			if (bHasChromeRT && bSupportBrowser && bSupportEclipse)
			{
				g_pCosmos->m_strStartupURL = strInitWebPage;
				g_pCosmos->m_strStartView = strInitEclipse;
				g_pCosmos->m_nAppType = APP_BROWSER_ECLIPSE;
			}
			if (g_pCosmos->m_strStartupURL == _T(""))
			{
				if (bSupportEclipse)
				{
					g_pCosmos->m_nAppType = APP_ECLIPSE;
					g_pCosmos->m_strStartupURL = strInitEclipse;
				}
				else
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
			for (auto it : g_pCosmos->m_mapBKFrame)
			{
				HWND hWnd = ::GetParent(it.first);
				IGalaxy* pGalaxy = nullptr;
				g_pCosmos->GetGalaxy((__int64)::GetParent(hWnd), &pGalaxy);
				CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
				if (_pGalaxy)
					_pGalaxy->m_pBKWnd = nullptr;
				::DestroyWindow(::GetParent(it.first));
			}

			if (g_pCosmos->m_pCLRProxy)
			{
				if (g_pCosmos->m_pCosmosAppProxy)
					g_pCosmos->m_pCosmosAppProxy->OnCosmosClose();
				if (g_pCosmos->m_pCLRProxy)
					g_pCosmos->m_pCLRProxy->CosmosAction(_T("<begin_quit_eclipse/>"), nullptr);
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
	case WM_WINDOWPOSCHANGED:
		if (hWnd == g_pCosmos->m_hHostWnd)
		{
			RECT rc;
			::GetClientRect(g_pCosmos->m_hHostWnd, &rc);
			::SetWindowPos(g_pCosmos->m_hChildHostWnd, NULL, 0, 0, rc.right, rc.bottom, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
		else if (hWnd == g_pCosmos->m_hTemplateWnd)
		{
			RECT rc;
			::GetClientRect(g_pCosmos->m_hTemplateWnd, &rc);
			::SetWindowPos(g_pCosmos->m_hTemplateChildWnd, NULL, 0, 0, rc.right, rc.bottom, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
		break;
	case WM_OPENDOCUMENT:
	{
		g_pCosmos->OnOpenDoc(wParam);
	}
	break;
	case WM_INITOUTLOOK:
	{
		((OfficePlus::OutLookPlus::COutLookAddin*)g_pCosmos)->InitOutLook();
	}
	break;
	case WM_OFFICEOBJECTCREATED:
	{
		HWND hWnd = (HWND)wParam;
		if (::IsWindow(hWnd))
			((OfficePlus::COfficeAddin*)g_pCosmos)->ConnectOfficeObj(hWnd);
	}
	break;
	case WM_COSMOSMSG:
		switch (lParam)
		{
		case 1222:
		{
			if (wParam == 1963)
			{
				OfficePlus::OutLookPlus::COutLookAddin* pAddin = (OfficePlus::OutLookPlus::COutLookAddin*)g_pCosmos;
				if (pAddin->m_pActiveOutlookExplorer)
				{
					pAddin->m_pActiveOutlookExplorer->SetDesignState();
				}
			}
		}
		break;
		case TANGRAM_CHROME_APP_INIT:
		{
			if (g_pCosmos->m_nAppType == APP_BROWSER_ECLIPSE || g_pCosmos->m_bEclipse)
			{
				ICosmosCLRImpl* pProxy = g_pCosmos->m_pCLRProxy;
				g_pCosmos->InitEclipseApp();
				if (pProxy)
				{
					pProxy->CosmosAction(CComBSTR("EndInitEclipseApp"), nullptr);
				}
			}
			else if (g_pCosmos->m_hMainWnd == NULL && g_pCosmos->m_pUniverseAppProxy)
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
			::InvalidateRect(hWnd, nullptr, true);
		}
	}
	break;
	case WM_HUBBLE_NEWOUTLOOKOBJ:
	{
		using namespace OfficePlus::OutLookPlus;
		int nType = wParam;
		HWND hWnd = ::GetActiveWindow();
		if (nType)
		{
			COutLookExplorer* pOutLookPlusItemWindow = (COutLookExplorer*)lParam;
			COutLookAddin* pAddin = (COutLookAddin*)g_pCosmos;
			pOutLookPlusItemWindow->m_strKey = pAddin->m_strCurrentKey;
			pAddin->m_mapOutLookPlusExplorerMap[hWnd] = pOutLookPlusItemWindow;
			pOutLookPlusItemWindow->m_hWnd = hWnd;
		}
	}
	break;
	case WM_HUBBLE_ACTIVEINSPECTORPAGE:
	{
		using namespace OfficePlus::OutLookPlus;
		COutLookInspector* pOutLookPlusItemWindow = (COutLookInspector*)wParam;
		pOutLookPlusItemWindow->ActivePage();
	}
	break;
	case WM_HUBBLE_ITEMLOAD:
	{
		using namespace OfficePlus::OutLookPlus;
		COutLookAddin* pAddin = (COutLookAddin*)g_pCosmos;
		HWND hWnd = ::GetActiveWindow();
		auto it = pAddin->m_mapOutLookPlusExplorerMap.find(hWnd);
		if (it != pAddin->m_mapOutLookPlusExplorerMap.end())
		{
			COutLookExplorer* pExplorer = it->second;
			if (pExplorer->m_pInspectorContainerWnd == nullptr)
			{
				HWND _hWnd = ::FindWindowEx(hWnd, NULL, _T("rctrl_renwnd32"), NULL);
				if (_hWnd)
				{
					_hWnd = ::FindWindowEx(_hWnd, NULL, _T("AfxWndW"), NULL);
					if (_hWnd)
					{
						pExplorer->m_pInspectorContainerWnd = new CInspectorContainerWnd();
						pExplorer->m_pInspectorContainerWnd->SubclassWindow(_hWnd);
					}
				}
			}

			long nKey = wParam;
			auto it = pAddin->m_mapCosmosInspectorItem.find(nKey);
			if (it != pAddin->m_mapCosmosInspectorItem.end())
			{
				CInspectorItem* pItem = (CInspectorItem*)wParam;
				if (pExplorer->m_pInspectorContainerWnd)
				{
					pExplorer->m_pInspectorContainerWnd->m_strXml = pItem->m_strXml;
					::PostMessage(pExplorer->m_pInspectorContainerWnd->m_hWnd, WM_HUBBLE_ITEMLOAD, 0, 0);
				}
			}
		}
	}
	break;
	case WM_HUBBLE_APPINIT:
	{
		if (g_pCosmos->m_bEclipse)
		{
			if (wParam == 1963 && lParam == 1222)
			{
				break;
			}

			if (g_pCosmos->m_pActiveEclipseWnd)
			{
				IXobj* pXobj = nullptr;
				if (g_pCosmos->m_strStartView == _T("TopView"))
				{
					g_pCosmos->m_pActiveEclipseWnd->Observe(_T("Start"), g_pCosmos->m_strStartXml.AllocSysString(), &pXobj);
				}
				else if (g_pCosmos->m_strStartView == _T("EclipseView"))
				{
					IEclipseCtrl* pCtrl = nullptr;
					g_pCosmos->m_pActiveEclipseWnd->get_Ctrl(CComVariant((long)0), &pCtrl);
					if (pCtrl)
					{
						pCtrl->Observe(CComBSTR(L"EclipseView"), CComBSTR(L"Start"), g_pCosmos->m_strStartXml.AllocSysString(), &pXobj);
					}
				}
			}
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
			//g_pCosmos->CosmosInit();
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
						for (auto it3 : it2.second->m_mapWPFView)
						{
							ATLTRACE(_T("HWND %x, WM_POWERBROADCAST\n"), it3.second->m_hWnd);
							::SetWindowLongPtr(it3.second->m_hWnd, GWLP_USERDATA, 1963);
						}
					}
				}
			}
			for (auto& it : g_pCosmos->m_mapBrowserWnd)
			{
				if (::IsWindowVisible(it.first))
				{
					CBrowser* pWnd = (CBrowser*)it.second;
					if (pWnd)
					{
						HWND hWnd = pWnd->m_pBrowser->GetActiveWebContentWnd();
						if (hWnd)
						{
							auto it1 = g_pCosmos->m_mapHtmlWnd.find(hWnd);
							if (it1 != g_pCosmos->m_mapHtmlWnd.end())
							{
								pWnd->m_pVisibleWebWnd = (CWebPage*)it1->second;
								it1->second->m_pChromeRenderFrameHost->ShowWebPage(true);
							}
						}
						::PostMessage(hWnd, WM_COSMOSMSG, 20200131, 0);
					}
					::PostMessage(it.first, WM_BROWSERLAYOUT, 0, 4);
				}
			}
			if (g_pCosmos->m_pMDIMainWnd && g_pCosmos->m_pMDIMainWnd->m_pGalaxy)
				g_pCosmos->m_pMDIMainWnd->m_pGalaxy->HostPosChanged();
		}
		break;
		}
	}
	break;
	case WM_COSMOSMSG:
	switch (lParam)
	{
	case 10001000:
	{
		if (g_pCosmos->m_nAppID != 9 && g_pCosmos->m_bEclipse == false && g_pCosmos->m_bOfficeApp == false)
		{
			::PostMessage(g_pCosmos->m_hCosmosWnd, WM_HUBBLE_APPQUIT, 0, 0);
		}
	}
	break;
	default:
		break;
	}
	return 1;
	break;
	case WM_HUBBLE_APPQUIT:
	{
		if (g_pCosmos->m_bEclipse == false && g_pCosmos->m_bOfficeApp == false)
		{
			if (g_pCosmos->m_pMDIMainWnd == NULL)
			{
				::PostAppMessage(::GetCurrentThreadId(), WM_QUIT, 0, 0);
			}
		}
	}
	break;
	case WM_ECLIPSEWORKBENCHCREATED:
	{
		HWND hEclipseWorkBench = (HWND)lParam;
		if (wParam || ::IsMenu(::GetMenu(hEclipseWorkBench)))
		{
			auto it = g_pCosmos->m_mapWorkBenchWnd.find(hEclipseWorkBench);
			if (it == g_pCosmos->m_mapWorkBenchWnd.end())
			{
				if (g_pCosmos->m_hEclipseHideWnd == nullptr)
				{
					for (auto it : g_pCosmos->m_vecEclipseHideTopWnd)
					{
						::GetWindowText(it, g_pCosmos->m_szBuffer, MAX_PATH);
						CString strText = CString(g_pCosmos->m_szBuffer);
						if (strText.CompareNoCase(_T("PartRenderingEngine's limbo")) == 0)
						{
							g_pCosmos->m_hEclipseHideWnd = it;
							::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 20191117);
							break;
						}
					}
				}

				CEclipseWnd* pEclipseWnd = new CComObject<CEclipseWnd>;
				HWND hParent = GetWindow(hEclipseWorkBench, GW_CHILD);
				HWND hClient = GetWindow(hParent, GW_CHILD);
				pEclipseWnd->m_hClient = hClient;
				if (hClient)
					::SetWindowLongPtr(pEclipseWnd->m_hClient, GWLP_ID, AFX_IDW_PANE_FIRST);

				g_pCosmos->m_mapWorkBenchWnd[hEclipseWorkBench] = pEclipseWnd;
				pEclipseWnd->SubclassWindow(hEclipseWorkBench);
				::PostMessage(hEclipseWorkBench, WM_COSMOSMSG, 19820911, 0);
				pEclipseWnd->CreatePage(true);
			}
		}
	}
	break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CUniverse::CosmosExtendedWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (msg)
	{
	case WM_BROWSERLAYOUT:
	{
		if (lParam == 0)
		{
			HWND m_hChildWnd = (HWND)wParam;
			if (::IsWindow(m_hChildWnd))
			{
				RECT rc;
				::GetClientRect(hWnd, &rc);
				::SetWindowPos(m_hChildWnd, HWND_BOTTOM, 0, 0, rc.right, rc.bottom, /*SWP_NOREDRAW*/SWP_FRAMECHANGED | SWP_NOACTIVATE);
				::SendMessage(hWnd, WM_SETREDRAW, 1, 0);
			}
		}
	}
	break;
	case WM_WINDOWPOSCHANGED:
	{
		HWND m_hChildWnd = (HWND)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (::IsWindow(m_hChildWnd) && ::IsWindowVisible(hWnd))
		{
			RECT rc;
			::GetClientRect(m_hChildWnd, &rc);
			WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
			if (rc.right != lpwndpos->cx || rc.bottom != lpwndpos->cy)
				::SetWindowPos(m_hChildWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, /**/SWP_FRAMECHANGED | SWP_NOREDRAW | SWP_NOACTIVATE);
		}
	}
	break;
	case WM_HUBBLE_DATA:
	{
		switch (lParam)
		{
		case 20200203:
		{
			LRESULT lRes = ::DefWindowProc(hWnd, msg, wParam, lParam);
			CWebPage* m_pHtmlWnd = (CWebPage*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (m_pHtmlWnd->m_pChromeRenderFrameHost)
			{
				IPCMsg* pMsg = (IPCMsg*)wParam;
				m_pHtmlWnd->m_pChromeRenderFrameHost->SendCosmosMessage(pMsg);
				g_pCosmos->m_pCurrentIPCMsg = nullptr;
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

extern HWND    topWindow;

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

		CosmosFrameWndInfo* pCosmosFrameWndInfo = nullptr;
		if (dwID == AFX_IDW_PANE_FIRST)
		{
			if (pCreateWnd->lpcs->lpCreateParams && strClassName != _T("MDIClient"))
			{
				CString strExt = g_pCosmos->m_pUniverseAppProxy->QueryParentInfo(hPWnd, pCreateWnd->lpcs->lpCreateParams);
				if (strExt != _T(""))
				{
					CString strType = g_pCosmos->m_pUniverseAppProxy->m_strCreatingDOCID;
					if (strType == _T(""))
						strType = _T("default");
					auto it = g_pCosmos->m_mapDocTemplate.find(strType);
					if (it != g_pCosmos->m_mapDocTemplate.end())
					{
						g_pCosmos->m_pUniverseAppProxy->SetFrameCaption(hPWnd, strType);
					}
				}
			}
			::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)hWnd, 20210110);
		}
		if (strClassName == _T("MDIClient"))
		{
			if (g_pCosmos->m_pMDIMainWnd == nullptr)
			{
				g_pCosmos->m_pMDIMainWnd = new CMDIParent();
				g_pCosmos->m_pMDIMainWnd->m_hMDIClient = hWnd;
				g_pCosmos->m_pMDIMainWnd->SubclassWindow(hPWnd);
			}
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
			if (pCosmosFrameWndInfo->m_hClient == NULL)
				pCosmosFrameWndInfo->m_hClient = hWnd;
			if (::IsWindow(g_pCosmos->m_hHostWnd) == false)
			{
				g_pCosmos->m_hHostWnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 400, 400, hWnd, 0, theApp.m_hInstance, NULL);
				g_pCosmos->m_hChildHostWnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, g_pCosmos->m_hHostWnd, 0, theApp.m_hInstance, NULL);
			}
		}
		else if (strClassName.Find(_T("Afx:ControlBar:")) == 0)
		{
			if (99 < dwID && dwID < 499)
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
				if (pCosmosFrameWndInfo->m_hClient == NULL)
					pCosmosFrameWndInfo->m_hClient = hWnd;
				pCosmosFrameWndInfo->m_mapCtrlBarWnd[dwID] = hWnd;
				HWND h = ::GetAncestor(hPWnd, GA_ROOT);
				if (::IsWindow(h))
				{
					::PostMessage(g_pCosmos->m_hCosmosWnd, WM_CONTROLBARCREATED, (WPARAM)hWnd, (LPARAM)h);
				}
			}
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
					//if (g_pCosmos->m_pCosmosDelegate)
					//	g_pCosmos->m_pCosmosDelegate->m_bBrowserWndCreated = true;
				}
				::PostMessage(hPWnd, WM_COSMOSMSG, 0, (LPARAM)hWnd);
			}
		}
		else if (strClassName.Find(_T("SysTreeView32")) == 0 || strClassName.Find(_T("SysTabControl32")) == 0 || strClassName.Find(_T("SysListView32")) == 0)
		{
			::PostMessage(hWnd, WM_XOBJCREATED, 0, 20210108);
			CWnd* pWnd = CWnd::FromHandlePermanent(hPWnd);
			if (pWnd == nullptr)
			{
				CCosmosWnd* _pWnd = new CCosmosWnd();
				_pWnd->SubclassWindow(hPWnd);
				_pWnd->m_hClient = hWnd;
			}
			if (strClassName.Find(_T("SysTreeView32")) == 0)
			{
				CCosmosTreeCtrl* pCtrl = new CCosmosTreeCtrl();
				pCtrl->SubclassWindow(hWnd);
			}
			else if (strClassName.Find(_T("SysListView32")) == 0)
			{
				CCosmosListCtrl* pCtrl = new CCosmosListCtrl();
				pCtrl->SubclassWindow(hWnd);
			}
			else if (strClassName.Find(_T("SysTabControl32")) == 0)
			{
				CCosmosTabCtrl* pCtrl = new CCosmosTabCtrl();
				pCtrl->SubclassWindow(hWnd);
			}
		}
		else if (strClassName.Find(_T("Afx:")) == 0 && (pCreateWnd->lpcs->style & WS_POPUP))
		{
		}
		else if (strClassName.Find(_T("#32770")) == 0)
		{
			if (hPWnd && (pCreateWnd->lpcs->style & WS_CHILD))
				::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)hWnd, 20210110);
		}
		if (strClassName == _T("SWT_Window0"))
		{
			if (::IsMenu(::GetMenu(hPWnd)))
				::SendMessage(g_pCosmos->m_hCosmosWnd, WM_ECLIPSEWORKBENCHCREATED, 1, (LPARAM)hPWnd);
			else if (hPWnd == nullptr)
			{
				if (g_pCosmos->m_hEclipseHideWnd == nullptr)
					g_pCosmos->m_vecEclipseHideTopWnd.push_back(hWnd);
				::PostMessage(g_pCosmos->m_hCosmosWnd, WM_ECLIPSEWORKBENCHCREATED, 0, (LPARAM)hWnd);
			}
		}
		else if (HIWORD(pCreateWnd->lpcs->lpszClass))
		{
			g_pCosmos->WindowCreated(strClassName, lpszName, hPWnd, hWnd);
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
		HANDLE hData = RemoveProp(hWnd, _T("CosmosInfo"));
		if (hData)
		{
			CosmosInfo* pInfo = (CosmosInfo*)hData;
			delete pInfo;
		}
		if (g_pCosmos && g_pCosmos->m_bOfficeApp)
			g_pCosmos->WindowDestroy(hWnd);
		else if (g_pCosmos->m_pCLRProxy)
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
			if (theApp.m_bHostCLR && g_pCosmos->m_nAppType == APP_BROWSERAPP)
				g_pCosmos->m_nAppType = APP_BROWSER;
			::DestroyWindow(g_pCosmos->m_hHostWnd);
			if (theApp.m_bHostCLR && g_pCosmos->m_nAppType == 0)
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
	case HCBT_SETFOCUS:
		if (g_pCosmos->m_bOfficeApp && g_pCosmos->m_nAppID != -1)
			((COfficeAddin*)g_pCosmos)->SetFocus(hWnd);
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
			auto it = g_pCosmos->m_mapWorkBenchWnd.find(hWnd);
			if (it != g_pCosmos->m_mapWorkBenchWnd.end())
			{
				CEclipseWnd* pWnd = (CEclipseWnd*)it->second;
				g_pCosmos->m_pActiveAppProxy->OnActiveMainFrame(::GetParent(pWnd->m_hClient));
			}
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
				CXobjWnd* pWnd = nullptr;
				CWPFView* pView = nullptr;
				if (g_pCosmos->m_bOMNIBOXPOPUPVISIBLE && lpMsg->wParam == VK_RETURN)
				{
					g_bRecturnPressed = true;
				}
				if (g_pCosmos->m_pActiveXobj)
				{
					if (g_pCosmos->m_pActiveXobj->m_nViewType != Grid)
					{
						if (g_pCosmos->m_pActiveXobj->m_nViewType == TangramWPFCtrl)
							pView = (CWPFView*)g_pCosmos->m_pActiveXobj->m_pHostWnd;
						else
							pWnd = (CXobjWnd*)g_pCosmos->m_pActiveXobj->m_pHostWnd;
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
								else if (pView)
								{
									if (pView->PreTranslateMessage(lpMsg))
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
					if (/*g_pCosmos->m_pGalaxy &&*/ g_pCosmos->m_pActiveXobj && pWnd && pWnd->PreTranslateMessage(lpMsg))
					{
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
					}
					else
					{
						HWND hwnd = lpMsg->hwnd;
						if ((pView && pView->PreTranslateMessage(lpMsg))/*|| IsDialogMessage(lpMsg->hwnd, lpMsg)*/)
						{
							::DispatchMessage(lpMsg);
							TranslateMessage(lpMsg);
							lpMsg->hwnd = NULL;
							lpMsg->lParam = 0;
							lpMsg->wParam = 0;
							lpMsg->message = 0;
						}
						else
						{
							if (::GetModuleHandle(_T("chrome.dll")))
							{
								TRACE(_T("======== CUniverse=========:%x,MSG:%x\n"), lpMsg->hwnd, lpMsg->message);
								if (g_pCosmos->m_pActiveHtmlWnd)
								{
									HWND hWnd = ::GetParent(::GetParent(g_pCosmos->m_pActiveHtmlWnd->m_hWnd));
									if (hWnd && ::IsChild(hWnd, lpMsg->hwnd))
									{
										TranslateMessage(lpMsg);
										::DispatchMessage(lpMsg);
										lpMsg->hwnd = NULL;
										lpMsg->lParam = 0;
										lpMsg->wParam = 0;
										lpMsg->message = 0;
										return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
									}
									else
									{
										if (((__int64)g_pCosmos->m_pUniverseAppProxy) > 1)
										{
											if (g_pCosmos->m_pCosmosDelegate->OnUniversePreTranslateMessage(lpMsg))
											{
												return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
											}
										}
									}
								}
								else
								{
									if (((__int64)g_pCosmos->m_pUniverseAppProxy) > 1)
									{
										if (g_pCosmos->m_pCosmosDelegate->OnUniversePreTranslateMessage(lpMsg))
										{
											return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
										}
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

							//g_pCosmos->ProcessMsg(lpMsg);
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
					if (pView)
					{
						TranslateMessage(lpMsg);
						::DispatchMessage(lpMsg);
						lpMsg->hwnd = NULL;
						lpMsg->lParam = 0;
						lpMsg->wParam = 0;
						lpMsg->message = 0;
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
							else if (pView)
								hWnd = pView->m_hWnd;
							if ((g_pCosmos->m_pActiveXobj->m_nViewType == ActiveX || g_pCosmos->m_pActiveXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0))
							{
								if (pWnd)
									pWnd->PreTranslateMessage(lpMsg);
								lpMsg->hwnd = NULL;
								lpMsg->wParam = 0;
								break;
							}
							if (pView)
							{
								TranslateMessage(lpMsg);
								::DispatchMessage(lpMsg);
								lpMsg->hwnd = NULL;
								lpMsg->lParam = 0;
								lpMsg->wParam = 0;
								lpMsg->message = 0;
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
						else
						{
							//if (g_pCosmos->m_pCosmosDelegate && theApp.m_bHostCLR == false)
							//{
							//	TranslateMessage(lpMsg);
							//	::DispatchMessage(lpMsg);
							//	break;
							//}
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
				if (lpMsg->hwnd == topWindow || ::IsChild(topWindow, lpMsg->hwnd))
				{
					static CPoint PrePoint = CPoint(0, 0);
					if (MK_LBUTTON == lpMsg->wParam)
					{
						CPoint point = CPoint(GET_X_LPARAM(lpMsg->lParam), GET_Y_LPARAM(lpMsg->lParam));
						if (point != PrePoint)
						{
							CPoint ptTemp = point - PrePoint;
							CRect rcWindow;
							::GetWindowRect(topWindow, &rcWindow);
							rcWindow.OffsetRect(ptTemp.x, ptTemp.y);
							::MoveWindow(topWindow, rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), true);
						}
						PrePoint = point;
					}
					break;
				}
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
						HWND hWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(MainWnd, NULL);
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
				case 20210309:
				{
					HWND hClient = (HWND)lpMsg->wParam;
					g_pCosmos->m_bSZMode = false;
					for (auto& it : g_pCosmos->m_mapSizingBrowser)
					{
						if (::IsWindow(it.first))
						{
							SendMessage(it.first, WM_BROWSERLAYOUT, 0, 7);
						}
					}
					g_pCosmos->m_mapSizingBrowser.erase(g_pCosmos->m_mapSizingBrowser.begin(), g_pCosmos->m_mapSizingBrowser.end());
					g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(QueryType::RecalcLayout, hClient);
				}
				break;
				case 20210110:
				{
					HWND hClient = (HWND)lpMsg->wParam;
					HWND hWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(DocView, hClient);
					if (::IsWindow(hWnd))
					{
						if (g_pCosmos->m_mapDocTemplate.size() == 0)
						{
							g_pCosmos->m_hFirstView = hClient;
						}
						CMDTWnd* pFrameWnd = nullptr;
						CGalaxyCluster* pGalaxyCluster = nullptr;
						CosmosFrameWndInfo* pCosmosFrameWndInfo = (CosmosFrameWndInfo*)::GetProp(hWnd, _T("CosmosFrameWndInfo"));
						if (pCosmosFrameWndInfo)
						{
							switch (pCosmosFrameWndInfo->m_nFrameType)
							{
							case 1:
							{
								auto it = g_pCosmos->m_mapMDTWindow.find(hWnd);
								if (it == g_pCosmos->m_mapMDTWindow.end())
								{
									pFrameWnd = new CMDTWnd();
									pFrameWnd->SubclassWindow(hWnd);
									g_pCosmos->m_mapMDTWindow[hWnd] = pFrameWnd;
								}
								else
									pFrameWnd = it->second;
							}
							break;
							case 2:
							{
								if (g_pCosmos->m_pMDIMainWnd && hWnd == g_pCosmos->m_pMDIMainWnd->m_hWnd)
									g_pCosmos->m_pMDIMainWnd->m_pCosmosFrameWndInfo = pCosmosFrameWndInfo;
							}
							break;
							}

							if (pCosmosFrameWndInfo->bControlBarProessed)
								break;
						}
						else
						{
							int nType = (int)::RemoveProp(hWnd, _T("CosmosFrameWndType"));
							if (nType)
							{
								pCosmosFrameWndInfo = new CosmosFrameWndInfo();
								::SetProp(hWnd, _T("CosmosFrameWndInfo"), pCosmosFrameWndInfo);
								g_pCosmos->m_mapCosmosFrameWndInfo[hWnd] = pCosmosFrameWndInfo;
								pCosmosFrameWndInfo->m_hClient = hClient;
								pCosmosFrameWndInfo->m_nFrameType = nType;
							}
						}

						if (pCosmosFrameWndInfo && pCosmosFrameWndInfo->m_nFrameType != 2)
						{
							CString strKey = g_pCosmos->m_pUniverseAppProxy->m_strCreatingDOCID;
							g_pCosmos->m_pUniverseAppProxy->m_strCreatingDOCID = _T("");
							if (strKey == _T(""))
								strKey = _T("default");
							if (pFrameWnd)
							{
								if (pFrameWnd->m_strDocTemplateKey == _T(""))
									pFrameWnd->m_strDocTemplateKey = strKey;
								else
									strKey = pFrameWnd->m_strDocTemplateKey;
								CString strDefaultName = _T("");
								auto itName = g_pCosmos->m_mapDocDefaultName.find(strKey);
								if (itName != g_pCosmos->m_mapDocDefaultName.end())
									strDefaultName = itName->second;
								g_pCosmos->m_pUniverseAppProxy->SetFrameCaption(pFrameWnd->m_hWnd, strDefaultName);
							}
							auto it = g_pCosmos->m_mapDocTemplate.find(strKey);
							if (it != g_pCosmos->m_mapDocTemplate.end())
							{
								CTangramXmlParse m_Parse;
								if (m_Parse.LoadXml(it->second))
								{
									IGalaxy* pGalaxy = nullptr;
									IXobj* _pXobj = nullptr;
									CTangramXmlParse* pClient = m_Parse.GetChild(_T("client"));
									if (pClient)
									{
										pGalaxyCluster = new CComObject<CGalaxyCluster>();
										pGalaxyCluster->m_hWnd = hWnd;
										g_pCosmos->m_mapWindowPage[hWnd] = pGalaxyCluster;

										for (auto& it2 : g_pCosmos->m_mapCosmosAppProxy)
										{
											CGalaxyClusterProxy* pCosmosProxy = it2.second->OnGalaxyClusterCreated(pGalaxyCluster);
											if (pCosmosProxy)
												pGalaxyCluster->m_mapGalaxyClusterProxy[it2.second] = pCosmosProxy;
										}
										pGalaxyCluster->CreateGalaxy(CComVariant((__int64)hWnd), CComVariant((__int64)hClient), CComBSTR(""), &pGalaxy);
										if (pGalaxy)
										{
											//pCosmosFrameWndInfo->m_mapCtrlBarGalaxys[10000] = pGalaxy;
											pGalaxy->Observe(CComBSTR(strKey), CComBSTR(pClient->xml()), &_pXobj);
										}
										switch (pCosmosFrameWndInfo->m_nFrameType)
										{
										case 1:
										{
											CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
											_pGalaxy->m_strDocTemplateID = strKey;
										}
										break;
										case 3:
										{
											CMDIChild* pWnd = new CMDIChild();
											if (pWnd->m_strDocTemplateKey == _T(""))
												pWnd->m_strDocTemplateKey = strKey;

											pWnd->SubclassWindow(hWnd);
											pWnd->m_pParent = g_pCosmos->m_pMDIMainWnd;
											g_pCosmos->m_pMDIMainWnd->m_mapMDIChild[hWnd] = pWnd;
											g_pCosmos->m_bSZMode = true;

											g_pCosmos->m_pHostBrowser->OpenURL(CComBSTR(g_pCosmos->m_strStartupURL), BrowserWndOpenDisposition::SWITCH_TO_TAB, CComBSTR(""), CComBSTR(""));
											if (g_pCosmos->m_pMDIMainWnd->m_pGalaxy)
											{
												g_pCosmos->m_pMDIMainWnd->m_pGalaxy->m_pWebPageWnd = g_pCosmos->m_pHostHtmlWnd;
												::PostMessage(g_pCosmos->m_pMDIMainWnd->m_hWnd, WM_COSMOSMSG, (WPARAM)pWnd, 20210202);
												if (pWnd->m_pGalaxy == nullptr)
													pWnd->m_pGalaxy = (CGalaxy*)pGalaxy;
												pWnd->m_hClient = hClient;
												pWnd->m_pGalaxy->m_nGalaxyType = GalaxyType::MDIChildGalaxy;
												pWnd->m_strKey = strKey;
												pWnd->m_pGalaxy->m_strDocTemplateID = strKey;
												CString strDefaultName = _T("");
												auto itName = g_pCosmos->m_mapDocDefaultName.find(strKey);
												if (itName != g_pCosmos->m_mapDocDefaultName.end())
													strDefaultName = itName->second;
												g_pCosmos->m_pUniverseAppProxy->SetFrameCaption(pWnd->m_hWnd, strDefaultName);
												pClient = m_Parse.GetChild(_T("mdiclient"));
												if (pClient)
												{
													if (g_pCosmos->m_pMDIMainWnd->m_pGalaxy->m_strCurrentKey != strKey)
														g_pCosmos->m_pMDIMainWnd->m_pGalaxy->Observe(CComBSTR(strKey), CComBSTR(pClient->xml()), &_pXobj);
												}
											}
										}
										break;
										}
										pClient = m_Parse.GetChild(_T("hostpage"));
										bool bProcessWebPage = false;
										if (pClient == nullptr)
											bProcessWebPage = true;
										if (pClient && g_pCosmos->m_pMDIMainWnd && g_pCosmos->m_pMDIMainWnd->m_pGalaxy && g_pCosmos->m_pMDIMainWnd->m_pGalaxy->m_pWebPageWnd)
										{
											if (g_pCosmos->m_pHostBrowser->m_pVisibleWebWnd != g_pCosmos->m_pMDIMainWnd->m_pGalaxy->m_pWebPageWnd) {
												bProcessWebPage = false;
											}
											else if (g_pCosmos->m_pMDIMainWnd->m_pGalaxy->m_pWebPageWnd->m_pGalaxy->m_strCurrentKey != strKey)
											{
												g_pCosmos->m_pMDIMainWnd->m_pGalaxy->m_pWebPageWnd->LoadDocument2Viewport(strKey, pClient->xml());
												bProcessWebPage = true;
											}
										}
										if (bProcessWebPage || g_pCosmos->m_pMDIMainWnd == nullptr)
										{
											pClient = m_Parse.GetChild(_T("controlbars"));
											if (pClient)
											{
												CosmosFrameWndInfo* pCosmosFrameWndInfo = nullptr;
												HANDLE hHandle = NULL;
												if (g_pCosmos->m_pMDIMainWnd)
												{
													pCosmosFrameWndInfo = g_pCosmos->m_pMDIMainWnd->m_pCosmosFrameWndInfo;
												}
												else
												{
													hHandle = ::GetProp(hWnd, _T("CosmosFrameWndInfo"));
													pCosmosFrameWndInfo = (CosmosFrameWndInfo*)hHandle;
												}
												if (pCosmosFrameWndInfo)
												{
													pCosmosFrameWndInfo->m_pWebPage = g_pCosmos->m_pHostHtmlWnd;

													int nCount = pClient->GetCount();
													for (int i = 0; i < nCount; i++)
													{
														CTangramXmlParse* pParse2 = pClient->GetChild(i);
														int nBarID = pParse2->attrInt(_T("ctrlbarid"), 0);
														if (nBarID)
														{
															if (g_pCosmos->m_pMDIMainWnd)
															{
																auto itX = pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.find(nBarID);
																if (itX != pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.end())
																{
																	CGalaxy* _pGalaxy = (CGalaxy*)itX->second;
																	if (_pGalaxy->m_strCurrentKey != strKey)
																	{
																		IXobj* pXobj = nullptr;
																		_pGalaxy->Observe(CComBSTR(strKey), CComBSTR(pParse2->xml()), &pXobj);
																	}
																}
															}
															//else
															//{
															//	auto it = pCosmosFrameWndInfo->m_mapCtrlBarWnd.find(nBarID);
															//	if (it != pCosmosFrameWndInfo->m_mapCtrlBarWnd.end())
															//	{
															//		HWND hWnd = it->second;
															//		int nID = pParse2->attrInt(_T("clientid"), 0);
															//		HWND hClient = ::GetDlgItem(hWnd, nID);
															//		if (hClient)
															//		{
															//			pCosmosFrameWndInfo->bControlBarProessed = true;
															//			CString strXml = pParse2->xml();
															//			if (pGalaxyCluster)
															//			{
															//				IGalaxy* pGalaxy = nullptr;
															//				pGalaxyCluster->CreateGalaxy(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(nBarID), &pGalaxy);
															//				if (pGalaxy)
															//				{
															//					CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
															//					pCosmosFrameWndInfo->m_mapCtrlBarGalaxys[nBarID] = _pGalaxy;
															//					_pGalaxy->m_pWebPageWnd = g_pCosmos->m_pHostHtmlWnd;
															//					IXobj* pXobj = nullptr;
															//					_pGalaxy->Observe(CComBSTR(strKey), CComBSTR(strXml), &pXobj);
															//				}
															//			}
															//			CString strCaption = pParse2->attr(_T("caption"), _T(""));
															//			if (strCaption != _T(""))
															//				::SetWindowText(::GetParent(hClient), strCaption);
															//		}
															//	}
															//}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				break;
				case 20191114:
				{
					if (g_pCosmos->m_mapCreatingWorkBenchInfo.size())
					{
						g_pCosmos->InitEclipseApp();
					}
				}
				break;
				case 20201028:
				{
					if (g_pCosmos->m_hTempBrowserWnd)
					{
						g_pCosmos->m_hTempBrowserWnd = NULL;
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
					if (lpMsg->wParam && g_pCosmos->m_bEclipse == false)
						::DestroyWindow(g_pCosmos->m_hHostWnd);
				}
				break;
				case 20191117:
				{
					if (g_pCosmos->m_pUniverseAppProxy)
						g_pCosmos->m_pUniverseAppProxy->EclipseAppInit();
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
				if (g_pCosmos->m_bEclipse == false && g_pCosmos->m_bOfficeApp == false)
				{
					if (g_pCosmos->m_pMDIMainWnd == NULL)
					{
						if (g_pCosmos->m_mapBrowserWnd.size())
						{
							g_pCosmos->m_bChromeNeedClosed = true;
							auto it = g_pCosmos->m_mapBrowserWnd.begin();
							((CBrowser*)it->second)->SendMessageW(WM_CLOSE, 0, 0);
						}
					}
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

CString CUniverse::GetFileVer()
{
	DWORD dwHandle, InfoSize;
	CString strVersion;

	struct LANGANDCODEPAGE
	{
		WORD wLanguage;
		WORD wCodePage;
	}*lpTranslate;
	unsigned int cbTranslate = 0;

	TCHAR cPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, cPath, MAX_PATH);
	InfoSize = GetFileVersionInfoSize(cPath, &dwHandle);


	char* InfoBuf = new char[InfoSize];

	GetFileVersionInfo(cPath, 0, InfoSize, InfoBuf);
	VerQueryValue(InfoBuf, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);

	TCHAR SubBlock[300] = { 0 };

	wsprintf(SubBlock, TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"), lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);

	TCHAR* lpBuffer = NULL;
	unsigned int dwBytes = 0;
	VerQueryValue(InfoBuf, SubBlock, (void**)&lpBuffer, &dwBytes);
	if (lpBuffer != NULL)
	{
		strVersion.Format(_T("%s"), (TCHAR*)lpBuffer);
	}

	delete[] InfoBuf;
	return strVersion;
}

HRESULT CUniverse::UpdateRegistry(BOOL bRegister)
{
	return theApp.UpdateRegistryFromResource(IDR_TANGRAM, bRegister);
}

HRESULT CUniverse::CreateInstance(void* pv, REFIID riid, LPVOID* ppv)
{
	if (g_pCosmos)
	{
		DWORD dwID = ::GetCurrentThreadId();
		if (dwID != g_pCosmos->m_dwThreadID)
		{
			IStream* pStream = 0;
			HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(IID_IDispatch, (ICosmos*)g_pCosmos, &pStream);
			if (hr == S_OK)
			{
				IDispatch* pTarget = nullptr;
				hr = ::CoGetInterfaceAndReleaseStream(pStream, IID_IDispatch, (LPVOID*)&pTarget);
				if (hr == S_OK && pTarget)
					return pTarget->QueryInterface(riid, ppv);
			}
		}
		return g_pCosmos->QueryInterface(riid, ppv);
	}
	return S_FALSE;
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (g_pCosmos)
	{
		bool bCanUnLoad = false;
		if (g_pCosmos->m_bOfficeApp)
			bCanUnLoad = g_pCosmos->m_bOfficeAddinUnLoad;
		if (::GetModuleHandle(L"chrome_elf.dll"))
			bCanUnLoad = false;
		if (bCanUnLoad && g_pCosmos && g_pCosmos->m_nTangramObj == 0)
		{
			g_pCosmos->ExitInstance();
			delete g_pCosmos;
			g_pCosmos = nullptr;
			return S_OK;
		}
	}
	//return (theApp.DllCanUnloadNow() == S_OK && theApp.GetLockCount() == 0) ? S_OK : S_FALSE;
	return (AfxDllCanUnloadNow() == S_OK && theApp.GetLockCount() == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return theApp.DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{
	//theApp.m_bRegisterServer = true;
	return theApp.DllRegisterServer();
}

STDAPI DllUnregisterServer(void)
{
	return theApp.DllUnregisterServer();
}

JNIEXPORT void JNICALL Java_Tangram_Host_Tangram_InitTangram(JNIEnv* env, jobject obj, jobject jTangram, jobject jGalaxyCluster, jobject jGalaxy, jobject jWndXobj)
{
	jclass cls = env->GetObjectClass(obj);

	return;
}

