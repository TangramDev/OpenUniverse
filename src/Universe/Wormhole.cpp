/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
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

#include "stdafx.h"
#include "Hubble.h"
#include "UniverseApp.h"
#include "Wormhole.h"

CWormhole::CWormhole()
{
	m_pSession = nullptr;
}

CWormhole::~CWormhole()
{
}

void CWormhole::InsertString(CString key, CString value)
{
	m_pOwner->m_pChromeRenderFrameHost->InsertString(m_pSession, key, value);
}

void CWormhole::InsertLong(CString key, long value)
{
	m_pOwner->m_pChromeRenderFrameHost->InsertLong(m_pSession, key, value);
}

void CWormhole::Insertint64(CString key, __int64 value)
{
	m_pOwner->m_pChromeRenderFrameHost->Insertint64(m_pSession, key, value);
}

void CWormhole::InsertFloat(CString key, float value)
{
	m_pOwner->m_pChromeRenderFrameHost->InsertFloat(m_pSession, key, value);
}

CString CWormhole::GetString(CString key)
{
	return m_pOwner->m_pChromeRenderFrameHost->GetString(m_pSession, key);
}

long CWormhole::GetLong(CString key)
{
	return m_pOwner->m_pChromeRenderFrameHost->GetLong(m_pSession, key);
}

__int64 CWormhole::Getint64(CString key)
{
	return m_pOwner->m_pChromeRenderFrameHost->Getint64(m_pSession, key);
}

float CWormhole::GetFloat(CString key)
{
	return m_pOwner->m_pChromeRenderFrameHost->GetFloat(m_pSession, key);
}

void CWormhole::SendMessage()
{
	if (m_pOwner)
	{
		m_pOwner->m_pChromeRenderFrameHost->SendHubbleMessage(m_pSession);
	}
}

void CWormhole::addEventListener(CString ListenerName)
{
}

void CWormhole::removeEventListener(CString ListenerName) {
}

