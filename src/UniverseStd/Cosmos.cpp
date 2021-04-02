/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202104020058           *
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

 // Cosmos.cpp : Implementation of CCosmos

#include "stdafx.h"
#include "Cosmos.h"
#include "UniverseApp.h"
#include "atlenc.h"
#include "EclipsePlus\EclipseAddin.h"

#include <io.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "GridWnd.h"
#include "XobjWnd.h"
#include "Xobj.h"
#include "Wormhole.h"
#include "Galaxy.h"
#include "TangramJavaHelper.h"
#include "CosmosEvents.h"

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
#include "eclipseUnicode.h"
#include "eclipseJni.h"
#include "eclipseCommon.h"

#include "Markup.h"
//#include "vcproject2.h"

using namespace Browser;

#ifdef _WIN32
#ifdef ReplaceText
#undef ReplaceText
#endif // ReplaceText
#endif // _WIN32

#define NAME         _T("-name")
#define VMARGS       _T("-vmargs")					/* special option processing required */
#define LIBRARY		  _T("--launcher.library")
#define SUPRESSERRORS _T("--launcher.suppressErrors")
#define INI			  _T("--launcher.ini")
#define PROTECT	     _T("-protect")	/* This argument is also handled in eclipse.c for Mac specific processing */
#define ROOT		  _T("root")		/* the only level of protection we care now */

_TCHAR* name = NULL;			/* program name */
extern _TCHAR** userVMarg;     		/* user specific args for the Java VM */
int      suppressErrors;/* = 0;	*/			/* supress error dialogs */
int      protectRoot = 0;				/* check if launcher was run as root, currently works only on Linux/UNIX platforms */

LPWSTR* szArglist = nullptr;
extern _TCHAR* jniLib;
extern _TCHAR* jarFile;

extern int initialArgc;
extern _TCHAR** initialArgv;
extern _TCHAR* eclipseLibrary;// = NULL; /* path to the eclipse shared library */
extern JNIEnv* env;

extern int GetLaunchMode();
extern _TCHAR* findStartupJar();

void setInitialArgsW(int argc, _TCHAR** argv, _TCHAR* lib);
int runW(int argc, _TCHAR* argv[], _TCHAR* vmArgs[]);
int readIniFile(_TCHAR* program, int* argc, _TCHAR*** argv);
int readConfigFile(_TCHAR* config_file, int* argc, _TCHAR*** argv);
_TCHAR* getIniFile(_TCHAR* program, int consoleLauncher);

typedef jint(JNICALL* JNI_GetCreatedJavaVMs_)(JavaVM**, jsize, jsize*);

// CCosmos
namespace CommonUniverse
{
	CCosmosImpl* g_pCosmosImpl = nullptr;
}

CCosmos::CCosmos()
{
	m_pCurrentIPCMsg = nullptr;
	m_pClrHost = nullptr;
	m_nJVMVersion = JNI_VERSION_10;
	g_pCosmos = this;
	m_bOfficeAddinUnLoad = true;
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
	m_strDefaultXml = _T("<default><cluster><xobj name=\"tangram\" gridtype=\"nucleus\"/></cluster></default>");
	m_nRef = 4;
	m_nAppID = -1;
	m_nAppType = 0;
	m_hCreatingWnd = NULL;
	m_hCosmosWnd = NULL;
	m_hHostBrowserWnd = NULL;
	m_hEclipseHideWnd = NULL;
	m_hActiveWnd = NULL;
	m_hCBTHook = NULL;
	m_hForegroundIdleHook = NULL;
	m_lpszSplitterClass = nullptr;
	m_pGalaxyCluster = nullptr;
	m_pActiveXobj = nullptr;
	m_pGalaxy = nullptr;
	m_pRootNodes = nullptr;
	m_pUniverseAppProxy = nullptr;
	m_pCosmosAppProxy = nullptr;
	m_pActiveAppProxy = nullptr;
	m_pCLRProxy = nullptr;
	m_pActiveEclipseWnd = nullptr;
	m_strWorkBenchStrs = _T("");
	m_strExeName = _T("");
	m_strAppName = _T("Tangram System");
	m_strAppKey = _T("");
	m_strMainWndXml = _T("");
	m_strDefaultWorkBenchXml = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentAppID = _T("");
	m_strConfigFile = _T("");
	m_strConfigDataFile = _T("");
	m_strAppCommonDocPath = _T("");
	m_strStartJarPath = _T("");
	m_strBridgeJavaClass = "";
	m_strCurrentEclipsePagePath = _T("");
	m_strOfficeAppIDs = _T("word.application,excel.application,outlook.application,onenote.application,infopath.application,project.application,visio.application,access.application,powerpoint.application,lync.ucofficeintegration.1,");
	m_nTangramObj = 0;
	launchMode = -1;
#ifdef _DEBUG
	m_nTangram = 0;
	m_nJsObj = 0;
	m_nTangramCtrl = 0;
	m_nTangramFrame = 0;
	m_nOfficeDocs = 0;
	m_nOfficeDocsSheet = 0;
	m_nTangramNodeCommonData = 0;
#endif
	g_pCosmosImpl = this;
	m_mapValInfo[_T("currenteclipeworkBenchid")] = CComVariant(_T(""));
	m_mapClassInfo[TGM_NUCLEUS] = RUNTIME_CLASS(CXobjWnd);
	m_mapClassInfo[_T("xobj")] = RUNTIME_CLASS(CGridWnd);

	m_mapIPCMsgIndexDic[IPC_NODE_CREARED_ID] = IPC_NODE_CREARED;
	m_mapIPCMsgIndexDic[IPC_NODE_ONMOUSEACTIVATE_ID] = IPC_NODE_ONMOUSEACTIVATE;
	m_mapIPCMsgIndexDic[IPC_MDIWINFORM_ACTIVEMDICHILD_ID] = IPC_MDIWINFORM_ACTIVEMDICHILD;
}

BOOL CCosmos::CopyFolder(CString strSrcPath, CString strDesPath)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	WCHAR _szTmp[1025] = { 0 };
	::memcpy(_szTmp, strSrcPath, 1024);

	CString _szSrcPath = strSrcPath;
	CString _szDesPath = strDesPath;

	WCHAR _szNewSrcPath[1025] = { 0 };
	WCHAR _szNewDesPath[1025] = { 0 };

	::wcscat(_szTmp, L"*");

	hFind = FindFirstFile(_szTmp, &FindFileData);
	if (hFind == NULL) return FALSE;

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (wcscmp(FindFileData.cFileName, L"."))
			{
				if (wcscmp(FindFileData.cFileName, L".."))
				{
					swprintf(_szNewDesPath, L"%s%s\\", _szDesPath.GetBuffer(), FindFileData.cFileName);
					swprintf(_szNewSrcPath, L"%s%s\\", _szSrcPath.GetBuffer(), FindFileData.cFileName);
					_szDesPath.ReleaseBuffer();
					_szSrcPath.ReleaseBuffer();
					CreateDirectory(_szNewDesPath, NULL);
					CopyFolder(_szNewSrcPath, _szNewDesPath);
				}
			}
		}
		else
		{
			WCHAR _szSrcFile[1025] = { 0 };
			WCHAR _szDesFile[1025] = { 0 };
			swprintf(_szDesFile, L"%s%s", _szDesPath.GetBuffer(), FindFileData.cFileName);
			swprintf(_szSrcFile, L"%s%s", _szSrcPath.GetBuffer(), FindFileData.cFileName);
			_szDesPath.ReleaseBuffer();
			_szSrcPath.ReleaseBuffer();
			BOOL l_bRet = CopyFile(_szSrcFile, _szDesFile, TRUE);
		}
	} while (FindNextFile(hFind, &FindFileData));

	::FindClose(hFind);
	return TRUE;
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

void CCosmos::Init()
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
		if (::PathIsDirectory(m_strAppDataPath) == false)
		{
			::SHCreateDirectory(nullptr, m_strAppDataPath);
		}
	}

	SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer);
	m_strProgramFilePath = CString(m_szBuffer);

	m_mapValInfo[_T("apppath")] = CComVariant(m_strAppPath);
	m_mapValInfo[_T("appdatapath")] = CComVariant(m_strAppDataPath);
	m_mapValInfo[_T("appdatafile")] = CComVariant(m_strConfigDataFile);
	m_mapValInfo[_T("appname")] = CComVariant(m_strExeName);
	m_mapValInfo[_T("appkey")] = CComVariant(m_strAppKey);

	if (m_nAppID != 9)
	{
		CosmosInit();
		m_hCosmosWnd = ::CreateWindowEx(WS_EX_NOACTIVATE, _T("Tangram Message Window Class"), _T(""), WS_CHILD, 0, 0, 0, 0, HWND_MESSAGE, nullptr, theApp.m_hInstance, nullptr);
	}


	CString _strPath = _T("");
	_strPath = m_strAppPath + m_strExeName + _T("InitData\\");
	if (::PathIsDirectory(_strPath))
	{
		//auto task = create_task([this, _strPath]()
		//{
		CTangramXmlParse m_Parse;
		CTangramXmlParse* pZipXobj = nullptr;
		BOOL b = m_Parse.LoadFile(m_strConfigDataFile);
		if (b)
		{
			pZipXobj = m_Parse.GetChild(_T("tangramappdata"));
			if (pZipXobj == nullptr)
			{
				CopyFolder(_strPath, m_strAppDataPath);
				pZipXobj = m_Parse.AddNode(_T("tangramappdata"));
				m_Parse.SaveFile(m_strConfigDataFile);
			}
		}
		//});
	}

	if (m_nAppID != 9 && ::PathFileExists(m_strConfigFile) && m_bOfficeApp == false)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadFile(m_strConfigFile))
		{
			CTangramXmlParse* _pXmlParse = m_Parse.GetChild(_T("defaultworkbench"));
			if (_pXmlParse)
			{
				g_pCosmos->m_strDefaultWorkBenchXml = _pXmlParse->xml();
			}
			if (::PathFileExists(m_strConfigDataFile) == FALSE)
			{
				_pXmlParse = m_Parse.GetChild(_T("hubblepage"));
				CString strXml = _T("");
				if (_pXmlParse)
				{
					strXml.Format(_T("<%s>%s</%s>"), m_strExeName, _pXmlParse->xml(), m_strExeName);
					CTangramXmlParse xmlParse;
					if (xmlParse.LoadXml(strXml))
					{
						xmlParse.SaveFile(m_strConfigDataFile);
					}
				}
				else
				{
					if (m_bEclipse)
					{
						strXml.Format(_T("<%s><openedworkbench></openedworkbench></%s>"), m_strExeName, m_strExeName);
						CTangramXmlParse xmlParse;
						if (xmlParse.LoadXml(strXml))
						{
							xmlParse.SaveFile(m_strConfigDataFile);
						}
					}
				}
			}
			else
			{
				if (m_bEclipse)
				{
					CTangramXmlParse xmlParse;
					if (xmlParse.LoadFile(m_strConfigDataFile))
					{
						CTangramXmlParse* pParse = xmlParse.GetChild(_T("openedworkbench"));
						if (pParse)
						{
							m_strWorkBenchStrs = pParse->text();
							pParse->put_text(_T(""));
							xmlParse.SaveFile(m_strConfigDataFile);
						}
					}
				}
			}
		}
	}
}

