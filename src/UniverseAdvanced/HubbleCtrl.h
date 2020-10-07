/********************************************************************************
*					Open Universe - version 1.0.1.8							*
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

// HubbleCtrl : Declaration of the CHubbleCtrl
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "universe.h"

// CHubbleCtrl
class ATL_NO_VTABLE CHubbleCtrlBase :
	public CComControl<CHubbleCtrlBase>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public IViewObjectExImpl<CHubbleCtrlBase>,
	public IOleInPlaceActiveObjectImpl<CHubbleCtrlBase>,
	public IOleInPlaceObjectWindowlessImpl<CHubbleCtrlBase>
{
public:
#pragma warning(push)
#pragma warning(disable: 4355) // 'this' : used in base member initializer list

	CHubbleCtrlBase();
	virtual ~CHubbleCtrlBase();

#pragma warning(pop)

	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |OLEMISC_CANTLINKINSIDE |OLEMISC_INSIDEOUT)

	DECLARE_WND_CLASS(_T("Tangram Control Class"))

	BEGIN_COM_MAP(CHubbleCtrlBase)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
	END_COM_MAP()

	BEGIN_PROP_MAP(CHubbleCtrlBase)
	END_PROP_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	virtual void OnFinalMessage(HWND hWnd);
};

// CHubbleAppCtrl
class ATL_NO_VTABLE CHubbleAppCtrl :
	public CHubbleCtrlBase,
	public IConnectionPointContainerImpl<CHubbleAppCtrl>,
	public IConnectionPointImpl<CHubbleAppCtrl, &DIID__IHubbleAppEvents>
{
public:
	CHubbleAppCtrl();
	virtual ~CHubbleAppCtrl();

	BEGIN_COM_MAP(CHubbleAppCtrl)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CHubbleAppCtrl)
		CONNECTION_POINT_ENTRY(DIID__IHubbleAppEvents)
	END_CONNECTION_POINT_MAP()

public:
	BEGIN_MSG_MAP(CHubbleAppCtrl)
	END_MSG_MAP()

	HRESULT Fire_HubbleEvent(IHubbleEventObj* pEventObj);

private:
	HWND m_hHostWnd;
};

// CHubbleCtrl
class ATL_NO_VTABLE CHubbleCtrl : 
	public CHubbleCtrlBase,
	public IOleObjectImpl<CHubbleCtrl>,
	public CComCoClass<CHubbleCtrl, &CLSID_HubbleCtrl>,
	public IDispatchImpl<IHubbleCtrl, &IID_IHubbleCtrl, &LIBID_Universe, 1, 0>
{
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_TANGRAMCTRL)

	BEGIN_COM_MAP(CHubbleCtrl)
		COM_INTERFACE_ENTRY(IHubbleCtrl)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IOleObject)
		COM_INTERFACE_ENTRY(IViewObject2)
		COM_INTERFACE_ENTRY(IViewObject)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(CHubbleCtrl)
		IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
		IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	END_CATEGORY_MAP()

public:
	STDMETHOD(get_HWND)(LONGLONG* pVal);
	STDMETHOD(get_Hubble)(IHubble** pVal);
	STDMETHOD(get_tag)(VARIANT* pVal);
	STDMETHOD(put_tag)(VARIANT newVal);

	static HRESULT WINAPI CreateInstance(void* pv, REFIID riid, LPVOID* ppv);
};

OBJECT_ENTRY_AUTO_EX(__uuidof(HubbleCtrl), CHubbleCtrl)
