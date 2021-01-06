/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101060005
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

// GalaxyCluster.h : CGalaxyCluster implementation file

#pragma once

#include <assert.h>
#include <wininet.h>

// CGalaxyCluster
class ATL_NO_VTABLE CGalaxyCluster : 
	public CComObjectRootBase,
	public IConnectionPointContainerImpl <CGalaxyCluster>,
	public IConnectionPointImpl<CGalaxyCluster, &__uuidof(_ICosmosObjEvents)>,
	public IDispatchImpl<IGalaxyCluster, &IID_IGalaxyCluster, &LIBID_Universe, 1, 0>
{
	friend CCosmos;
public:
	CGalaxyCluster();
	virtual ~CGalaxyCluster();

	HWND										m_hWnd;

	IUniverseAppProxy*							m_pUniverseAppProxy;
	map<CString, HWND>							m_mapWnd;
	map<HWND, CGalaxy*>							m_mapGalaxy;
	map<CString, CXobj*>						m_mapXobj;

	BEGIN_COM_MAP(CGalaxyCluster)
		COM_INTERFACE_ENTRY(IGalaxyCluster)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CGalaxyCluster)
		CONNECTION_POINT_ENTRY(__uuidof(_ICosmosObjEvents))
	END_CONNECTION_POINT_MAP()

	void Lock(){}
	void Unlock(){}
	void BeforeDestory();

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
	STDMETHOD(get_Xobj)(BSTR bstrNodeName, IXobj** pVal);
	STDMETHOD(get_XobjNames)(BSTR* pVal);
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
	STDMETHOD(GetXobj)(BSTR bstrGalaxyName, BSTR bstrNodeName, IXobj** pRetNode);
	STDMETHOD(GetGalaxyFromCtrl)(IDispatch* ctrl, IGalaxy** ppGalaxy);
	STDMETHOD(GetCtrlInXobj)(BSTR NodeName, BSTR CtrlName, IDispatch** ppCtrl);
	STDMETHOD(Observe)(IDispatch* Parent, BSTR CtrlName, BSTR GalaxyName, BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj);
	STDMETHOD(ObserveCtrl)(VARIANT MdiForm, BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj);
	STDMETHOD(ConnectCosmosCtrl)(ICosmosCtrl* eventSource);
	STDMETHOD(CreateGalaxyWithDefaultNode)(ULONGLONG hGalaxyWnd, BSTR bstrGalaxyName, BSTR bstrDefaultNodeKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfig, IXobj** ppXobj);
	STDMETHOD(ObserveGalaxys)(BSTR bstrGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfigFile);
	STDMETHOD(get_CurrentDesignGalaxyType)(GalaxyType* pVal);
	STDMETHOD(get_CurrentDesignNode)(IXobj** pVal);
};