CCosmos::~CCosmos()
{
	OutputDebugString(_T("------------------Begin Release CCosmos------------------------\n"));

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

	while (m_mapWndXobjCollection.size())
	{
		auto it = m_mapWndXobjCollection.begin();
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
	//if (m_pBrowserFactory)
	//{
	//    delete m_pBrowserFactory;
	//    m_pBrowserFactory = nullptr;
	//}

	if (m_pExtender)
		m_pExtender->Close();

	if (m_pRootNodes)
		CCommonFunction::ClearObject<CXobjCollection>(m_pRootNodes);

	if (m_nAppID == 3)
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

		_clearObjects();

		for (auto it : m_mapObjDic)
		{
			it.second->Release();
		}
		for (auto it : m_mapValInfo)
		{
			::VariantClear(&it.second);
		}
		g_pCosmos->m_mapValInfo.clear();

		m_mapClassInfo.clear();
	}

	m_mapXobj.clear();
	m_mapMDTWindow.clear();
	m_mapWebView.clear();
	m_mapFormWebPage.clear();
	m_mapUIData.clear();

	if (m_pClrHost && m_nAppID == -1 && theApp.m_bHostCLR == false)
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
	g_pCosmos = nullptr;
	OutputDebugString(_T("------------------End Release CCosmos------------------------\n"));
}

LRESULT CCosmos::Close(void)
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

void CCosmos::FireNodeEvent(int nIndex, CXobj* pXobj, CCosmosEvent* pObj)
{
	switch (nIndex)
	{
	case 0:
	{
		XobjType type = pXobj->m_nViewType;
		if (type == Grid || type == TabGrid)
		{
			for (auto it : pXobj->m_vChildNodes)
			{
				FireNodeEvent(nIndex, it, pObj);
			}
		}
		else
		{
			for (auto it : pXobj->m_mapWndXobjProxy)
			{
				it.second->OnCosmosDocEvent(pObj);
			}
		}
	}
	break;
	case 1:
	{
		for (auto it : pXobj->m_mapWndXobjProxy)
		{
			it.second->OnCosmosDocEvent(pObj);
		}
	}
	break;
	case 2:
	{
		for (auto it : pXobj->m_mapWndXobjProxy)
		{
			it.second->OnCosmosDocEvent(pObj);
		}
	}
	break;
	}
}

void CCosmos::FireAppEvent(CCosmosEvent* pObj)
{
	if (pObj)
	{
		if (m_pUniverseAppProxy)
			m_pUniverseAppProxy->OnCosmosEvent(pObj);
		for (auto it : m_mapCosmosAppProxy)
		{
			if (it.second != m_pUniverseAppProxy)
				it.second->OnCosmosEvent(pObj);
		}
		if (m_pCosmosAppProxy)
			m_pCosmosAppProxy->OnCosmosEvent(pObj);
		CString strEventName = pObj->m_strEventName;
		strEventName.MakeLower();
		HRESULT hr = S_OK;
		int cConnections = m_vec.GetSize();

		if (cConnections)
		{
			CComVariant avarParams[1];
			avarParams[0] = (ICosmosEventObj*)pObj;
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

CString CCosmos::GetXmlData(CString strName, CString strXml)
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

BOOL CCosmos::LoadImageFromResource(ATL::CImage* pImage, HMODULE hMod, CString strResID, LPCTSTR lpTyp)
{
	if (pImage == nullptr)
		return false;

	pImage->Destroy();

	// 查找资源
	//HRSRC hRsrc = ::FindResource(hMod, nResID, lpTyp);
	HRSRC hRsrc = ::FindResource(hMod, strResID, lpTyp);
	if (hRsrc == NULL)
		return false;
	HGLOBAL hImgData = ::LoadResource(hMod, hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);

	LPSTREAM pStream = nullptr;
	DWORD dwSize = ::SizeofResource(hMod, hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);
	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, true, &pStream);
	if (ht == S_OK)
	{
		// 加载图片
		pImage->Load(pStream);

	}
	GlobalFree(hNew);
	// 释放资源
	::FreeResource(hImgData);
	return true;
}

BOOL CCosmos::LoadImageFromResource(ATL::CImage* pImage, HMODULE hMod, UINT nResID, LPCTSTR lpTyp)
{
	if (pImage == nullptr)
		return false;

	pImage->Destroy();

	// 查找资源
	//HRSRC hRsrc = ::FindResource(hMod, nResID, lpTyp);
	HRSRC hRsrc = ::FindResource(hMod, MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL)
		return false;
	HGLOBAL hImgData = ::LoadResource(hMod, hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);

	LPSTREAM pStream = nullptr;
	DWORD dwSize = ::SizeofResource(hMod, hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);
	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, true, &pStream);
	if (ht == S_OK)
	{
		// 加载图片
		pImage->Load(pStream);

	}
	GlobalFree(hNew);
	// 释放资源
	::FreeResource(hImgData);
	return true;
}

void CCosmos::CosmosLoad()
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

void CCosmos::TangramInitFromeWeb()
{
	CTangramXmlParse m_Parse;
	if (m_Parse.LoadXml(m_strAppXml))
	{
		m_pHostHtmlWnd->m_strPageName = m_Parse.attr(_T("pagename"), _T("default"));
		CTangramXmlParse* pParse = nullptr;
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
		pParse = m_Parse.GetChild(_T("doctemplate"));
		if (pParse)
		{
			int nCount = pParse->GetCount();
			if (nCount)
			{
				for (int i = 0; i < nCount; i++)
				{
					CTangramXmlParse* pChild = pParse->GetChild(i);
					CString strName = pChild->name();
					CString strDefaultName = pChild->attr(_T("defaultname"), strName);
					m_mapDocTemplate[strName] = pChild->xml();
					m_mapDocDefaultName[strName] = strDefaultName;
				}
				::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)g_pCosmos->m_hFirstView, 20210110);
			}
		}
		pParse = m_Parse.GetChild(_T("defaultworkbench"));
		if (pParse)
			m_strDefaultWorkBenchXml = m_Parse[_T("defaultworkbench")].xml();

		auto it = m_mapBrowserWnd.find(m_hHostBrowserWnd);
		if (it != m_mapBrowserWnd.end())
		{
			CBrowser* pWnd = (CBrowser*)it->second;
			if (pWnd->m_pVisibleWebView)
			{
				m_pMainWebPageImpl = pWnd->m_pVisibleWebView;

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
								strURL = g_pCosmos->m_strAppPath + strURL.Mid(nPos2 + 1);
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
						if (pWnd->m_pVisibleWebView->m_pChromeRenderFrameHost)
						{
							IPCMsg msg;
							msg.m_strId = L"ADD_URL";
							msg.m_strParam1 = strUrls;
							msg.m_strParam2 = strDisposition;
							pWnd->m_pVisibleWebView->m_pChromeRenderFrameHost->SendCosmosMessage(&msg);
						}
					}
					::SetParent(g_pCosmos->m_hCosmosWnd, g_pCosmos->m_hMainWnd);
				}
			}
		}

	}
}

BOOL DelTree(LPCTSTR lpszPath)
{
	SHFILEOPSTRUCT FileOp;
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = lpszPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}

void CCosmos::CosmosInit()
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
		strXml.Format(_T("<%s developermodel='true' companypathname='%s %s'  productname='%s' />"), g_pCosmos->m_strExeName, g_pCosmos->m_strExeName, _T(" team"), g_pCosmos->m_strExeName);
		_m_Parse.LoadXml(strXml);
		_m_Parse.SaveFile(m_strConfigFile);
		bLoad = true;
		m_bEclipse = false;
		CString strplugins = m_strAppPath + _T("plugins\\");
		if (::PathIsDirectory(strplugins)) {
			CString strPath = strplugins + _T("*.jar");
			_wfinddata_t fd;
			fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
			intptr_t pf = _wfindfirst(strPath, &fd);
			if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				m_bEclipse = true;
			}
			else {
				while (!_wfindnext(pf, &fd)) {
					if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0) {
						m_bEclipse = true;
						break;
					}
				}
			}
			_findclose(pf);
		}
		if (m_bEclipse) {
			_m_Parse.put_attr(_T("eclipseapp"), _T("true"));
			_m_Parse.SaveFile(m_strConfigDataFile);
		}
	}

	if (bLoad == false) {
		bLoad = _m_Parse.LoadFile(m_strConfigFile);
	}
	if (bLoad) {
		m_nJVMVersion = _m_Parse.attrInt(_T("jvmver"), JNI_VERSION_10);
		m_strAppName = _m_Parse.attr(_T("appname"), _T("Tangram System"));
	}

	if (m_bEclipse) {
		if (launchMode == -1)
			GetLaunchMode();
		if (launchMode != -1) {
			m_strStartJarPath = _m_Parse.attr(_T("startupjarname"), DEFAULT_EQUINOX_STARTUP);
			CString _strBridgeJavaClass = _m_Parse.attr(_T("bridgeclass"), _T("org.eclipse.equinox.launcher.JNIBridge"));
			_strBridgeJavaClass.Replace(_T("."), _T("/"));
			USES_CONVERSION;
			m_strBridgeJavaClass = T2A(_strBridgeJavaClass);
			jarFile = findStartupJar();
			m_bEclipse = (jarFile != nullptr);
		}
		else
			m_bEclipse = false;
	}
}

void CCosmos::ExitInstance()
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
	if (m_mapWindowPage.size() > 1)
	{
		for (auto it2 = m_mapWindowPage.begin(); it2 != m_mapWindowPage.end(); ++it2)
		{
			delete it2->second;
		}
	}
	m_mapWindowPage.clear();

	for (auto it : m_mapObjDic)
	{
		it.second->Release();
	}

	m_mapObjDic.erase(m_mapObjDic.begin(), m_mapObjDic.end());

	for (auto it : m_mapValInfo)
	{
		if (it.first != _T("dtetangram"))
			::VariantClear(&it.second);
	}
	m_mapValInfo.erase(m_mapValInfo.begin(), m_mapValInfo.end());
	m_mapValInfo.clear();
}

CommonThreadInfo* CCosmos::GetThreadInfo(DWORD ThreadID)
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

ULONG CCosmos::InternalRelease()
{
	if (m_bCanClose == false)
		return 1;
	else if (m_nAppID == 3)
	{
		m_nRef--;
		return m_nRef;
	}

	return 1;
}

