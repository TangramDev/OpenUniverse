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
	virtual void ProcessMsg(MSG* msg);
	virtual void ForegroundIdleProc() {};
	virtual bool DoIdleWork();
	virtual bool GetClientAreaBounds(HWND hWnd, RECT& rc) { return false; };
	virtual bool HookAppDocTemplateInfo() { return false; };
	virtual BOOL OnAppIdle(BOOL& bIdle, LONG lCount) {
		return false;
	};
	virtual HWND QueryCanClose(HWND hWnd) { return NULL; };

	virtual BOOL IsAppIdleMessage() {
		return false;
	};

	virtual HWND GetMainWnd()
	{
		return NULL;
	};

	virtual void CosmosInit() {};
	virtual CString GetNTPXml() { return _T(""); };
	virtual bool OnUniversePreTranslateMessage(MSG* pMsg);

	void InitCosmosApp(bool bCrashReporting);
private:
	//CCosmosCoreEvents:
	void OnCosmosClose();
	void OnObserverComplete(HWND hWnd, CString strUrl, IXobj* pRootXobj);
	HWND InitCosmosApp();
	CString _GetLibPathFromAssemblyQualifiedName(CString strDir, CString strLibName);
	virtual bool EclipseAppInit() { return false; };
	virtual void IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2);
	virtual void CustomizedDOMElement(HWND hWnd, CString strRuleName, CString strHTML);
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
