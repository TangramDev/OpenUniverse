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
}

void CGridCLREvent::OnDestroy()
{
	m_pGrid->Fire_OnDestroy(m_pGrid);
}

void CGridCLREvent::OnGridAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
}

void CGridCLREvent::OnGridAddInsCreated()
{
}

void CGridCLREvent::OnTabChange(int nActivePage, int nOldPage)
{
	DOMPlus::Grid^ pActiveGrid = m_pGrid->GetGrid(0, nActivePage);
	DOMPlus::Grid^ pOldGrid = m_pGrid->GetGrid(0, nOldPage);
	m_pGrid->Fire_OnTabChange(pActiveGrid, pOldGrid);
}

void CGridCLREvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
}
