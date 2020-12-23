/********************************************************************************
*					DOM Plus for Application - Version 1.1.6.35                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
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
using namespace Universe;

class CXobjCLREvent
{
public:
	CXobjCLREvent();
	virtual ~CXobjCLREvent();

	gcroot<Universe::Xobj^>	m_pXobj;

	void OnDestroy();
	void OnXobjAddInsCreated();
	void OnObserverComplete(IXobj* pXobj);
	void OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl);
	void OnXobjAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml);
	void OnTabChange(int nActivePage, int nOldPage);
	void OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra);
};