void CCosmos::ProcessMsg(LPMSG lpMsg)
{
	switch (lpMsg->message)
	{
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	{
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
					CWebView* pWnd = pBrowserWnd->m_pVisibleWebView;
					if (pWnd && lpMsg->hwnd != pWnd->m_hWnd)
					{
						CXobj* pRetNode = (CXobj*)::SendMessage(lpMsg->hwnd, WM_HUBBLE_GETNODE, 0, 0);
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
	}
	if (m_bEclipse && m_pUniverseAppProxy)
	{
		BOOL bToolBarMg = false;
		IUniverseAppProxy* pProxy = m_pActiveAppProxy;
		HWND hActiveMenu = nullptr;
		if (pProxy == nullptr)
			pProxy = m_pUniverseAppProxy;
		if (pProxy)
		{
			hActiveMenu = pProxy->GetActivePopupMenu(lpMsg->hwnd);
		}
		if (lpMsg->message != WM_LBUTTONDOWN)
		{
			if (m_pCosmosDelegate)
			{
				if (m_pCosmosDelegate->OnUniversePreTranslateMessage(lpMsg))
					return;
			}
			if (pProxy)
			{
				pProxy->UniversePreTranslateMessage(lpMsg);
			}
		}
		else
		{
			::GetClassName(lpMsg->hwnd, m_szBuffer, MAX_PATH);
			CString strClassName = CString(m_szBuffer);
			if (strClassName.Find(_T("Afx:ToolBar:")) == 0)
			{
				bToolBarMg = true;
				ATLTRACE(_T("Afx:ToolBar:%x\n"), lpMsg->hwnd);
				if (::GetWindowLong(::GetParent(lpMsg->hwnd), GWL_STYLE) & WS_POPUP)
				{
					TranslateMessage(lpMsg);
					DispatchMessage(lpMsg);//
					return;
				}
			}
			else if (pProxy)
			{
				pProxy->UniversePreTranslateMessage(lpMsg);
			}
		}
		if (bToolBarMg == false && ::IsChild(hActiveMenu, lpMsg->hwnd) == false)
			::PostMessage(hActiveMenu, WM_CLOSE, 0, 0);
		return;
	}
	if (m_pActiveAppProxy)
	{
		HWND hMenuWnd = m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (hMenuWnd && ::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
}

void CCosmos::AttachXobj(void* pXobjEvents)
{
	CXobjEvents* m_pCLREventConnector = (CXobjEvents*)pXobjEvents;
	CXobj* pXobj = (CXobj*)m_pCLREventConnector->m_pXobj;
	pXobj->m_pCLREventConnector = m_pCLREventConnector;
}

long CCosmos::GetIPCMsgIndex(CString strMsgID)
{
	auto it = m_mapIPCMsgIndexDic.find(strMsgID);
	if (it != m_mapIPCMsgIndexDic.end())
		return it->second;
	else
		return 0;
}

CSession* CCosmos::CreateCloudSession(CWebPageImpl* pOwner)
{
	CWormhole* pSession = new CWormhole();
	pSession->m_pOwner = pOwner ? pOwner : m_pMainWebPageImpl;
	pSession->m_pSession = pSession->m_pOwner->m_pChromeRenderFrameHost->GetIPCSession();
	pSession->Insertint64(_T("domhandle"), (__int64)pSession);
	pSession->InsertString(L"sessionid", GetNewGUID());
	return pSession;
}

CSession* CCosmos::GetCloudSession(IXobj* _pXobj)
{
	if (_pXobj)
	{
		CXobj* pXobj = (CXobj*)_pXobj;
		return pXobj->m_pWormhole;
	}
	return nullptr;
}

void CCosmos::SetMainWnd(HWND hMain)
{
	m_hMainWnd = hMain;
	CWinForm* pWnd = new CWinForm();
	pWnd->SubclassWindow(g_pCosmos->m_hMainWnd);
}

IWebPage* CCosmos::GetWebPageFromForm(HWND hForm)
{
	auto it = m_mapFormWebPage.find(hForm);
	if (it != m_mapFormWebPage.end())
		return it->second;
	return nullptr;
}

IGalaxy* CCosmos::ConnectGalaxyCluster(HWND hGalaxy, CString _strGalaxyName, IGalaxyCluster* _pGalaxyCluster, GalaxyInfo* pInfo)
{
	if (m_nAppID == 9)
		return nullptr;
	CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)_pGalaxyCluster;
	CString strGalaxyName = _strGalaxyName;

	IGalaxy* pGalaxy = nullptr;
	pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((__int64)hGalaxy), strGalaxyName.AllocSysString(), &pGalaxy);
	if (pGalaxy)
	{
		CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
		_pGalaxy->m_pGalaxyInfo = pInfo;
		_pGalaxy->m_strCurrentXml = pInfo->m_strXobjXml;
		CComQIPtr<IXobj> pParentNode(pInfo->m_pParentDisp);
		IXobj* pXobj = nullptr;
		CString str = _T("");
		m_mapGalaxy2GalaxyCluster[hGalaxy] = pGalaxyCluster;
		CString strKey = _T("default");
		str.Format(_T("<%s><cluster><xobj name='%s' /></cluster></%s>"), strKey, _strGalaxyName, strKey);
		pGalaxy->Observe(CComBSTR(strKey), CComBSTR(str), &pXobj);
		CXobj* _pXobj = (CXobj*)pXobj;
		HWND hWnd = _pXobj->m_pXobjShareData->m_pGalaxyCluster->m_hWnd;
		CWinForm* pWnd = (CWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
		if (pWnd)
		{
			if ((::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD) || (pWnd->m_bMdiForm && pWnd->m_strChildFormPath != _T("")))
				return pGalaxy;
		}
	}

	return pGalaxy;
}

bool g_bInit = false;
void CCosmos::BrowserAppStart()
{
	Init();
	if (g_bInit == true)
		return;
	g_bInit = true;
	auto it = m_mapValInfo.find(_T("startupurl"));
	if (it != m_mapValInfo.end())
		g_pCosmos->m_strStartupURL = OLE2T(it->second.bstrVal);
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
				g_pCosmos->m_strStartupURL = strUrl;
			}
		}
	}
	if (g_pCosmos->m_strStartupURL != _T(""))
	{
		if ((m_nAppType != APP_BROWSER) && g_pCosmos->m_pBrowserFactory && ::IsWindow(m_hCosmosWnd)) {
			if (m_nAppType == APP_BROWSERAPP && m_hMainWnd == NULL)
				m_hMainWnd = m_hCosmosWnd;
			::PostMessage(g_pCosmos->m_hCosmosWnd, WM_COSMOSMSG, 0, TANGRAM_CHROME_APP_INIT);
			if (g_pCosmos->m_nAppType != APP_BROWSER_ECLIPSE)
				g_pCosmos->m_nAppType = APP_BROWSERAPP;
			CString str = _T("<host popup='true'><url></url></host>");
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(str)) {
				CTangramXmlParse* pParse = nullptr;
				m_Parse[_T("url")].put_text(m_strStartupURL);
			}
			m_hHostBrowserWnd = m_pBrowserFactory->CreateBrowser((HWND)m_hCosmosWnd, m_Parse.xml());
		}
		::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 2019111701);
	}
}

bool CCosmos::IsMDIClientGalaxyNode(IXobj* pXobj)
{
	CXobj* _pXobj = (CXobj*)pXobj;
	HWND hWnd = _pXobj->m_pXobjShareData->m_pGalaxyCluster->m_hWnd;
	if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
	{
		return false;
	}

	return true;
}

IXobj* CCosmos::ObserveCtrl(__int64 handle, CString name, CString NodeTag)
{
	IGalaxy* pGalaxy = nullptr;
	GetGalaxy(handle, &pGalaxy);
	if (pGalaxy)
	{
		CString strPath = m_strAppDataPath + name + _T("\\");
		if (::PathIsDirectory(strPath) == false)
		{
			::CreateDirectory(strPath, nullptr);
		}
		strPath += NodeTag + _T(".nodexml");
		if (::PathFileExists(strPath) == false)
		{
			CString strXml = _T("<nodexml><cluster><xobj name='StartNode' /></cluster></nodexml>");
			CTangramXmlParse m_Parse;
			m_Parse.LoadXml(strXml);
			m_Parse.SaveFile(strPath);
		}
		IXobj* pXobj = nullptr;
		pGalaxy->Observe(NodeTag.AllocSysString(), strPath.AllocSysString(), &pXobj);
		return pXobj;
	}
	return nullptr;
};

IGalaxyCluster* CCosmos::Observe(HWND hGalaxy, CString strName, CString strKey)
{
	auto it = m_mapGalaxy2GalaxyCluster.find(hGalaxy);
	if (it != m_mapGalaxy2GalaxyCluster.end())
	{
		CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)it->second;
		IGalaxy* pGalaxy = nullptr;
		auto it2 = pGalaxyCluster->m_mapGalaxy.find(hGalaxy);
		if (it2 == pGalaxyCluster->m_mapGalaxy.end())
			pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((__int64)hGalaxy), CComBSTR(strName), &pGalaxy);
		else
			pGalaxy = it2->second;
		IXobj* pXobj = nullptr;
		CString str = _T("");
		str.Format(_T("<default><cluster><xobj name='%s' /></cluster></default>"), strName);
		pGalaxy->Observe(CComBSTR(strKey), CComBSTR(str), &pXobj);
		return pGalaxyCluster;
	}
	return nullptr;
};

CString CCosmos::InitEclipse(_TCHAR* jarFile)
{
	if (m_hForegroundIdleHook == NULL)
		m_hForegroundIdleHook = SetWindowsHookEx(WH_FOREGROUNDIDLE, CUniverse::ForegroundIdleProc, NULL, ::GetCurrentThreadId());
	m_bEnableProcessFormTabKey = true;

	jclass			jarFileClass = nullptr;
	jclass			manifestClass = nullptr;
	jclass			attributesClass = nullptr;

	jmethodID		jarFileConstructor = nullptr;
	jmethodID		getManifestMethod = nullptr;
	jmethodID		getMainAttributesMethod = nullptr;
	jmethodID		closeJarMethod = nullptr;
	jmethodID		getValueMethod = nullptr;

	JNIEnv* pJVMenv = m_pCosmosDelegate->m_pJVMenv;
	jarFileClass = pJVMenv->FindClass("java/util/jar/JarFile");
	if (jarFileClass != nullptr) {
		manifestClass = pJVMenv->FindClass("java/util/jar/Manifest");
		if (manifestClass != nullptr) {
			attributesClass = pJVMenv->FindClass("java/util/jar/Attributes");
		}
	}
	DefaultExceptionProcess(pJVMenv);
	if (attributesClass == nullptr)
		return _T("");
	/* get the classes we need */

	/* find the methods */
	jarFileConstructor = pJVMenv->GetMethodID(jarFileClass, "<init>", "(Ljava/lang/String;Z)V");
	if (jarFileConstructor != nullptr) {
		getManifestMethod = pJVMenv->GetMethodID(jarFileClass, "getManifest", "()Ljava/util/jar/Manifest;");
		if (getManifestMethod != nullptr) {
			closeJarMethod = pJVMenv->GetMethodID(jarFileClass, "close", "()V");
			if (closeJarMethod != nullptr) {
				getMainAttributesMethod = pJVMenv->GetMethodID(manifestClass, "getMainAttributes", "()Ljava/util/jar/Attributes;");
				if (getMainAttributesMethod != nullptr) {
					getValueMethod = pJVMenv->GetMethodID(attributesClass, "getValue", "(Ljava/lang/String;)Ljava/lang/String;");
				}
			}
		}
	}
	DefaultExceptionProcess(pJVMenv);
	if (getValueMethod == nullptr)
		return _T("");

	jobject jarFileObject, manifest, attributes;

	jstring mainClassString = nullptr;
	jstring jarFileString, headerString;

	jarFileString = newJavaString(pJVMenv, jarFile);
	/* headerString = new String("Main-Class"); */
	headerString = newJavaString(pJVMenv, _T("Main-Class"));
	if (jarFileString != nullptr && headerString != nullptr) {
		/* jarfileObject = new JarFile(jarFileString, false); */
		jarFileObject = pJVMenv->NewObject(jarFileClass, jarFileConstructor, jarFileString, JNI_FALSE);
		if (jarFileObject != nullptr) {
			/* manifest = jarFileObject.getManifest(); */
			manifest = pJVMenv->CallObjectMethod(jarFileObject, getManifestMethod);
			if (manifest != nullptr) {
				/*jarFileObject.close() */
				pJVMenv->CallVoidMethod(jarFileObject, closeJarMethod);
				if (!pJVMenv->ExceptionOccurred()) {
					/* attributes = manifest.getMainAttributes(); */
					attributes = pJVMenv->CallObjectMethod(manifest, getMainAttributesMethod);
					if (attributes != nullptr) {
						/* mainClassString = attributes.getValue(headerString); */
						mainClassString = (jstring)pJVMenv->CallObjectMethod(attributes, getValueMethod, headerString);
					}
				}
			}
			pJVMenv->DeleteLocalRef(jarFileObject);
		}
	}

	if (jarFileString != nullptr)
		pJVMenv->DeleteLocalRef(jarFileString);
	if (headerString != NULL)
		pJVMenv->DeleteLocalRef(headerString);

	DefaultExceptionProcess(pJVMenv);

	if (mainClassString == nullptr)
		return _T("");

	const _TCHAR* stringChars = (_TCHAR*)pJVMenv->GetStringChars(mainClassString, 0);
	CString strName = CString(stringChars);
	pJVMenv->ReleaseStringChars(mainClassString, (const jchar*)stringChars);
	strName.Trim();
	strName.Replace(_T("."), _T("/"));

	return strName;
}

