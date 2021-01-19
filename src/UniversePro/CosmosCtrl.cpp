/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101190013           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser: Independent Browser Window and Browser Window
 *    as sub windows of other windows are supported in the application process;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

// TangramCtrl.cpp : Implementation of CCosmosCtrl
#include "stdafx.h"
#include "UniverseApp.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "CosmosCtrl.h"

HRESULT WINAPI CCosmosCtrl::CreateInstance(void* pv, REFIID riid, LPVOID* ppv)
{
	if (g_pCosmos->m_strCurrentAppID == _T(""))
	{
		if(g_pCosmos->m_pCosmosDelegate->m_pJVM)
		{
			return CEclipseCtrl::_CreatorClass::CreateInstance(pv, riid, ppv);
		}
		if (g_pCosmos->m_pUniverseAppProxy)
		{
			HRESULT hr = g_pCosmos->m_pUniverseAppProxy->CreateCosmosCtrl(pv, riid, ppv);
			if (hr == S_OK)
				return hr;
		}
		return CCosmosCtrl::_CreatorClass::CreateInstance(pv, riid, ppv);
	}
	else
	{
		ICosmosCtrl* pCtrl = nullptr;
		HRESULT hr = g_pCosmos->CreateCosmosCtrl(g_pCosmos->m_strCurrentAppID.AllocSysString(), &pCtrl);
		if (hr == S_OK)
		{
			*ppv = pCtrl;
			return hr;
		}
		g_pCosmos->m_strCurrentAppID = _T("");
		return S_FALSE;
	}
};

// CCosmosCtrl
CCosmosCtrlBase::CCosmosCtrlBase()
{
	m_bWindowOnly = true;
	m_hWnd = NULL;
#ifdef _DEBUG
	g_pCosmos->m_nTangramCtrl++;
#endif
}

CCosmosCtrlBase::~CCosmosCtrlBase()
{
#ifdef _DEBUG
	g_pCosmos->m_nTangramCtrl--;
#endif
}

void CCosmosCtrlBase::OnFinalMessage(HWND hWnd)
{
	if (g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_pCLRProxy->ReleaseCosmosObj((ICosmosCtrl*)this);
	}
	__super::OnFinalMessage(hWnd);
}

STDMETHODIMP CCosmosCtrl::get_HWND(LONGLONG* pVal)
{
	*pVal = (LONGLONG)m_hWnd;
	return S_OK;
}

STDMETHODIMP CCosmosCtrl::get_Cosmos(ICosmos** pVal)
{
	*pVal = g_pCosmos;
	return S_OK;
}

CCosmosAppCtrl::CCosmosAppCtrl()
{
}

CCosmosAppCtrl::~CCosmosAppCtrl()
{
	if (g_pCosmos->m_pCosmosAppCtrl == (CCosmosAppCtrl*)this)
		g_pCosmos->m_pCosmosAppCtrl = nullptr;
}

HRESULT CCosmosAppCtrl::Fire_CosmosEvent(ICosmosEventObj* pEventObj)
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
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();
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

STDMETHODIMP CCosmosCtrl::get_tag(VARIANT* pVal)
{
	return S_OK;
}


STDMETHODIMP CCosmosCtrl::put_tag(VARIANT newVal)
{
	return S_OK;
}
