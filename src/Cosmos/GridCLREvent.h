/********************************************************************************
*					Open Universe - version 1.0.1.15							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
********************************************************************************/

#pragma once
//#include "dllmain.h"

#include "Cosmos.h"
#include "Universe.h"       // main symbols
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
