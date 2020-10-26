/********************************************************************************
*					Open Universe - version 1.0.1.10							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
********************************************************************************/

// dllmain.h : Declaration of module class.

#include "HubbleEvents.h"
#include <cstring>
#include <iostream>

class CGridCLREvent;

class CCosmos :
	public IHubbleDelegate,
	public IUniverseAppProxy
{
public:
	CCosmos();
	virtual ~CCosmos();

	bool									m_bHostApp;
	bool									m_bBrowserModeInit = false;
	DWORD									m_dwThreadID;
	HWND									m_hHostWnd;
	IHubble*								m_pHubble;

	CString									m_strAppPath;

	CString GetLibPathFromAssemblyQualifiedName(CString strAssemblyQualifiedName);

	void InitHubbleApp(bool bCrashReporting);
private:
	void ProcessMsg(MSG* msg);
	//CCosmosCoreEvents:
	void OnHubbleClose();
	void OnObserverComplete(HWND hWnd, CString strUrl, IGrid* pRootGrid);
	void HubbleInit() {};
	bool OnUniversePreTranslateMessage(MSG* pMsg);
	CString _GetLibPathFromAssemblyQualifiedName(CString strDir, CString strLibName);
};

extern CCosmos theApp;

class CCosmosGridEvent : public CGridEvents
{
public:
	CCosmosGridEvent();
	virtual ~CCosmosGridEvent();

	CGridCLREvent* m_pGridCLREvent;
private:
	void __stdcall  OnObserverComplete();
	void __stdcall  OnDestroy();
	void __stdcall  OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl);
	void __stdcall  OnGridAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml);
	void __stdcall  OnTabChange(int nActivePage, int nOldPage);
	void __stdcall  OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra);
};
