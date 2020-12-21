/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
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
* http://www.tangramteam.com/
*
********************************************************************************/

#pragma once

#include "universe.h"
extern _ATL_FUNC_INFO OpenComplete;
extern _ATL_FUNC_INFO CosmosNotify;
extern _ATL_FUNC_INFO DocumentComplete;
extern _ATL_FUNC_INFO NodeOpenComplete;
extern _ATL_FUNC_INFO Destroy;
extern _ATL_FUNC_INFO TabChange;
extern _ATL_FUNC_INFO TabChange2;
extern _ATL_FUNC_INFO MessageHandle;
extern _ATL_FUNC_INFO CosmosClose;
extern _ATL_FUNC_INFO TangramEvent;
extern _ATL_FUNC_INFO Initialize;
extern _ATL_FUNC_INFO XobjAddInCreated;
extern _ATL_FUNC_INFO XobjAddInsCreated;


class CCosmosEvents : public IDispEventSimpleImpl</*nID =*/ 1, CCosmosEvents, &__uuidof(_ICosmos)>
{
public:
	virtual void __stdcall OnObserverComplete(long hWnd, BSTR bstrUrl, IXobj* pRootXobj){}
	virtual void __stdcall OnClose(){}
	virtual void __stdcall OnCosmosEvent(ICosmosEventObj* pRootXobj){}

	BEGIN_SINK_MAP(CCosmosEvents)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_ICosmos), /*dispid =*/ 0x00000001, OnObserverComplete, &OpenComplete)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_ICosmos), /*dispid =*/ 0x00000002, OnClose, &CosmosClose)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_ICosmos), /*dispid =*/ 0x00000003, OnCosmosEvent, &TangramEvent)
	END_SINK_MAP()
};

class CXobjEvents : public IDispEventSimpleImpl</*nID =*/ 1, CXobjEvents, &__uuidof(_IXobjEvents)>
{
public:
	CXobjEvents(){ m_pXobj = NULL; };
	virtual ~CXobjEvents(){};

	IXobj* m_pXobj;

	virtual void __stdcall  OnDestroy(){}
	virtual void __stdcall  OnObserverComplete(){}
	virtual void __stdcall  OnXobjAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml){}
	virtual void __stdcall  OnXobjAddInsCreated(){}
	virtual void __stdcall  OnTabChange(int nActivePage, int nOldPage){}
	virtual void __stdcall  OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra) {};

	BEGIN_SINK_MAP(CXobjEvents)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IXobjEvents), /*dispid =*/ 0x00000001, OnObserverComplete, &NodeOpenComplete)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IXobjEvents), /*dispid =*/ 0x00000002, OnDestroy, &Destroy)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IXobjEvents), /*dispid =*/ 0x00000003, OnXobjAddInCreated, &XobjAddInCreated)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IXobjEvents), /*dispid =*/ 0x00000004, OnXobjAddInsCreated, &XobjAddInsCreated)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IXobjEvents), /*dispid =*/ 0x00000007, OnTabChange, &TabChange2)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IXobjEvents), /*dispid =*/ 0x00000008, OnIPCMessageReceived, &MessageHandle)
	END_SINK_MAP()
};