void CCosmos::CreateEclipseApp(CString strKey, CString strXml)
{
	if (eclipseLibrary)
		return;
	m_bEclipse = false;
	CString strplugins = m_strAppPath + _T("plugins\\");
	if (::PathIsDirectory(strplugins))
	{
		CString strPath = strplugins + _T("*.jar");

		_wfinddata_t fd;
		fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
		intptr_t pf = _wfindfirst(strPath, &fd);
		if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			m_bEclipse = true;
		}
		else
		{
			while (!_wfindnext(pf, &fd))
			{
				if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					m_bEclipse = true;
					break;
				}
			}
		}
		_findclose(pf);
	}
	if (m_bEclipse)
	{
		if (launchMode == -1)
			GetLaunchMode();
		if (launchMode != -1)
		{
			CTangramXmlParse _m_Parse;
			_m_Parse.LoadFile(m_strConfigFile);
			m_strStartJarPath = _m_Parse.attr(_T("startupjarname"), DEFAULT_EQUINOX_STARTUP);
			CString _strBridgeJavaClass = _m_Parse.attr(_T("bridgeclass"), _T("org.eclipse.equinox.launcher.JNIBridge"));
			_strBridgeJavaClass.Replace(_T("."), _T("/"));
			USES_CONVERSION;
			m_strBridgeJavaClass = T2A(_strBridgeJavaClass);
			jarFile = findStartupJar();
			m_bEclipse = (jarFile != nullptr);
		}
		else
			m_bEclipse = false;
	}
	if (m_bEclipse)
	{
		if (m_pUniverseAppProxy)
		{
			m_pUniverseAppProxy->m_pvoid = nullptr;
			if (m_hCBTHook == NULL)
				m_hCBTHook = SetWindowsHookEx(WH_CBT, CUniverse::CBTProc, NULL, ::GetCurrentThreadId());
			m_bEnableProcessFormTabKey = true;
			GetLaunchMode();
			if (launchMode != -1)
			{
				TCHAR	m_szBuffer[MAX_PATH];
				::GetModuleFileName(theApp.m_hInstance, m_szBuffer, MAX_PATH);
				eclipseLibrary = m_szBuffer;

				setlocale(LC_ALL, "");
				int		nArgs;
				szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
				CEclipseExtender* pExtender = new CComObject<CEclipseExtender>;
				m_pExtender = pExtender;
				runW(nArgs, szArglist, userVMarg);
				::PostQuitMessage(0);
			}
		}
	}
}

