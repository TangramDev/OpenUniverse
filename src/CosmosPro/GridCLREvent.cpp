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
* https://www.tangram.dev
*
*
********************************************************************************/

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
		DOMPlus::Xobj^ pXobj = nullptr;
		if (DOMPlus::Cosmos::m_pFrameworkElementDic->TryGetValue(m_pXobj->m_pHostObj, pXobj))
		{
			DOMPlus::Cosmos::m_pFrameworkElementDic->Remove(m_pXobj->m_pHostObj);
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
	DOMPlus::Xobj^ pActiveXobj = m_pXobj->GetXobj(0, nActivePage);
	DOMPlus::Xobj^ pOldXobj = m_pXobj->GetXobj(0, nOldPage);
	m_pXobj->Fire_OnTabChange(pActiveXobj, pOldXobj);
}

void CXobjCLREvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	m_pXobj->Fire_OnIPCMessageReceived(BSTR2STRING(bstrFrom), BSTR2STRING(bstrTo), BSTR2STRING(bstrMsgId), BSTR2STRING(bstrPayload), BSTR2STRING(bstrExtra));
}
