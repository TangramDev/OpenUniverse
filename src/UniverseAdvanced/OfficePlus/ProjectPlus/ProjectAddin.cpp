/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102100029
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
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#include "../../stdafx.h"
#include "../../UniverseApp.h"
#include "ProjectAddin.h"
#include "ProjectPlusEvents.cpp"

namespace OfficePlus
{
	namespace ProjectPlus
	{
		CProjectAddin::CProjectAddin():COfficeAddin()
		{
		}

		CProjectAddin::~CProjectAddin()
		{
		}

		STDMETHODIMP CProjectAddin::CosmosCommand(IDispatch* RibbonControl)
		{
			if (m_spRibbonUI)
				m_spRibbonUI->Invalidate();
			return S_OK;
		}

		HRESULT CProjectAddin::OnConnection(IDispatch* pHostApp, int ConnectMode)
		{
			COfficeAddin::OnConnection(pHostApp, ConnectMode);
			if (m_strRibbonXml == _T(""))
			{
				CTangramXmlParse m_Parse;
				if (m_Parse.LoadXml(m_strConfigFile) || m_Parse.LoadFile(m_strConfigFile))
				{
					m_strRibbonXml = m_Parse[_T("RibbonUI")][_T("customUI")].xml();
				}
			}
			pHostApp->QueryInterface(__uuidof(IDispatch), (LPVOID*)&m_pApplication);
			return S_OK;
		}

		HRESULT CProjectAddin::OnDisconnection(int DisConnectMode)
		{
			m_pApplication.p->Release();
			m_pApplication.Detach();
			return S_OK;
		}

		STDMETHODIMP CProjectAddin::GetCustomUI(BSTR RibbonID, BSTR* RibbonXml)
		{
			if (!RibbonXml)
				return E_POINTER;
			*RibbonXml = m_strRibbonXml.AllocSysString();
			return (*RibbonXml ? S_OK : E_OUTOFMEMORY);
		}

		HRESULT CProjectAddin::CreateCosmosCtrl(void* pv, REFIID riid, LPVOID* ppv)
		{
			return CProjectAppCtrl::_CreatorClass::CreateInstance(pv, riid, ppv);
		}

		void CProjectAddin::WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd)
		{
		}

		CProjectAppCtrl::CProjectAppCtrl()
		{

		}

		void CProjectAppCtrl::OnFinalMessage(HWND hWnd)
		{
			__super::OnFinalMessage(hWnd);
		}

		STDMETHODIMP CProjectAppCtrl::put_AppCtrl(VARIANT_BOOL newVal)
		{
			g_pCosmos->m_pCosmosAppCtrl = this;

			return S_OK;
		}

		STDMETHODIMP CProjectAppCtrl::get_tag(VARIANT* pVal)
		{
			return S_OK;
		}


		STDMETHODIMP CProjectAppCtrl::put_tag(VARIANT newVal)
		{
			return S_OK;
		}

		STDMETHODIMP CProjectAppCtrl::get_HWND(LONGLONG* pVal)
		{
			*pVal = (long)m_hWnd;
			return S_OK;
		}

		STDMETHODIMP CProjectAppCtrl::get_Cosmos(ICosmos** pVal)
		{
			*pVal = g_pCosmos;
			return S_OK;
		}
	}
}// namespace OfficePlus

