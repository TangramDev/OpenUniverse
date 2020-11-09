/********************************************************************************
*					Open Universe - version 1.0.1.13							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
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

		STDMETHODIMP CVisioAddin::HubbleCommand(IDispatch* RibbonControl)
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

		HRESULT CVisioAddin::CreateHubbleCtrl(void* pv, REFIID riid, LPVOID* ppv)
		{
			return CVisioAppCtrl::_CreatorClass::CreateInstance(pv, riid, ppv);
		}

		CVisioAppCtrl::CVisioAppCtrl()
		{

		}

		STDMETHODIMP CVisioAppCtrl::put_AppCtrl(VARIANT_BOOL newVal)
		{
			g_pHubble->m_pHubbleAppCtrl = this;

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

		STDMETHODIMP CVisioAppCtrl::get_Hubble(IHubble** pVal)
		{
			*pVal = g_pHubble;
			return S_OK;
		}
	}
}// namespace OfficePlus
