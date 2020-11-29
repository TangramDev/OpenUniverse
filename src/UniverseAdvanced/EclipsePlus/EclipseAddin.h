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
* OUTLINED IN THE MIT LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
********************************************************************************/


#pragma once
#include <jni.h>
#include "../Hubble.h"
#include "../HubbleCtrl.h"
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
	CGrid*							m_pCurGrid;
	CGrid*							m_pHostGrid;
	CGalaxy*						m_pGalaxy;
	map<HWND, CEclipseCtrl*>		m_mapCtrl;
	CHubbleDoc*						m_pDoc;
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
		MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_QUERYAPPPROXY, OnQueryAppProxy)
		MESSAGE_HANDLER(WM_HUBBLE_GETXML, OnHubbleGetXml)
	END_MSG_MAP()
protected:
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease() { return 1; }

	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnQueryAppProxy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnHubbleMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnHubbleGetXml(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );

	void OnFinalMessage(HWND hWnd);

	STDMETHOD(get_Count)(long* pCount);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	STDMETHOD(get_Handle)(LONGLONG* pVal);
	STDMETHOD(get_HubbleCtrl)(LONGLONG hWnd, IEclipseCtrl** pVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_Galaxy)(IGalaxy** pVal);
	STDMETHOD(Active)();
	STDMETHOD(CloseTangramUI)();
	STDMETHOD(ObserveEx)(BSTR bstrKey, BSTR bstrXml, IGrid** ppGrid);
	STDMETHOD(ObserveInView)(int nIndex, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);

public:
	STDMETHOD(get_Ctrl)(VARIANT vIndex, IEclipseCtrl **ppCtrl);
	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IGrid** ppGrid);
	void CreatePage(BOOL bSaveToConfigFile);
};

// CEclipseCtrl
class ATL_NO_VTABLE CEclipseCtrl :
	public CHubbleCtrlBase,
	public IOleObjectImpl<CEclipseCtrl>,
	public IPersistStorageImpl<CEclipseCtrl>,
	public IPersistStreamInitImpl<CEclipseCtrl>,
	public IConnectionPointContainerImpl<CEclipseCtrl>,
	public CComCoClass<CEclipseCtrl, &CLSID_HubbleCtrl>,
	public IConnectionPointImpl<CEclipseCtrl, &DIID__IHubbleObjEvents>,
	public IDispatchImpl<IEclipseCtrl, &IID_IEclipseCtrl, &LIBID_Universe, 1, 0>
{
public:
	HWND						m_hEclipseViewWnd;
	VARIANT						m_varTag;
	CGrid*						m_pCurGrid;
	map<CString, CString>		m_mapHubbleInfo;
	map<CString, HWND>			m_mapHubbleHandle;
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
		CONNECTION_POINT_ENTRY(DIID__IHubbleObjEvents)
	END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct();
	void OnFinalMessage(HWND hWnd);

public:
	BEGIN_MSG_MAP(CEclipseCtrl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
	END_MSG_MAP()

	HRESULT Fire_GalaxyClusterLoaded(IDispatch* sender, BSTR url);
	HRESULT Fire_NodeCreated(IGrid * pGridCreated);
	HRESULT Fire_AddInCreated(IGrid * pRootGrid, IDispatch * pAddIn, BSTR bstrID, BSTR bstrAddInXml);
	HRESULT Fire_BeforeOpenXml(BSTR bstrXml, LONGLONG hWnd);
	HRESULT Fire_OpenXmlComplete(BSTR bstrXml, LONGLONG hWnd, IGrid * pRetRootNode);
	HRESULT Fire_Destroy();
	HRESULT Fire_NodeMouseActivate(IGrid * pActiveNode);
	HRESULT Fire_ClrControlCreated(IGrid * Node, IDispatch * Ctrl, BSTR CtrlName, LONGLONG CtrlHandle);
	HRESULT Fire_TabChange(IGrid* sender, LONG ActivePage, LONG OldPage);
	HRESULT Fire_Event(IGrid* sender, IDispatch* EventArg);
	HRESULT Fire_ControlNotify(IGrid * sender, LONG NotifyCode, LONG CtrlID, LONGLONG CtrlHandle, BSTR CtrlClassName);
	HRESULT Fire_HubbleEvent(IHubbleEventObj* pEventObj);

	// IEclipseCtrl
	STDMETHOD(get_tag)(VARIANT* pVal);
	STDMETHOD(put_tag)(VARIANT newVal);
	STDMETHOD(get_HWND)(LONGLONG* pVal);
	STDMETHOD(put_Handle)(BSTR bstrHandleName, LONGLONG newVal);
	STDMETHOD(get_Hubble)(IHubble** pVal);
	STDMETHOD(get_EclipseViewHandle)(LONGLONG* pVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_WorkBenchWindow)(IWorkBenchWindow** pVal);
	STDMETHOD(get_TopGalaxy)(IGalaxy** pVal);
	STDMETHOD(get_TopGalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_ActiveTopGrid)(IGrid** pVal);
	STDMETHOD(get_AppKeyValue)(BSTR bstrKey, VARIANT* pVal);
	STDMETHOD(put_AppKeyValue)(BSTR bstrKey, VARIANT newVal);

	STDMETHOD(Observe)(BSTR bstrGalaxyName, BSTR bstrKey, BSTR bstrXml, IGrid** ppGrid);
	STDMETHOD(ObserveEx)(BSTR bstrGalaxyName, BSTR bstrKey, BSTR bstrXml, IGrid** ppGrid);
	STDMETHOD(InitCtrl)(BSTR bstrXml);

private:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnHubbleMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
};

