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
* OUTLINED IN THE GPL LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* http://www.tangramteam.com/
*
********************************************************************************/

#pragma once

#include "universe.h"
extern _ATL_FUNC_INFO OpenComplete;
extern _ATL_FUNC_INFO HubbleNotify;
extern _ATL_FUNC_INFO DocumentComplete;
extern _ATL_FUNC_INFO NodeOpenComplete;
extern _ATL_FUNC_INFO Destroy;
extern _ATL_FUNC_INFO TabChange;
extern _ATL_FUNC_INFO TabChange2;
extern _ATL_FUNC_INFO MessageHandle;
extern _ATL_FUNC_INFO TangramCoreClose;
extern _ATL_FUNC_INFO TangramEvent;
extern _ATL_FUNC_INFO Initialize;
extern _ATL_FUNC_INFO NodeAddInCreated;
extern _ATL_FUNC_INFO NodeAddInsCreated;


class CTangramCoreEvents : public IDispEventSimpleImpl</*nID =*/ 1, CTangramCoreEvents, &__uuidof(_IHubble)>
{
public:
	virtual void __stdcall OnOpenComplete(long hWnd, BSTR bstrUrl, IGrid* pRootGrid){}
	virtual void __stdcall OnClose(){}
	virtual void __stdcall OnHubbleEvent(IHubbleEventObj* pRootGrid){}

	BEGIN_SINK_MAP(CTangramCoreEvents)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IHubble), /*dispid =*/ 0x00000001, OnOpenComplete, &OpenComplete)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IHubble), /*dispid =*/ 0x00000002, OnClose, &TangramCoreClose)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IHubble), /*dispid =*/ 0x00000003, OnHubbleEvent, &TangramEvent)
	END_SINK_MAP()
};

class CGridEvents : public IDispEventSimpleImpl</*nID =*/ 1, CGridEvents, &__uuidof(_IGridEvents)>
{
public:
	CGridEvents(){ m_pGrid = NULL; };
	virtual ~CGridEvents(){};

	IGrid* m_pGrid;

	virtual void __stdcall  OnDestroy(){}
	virtual void __stdcall  OnOpenComplete(){}
	virtual void __stdcall  OnNodeAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml){}
	virtual void __stdcall  OnNodeAddInsCreated(){}
	virtual void __stdcall  OnTabChange(int nActivePage, int nOldPage){}
	virtual void __stdcall  OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra) {};

	BEGIN_SINK_MAP(CGridEvents)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000001, OnOpenComplete, &NodeOpenComplete)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000002, OnDestroy, &Destroy)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000003, OnNodeAddInCreated, &NodeAddInCreated)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000004, OnNodeAddInsCreated, &NodeAddInsCreated)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000007, OnTabChange, &TabChange2)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IGridEvents), /*dispid =*/ 0x00000008, OnIPCMessageReceived, &MessageHandle)
	END_SINK_MAP()
};
