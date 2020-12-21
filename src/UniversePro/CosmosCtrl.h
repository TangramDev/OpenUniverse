/********************************************************************************
*					DOM Plus for Application - Version 1.1.6.35                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
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
	public IDispatchImpl<ICosmosCtrl, &IID_ICosmosCtrl, &LIBID_World, 1, 0>
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

	BEGIN_CATEGORY_MAP(CCosmosCtrl)
		IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
		IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	END_CATEGORY_MAP()

public:
	STDMETHOD(get_HWND)(LONGLONG* pVal);
	STDMETHOD(get_Cosmos)(ICosmos** pVal);
	STDMETHOD(get_tag)(VARIANT* pVal);
	STDMETHOD(put_tag)(VARIANT newVal);

	static HRESULT WINAPI CreateInstance(void* pv, REFIID riid, LPVOID* ppv);
};

OBJECT_ENTRY_AUTO_EX(__uuidof(CosmosCtrl), CCosmosCtrl)
