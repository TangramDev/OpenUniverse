/********************************************************************************
*					Open Universe - version 0.9.9								*
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

// dllmain.h : Declaration of module class.

#include "TangramCoreEvents.h"
#include <cstring>
#include <iostream>

class CGridCLREvent;

class CCosmos :
	public IHubbleDelegate,
	public IHubbleAppProxy
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

	virtual void HubbleInit() {};
	virtual CString GetNTPXml() { return _T(""); };
	virtual bool OnUniversePreTranslateMessage(MSG* pMsg);

	void InitHubbleApp(bool bCrashReporting);
private:
	//CCosmosCoreEvents:
	void OnHubbleClose();
	void OnOpenComplete(HWND hWnd, CString strUrl, IGrid* pRootGrid);
	HWND InitHubbleApp();
	CString _GetLibPathFromAssemblyQualifiedName(CString strDir, CString strLibName);
	virtual bool EclipseAppInit() { return false; };
	virtual void IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2);
	virtual void RenderHTMLElement(HWND hWnd, CString strRuleName, CString strHTML);
};

extern CCosmos theApp;

class CCosmosGridEvent : public CGridEvents
{
public:
	CCosmosGridEvent();
	virtual ~CCosmosGridEvent();

	CGridCLREvent* m_pGridCLREvent;
private:
	void __stdcall  OnOpenComplete();
	void __stdcall  OnDestroy();
	void __stdcall  OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl);
	void __stdcall  OnGridAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml);
	void __stdcall  OnTabChange(int nActivePage, int nOldPage);
	void __stdcall  OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra);
};