int CCosmos::LoadCLR()
{
	if (m_pCLRProxy == nullptr && m_pClrHost == nullptr)
	{
		HMODULE	hMscoreeLib = LoadLibrary(TEXT("mscoree.dll"));
		if (hMscoreeLib)
		{
			CosmosCLRCreateInstance CLRCreateInstance = (CosmosCLRCreateInstance)GetProcAddress(hMscoreeLib, "CLRCreateInstance");
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
					theApp.m_bHostCLR = true;
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

				CCosmosImpl* pCosmosProxyBase = static_cast<CCosmosImpl*>(this);
				CString strInfo = _T("");
				strInfo.Format(_T("%I64d"), (__int64)pCosmosProxyBase);
				DWORD dwRetCode = 0;
				hrStart = m_pClrHost->ExecuteInDefaultAppDomain(
					strPath,
					_T("Universe.Cosmos"),
					_T("CosmosInit"),
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
		CosmosCLRCreateInstance CLRCreateInstance = (CosmosCLRCreateInstance)GetProcAddress(hMscoreeLib, "CLRCreateInstance");
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

CString CCosmos::RemoveUTF8BOM(CString strUTF8)
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

CXobj* CCosmos::ObserveEx(long hWnd, CString strExXml, CString strXml)
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
	CTangramXmlParse* pWndXobj = m_pParse->GetChild(TGM_CLUSTER);
	if (pWndXobj == nullptr)
	{
		delete m_pParse;
		return nullptr;
	}

	CTangramXmlParse* pXobj = pWndXobj->GetChild(TGM_XOBJ);
	if (pXobj == nullptr)
	{
		delete m_pParse;
		return nullptr;
	}

	HWND m_hHostMain = (HWND)hWnd;
	CGalaxy* _pGalaxy = m_pGalaxy;
	CWnd* pWnd = CWnd::FromHandle(m_hHostMain);
	if (pWnd)
	{
		::GetClassName(m_hHostMain, m_szBuffer, MAX_PATH);
		CString strName = CString(m_szBuffer);
		if (strName.Find(_T("AfxMDIClientGalaxy")) == 0)
			pWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		else
			pWnd->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	}

	CXobj* pRootXobj = nullptr;
	m_pGalaxyCluster = nullptr;
	pRootXobj = _pGalaxy->ObserveXtmlDocument(m_pParse, m_strCurrentKey);
	m_strCurrentKey = _T("");
	if (pRootXobj != nullptr)
	{
		if (_pGalaxy->m_nGalaxyType == GalaxyType::CtrlBarGalaxy)
			pRootXobj->m_strCaption = m_pParse->attr(_T("caption"), _T(""));
		if (bSizable)
		{
			HWND hParent = ::GetParent(pRootXobj->m_pHostWnd->m_hWnd);
			CWindow m_wnd;
			m_wnd.Attach(hParent);
			if ((m_wnd.GetStyle() | WS_CHILD) == 0)
			{
				m_wnd.ModifyStyle(0, WS_SIZEBOX | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			}
			m_wnd.Detach();
			::PostMessage(hParent, WM_COSMOSMSG, 0, 1965);
		}
	}
	return pRootXobj;
}

STDMETHODIMP CCosmos::get_ActiveChromeBrowserWnd(IBrowser** ppChromeWebBrowser)
{
	if (m_pActiveBrowser->m_pProxy)
	{
		CBrowser* pBrowserWnd = (CBrowser*)m_pActiveBrowser->m_pProxy;
		pBrowserWnd->QueryInterface(__uuidof(IBrowser), (void**)ppChromeWebBrowser);
	}
	return S_OK;
}

STDMETHODIMP CCosmos::get_HostChromeBrowserWnd(IBrowser** ppChromeWebBrowser)
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

STDMETHODIMP CCosmos::get_RootNodes(IXobjCollection** pXobjColletion)
{
	if (m_pRootNodes == nullptr)
	{
		CComObject<CXobjCollection>::CreateInstance(&m_pRootNodes);
		m_pRootNodes->AddRef();
	}

	m_pRootNodes->m_pXobjs->clear();

	for (auto& it : m_mapWindowPage)
	{
		CGalaxyCluster* pGalaxy = (CGalaxyCluster*)it.second;
		for (auto fit : pGalaxy->m_mapGalaxy)
		{
			CGalaxy* pGalaxy = fit.second;
			for (auto it : pGalaxy->m_mapXobj)
			{
				m_pRootNodes->m_pXobjs->push_back(it.second);
			}
		}
	}
	return m_pRootNodes->QueryInterface(IID_IXobjCollection, (void**)pXobjColletion);
}

STDMETHODIMP CCosmos::get_CurrentActiveXobj(IXobj** pVal)
{
	if (m_pActiveXobj)
		*pVal = m_pActiveXobj;

	return S_OK;
}

STDMETHODIMP CCosmos::SetHostFocus(void)
{
	m_pGalaxy = nullptr;
	return S_OK;
}

STDMETHODIMP CCosmos::CreateCLRObj(BSTR bstrObjID, IDispatch** ppDisp)
{
	CString strID = OLE2T(bstrObjID);
	strID.Trim();
	strID.MakeLower();
	int nPos = strID.Find(_T(","));
	if (nPos != -1 || strID.CompareNoCase(_T("webruntimeproxy")) == 0 || strID.CompareNoCase(_T("chromert")) == 0)
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

STDMETHODIMP CCosmos::get_CreatingXobj(IXobj** pVal)
{
	if (m_pActiveXobj)
		*pVal = m_pActiveXobj;

	return S_OK;
}

CString CCosmos::EncodeFileToBase64(CString strSRC)
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

CString CCosmos::Encode(CString strSRC, BOOL bEnCode)
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

CString CCosmos::tangram_for_eclipse(CString strKey, CString strData, CString strFeatures)
{
	return _T("");
}

STDMETHODIMP CCosmos::get_RemoteHelperHWND(LONGLONG* pVal)
{
	return S_OK;
}

STDMETHODIMP CCosmos::get_DocTemplate(BSTR bstrKey, LONGLONG* pVal)
{
	return S_OK;
}

STDMETHODIMP CCosmos::get_AppKeyValue(BSTR bstrKey, VARIANT* pVal)
{
	CString strKey = OLE2T(bstrKey);
	if (strKey != _T(""))
	{
		strKey.Trim();
		strKey.MakeLower();
		if (strKey == _T("tangramproxy"))
		{
			(*pVal).vt = VT_I8;
			(*pVal).llVal = (__int64)(CCosmosImpl*)this;
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
			(*pVal).llVal = (LONGLONG)m_hCosmosWnd;
			return S_OK;
		}
		if (strKey == _T("eclipseapp"))
		{
			*pVal = CComVariant((bool)m_bEclipse);
			return S_OK;
		}
		if (strKey == _T("clrproxy"))
		{
			(*pVal).vt = VT_I8;
			(*pVal).llVal = (__int64)m_pCLRProxy;
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

STDMETHODIMP CCosmos::put_AppKeyValue(BSTR bstrKey, VARIANT newVal)
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

			if (m_hCBTHook)
				UnhookWindowsHookEx(m_hCBTHook);
			if (m_hForegroundIdleHook)
				UnhookWindowsHookEx(m_hForegroundIdleHook);
			_clearObjects();
			m_pCosmosAppProxy = nullptr;
		}
		return S_OK;
	}

	if (strKey.CompareNoCase(_T("StartData")) == 0)
	{
		m_strCurrentEclipsePagePath = OLE2T(newVal.bstrVal);
		if (m_mapWorkBenchWnd.size())
		{
			CComPtr<IWorkBenchWindow> pWorkBenchWindow;
			NewWorkBench(newVal.bstrVal, &pWorkBenchWindow);
		}
		return S_OK;
	}
	if (strKey.CompareNoCase(_T("unloadclr")) == 0)
	{
		if (m_pClrHost && m_nAppID == -1 && theApp.m_bHostCLR == false)
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

STDMETHODIMP CCosmos::NavigateNode(IXobj* _pXobj, BSTR bstrBrowserID, BSTR bstrXml, IXobj** pRet)
{
	CXobj* pXobj = (CXobj*)_pXobj;
	if (pXobj->m_nViewType == Grid)
	{
		IXobj* pRet = nullptr;
		pXobj->m_pParentObj->ObserveEx(pXobj->m_nRow, pXobj->m_nCol, bstrBrowserID, bstrXml, &pRet);
		if (pRet)
		{
			return S_OK;
		}
	}
	return S_OK;
}

STDMETHODIMP CCosmos::MessageBox(LONGLONG hWnd, BSTR bstrContext, BSTR bstrCaption, long nStyle, int* nRet)
{
	*nRet = ::MessageBox((HWND)hWnd, OLE2T(bstrContext), OLE2T(bstrCaption), nStyle);
	return S_OK;
}

CString CCosmos::GetNewGUID()
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

CString CCosmos::GetPropertyFromObject(IDispatch* pObj, CString strPropertyName)
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

STDMETHODIMP CCosmos::CreateGalaxyCluster(LONGLONG hWnd, IGalaxyCluster** ppGalaxyCluster)
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

			for (auto it : m_mapCosmosAppProxy)
			{
				CGalaxyClusterProxy* pCosmosProxy = it.second->OnGalaxyClusterCreated(pGalaxyCluster);
				if (pCosmosProxy)
					pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pCosmosProxy;
			}
		}
		*ppGalaxyCluster = pGalaxyCluster;
	}
	return S_OK;
}

STDMETHODIMP CCosmos::GetGalaxy(LONGLONG hHostWnd, IGalaxy** ppGalaxy)
{
	HWND m_hHostMain = (HWND)hHostWnd;
	DWORD dwID = ::GetWindowThreadProcessId(m_hHostMain, NULL);
	CommonThreadInfo* pThreadInfo = GetThreadInfo(dwID);

	CGalaxy* m_pGalaxy = nullptr;
	auto iter = pThreadInfo->m_mapGalaxy.find(m_hHostMain);
	if (iter != pThreadInfo->m_mapGalaxy.end())
	{
		m_pGalaxy = (CGalaxy*)iter->second;
		*ppGalaxy = m_pGalaxy;
	}

	return S_OK;
}

STDMETHODIMP CCosmos::GetItemText(IXobj* pXobj, long nCtrlID, LONG nMaxLengeh, BSTR* bstrRet)
{
	if (pXobj == nullptr)
		return S_OK;
	LONGLONG h = 0;
	pXobj->get_Handle(&h);

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

STDMETHODIMP CCosmos::SetItemText(IXobj* pXobj, long nCtrlID, BSTR bstrText)
{
	if (pXobj == nullptr)
		return S_OK;
	LONGLONG h = 0;
	pXobj->get_Handle(&h);

	HWND hWnd = (HWND)h;
	if (::IsWindow(hWnd))
		::SetDlgItemText(hWnd, nCtrlID, OLE2T(bstrText));

	return S_OK;
}

STDMETHODIMP CCosmos::StartApplication(BSTR bstrAppID, BSTR bstrXml)
{
	return S_OK;
}

STDMETHODIMP CCosmos::UpdateXobj(IXobj* pXobj)
{
	CXobj* pWndXobj = (CXobj*)pXobj;
	if (pWndXobj)
	{
		if (pWndXobj->m_pWindow)
		{
			if (pWndXobj->m_nActivePage > 0)
			{
				CString strVal = _T("");
				strVal.Format(_T("%d"), pWndXobj->m_nActivePage);
				pWndXobj->m_pHostParse->put_attr(_T("activepage"), strVal);
			}
			pWndXobj->m_pWindow->Save();
		}
		if (pWndXobj->m_nViewType == Grid)
		{
			((CGridWnd*)pWndXobj->m_pHostWnd)->Save();
		}
		for (auto it2 : pWndXobj->m_vChildNodes)
		{
			UpdateXobj(it2);
		}
	}

	return S_OK;
}

HRESULT CCosmos::CreateBrowser(ULONGLONG hParentWnd, BSTR bstrUrls, IBrowser** ppRet)
{
	HWND hPWnd = (HWND)hParentWnd;
	if (hParentWnd == 1)
		hPWnd = g_pCosmos->m_hCosmosWnd;
	CString strUrls = OLE2T(bstrUrls);
	CString strPath = m_strAppPath;
	strUrls.Replace(_T("host:"), strPath);
	if (m_pBrowserFactory)
	{
		strUrls += _T("|");
		strUrls.Replace(_T("||"), _T("|"));
		HWND hRet = g_pCosmos->m_pBrowserFactory->CreateBrowser(hPWnd, strUrls);
		if (hParentWnd == 1)
		{
			m_hTempBrowserWnd = hRet;
			::PostAppMessage(m_dwThreadID, WM_COSMOSMSG, 0, 20201028);
		}
		*ppRet = (IBrowser*)::SendMessage(hRet, WM_COSMOSMSG, 20190527, 0);
	}
	return S_OK;
}

STDMETHODIMP CCosmos::GetXobjFromHandle(LONGLONG hWnd, IXobj** ppRetXobj)
{
	HWND _hWnd = (HWND)hWnd;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
		*ppRetXobj = pInfo->m_pXobj;
	else
		return S_FALSE;
	return S_OK;
}

CString CCosmos::GetDocTemplateXml(CString strCaption, CString _strPath, CString strFilter)
{
	CString strTemplate = _T("");
	return strTemplate;
}

STDMETHODIMP CCosmos::get_HostWnd(LONGLONG* pVal)
{
	*pVal = (LONGLONG)m_hCosmosWnd;

	return S_OK;
}

STDMETHODIMP CCosmos::get_RemoteCosmos(BSTR bstrID, ICosmos** pVal)
{
	CString strID = OLE2T(bstrID);
	strID.MakeLower();
	auto it = m_mapRemoteCosmos.find(strID);
	if (it != m_mapRemoteCosmos.end())
	{
		*pVal = it->second;
	}
	return S_OK;
}

STDMETHODIMP CCosmos::GetCtrlByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp)
{
	if (m_pCLRProxy)
	{
		*ppRetDisp = m_pCLRProxy->GetCtrlByName(pCtrl, bstrName, bFindInChild ? true : false);
	}
	return S_OK;
}

STDMETHODIMP CCosmos::GetCtrlValueByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal)
{
	if (m_pCLRProxy)
	{
		*bstrVal = m_pCLRProxy->GetCtrlValueByName(pCtrl, bstrName, bFindInChild ? true : false);
	}
	return S_OK;
}

STDMETHODIMP CCosmos::SetCtrlValueByName(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal)
{
	if (m_pCLRProxy)
	{
		m_pCLRProxy->SetCtrlValueByName(pCtrl, bstrName, bFindInChild ? true : false, bstrVal);
	}
	return S_OK;
}

STDMETHODIMP CCosmos::get_Extender(ICosmosExtender** pVal)
{
	if (m_pExtender)
	{
		*pVal = m_pExtender;
		(*pVal)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CCosmos::put_Extender(ICosmosExtender* newVal)
{
	if (m_pExtender == nullptr)
	{
		m_pExtender = newVal;
		m_pExtender->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CCosmos::CreateCosmosCtrl(BSTR bstrAppID, ICosmosCtrl** ppRetCtrl)
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
	auto it = g_pCosmos->m_mapEvent.find((LONGLONG)this);
	if (it != g_pCosmos->m_mapEvent.end())
		g_pCosmos->m_mapEvent.erase(it);
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

STDMETHODIMP CCosmos::AttachObjEvent(IDispatch* pDisp, int nEventIndex)
{
	if (pDisp)
	{
		IDispatch* _pDisp = nullptr;
		if (pDisp->QueryInterface(IID_IDispatch, (void**)_pDisp) == S_OK && _pDisp)
		{
			auto it = m_mapObjEventDic.find(pDisp);
			if (it != m_mapObjEventDic.end())
			{
				CString strEventIndexs = it->second;
				if (nEventIndex >= 0)
				{
					CString strIndex = _T("");
					strIndex.Format(_T(",%d,"), nEventIndex);
					if (strEventIndexs.Find(strIndex) == -1)
					{
						strEventIndexs += strIndex;
						m_mapObjEventDic.erase(it);
						m_mapObjEventDic[pDisp] = strEventIndexs;
					}
				}
			}
			else
			{
				if (nEventIndex >= 0)
				{
					CString strIndex = _T("");
					strIndex.Format(_T(",%d,"), nEventIndex);
					m_mapObjEventDic[pDisp] = strIndex;
				}
			}
		}
	}
	return S_OK;
}

#include <wincrypt.h>

int CCosmos::CalculateByteMD5(BYTE* pBuffer, int BufferSize, CString& MD5)
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

CString CCosmos::ComputeHash(CString source)
{
	std::string strSrc = (LPCSTR)CW2A(source, CP_UTF8);
	int nSrcLen = strSrc.length();
	CString strRet = _T("");
	CalculateByteMD5((BYTE*)strSrc.c_str(), nSrcLen, strRet);
	return strRet;
}


BOOL CCosmos::IsUserAdministrator()
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


STDMETHODIMP CCosmos::GetWindowClientDefaultNode(IDispatch* pAddDisp, LONGLONG hParent, BSTR bstrWndClsName, BSTR bstrGalaxyClusterName, IXobj** ppXobj)
{
	return S_FALSE;
}

STDMETHODIMP CCosmos::CreateCosmosEventObj(ICosmosEventObj** ppCosmosEventObj)
{
	CCosmosEvent* pObj = new CComObject<CCosmosEvent>;
	*ppCosmosEventObj = pObj;
	m_mapEvent[(LONGLONG)pObj] = pObj;
	return S_OK;
}

STDMETHODIMP CCosmos::FireCosmosEventObj(ICosmosEventObj* pCosmosEventObj)
{
	CCosmosEvent* pObj = (CCosmosEvent*)pCosmosEventObj;
	if (pObj)
	{
		FireAppEvent(pObj);
	}
	return S_OK;
}

STDMETHODIMP CCosmos::ObserveGalaxys(LONGLONG hWnd, BSTR bstrGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSave)
{
	LRESULT lRes = ::SendMessage((HWND)hWnd, WM_HUBBLE_DATA, 0, 20190214);
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
	auto it = g_pCosmos->m_mapWindowPage.find((HWND)hWnd);
	if (it != g_pCosmos->m_mapWindowPage.end())
	{
		CString strFrames = OLE2T(bstrGalaxys);
		CString strKey = OLE2T(bstrKey);
		CString strXml = OLE2T(bstrXml);
		if (strFrames == _T(""))
		{
			for (auto it1 : ((CGalaxyCluster*)it->second)->m_mapGalaxy)
			{
				if (it1.second != ((CGalaxyCluster*)it->second)->m_pBKGalaxy)
				{
					IXobj* pXobj = nullptr;
					CXobj* pXobj2 = it1.second->m_pContainerNode;
					if (pXobj2)
					{
						it1.second->Observe(CComBSTR(it1.second->m_strCurrentKey), bstrXml, &pXobj);
					}
					else
					{
						it1.second->Observe(bstrKey, bstrXml, &pXobj);
						if (pWnd && pWnd->m_bMdiForm)
						{
							CXobj* _pXobj = (CXobj*)pXobj;
							CString strXml = _pXobj->m_pXobjShareData->m_pCosmosParse->xml();
							strXml.Replace(_T("/><"), _T("/>\r\n<"));
							strXml.Replace(_T("/>"), _T("></xobj>"));
							CString s = _T("");
							s.Format(_T("<%s>%s</%s>"), it1.second->m_strGalaxyName, strXml, it1.second->m_strGalaxyName);
							_strXml += s;
						}
					}
				}
			}
		}
		else
		{
			strFrames = _T(",") + strFrames;
			for (auto it1 : ((CGalaxyCluster*)it->second)->m_mapGalaxy)
			{
				CString strName = _T(",") + it1.second->m_strGalaxyName + _T(",");
				if (strFrames.Find(strName) != -1)
				{
					IXobj* pXobj = nullptr;
					it1.second->Observe(bstrKey, bstrXml, &pXobj);
				}
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CCosmos::DeleteGalaxy(IGalaxy* pGalaxy)
{
	CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
	if (_pGalaxy)
	{
		HWND hwnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_CHILD, 0, 0, 0, 0, m_hCosmosWnd, NULL, AfxGetInstanceHandle(), NULL);
		_pGalaxy->ModifyHost((LONGLONG)::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_CHILD, 0, 0, 0, 0, (HWND)hwnd, NULL, AfxGetInstanceHandle(), NULL));
		::DestroyWindow(hwnd);
	}
	return S_OK;
}

STDMETHODIMP CCosmos::DeletePage(LONGLONG GalaxyClusterHandle)
{
	m_bDeleteGalaxyCluster = TRUE;
	HWND hPage = (HWND)GalaxyClusterHandle;
	auto it = g_pCosmos->m_mapWindowPage.find(hPage);
	if (it != g_pCosmos->m_mapWindowPage.end())
	{
		CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)it->second;
		auto it2 = pGalaxyCluster->m_mapGalaxy.begin();
		while (it2 != pGalaxyCluster->m_mapGalaxy.end())
		{
			CGalaxy* pGalaxy = it2->second;
			pGalaxyCluster->m_mapGalaxy.erase(it2);
			if (pGalaxy)
			{
				pGalaxy->m_pGalaxyCluster = nullptr;
				RECT rc;
				HWND hwnd = pGalaxy->m_hWnd;
				int nSize = pGalaxy->m_mapXobj.size();
				if (nSize > 1)
				{
					for (auto it : pGalaxy->m_mapXobj)
					{
						if (it.second != pGalaxy->m_pWorkXobj)
						{
							::SetParent(it.second->m_pHostWnd->m_hWnd, pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd);
						}
					}
				}
				if (pGalaxy->m_pWorkXobj)
				{
					::GetWindowRect(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd, &rc);
					pGalaxy->GetParent().ScreenToClient(&rc);
					::DestroyWindow(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd);
					::SetWindowPos(hwnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE);
				}
			}
			it2 = pGalaxyCluster->m_mapGalaxy.begin();
		}
		delete pGalaxyCluster;
	}
	return S_OK;
}

_TCHAR* findProgram(_TCHAR* argv[]) {
	_TCHAR* program;
	/* windows, make sure we are looking for the .exe */
	_TCHAR* ch;
	int length = _tcslen(argv[0]);
	ch = (_TCHAR*)malloc((length + 5) * sizeof(_TCHAR));
	_tcscpy(ch, argv[0]);

	if (length <= 4 || _tcsicmp(&ch[length - 4], _T(".exe")) != 0)
		_tcscat(ch, _T_ECLIPSE(".exe"));

	program = findCommand(ch);
	if (ch != program)
		free(ch);
	if (program == NULL)
	{
		program = (_TCHAR*)malloc(MAX_PATH_LENGTH + 1);
		GetModuleFileName(NULL, program, MAX_PATH_LENGTH);
		argv[0] = program;
	}
	else if (_tcscmp(argv[0], program) != 0) {
		argv[0] = program;
	}
	return program;
}

/*
* Parse arguments of the command.
*/
void parseArgs(int* pArgc, _TCHAR* argv[], int useVMargs)
{
	int     index;

	/* Ensure the list of user argument is NULL terminated. */
	argv[*pArgc] = NULL;

	/* For each user defined argument */
	for (index = 0; index < *pArgc; index++) {
		if (_tcsicmp(argv[index], VMARGS) == 0) {
			if (useVMargs == 1) { //Use the VMargs as the user specified vmArgs
				userVMarg = &argv[index + 1];
			}
			argv[index] = NULL;
			*pArgc = index;
		}
		else if (_tcsicmp(argv[index], NAME) == 0) {
			name = argv[++index];
		}
		else if (_tcsicmp(argv[index], LIBRARY) == 0) {
			eclipseLibrary = argv[++index];
			index++;
		}
		else if (_tcsicmp(argv[index], SUPRESSERRORS) == 0) {
			suppressErrors = 1;
		}
		else if (_tcsicmp(argv[index], PROTECT) == 0) {
			if (_tcsicmp(argv[++index], ROOT) == 0) {
				protectRoot = 1;
			}
		}
	}
}

/* We need to look for --launcher.ini before parsing the other args */
_TCHAR* checkForIni(int argc, _TCHAR* argv[])
{
	int index;
	for (index = 0; index < (argc - 1); index++) {
		if (_tcsicmp(argv[index], INI) == 0) {
			return argv[++index];
		}
	}
	return NULL;
}

/*
* Create a new array containing user arguments from the config file first and
* from the command line second.
* Allocate an array large enough to host all the strings passed in from
* the argument configArgv and argv. That array is passed back to the
* argv argument. That array must be freed with the regular free().
* Note that both arg lists are expected to contain the argument 0 from the C
* main method. That argument contains the path/executable name. It is
* only copied once in the resulting list.
*
* Returns 0 if success.
*/
int createUserArgs(int configArgc, _TCHAR** configArgv, int* argc, _TCHAR*** argv)
{
	_TCHAR** newArray = (_TCHAR**)malloc((configArgc + *argc + 1) * sizeof(_TCHAR*));

	newArray[0] = (*argv)[0];	/* use the original argv[0] */
	memcpy(newArray + 1, configArgv, configArgc * sizeof(_TCHAR*));

	/* Skip the argument zero (program path and name) */
	memcpy(newArray + 1 + configArgc, *argv + 1, (*argc - 1) * sizeof(_TCHAR*));

	/* Null terminate the new list of arguments and return it. */
	*argv = newArray;
	*argc += configArgc;
	(*argv)[*argc] = NULL;

	return 0;
}

/*
* Determine the default official application name
*
* This function provides the default application name that appears in a variety of
* places such as: title of message dialog, title of splash screen window
* that shows up in Windows task bar.
* It is computed from the name of the launcher executable and
* by capitalizing the first letter. e.g. "c:/ide/eclipse.exe" provides
* a default name of "Eclipse".
*/
_TCHAR* getDefaultOfficialName(_TCHAR* program)
{
	_TCHAR* ch = NULL;

	/* Skip the directory part */
	ch = lastDirSeparator(program);
	if (ch == NULL) ch = program;
	else ch++;

	ch = _tcsdup(ch);
#ifdef _WIN32
	{
		/* Search for the extension .exe and cut it */
		_TCHAR* extension = _tcsrchr(ch, _T_ECLIPSE('.'));
		if (extension != NULL)
		{
			*extension = _T_ECLIPSE('\0');
		}
	}
#endif
	/* Upper case the first character */
#ifndef LINUX
	{
		*ch = _totupper(*ch);
	}
#else
	{
		if (*ch >= 'a' && *ch <= 'z')
		{
			*ch -= 32;
		}
	}
#endif
	return ch;
}

void CCosmos::EclipseInit()
{
	if (eclipseLibrary)
		return;
	GetLaunchMode();
	if (launchMode != -1)
	{
		TCHAR	m_szBuffer[MAX_PATH];
		::GetModuleFileName(theApp.m_hInstance, m_szBuffer, MAX_PATH);
		eclipseLibrary = m_szBuffer;
		if (m_pUniverseAppProxy == nullptr)
		{
			LoadCLR();
			m_pUniverseAppProxy = m_pCosmosAppProxy;
		}
		m_pUniverseAppProxy->m_pvoid = nullptr;
		m_bEnableProcessFormTabKey = true;

		setlocale(LC_ALL, "");
		int		nArgs;
		szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		CEclipseExtender* pExtender = new CComObject<CEclipseExtender>;
		g_pCosmos->m_pExtender = pExtender;
		if (m_pUniverseAppProxy == NULL)
		{
			LoadCLR();
			m_pUniverseAppProxy = m_pCosmosAppProxy;
		}
		m_pUniverseAppProxy->m_pvoid = nullptr;
		m_bEnableProcessFormTabKey = true;
		if (m_nAppType != APP_BROWSER_ECLIPSE)
			m_nAppType = APP_ECLIPSE;
		runW(nArgs, szArglist, userVMarg);

		m_bChromeNeedClosed = true;

		for (auto it : g_pCosmos->m_mapBrowserWnd)
		{
			::PostMessage(it.first, WM_CLOSE, 0, 0);
			//it.second->PostMessageW(WM_CLOSE, 0, 0);
		}

		if (::IsWindow(m_hCosmosWnd))
			::DestroyWindow(m_hCosmosWnd);
		if (::IsWindow(m_hMainWnd))
			::DestroyWindow(m_hMainWnd);
	}
}

void CCosmos::InitCosmosDocManager()
{
}

CString CCosmos::ConfigJavaVMInfo(CString stroption)
{
	CString strRet = _T("");
	if (stroption.Find(_T("java.class.path=")) != -1)
	{
		CString strJavaClassPath = m_strAppPath + _T("TangramJavaClassPath.xml");
		if (::PathFileExists(strJavaClassPath))
		{
			int nPos = stroption.Find(_T("="));
			CString s0 = stroption.Left(nPos + 1);
			stroption = stroption.Mid(nPos + 1);
			nPos = stroption.ReverseFind('/');
			if (nPos == -1)
				nPos = stroption.ReverseFind('\\');
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadFile(strJavaClassPath))
			{
				CString s1 = _T("");
				CString str2 = stroption.Left(nPos + 1);
				int nCount = m_Parse.GetCount();
				for (int j = 0; j < nCount; j++)
				{
					CTangramXmlParse* pParse = m_Parse.GetChild(j);
					if (pParse)
					{
						CString strClass = pParse->attr(_T("classinfo"), _T(""));
						CString strLib = pParse->attr(_T("lib"), _T(""));
						if (strLib != _T("") && strClass != _T(""))
						{
							g_pCosmos->m_mapJavaNativeInfo[strLib] = strClass;
						}
						CString strFile = str2 + pParse->text();
						if (::PathFileExists(strFile))
						{
							s1 += strFile;
							s1 += _T(";");
						}
					}
				}
				stroption = s0 + stroption + ";" + s1;
				//stroption += m_strProgramFilePath + _T("Tangram\\tangramjava");
				strRet = stroption;
			}
		}
	}
	return strRet;
}

HICON CCosmos::GetAppIcon(int nIndex)
{
	switch (nIndex)
	{
	case 0:
	{
		if (m_hLargeIcon == nullptr)
		{
			if (m_pCosmosDelegate)
			{
				HICON icon = m_pCosmosDelegate->GetAppIcon(nIndex);
				if (icon)
				{
					m_hLargeIcon = icon;
					return m_hLargeIcon;
				}
			}
			if (g_pCosmos->m_pCLRProxy)
				m_hLargeIcon = g_pCosmos->m_pCLRProxy->GetAppIcon(nIndex);
		}
		return m_hLargeIcon;
	}
	break;
	case 1:
	{
		if (m_hSmallIcon == nullptr)
		{
			if (m_pCosmosDelegate)
			{
				HICON icon = m_pCosmosDelegate->GetAppIcon(nIndex);
				if (icon)
				{
					m_hLargeIcon = icon;
					return m_hLargeIcon;
				}
			}
			if (g_pCosmos->m_pCLRProxy)
				m_hSmallIcon = g_pCosmos->m_pCLRProxy->GetAppIcon(nIndex);
		}
		return m_hSmallIcon;
	}
	break;
	}

	return nullptr;
}

STDMETHODIMP CCosmos::CreateOfficeDocument(BSTR bstrXml)
{
	CComPtr<IWorkBenchWindow> pWorkBenchWindow;
	NewWorkBench(bstrXml, &pWorkBenchWindow);
	return S_OK;
}

STDMETHODIMP CCosmos::NewWorkBench(BSTR bstrCosmosDoc, IWorkBenchWindow** ppWorkBenchWindow)
{
	return S_OK;
}

STDMETHODIMP CCosmos::CreateOutLookObj(BSTR bstrObjType, int nType, BSTR bstrURL, IDispatch** ppRetDisp)
{
	return S_OK;
}

STDMETHODIMP CCosmos::InitEclipseApp()
{
	if (launchMode == -1)
	{
		GetLaunchMode();
		if (launchMode == -1)
			return S_OK;
	}
	if (launchMode != -1 && g_pCosmos->m_pCosmosDelegate->m_pJVMenv == nullptr)
	{
		{
			if (m_strAppPath == _T(""))
				Init();
			CString strplugins = m_strAppPath + _T("plugins\\");
			m_bEclipse = ::PathIsDirectory(strplugins);
		}
		if (m_bEclipse)
		{
			CString strPath = m_strAppPath + _T("plugins\\*.jar");
			//CString strPath = strplugins + _T("*.jar");

			_wfinddata_t fd;
			fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
			intptr_t pf = _wfindfirst(strPath, &fd);
			m_bEclipse = (fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0;
			if (m_bEclipse == false)
			{
				while (!_wfindnext(pf, &fd))
				{
					m_bEclipse = (fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0;
					if (m_bEclipse)
					{
						break;
					}
				}
			}
			_findclose(pf);
		}
		if (m_bEclipse)
		{
			m_strStartJarPath = DEFAULT_EQUINOX_STARTUP;
			//CString _strBridgeJavaClass = _T("org/eclipse/equinox/launcher/JNIBridge");
			//_strBridgeJavaClass.Replace(_T("."), _T("/"));
			//USES_CONVERSION;
			m_strBridgeJavaClass = "org/eclipse/equinox/launcher/JNIBridge";// T2A(_strBridgeJavaClass);
			jarFile = findStartupJar();
			if (jarFile != nullptr)
			{
				//m_bLoadEclipseDelay = true;
				EclipseInit();
				if (::IsWindow(m_hMainWnd) == false)
				{
					if (m_nAppType == APP_ECLIPSE)
						::PostQuitMessage(0);
					else
						m_nAppType = APP_WILLCLOSE;
				}
			}
			else
				m_bEclipse = false;
		}
	}
	return S_OK;
}

STDMETHODIMP CCosmos::InitCLRApp(BSTR strInitXml, LONGLONG* llHandle)
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

__declspec(dllexport) ICosmos* __stdcall  GetCosmos()
{
	return static_cast<ICosmos*>(g_pCosmos);
}

__declspec(dllexport) CCosmosImpl* __stdcall  GetCosmosImpl(ICosmos** pCosmos)
{
	*pCosmos = static_cast<ICosmos*>(g_pCosmos);
	return static_cast<CCosmosImpl*>(g_pCosmos);
}

__declspec(dllexport) void __stdcall  FreeCLR()
{
	if (g_pCosmos && g_pCosmos->m_pClrHost)
	{
		OutputDebugString(_T("------------------Begin Stop CLR------------------------\n"));
		HRESULT hr = g_pCosmos->m_pClrHost->Stop();
		ASSERT(hr == S_OK);
		if (hr == S_OK)
		{
			OutputDebugString(_T("------------------Stop CLR Successed!------------------------\n"));
		}
		DWORD dw = g_pCosmos->m_pClrHost->Release();
		ASSERT(dw == 0);
		if (dw == 0)
		{
			g_pCosmos->m_pClrHost = nullptr;
			OutputDebugString(_T("------------------ClrHost Release Successed!------------------------\n"));
		}
		OutputDebugString(_T("------------------End Stop CLR------------------------\n"));
	}
}

__declspec(dllexport) void __stdcall  SetMainDllLoader(CCosmosMainDllLoader* pLoader)
{
	if (pLoader)
		pLoader->LaunchEx(g_pCosmos);
	if (::IsWindow(g_pCosmos->m_hMainWnd))
	{
		::DestroyWindow(g_pCosmos->m_hMainWnd);
	}
	switch (g_pCosmos->m_nAppType)
	{
	case APP_BROWSER:
		if (g_pCosmos->m_pCosmosDelegate->m_pJVM == nullptr)
			::PostQuitMessage(0);
		break;
	case APP_WILLCLOSE:
		::PostQuitMessage(0);
		break;
	case APP_BROWSERAPP:
		::PostQuitMessage(0);
		break;
	}
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = nullptr;
	if (vm->GetEnv(reinterpret_cast<void**>(&env), g_pCosmos->m_nJVMVersion) != JNI_OK) {
		return -1;
	}
	g_pCosmos->InitJNIForTangram();
	////FULL_CLASS_NAME是个Java类名字，定义了对应java类的全名（要把包名中的点(.)_替换成斜杠(/)）
	//clazz = env->FindClass(FULL_CLASS_NAME);
	//LOGDBG("register method, method count:%d", gsMethodCount);
	////注册JNI函数
	//env->RegisterNatives(clazz, gsNativeMethods,
	//	gsMethodCount);
	////必须返回一个JNI_VERSION_1_1以上（不含）的版本号，否则直接加载失败
	return g_pCosmos->m_nJVMVersion;
}

BOOL CCosmos::InitJNIForTangram()
{
	JNIEnv* _pJVMenv = nullptr;
	if (g_pCosmos->m_pCosmosDelegate->m_pJVMenv)
		_pJVMenv = g_pCosmos->m_pCosmosDelegate->m_pJVMenv;
	else
	{
		JNI_GetCreatedJavaVMs_ GetJavaVM = (JNI_GetCreatedJavaVMs_)::GetProcAddress(::GetModuleHandle(_T("jvm.dll")), "JNI_GetCreatedJavaVMs");
		if (GetJavaVM != nullptr) {
			JavaVM* pVM = nullptr;
			jsize nSize = 1;
			jsize nCount = 0;
			jint nRet = GetJavaVM(&pVM, nSize, &nCount);
			if (nRet == 0) {
				ASSERT(pVM != nullptr);
				if (pVM->GetEnv(reinterpret_cast<void**>(&_pJVMenv), g_pCosmos->m_nJVMVersion) != JNI_OK) {
					return FALSE;
				}
			}
			else {
				TRACE(_T("GetVM Failed\n"));
			}
		}
	}
	if (_pJVMenv) {
		CString strJavaJniPath = m_strAppPath + _T("TangramJniInfo.xml");
		CTangramXmlParse m_Parse;
		if (!(::PathFileExists(strJavaJniPath) && m_Parse.LoadFile(strJavaJniPath))) {
			auto it = g_pCosmos->m_mapValInfo.find(_T("tangramjniinfo"));
			if (it != g_pCosmos->m_mapValInfo.end()) {
				if (m_Parse.LoadXml(it->second.bstrVal) == false)
					return FALSE;
			}
			else
				return FALSE;
		}
		int nCount = m_Parse.GetCount();
		if (nCount == 0)
			return FALSE;
		jclass systemClass = NULL;
		jmethodID loadMethod = NULL;
		systemClass = _pJVMenv->FindClass("java/lang/System");
		try
		{
			if (systemClass != NULL) {
				loadMethod = _pJVMenv->GetStaticMethodID(systemClass, "load", "(Ljava/lang/String;)V");
				if (loadMethod != NULL) {
					for (int j = 0; j < nCount; j++) {
						CTangramXmlParse* pParse = m_Parse.GetChild(j);
						CString strLib = pParse->text();
						bool bExists = true;
						if (::PathFileExists(strLib) == false) {
							CString _strLib = strLib;
							strLib = m_strAppPath + _T("tangramjni\\") + strLib;
							if (::PathFileExists(strLib) == false) {
								strLib = m_strProgramFilePath + _T("tangram\\tangramjni\\") + _strLib;
								if (::PathFileExists(strLib) == false)
									bExists = false;
							}
						}
						if (bExists) {
							wchar_t* _strLib = strLib.GetBuffer();
							jstring jarFileString = newJavaString(_pJVMenv, _strLib);
							strLib.ReleaseBuffer();
							_pJVMenv->CallStaticVoidMethod(systemClass, loadMethod, jarFileString);
							if (jarFileString != NULL)
								_pJVMenv->DeleteLocalRef(jarFileString);
						}
					}
				}
			}
		}
		catch (...)
		{
		}
		if (_pJVMenv->ExceptionOccurred()) {
			_pJVMenv->ExceptionDescribe();
			_pJVMenv->ExceptionClear();
		}
	}
	return FALSE;
}

void CCosmos::ChromeTabCreated(CChromeTab* pTab)
{
	ATLTRACE(_T("Create Chrome Tab :%x\n"), pTab);
}

void CCosmos::OnRenderProcessCreated(CChromeRenderProcess* pProcess)
{

}

CChromeBrowserBase* CCosmos::GetChromeBrowserBase(HWND hHostWnd)
{
	CBrowser* pPWnd = nullptr;
	auto it2 = g_pCosmos->m_mapBrowserWnd.find(hHostWnd);
	if (it2 != g_pCosmos->m_mapBrowserWnd.end())
	{
		pPWnd = (CBrowser*)it2->second;
		return pPWnd->m_pBrowser;
	}
	return nullptr;
}

IBrowser* CCosmos::GetHostBrowser(HWND hNodeWnd)
{
	IXobj* pWndXobj = nullptr;
	HRESULT hr = g_pCosmos->GetXobjFromHandle((LONGLONG)hNodeWnd, &pWndXobj);
	if (hr == S_OK && pWndXobj != nullptr)
	{
		IGalaxy* pGalaxy = nullptr;
		hr = pWndXobj->get_Galaxy(&pGalaxy);
		if (hr == S_OK && pGalaxy != nullptr)
		{
			IBrowser* pBrowser = nullptr;
			hr = pGalaxy->get_HostBrowser(&pBrowser);
			if (hr == S_OK && pBrowser != nullptr)
			{
				return pBrowser;
			}
		}
	}
	return nullptr;
}

void CCosmos::OnDocumentOnLoadCompleted(CChromeRenderFrameHost* pFrameHostBase, HWND hHtmlWnd, void* pVoid)
{
	CChromeRenderFrameHost* pHost = (CChromeRenderFrameHost*)pFrameHostBase;
	HWND hPWnd = ::GetParent(hHtmlWnd);
	auto it = g_pCosmos->m_mapWebView.find(hHtmlWnd);
	if (it != g_pCosmos->m_mapWebView.end())
	{
		((CWebView*)it->second)->m_pChromeRenderFrameHost = pFrameHostBase;
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

void CCosmos::ChromeChildProcessCreated(
	CChromeChildProcessHostImpl*
	pChromeChildProcessHostImpl)
{

}

void CCosmos::OnSubBrowserWndCreated(HWND hParent, HWND hBrowser)
{
	m_hParent = NULL;
	LRESULT lRes = ::SendMessage(::GetParent(hBrowser), WM_HUBBLE_GETNODE, 0, (LPARAM)hBrowser);
}

CString CCosmos::GetProcessPath(const char* _ver, CString process_type)
{
	if (g_pCosmos == nullptr)
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

CString CCosmos::GetSchemeBaseName()
{
	return m_strExeName.MakeLower();
}

char* CCosmos::GetSchemeString(int nType, CString strKey)
{
	switch (nType)
	{
	case 0:
		return "chrome";
	}
	return nullptr;
}

void CCosmos::InserttoDataMap(int nType, CString strKey, void* pData)
{
	switch (nType)
	{
	case 0:
	{
		if (pData)
			m_mapCosmosAppProxy[strKey] = (IUniverseAppProxy*)pData;
		else
		{
			auto it = m_mapCosmosAppProxy.find(strKey);
			if (it != m_mapCosmosAppProxy.end())
			{
				m_mapCosmosAppProxy.erase(it);
			}
		}
	}
	break;
	case 1:
	{
		if (pData)
		{
			m_mapWindowProvider[strKey] = (ICosmosWindowProvider*)pData;
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

void CCosmos::SendIPCMsg(HWND hXobj, CString strMsgID)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->InsertString(_T("msgID"), strMsgID);
			_pObj->m_pWormhole->SendMessage();
		}
	}
}

void CCosmos::InsertMsgData(HWND hXobj, CString strKey, CString strVal)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->InsertString(strKey, strVal);
		}
	}
}

void CCosmos::InsertMsgData(HWND hXobj, CString strKey, __int64 llVal)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->Insertint64(strKey, llVal);
		}
	}
}

void CCosmos::InsertMsgData(HWND hXobj, CString strKey, long lVal)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->InsertLong(strKey, lVal);
		}
	}
}

void CCosmos::InsertMsgData(HWND hXobj, CString strKey, float fVal)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			_pObj->m_pWormhole->InsertFloat(strKey, fVal);
		}
	}
}

CString CCosmos::GetMsgStringData(HWND hXobj, CString strKey)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			return _pObj->m_pWormhole->GetString(strKey);
		}
	}
	return _T("");
}

