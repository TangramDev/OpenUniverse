/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
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

#include "stdafx.h"
#include "GridCLREvent.h"

using namespace System::Runtime::InteropServices;

CXobjCLREvent::CXobjCLREvent()
{
}

CXobjCLREvent::~CXobjCLREvent()
{
}

void CXobjCLREvent::OnObserverComplete(IXobj* pXobj)
{
	m_pXobj->Fire_ObserveComplete(m_pXobj);
}

void CXobjCLREvent::OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl)
{
	Object^ pObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pDocdisp)));
	m_pXobj->Fire_OnDocumentComplete(m_pXobj, pObj, BSTR2STRING(bstrUrl));
}

void CXobjCLREvent::OnDestroy()
{
	m_pXobj->Fire_OnDestroy(m_pXobj);
}

void CXobjCLREvent::OnGridAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
}

void CXobjCLREvent::OnGridAddInsCreated()
{
}

void CXobjCLREvent::OnTabChange(int nActivePage, int nOldPage)
{
	DOMPlus::Xobj^ pActiveGrid = m_pXobj->GetGrid(0, nActivePage);
	DOMPlus::Xobj^ pOldGrid = m_pXobj->GetGrid(0, nOldPage);
	m_pXobj->Fire_OnTabChange(pActiveGrid, pOldGrid);
}

void CXobjCLREvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	m_pXobj->Fire_OnIPCMessageReceived(BSTR2STRING(bstrFrom), BSTR2STRING(bstrTo), BSTR2STRING(bstrMsgId), BSTR2STRING(bstrPayload), BSTR2STRING(bstrExtra));
}
