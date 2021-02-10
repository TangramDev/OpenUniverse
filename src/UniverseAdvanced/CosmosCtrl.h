/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102100029           *
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
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

// CosmosCtrl : Declaration of the CCosmosCtrl
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "universe.h"

// CCosmosCtrl
class ATL_NO_VTABLE CCosmosCtrlBase :
	public CComControl<CCosmosCtrlBase>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public IViewObjectExImpl<CCosmosCtrlBase>,
	public IOleInPlaceActiveObjectImpl<CCosmosCtrlBase>,
	public IOleInPlaceObjectWindowlessImpl<CCosmosCtrlBase>
{
public:
#pragma warning(push)
#pragma warning(disable: 4355) // 'this' : used in base member initializer list

	CCosmosCtrlBase();
	virtual ~CCosmosCtrlBase();

#pragma warning(pop)

	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |OLEMISC_CANTLINKINSIDE |OLEMISC_INSIDEOUT)

	DECLARE_WND_CLASS(_T("Tangram Control Class"))

	BEGIN_COM_MAP(CCosmosCtrlBase)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
	END_COM_MAP()

	BEGIN_PROP_MAP(CCosmosCtrlBase)
	END_PROP_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	virtual void OnFinalMessage(HWND hWnd);
};

// CCosmosAppCtrl
class ATL_NO_VTABLE CCosmosAppCtrl :
	public CCosmosCtrlBase,
	public IConnectionPointContainerImpl<CCosmosAppCtrl>,
	public IConnectionPointImpl<CCosmosAppCtrl, &DIID__ICosmosAppEvents>
{
public:
	CCosmosAppCtrl();
	virtual ~CCosmosAppCtrl();

	BEGIN_COM_MAP(CCosmosAppCtrl)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CCosmosAppCtrl)
		CONNECTION_POINT_ENTRY(DIID__ICosmosAppEvents)
	END_CONNECTION_POINT_MAP()

public:
	BEGIN_MSG_MAP(CCosmosAppCtrl)
	END_MSG_MAP()

	HRESULT Fire_CosmosEvent(ICosmosEventObj* pEventObj);

private:
	HWND m_hHostWnd;
};

// CCosmosCtrl
class ATL_NO_VTABLE CCosmosCtrl : 
	public CCosmosCtrlBase,
	public IOleObjectImpl<CCosmosCtrl>,
	public CComCoClass<CCosmosCtrl, &CLSID_CosmosCtrl>,
	public IDispatchImpl<ICosmosCtrl, &IID_ICosmosCtrl, &LIBID_Universe, 1, 0>
{
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_TANGRAMCTRL)

	BEGIN_COM_MAP(CCosmosCtrl)
		COM_INTERFACE_ENTRY(ICosmosCtrl)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IOleObject)
		COM_INTERFACE_ENTRY(IViewObject2)
		COM_INTERFACE_ENTRY(IViewObject)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	END_COM_MAP()

public:
	STDMETHOD(get_HWND)(LONGLONG* pVal);
	STDMETHOD(get_Cosmos)(ICosmos** pVal);
	STDMETHOD(get_tag)(VARIANT* pVal);
	STDMETHOD(put_tag)(VARIANT newVal);

	static HRESULT WINAPI CreateInstance(void* pv, REFIID riid, LPVOID* ppv);
};

OBJECT_ENTRY_AUTO_EX(__uuidof(CosmosCtrl), CCosmosCtrl)
