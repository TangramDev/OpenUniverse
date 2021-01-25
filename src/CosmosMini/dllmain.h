/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101240017
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

// dllmain.h : Declaration of module class.

#include "CosmosEvents.h"
#include <cstring>
#include <iostream>

class CXobjCLREvent;

class CCosmos :
	public ICosmosDelegate,
	public IUniverseAppProxy
{
public:
	CCosmos();
	virtual ~CCosmos();

	bool									m_bHostApp;
	bool									m_bBrowserModeInit = false;
	DWORD									m_dwThreadID;
	HWND									m_hHostWnd;
	ICosmos*								m_pCosmos;

	CString									m_strAppPath;

	CString GetLibPathFromAssemblyQualifiedName(CString strAssemblyQualifiedName);

	void InitCosmosApp(bool bCrashReporting);
private:
	void ProcessMsg(MSG* msg);
	//CCosmosCoreEvents:
	void OnCosmosClose();
	void OnObserverComplete(HWND hWnd, CString strUrl, IXobj* pRootXobj);
	void CosmosInit() {};
	bool OnUniversePreTranslateMessage(MSG* pMsg);
	CString _GetLibPathFromAssemblyQualifiedName(CString strDir, CString strLibName);
};

extern CCosmos theApp;

class CCosmosXobjEvent : public CXobjEvents
{
public:
	CCosmosXobjEvent();
	virtual ~CCosmosXobjEvent();

	CXobjCLREvent* m_pXobjCLREvent;
private:
	void __stdcall  OnObserverComplete();
	void __stdcall  OnDestroy();
	void __stdcall  OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl);
	void __stdcall  OnXobjAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml);
	void __stdcall  OnTabChange(int nActivePage, int nOldPage);
	void __stdcall  OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra);
};