__int64 CCosmos::GetMsgInt64(HWND hXobj, CString strKey)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			return _pObj->m_pWormhole->Getint64(strKey);
		}
	}
	return 0;
}

long CCosmos::GetMsgLong(HWND hXobj, CString strKey)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			return _pObj->m_pWormhole->GetLong(strKey);
		}
	}
	return 0;
}

float CCosmos::GetMsgFloat(HWND hXobj, CString strKey)
{
	HWND _hWnd = (HWND)hXobj;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		if (_pObj->m_pWormhole)
		{
			return _pObj->m_pWormhole->GetFloat(strKey);
		}
	}
	return 0.0f;
}

IXobj* CCosmos::GetXobj(HWND hWnd)
{
	HWND _hWnd = (HWND)hWnd;
	CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	while (pInfo == nullptr)
	{
		_hWnd = ::GetParent(_hWnd);
		if (_hWnd == 0)
			break;
		pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
	}
	if (pInfo)
	{
		CXobj* _pObj = (CXobj*)pInfo->m_pXobj;
		return (IXobj*)_pObj;
	}
	return nullptr;
}

IGalaxy* CCosmos::GetGalaxy(HWND hWnd)
{
	DWORD dwID = ::GetWindowThreadProcessId(hWnd, NULL);
	CommonThreadInfo* pThreadInfo = GetThreadInfo(dwID);

	CGalaxy* m_pGalaxy = nullptr;
	auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
	if (iter != pThreadInfo->m_mapGalaxy.end())
	{
		m_pGalaxy = (CGalaxy*)iter->second;
		return m_pGalaxy;
	}
	return nullptr;
}

