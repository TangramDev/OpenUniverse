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
* OUTLINED IN THE GPL LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
********************************************************************************/

// TangramApp.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "UniverseApp.h" 
#include <VersionHelpers.h> 
#include <shellscalingapi.h>

#include "fm20.h"
#include "CloudUtilities\DownLoad.h"
#include "EclipsePlus\EclipseAddin.h"

#include "OfficePlus\WordPlus\WordAddin.h"
#include "OfficePlus\ExcelPlus\ExcelAddin.h"
#include "OfficePlus\VisioPlus\VisioAddin.h"
#include "OfficePlus\AccessPlus\AccessAddin.h"
#include "OfficePlus\OutLookPlus\OutLookAddin.h"
#include "OfficePlus\ProjectPlus\ProjectAddin.h"
#include "OfficePlus\PowerPointPlus\PowerPointAddin.h"

#include "XobjHelper.h"
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

void CTBToolboxPaneWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CTBToolboxPaneWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	::GetModuleFileName(::GetModuleHandle(_T("tangramdesigner.dll")), g_pCosmos->m_szBuffer, MAX_PATH);
	CString strLib = CString(g_pCosmos->m_szBuffer);
	CString strObjName = _T("TangramDesigner.WebRuntimeHelper");
	CString strFunctionName = _T("CosmosInit");
	DWORD dwRetCode = 0;
	CString strData = _T("VSToolBoxSelected");
	g_pCosmos->m_pClrHost->ExecuteInDefaultAppDomain(
		strLib,
		strObjName,
		strFunctionName,
		strData,
		&dwRetCode);
	return lRes;
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

void CGenericPaneWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CGenericPaneWnd::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	switch (lParam)
	{
	case 20200516:
	{
		//if (g_pCosmos->m_pCosmosVS == nullptr && m_hChild == NULL)
		//{
		//	m_hChild = (HWND)wParam;
		//	::GetModuleFileName(::GetModuleHandle(_T("tangramdesigner.dll")), g_pCosmos->m_szBuffer, MAX_PATH);
		//	CString strLib = CString(g_pCosmos->m_szBuffer);
		//	CString strObjName = _T("TangramDesigner.WebRuntimeHelper");
		//	CString strFunctionName = _T("CosmosInit");
		//	DWORD dwRetCode = 0;
		//	g_pCosmos->m_pClrHost->ExecuteInDefaultAppDomain(
		//		strLib,
		//		strObjName,
		//		strFunctionName,
		//		L"visualstudioneedadmin",
		//		&dwRetCode);
		//	return lRes;
		//}
		if (g_pCosmos->m_pCosmosVS)
		{
			if (m_strToolType == _T("TBToolboxPane"))
			{
				HWND hWnd = (HWND)wParam;
				if (m_hChild == NULL && ::IsWindow(hWnd))
					m_hChild = hWnd;
				CComVariant var;
				g_pCosmos->m_pCosmosVS->get_AppKeyValue(CComBSTR("appdata"), &var);
				CString strXml = OLE2T(var.bstrVal);
				if (strXml != _T(""))
				{
					CTangramXmlParse m_Parse;
					if (m_Parse.LoadXml(strXml))
					{
						CTangramXmlParse* pVSParse = m_Parse.GetChild(_T("editor"));
						if (pVSParse)
						{
							::GetModuleFileName(::GetModuleHandle(_T("tangramdesigner.dll")), g_pCosmos->m_szBuffer, MAX_PATH);
							CString strLib = CString(g_pCosmos->m_szBuffer);
							CString strObjName = _T("TangramDesigner.WebRuntimeHelper");
							CString strFunctionName = _T("CosmosInit");
							DWORD dwRetCode = 0;
							CString strData = _T("initeditordata:") + pVSParse->xml();
							g_pCosmos->m_pClrHost->ExecuteInDefaultAppDomain(
								strLib,
								strObjName,
								strFunctionName,
								strData,
								&dwRetCode);
						}
						pVSParse = m_Parse.GetChild(_T("vstoolbox"));
						if (pVSParse)
						{
							IGalaxyCluster* pManager = nullptr;
							g_pCosmos->CreateGalaxyCluster((LONGLONG)m_hWnd, &pManager);
							if (pManager)
							{
								IGalaxy* pIGalaxy = nullptr;
								pManager->CreateGalaxy(CComVariant((LONGLONG)m_hWnd), CComVariant((__int64)m_hChild), CComBSTR("default"), &pIGalaxy);
								if (pIGalaxy)
								{
									IXobj* pXobj = nullptr;
									pIGalaxy->Observe(CComBSTR("default"), CComBSTR(pVSParse->xml()), &pXobj);
									CTangramXmlParse* pVSData = m_Parse.GetChild(_T("vstoolwindow"));
									if (pVSData)
									{
										::GetModuleFileName(::GetModuleHandle(_T("tangramdesigner.dll")), g_pCosmos->m_szBuffer, MAX_PATH);
										CString strLib = CString(g_pCosmos->m_szBuffer);
										CString strObjName = _T("TangramDesigner.WebRuntimeHelper");
										CString strFunctionName = _T("CosmosInit");
										DWORD dwRetCode = 0;
										g_pCosmos->m_pClrHost->ExecuteInDefaultAppDomain(
											strLib,
											strObjName,
											strFunctionName,
											pVSData->xml(),
											&dwRetCode);
									}
								}
							}
						}
					}
				}
				else
				{
					auto t = create_task([this]()
						{
							SleepEx(1000, true);
							::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20200516);
							return 1;
						});
				}
			}
		}
	}
	break;
	case 20200531:
		m_hChild = (HWND)wParam;
		break;
	}
	return lRes;
}

