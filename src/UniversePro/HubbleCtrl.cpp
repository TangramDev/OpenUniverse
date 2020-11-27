/********************************************************************************
*					Open Universe - version 1.0.1.20							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE MIT LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
********************************************************************************/

// TangramCtrl.cpp : Implementation of CHubbleCtrl
#include "stdafx.h"
#include "UniverseApp.h"
#include "Grid.h"
#include "Galaxy.h"
#include "HubbleCtrl.h"

HRESULT WINAPI CHubbleCtrl::CreateInstance(void* pv, REFIID riid, LPVOID* ppv)
{
	if (g_pHubble->m_strCurrentAppID == _T(""))
	{
		if(g_pHubble->m_pHubbleDelegate->m_pJVM)
		{
			return CEclipseCtrl::_CreatorClass::CreateInstance(pv, riid, ppv);
		}
		if (g_pHubble->m_pUniverseAppProxy)
		{
			HRESULT hr = g_pHubble->m_pUniverseAppProxy->CreateHubbleCtrl(pv, riid, ppv);
			if (hr == S_OK)
				return hr;
		}
		return CHubbleCtrl::_CreatorClass::CreateInstance(pv, riid, ppv);
	}
	else
	{
		IHubbleCtrl* pCtrl = nullptr;
		HRESULT hr = g_pHubble->CreateHubbleCtrl(g_pHubble->m_strCurrentAppID.AllocSysString(), &pCtrl);
		if (hr == S_OK)
		{
			*ppv = pCtrl;
			return hr;
		}
		g_pHubble->m_strCurrentAppID = _T("");
		return S_FALSE;
	}
};

// CHubbleCtrl
CHubbleCtrlBase::CHubbleCtrlBase()
{
	m_bWindowOnly = true;
	m_hWnd = NULL;
#ifdef _DEBUG
	g_pHubble->m_nTangramCtrl++;
#endif
}

CHubbleCtrlBase::~CHubbleCtrlBase()
{
#ifdef _DEBUG
	g_pHubble->m_nTangramCtrl--;
#endif
}

void CHubbleCtrlBase::OnFinalMessage(HWND hWnd)
{
	if (g_pHubble->m_pCLRProxy)
	{
		g_pHubble->m_pCLRProxy->ReleaseHubbleObj((IHubbleCtrl*)this);
	}
	__super::OnFinalMessage(hWnd);
}

STDMETHODIMP CHubbleCtrl::get_HWND(LONGLONG* pVal)
{
	*pVal = (LONGLONG)m_hWnd;
	return S_OK;
}

STDMETHODIMP CHubbleCtrl::get_Hubble(IHubble** pVal)
{
	*pVal = g_pHubble;
	return S_OK;
}

CHubbleAppCtrl::CHubbleAppCtrl()
{
}

CHubbleAppCtrl::~CHubbleAppCtrl()
{
	if (g_pHubble->m_pHubbleAppCtrl == (CHubbleAppCtrl*)this)
		g_pHubble->m_pHubbleAppCtrl = nullptr;
}

HRESULT CHubbleAppCtrl::Fire_HubbleEvent(IHubbleEventObj* pEventObj)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[1];
		avarParams[0] = pEventObj;
		avarParams[0].vt = VT_DISPATCH;
		DISPPARAMS params = { avarParams, NULL, 1, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	return hr;
}

STDMETHODIMP CHubbleCtrl::get_tag(VARIANT* pVal)
{
	return S_OK;
}


STDMETHODIMP CHubbleCtrl::put_tag(VARIANT newVal)
{
	return S_OK;
}
