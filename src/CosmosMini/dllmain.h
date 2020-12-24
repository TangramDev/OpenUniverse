/********************************************************************************
 *					DOM Plus for Application - Version 1.1.7.40
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
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