LRESULT CGenericPaneWnd::OnCosmosData(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	if (wParam == 0 && lParam == 0)
		return (LRESULT)this;
	return lRes;
}

LRESULT CGenericPaneWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	if (wParam) {
		if (::IsWindow(m_hChild))
		{
			if (m_pGalaxy == nullptr)
			{
				IGalaxy* pGalaxy = nullptr;
				g_pCosmos->GetGalaxy((__int64)m_hChild, &pGalaxy);
				if (pGalaxy)
				{
					m_pGalaxy = (CGalaxy*)pGalaxy;
				}
			}
			if (m_pGalaxy)
			{
				RECT rc;
				::GetClientRect(m_hChild, &rc);
				if (rc.right * rc.bottom == 0)
					m_pGalaxy->HostPosChanged();
			}
			::PostMessage(m_hChild, WM_COSMOSMSG, 0, 20180115);
		}
	}
	else
	{
	}
	return lRes;
}

LRESULT CGenericPaneWnd::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	if (m_hChild == NULL)
	{
		m_hChild = ::GetWindow(m_hWnd, GW_CHILD);
	}
	if (m_pGalaxy == nullptr)
	{
		IGalaxy* pGalaxy = nullptr;
		g_pCosmos->GetGalaxy((__int64)m_hChild, &pGalaxy);
		if (pGalaxy)
		{
			m_pGalaxy = (CGalaxy*)pGalaxy;
		}
	}
	if (m_pGalaxy)
	{
		RECT rc;
		::GetClientRect(m_hChild, &rc);
		if (rc.right * rc.bottom == 0)
			m_pGalaxy->HostPosChanged();
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
				new CComObject < AccessPlus::CAccessAddin >;
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
				new CComObject < VisioPlus::CVisioAddin >;
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
			if (g_pCosmos->m_mapMDTFrame.size())
			{
				auto it = g_pCosmos->m_mapMDTFrame.begin();
				while (it != g_pCosmos->m_mapMDTFrame.end())
				{
					if (it->second)
					{
						it->second->DestroyWindow();
					}
					it = g_pCosmos->m_mapMDTFrame.begin();
				}
			}
			g_pCosmos->m_mapMDTFrame.clear();

			if (g_pCosmos->m_mapCosmosMDIChildWnd.size())
			{
				auto it = g_pCosmos->m_mapCosmosMDIChildWnd.begin();
				while (it != g_pCosmos->m_mapCosmosMDIChildWnd.end())
				{
					if (it->second)
					{
						it->second->DestroyWindow();
					}
					it = g_pCosmos->m_mapCosmosMDIChildWnd.begin();
				}
			}
			g_pCosmos->m_mapCosmosMDIChildWnd.clear();

			if (::IsWindow(g_pCosmos->m_hHostBrowserWnd))
			{
				::SendMessage(g_pCosmos->m_hHostBrowserWnd, WM_CLOSE, 0, 0);
			}
			if (g_pCosmos->m_hForegroundIdleHook)
				UnhookWindowsHookEx(g_pCosmos->m_hForegroundIdleHook);

			g_pCosmos->m_pDesignerFrame = nullptr;
			g_pCosmos->m_pDesignerGalaxyCluster = nullptr;
		}
		break;
	}
	break;
	case WM_POWERBROADCAST:
	{
		switch (wParam)
		{
		case PBT_APMRESUMEAUTOMATIC:
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
		}
		break;
		case PBT_APMPOWERSTATUSCHANGE:
		{
			for (auto it : g_pCosmos->m_mapBrowserWnd)
			{
				if (::IsWindowVisible(it.first))
				{
					CBrowser* pWnd = (CBrowser*)it.second;
					if (pWnd && pWnd->m_pVisibleWebWnd)
					{
						HWND hWnd = pWnd->m_pBrowser->GetActiveWebContentWnd();
						if (pWnd->m_pVisibleWebWnd->m_hWnd != hWnd)
						{
							auto it = g_pCosmos->m_mapHtmlWnd.find(hWnd);
							if (it != g_pCosmos->m_mapHtmlWnd.end())
							{
								pWnd->m_pVisibleWebWnd = (CWebPage*)it->second;
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
		case 20200611:
		{
			if (wParam)
			{
				HWND _hWnd = (HWND)wParam;
				if (::IsWindow(hWnd))
				{
					HWND hWndChild = ::GetWindow(_hWnd, GW_CHILD);
					TCHAR	szBuffer[MAX_PATH];
					::GetClassName(_hWnd, szBuffer, MAX_PATH);
					CString strClassName = CString(szBuffer);
					if (strClassName == _T("GenericPane"))
					{
						HWND hDlg = ::GetDlgItem(_hWnd, 0);
						HWND m_hClassWnd = ::GetDlgItem(hDlg, 0x00100);
						HWND m_hClassDetailWnd = ::GetDlgItem(hDlg, 0x00200);
						::GetClassName(m_hClassWnd, szBuffer, MAX_PATH);
						strClassName = CString(szBuffer);
						if (strClassName == _T("LiteTreeView32"))
						{
							CGenericPaneWnd* pWnd = new CGenericPaneWnd();
							pWnd->m_strToolType = strClassName;
							pWnd->SubclassWindow(_hWnd);
							::PostMessage(_hWnd, WM_COSMOSMSG, 0, 20200516);
						}
					}
				}
			}
		}
		break;
		case 20200519:
		{
			if (wParam)
			{
				HWND _hWnd = (HWND)wParam;
				if (::IsWindow(hWnd))
				{
					HWND hWndChild = ::GetWindow(_hWnd, GW_CHILD);
					TCHAR	szBuffer[MAX_PATH];
					::GetClassName(_hWnd, szBuffer, MAX_PATH);
					CString strClassName = CString(szBuffer);
					if (strClassName == _T("GenericPane"))
					{
						::GetClassName(hWndChild, szBuffer, MAX_PATH);
						strClassName = CString(szBuffer);
						if (strClassName == _T("TBToolboxPane"))
						{
							CTBToolboxPaneWnd* pPaneWnd = new CTBToolboxPaneWnd();
							pPaneWnd->SubclassWindow(hWndChild);
							CGenericPaneWnd* pWnd = new CGenericPaneWnd();
							pWnd->m_strToolType = strClassName;
							pWnd->SubclassWindow(_hWnd);
							::PostMessage(_hWnd, WM_COSMOSMSG, 0, 20200516);
						}
					}
				}
			}
		}
		break;
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
			if (g_pCosmos->m_nAppType == APP_BROWSER_ECLIPSE||g_pCosmos->m_bEclipse)
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
	case WM_MDICLIENTCREATED:
	{
		if (g_pCosmos->m_pMDIMainWnd)
		{
			IGalaxyCluster* pGalaxyCluster = nullptr;
			g_pCosmos->CreateGalaxyCluster((__int64)g_pCosmos->m_pMDIMainWnd->m_hWnd, &pGalaxyCluster);
			g_pCosmos->m_pMDIMainWnd->m_pGalaxyCluster = (CGalaxyCluster*)pGalaxyCluster;
			IXobj* pXobj = nullptr;
			g_pCosmos->m_pMDIMainWnd->m_pDocTemplate->m_strKey = _T("default");
			g_pCosmos->m_pMDIMainWnd->m_pDocTemplate->m_strClientKey = _T("default");
			g_pCosmos->m_pMDIMainWnd->m_pDocTemplate->InitXmlData();
			IGalaxy* pGalaxy = nullptr;
			g_pCosmos->m_pMDIMainWnd->m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)g_pCosmos->m_pMDIMainWnd->m_hMDIClient), CComBSTR("mdiclient"), &pGalaxy);
			if (pGalaxy)
			{
				CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
				_pGalaxy->m_pCosmosDocTemplate = g_pCosmos->m_pMDIMainWnd->m_pDocTemplate;
				pGalaxy->Observe(CComBSTR("mdiclient"), CComBSTR(""), &pXobj);
				g_pCosmos->m_pMDIMainWnd->m_pDocTemplate->m_mapMainPageNode[g_pCosmos->m_pMDIMainWnd->m_hMDIClient] = (CXobj*)pXobj;
			}
			for (auto it : g_pCosmos->m_pMDIMainWnd->m_mapDesignableWnd)
			{
				HWND h = ::GetParent(it.first);
				::GetWindowText(h, g_pCosmos->m_szBuffer, MAX_PATH);
				CString str = CString(g_pCosmos->m_szBuffer);
				str.Replace(_T(" "), _T("_"));
				g_pCosmos->m_pMDIMainWnd->m_mapDesignableWnd[it.first] = str;
			}
		}
		return 0;
	}
	break;
	case WM_CONTROLBARCREATED:
	{
		HWND hwnd = (HWND)wParam;
		HWND hPWnd = (HWND)lParam;
		HWND hTop = ::GetAncestor(hPWnd, GA_ROOT);
		auto it = g_pCosmos->m_mapCosmosMDIChildWnd.find(hTop);
		if (it != g_pCosmos->m_mapCosmosMDIChildWnd.end() || (g_pCosmos->m_nAppType == 1965 && g_pCosmos->m_pActiveMDIChildWnd))
		{
			return 1;
		}

		if (lParam && ::IsWindow(hwnd))
		{
			HWND hClient = nullptr;
			auto it = g_pCosmos->m_mapWorkBenchWnd.find(hPWnd);
			if (it != g_pCosmos->m_mapWorkBenchWnd.end())
			{
				hClient = ((CEclipseWnd*)it->second)->m_hClient;
				hPWnd = ::GetParent(hClient);
			}
			else
				hClient = ::GetDlgItem(hPWnd, AFX_IDW_PANE_FIRST);
			BOOL bMDIClientGalaxy = FALSE;
			if (::GetWindowLong(hClient, GWL_STYLE) & MDIS_ALLCHILDSTYLES)
			{
				bMDIClientGalaxy = TRUE;
			}
			CAFXHelperWnd* pCtrlBar = new CAFXHelperWnd();
			pCtrlBar->m_hParent = hPWnd;
			pCtrlBar->SubclassWindow(hwnd);
			g_pCosmos->m_mapCosmosAFXHelperWnd[hwnd] = pCtrlBar;
			HWND hGalaxy = ::GetDlgItem(hwnd, AFX_IDW_PANE_FIRST);
			if (::IsWindow(hGalaxy))
			{
				pCtrlBar->m_hFrame = hGalaxy;
				if (::IsWindow(hPWnd))
				{
					CGalaxyCluster* pGalaxyCluster = nullptr;
					auto it = g_pCosmos->m_mapWindowPage.find(hPWnd);
					if (it == g_pCosmos->m_mapWindowPage.end())
					{
						HWND hPWnd = (HWND)lParam;
						IXobj* pXobj = nullptr;
						pGalaxyCluster = new CComObject<CGalaxyCluster>;
						pGalaxyCluster->m_hWnd = hPWnd;
						g_pCosmos->m_mapWindowPage[hPWnd] = pGalaxyCluster;
						pGalaxyCluster->put_ConfigName(CComBSTR(L""));
						for (auto it : g_pCosmos->m_mapCosmosAppProxy)
						{
							CGalaxyClusterProxy* pCosmosProxy = it.second->OnGalaxyClusterCreated(pGalaxyCluster);
							if (pCosmosProxy)
								pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pCosmosProxy;
						}
					}
					else
					{
						pGalaxyCluster = (CGalaxyCluster*)it->second;
					}
					if (g_pCosmos->m_pMDIMainWnd && g_pCosmos->m_pMDIMainWnd->m_pDocTemplate)
						pGalaxyCluster->m_pCosmosDocTemplate = g_pCosmos->m_pMDIMainWnd->m_pDocTemplate;
					auto it2 = pGalaxyCluster->m_mapCtrlBarGalaxy.find(hGalaxy);
					if (it2 == pGalaxyCluster->m_mapCtrlBarGalaxy.end())
					{
						CString strCaption = _T("");
						::GetWindowText(hGalaxy, g_pCosmos->m_szBuffer, MAX_PATH);
						strCaption = CString(g_pCosmos->m_szBuffer);
						strCaption.Trim();
						if (strCaption == _T(""))
						{
							::GetWindowText(hwnd, g_pCosmos->m_szBuffer, MAX_PATH);
							strCaption = CString(g_pCosmos->m_szBuffer);
							strCaption.Trim();
						}
						strCaption.Replace(_T(" "), _T("_"));
						IXobj* pXobj = nullptr;
						bool bSavetocfg = (pGalaxyCluster->m_pCosmosDocTemplate == nullptr);
						pGalaxyCluster->CreateGalaxyWithDefaultNode((LONGLONG)hGalaxy, CComBSTR(strCaption), CComBSTR(L""), CComBSTR(L""), bSavetocfg, &pXobj);
						if (pXobj)
						{
							if (pGalaxyCluster->m_pCosmosDocTemplate)
							{
								pGalaxyCluster->m_pCosmosDocTemplate->m_mapMainPageNode[hGalaxy] = (CXobj*)pXobj;
							}
							IGalaxy* pGalaxy = nullptr;
							pXobj->get_Galaxy(&pGalaxy);
							((CGalaxy*)pGalaxy)->m_nGalaxyType = CtrlBarGalaxy;
							pGalaxyCluster->m_mapCtrlBarGalaxy[hGalaxy] = (CGalaxy*)pGalaxy;
						}
					}
				}
			}
		}
		return 1;
	}
	break;
	case WM_COSMOSMSG:
	{
		if (wParam)
		{
			switch (lParam)
			{
			case 20200718:
			{
				RemoteDebugInfo* pRemoteDebugInfo = (RemoteDebugInfo*)wParam;
				int nHandle = pRemoteDebugInfo->nHandle;
				auto it = g_pCosmos->m_mapRemoteTangramApp.find(nHandle);
				if (it != g_pCosmos->m_mapRemoteTangramApp.end())
				{
					it->second->SelectVSObj(CComBSTR(pRemoteDebugInfo->m_strKey), nullptr, nHandle);
				}
				delete pRemoteDebugInfo;
			}
			break;
			case 20200625:
			{
				RemoteDebugInfo* pRemoteDebugInfo = (RemoteDebugInfo*)wParam;
				if (g_pCosmos->m_pCLRProxy)
				{
					g_pCosmos->m_pCLRProxy->CosmosAction(CComBSTR(pRemoteDebugInfo->m_strKey), nullptr);
					delete pRemoteDebugInfo;
				}
			}
			break;
			case 19631963:
			{
				DocTemplateInfo* pDocTemplateInfo = (DocTemplateInfo*)wParam;
				CString strPath = g_pCosmos->m_strAppCommonDocPath + _T("Tangramdoctemplate.xml");
				if (::PathFileExists(strPath))
				{
					CTangramXmlParse m_Parse;
					m_Parse.LoadFile(strPath);
					CString _strKey = pDocTemplateInfo->strProxyID + pDocTemplateInfo->strExt;
					if (m_Parse.GetChild(_strKey) == nullptr)
					{
						CTangramXmlParse* pParse = m_Parse.AddNode(_strKey);
						CString strVal = _T("");
						strVal += pDocTemplateInfo->strFileTypeID;
						strVal += _T("|");
						strVal += pDocTemplateInfo->strfilterName;
						strVal += _T("|");
						strVal += _T("*") + pDocTemplateInfo->strExt;
						strVal += _T("|");
						strVal += pDocTemplateInfo->strProxyName;
						strVal += _T("|");
						if (pDocTemplateInfo->nResID != -1)
						{
							CString strID = _T("");
							strID.Format(_T("%d"), pDocTemplateInfo->nResID);
							pParse->put_attr(_T("ResID"), strID);
						}
						pParse->put_text(strVal);
						m_Parse.SaveFile(strPath);
						if (pDocTemplateInfo->bDll)
						{
							TCHAR	m_szBuffer[MAX_PATH];
							HRESULT hr = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, m_szBuffer);
							CString strPath = CString(m_szBuffer);
							strPath += _T("\\TangramCommonDocTemplate\\TangramReg.xml");
							CTangramXmlParse m_Parse2;
							if (::PathFileExists(strPath) == FALSE)
							{
								m_Parse2.LoadXml(_T("<CosmosDocReg />"));
								m_Parse2.SaveFile(strPath);
							}
							else
							{
								m_Parse2.LoadFile(strPath);
							}
							if (m_Parse2.GetChild(pDocTemplateInfo->strProxyID) == false)
							{
								m_Parse2.AddNode(pDocTemplateInfo->strProxyID);
								m_Parse2.SaveFile(strPath);
							}
						}
					}
				}
				//delete pDocTemplateInfo;
			}
			break;
			default:
			{
				if (g_pCosmos->m_mapCosmosDocTemplateInfo.size() == 0)
					g_pCosmos->InitCosmosDocManager();

				CString strFile = _T("");
				CFileDialog openFileDlg(true, _T(""), _T(""), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, g_pCosmos->m_strDocFilters, NULL);
				openFileDlg.m_ofn.nMaxCustFilter++;
				if (openFileDlg.DoModal() == IDOK)
				{
					CString filePath = openFileDlg.GetPathName();
					ICosmosDoc* pDoc = nullptr;
					g_pCosmos->OpenCosmosDocFile(filePath.AllocSysString(), &pDoc);
					if (pDoc != nullptr)
					{
						CCosmosDoc* _pDoc = (CCosmosDoc*)pDoc;
						return (LRESULT)_pDoc;
					}
				}
			}
			break;
			}
			return 0;
		}
	}
	switch (lParam)
	{
	case 10001000:
	{
		if (g_pCosmos->m_nAppID != 9 && g_pCosmos->m_bEclipse == false && g_pCosmos->m_bOfficeApp == false)
		{
			int nCount = g_pCosmos->m_mapMDTFrame.size();
			if (nCount == 0)
			{
				::PostMessage(g_pCosmos->m_hCosmosWnd, WM_HUBBLE_APPQUIT, 0, 0);
			}
		}
	}
	break;
	default:
		break;
	}
	return 1;
	break;
	case WM_HUBBLE_INIT:
		if (lParam == 20002000)
		{
			g_pCosmos->CosmosInit();
		}
		break;
	case WM_HUBBLE_APPQUIT:
	{
		if (g_pCosmos->m_bEclipse == false && g_pCosmos->m_bOfficeApp == false)
		{
			if (g_pCosmos->m_pMDIMainWnd == NULL && g_pCosmos->m_mapMDTFrame.size() == 0)
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
	case WM_WINDOWPOSCHANGED:
	{
		HWND m_hChildWnd = (HWND)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (::IsWindow(m_hChildWnd)&&::IsWindowVisible(hWnd)) {
			RECT rc;
			::GetClientRect(m_hChildWnd, &rc);
			WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
			if (rc.right != lpwndpos->cx || rc.bottom != lpwndpos->cy)
				::SetWindowPos(m_hChildWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_FRAMECHANGED | SWP_NOREDRAW | SWP_NOACTIVATE);
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
		if (::GetWindowLongPtr(hPWnd, GWL_EXSTYLE) & WS_EX_MDICHILD && ::SendMessage(hPWnd, WM_COSMOSMSG, 0, 19631222) == 0)
		{
			if (g_pCosmos->m_nAppType == 0 || g_pCosmos->m_nAppType == 1992)
			{
				LRESULT lRes = ::SendMessage(hPWnd, WM_COSMOSMSG, (WPARAM)pCreateWnd->lpcs->lpCreateParams, TANGRAM_CONST_PANE_FIRST);
				if (g_pCosmos->m_nAppType == 0)
					g_pCosmos->m_nAppType = lRes;
				if (lRes == 1992 || g_pCosmos->m_nAppType == 1992)
				{
					CUniverseMDIChild* pMDIChildWnd = new CUniverseMDIChild();
					pMDIChildWnd->m_hChild = hWnd;
					pMDIChildWnd->SubclassWindow(hPWnd);
					g_pCosmos->m_pActiveMDIChildWnd = pMDIChildWnd;
					pMDIChildWnd->m_bNoDocView = (pCreateWnd->lpcs->lpCreateParams == 0);
					::PostMessage(hPWnd, WM_COSMOSMSG, 0, 19922017);
				}
			}
		}

		::GetClassName(hWnd, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strClassName = CString(g_pCosmos->m_szBuffer);
		::GetClassName(hPWnd, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strPClassName = CString(g_pCosmos->m_szBuffer);

		if (dwID == AFX_IDW_PANE_FIRST)
		{
			int lRes = (int)::SendMessage(hPWnd, WM_COSMOSMSG, (WPARAM)pCreateWnd->lpcs->lpCreateParams, TANGRAM_CONST_PANE_FIRST);

			switch (lRes)
			{
			case APP_SDI://for SDI Child
			case APP_MDT://for MDT Child
			{
				if (g_pCosmos->m_pCosmosDelegate)
					g_pCosmos->m_pCosmosDelegate->HookAppDocTemplateInfo();
			}
			case APP_MDI://for MDI Child
			{
				CUniverseMDIChild* pMDIChildWnd = new CUniverseMDIChild();
				pMDIChildWnd->m_hChild = hWnd;
				pMDIChildWnd->SubclassWindow(hPWnd);
				g_pCosmos->m_pActiveMDIChildWnd = pMDIChildWnd;
				pMDIChildWnd->m_bNoDocView = (pCreateWnd->lpcs->lpCreateParams == 0);
				if (g_pCosmos->m_nAppType == 1963)
					g_pCosmos->m_mapCosmosMDIChildWnd[hPWnd] = pMDIChildWnd;
				::PostMessage(hPWnd, WM_COSMOSMSG, 0, 19922017);
			}
			break;
			default:
			{
				if (g_pCosmos->m_pMDIMainWnd)
				{
					CString strClsName = CString(pCreateWnd->lpcs->lpszClass);
					if (strClsName.CompareNoCase(_T("Cosmos Xobj Class")) && strClsName.CompareNoCase(_T("Universe GridWindow Class")))
						::SendMessage(g_pCosmos->m_pMDIMainWnd->m_hWnd, WM_CONTROLBARCREATED, (WPARAM)hPWnd, (LPARAM)hWnd);
				}
				else
				{
					if (g_pCosmos->m_pActiveMDIChildWnd && ::IsWindow(g_pCosmos->m_pActiveMDIChildWnd->m_hWnd))
					{
						CString strClsName = CString(pCreateWnd->lpcs->lpszClass);
						if (strClsName.CompareNoCase(_T("Cosmos Xobj Class")) && strClsName.CompareNoCase(_T("Universe GridWindow Class")))
							::PostMessage(g_pCosmos->m_pActiveMDIChildWnd->m_hWnd, WM_CONTROLBARCREATED, (WPARAM)hPWnd, (LPARAM)hWnd);
					}
				}
			}
			break;
			}
		}

		if (strClassName == _T("MDIClient"))
		{
			if (::IsWindow(g_pCosmos->m_hHostWnd) == false)
			{
				auto it = g_pCosmos->m_mapValInfo.find(_T("designertoolcaption"));
				if (it != g_pCosmos->m_mapValInfo.end())
					g_pCosmos->m_strDesignerToolBarCaption = OLE2T(it->second.bstrVal);
				g_pCosmos->m_hHostWnd = ::CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW, _T("Cosmos Xobj Class"), g_pCosmos->m_strDesignerToolBarCaption, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 400, 400, NULL, 0, theApp.m_hInstance, NULL);
				g_pCosmos->m_hChildHostWnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, g_pCosmos->m_hHostWnd, 0, theApp.m_hInstance, NULL);
			}
			if (::SendMessage(hPWnd, WM_QUERYAPPPROXY, (WPARAM)pCreateWnd->lpcs->lpCreateParams, TANGRAM_CONST_PANE_FIRST) == 1992)
			{
				if (g_pCosmos->m_pMDIMainWnd == nullptr)
				{
					g_pCosmos->m_pMDIMainWnd = new CUniverseMDIMain();
					g_pCosmos->m_pMDIMainWnd->m_hMDIClient = hWnd;
					g_pCosmos->m_pMDIMainWnd->SubclassWindow(hPWnd);
					if (g_pCosmos->m_pCosmosDelegate)
						g_pCosmos->m_pCosmosDelegate->HookAppDocTemplateInfo();
				}
			}
			::PostMessage(g_pCosmos->m_hCosmosWnd, WM_MDICLIENTCREATED, (WPARAM)hWnd, (LPARAM)hPWnd);
		}
		else if (strClassName.Find(_T("Afx:ControlBar:")) == 0)
		{
			HWND h = ::GetAncestor(hPWnd, GA_ROOT);
			if (::IsWindow(h))
			{
				::PostMessage(g_pCosmos->m_hCosmosWnd, WM_CONTROLBARCREATED, (WPARAM)hWnd, (LPARAM)h);
			}
		}
		else if (strClassName.Find(_T("Afx:MiniFrame:")) == 0)
		{
			::PostMessage(hWnd, WM_QUERYAPPPROXY, (WPARAM)hWnd, (LPARAM)19650601);
		}
		else if (strClassName.Find(_T("Afx:RibbonBar:")) == 0)
		{
			CAFXHelperWnd* pRibbonBar = new CAFXHelperWnd();
			pRibbonBar->SubclassWindow(hWnd);
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
		}
		else if (strClassName.Find(_T("SysTreeView32")) == 0 || strClassName.Find(_T("SysTabControl32")) == 0 || strClassName.Find(_T("SysListView32")) == 0)
		{
			g_pCosmos->m_mapCosmosCommonCtrl[hWnd] = nullptr;
			::PostMessage(hWnd, WM_COSMOSMSG, 0, 19820911);
		}
		else if (strClassName.Find(_T("Afx:")) == 0 && (pCreateWnd->lpcs->style & WS_POPUP))
		{
			auto it = g_pCosmos->m_mapMDTFrame.find(hPWnd);
			if (it != g_pCosmos->m_mapMDTFrame.end())
			{
				ATLTRACE(_T("%x\n"), hWnd);
				::SendMessage(hPWnd, WM_CONTROLBARCREATED, (WPARAM)hWnd, 0);
			}
		}

		if (strPClassName == _T("GenericPane"))
		{
			HWND hWnd = (HWND)wParam;
			if (::IsWindow(hWnd))
			{
				if (::SendMessageW(hPWnd, WM_HUBBLE_DATA, 0, 0) == 0)
				{
					CGenericPaneWnd* pWnd = new CGenericPaneWnd();
					pWnd->SubclassWindow(hPWnd);
					pWnd->m_hChild = hWnd;
					::GetWindowText(hWnd, g_pCosmos->m_szBuffer, MAX_PATH);
					pWnd->m_strToolType = CString(g_pCosmos->m_szBuffer);
					if (pWnd->m_strToolType == _T("WebRuntimeToolBox"))
					{
						g_pCosmos->m_mapValInfo[_T("WebRuntimeToolBox")] = CComVariant((__int64)hWnd);
					}
				}
			}

			break;
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

		auto it2 = g_pCosmos->m_mapCosmosAFXHelperWnd.find(hWnd);
		if (it2 != g_pCosmos->m_mapCosmosAFXHelperWnd.end())
		{
			g_pCosmos->m_mapCosmosAFXHelperWnd.erase(it2);
		}
		auto it3 = g_pCosmos->m_mapCosmosCommonCtrl.find(hWnd);
		if (it3 != g_pCosmos->m_mapCosmosCommonCtrl.end())
			g_pCosmos->m_mapCosmosCommonCtrl.erase(it3);
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
			auto it = g_pCosmos->m_mapMDTFrame.find(hWnd);
			if (it != g_pCosmos->m_mapMDTFrame.end())
			{
				g_pCosmos->m_pActiveAppProxy->OnActiveMainFrame(hWnd);
			}
			else
			{
				auto it = g_pCosmos->m_mapWorkBenchWnd.find(hWnd);
				if (it != g_pCosmos->m_mapWorkBenchWnd.end())
				{
					CEclipseWnd* pWnd = (CEclipseWnd*)it->second;
					g_pCosmos->m_pActiveAppProxy->OnActiveMainFrame(::GetParent(pWnd->m_hClient));
				}
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
				CXobjHelper* pWnd = nullptr;
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
				//if (g_pCosmos->m_bEclipse)
			{
				//if (MK_LBUTTON == lpMsg->wParam)
				//{
				//	CWnd* pWnd = nullptr;
				//	if (g_pCosmos->m_pActiveXobj)
				//		pWnd = (CXobjHelper*)g_pCosmos->m_pActiveXobj->m_pHostWnd;
				//	if (g_pCosmos->m_pGalaxy && g_pCosmos->m_pActiveXobj && pWnd && pWnd->PreTranslateMessage(lpMsg))
				//	{
				//		if (g_pCosmos->m_pCLRProxy->IsWinForm(pWnd->m_hWnd))
				//		{
				//			TranslateMessage(lpMsg);
				//			::DispatchMessage(lpMsg);
				//		}
				//		return CallNextHookEx(pThreadInfo->m_hGetMessageHook, nCode, wParam, lParam);
				//	}
				//}
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
			case WM_DOWNLOAD_MSG:
			{
				Utilities::CDownLoadObj* pObj = (Utilities::CDownLoadObj*)lpMsg->wParam;
				if (pObj)
					delete pObj;
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
				case 20200705:
				{
					//CString strID = _T("caswebagent.server.1");
					//HRESULT hr = g_pCosmos->StartApplication(CComBSTR(strID), CComBSTR(""));
					//if (hr == S_OK)
					//{
					//	auto it = g_pCosmos->m_mapRemoteCosmos.find(strID);
					//	if (it != g_pCosmos->m_mapRemoteCosmos.end())
					//	{
					//		g_pCosmos->m_pCosmosVS = it->second;
					//	}
					//	__int64 nHandle = (__int64)g_pCosmos->m_pCosmosVS;
					//	g_pCosmos->m_mapValInfo[_T("vstangramhandle")] = CComVariant((__int64)nHandle);
					//}
				}
				break;
				case 20200628:
				{
					::GetModuleFileName(::GetModuleHandle(_T("tangramdesigner.dll")), g_pCosmos->m_szBuffer, MAX_PATH);
					CString strLib = CString(g_pCosmos->m_szBuffer);
					CString strObjName = _T("TangramDesigner.WebRuntimeHelper");
					CString strFunctionName = _T("CosmosInit");
					DWORD dwRetCode = 0;
					g_pCosmos->m_pClrHost->ExecuteInDefaultAppDomain(
						strLib,
						strObjName,
						strFunctionName,
						_T("internetconnected"),
						&dwRetCode);
				}
				break;
				case 20200603:
				{
					auto it = g_pCosmos->m_mapValInfo.find(_T("appdata"));
					if (it != g_pCosmos->m_mapValInfo.end())
					{
						for (auto it2 : g_pCosmos->m_mapRemoteTangramApp)
						{
							it2.second->put_AppKeyValue(CComBSTR("appdata"), it->second);
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
					if (g_pCosmos->m_pCosmosDelegate)
						g_pCosmos->m_pCosmosDelegate->EclipseAppInit();
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
					//CString strHelper = g_pCosmos->m_strAppPath + _T("tangramhelper.xml");
					//if (::PathFileExists(strHelper))
					//{
					//	CTangramXmlParse m_Parse;
					//	if (m_Parse.LoadFile(strHelper))
					//	{
					//		switch (g_pCosmos->m_nAppType)
					//		{
					//		case APP_BROWSER:
					//		case APP_BROWSERAPP:
					//		case APP_BROWSER_ECLIPSE:
					//		{
					//			CTangramXmlParse* pChild = nullptr;
					//			if (g_pCosmos->m_nAppType == APP_BROWSERAPP)
					//				pChild = m_Parse.GetChild(_T("browser_app"));
					//			else if (g_pCosmos->m_nAppType == APP_BROWSER)
					//				pChild = m_Parse.GetChild(_T("browser"));
					//			else
					//				pChild = m_Parse.GetChild(_T("browser_eclipse"));
					//			if (pChild)
					//			{
					//				int nCount = pChild->GetCount();
					//				CString strUrls = _T("");
					//				for (int i = 0; i < nCount; i++)
					//				{
					//					CString strUrl = pChild->GetChild(i)->text();
					//					int nPos2 = strUrl.Find(_T(":"));
					//					if (nPos2 != -1)
					//					{
					//						CString strURLHeader = strUrl.Left(nPos2);
					//						if (strURLHeader.CompareNoCase(_T("host")) == 0)
					//						{
					//							strUrl = g_pCosmos->m_strAppPath + strUrl.Mid(nPos2 + 1);
					//						}
					//					}
					//					strUrls += strUrl;
					//					strUrls += _T("|");
					//				}
					//				if (nCount)
					//					g_pCosmos->m_pBrowserFactory->CreateBrowser(NULL, strUrls);
					//			}
					//		}
					//		break;
					//		}
					//	}
					//}
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
					if (g_pCosmos->m_pMDIMainWnd == NULL && g_pCosmos->m_mapMDTFrame.size() == 0)
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

