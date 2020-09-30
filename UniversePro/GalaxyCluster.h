/********************************************************************************
*					Tangram Library - version 10.0.0							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE TANGRAM LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
*
********************************************************************************/

// GalaxyCluster.h : CGalaxyCluster implementation file

#pragma once

#include <assert.h>
#include <wininet.h>

#include "EclipsePlus\EclipseAddin.h"

// CGalaxyCluster
class ATL_NO_VTABLE CGalaxyCluster : 
	public CComObjectRootBase,
	public IConnectionPointContainerImpl <CGalaxyCluster>,
	public IConnectionPointImpl<CGalaxyCluster, &__uuidof(_IHubbleObjEvents)>,
	public IDispatchImpl<IGalaxyCluster, &IID_IGalaxyCluster, &LIBID_Universe, 1, 0>
{
	friend CHubble;
public:
	CGalaxyCluster();
	virtual ~CGalaxyCluster();

	BOOL										m_bDoc;
	BOOL										m_bPageDataLoaded;
	HWND										m_hWnd;

	CString										m_strPageFilePath;
	CString										m_strPageFileName;
	CString										m_strConfigFileNodeName;
	CString										m_strXmlHeader;
	CString										m_strXmlBottom;

	CGalaxy*									m_pBKGalaxy;
	CHubbleDoc*									m_pActiveDoc;
	IUniverseAppProxy*							m_pUniverseAppProxy;
	CHubbleDocTemplate*							m_pHubbleDocTemplate;
	map<CString, CString>						m_strMapKey;
	map<HWND, CGalaxy*>							m_mapCtrlBarGalaxy;
	map<CString, HWND>							m_mapWnd;
	map<HWND, CGalaxy*>							m_mapGalaxy;
	map<HWND, CGalaxy*>							m_mapNeedSaveGalaxy;
	map<CString, CGrid*>						m_mapGrid;
	map<CString, IDispatch*>					m_mapExternalObj;
	map<HWND, CEclipseCtrl*>					m_mapNotifyCtrl;
	map<CString, CString>						m_mapXtml;
	map<__int64, CCosmosEvent*>					m_mapEventObj;
	map<IUniverseAppProxy*, CGalaxyClusterProxy*> m_mapGalaxyClusterProxy;

	BEGIN_COM_MAP(CGalaxyCluster)
		COM_INTERFACE_ENTRY(IGalaxyCluster)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CGalaxyCluster)
		CONNECTION_POINT_ENTRY(__uuidof(_IHubbleObjEvents))
	END_CONNECTION_POINT_MAP()

	void Lock(){}
	void Unlock(){}
	void BeforeDestory();
	void UpdateMapKey(CString);
	HRESULT Fire_GalaxyClusterLoaded(IDispatch* sender, BSTR url);
	HRESULT Fire_NodeCreated(IGrid * pGridCreated);
	HRESULT Fire_AddInCreated(IGrid * pRootGrid, IDispatch * pAddIn, BSTR bstrID, BSTR bstrAddInXml);
	HRESULT Fire_BeforeOpenXml(BSTR bstrXml, LONGLONG hWnd);
	HRESULT Fire_OpenXmlComplete(BSTR bstrXml, LONGLONG hWnd, IGrid * pRetRootNode);
	HRESULT Fire_Destroy();
	HRESULT Fire_NodeMouseActivate(IGrid * pActiveNode);
	HRESULT Fire_ClrControlCreated(IGrid * Node, IDispatch * Ctrl, BSTR CtrlName, LONGLONG CtrlHandle);
	HRESULT Fire_TabChange(IGrid* sender, LONG ActivePage, LONG OldPage);
	HRESULT Fire_HubbleEvent(IHubbleEventObj* pEventObj);
	HRESULT Fire_IPCMsg(IGalaxy* pSender, BSTR bstrType, BSTR bstrContent, BSTR bstrFeature);

	//void OnNodeDocComplete(WPARAM);

protected:
	ULONG InternalAddRef(){ return 1; }
	ULONG InternalRelease(){ return 1; }

public:
	STDMETHOD(get_Count)(long* pCount);
	STDMETHOD(get_Galaxy)(VARIANT vIndex, IGalaxy ** ppGalaxy);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	STDMETHOD(get_Handle)(LONGLONG* pVal);
	STDMETHOD(get_Extender)(BSTR bstrExtenderName, IDispatch** pVal);
	STDMETHOD(put_Extender)(BSTR bstrExtenderName, IDispatch* newVal);
	STDMETHOD(get_GalaxyName)(LONGLONG hHwnd, BSTR* pVal);
	STDMETHOD(get_Grid)(BSTR bstrNodeName, IGrid** pVal);
	STDMETHOD(get_GridNames)(BSTR* pVal);
	STDMETHOD(get_XObject)(BSTR bstrName, IDispatch** pVal);
	STDMETHOD(get_Parent)(IGalaxyCluster** pVal);
	STDMETHOD(get_Width)(long* pVal);
	STDMETHOD(put_Width)(long newVal);
	STDMETHOD(get_Height)(long* pVal);
	STDMETHOD(put_Height)(long newVal);
	STDMETHOD(get_xtml)(BSTR strKey, BSTR* pVal);
	STDMETHOD(put_xtml)(BSTR strKey, BSTR newVal);
	STDMETHOD(get_GalaxyClusterXML)(BSTR* pVal);
	STDMETHOD(put_ConfigName)(BSTR newVal);

	STDMETHOD(CreateGalaxy)(VARIANT ParentObj, VARIANT HostWnd, BSTR bstrGalaxyName, IGalaxy** pRetFrame);
	STDMETHOD(GetGrid)(BSTR bstrGalaxyName, BSTR bstrNodeName, IGrid** pRetNode);
	STDMETHOD(GetGalaxyFromCtrl)(IDispatch* ctrl, IGalaxy** ppGalaxy);
	STDMETHOD(GetCtrlInGrid)(BSTR NodeName, BSTR CtrlName, IDispatch** ppCtrl);
	STDMETHOD(Observe)(IDispatch* Parent, BSTR CtrlName, BSTR GalaxyName, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);
	STDMETHOD(ObserveCtrl)(VARIANT MdiForm, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);
	STDMETHOD(ConnectHubbleCtrl)(IHubbleCtrl* eventSource);
	STDMETHOD(CreateGalaxyWithDefaultNode)(ULONGLONG hFrameWnd, BSTR bstrGalaxyName, BSTR bstrDefaultNodeKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfig, IGrid** ppGrid);
	STDMETHOD(ObserveGalaxys)(BSTR bstrFrames, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfigFile);
	STDMETHOD(get_CurrentDesignGalaxyType)(GalaxyType* pVal);
	STDMETHOD(get_CurrentDesignNode)(IGrid** pVal);

private:
	bool										m_bIsBlank;
	bool										m_bIsDestory;
	bool										m_bDocComplete;
};

