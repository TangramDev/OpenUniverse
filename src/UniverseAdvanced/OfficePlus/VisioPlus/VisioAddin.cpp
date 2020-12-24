/********************************************************************************
 *					DOM Plus for Application - Version 1.1.7.40
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

#include "../../stdafx.h"
#include "../../UniverseApp.h"
#include "VisioAddin.h"

namespace OfficePlus
{
	namespace VisioPlus
	{
		CVisioAddin::CVisioAddin()
		{
			CString strVer = theApp.GetFileVer();
			int nPos = strVer.Find(_T("."));
			strVer = strVer.Left(nPos);
			int nVer = _wtoi(strVer);
		}

		CVisioAddin::~CVisioAddin()
		{
		}

		STDMETHODIMP CVisioAddin::CosmosCommand(IDispatch* RibbonControl)
		{
			if (m_spRibbonUI)
				m_spRibbonUI->Invalidate();
			return S_OK;
		}

		HRESULT CVisioAddin::OnConnection(IDispatch* pHostApp, int ConnectMode)
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
			//pHostApp->QueryInterface(__uuidof(IDispatch), (LPVOID*)&m_pApplication);
			return S_OK;
		}

		HRESULT CVisioAddin::OnDisconnection(int DisConnectMode)
		{
			//m_pApplication.p->Release();
			//m_pApplication.Detach();
			return S_OK;
		}

		STDMETHODIMP CVisioAddin::GetCustomUI(BSTR RibbonID, BSTR* RibbonXml)
		{
			if (!RibbonXml)
				return E_POINTER;
			*RibbonXml = m_strRibbonXml.AllocSysString();
			return (*RibbonXml ? S_OK : E_OUTOFMEMORY);
		}

		void CVisioAddin::WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd)
		{
		}

		HRESULT CVisioAddin::CreateCosmosCtrl(void* pv, REFIID riid, LPVOID* ppv)
		{
			return CVisioAppCtrl::_CreatorClass::CreateInstance(pv, riid, ppv);
		}

		CVisioAppCtrl::CVisioAppCtrl()
		{

		}

		STDMETHODIMP CVisioAppCtrl::put_AppCtrl(VARIANT_BOOL newVal)
		{
			g_pCosmos->m_pCosmosAppCtrl = this;

			return S_OK;
		}

		void CVisioAppCtrl::OnFinalMessage(HWND hWnd)
		{
			__super::OnFinalMessage(hWnd);
		}

		STDMETHODIMP CVisioAppCtrl::get_tag(VARIANT* pVal)
		{
			return S_OK;
		}


		STDMETHODIMP CVisioAppCtrl::put_tag(VARIANT newVal)
		{
			return S_OK;
		}

		STDMETHODIMP CVisioAppCtrl::get_HWND(LONGLONG* pVal)
		{
			*pVal = (long)m_hWnd;
			return S_OK;
		}

		STDMETHODIMP CVisioAppCtrl::get_Cosmos(ICosmos** pVal)
		{
			*pVal = g_pCosmos;
			return S_OK;
		}
	}
}// namespace OfficePlus