IXobj* CCosmos::ObserveXml(HWND hWnd, CString strKey, CString strXml)
{
	DWORD dwID = ::GetWindowThreadProcessId(hWnd, NULL);
	CommonThreadInfo* pThreadInfo = GetThreadInfo(dwID);

	CGalaxy* m_pGalaxy = nullptr;
	auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
	if (iter != pThreadInfo->m_mapGalaxy.end())
	{
		m_pGalaxy = (CGalaxy*)iter->second;
		IXobj* pXObj = nullptr;
		m_pGalaxy->Observe(CComBSTR(strKey), CComBSTR(strXml), &pXObj);
		return pXObj;
	}
	else
	{
		HWND hPWnd = ::GetParent(hWnd);
		while (hPWnd)
		{
			bool bMDIChild = ::GetWindowLong(hPWnd, GWL_EXSTYLE) & WS_EX_MDICHILD;
			if (bMDIChild)
				break;
			HWND _hPWnd = ::GetParent(hPWnd);
			if (_hPWnd == nullptr)
			{
				break;
			}
			hPWnd = _hPWnd;
		}
		CGalaxyCluster* pGalaxyCluster = nullptr;
		auto it = g_pCosmos->m_mapWindowPage.find(hPWnd);
		if (it != g_pCosmos->m_mapWindowPage.end())
			pGalaxyCluster = (CGalaxyCluster*)it->second;
		else
		{
			pGalaxyCluster = new CComObject<CGalaxyCluster>();
			pGalaxyCluster->m_hWnd = hPWnd;
			g_pCosmos->m_mapWindowPage[hPWnd] = pGalaxyCluster;
		}

		m_pGalaxy = new CComObject<CGalaxy>();
		CString strName = _T("default");
		m_pGalaxy->m_strGalaxyName = strName;
		HWND _hPWnd = ::GetParent(hWnd);
		::GetClassName(_hPWnd, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strClassName = CString(g_pCosmos->m_szBuffer);
		if (strClassName.Find(_T("Afx:ControlBar:")) == 0)
		{
			m_pGalaxy->m_nGalaxyType = CtrlBarGalaxy;
			m_pGalaxy->m_strDocTemplateID = _T("ControlBar");
			CWnd* pWnd = CWnd::FromHandlePermanent(_hPWnd);
			if (pWnd == nullptr)
			{
				CCosmosWnd* _pWnd = new CCosmosWnd();
				_pWnd->SubclassWindow(_hPWnd);
				_pWnd->m_hClient = hWnd;
			}
		}

		m_pGalaxy->m_hHostWnd = hWnd;
		pThreadInfo->m_mapGalaxy[hWnd] = m_pGalaxy;
		pGalaxyCluster->m_mapGalaxy[hWnd] = m_pGalaxy;
		pGalaxyCluster->m_mapWnd[strName] = hWnd;

		for (auto it : g_pCosmos->m_mapCosmosAppProxy)
		{
			CGalaxyProxy* pGalaxyProxy = it.second->OnGalaxyCreated(m_pGalaxy);
			if (pGalaxyProxy)
			{
				m_pGalaxy->m_mapGalaxyProxy[it.second] = pGalaxyProxy;
			}
		}
		IXobj* pXObj = nullptr;
		m_pGalaxy->Observe(CComBSTR(strKey), CComBSTR(strXml), &pXObj);
		return pXObj;
	}
	return nullptr;
}

CWebPageImpl* CCosmos::GetWebPageImpl(HWND hWnd)
{
	CGalaxy* pGalaxy = (CGalaxy*)GetGalaxy(hWnd);
	if (pGalaxy)
	{
		return pGalaxy->m_pWebPageWnd;
	}
	else
	{
		CXobj* pObj = (CXobj*)GetXobj(hWnd);
		if (pObj)
			return pObj->m_pWebPage;
	}
	return nullptr;
}

CBrowserImpl* CCosmos::GetBrowserImpl(HWND hWnd)
{
	CWebPageImpl* pPageImpl = GetWebPageImpl(hWnd);
	if (pPageImpl)
	{
		HWND hBrowser = pPageImpl->m_pChromeRenderFrameHost->GetHostBrowserWnd();
		auto it = m_mapBrowserWnd.find(hBrowser);
		if (it != m_mapBrowserWnd.end())
			return (CBrowser*)it->second;
	}
	return nullptr;
}

bool CCosmos::SetFrameInfo(HWND hWnd, HWND hFrame, CString strTemplateID, void* pDoc, void* pDocTemplate)
{
	CosmosFrameWndInfo* pCosmosFrameWndInfo = (CosmosFrameWndInfo*)::GetProp(hFrame, _T("CosmosFrameWndInfo"));
	if (pCosmosFrameWndInfo == nullptr)
	{
		pCosmosFrameWndInfo = new CosmosFrameWndInfo();
		pCosmosFrameWndInfo->m_pDoc = pDoc;
		pCosmosFrameWndInfo->m_pDocTemplate = pDocTemplate;
		pCosmosFrameWndInfo->m_hClient = hWnd;
		g_pCosmos->m_mapCosmosFrameWndInfo[hFrame] = pCosmosFrameWndInfo;
		::SetProp(hFrame, _T("CosmosFrameWndInfo"), pCosmosFrameWndInfo);
	}
	auto it = m_mapDocDefaultName.find(strTemplateID);
	if (it != m_mapDocDefaultName.end())
	{
		g_pCosmosImpl->m_pUniverseAppProxy->SetFrameCaption(hWnd, it->second);
	}
	return false;
}

CTabStatsTrackerDelegate* CCosmos::SetTabStatsTrackerDelegate()
{
	return this;
}


void CCosmos::HeartbeatEvent()
{
	//int num_cpus = 0;
	//SYSTEM_INFO system_info;
	//GetSystemInfo(&system_info);
	//num_cpus = system_info.dwNumberOfProcessors;
	//std::vector<SYSTEM_POWER_INFORMATION> info(num_cpus);
	//if (CallNtPowerInformation(SystemPowerInformation,
	//	nullptr,
	//	0,
	//	&info[0],
	//	sizeof(SYSTEM_POWER_INFORMATION) * num_cpus) !=
	//	0) 
	//{
	//	UCHAR nVal = info[0].CoolingMode;
	//	switch (nVal)
	//	{
	//	case 0:
	//		break;
	//	case 1:
	//	{
	//	}
	//		break;
	//	default:
	//		break;
	//	}
	//}
#ifdef _DEBUG
	SYSTEMTIME st;
	GetLocalTime(&st);
	TRACE(_T("\n******HeartbeatEvent : %02d:%02d:%02d OnHeartbeatEvent was signaled. ******\n"), st.wHour, st.wMinute, st.wSecond);
#endif	
	::PostAppMessage(m_dwThreadID, WM_POWERBROADCAST, PBT_APMRESUMEAUTOMATIC, 0);
}

void CCosmos::Resume()
{

}

void CCosmos::BrowserAdded(CChromeBrowserBase* browser, HWND hBrowser)
{
	CBrowser* pBrowserWnd = new CComObject<CBrowser>();
	pBrowserWnd->SubclassWindow(hBrowser);
	g_pCosmos->m_mapBrowserWnd[hBrowser] = pBrowserWnd;
	pBrowserWnd->m_pBrowser = browser;
	if (pBrowserWnd->m_pBrowser)
		pBrowserWnd->m_pBrowser->m_pProxy = pBrowserWnd;
}

void CCosmos::OnTabChangedAt(HWND hWebView, HWND hBrowser, int nIndex, BrowserTabChangeType type, void* content)
{
	auto it = m_mapBrowserWnd.find(hBrowser);
	if (it != m_mapBrowserWnd.end())
	{
		CBrowser* pBrowser = (CBrowser*)it->second;
		pBrowser->m_bSZMode = true;
		switch (type)
		{
		case BrowserTabChangeType::LoadingOnly:
		{
		}
		break;
		case BrowserTabChangeType::All:
		{
			::PostMessage(hBrowser, WM_BROWSERLAYOUT, 1, 7);
		}
		break;
		}
	}
}

void CCosmos::InitialOrInsertedTab(HWND hWebView, HWND hBrowser)
{
	CBrowser* pBrowser = nullptr;
	auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
	if (it != g_pCosmos->m_mapBrowserWnd.end())
	{
		pBrowser = (CBrowser*)it->second;
		auto it2 = g_pCosmos->m_mapWebView.find(hWebView);
		if (it2 == g_pCosmos->m_mapWebView.end())
		{
			g_pCosmos->m_pHtmlWndCreated = new CComObject<CWebView>;
			g_pCosmos->m_pHtmlWndCreated->SubclassWindow(hWebView);
			if (g_pCosmos->m_pCLRProxy)
				g_pCosmos->m_pCLRProxy->OnWebPageCreated(hWebView, (CWebPageImpl*)g_pCosmos->m_pHtmlWndCreated, (IWebPage*)g_pCosmos->m_pHtmlWndCreated, 0);
			g_pCosmos->m_pHtmlWndCreated->m_bDevToolWnd = false;
			g_pCosmos->m_mapWebView[hWebView] = g_pCosmos->m_pHtmlWndCreated;
			//if (g_pCosmos->m_mapWebView.size() > 1)
			//	g_pCosmos->m_pHtmlWndCreated->m_bCanShow = true;
			if (pBrowser->m_pBrowser && hWebView == pBrowser->m_pBrowser->GetActiveWebContentWnd())
				pBrowser->m_pVisibleWebView = g_pCosmos->m_pHtmlWndCreated;
			g_pCosmos->m_pActiveHtmlWnd = pBrowser->m_pVisibleWebView;
			g_pCosmos->m_pGalaxy = nullptr;
			g_pCosmos->m_bWinFormActived = false;
			pBrowser->m_mapChildPage[hWebView] = g_pCosmos->m_pHtmlWndCreated;
			::PostMessage(hWebView, WM_COSMOSMSG, 20190331, 1);
		}
	}
}

void CCosmos::WebContentsDestroyed(HWND hWebView, HWND hBrowser)
{

}

void CCosmos::BrowserRemoved(CChromeBrowserBase* browser, HWND hBrowser)
{
	if (browser)
	{
	}
}

//NetworkChangeNotify:
void CCosmos::OnNetworkChangeNotify(int ConnectType, __int64 NetworkHandle)
{

}
