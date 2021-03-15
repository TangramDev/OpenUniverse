/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103150050
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
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
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#pragma once
#include "../Cosmos.h"
#include "../CosmosCtrl.h"
#include "tangrambase.h"

class CEclipseWnd;
class CEclipseCtrl;

class ATL_NO_VTABLE CEclipseExtender :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IEclipseExtender, &IID_IEclipseExtender, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CEclipseExtender(void);
	virtual ~CEclipseExtender(void);

	BEGIN_COM_MAP(CEclipseExtender)
		COM_INTERFACE_ENTRY(IEclipseExtender)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	STDMETHOD(Close)();
	STDMETHOD(get_ActiveWorkBenchWindow)(BSTR bstrID, IWorkBenchWindow** pVal);
};

class ATL_NO_VTABLE CEclipseWnd :
	public CWindowImpl<CEclipseWnd, CWindow>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IWorkBenchWindow, &IID_IWorkBenchWindow, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CEclipseWnd(void);
	virtual ~CEclipseWnd(void);
	BOOL							m_bInit;
	int								m_nNewWinCmdID;
	HWND							m_hClient;
	CString							m_strAppProxyID;
	CString							m_strDocKey;
	CString							m_strXml;
	CString							m_strPath;
	CString							m_strFrameID;
	CString							m_strNodeName;
	CGalaxyCluster*					m_pGalaxyCluster;
	CXobj*							m_pCurXobj;
	CXobj*							m_pHostXobj;
	CGalaxy*						m_pGalaxy;
	map<HWND, CEclipseCtrl*>		m_mapCtrl;
	IUniverseAppProxy*				m_pAppProxy;
	void Show(CString strID);
	BEGIN_COM_MAP(CEclipseWnd)
		COM_INTERFACE_ENTRY(IWorkBenchWindow)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	BEGIN_MSG_MAP(CEclipseWnd)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_QUERYAPPPROXY, OnQueryAppProxy)
		MESSAGE_HANDLER(WM_HUBBLE_GETXML, OnCosmosGetXml)
	END_MSG_MAP()
protected:
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease() { return 1; }

	LRESULT OnCommand(UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnQueryAppProxy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnActivate(UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnCosmosMsg(UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnCosmosGetXml(UINT, WPARAM, LPARAM, BOOL& );

	void OnFinalMessage(HWND hWnd);

	STDMETHOD(get_Count)(long* pCount);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	STDMETHOD(get_Handle)(LONGLONG* pVal);
	STDMETHOD(get_CosmosCtrl)(LONGLONG hWnd, IEclipseCtrl** pVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_Galaxy)(IGalaxy** pVal);
	STDMETHOD(Active)();
	STDMETHOD(CloseTangramUI)();
	STDMETHOD(ObserveEx)(BSTR bstrKey, BSTR bstrXml, IXobj** ppXobj);
	STDMETHOD(ObserveInView)(int nIndex, BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj);

public:
	STDMETHOD(get_Ctrl)(VARIANT vIndex, IEclipseCtrl **ppCtrl);
	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IXobj** ppXobj);
	void CreatePage(BOOL bSaveToConfigFile);
};

// CEclipseCtrl
class ATL_NO_VTABLE CEclipseCtrl :
	public CCosmosCtrlBase,
	public IOleObjectImpl<CEclipseCtrl>,
	public IPersistStorageImpl<CEclipseCtrl>,
	public IPersistStreamInitImpl<CEclipseCtrl>,
	public IConnectionPointContainerImpl<CEclipseCtrl>,
	public CComCoClass<CEclipseCtrl, &CLSID_CosmosCtrl>,
	public IConnectionPointImpl<CEclipseCtrl, &DIID__ICosmosObjEvents>,
	public IDispatchImpl<IEclipseCtrl, &IID_IEclipseCtrl, &LIBID_Universe, 1, 0>
{
public:
	HWND						m_hEclipseViewWnd;
	VARIANT						m_varTag;
	CXobj*						m_pCurXobj;
	map<CString, CString>		m_mapCosmosInfo;
	map<CString, HWND>			m_mapCosmosHandle;
	map<CString, CGalaxy*>		m_mapGalaxy;

	CEclipseWnd*				m_pEclipseWnd;
	CGalaxyClusterProxy*		m_pGalaxyClusterProxy;

	CEclipseCtrl();

	BEGIN_COM_MAP(CEclipseCtrl)
		COM_INTERFACE_ENTRY(IEclipseCtrl)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IOleObject)
		COM_INTERFACE_ENTRY(IViewObject)
		COM_INTERFACE_ENTRY(IViewObject2)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY(IPersistStorage)
		COM_INTERFACE_ENTRY(IPersistStreamInit)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CEclipseCtrl)
		CONNECTION_POINT_ENTRY(DIID__ICosmosObjEvents)
	END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct();
	void OnFinalMessage(HWND hWnd);

public:
	BEGIN_MSG_MAP(CEclipseCtrl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
	END_MSG_MAP()

	HRESULT Fire_GalaxyClusterLoaded(IDispatch* sender, BSTR url);
	HRESULT Fire_NodeCreated(IXobj * pXobjCreated);
	HRESULT Fire_AddInCreated(IXobj * pRootXobj, IDispatch * pAddIn, BSTR bstrID, BSTR bstrAddInXml);
	HRESULT Fire_BeforeOpenXml(BSTR bstrXml, LONGLONG hWnd);
	HRESULT Fire_OpenXmlComplete(BSTR bstrXml, LONGLONG hWnd, IXobj * pRetRootNode);
	HRESULT Fire_Destroy();
	HRESULT Fire_NodeMouseActivate(IXobj * pActiveNode);
	HRESULT Fire_ClrControlCreated(IXobj * Node, IDispatch * Ctrl, BSTR CtrlName, LONGLONG CtrlHandle);
	HRESULT Fire_TabChange(IXobj* sender, LONG ActivePage, LONG OldPage);
	HRESULT Fire_Event(IXobj* sender, IDispatch* EventArg);
	HRESULT Fire_ControlNotify(IXobj * sender, LONG NotifyCode, LONG CtrlID, LONGLONG CtrlHandle, BSTR CtrlClassName);
	HRESULT Fire_CosmosEvent(ICosmosEventObj* pEventObj);

	// IEclipseCtrl
	STDMETHOD(get_tag)(VARIANT* pVal);
	STDMETHOD(put_tag)(VARIANT newVal);
	STDMETHOD(get_HWND)(LONGLONG* pVal);
	STDMETHOD(put_Handle)(BSTR bstrHandleName, LONGLONG newVal);
	STDMETHOD(get_Cosmos)(ICosmos** pVal);
	STDMETHOD(get_EclipseViewHandle)(LONGLONG* pVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_WorkBenchWindow)(IWorkBenchWindow** pVal);
	STDMETHOD(get_TopGalaxy)(IGalaxy** pVal);
	STDMETHOD(get_TopGalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_ActiveTopXobj)(IXobj** pVal);
	STDMETHOD(get_AppKeyValue)(BSTR bstrKey, VARIANT* pVal);
	STDMETHOD(put_AppKeyValue)(BSTR bstrKey, VARIANT newVal);

	STDMETHOD(Observe)(BSTR bstrGalaxyName, BSTR bstrKey, BSTR bstrXml, IXobj** ppXobj);
	STDMETHOD(ObserveEx)(BSTR bstrGalaxyName, BSTR bstrKey, BSTR bstrXml, IXobj** ppXobj);
	STDMETHOD(InitCtrl)(BSTR bstrXml);

private:
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnCosmosMsg(UINT, WPARAM, LPARAM, BOOL& );
};

