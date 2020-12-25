/********************************************************************************
 *             DOM Plus for Application - Version 1.1.8.202012250001
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#include "stdafx.h"
#include "GridCLREvent.h"

using namespace System::Runtime::InteropServices;

CXobjCLREvent::CXobjCLREvent()
{
}


CXobjCLREvent::~CXobjCLREvent()
{
	//delete m_pXobj;
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
	if (theAppProxy.m_pPropertyGrid&&theAppProxy.m_pPropertyGrid->SelectedObject==(Object^)m_pXobj)
	{
		theAppProxy.m_pPropertyGrid->SelectedObject = nullptr;
	}
	if (m_pXobj->m_pHostObj)
	{
		Universe::Xobj^ pXobj = nullptr;
		if (Universe::Cosmos::m_pFrameworkElementDic->TryGetValue(m_pXobj->m_pHostObj, pXobj))
		{
			Universe::Cosmos::m_pFrameworkElementDic->Remove(m_pXobj->m_pHostObj);
		}
	}
	m_pXobj->Fire_OnDestroy(m_pXobj);
}

void CXobjCLREvent::OnXobjAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
	Object^ pAddinObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pAddIndisp)));
	m_pXobj->Fire_XobjAddInCreated(m_pXobj, pAddinObj, BSTR2STRING(bstrAddInID), BSTR2STRING(bstrAddInXml));
}

void CXobjCLREvent::OnXobjAddInsCreated()
{
	m_pXobj->Fire_XobjAddInsCreated(m_pXobj);
}

void CXobjCLREvent::OnTabChange(int nActivePage, int nOldPage)
{
	Universe::Xobj^ pActiveXobj = m_pXobj->GetXobj(0, nActivePage);
	Universe::Xobj^ pOldXobj = m_pXobj->GetXobj(0, nOldPage);
	m_pXobj->Fire_OnTabChange(pActiveXobj, pOldXobj);
}

void CXobjCLREvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	m_pXobj->Fire_OnIPCMessageReceived(BSTR2STRING(bstrFrom), BSTR2STRING(bstrTo), BSTR2STRING(bstrMsgId), BSTR2STRING(bstrPayload), BSTR2STRING(bstrExtra));
}
