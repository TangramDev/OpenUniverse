/********************************************************************************
*					Open Universe - version 1.1.1.21							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
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
	bool									m_bVSToolBoxConnected = false;
	bool									m_bBrowserModeInit = false;
	DWORD									m_dwThreadID;
	HWND									m_hHostWnd;
	IHubble*								m_pHubble;
	IHubble*								m_pHubbleVS = nullptr;
	//IVSExtender*							m_pVSExtender;

	CString									m_strAppPath;

	int CalculateByteMD5(BYTE* pBuffer, int BufferSize, CString &MD5);
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
	void ExitJVM();
private:
	//CHubbleEvents:
	void OnHubbleClose();
	void OnObserverComplete(HWND hWnd, CString strUrl, IGrid* pRootGrid);
	HWND InitHubbleApp();
	CString _GetLibPathFromAssemblyQualifiedName(CString strDir, CString strLibName);
	virtual bool EclipseAppInit();
	virtual void IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2);
	virtual void CustomizedDOMElement(HWND hWnd, CString strRuleName, CString strHTML);
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
