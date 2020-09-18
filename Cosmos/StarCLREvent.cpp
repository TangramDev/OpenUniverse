/********************************************************************************
*					Open Universe - version 0.9.0								*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
********************************************************************************/

#include "stdafx.h"
#include "StarCLREvent.h"

using namespace System::Runtime::InteropServices;

CStarCLREvent::CStarCLREvent()
{
}


CStarCLREvent::~CStarCLREvent()
{
	//delete m_pStar;
}

void CStarCLREvent::OnOpenComplete(IStar* pNode)
{
	m_pStar->Fire_OpenComplete(m_pStar);
}

void CStarCLREvent::OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl)
{
	Object^ pObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pDocdisp)));
	m_pStar->Fire_OnDocumentComplete(m_pStar, pObj, BSTR2STRING(bstrUrl));
}

void CStarCLREvent::OnDestroy()
{
	if (m_pStar->m_pHostObj)
	{
		Star^ pNode = nullptr;
		if (Cosmos::Hubble::m_pFrameworkElementDic->TryGetValue(m_pStar->m_pHostObj, pNode))
		{
			Cosmos::Hubble::m_pFrameworkElementDic->Remove(m_pStar->m_pHostObj);
		}
	}
	m_pStar->Fire_OnDestroy(m_pStar);
}

void CStarCLREvent::OnNodeAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
	Object^ pAddinObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pAddIndisp)));
	m_pStar->Fire_NodeAddInCreated(m_pStar, pAddinObj, BSTR2STRING(bstrAddInID), BSTR2STRING(bstrAddInXml));
}

void CStarCLREvent::OnNodeAddInsCreated()
{
	m_pStar->Fire_NodeAddInsCreated(m_pStar);
}

void CStarCLREvent::OnTabChange(int nActivePage, int nOldPage)
{
	m_pStar->Fire_OnTabChange(nActivePage, nOldPage);
}

void CStarCLREvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	m_pStar->Fire_OnIPCMessageReceived(BSTR2STRING(bstrFrom), BSTR2STRING(bstrTo), BSTR2STRING(bstrMsgId), BSTR2STRING(bstrPayload), BSTR2STRING(bstrExtra));
}
