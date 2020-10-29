/********************************************************************************
*					Open Universe - version 1.0.1.11							*
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
*
********************************************************************************/

#pragma once
//#include "dllmain.h"

#include "Cosmos.h"
#include "universe.h"       // main symbols
#include "TangramBase.h"       // main symbols
using namespace System;
using namespace System::Reflection;
using namespace Cosmos;

class CGridCLREvent
{
public:
	CGridCLREvent();
	virtual ~CGridCLREvent();

	gcroot<Cosmos::Grid^>	m_pGrid;

	void OnDestroy();
	void OnGridAddInsCreated();
	void OnObserverComplete(IGrid* pGrid);
	void OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl);
	void OnGridAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml);
	void OnTabChange(int nActivePage, int nOldPage);
	void OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra);
};
