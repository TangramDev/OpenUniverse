/********************************************************************************
*					Open Universe - version 0.9.99								*
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

// Hubble.cpp : Implementation of CHubble

#include "stdafx.h"
#include "Hubble.h"
#include "UniverseApp.h"
#include "atlenc.h"
#include <io.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "GridHelperWnd.h"
#include "grid.h"
#include "Quasar.h"
#include "GridWnd.h"
#include "TangramJavaHelper.h"
#include "TangramCoreEvents.h"
#include "Wormhole.h"

#include <shellapi.h>
#include <shlobj.h>
#define MAX_LOADSTRING 100
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <sys/stat.h>
#include "Markup.h"

using namespace Web;

// CHubble

CHubble::CHubble()
{
	m_pCurrentIPCMsg = nullptr;
	m_pClrHost = nullptr;
	g_pHubble = this;
	m_bCLRObjTemplateInit = false;
	m_bUsingDefaultAppDocTemplate = false;
	m_bWinFormActived = false;
	m_bCanClose = false;
	m_bDeleteGalaxyCluster = false;
	m_bEnableProcessFormTabKey = false;
	m_bChromeNeedClosed = false;
	m_pActiveHtmlWnd = nullptr;
	m_bCreatingDevTool = false;
	m_bOMNIBOXPOPUPVISIBLE = false;
	m_pActiveWinFormWnd = nullptr;
	m_pHostHtmlWnd = nullptr;
	m_pHtmlWndCreated = nullptr;
	m_strAppXml = _T("");
	m_strDefaultXml = _T("<default><layout><grid name=\"tangram\" gridtype=\"nucleus\"/></layout></default>");
	m_bNewFile = FALSE;
	m_nRef = 4;
	m_nAppID = -1;
	m_nAppType = 0;
	m_hTangramWnd = NULL;
	m_hHostBrowserWnd = NULL;
	m_hHostWnd = NULL;
	m_hEclipseHideWnd = NULL;
	m_hActiveWnd = NULL;
	m_hTemplateWnd = NULL;
	m_hTemplateChildWnd = NULL;
	m_hChildHostWnd = NULL;
	m_hCBTHook = NULL;
	m_hVSToolBoxWnd = NULL;
	m_hForegroundIdleHook = NULL;
	m_lpszSplitterClass = nullptr;
	//m_pGalaxyCluster = nullptr;
	m_pActiveGrid = nullptr;
	m_pQuasar = nullptr;
	m_pDesignGrid = nullptr;
	m_pRootNodes = nullptr;
	m_pUniverseAppProxy = nullptr;
	m_pCosmosAppProxy = nullptr;
	m_pActiveAppProxy = nullptr;
	m_pCLRProxy = nullptr;
	m_pHubblePackageProxy = nullptr;
	m_strStartupCLRObj = _T("");
	m_strWorkBenchStrs = _T("");
	m_strExeName = _T("");
	m_strAppName = _T("Tangram System");
	m_strAppKey = _T("");
	m_strMainWndXml = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentAppID = _T("");
	m_strConfigFile = _T("");
	m_strConfigDataFile = _T("");
	m_strAppCommonDocPath = _T("");
	m_strGridSelectedText = _T("");
	m_strTemplatePath = _T("");
	m_strDesignerTip1 = _T("");
	m_strDesignerTip2 = _T("");
	m_strDesignerXml = _T("");
	m_strNewDocXml = _T("");
	m_strExcludeAppExtenderIDs = _T("");
	m_strCurrentDocTemplateXml = _T("");
	m_strCurrentFrameID = _T("");
	m_strDocFilters = _T("");
	m_strStartJarPath = _T("");
	m_strBridgeJavaClass = "";
	m_strDocTemplateStrs = _T("");
	m_strDefaultTemplate = _T("");
	m_strDefaultTemplate2 = _T("");
	m_strCurrentEclipsePagePath = _T("");
	m_strDesignerToolBarCaption = _T("Hubble Designer");
	m_nTangramObj = 0;
#ifdef _DEBUG
	m_nTangram = 0;
	m_nJsObj = 0;
	m_nTangramCtrl = 0;
	m_nTangramFrame = 0;
	m_nOfficeDocs = 0;
	m_nOfficeDocsSheet = 0;
	m_nTangramNodeCommonData = 0;
#endif
	m_TabWndClassInfoDictionary[TGM_NUCLEUS] = RUNTIME_CLASS(CGridHelperWnd);
	m_TabWndClassInfoDictionary[_T("grid")] = RUNTIME_CLASS(CGridWnd);
	m_mapEventDic[_T("textchanged")] = 1;
	m_mapEventDic[_T("keydown")] = 2;
	m_mapEventDic[_T("onclick")] = 3;

	m_mapIPCMsgIndexDic[IPC_NODE_CREARED_ID] = IPC_NODE_CREARED;
	m_mapIPCMsgIndexDic[IPC_NODE_ONMOUSEACTIVATE_ID] = IPC_NODE_ONMOUSEACTIVATE;
	m_mapIPCMsgIndexDic[IPC_MDIWINFORM_ACTIVEMDICHILD_ID] = IPC_MDIWINFORM_ACTIVEMDICHILD;
}

typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}

void CHubble::Init()
{
	static bool bInit = false;
	if (bInit)
		return;
	bInit = true;
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);

	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64)
		m_b64bitSystem = true;

	if (m_bOfficeApp == false && m_nAppID != 9)
	{
		//m_strConfigDataFile = m_strAppDataPath;
		if (::PathIsDirectory(m_strAppDataPath) == false)
		{
			::SHCreateDirectory(nullptr, m_strAppDataPath);
		}
		m_strAppFormsPath = m_strAppPath + _T("CommonForms\\");
		if (::PathIsDirectory(m_strAppFormsPath) == false)
		{
			::SHCreateDirectory(nullptr, m_strAppFormsPath);
		}
		m_strAppFormsInfoPath = m_strAppDataPath + _T("TangramFormsInfo\\");
		if (::PathIsDirectory(m_strAppFormsInfoPath) == false)
		{
			::SHCreateDirectory(nullptr, m_strAppFormsInfoPath);
		}
	}

	SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer);
	m_strProgramFilePath = CString(m_szBuffer);

	m_strAppCommonDocPath2 = m_strProgramFilePath + _T("\\Tangram\\CommonDocComponent\\");
	m_strAppCommonFormsPath = m_strProgramFilePath + _T("\\Tangram\\CommonForms\\");
	if (::PathIsDirectory(m_strAppCommonFormsPath) == false)
	{
		::SHCreateDirectory(nullptr, m_strAppCommonFormsPath);
	}
	m_mapValInfo[_T("apppath")] = CComVariant(m_strAppPath);
	m_mapValInfo[_T("appdatapath")] = CComVariant(m_strAppDataPath);
	m_mapValInfo[_T("appdatafile")] = CComVariant(m_strConfigDataFile);
	m_mapValInfo[_T("appname")] = CComVariant(m_strExeName);
	m_mapValInfo[_T("appkey")] = CComVariant(m_strAppKey);

	if (m_nAppID != 9)
	{
		HubbleInit();
		HWND hWnd = ::CreateWindowEx(WS_EX_NOACTIVATE, _T("Tangram Message Window Class"), _T(""), WS_VISIBLE | WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, theUniverse.m_hInstance, nullptr);
	}

	if (m_strGridSelectedText == _T(""))
	{
		m_strGridSelectedText = m_strGridSelectedText + _T("  ----Please Select an Object Type From Designer ToolBox for this Tangram View----") +
			_T("\n  you can use Tangram XML to various applications such as ") +
			_T("\n  .net framework application, MFC Application, Eclipcse RCP, ") +
			_T("\n  Office Application etc.") +
			_T("\n  ") +
			_T("\n  ") +
			_T("\n  Creating a \"nucleus\" in this place,if you want to show application") +
			_T("\n  Component come from original application, ") +
			_T("\n  Creating an Object Type other than \"nucleus\" in this place, if you want to show dynamic") +
			_T("\n  Component come from some Components... ");
	}
	if (m_strDesignerTip1 == _T(""))
		m_strDesignerTip1 = _T("  ----Click me to Design This Tangram Object----\n  ");
	if (m_strDesignerTip2 == _T(""))
	{
		m_strDesignerTip2 = m_strDesignerTip2 +
			_T("  ----Tangram Object Information----") +
			_T("\n  ") +
			_T("\n   Object Name:   %s") +
			_T("\n   Object Caption:%s\n\n");
	}

	if (m_nAppID != 9 && m_bOfficeApp == false && ::IsWindow(m_hHostWnd) == false)
	{
		CString strExeName = m_strExeName;
		strExeName.MakeLower();
		m_strDesignerToolBarCaption = strExeName + _T(" - ") + m_strDesignerToolBarCaption;
		m_hHostWnd = ::CreateWindowEx(WS_EX_PALETTEWINDOW, _T("Hubble Grid Class"), m_strDesignerToolBarCaption, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 400, 400, NULL, 0, theUniverse.m_hInstance, NULL);
		m_hChildHostWnd = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_hHostWnd, 0, theUniverse.m_hInstance, NULL);
	}
}

CHubble::~CHubble()
{
	OutputDebugString(_T("------------------Begin Release CHubble------------------------\n"));

	for (auto it : m_mapHubbleDocTemplateInfo)
	{
		delete it.second;
	}
	m_mapHubbleDocTemplateInfo.clear();
	for (auto it : m_mapHubbleFormsTemplateInfo)
	{
		delete it.second;
	}
	m_mapHubbleFormsTemplateInfo.clear();

	if (m_mapWindowPage.size())
	{
		auto it = m_mapWindowPage.begin();
		while (it != m_mapWindowPage.end())
		{
			CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)it->second;
			delete pGalaxyCluster;
			m_mapWindowPage.erase(it);
			if (m_mapWindowPage.size())
				it = m_mapWindowPage.begin();
			else
				it = m_mapWindowPage.end();
		}
	}

	while (m_mapWndGridCollection.size())
	{
		auto it = m_mapWndGridCollection.begin();
		delete it->second;
	}

	while (m_mapWindowProvider.size())
		m_mapWindowProvider.erase(m_mapWindowProvider.begin());

	if (m_nTangramObj)
		TRACE(_T("TangramObj Count: %d\n"), m_nTangramObj);
#ifdef _DEBUG
	if (m_nTangram)
		TRACE(_T("Tangram Count: %d\n"), m_nTangram);
	if (m_nJsObj)
		TRACE(_T("JSObj Count: %d\n"), m_nJsObj);
	if (m_nTangramCtrl)
		TRACE(_T("TangramCtrl Count: %d\n"), m_nTangramCtrl);
	if (m_nTangramFrame)
		TRACE(_T("TangramFrame Count: %d\n"), m_nTangramFrame);
	if (m_nOfficeDocs)
		TRACE(_T("TangramOfficeDoc Count: %d\n"), m_nOfficeDocs);
	if (m_nOfficeDocsSheet)
		TRACE(_T("TangramExcelWorkBookSheet Count: %d\n"), m_nOfficeDocsSheet);
	if (m_nTangramNodeCommonData)
		TRACE(_T("m_nTangramNodeCommonData Count: %d\n"), m_nTangramNodeCommonData);
#endif

	if (m_pExtender)
		m_pExtender->Close();

	if (m_pRootNodes)
		CCommonFunction::ClearObject<CGridCollection>(m_pRootNodes);

	if (m_pClrHost && m_nAppID == -1 && theUniverse.m_bHostCLR == false)
	{
		OutputDebugString(_T("------------------Begin Stop CLR------------------------\n"));
		HRESULT hr = m_pClrHost->Stop();
		ASSERT(hr == S_OK);
		if (hr == S_OK)
		{
			OutputDebugString(_T("------------------Stop CLR Successed!------------------------\n"));
		}
		DWORD dw = m_pClrHost->Release();
		ASSERT(dw == 0);
		if (dw == 0)
		{
			m_pClrHost = nullptr;
			OutputDebugString(_T("------------------ClrHost Release Successed!------------------------\n"));
		}
		OutputDebugString(_T("------------------End Stop CLR------------------------\n"));
	}
	g_pHubble = nullptr;
	OutputDebugString(_T("------------------End Release CHubble------------------------\n"));
}

void CHubble::ExportComponentInfo()
{
};

LRESULT CHubble::Close(void)
{
	if (m_mapEvent.size())
	{
		for (auto it = m_mapEvent.begin(); it != m_mapEvent.end(); it++)
		{
			CCosmosEvent* pObj = it->second;
			delete pObj;
		}
		m_mapEvent.clear();
	}

	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		DISPPARAMS params = { NULL, NULL, 0, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			Unlock();
			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	return S_OK;
}

void CHubble::FireNodeEvent(int nIndex, CGrid* pGrid, CCosmosEvent* pObj)
{
	switch (nIndex)
	{
	case 0:
	{
		GridType type = pGrid->m_nViewType;
		if (type == Grid || type == TabGrid)
		{
			for (auto it : pGrid->m_vChildNodes)
			{
				FireNodeEvent(nIndex, it, pObj);
			}
		}
		else
		{
			for (auto it : pGrid->m_mapWndGridProxy)
			{
				it.second->OnHubbleDocEvent(pObj);
			}
		}
	}
	break;
	case 1:
	{
		for (auto it : pGrid->m_mapWndGridProxy)
		{
			it.second->OnHubbleDocEvent(pObj);
		}
	}
	break;
	case 2:
	{
		for (auto it : pGrid->m_mapWndGridProxy)
		{
			it.second->OnHubbleDocEvent(pObj);
		}
	}
	break;
	}
}

void CHubble::FireAppEvent(CCosmosEvent* pObj)
{
	if (pObj)
	{
		if (m_pUniverseAppProxy)
			m_pUniverseAppProxy->OnHubbleEvent(pObj);
		for (auto it : m_mapHubbleAppProxy)
		{
			if (it.second != m_pUniverseAppProxy)
				it.second->OnHubbleEvent(pObj);
		}
		if (m_pCosmosAppProxy)
			m_pCosmosAppProxy->OnHubbleEvent(pObj);
		CString strEventName = pObj->m_strEventName;
		strEventName.MakeLower();

		HRESULT hr = S_OK;
		int cConnections = m_vec.GetSize();
		int cConnections2 = 0;

		if (cConnections + cConnections2)
		{
			CComVariant avarParams[1];
			avarParams[0] = (IHubbleEventObj*)pObj;
			avarParams[0].vt = VT_DISPATCH;
			DISPPARAMS params = { avarParams, NULL, 1, 0 };
			IDispatch* pConnection = nullptr;
			if (cConnections)
			{
				for (int iConnection = 0; iConnection < cConnections; iConnection++)
				{
					Lock();
					CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
					Unlock();
					pConnection = static_cast<IDispatch*>(punkConnection.p);
					if (pConnection)
					{
						CComVariant varResult;
						hr = pConnection->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
					}
				}
			}
		}

		delete pObj;
	}
}

CString CHubble::GetXmlData(CString strName, CString strXml)
{
	if (strName == _T("") || strXml == _T(""))
		return _T("");
	int nLength = strName.GetLength();
	CString strKey = _T("<") + strName + _T(">");
	int nPos = strXml.Find(strKey);
	if (nPos != -1)
	{
		CString strData1 = strXml.Mid(nPos);
		strKey = _T("</") + strName + _T(">");
		nPos = strData1.Find(strKey);
		if (nPos != -1)
			return strData1.Left(nPos + nLength + 3);
	}
	return _T("");
}

void CHubble::HubbleLoad()
{
	TCHAR szDriver[MAX_PATH] = { 0 };
	TCHAR szDir[MAX_PATH] = { 0 };
	TCHAR szExt[MAX_PATH] = { 0 };
	TCHAR szFile2[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, m_szBuffer, MAX_PATH);

	m_strConfigFile = CString(m_szBuffer);
	m_strConfigFile.MakeLower();
	m_strAppKey = ComputeHash(m_strConfigFile);
	m_strConfigFile += _T(".tangram");
	_tsplitpath_s(m_szBuffer, szDriver, szDir, szFile2, szExt);
	m_strAppPath = szDriver;
	m_strAppPath += szDir;

	HRESULT hr = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, m_szBuffer);
	m_strAppDataPath = CString(m_szBuffer) + _T("\\");
	m_strAppDataPath.Replace(_T("\\\\"), _T("\\"));

	m_strAppDataPath += _T("TangramData\\");
	m_strAppDataPath += m_strExeName;
	m_strAppDataPath += _T("\\");
	m_strAppDataPath += m_strAppKey;
	m_strAppDataPath += _T("\\");
}

void CHubble::HubbleInitFromeWeb()
{
	CTangramXmlParse m_Parse;
	if (m_Parse.LoadXml(m_strAppXml))
	{
		CTangramXmlParse* pParse = nullptr;
		pParse = m_Parse.GetChild(_T("appdata"));
		if (pParse)
		{
			m_mapValInfo[_T("appdata")] = CComVariant(pParse->xml());
			::PostAppMessage(g_pHubble->m_dwThreadID, WM_COSMOSMSG, 0, 20200603);
		}
		pParse = m_Parse.GetChild(_T("modules"));
		if (pParse)
		{
			int nCount = pParse->GetCount();
			LONGLONG hHandle = 0;
			for (int i = 0; i < nCount; i++)
			{
				CTangramXmlParse* pCLRApp = pParse->GetChild(i);
				BSTR bstrAppXML = pCLRApp->xml().AllocSysString();
				InitCLRApp(bstrAppXML, &hHandle);
				::SysFreeString(bstrAppXML);
			}
		}
		pParse = m_Parse.GetChild(_T("ntp"));
		if (pParse)
			m_strNtpXml = m_Parse[_T("ntp")].xml();

		auto it = m_mapBrowserWnd.find(m_hHostBrowserWnd);
		if (it != m_mapBrowserWnd.end())
		{
			CBrowser* pWnd = (CBrowser*)it->second;
			if (pWnd->m_pVisibleWebWnd)
			{
				m_pMainWebPageImpl = pWnd->m_pVisibleWebWnd;

				pParse = m_Parse.GetChild(_T("urls"));
				if (pParse)
				{
					CString strUrls = _T("");
					int nCount = pParse->GetCount();
					for (int i = 0; i < nCount; i++)
					{
						CString strURL = pParse->GetChild(i)->attr(_T("url"), _T(""));
						int nPos2 = strURL.Find(_T(":"));
						if (nPos2 != -1)
						{
							CString strURLHeader = strURL.Left(nPos2);
							if (strURLHeader.CompareNoCase(_T("host")) == 0)
							{
								strURL = g_pHubble->m_strAppPath + strURL.Mid(nPos2 + 1);
							}
						}
						if (strURL != _T(""))
						{
							strUrls += strURL;
							if (i < nCount - 1)
								strUrls += _T("|");
						}
					}
					if (strUrls != _T(""))
					{
						CString strDisposition = _T("");
						strDisposition.Format(_T("%d"), NEW_BACKGROUND_TAB);
						if (pWnd->m_pVisibleWebWnd->m_pChromeRenderFrameHost)
						{
							IPCMsg msg;
							msg.m_strId = L"ADD_URL";
							msg.m_strParam1 = strUrls;
							msg.m_strParam2 = strDisposition;
							pWnd->m_pVisibleWebWnd->m_pChromeRenderFrameHost->SendHubbleMessage(&msg);
						}
					}
				}
			}
		}

	}
}

void CHubble::HubbleInit()
{
	CTangramXmlParse _m_Parse;
	bool bLoad = false;
	m_strConfigDataFile += m_strExeName;
	m_strConfigDataFile += _T(".tangram");
	if (::PathFileExists(m_strConfigFile) == FALSE)
	{
		if (::PathFileExists(m_strConfigDataFile) == TRUE)
			::DeleteFile(m_strConfigDataFile);
		CString strXml = _T("");
		strXml.Format(_T("<%s developermodel='true' companypathname='%s %s'  productname='%s' />"), g_pHubble->m_strExeName, g_pHubble->m_strExeName, _T(" team"), g_pHubble->m_strExeName);
		_m_Parse.LoadXml(strXml);
		_m_Parse.SaveFile(m_strConfigFile);
		bLoad = true;
		m_bEclipse = false;
	}

	if (bLoad == false) {
		bLoad = _m_Parse.LoadFile(m_strConfigFile);
	}
	if (bLoad) {
		m_strAppName = _m_Parse.attr(_T("appname"), _T("Tangram System"));
		m_strStartupCLRObj = _m_Parse.attr(_T("startupclrobj"), _T(""));
	}
}

DWORD CHubble::ExecCmd(const CString cmd, const BOOL setCurrentDirectory)
{
	BOOL  bReturnVal = false;
	STARTUPINFO  si;
	DWORD  dwExitCode = ERROR_NOT_SUPPORTED;
	SECURITY_ATTRIBUTES saProcess, saThread;
	PROCESS_INFORMATION process_info;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = true;

	saThread.nLength = sizeof(saThread);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = false;

	CString currentDirectory = _T("");

	bReturnVal = CreateProcess(NULL,
		(LPTSTR)(LPCTSTR)cmd,
		&saProcess,
		&saThread,
		false,
		DETACHED_PROCESS,
		NULL,
		currentDirectory,
		&si,
		&process_info);

	if (bReturnVal)
	{
		CloseHandle(process_info.hThread);
		WaitForSingleObject(process_info.hProcess, INFINITE);
		GetExitCodeProcess(process_info.hProcess, &dwExitCode);
		CloseHandle(process_info.hProcess);
	}
	//else
	//{
	//	DWORD dw =  GetLastError();
	//	dwExitCode = dw;
	//}

	return dwExitCode;
}

void CHubble::ExitInstance()
{
	if (m_mapEvent.size())
	{
		auto it = m_mapEvent.begin();
		for (it = m_mapEvent.begin(); it != m_mapEvent.end(); it++)
		{
			delete it->second;
		}
		m_mapEvent.clear();
	}

	if (m_hCBTHook)
		UnhookWindowsHookEx(m_hCBTHook);
	if (m_hForegroundIdleHook)
		UnhookWindowsHookEx(m_hForegroundIdleHook);
	for (auto it : m_mapThreadInfo)
	{
		if (it.second->m_hGetMessageHook)
		{
			UnhookWindowsHookEx(it.second->m_hGetMessageHook);
			it.second->m_hGetMessageHook = NULL;
		}
		delete it.second;
	}
	m_mapThreadInfo.erase(m_mapThreadInfo.begin(), m_mapThreadInfo.end());
	_clearObjects();
	if (m_mapWindowPage.size()>1)
	{
		for (auto it2 = m_mapWindowPage.begin(); it2 != m_mapWindowPage.end(); ++it2)
		{
			delete it2->second;
		}
	}
	m_mapWindowPage.clear();

	for (auto it : m_mapObjDic)
	{
		CString strKey = _T(",");
		strKey += it.first;
		strKey += _T(",");
		if (m_strExcludeAppExtenderIDs.Find(strKey) == -1)
			it.second->Release();
		else
			m_strExcludeAppExtenderIDs.Replace(strKey, _T(""));
	}

	m_mapObjDic.erase(m_mapObjDic.begin(), m_mapObjDic.end());

	for (auto it : m_mapValInfo)
	{
		::VariantClear(&it.second);
	}
	m_mapValInfo.erase(m_mapValInfo.begin(), m_mapValInfo.end());
	m_mapValInfo.clear();
}

CommonThreadInfo* CHubble::GetThreadInfo(DWORD ThreadID)
{
	CommonThreadInfo* pInfo = nullptr;

	DWORD nThreadID = ThreadID;
	if (nThreadID == 0)
		nThreadID = GetCurrentThreadId();
	auto it = m_mapThreadInfo.find(nThreadID);
	if (it != m_mapThreadInfo.end())
	{
		pInfo = it->second;
	}
	else
	{
		pInfo = new CommonThreadInfo();
		pInfo->m_hGetMessageHook = NULL;
		m_mapThreadInfo[nThreadID] = pInfo;
	}
	return pInfo;
}

ULONG CHubble::InternalRelease()
{
	return 1;
}

void CHubble::ProcessMsg(LPMSG lpMsg)
{
	switch (lpMsg->message)
	{
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		HWND hPWnd = ::GetAncestor(lpMsg->hwnd, GA_ROOT);
		if (lpMsg->hwnd != hPWnd)
		{
			auto it = m_mapBrowserWnd.find(hPWnd);
			if (it != m_mapBrowserWnd.end())
			{
				HWND _hPWnd = ::GetParent(hPWnd);
				if (_hPWnd == nullptr)
				{
					CBrowser* pBrowserWnd = (CBrowser*)it->second;
					if (pBrowserWnd->m_pOmniboxViewViews && pBrowserWnd->m_pOmniboxViewViews->IsFocused() == false)
						return;
					CWebPage* pWnd = pBrowserWnd->m_pVisibleWebWnd;
					if (pWnd && lpMsg->hwnd != pWnd->m_hWnd)
					{
						CGrid* pRetNode = (CGrid*)::SendMessage(lpMsg->hwnd, WM_TANGRAMGETNODE, 0, 0);
						switch (lpMsg->message)
						{
							//case WM_POINTERDOWN:
						case WM_LBUTTONDOWN:
							if (pRetNode == nullptr || pRetNode->m_nViewType != Grid)
							{
								::SendMessage(pWnd->m_hWnd, WM_LBUTTONDOWN, (WPARAM)lpMsg, 0);
							}
							break;
						case WM_LBUTTONUP:
							//case WM_POINTERUP:
							if (pRetNode && pRetNode->m_nViewType == Grid)
							{
								::PostMessage(pWnd->m_hWnd, WM_LBUTTONDOWN, (WPARAM)lpMsg, 0);
							}
							break;
						}
					}
				}
			}
			return;
		}
		return;
	}
	if (m_pActiveAppProxy)
	{
		HWND hMenuWnd = m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (hMenuWnd && ::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
}

void CHubble::CreateCommonDesignerToolBar()
{
}

void CHubble::AttachGrid(void* pGridEvents)
{
	CGridEvents* m_pCLREventConnector = (CGridEvents*)pGridEvents;
	CGrid* pGrid = (CGrid*)m_pCLREventConnector->m_pGrid;
	pGrid->m_pCLREventConnector = m_pCLREventConnector;
}

long CHubble::GetIPCMsgIndex(CString strMsgID)
{
	auto it = m_mapIPCMsgIndexDic.find(strMsgID);
	if (it != m_mapIPCMsgIndexDic.end())
		return it->second;
	else
		return 0;
}

CSession* CHubble::CreateCloudSession(CWebPageImpl* pOwner)
{
	CWormhole* pSession = new CWormhole();
	pSession->m_pOwner = pOwner ? pOwner : m_pMainWebPageImpl;
	pSession->m_pSession = pSession->m_pOwner->m_pChromeRenderFrameHost->GetIPCSession();
	pSession->Insertint64(_T("domhandle"), (__int64)pSession);
	pSession->InsertString(L"sessionid", GetNewGUID());
	return pSession;
}

CSession* CHubble::GetCloudSession(IGrid* _pGrid)
{
	if (_pGrid)
	{
		CGrid* pGrid = (CGrid*)_pGrid;
		return pGrid->m_pHubbleCloudSession;
	}
	return nullptr;
}

void CHubble::SetMainWnd(HWND hMain)
{
	m_hMainWnd = hMain;
	CWinForm* pWnd = new CWinForm();
	pWnd->SubclassWindow(g_pHubble->m_hMainWnd);
}

IWebPage* CHubble::GetWebPageFromForm(HWND hForm)
{
	auto it = m_mapFormWebPage.find(hForm);
	if (it != m_mapFormWebPage.end())
		return it->second;
	return nullptr;
}

IQuasar* CHubble::ConnectGalaxyCluster(HWND hQuasar, CString _strFrameName, IGalaxyCluster* _pGalaxyCluster, QuasarInfo* pInfo)
{
	CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)_pGalaxyCluster;
	if (pGalaxyCluster->m_hWnd == m_hHostWnd)
		return nullptr;
	CString strFrameName = _strFrameName;

	IQuasar* pQuasar = nullptr;
	pGalaxyCluster->CreateQuasar(CComVariant(0), CComVariant((__int64)hQuasar), strFrameName.AllocSysString(), &pQuasar);
	if (pQuasar)
	{
		CQuasar* _pQuasar = (CQuasar*)pQuasar;
		_pQuasar->m_pQuasarInfo = pInfo;
		_pQuasar->m_strCurrentXml = pInfo->m_strGridXml;
		CComQIPtr<IGrid> pParentNode(pInfo->m_pParentDisp);
		IGrid* pGrid = nullptr;
		CString str = _T("");
		m_mapQuasar2GalaxyCluster[hQuasar] = pGalaxyCluster;

		CString strKey = _T("default");
		str.Format(_T("<%s><layout><grid name='%s' /></layout></%s>"), strKey, _strFrameName, strKey);
		pQuasar->Observe(CComBSTR(strKey), CComBSTR(str), &pGrid);
		if(pGrid)
		{
			CGrid* _pGrid = (CGrid*)pGrid;
			HWND hWnd = _pGrid->m_pGridCommonData->m_pGalaxyCluster->m_hWnd;
			
			CWinForm* pWnd = (CWinForm*)::SendMessage(hWnd, WM_TANGRAMDATA, 0, 20190214);
			if (pWnd)
			{
				//if ((::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD) || (pWnd->m_bMdiForm && pWnd->m_strChildFormPath != _T("")))
					return pQuasar;
			}
			pGrid->put_SaveToConfigFile(true);
		}
	}

	return pQuasar;
}

bool g_bInit = false;
void CHubble::BrowserAppStart()
{
	Init();
	if (g_bInit == true)
		return;
	g_bInit = true;
	auto it = m_mapValInfo.find(_T("startupurl"));
	if(it!=m_mapValInfo.end())
		m_strStartupURL = OLE2T(it->second.bstrVal);
	else
	{
		CTangramXmlParse parse;
		CString strUrl = _T("");
		if (parse.LoadFile(m_strConfigFile))
		{
			CString _strUrl = parse.attr(_T("url"), _T(""));
			if (_strUrl != _T(""))
			{
				strUrl = _strUrl;
				m_strStartupURL = strUrl;
			}
		}
	}
	if (m_strStartupURL!=_T(""))
	{
		if ((m_nAppType != APP_BROWSER) && m_pBrowserFactory && ::IsWindow(m_hChildHostWnd)) {
			if (m_nAppType == APP_BROWSERAPP)
				m_hMainWnd = m_hHostWnd;
			::PostMessage(m_hHostWnd, WM_COSMOSMSG, 0, TANGRAM_CHROME_APP_INIT);
			m_nAppType = APP_BROWSERAPP;
			CString str = _T("<host popup='true'><url></url></host>");
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(str)) {
				CTangramXmlParse* pParse = nullptr;
				m_Parse[_T("url")].put_text(m_strStartupURL);
			}
			m_hHostBrowserWnd = m_pBrowserFactory->CreateBrowser((HWND)m_hChildHostWnd, m_Parse.xml());
		}
		::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 2019111701);
	}
}

bool CHubble::IsMDIClientQuasarNode(IGrid* pGrid)
{
	return false;
}

IGrid* CHubble::ObserveCtrl(__int64 handle, CString name, CString NodeTag)
{
	IQuasar* pQuasar = nullptr;
	GetQuasar(handle, &pQuasar);
	if (pQuasar)
	{
		CString strPath = m_strAppDataPath + name + _T("\\");
		if (::PathIsDirectory(strPath) == false)
		{
			::CreateDirectory(strPath, nullptr);
		}
		strPath += NodeTag + _T(".nodexml");
		if (::PathFileExists(strPath) == false)
		{
			CString strXml = _T("<nodexml><layout><grid name='StartNode' /></layout></nodexml>");
			CTangramXmlParse m_Parse;
			m_Parse.LoadXml(strXml);
			m_Parse.SaveFile(strPath);
		}
		IGrid* pGrid = nullptr;
		pQuasar->Observe(NodeTag.AllocSysString(), strPath.AllocSysString(), &pGrid);
		CQuasar* _pQuasar = (CQuasar*)pQuasar;
		//_pQuasar->m_mapGridScript[strPath] = (CGrid*)pGrid;
		//m_mapTreeCtrlScript[(HWND)handle] = NodeTag;
		return pGrid;
	}
	return nullptr;
};

IGalaxyCluster* CHubble::Observe(HWND hFrame, CString strName, CString strKey)
{
	auto it = m_mapQuasar2GalaxyCluster.find(hFrame);
	if (it != m_mapQuasar2GalaxyCluster.end())
	{
		CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)it->second;
		IQuasar* pQuasar = nullptr;
		auto it2 = pGalaxyCluster->m_mapQuasar.find(hFrame);
		if (it2 == pGalaxyCluster->m_mapQuasar.end())
			pGalaxyCluster->CreateQuasar(CComVariant(0), CComVariant((__int64)hFrame), CComBSTR(strName), &pQuasar);
		else
			pQuasar = it2->second;
		IGrid* pGrid = nullptr;
		CString str = _T("");
		str.Format(_T("<default><layout><grid name='%s' /></layout></default>"), strName);
		pQuasar->Observe(CComBSTR(strKey), CComBSTR(str), &pGrid);
		VARIANT_BOOL bNewVersion;
		pGalaxyCluster->get_NewVersion(&bNewVersion);
		if (pGrid&&!bNewVersion)
		{
			pGrid->put_SaveToConfigFile(true);
		}
		return pGalaxyCluster;
	}
	return nullptr;
};

CString CHubble::GetNewLayoutNodeName(BSTR bstrCnnID, IGrid* pDesignNode)
{
	BOOL bGetNew = false;
	CString strNewName = _T("");
	CString strName = OLE2T(bstrCnnID);
	CString str = m_strExeName + _T(".appwnd.");
	strName.Replace(str, _T(""));
	int nIndex = 0;
	CGrid* _pGrid = ((CGrid*)pDesignNode);
	CGrid* pGrid = _pGrid->m_pRootObj;
	auto it = pGrid->m_pGridCommonData->m_mapLayoutNodes.find(strName);
	if (it == pGrid->m_pGridCommonData->m_mapLayoutNodes.end())
	{
		return strName;
	}
	while (bGetNew == false)
	{
		strNewName.Format(_T("%s%d"), strName, nIndex);
		it = pGrid->m_pGridCommonData->m_mapLayoutNodes.find(strNewName);
		if (it == pGrid->m_pGridCommonData->m_mapLayoutNodes.end())
		{
			return strNewName;
		}
		nIndex++;
	}
	return _T("");
};

CString CHubble::GetDesignerInfo(CString strIndex)
{
	return _T("");
};

int CHubble::LoadCLR()
{
	if (m_pCLRProxy == nullptr && m_pClrHost == nullptr)
	{
		HMODULE	hMscoreeLib = LoadLibrary(TEXT("mscoree.dll"));
		if (hMscoreeLib)
		{
			HubbleCLRCreateInstance CLRCreateInstance = (HubbleCLRCreateInstance)GetProcAddress(hMscoreeLib, "CLRCreateInstance");
			if (CLRCreateInstance)
			{
				HRESULT hrStart = 0;
				ICLRMetaHost* m_pMetaHost = NULL;
				hrStart = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&m_pMetaHost);
				CString strVer = _T("v4.0.30319");
				ICLRRuntimeInfo* lpRuntimeInfo = nullptr;
				hrStart = m_pMetaHost->GetRuntime(strVer.AllocSysString(), IID_ICLRRuntimeInfo, (LPVOID*)&lpRuntimeInfo);
				if (FAILED(hrStart))
					return S_FALSE;
				hrStart = lpRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&m_pClrHost);
				if (FAILED(hrStart))
					return S_FALSE;

				hrStart = m_pClrHost->Start();
				if (FAILED(hrStart))
				{
					return S_FALSE;
				}
				if (hrStart == S_FALSE)
				{
					theUniverse.m_bHostCLR = true;
				}
				else
					m_bEnableProcessFormTabKey = true;

				CString strPath = m_strAppPath + _T("Cosmos.dll");

				HANDLE hFind; // file handle
				WIN32_FIND_DATA FindFileData;
				if (::PathFileExists(strPath) == false)
				{
					strPath = _T("");
					hFind = FindFirstFile(m_strAppPath + _T("Cosmos.dll"), &FindFileData); // find the first file
					if (hFind != INVALID_HANDLE_VALUE)
					{
						bool bSearch = true;
						while (bSearch) // until we finds an entry
						{
							if (FindNextFile(hFind, &FindFileData))
							{
								// Don't care about . and ..
								//if(IsDots(FindFileData.cFileName))
								if ((_tcscmp(FindFileData.cFileName, _T(".")) == 0) ||
									(_tcscmp(FindFileData.cFileName, _T("..")) == 0))
									continue;

								// We have found a directory
								if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
								{
									strPath = m_strAppPath + FindFileData.cFileName + _T("\\");
									strPath += _T("Cosmos.dll");
									if (::PathFileExists(strPath))
									{
										break;
									}
								}

							}//FindNextFile
						}//while
					}
				}

				if (strPath == _T(""))
				{
					strPath = m_strAppPath + _T("PublicAssemblies\\Cosmos.dll"); // find the first file
					if (!::PathFileExists(strPath))
					{
						int nVer = 0;
#ifdef _WIN64
						nVer = 64;
#else
						nVer = 32;
#endif
						strPath.Format(_T("%s\\Microsoft.NET\\assembly\\GAC_%d\\Cosmos\\v4.0_1.0.1992.1963__1bcc94f26a4807a7\\Cosmos.dll"), m_szBuffer, nVer);
					}
				}

				CHubbleImpl* pHubbleProxyBase = static_cast<CHubbleImpl*>(this);
				CString strInfo = _T("");
				strInfo.Format(_T("%I64d"), (__int64)pHubbleProxyBase);
				DWORD dwRetCode = 0;
				hrStart = m_pClrHost->ExecuteInDefaultAppDomain(
					strPath,
					_T("Cosmos.Hubble"),
					_T("HubbleInit"),
					CComBSTR(strInfo),
					&dwRetCode);
				m_pMetaHost->Release();
				m_pMetaHost = nullptr;
				FreeLibrary(hMscoreeLib);
				if (hrStart != S_OK)
					return -1;
			}
		}
	}
	if (m_pCLRProxy != nullptr && m_pClrHost == nullptr)
	{
		HMODULE	hMscoreeLib = LoadLibrary(TEXT("mscoree.dll"));
		HubbleCLRCreateInstance CLRCreateInstance = (HubbleCLRCreateInstance)GetProcAddress(hMscoreeLib, "CLRCreateInstance");
		if (CLRCreateInstance)
		{
			HRESULT hrStart = 0;
			ICLRMetaHost* m_pMetaHost = NULL;
			hrStart = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&m_pMetaHost);
			CString strVer = _T("v4.0.30319");
			ICLRRuntimeInfo* lpRuntimeInfo = nullptr;
			hrStart = m_pMetaHost->GetRuntime(strVer.AllocSysString(), IID_ICLRRuntimeInfo, (LPVOID*)&lpRuntimeInfo);
			if (FAILED(hrStart))
				return S_FALSE;
			hrStart = lpRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&m_pClrHost);
			if (FAILED(hrStart))
				return S_FALSE;
		}
	}
	return 0;
}

CString CHubble::RemoveUTF8BOM(CString strUTF8)
{
	int cc = 0;
	if ((cc = WideCharToMultiByte(CP_UTF8, 0, strUTF8, -1, NULL, 0, 0, 0)) > 2)
	{
		char* cstr = (char*)malloc(cc);
		WideCharToMultiByte(CP_UTF8, 0, strUTF8, -1, cstr, cc, 0, 0);

		if (cstr[0] == (char)0xEF && cstr[1] == (char)0xBB && cstr[2] == (char)0xBF)
		{
			char* new_cstr = (char*)malloc(cc - 3);
			memcpy(new_cstr, cstr + 3, cc - 3);

			CStringW newStrUTF8;
			wchar_t* buf = newStrUTF8.GetBuffer(cc - 3);
			MultiByteToWideChar(CP_UTF8, 0, new_cstr, -1, buf, cc - 3);
			newStrUTF8.ReleaseBuffer();
			free(new_cstr);
			free(cstr);
			return newStrUTF8;
		}

		free(cstr);
	}
	return strUTF8;
}

CGrid* CHubble::ObserveEx(long hWnd, CString strExXml, CString strXml)
{
	strXml = RemoveUTF8BOM(strXml);
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CTangramXmlParse* m_pParse = new CTangramXmlParse();
	bool bXml = m_pParse->LoadXml(strXml);
	if (bXml == false)
		bXml = m_pParse->LoadFile(strXml);

	if (bXml == false)
	{
		delete m_pParse;
		return nullptr;
	}

	BOOL bSizable = m_pParse->attrBool(_T("sizable"), false);
	CTangramXmlParse* pWndGrid = m_pParse->GetChild(TGM_CLUSTER);
	if (pWndGrid == nullptr)
	{
		delete m_pParse;
		return nullptr;
	}

	CTangramXmlParse* pGrid = pWndGrid->GetChild(TGM_NODE);
	if (pGrid == nullptr)
	{
		delete m_pParse;
		return nullptr;
	}

	HWND m_hHostMain = (HWND)hWnd;
	CQuasar* _pQuasar = m_pQuasar;
	CWnd* pWnd = CWnd::FromHandle(m_hHostMain);
	if (pWnd)
	{
		::GetClassName(m_hHostMain, m_szBuffer, MAX_PATH);
		CString strName = CString(m_szBuffer);
		if (strName.Find(_T("AfxMDIClientQuasar")) == 0)
			pWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		else
			pWnd->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	}

	CGrid* pRootGrid = nullptr;
	pRootGrid = _pQuasar->OpenXtmlDocument(m_pParse, m_strCurrentKey, strXml);
	m_strCurrentKey = _T("");
	if (pRootGrid != nullptr)
	{
		if (bSizable)
		{
			HWND hParent = ::GetParent(pRootGrid->m_pHostWnd->m_hWnd);
			CWindow m_wnd;
			m_wnd.Attach(hParent);
			if ((m_wnd.GetStyle() | WS_CHILD) == 0)
			{
				m_wnd.ModifyStyle(0, WS_SIZEBOX | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			}
			m_wnd.Detach();
		}
	}
	return pRootGrid;
}

STDMETHODIMP CHubble::get_ActiveChromeBrowserWnd(IBrowser** ppChromeWebBrowser)
{
	if (m_pActiveBrowser->m_pProxy)
	{
		CBrowser* pBrowserWnd = (CBrowser*)m_pActiveBrowser->m_pProxy;
		pBrowserWnd->QueryInterface(__uuidof(IBrowser), (void**)ppChromeWebBrowser);
	}
	return S_OK;
}

STDMETHODIMP CHubble::get_HostChromeBrowserWnd(IBrowser** ppChromeWebBrowser)
{
	if (::GetModuleHandle(L"chrome_elf.dll"))
	{
		auto it = m_mapBrowserWnd.find(m_hHostBrowserWnd);
		if (it != m_mapBrowserWnd.end())
		{
			*ppChromeWebBrowser = it->second;
		}
	}
	return S_OK;
}

STDMETHODIMP CHubble::get_RootNodes(IGridCollection** pGridColletion)
{
	if (m_pRootNodes == nullptr)
	{
		CComObject<CGridCollection>::CreateInstance(&m_pRootNodes);
		m_pRootNodes->AddRef();
	}

	m_pRootNodes->m_pGrids->clear();

	for (auto& it : m_mapWindowPage)
	{
		CGalaxyCluster* pQuasar = (CGalaxyCluster*)it.second;
		for (auto fit : pQuasar->m_mapQuasar)
		{
			CQuasar* pQuasar = fit.second;
			for (auto it : pQuasar->m_mapGrid)
			{
				m_pRootNodes->m_pGrids->push_back(it.second);
			}
		}
	}
	return m_pRootNodes->QueryInterface(IID_IGridCollection, (void**)pGridColletion);
}

STDMETHODIMP CHubble::get_CurrentActiveGrid(IGrid** pVal)
{
	if (m_pActiveGrid)
		*pVal = m_pActiveGrid;

	return S_OK;
}

STDMETHODIMP CHubble::SetHostFocus(void)
{
	m_pQuasar = nullptr;
	return S_OK;
}

STDMETHODIMP CHubble::CreateCLRObj(BSTR bstrObjID, IDispatch** ppDisp)
{
	CString strID = OLE2T(bstrObjID);
	strID.Trim();
	strID.MakeLower();
	int nPos = strID.Find(_T("@"));
	nPos = strID.Find(_T(","));
	if (nPos != -1 || strID.CompareNoCase(_T("chromert")) == 0)
	{
		LoadCLR();

		if (m_pCLRProxy && bstrObjID != L"")
		{
			*ppDisp = m_pCLRProxy->CreateCLRObj(OLE2T(bstrObjID));
			if (*ppDisp)
				(*ppDisp)->AddRef();
		}
	}
	else
	{
		CComPtr<IDispatch> pDisp;
		pDisp.CoCreateInstance(CComBSTR(strID));
		*ppDisp = pDisp.Detach();
		if (*ppDisp)
		{
			(*ppDisp)->AddRef();
			return S_OK;
		}
		else
			return S_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CHubble::get_CreatingGrid(IGrid** pVal)
{
	if (m_pActiveGrid)
		*pVal = m_pActiveGrid;

	return S_OK;
}

STDMETHODIMP CHubble::get_DesignNode(IGrid** pVal)
{
	return S_OK;
}

CString CHubble::EncodeFileToBase64(CString strSRC)
{
	DWORD dwDesiredAccess = GENERIC_READ;
	DWORD dwShareMode = FILE_SHARE_READ;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE hFile = ::CreateFile(strSRC, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE(_T("ERROR: CreateFile failed - %s\n"), strSRC);
		return _T("");
	}
	else
	{
		DWORD dwFileSizeHigh = 0;
		__int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
		qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
		DWORD dwFileSize = qwFileSize;
		if ((dwFileSize == 0) || (dwFileSize == INVALID_FILE_SIZE))
		{
			TRACE(_T("ERROR: GetFileSize failed - %s\n"), strSRC);
			CloseHandle(hFile);
			return _T("");
		}
		else
		{
			BYTE* buffer = new BYTE[dwFileSize];
			memset(buffer, 0, (dwFileSize) * sizeof(BYTE));
			DWORD dwBytesRead = 0;
			if (!ReadFile(hFile, buffer, dwFileSize, &dwBytesRead, NULL))
			{
				TRACE(_T("ERROR: ReadFile failed - %s\n"), strSRC);
				CloseHandle(hFile);
			}
			else
			{
				int nMaxLineLen = dwFileSize * 2;
				char* pDstInfo = new char[nMaxLineLen];
				memset(pDstInfo, 0, dwFileSize * 2);
				Base64Encode(buffer, dwFileSize, pDstInfo, &nMaxLineLen, 0);
				CString strInfo = CA2W(pDstInfo);
				delete[] pDstInfo;
				delete[] buffer;
				CloseHandle(hFile);
				return strInfo;
			}
		}
	}

	return _T("");
}

CString CHubble::Encode(CString strSRC, BOOL bEnCode)
{
	if (bEnCode)
	{
		LPCWSTR srcInfo = strSRC;
		std::string strSrc = (LPCSTR)CW2A(srcInfo, CP_UTF8);
		int nSrcLen = strSrc.length();
		int nDstLen = Base64EncodeGetRequiredLength(nSrcLen);
		char* pDstInfo = new char[nSrcLen * 2];
		memset(pDstInfo, 0, nSrcLen * 2);
		ATL::Base64Encode((BYTE*)strSrc.c_str(), nSrcLen, pDstInfo, &nDstLen);
		CString strInfo = CA2W(pDstInfo);
		delete[] pDstInfo;
		return strInfo;
	}
	else
	{
		long nSrcSize = strSRC.GetLength();
		BYTE* pDecodeStr = new BYTE[nSrcSize];
		memset(pDecodeStr, 0, nSrcSize);
		int nLen = nSrcSize;
		ATL::Base64Decode(CW2A(strSRC), nSrcSize, pDecodeStr, &nLen);
		////直接在内存里面构建CIMAGE,需要使用IStream接口,如何使用
		////构建内存环境 
		//HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nLen); 
		//void * pData = GlobalLock(hGlobal); 
		//memcpy(pData, pDecodeStr, nLen); 
		//// 拷贝位图数据进去 
		//GlobalUnlock(hGlobal); 
		//// 创建IStream 
		//IStream * pStream = NULL; 
		//if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK) 
		//	return _T(""); 
		//// 使用CImage加载位图内存 
		//CImage img; 
		//if (SUCCEEDED(img.Load(pStream)) ) 
		//{ 
		//	//CClientDC dc(this);
		//	////使用内在中构造的图像 直接在对话框上绘图 
		//	//img.Draw(dc.m_hDC, 0, 0, 500, 300); 
		//} 
		////释放内存
		//pStream->Release(); 
		//GlobalFree(hGlobal); 
		CString str = CA2W((char*)pDecodeStr, CP_UTF8);
		delete[] pDecodeStr;
		pDecodeStr = NULL;
		return str;
	}
}

STDMETHODIMP CHubble::Encode(BSTR bstrSRC, VARIANT_BOOL bEncode, BSTR* bstrRet)
{
	CString strSRC = OLE2T(bstrSRC);
	strSRC.Trim();
	if (::PathFileExists(strSRC))
		strSRC = EncodeFileToBase64(strSRC);
	else if (strSRC != _T(""))
		strSRC = Encode(strSRC, bEncode ? true : false);
	::SysFreeString(bstrSRC);
	if (bstrRet != nullptr)
		::SysFreeString(*bstrRet);
	*bstrRet = strSRC.AllocSysString();
	return S_OK;
}

STDMETHODIMP CHubble::get_AppExtender(BSTR bstrKey, IDispatch** pVal)
{
	CString strName = OLE2T(bstrKey);
	strName.MakeLower();
	if (strName != _T(""))
	{
		auto it = m_mapObjDic.find(strName);
		if (it == m_mapObjDic.end())
			return S_OK;
		else {
			*pVal = it->second;
			(*pVal)->AddRef();
		}
	}

	return S_OK;
}

STDMETHODIMP CHubble::put_AppExtender(BSTR bstrKey, IDispatch* newVal)
{
	CString strName = OLE2T(bstrKey);
	strName.Trim();
	strName.MakeLower();
	CString strKey = _T(",");
	strKey += strName;
	strKey += _T(",");
	if (strName != _T(""))
	{
		auto it = m_mapObjDic.find(strName);
		if (it != m_mapObjDic.end())
		{
			m_mapObjDic.erase(it);
			m_strExcludeAppExtenderIDs.Replace(strKey, _T(""));
		}
		if (newVal != nullptr)
		{
			if (strName.CompareNoCase(_T("HostViewNode")) == 0)
			{
				CComQIPtr<IGrid> pGrid(newVal);
				if (pGrid)
					m_pHostViewDesignerNode = pGrid.Detach();
				return S_OK;
			}
			m_mapObjDic[strName] = newVal;
			newVal->AddRef();
			void* pDisp = nullptr;
			if (newVal->QueryInterface(IID_IGrid, (void**)&pDisp) == S_OK
				|| newVal->QueryInterface(IID_IQuasar, (void**)&pDisp) == S_OK
				|| newVal->QueryInterface(IID_IGalaxyCluster, (void**)&pDisp) == S_OK)
			{
				if (m_strExcludeAppExtenderIDs.Find(strKey) == -1)
				{
					m_strExcludeAppExtenderIDs += strKey;
				}
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CHubble::get_RemoteHelperHWND(LONGLONG* pVal)
{
	if (::IsWindow(m_hHostWnd) == false)
	{
		m_hHostWnd = ::CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW, _T("Hubble Grid Class"), m_strDesignerToolBarCaption, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, NULL, NULL, theUniverse.m_hInstance, NULL);
		if (::IsWindow(m_hHostWnd))
			m_hChildHostWnd = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_hHostWnd, NULL, theUniverse.m_hInstance, NULL);
	}
	*pVal = (LONGLONG)m_hHostWnd;
	return S_OK;
}

STDMETHODIMP CHubble::get_DocTemplate(BSTR bstrKey, LONGLONG* pVal)
{
	CString strKey = OLE2T(bstrKey);
	strKey.MakeLower();
	auto it = m_mapTemplateInfo.find(strKey);
	if (it != m_mapTemplateInfo.end())
		*pVal = (LONGLONG)it->second;
	return S_OK;
}

STDMETHODIMP CHubble::get_AppKeyValue(BSTR bstrKey, VARIANT* pVal)
{
	CString strKey = OLE2T(bstrKey);
	if (strKey != _T(""))
	{
		strKey.Trim();
		strKey.MakeLower();
		if (strKey == _T("tangramproxy"))
		{
			(*pVal).vt = VT_I8;
			(*pVal).llVal = (__int64)(CHubbleImpl*)this;
			return S_OK;
		}
		if (strKey == _T("tangramobjcount"))
		{
			(*pVal).vt = VT_I4;
			(*pVal).lVal = (long)m_nTangramObj;
			return S_OK;
		}
		if (strKey == _T("tangrammsgwnd"))
		{
			(*pVal).vt = VT_I8;
			(*pVal).llVal = (LONGLONG)m_hTangramWnd;
			return S_OK;
		}
		if (strKey == _T("clrproxy"))
		{
			(*pVal).vt = VT_I8;
			(*pVal).llVal = (__int64)m_pCLRProxy;
			return S_OK;
		}

		if (strKey == _T("toolboxxml"))
		{
			(*pVal).vt = VT_BSTR;
			pVal->bstrVal = CComBSTR(m_strDesignerXml);
			return S_OK;
		}
		auto it = m_mapValInfo.find(strKey);
		if (it != m_mapValInfo.end())
		{
			*pVal = CComVariant(it->second);
			return S_OK;
		}
	}
	return S_FALSE;
}

STDMETHODIMP CHubble::put_AppKeyValue(BSTR bstrKey, VARIANT newVal)
{
	CString strKey = OLE2T(bstrKey);

	if (strKey == _T(""))
		return S_OK;
	strKey.Trim();
	strKey.MakeLower();

	auto it = m_mapValInfo.find(strKey);
	if (it != m_mapValInfo.end())
	{
		::VariantClear(&it->second);
		m_mapValInfo.erase(it);
	}
	if (newVal.vt == VT_BSTR)
	{
		CString strData = OLE2T(newVal.bstrVal);
		strData = strData.Trim();
		if (strKey == _T("tangramctrlappid") && strData != _T(""))
		{
			m_strCurrentAppID = strData;
		}
		if (strKey == _T("appname") && strData != _T(""))
		{
			m_strAppName = strData;
			::VariantClear(&newVal);
			return S_OK;
		}
	}

	if (strKey.CompareNoCase(_T("CLRProxy")) == 0)
	{
		if (newVal.llVal == 0 && m_pCLRProxy)
		{
			for (auto it : m_mapThreadInfo)
			{
				if (it.second->m_hGetMessageHook)
				{
					UnhookWindowsHookEx(it.second->m_hGetMessageHook);
					it.second->m_hGetMessageHook = NULL;
				}
				delete it.second;
			}
			m_mapThreadInfo.erase(m_mapThreadInfo.begin(), m_mapThreadInfo.end());
			if (m_mapEvent.size())
			{
				auto it = m_mapEvent.begin();
				for (it = m_mapEvent.begin(); it != m_mapEvent.end(); it++)
				{
					delete it->second;
				}
				m_mapEvent.clear();
			}

			m_pCLRProxy = nullptr;

			if (::IsWindow(m_hHostWnd))
			{
				//::DestroyWindow(m_hHostWnd);
				//m_hHostWnd = NULL;
			}

			if (m_hCBTHook)
				UnhookWindowsHookEx(m_hCBTHook);
			if (m_hForegroundIdleHook)
				UnhookWindowsHookEx(m_hForegroundIdleHook);
			_clearObjects();
			m_pCosmosAppProxy = nullptr;
		}
		return S_OK;
	}

	if (strKey.CompareNoCase(_T("unloadclr")) == 0)
	{
		if (m_pClrHost && m_nAppID == -1 && theUniverse.m_bHostCLR == false)
		{
			if (m_hCBTHook)
			{
				UnhookWindowsHookEx(m_hCBTHook);
				m_hCBTHook = nullptr;
			}
			DWORD dw = m_pClrHost->Release();
			ASSERT(dw == 0);
			if (dw == 0)
			{
				m_pClrHost = nullptr;
				m_pCLRProxy = nullptr;
				OutputDebugString(_T("------------------ClrHost Release Successed!------------------------\n"));
			}
			OutputDebugString(_T("------------------End Stop CLR------------------------\n"));
		}
		return S_OK;
	}
	
	m_mapValInfo[strKey] = newVal;
	if (strKey.CompareNoCase(_T("EnableProcessFormTabKey")) == 0)
	{
		m_bEnableProcessFormTabKey = (newVal.vt == VT_I4 && newVal.lVal == 0) ? false : true;
	}

	return S_OK;
}

STDMETHODIMP CHubble::NavigateNode(IGrid* pGrid, BSTR bstrBrowserID, BSTR bstrXnl)
{
	return S_OK;
}

STDMETHODIMP CHubble::MessageBox(LONGLONG hWnd, BSTR bstrContext, BSTR bstrCaption, long nStyle, int* nRet)
{
	*nRet = ::MessageBox((HWND)hWnd, OLE2T(bstrContext), OLE2T(bstrCaption), nStyle);
	return S_OK;
}

CString CHubble::GetNewGUID()
{
	GUID   m_guid;
	CString   strGUID = _T("");
	if (S_OK == ::CoCreateGuid(&m_guid))
	{
		strGUID.Format(_T("%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"),
			m_guid.Data1, m_guid.Data2, m_guid.Data3,
			m_guid.Data4[0], m_guid.Data4[1],
			m_guid.Data4[2], m_guid.Data4[3],
			m_guid.Data4[4], m_guid.Data4[5],
			m_guid.Data4[6], m_guid.Data4[7]);
	}

	return strGUID;
}

CString CHubble::GetPropertyFromObject(IDispatch* pObj, CString strPropertyName)
{
	CString strRet = _T("");
	if (pObj)
	{
		//ITypeLib* pTypeLib = nullptr;
		//ITypeInfo* pTypeInfo = nullptr;
		//pObj->GetTypeInfo(0, 0, &pTypeInfo);
		//if (pTypeInfo)
		//{
		//	pTypeInfo->GetContainingTypeLib(&pTypeLib, 0);
		//	pTypeInfo->Release();
		//	pTypeLib->Release();
		//}

		BSTR szMember = strPropertyName.AllocSysString();
		DISPID dispid = -1;
		HRESULT hr = pObj->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispid);
		if (hr == S_OK)
		{
			DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
			VARIANT result = { 0 };
			EXCEPINFO excepInfo;
			memset(&excepInfo, 0, sizeof excepInfo);
			UINT nArgErr = (UINT)-1;
			HRESULT hr = pObj->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispParams, &result, &excepInfo, &nArgErr);
			if (S_OK == hr && VT_BSTR == result.vt)
			{
				strRet = OLE2T(result.bstrVal);
			}
			::VariantClear(&result);
		}
	}
	return strRet;
}

STDMETHODIMP CHubble::NewGUID(BSTR* retVal)
{
	*retVal = GetNewGUID().AllocSysString();
	return S_OK;
}

STDMETHODIMP CHubble::LoadDocComponent(BSTR bstrLib, LONGLONG* llAppProxy)
{
	CString strLib = OLE2T(bstrLib);
	strLib.Trim();
	strLib.MakeLower();
	BOOL bOK = FALSE;
	if (strLib == _T("") || strLib.CompareNoCase(_T("default")) == 0)
	{
		*llAppProxy = (LONGLONG)m_pUniverseAppProxy;
		return S_OK;
	}
	auto it = m_mapValInfo.find(strLib);
	if (it == m_mapValInfo.end())
	{
		CString strPath = m_strAppCommonDocPath + strLib + _T(".xml");
		CString strPath2 = m_strAppCommonDocPath2 + strLib + _T("\\");
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadFile(strPath))
		{
			strPath2 += m_Parse.attr(_T("LibName"), _T(""));
			strPath2 += _T(".dll");
			if (::PathFileExists(strPath2) && ::LoadLibrary(strPath2))
				bOK = TRUE;
		}
		if (bOK)
		{
			if (m_hForegroundIdleHook == NULL)
				m_hForegroundIdleHook = SetWindowsHookEx(WH_FOREGROUNDIDLE, CUniverse::ForegroundIdleProc, NULL, ::GetCurrentThreadId());
			auto it = m_mapValInfo.find(strLib);
			if (it != m_mapValInfo.end())
			{
				LONGLONG llProxy = it->second.llVal;
				*llAppProxy = llProxy;
				m_mapHubbleAppProxy[strLib] = (IHubbleAppProxy*)llProxy;
			}
			return S_OK;
		}
	}
	else
	{
		if (it->second.vt == VT_I8)
			*llAppProxy = it->second.llVal;
	}
	return S_OK;
}

STDMETHODIMP CHubble::GetCLRControl(IDispatch* CtrlDisp, BSTR bstrNames, IDispatch** ppRetDisp)
{
	CString strNames = OLE2T(bstrNames);
	if (m_pCLRProxy && strNames != _T("") && CtrlDisp)
		*ppRetDisp = m_pCLRProxy->GetCLRControl(CtrlDisp, bstrNames);

	return S_OK;
}

STDMETHODIMP CHubble::ActiveCLRMethod(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData)
{
	LoadCLR();

	if (m_pCLRProxy)
		m_pCLRProxy->ActiveCLRMethod(bstrObjID, bstrMethod, bstrParam, bstrData);

	return S_OK;
}

STDMETHODIMP CHubble::CreateGalaxyCluster(LONGLONG hWnd, IGalaxyCluster** ppGalaxyCluster)
{
	HWND _hWnd = (HWND)hWnd;
	if (::IsWindow(_hWnd))
	{
		CGalaxyCluster* pGalaxyCluster = nullptr;
		auto it = m_mapWindowPage.find(_hWnd);
		if (it != m_mapWindowPage.end())
			pGalaxyCluster = (CGalaxyCluster*)it->second;
		else
		{
			pGalaxyCluster = new CComObject<CGalaxyCluster>();
			pGalaxyCluster->m_hWnd = _hWnd;
			m_mapWindowPage[_hWnd] = pGalaxyCluster;
			for (auto it : m_mapHubbleAppProxy)
			{
				CGalaxyClusterProxy* pHubbleProxy = it.second->OnGalaxyClusterCreated(pGalaxyCluster);
				if (pHubbleProxy)
					pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pHubbleProxy;
			}
		}
		*ppGalaxyCluster = pGalaxyCluster;
	}
	return S_OK;
}

STDMETHODIMP CHubble::GetQuasar(LONGLONG hHostWnd, IQuasar** ppQuasar)
{
	HWND m_hHostMain = (HWND)hHostWnd;
	DWORD dwID = ::GetWindowThreadProcessId(m_hHostMain, NULL);
	CommonThreadInfo* pThreadInfo = GetThreadInfo(dwID);

	CQuasar* m_pQuasar = nullptr;
	auto iter = pThreadInfo->m_mapQuasar.find(m_hHostMain);
	if (iter != pThreadInfo->m_mapQuasar.end())
	{
		m_pQuasar = (CQuasar*)iter->second;
		*ppQuasar = m_pQuasar;
	}

	return S_OK;
}

STDMETHODIMP CHubble::GetItemText(IGrid* pGrid, long nCtrlID, LONG nMaxLengeh, BSTR* bstrRet)
{
	if (pGrid == nullptr)
		return S_OK;
	LONGLONG h = 0;
	pGrid->get_Handle(&h);

	HWND hWnd = (HWND)h;
	if (::IsWindow(hWnd))
	{
		if (nMaxLengeh == 0)
		{
			hWnd = ::GetDlgItem(hWnd, nCtrlID);
			m_HelperWnd.Attach(hWnd);
			CString strText(_T(""));
			m_HelperWnd.GetWindowText(strText);
			m_HelperWnd.Detach();
			*bstrRet = strText.AllocSysString();
		}
		else
		{
			LPWSTR lpsz = _T("");
			::GetDlgItemText(hWnd, nCtrlID, lpsz, nMaxLengeh);
			*bstrRet = CComBSTR(lpsz);
		}
	}
	return S_OK;
}

STDMETHODIMP CHubble::SetItemText(IGrid* pGrid, long nCtrlID, BSTR bstrText)
{
	if (pGrid == nullptr)
		return S_OK;
	LONGLONG h = 0;
	pGrid->get_Handle(&h);

	HWND hWnd = (HWND)h;
	if (::IsWindow(hWnd))
		::SetDlgItemText(hWnd, nCtrlID, OLE2T(bstrText));

	return S_OK;
}

STDMETHODIMP CHubble::StartApplication(BSTR bstrAppID, BSTR bstrXml)
{
	return S_OK;
}

bool CHubble::CheckUrl(CString& url)
{
	char* res = nullptr;
	char		dwCode[20];
	DWORD		dwIndex, dwCodeLen;
	HINTERNET   hSession, hFile;

	url.MakeLower();

	hSession = InternetOpen(_T("Tangram"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession)
	{
		//hFile = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
		hFile = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS, 0);
		if (hFile == NULL)
		{
			InternetCloseHandle(hSession);
			return false;
		}
		dwIndex = 0;
		dwCodeLen = 10;
		HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE, dwCode, &dwCodeLen, &dwIndex);
		res = dwCode;
		if (strcmp(res, "200 ") || strcmp(res, "302 "))
		{
			//200,302未重定位标志    
			if (hFile)
				InternetCloseHandle(hFile);
			InternetCloseHandle(hSession);
			return   true;
		}
	}
	return   false;
}

STDMETHODIMP CHubble::DownLoadFile(BSTR bstrFileURL, BSTR bstrTargetFile, BSTR bstrActionXml)
{
	return S_OK;
}

STDMETHODIMP CHubble::UpdateGrid(IGrid* pGrid)
{
	CGrid* pWndGrid = (CGrid*)pGrid;
	if (pWndGrid)
	{
		if (pWndGrid->m_pWindow)
		{
			if (pWndGrid->m_nActivePage > 0)
			{
				CString strVal = _T("");
				strVal.Format(_T("%d"), pWndGrid->m_nActivePage);
				pWndGrid->m_pHostParse->put_attr(_T("activepage"), strVal);
			}
			pWndGrid->m_pWindow->Save();
		}
		if (pWndGrid->m_nViewType == Grid)
		{
			((CGridWnd*)pWndGrid->m_pHostWnd)->Save();
		}
		for (auto it2 : pWndGrid->m_vChildNodes)
		{
			UpdateGrid(it2);
		}
	}

	return S_OK;
}

HRESULT CHubble::SelectVSObj(BSTR strData, IDispatch* pVSObj, LONGLONG nHandle)
{
	return S_OK;
}

HRESULT CHubble::HubbleNotify(BSTR strXml1, BSTR strXml2, LONGLONG wParam, LONGLONG lParam)
{
	return S_OK;
}

void CHubble::HubbleNotify(CString strXml1, CString strXml2, LONGLONG wParam, LONGLONG lParam)
{
}

HRESULT CHubble::CreateBrowser(ULONGLONG hParentWnd, BSTR bstrUrls, IBrowser** ppRet)
{
	HWND hPWnd = (HWND)hParentWnd;
	CString strUrls = OLE2T(bstrUrls);
	CString strPath = m_strAppPath;
	strUrls.Replace(_T("host:"), strPath);
	if (m_pBrowserFactory)
	{
		strUrls += _T("|");
		strUrls.Replace(_T("||"), _T("|"));
		*ppRet = (IBrowser*)::SendMessage(m_pBrowserFactory->CreateBrowser(hPWnd, strUrls), WM_COSMOSMSG, 20190527, 0);
	}
	return S_OK;
}

STDMETHODIMP CHubble::GetGridFromHandle(LONGLONG hWnd, IGrid** ppRetGrid)
{
	HWND _hWnd = (HWND)hWnd;
	if (::IsWindow(_hWnd))
	{
		LRESULT lRes = ::SendMessage(_hWnd, WM_TANGRAMGETNODE, 0, 0);
		if (lRes)
		{
			CGrid* pGrid = (CGrid*)lRes;
			pGrid->QueryInterface(IID_IGrid, (void**)ppRetGrid);
		}
	}
	return S_OK;
}

CString CHubble::GetDocTemplateXml(CString strCaption, CString _strPath, CString strFilter)
{
	CString strTemplate = _T("");
	return strTemplate;
}

STDMETHODIMP CHubble::get_HostWnd(LONGLONG* pVal)
{
	*pVal = (LONGLONG)m_hHostWnd;

	return S_OK;
}

STDMETHODIMP CHubble::get_RemoteHubble(BSTR bstrID, IHubble** pVal)
{
	return S_OK;
}

STDMETHODIMP CHubble::GetCtrlByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp)
{
	if (m_pCLRProxy)
	{
		*ppRetDisp = m_pCLRProxy->GetCtrlByName(pCtrl, bstrName, bFindInChild ? true : false);
	}
	return S_OK;
}

STDMETHODIMP CHubble::GetCtrlValueByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal)
{
	if (m_pCLRProxy)
	{
		*bstrVal = m_pCLRProxy->GetCtrlValueByName(pCtrl, bstrName, bFindInChild ? true : false);
	}
	return S_OK;
}

STDMETHODIMP CHubble::SetCtrlValueByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal)
{
	if (m_pCLRProxy)
	{
		m_pCLRProxy->SetCtrlValueByName(pCtrl, bstrName, bFindInChild ? true : false, bstrVal);
	}
	return S_OK;
}

STDMETHODIMP CHubble::get_Extender(IHubbleExtender** pVal)
{
	if (m_pExtender)
	{
		*pVal = m_pExtender;
		(*pVal)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CHubble::put_Extender(IHubbleExtender* newVal)
{
	if (m_pExtender == nullptr)
	{
		m_pExtender = newVal;
		m_pExtender->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CHubble::CreateHubbleCtrl(BSTR bstrAppID, IHubbleCtrl** ppRetCtrl)
{
	return S_OK;
}

CCosmosEvent::CCosmosEvent()
{
	m_bNotFired = true;
	m_nEventIndex = 19631965;
	m_strEventName = _T("");
	m_pSourceObj = nullptr;
}

CCosmosEvent::~CCosmosEvent()
{
	auto it = g_pHubble->m_mapEvent.find((LONGLONG)this);
	if (it != g_pHubble->m_mapEvent.end())
		g_pHubble->m_mapEvent.erase(it);
	m_strEventName = _T("");
	//m_pSourceObj->Release();
	m_pSourceObj = nullptr;
	for (auto it : m_mapVar)
	{
		::VariantClear(&it.second);
	}
	m_mapVar.clear();
	for (auto it : m_mapDisp)
	{
		//it.second->Release();
	}
	m_mapDisp.clear();
}

STDMETHODIMP CCosmosEvent::get_EventName(BSTR* pVal)
{
	*pVal = m_strEventName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CCosmosEvent::put_EventName(BSTR newVal)
{
	m_strEventName = OLE2T(newVal);
	return S_OK;
}

STDMETHODIMP CCosmosEvent::get_Index(int* pVal)
{
	*pVal = m_nEventIndex;
	return S_OK;
}

STDMETHODIMP CCosmosEvent::put_Index(int newVal)
{
	m_nEventIndex = newVal;
	return S_OK;
}

STDMETHODIMP CCosmosEvent::get_Object(int nIndex, IDispatch** pVal)
{
	auto it = m_mapDisp.find(nIndex);
	if (it != m_mapDisp.end())
	{
		*pVal = it->second;
		//(*pVal)->AddRef();
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CCosmosEvent::put_Object(int nIndex, IDispatch* newVal)
{
	auto it = m_mapDisp.find(nIndex);
	if (it != m_mapDisp.end())
	{
		it->second->Release();
		m_mapDisp.erase(it);
	}
	if (newVal)
	{
		m_mapDisp[nIndex] = newVal;
		newVal->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CCosmosEvent::get_Value(int nIndex, VARIANT* pVal)
{
	auto it = m_mapVar.find(nIndex);
	if (it != m_mapVar.end())
	{
		*pVal = it->second;
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CCosmosEvent::put_Value(int nIndex, VARIANT newVal)
{
	auto it = m_mapVar.find(nIndex);
	if (it != m_mapVar.end())
	{
		::VariantClear(&it->second);
		m_mapVar.erase(it);
		return S_OK;
	}
	m_mapVar[nIndex] = newVal;
	return S_FALSE;
}

STDMETHODIMP CCosmosEvent::get_eventSource(IDispatch** pVal)
{
	if (m_pSourceObj)
	{
		*pVal = m_pSourceObj;
		(*pVal)->AddRef();
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CCosmosEvent::put_eventSource(IDispatch* pSource)
{
	if (m_pSourceObj == nullptr)
	{
		m_pSourceObj = pSource;
		m_pSourceObj->AddRef();
		return S_OK;
	}
	else
	{
		m_pSourceObj->Release();
		m_pSourceObj = nullptr;
		m_pSourceObj = pSource;
		m_pSourceObj->AddRef();
	}

	return S_FALSE;
}

STDMETHODIMP CHubble::get_TangramDoc(LONGLONG AppProxy, LONGLONG nDocID, IHubbleDoc** pVal)
{
	IHubbleAppProxy* pProxy = (IHubbleAppProxy*)AppProxy;
	*pVal = pProxy->GetDoc(nDocID);

	return S_OK;
}

#include <wincrypt.h>

int CHubble::CalculateByteMD5(BYTE* pBuffer, int BufferSize, CString& MD5)
{
	HCRYPTPROV hProv = NULL;
	DWORD dw = 0;
	// Acquire a cryptographic provider context handle.
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))
	{
		HCRYPTHASH hHash;
		// Create the hash object.
		if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		{
			// Compute the cryptographic hash of the buffer.
			if (CryptHashData(hHash, pBuffer, BufferSize, 0))
			{
				DWORD dwCount = 16;
				unsigned char digest[16];
				CryptGetHashParam(hHash, HP_HASHVAL, digest, &dwCount, 0);

				if (hHash)
					CryptDestroyHash(hHash);

				// Release the provider handle.
				if (hProv)
					CryptReleaseContext(hProv, 0);

				unsigned char b;
				char c;
				char* Value = new char[1024];
				int k = 0;
				for (int i = 0; i < 16; i++)
				{
					b = digest[i];
					for (int j = 4; j >= 0; j -= 4)
					{
						c = ((char)(b >> j) & 0x0F);
						if (c < 10) c += '0';
						else c = ('a' + (c - 10));
						Value[k] = c;
						k++;
					}
				}
				Value[k] = '\0';
				MD5 = CString(Value);
				delete Value;
			}
		}
	}
	else
	{
		dw = GetLastError();
		if (dw == NTE_BAD_KEYSET)               //同样，如果当不存在这样的容器的时候，创建一个
		{
			if (CryptAcquireContext(
				&hProv,
				NULL,
				NULL,
				PROV_RSA_FULL,
				CRYPT_NEWKEYSET))
			{
				_tprintf(TEXT("CryptAcquireContext succeeded.\n"));
			}
			else
			{
				_tprintf(TEXT("CryptAcquireContext falied.\n"));
			}
		}
	}

	return 1;
}

CString CHubble::ComputeHash(CString source)
{
	std::string strSrc = (LPCSTR)CW2A(source, CP_UTF8);
	int nSrcLen = strSrc.length();
	CString strRet = _T("");
	CalculateByteMD5((BYTE*)strSrc.c_str(), nSrcLen, strRet);
	return strRet;
}


BOOL CHubble::IsUserAdministrator()
{
	BOOL bRet = false;
	PSID psidRidGroup;
	SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;

	bRet = AllocateAndInitializeSid(&siaNtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &psidRidGroup);
	if (bRet)
	{
		if (!CheckTokenMembership(NULL, psidRidGroup, &bRet))
			bRet = false;
		FreeSid(psidRidGroup);
	}

	return (BOOL)bRet;
}


STDMETHODIMP CHubble::GetWindowClientDefaultNode(IDispatch* pAddDisp, LONGLONG hParent, BSTR bstrWndClsName, BSTR bstrGalaxyClusterName, IGrid** ppGrid)
{
	if (hParent == 0)
		return S_FALSE;
	HWND hPWnd = (HWND)hParent;
	CString strClsName = OLE2T(bstrWndClsName);
	strClsName.Trim();
	if (strClsName == _T(""))
	{
		strClsName = _T("MDIClient");
	}
	strClsName.MakeLower();
	HWND hWnd = ::FindWindowEx(hPWnd, NULL, strClsName, NULL);
	if (hWnd == nullptr)
		return S_FALSE;
	strClsName = OLE2T(bstrGalaxyClusterName);
	strClsName.Trim();
	if (strClsName == _T(""))
	{
		strClsName = _T("default");
	}
	CGalaxyCluster* pGalaxyCluster = nullptr;
	auto it = m_mapWindowPage.find(hPWnd);
	if (it == m_mapWindowPage.end())
	{
		pGalaxyCluster = new CComObject<CGalaxyCluster>;
		pGalaxyCluster->m_hWnd = hPWnd;
		m_mapWindowPage[hPWnd] = pGalaxyCluster;
		for (auto it : m_mapHubbleAppProxy)
		{
			CGalaxyClusterProxy* pProxy = it.second->OnGalaxyClusterCreated(pGalaxyCluster);
			if (pProxy)
				pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pProxy;
		}
	}
	else
		pGalaxyCluster = (CGalaxyCluster*)it->second;
	if (pGalaxyCluster != nullptr)
	{
		pGalaxyCluster->put_ConfigName(strClsName.AllocSysString());
		IQuasar* pQuasar = nullptr;
		pGalaxyCluster->CreateQuasar(CComVariant(0), CComVariant((LONGLONG)hWnd), CComBSTR(L"default"), &pQuasar);
		if (pQuasar)
		{
			return pQuasar->Observe(CComBSTR(L"default"), CComBSTR(L"<default><layout><grid name=\"Start\" /></layout></default>"), ppGrid);
		}
	}
	return S_FALSE;
}

STDMETHODIMP CHubble::GetDocTemplateXml(BSTR bstrCaption, BSTR bstrPath, BSTR bstrFilter, BSTR* bstrTemplatePath)
{
	CString strTemplate = GetDocTemplateXml(OLE2T(bstrCaption), OLE2T(bstrPath), OLE2T(bstrFilter));
	if (strTemplate == _T(""))
		return S_FALSE;
	*bstrTemplatePath = strTemplate.AllocSysString();
	return S_OK;
}

STDMETHODIMP CHubble::CreateHubbleEventObj(IHubbleEventObj** ppHubbleEventObj)
{
	CCosmosEvent* pObj = new CComObject<CCosmosEvent>;
	*ppHubbleEventObj = pObj;
	m_mapEvent[(LONGLONG)pObj] = pObj;
	return S_OK;
}

STDMETHODIMP CHubble::FireHubbleEventObj(IHubbleEventObj* pHubbleEventObj)
{
	CCosmosEvent* pObj = (CCosmosEvent*)pHubbleEventObj;
	if (pObj)
	{
		FireAppEvent(pObj);
	}
	return S_OK;
}

STDMETHODIMP CHubble::ObserveQuasars(LONGLONG hWnd, BSTR bstrFrames, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSave)
{
	LRESULT lRes = ::SendMessage((HWND)hWnd, WM_TANGRAMDATA, 0, 20190214);
	CString _strXml = _T("<");
	CString strKey = OLE2T(bstrKey);
	CWinForm* pWnd = nullptr;
	if (lRes)
	{
		pWnd = (CWinForm*)lRes;
	}
	if (pWnd)
	{
		_strXml += strKey;
		_strXml += _T(">");
	}
	auto it = g_pHubble->m_mapWindowPage.find((HWND)hWnd);
	if (it != g_pHubble->m_mapWindowPage.end())
	{
		CString strFrames = OLE2T(bstrFrames);
		CString strKey = OLE2T(bstrKey);
		CString strXml = OLE2T(bstrXml);
		if (strFrames == _T(""))
		{
			for (auto it1 : ((CGalaxyCluster*)it->second)->m_mapQuasar)
			{
				if (it1.second != ((CGalaxyCluster*)it->second)->m_pBKFrame)
				{
					IGrid* pGrid = nullptr;
					CGrid* pGrid2 = it1.second->m_pContainerNode;
					if (pGrid2)
					{
						it1.second->Observe(CComBSTR(it1.second->m_strCurrentKey), bstrXml, &pGrid);
					}
					else
					{
						it1.second->Observe(bstrKey, bstrXml, &pGrid);
						if (pGrid && bSave)
							pGrid->put_SaveToConfigFile(true);
						if (pWnd && pWnd->m_bMdiForm)
						{
							CGrid* _pGrid = (CGrid*)pGrid;
							CString strXml = _pGrid->m_pGridCommonData->m_pHubbleParse->xml();
							strXml.Replace(_T("/><"), _T("/>\r\n<"));
							strXml.Replace(_T("/>"), _T("></node>"));
							CString s = _T("");
							s.Format(_T("<%s>%s</%s>"), it1.second->m_strQuasarName, strXml, it1.second->m_strQuasarName);
							_strXml += s;
						}
					}
				}
			}
			if (pWnd && pWnd->m_bMdiForm)
			{
				_strXml += _T("</");
				_strXml += strKey;
				_strXml += _T(">");
				auto it = pWnd->m_mapKey.find(strKey);
				if (it == pWnd->m_mapKey.end())
				{
					pWnd->m_mapKey[strKey] = _strXml;
				}
				else
				{
					pWnd->m_mapKey.erase(it);
					pWnd->m_mapKey[strKey] = _strXml;
				}
			}
		}
		else
		{
			strFrames = _T(",") + strFrames;
			for (auto it1 : ((CGalaxyCluster*)it->second)->m_mapQuasar)
			{
				CString strName = _T(",") + it1.second->m_strQuasarName + _T(",");
				if (strFrames.Find(strName) != -1)
				{
					IGrid* pGrid = nullptr;
					it1.second->Observe(bstrKey, bstrXml, &pGrid);
					if (pGrid && bSave)
						pGrid->put_SaveToConfigFile(true);
				}
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CHubble::DeleteQuasar(IQuasar* pQuasar)
{
	CQuasar* _pQuasar = (CQuasar*)pQuasar;
	if (_pQuasar)
	{
		HWND hwnd = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_CHILD, 0, 0, 0, 0, m_hHostWnd, NULL, AfxGetInstanceHandle(), NULL);
		_pQuasar->ModifyHost((LONGLONG)::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_CHILD, 0, 0, 0, 0, (HWND)hwnd, NULL, AfxGetInstanceHandle(), NULL));
		::DestroyWindow(hwnd);
	}
	return S_OK;
}

STDMETHODIMP CHubble::DeletePage(LONGLONG GalaxyClusterHandle)
{
	m_bDeleteGalaxyCluster = TRUE;
	HWND hPage = (HWND)GalaxyClusterHandle;
	auto it = g_pHubble->m_mapWindowPage.find(hPage);
	if (it != g_pHubble->m_mapWindowPage.end())
	{
		CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)it->second;
		auto it2 = pGalaxyCluster->m_mapQuasar.begin();
		while (it2 != pGalaxyCluster->m_mapQuasar.end())
		{
			CQuasar* pQuasar = it2->second;
			pGalaxyCluster->m_mapQuasar.erase(it2);
			if (pQuasar)
			{
				pQuasar->m_pGalaxyCluster = nullptr;
				RECT rc;
				HWND hwnd = pQuasar->m_hWnd;
				int nSize = pQuasar->m_mapGrid.size();
				if (nSize > 1)
				{
					for (auto it : pQuasar->m_mapGrid)
					{
						if (it.second != pQuasar->m_pWorkGrid)
						{
							::SetParent(it.second->m_pHostWnd->m_hWnd, pQuasar->m_pWorkGrid->m_pHostWnd->m_hWnd);
						}
					}
				}
				if (pQuasar->m_pWorkGrid)
				{
					::GetWindowRect(pQuasar->m_pWorkGrid->m_pHostWnd->m_hWnd, &rc);
					pQuasar->GetParent().ScreenToClient(&rc);
					::DestroyWindow(pQuasar->m_pWorkGrid->m_pHostWnd->m_hWnd);
					::SetWindowPos(hwnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE);
				}
			}
			it2 = pGalaxyCluster->m_mapQuasar.begin();
		}
		delete pGalaxyCluster;
	}
	return S_OK;
}

STDMETHODIMP CHubble::OpenTangramFile(IHubbleDoc** ppDoc)
{
	return S_OK;
}

HICON CHubble::GetAppIcon(int nIndex)
{
	switch (nIndex)
	{
	case 0:
	{
		if (m_hLargeIcon == nullptr)
		{
			if (g_pHubble->m_pCLRProxy)
				m_hLargeIcon = g_pHubble->m_pCLRProxy->GetAppIcon(nIndex);
		}
		return m_hLargeIcon;
	}
	break;
	case 1:
	{
		if (m_hSmallIcon == nullptr)
		{
			if (g_pHubble->m_pCLRProxy)
				m_hSmallIcon = g_pHubble->m_pCLRProxy->GetAppIcon(nIndex);
		}
		return m_hSmallIcon;
	}
	break;
	}

	return nullptr;
}

STDMETHODIMP CHubble::OpenTangramDocFile(BSTR bstrFilePath, IHubbleDoc** ppDoc)
{
	return S_OK;
}

STDMETHODIMP CHubble::CreateOfficeDocument(BSTR bstrXml)
{
	return S_OK;
}

STDMETHODIMP CHubble::NewWorkBench(BSTR bstrTangramDoc, IWorkBenchWindow** ppWorkBenchWindow)
{
	return S_OK;
}

STDMETHODIMP CHubble::CreateOutLookObj(BSTR bstrObjType, int nType, BSTR bstrURL, IDispatch** ppRetDisp)
{
	return S_OK;
}

STDMETHODIMP CHubble::InitEclipseApp()
{
	return S_OK;
}

STDMETHODIMP CHubble::InitCLRApp(BSTR strInitXml, LONGLONG* llHandle)
{
	LoadCLR();
	CTangramXmlParse m_Parse;
	if (m_Parse.LoadXml(OLE2T(strInitXml)))
	{
		CString strLib = m_strAppPath + m_Parse.attr(_T("libname"), _T(""));
		CString strObjName = m_Parse.attr(_T("objname"), _T(""));
		CString strFunctionName = m_Parse.attr(_T("functionname"), _T(""));
		if (strLib != _T("") && strObjName != _T("") && strFunctionName != _T(""))
		{
			DWORD dwRetCode = 0;
			HRESULT hrStart = m_pClrHost->ExecuteInDefaultAppDomain(
				strLib,
				strObjName,
				strFunctionName,
				strInitXml,
				&dwRetCode);
			*llHandle = (LONGLONG)dwRetCode;
		}
	}

	return S_OK;
}
//
//void CHubble::ConnectDocTemplate(LPCTSTR strType, LPCTSTR strExt, void* pTemplate)
//{
//	g_pHubble->m_mapTemplateInfo[strType] = (void*)pTemplate;
//	g_pHubble->m_mapTemplateInfo[strExt] = (void*)pTemplate;
//}

__declspec(dllexport) IHubble* __stdcall  GetHubble()
{
	return static_cast<IHubble*>(g_pHubble);
}

__declspec(dllexport) CHubbleImpl* __stdcall  GetHubbleImpl(IHubble** pHubble)
{
	*pHubble = static_cast<IHubble*>(g_pHubble);
	return static_cast<CHubbleImpl*>(g_pHubble);
}

__declspec(dllexport) void __stdcall  SetMainDllLoader(CHubbleMainDllLoader* pLoader)
{
	if (pLoader)
		pLoader->LaunchEx(g_pHubble);
	if (::IsWindow(g_pHubble->m_hMainWnd))
	{
		::DestroyWindow(g_pHubble->m_hMainWnd);
	}
	switch (g_pHubble->m_nAppType)
	{
	case APP_BROWSER:
	case APP_WILLCLOSE:
	case APP_BROWSERAPP:
		::PostQuitMessage(0);
		break;
	}
}

void CHubble::ChromeTabCreated(CChromeTab* pTab)
{
	ATLTRACE(_T("Create Chrome Tab :%x\n"), pTab);
}

void CHubble::OnRenderProcessCreated(CChromeRenderProcess* pProcess)
{

}

CChromeBrowserBase* CHubble::GetChromeBrowserBase(HWND hHostWnd)
{
	CBrowser* pPWnd = nullptr;
	auto it2 = g_pHubble->m_mapBrowserWnd.find(hHostWnd);
	if (it2 != g_pHubble->m_mapBrowserWnd.end())
	{
		pPWnd = (CBrowser*)it2->second;
		return pPWnd->m_pBrowser;
	}
	return nullptr;
}

IBrowser* CHubble::GetHostBrowser(HWND hNodeWnd)
{
	IGrid* pWndGrid = nullptr;
	HRESULT hr = g_pHubble->GetGridFromHandle((LONGLONG)hNodeWnd, &pWndGrid);
	if (hr == S_OK && pWndGrid != nullptr)
	{
		IQuasar* pQuasar = nullptr;
		hr = pWndGrid->get_Quasar(&pQuasar);
		if (hr == S_OK && pQuasar != nullptr)
		{
			IBrowser* pChromeWebBrowser = nullptr;
			hr = pQuasar->get_HostBrowser(&pChromeWebBrowser);
			if (hr == S_OK && pChromeWebBrowser != nullptr)
			{
				return pChromeWebBrowser;
			}
		}
	}
	return nullptr;
}

void CHubble::OnDocumentOnLoadCompleted(CChromeRenderFrameHost* pFrameHostBase, HWND hHtmlWnd, void* pVoid)
{
	CChromeRenderFrameHost* pHost = (CChromeRenderFrameHost*)pFrameHostBase;
	HWND hPWnd = ::GetParent(hHtmlWnd);
	auto it = g_pHubble->m_mapHtmlWnd.find(hHtmlWnd);
	if (it != g_pHubble->m_mapHtmlWnd.end())
	{
		((CWebPage*)it->second)->m_pChromeRenderFrameHost = pFrameHostBase;
		// Set m_pProxy to CChromeRenderFrameHostProxyBase
		// TODO: Not work
		pFrameHostBase->m_pProxy = (CWebPageImpl*)it->second;
		if (pVoid)
		{
		}
		else
		{
			::PostMessage(hHtmlWnd, WM_COSMOSMSG, 20190115, 0);
		}
	}
}

void CHubble::ChromeChildProcessCreated(
	CChromeChildProcessHostImpl*
	pChromeChildProcessHostImpl)
{

}

void CHubble::OnSubBrowserWndCreated(HWND hParent, HWND hBrowser)
{
	m_hParent = NULL;
	LRESULT lRes = ::SendMessage(::GetParent(hBrowser), WM_TANGRAMGETNODE, 0, (LPARAM)hBrowser);
}

CString CHubble::GetProcessPath(const char* _ver, CString process_type)
{
	if (g_pHubble == nullptr)
		return _T("");
	CString strRet = _T("");
	strRet.Format(_T("%suniverse.exe"), m_strAppPath, process_type);
	//strRet.Format(_T("%stangram%s.exe"), m_strAppPath, process_type);
	if (::PathFileExists(strRet))
		return strRet;
	USES_CONVERSION;
	strRet.Format(_T("%s%s\\universe.exe"), m_strAppPath, A2W(_ver), process_type);
	//strRet.Format(_T("%s%s\\tangram%s.exe"), m_strAppPath, A2W(_ver), process_type);
	if (::PathFileExists(strRet))
		return strRet;
	return _T("");
}

CString CHubble::GetSchemeBaseName()
{
	return m_strExeName.MakeLower();
}

char* CHubble::GetSchemeString(int nType, CString strKey)
{
	switch (nType)
	{
	case 0:
		return "chrome";
	}
	return nullptr;
}

void CHubble::InserttoDataMap(int nType, CString strKey, void* pData)
{
	switch (nType)
	{
	case 0:
	{
		if (pData)
			m_mapHubbleAppProxy[strKey] = (IHubbleAppProxy*)pData;
		else
		{
			auto it = m_mapHubbleAppProxy.find(strKey);
			if (it != m_mapHubbleAppProxy.end())
			{
				m_mapHubbleAppProxy.erase(it);
			}
		}
	}
	break;
	case 1:
	{
		if (pData)
		{
			m_mapWindowProvider[strKey] = (IHubbleWindowProvider*)pData;
		}
		else
		{
			auto it = m_mapWindowProvider.find(strKey);
			if (it != m_mapWindowProvider.end())
			{
				m_mapWindowProvider.erase(it);
			}
		}
	}
	break;
	}
}
