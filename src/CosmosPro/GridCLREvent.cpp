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

CGridCLREvent::CGridCLREvent()
{
}


CGridCLREvent::~CGridCLREvent()
{
	//delete m_pGrid;
}

void CGridCLREvent::OnObserverComplete(IGrid* pGrid)
{
	m_pGrid->Fire_ObserveComplete(m_pGrid);
}

void CGridCLREvent::OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl)
{
	Object^ pObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pDocdisp)));
	m_pGrid->Fire_OnDocumentComplete(m_pGrid, pObj, BSTR2STRING(bstrUrl));
}

void CGridCLREvent::OnDestroy()
{
	if (theAppProxy.m_pPropertyGrid&&theAppProxy.m_pPropertyGrid->SelectedObject==(Object^)m_pGrid)
	{
		theAppProxy.m_pPropertyGrid->SelectedObject = nullptr;
	}
	if (m_pGrid->m_pHostObj)
	{
		DOMPlus::Grid^ pGrid = nullptr;
		if (DOMPlus::Cosmos::m_pFrameworkElementDic->TryGetValue(m_pGrid->m_pHostObj, pGrid))
		{
			DOMPlus::Cosmos::m_pFrameworkElementDic->Remove(m_pGrid->m_pHostObj);
		}
	}
	m_pGrid->Fire_OnDestroy(m_pGrid);
}

void CGridCLREvent::OnGridAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
	Object^ pAddinObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pAddIndisp)));
	m_pGrid->Fire_GridAddInCreated(m_pGrid, pAddinObj, BSTR2STRING(bstrAddInID), BSTR2STRING(bstrAddInXml));
}

void CGridCLREvent::OnGridAddInsCreated()
{
	m_pGrid->Fire_GridAddInsCreated(m_pGrid);
}

void CGridCLREvent::OnTabChange(int nActivePage, int nOldPage)
{
	DOMPlus::Grid^ pActiveGrid = m_pGrid->GetGrid(0, nActivePage);
	DOMPlus::Grid^ pOldGrid = m_pGrid->GetGrid(0, nOldPage);
	m_pGrid->Fire_OnTabChange(pActiveGrid, pOldGrid);
}

void CGridCLREvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	m_pGrid->Fire_OnIPCMessageReceived(BSTR2STRING(bstrFrom), BSTR2STRING(bstrTo), BSTR2STRING(bstrMsgId), BSTR2STRING(bstrPayload), BSTR2STRING(bstrExtra));
}
