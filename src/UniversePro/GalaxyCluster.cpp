/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102050025           *
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

// GalaxyCluster.cpp : Implementation of CGalaxyCluster

#include "stdafx.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "CosmosCtrl.h"
#include "UniverseApp.h"
#include "Cosmos.h"

CXobjShareData::CXobjShareData()
{
	m_pOldGalaxy			= nullptr;
	m_pOfficeObj		= nullptr;
	m_pCosmosParse		= nullptr;
	m_pHostClientView	= nullptr;
#ifdef _DEBUG
	g_pCosmos->m_nTangramNodeCommonData++;
#endif	
};

CXobjShareData::~CXobjShareData()
{
	if (m_pCosmosParse)
		delete m_pCosmosParse;
#ifdef _DEBUG
	g_pCosmos->m_nTangramNodeCommonData--;
#endif	
};

// CGalaxyCluster
CGalaxyCluster::CGalaxyCluster()
{
	m_hWnd								= 0;
	m_bPageDataLoaded					= false;
	m_bDoc								= false;
	m_strXmlHeader						= _T("");
	m_strPageFileName					= _T("");
	m_strPageFilePath					= _T("");
	m_strConfigFileNodeName				= _T("tangramdefaultpage");
	m_pBKGalaxy							= nullptr;
	g_pCosmos->m_pGalaxyCluster					= this;
	m_pUniverseAppProxy					= nullptr;

#ifdef _DEBUG
	g_pCosmos->m_nTangram++;
#endif	
}

CGalaxyCluster::~CGalaxyCluster()
{
	if (this == g_pCosmos->m_pDesignerGalaxyCluster)
		g_pCosmos->m_pDesignerGalaxyCluster = nullptr;
#ifdef _DEBUG
	g_pCosmos->m_nTangram--;
#endif	
	//if(g_pCosmos->m_nTangram==0)
	//	return;

	for (auto it2 : m_mapExternalObj)
	{
		it2.second->Release();
	}
	m_mapExternalObj.erase(m_mapExternalObj.begin(), m_mapExternalObj.end());

	m_mapGalaxy.erase(m_mapGalaxy.begin(), m_mapGalaxy.end());
	m_mapXobj.erase(m_mapXobj.begin(), m_mapXobj.end());
	auto it = g_pCosmos->m_mapWindowPage.find(m_hWnd);
	if (it != g_pCosmos->m_mapWindowPage.end())
	{
		g_pCosmos->m_mapWindowPage.erase(it);
	}
	if (g_pCosmos->m_mapWindowPage.size() == 0)
		g_pCosmos->Close();
	else
	{
		if (g_pCosmos->m_mapWindowPage.size() == 1 && g_pCosmos->m_pDesignerGalaxyCluster)
		{
			if (g_pCosmos->m_bDeleteGalaxyCluster == FALSE)
				::DestroyWindow(g_pCosmos->m_hHostWnd);
			else
				g_pCosmos->m_bDeleteGalaxyCluster = FALSE;
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		if (it.second->m_bAutoDelete)
			delete it.second;
	}
	m_mapGalaxyClusterProxy.clear();
	for (auto it : m_mapEventObj)
		delete it.second;
	m_mapEventObj.clear();
}

STDMETHODIMP CGalaxyCluster::get_Count(long* pCount)
{
	*pCount = (long)m_mapGalaxy.size();
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_Galaxy(VARIANT vIndex, IGalaxy ** ppGalaxy)
{
	if (vIndex.vt == VT_I4)
	{
		long lCount = m_mapGalaxy.size();
		int iIndex = vIndex.lVal;
		HWND hWnd = (HWND)iIndex;
		if (::IsWindow(hWnd))
		{
			auto it = m_mapGalaxy.find(hWnd);
			if (it != m_mapGalaxy.end())
			{
				*ppGalaxy = it->second;
				return S_OK;
			}
		}
		if (iIndex < 0 || iIndex >= lCount) return E_INVALIDARG;

		auto it = m_mapGalaxy.begin();
		int iPos = 0;
		while (it != m_mapGalaxy.end())
		{
			if (iPos == iIndex)
			{
				*ppGalaxy = it->second;
				return S_OK;
			};
			iPos++;
			it++;
		}
		return S_OK;
	}

	if (vIndex.vt == VT_BSTR)
	{
		CString strKey = OLE2T(vIndex.bstrVal);
		auto it = m_mapWnd.find(strKey);
		if (it != m_mapWnd.end())
		{
			auto it2 = m_mapGalaxy.find(it->second);
			if (it2 != m_mapGalaxy.end())
			{
				*ppGalaxy = it2->second;
				return S_OK;
			}
		}
		return E_INVALIDARG;
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get__NewEnum(IUnknown** ppVal)
{
	typedef CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>>
		CComEnumVariant;

	CComObject<CComEnumVariant> *pe = 0;
	HRESULT hr = pe->CreateInstance(&pe);

	if (SUCCEEDED(hr))
	{
		pe->AddRef();
		long nLen = (long)m_mapGalaxy.size();
		VARIANT* rgvar = new VARIANT[nLen];
		ZeroMemory(rgvar, sizeof(VARIANT)*nLen);
		VARIANT* pItem = rgvar;
		for (auto it : m_mapGalaxy)
		{
			IUnknown* pDisp = nullptr;
			CGalaxy* pObj = it.second;
			hr = pObj->QueryInterface(IID_IUnknown, (void**)&pDisp);
			pItem->vt = VT_UNKNOWN;
			pItem->punkVal = pDisp;
			if (pItem->punkVal != nullptr)
				pItem->punkVal->AddRef();
			pItem++;
			pDisp->Release();
		}

		hr = pe->Init(rgvar, &rgvar[nLen], 0, AtlFlagTakeOwnership);
		if (SUCCEEDED(hr))
			hr = pe->QueryInterface(IID_IUnknown, (void**)ppVal);
		pe->Release();
	}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::CreateGalaxy(VARIANT ParentObj, VARIANT HostWnd, BSTR bstrGalaxyName, IGalaxy** pRetFrame)
{
	HWND h = 0; 
	CString strGalaxyName = OLE2T(bstrGalaxyName);
	//auto it = m_mapWnd.find(strGalaxyName);
	//if (it != m_mapWnd.end())
	//{
	//	int i = 0;
	//	CString s = _T("");
	//	s.Format(_T("%s%d"), strGalaxyName,i);
	//	auto it = m_mapWnd.find(s);
	//	while (it != m_mapWnd.end())
	//	{
	//		i++;
	//		s.Format(_T("%s%d"), strGalaxyName, i);
	//		it = m_mapWnd.find(s);
	//		if (it == m_mapWnd.end())
	//			break;
	//	}
	//	strGalaxyName = s;
	//}
	BSTR bstrName = strGalaxyName.MakeLower().AllocSysString();
	if (ParentObj.vt == VT_DISPATCH&&HostWnd.vt == VT_BSTR)
	{
		if (g_pCosmos->m_pCLRProxy)
		{
			IDispatch* pDisp = nullptr;
			pDisp = g_pCosmos->m_pCLRProxy->GetCtrlByName(ParentObj.pdispVal, HostWnd.bstrVal, true);
			if (pDisp)
			{
				h = g_pCosmos->m_pCLRProxy->GetCtrlHandle(pDisp);
				if (h)
				{
					HRESULT hr = CreateGalaxy(CComVariant(0), CComVariant((long)h), bstrName, pRetFrame);
					::SysFreeString(bstrName);
					return hr;
				}
			}
		}
		::SysFreeString(bstrName);
		return S_FALSE;
	}
	if (HostWnd.vt == VT_DISPATCH)
	{
		if (g_pCosmos->m_pCLRProxy)
		{
			h = g_pCosmos->m_pCLRProxy->IsGalaxy(HostWnd.pdispVal);
			if (h)
			{
				CString strName = strGalaxyName;
				if (strName == _T(""))
				{
					::SysFreeString(bstrName);
					bstrGalaxyName = g_pCosmos->m_pCLRProxy->GetCtrlName(HostWnd.pdispVal);
					strName = OLE2T(bstrGalaxyName);
					if (strName == _T(""))
						bstrGalaxyName = CComBSTR(L"Default");
					
					strGalaxyName = OLE2T(bstrGalaxyName);
					auto it = m_mapWnd.find(strGalaxyName);
					if (it != m_mapWnd.end())
					{
						int i = 0;
						CString s = _T("");
						s.Format(_T("%s%d"), strGalaxyName, i);
						auto it = m_mapWnd.find(s);
						while (it != m_mapWnd.end())
						{
							i++;
							s.Format(_T("%s%d"), strGalaxyName, i);
							it = m_mapWnd.find(s);
							if (it == m_mapWnd.end())
								break;
						}
						strGalaxyName = s;
					}

				}
				auto it = m_mapGalaxy.find((HWND)h);
				if (it == m_mapGalaxy.end())
					return CreateGalaxy(CComVariant(0), CComVariant((long)h), CComBSTR(strGalaxyName.MakeLower()), pRetFrame);
				else
					*pRetFrame = it->second;
			}
		}
	}
	else if (HostWnd.vt == VT_I2||HostWnd.vt == VT_I4 || HostWnd.vt == VT_I8)
	{
		BOOL bIsMDI = FALSE;
		HWND _hWnd = NULL;
		if(HostWnd.vt == VT_I4)
			_hWnd = (HWND)HostWnd.lVal;
		if(HostWnd.vt == VT_I8)
			_hWnd = (HWND)HostWnd.llVal;
		if (_hWnd == 0)
		{
			_hWnd = ::FindWindowEx(m_hWnd, NULL, _T("MDIClient"), NULL);
			if (_hWnd == NULL)
				_hWnd = ::GetWindow(m_hWnd, GW_CHILD);
			else
				bIsMDI = TRUE;
		}
		if (_hWnd&&::IsWindow(_hWnd))
		{
			auto it = m_mapGalaxy.find(_hWnd);
			if (it == m_mapGalaxy.end())
			{
				DWORD dwID = ::GetWindowThreadProcessId(_hWnd, NULL);
				CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);

				CGalaxy* m_pExtenderGalaxy = new CComObject<CGalaxy>();
				CString strName = strGalaxyName;
				if (strName == _T(""))
					strName = _T("default");
				strName.MakeLower();
				m_pExtenderGalaxy->m_strGalaxyName = strName;
				if (strName.CompareNoCase(_T("System.Windows.Forms.MdiClient")) == 0)
					m_pExtenderGalaxy->m_nGalaxyType = WinFormMDIClientGalaxy;
				else if(bIsMDI)
					m_pExtenderGalaxy->m_nGalaxyType = MDIClientGalaxy;
				::GetClassName(::GetParent(_hWnd), g_pCosmos->m_szBuffer, MAX_PATH);
				CString strClassName = CString(g_pCosmos->m_szBuffer);
				if (strClassName.Find(_T("Afx:ControlBar:")) == 0)
				{
					m_pExtenderGalaxy->m_nGalaxyType = CtrlBarGalaxy;
				}
				else if (strClassName.Find(_T("MDIClient")) == 0)
				{
					m_pExtenderGalaxy->m_nGalaxyType = MDIClientGalaxy;
				}
				m_pExtenderGalaxy->m_pGalaxyCluster = this;
				m_pExtenderGalaxy->m_hHostWnd = _hWnd;
				if (ParentObj.vt == VT_I8 || ParentObj.vt == VT_I4)
				{
					HWND hPWnd = (HWND)ParentObj.llVal;
					if (::IsWindow(hPWnd))
					{
						m_pExtenderGalaxy->m_hPWnd = hPWnd;
					}
				}
				pThreadInfo->m_mapGalaxy[_hWnd] = m_pExtenderGalaxy;
				m_mapGalaxy[_hWnd] = m_pExtenderGalaxy;
				m_mapWnd[strName] = _hWnd;

				for (auto it : g_pCosmos->m_mapCosmosAppProxy)
				{
					CGalaxyProxy* pGalaxyProxy = it.second->OnGalaxyCreated(m_pExtenderGalaxy);
					if (pGalaxyProxy)
					{
						m_pExtenderGalaxy->m_mapGalaxyProxy[it.second] = pGalaxyProxy;
					}
				}

				*pRetFrame = m_pExtenderGalaxy;
			}
			else
				*pRetFrame = it->second;
		}
	}
		
	return S_OK;
}


STDMETHODIMP CGalaxyCluster::GetGalaxyFromCtrl(IDispatch* CtrlObj, IGalaxy** ppGalaxy)
{
	if (g_pCosmos->m_pCLRProxy)
	{
		HWND h = g_pCosmos->m_pCLRProxy->IsGalaxy(CtrlObj);
		if (h)
		{
			auto it = m_mapGalaxy.find(h);
			if (it != m_mapGalaxy.end())
				* ppGalaxy = it->second;
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::GetXobj(BSTR bstrGalaxyName, BSTR bstrNodeName, IXobj** pRetNode)
{
	CString strKey = OLE2T(bstrGalaxyName);
	CString strName = OLE2T(bstrNodeName);
	if (strKey == _T("") || strName == _T(""))
		return S_FALSE;
	auto it = m_mapWnd.find(strKey);
	if (it != m_mapWnd.end())
	{
		HWND hWnd = it->second;
		if (::IsWindow(hWnd))
		{
			auto it = m_mapGalaxy.find(hWnd);
			if (it != m_mapGalaxy.end())
			{
				CGalaxy* pGalaxy = it->second;
				strName = strName.MakeLower();
				auto it2 = pGalaxy->m_mapXobj.find(strName);
				if (it2 != pGalaxy->m_mapXobj.end())
					*pRetNode = (IXobj*)it2->second;
				else
				{
					it2 = m_mapXobj.find(strName);
					if (it2 != m_mapXobj.end())
						*pRetNode = (IXobj*)it2->second;
				}
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_Extender(BSTR bstrExtenderName, IDispatch** pVal)
{
	CString strName = OLE2T(bstrExtenderName);
	if (strName == _T(""))
		return S_OK;
	auto it = m_mapExternalObj.find(strName);
	if (it != m_mapExternalObj.end())
	{
		*pVal = it->second;
		(*pVal)->AddRef();
	}
	return S_OK;
}


STDMETHODIMP CGalaxyCluster::put_Extender(BSTR bstrExtenderName, IDispatch* newVal)
{
	CString strName = OLE2T(bstrExtenderName);
	if (strName == _T(""))
		return S_OK;
	auto it = m_mapExternalObj.find(strName);
	if (it == m_mapExternalObj.end())
	{
		m_mapExternalObj[strName] = newVal;
		newVal->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_GalaxyName(LONGLONG hHwnd, BSTR* pVal)
{
	HWND _hWnd = (HWND)hHwnd;
	if (_hWnd)
	{
		auto it = m_mapGalaxy.find(_hWnd);
		if (it!=m_mapGalaxy.end())
			*pVal = it->second->m_strGalaxyName.AllocSysString();
	}

	return S_OK;
}

//STDMETHODIMP CGalaxyCluster::get_Galaxy(BSTR bstrGalaxyName, IGalaxy** pVal)
//{
//	CString strName = OLE2T(bstrGalaxyName);
//	if (strName != _T(""))
//	{
//		auto it = m_mapWnd.find(strName);
//		if (it != m_mapWnd.end())
//		{
//			HWND h = it->second;
//			auto it2 = m_mapGalaxy.find(h);
//			if (it2 != m_mapGalaxy.end())
//				*pVal = it2->second;
//		}
//	}
//	return S_OK;
//}
//
//void CGalaxyCluster::OnNodeDocComplete(WPARAM wParam)
//{
//	bool bState = false;
//	for (auto it1 : m_mapGalaxy)
//	{
//		for (auto it2 : it1.second->m_mapXobj)
//		{
//			if (it2.second->m_bCreated == false)
//			{
//				::PostMessage(m_hWnd, WM_HUBBLE__WEBNODEDOCCOMPLETE, wParam, 0);
//				return;
//			}
//		}
//	}
//
//	switch (wParam)
//	{
//	case 0:
//		break;
//	case 1:
//	default:
//		break;
//	}
//}

void CGalaxyCluster::UpdateMapKey(CString strXml)
{
	if (m_strXmlHeader != _T(""))
		return;
	CTangramXmlParse m_Parse;
	if (m_Parse.LoadXml(strXml)||m_Parse.LoadFile(strXml))
	{
		strXml = m_Parse.xml();
		int nPos = strXml.Find(_T(">"));
		m_strXmlHeader = strXml.Left(nPos + 1);
		nPos = strXml.ReverseFind('<');
		if (nPos != -1)
			m_strXmlBottom = strXml.Mid(nPos);
		CString strMainKey = _T("tangramdefaultpage");
		int nCount = m_Parse.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CTangramXmlParse* pChild = m_Parse.GetChild(i);
			CString strGalaxyName = pChild->name();
			int nCount = pChild->GetCount();
			for (int i = 0; i < nCount; i++)
			{
				CTangramXmlParse* _pChild = pChild->GetChild(i);
				CString strKey = _pChild->name();
				if (strKey.CompareNoCase(_T("tangram")) == 0)
					strKey = _T("default");
				CString _strKey = strKey + _T("@") + strGalaxyName + _T("@") + strMainKey;
				m_strMapKey[_strKey] = _pChild->xml();
			}
		}
	}
}

void CGalaxyCluster::BeforeDestory()
{
	Fire_Destroy();

	for (auto it : m_mapGalaxy)
		it.second->Destroy();

	if (g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_pCLRProxy->ReleaseCosmosObj((IGalaxyCluster*)this);
	}
}

STDMETHODIMP CGalaxyCluster::get_Xobj(BSTR bstrNodeName, IXobj** pVal)
{
	CString strName = OLE2T(bstrNodeName);
	if (strName == _T(""))
		return S_OK;
	auto it2 = m_mapXobj.find(strName);
	if (it2 == m_mapXobj.end())
		return S_OK;

	if (it2->second)
		*pVal = it2->second;

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_XObject(BSTR bstrName, IDispatch** pVal)
{
	CString strName = OLE2T(bstrName);
	if (strName == _T(""))
		return S_OK;
	auto it2 = m_mapXobj.find(strName);
	if (it2 == m_mapXobj.end())
		return S_OK;
	if (it2->second->m_pDisp)
	{
		*pVal = it2->second->m_pDisp;
		(*pVal)->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_Width(long* pVal)
{
	if (m_hWnd)
	{
		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		*pVal = rc.right - rc.left;
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::put_Width(long newVal)
{
	if (m_hWnd&&newVal)
	{
		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		rc.right = rc.left + newVal;
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, newVal, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_NOREDRAW);
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_Height(long* pVal)
{
	if (m_hWnd)
	{
		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		*pVal = rc.bottom - rc.top;
	}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::put_Height(long newVal)
{
	if (m_hWnd&&newVal)
	{
		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		rc.right = rc.left + newVal;
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, newVal, SWP_NOACTIVATE | SWP_NOREDRAW);
	}
	return S_OK; 
}

STDMETHODIMP CGalaxyCluster::get_XobjNames(BSTR* pVal)
{
	CString strNames = _T("");
	for (auto it : m_mapXobj)
	{
		strNames += it.first;
		strNames += _T(",");
	}
	int nPos = strNames.ReverseFind(',');
	strNames = strNames.Left(nPos);
	*pVal = strNames.AllocSysString();
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_Parent(IGalaxyCluster** pVal)
{
	HWND hWnd = ::GetParent(m_hWnd);
	if (hWnd == NULL)
		return S_OK;

	auto it = g_pCosmos->m_mapWindowPage.find(hWnd);
	while (it == g_pCosmos->m_mapWindowPage.end())
	{
		hWnd = ::GetParent(hWnd);
		if (hWnd == NULL)
			return S_OK;
		it = g_pCosmos->m_mapWindowPage.find(hWnd);
		if (it != g_pCosmos->m_mapWindowPage.end())
		{
			*pVal = it->second;
			return S_OK;
		}
	}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_Handle(LONGLONG* pVal)
{
	if (m_hWnd)
		*pVal = (LONGLONG)m_hWnd;
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::GetCtrlInXobj(BSTR NodeName, BSTR CtrlName, IDispatch** ppCtrl)
{
	CString strName = OLE2T(NodeName);
	if (strName == _T(""))
		return S_OK;
	auto it2 = m_mapXobj.find(strName);
	if (it2 == m_mapXobj.end())
		return S_OK;

	CXobj* pXobj = it2->second;
	if (pXobj)
		pXobj->GetCtrlByName(CtrlName, true, ppCtrl);

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_xtml(BSTR strKey, BSTR* pVal)
{
	map<CString, CString>::iterator it = m_mapXtml.find(OLE2T(strKey));
	if (it != m_mapXtml.end())
		*pVal = it->second.AllocSysString();

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::put_xtml(BSTR strKey, BSTR newVal)
{
	CString strkey = OLE2T(strKey);
	CString strVal = OLE2T(newVal);
	if (strkey == _T("") || strVal == _T(""))
		return S_OK;
	auto it = m_mapXtml.find(strkey);
	if (it != m_mapXtml.end())
		m_mapXtml.erase(it);

	m_mapXtml[strkey] = strVal;
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::Observe(IDispatch* Parent, BSTR CtrlName, BSTR GalaxyName, BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj)
{
	if (g_pCosmos->m_pCLRProxy)
	{
		IDispatch* pDisp = nullptr;
		pDisp =g_pCosmos->m_pCLRProxy->GetCtrlByName(Parent, CtrlName, true);
		if (pDisp)
		{
			HWND h = 0;
			h = g_pCosmos->m_pCLRProxy->IsGalaxy(pDisp);
			if (h)
			{
				CString strGalaxyName = OLE2T(GalaxyName);
				CString strKey = OLE2T(bstrKey);
				if (strGalaxyName == _T(""))
					GalaxyName = CtrlName;
				if (strKey == _T(""))
					bstrKey = CComBSTR(L"Default");
				auto it = m_mapGalaxy.find((HWND)h);
				if (it == m_mapGalaxy.end())
				{
					IGalaxy* pGalaxy = nullptr;
					CreateGalaxy(CComVariant(0), CComVariant((long)h), GalaxyName, &pGalaxy);
					return pGalaxy->Observe(bstrKey, bstrXml, ppRetXobj);
				}
				else
				{
					return it->second->Observe(bstrKey, bstrXml, ppRetXobj);
				}
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::ObserveCtrl(VARIANT MdiForm, BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj)
{
	HWND h = 0;
	bool bMDI = false;
	if (MdiForm.vt == VT_DISPATCH)
	{
		if (g_pCosmos->m_pCLRProxy)
		{
			h = g_pCosmos->m_pCLRProxy->GetMDIClientHandle(MdiForm.pdispVal);
			if (h)
				bMDI = true;
			else
			{
				h = g_pCosmos->m_pCLRProxy->IsGalaxy(MdiForm.pdispVal);
				if (h)
				{
					CComBSTR bstrName(L"");
					bstrName = g_pCosmos->m_pCLRProxy->GetCtrlName(MdiForm.pdispVal);
					CString strKey = OLE2T(bstrKey);
					if (strKey == _T(""))
						bstrKey = CComBSTR(L"Default");
					IGalaxy* pGalaxy = nullptr;
					map<HWND, CGalaxy*>::iterator it = m_mapGalaxy.find((HWND)h);
					if (it == m_mapGalaxy.end())
					{
						CreateGalaxy(CComVariant(0), CComVariant((long)h), bstrName, &pGalaxy);
						return pGalaxy->Observe(bstrKey, bstrXml, ppRetXobj);
					}
					else
					{
						return it->second->Observe(bstrKey, bstrXml, ppRetXobj);
					}
				}
			}
		}
	}
	else if (MdiForm.vt == VT_I4 || MdiForm.vt == VT_I8)
	{
		HWND hWnd = NULL;
		if (MdiForm.vt == VT_I4)
		{
			if (MdiForm.lVal == 0)
			{
				hWnd = ::FindWindowEx(m_hWnd, NULL, _T("MDIClient"), NULL);
				if (hWnd)
				{
					bMDI = true;
				}
				else
				{
					hWnd = ::GetWindow(m_hWnd, GW_CHILD);
				}
			}
			else
			{
				hWnd = (HWND)MdiForm.lVal;
				if (::IsWindow(hWnd) == false)
					hWnd = ::GetWindow(m_hWnd, GW_CHILD);
			}
		}
		if (MdiForm.vt == VT_I8)
		{
			if (MdiForm.llVal == 0)
			{
				hWnd = ::FindWindowEx(m_hWnd, NULL, _T("MDIClient"), NULL);
				if (hWnd)
				{
					bMDI = true;
				}
				else
				{
					hWnd = ::GetWindow(m_hWnd, GW_CHILD);
				}
			}
			else
			{
				hWnd = (HWND)MdiForm.llVal;
				if (::IsWindow(hWnd) == false)
					hWnd = ::GetWindow(m_hWnd, GW_CHILD);

			}
		}
		h = hWnd;
	}
	if (h)
	{
		CString strKey = OLE2T(bstrKey);
		if (strKey == _T(""))
			bstrKey = CComBSTR(L"Default");
		IGalaxy* pGalaxy = nullptr;
		if (bMDI)
		{
			HRESULT hr = CreateGalaxy(CComVariant(0), CComVariant((long)h), CComBSTR(L"TangramMDIClientGalaxy"), &pGalaxy);
			if (pGalaxy)
			{
				return pGalaxy->Observe(bstrKey, bstrXml, ppRetXobj);
			}
		}
		else
		{
			CString strKey = OLE2T(bstrKey);
			if (strKey == _T(""))
				bstrKey = CComBSTR(L"Default");

			IGalaxy* pGalaxy = nullptr;
			auto it = m_mapGalaxy.find((HWND)h);
			if (it == m_mapGalaxy.end())
			{
				TCHAR szBuffer[MAX_PATH];
				::GetWindowText((HWND)h, szBuffer, MAX_PATH);
				CString strText = szBuffer;
				if (strText == _T(""))
				{
					CString s = _T("");
					s.Format(_T("Frame_%p"), h);
					strText = s;
				}
				CreateGalaxy(CComVariant(0), CComVariant((long)h), CComBSTR(strText), &pGalaxy);
				return pGalaxy->Observe(bstrKey, bstrXml, ppRetXobj);
			}
			else
			{
				return it->second->Observe(bstrKey, bstrXml, ppRetXobj);
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::ConnectCosmosCtrl(ICosmosCtrl* eventSource)
{
	if (g_pCosmos->m_bEclipse)
	{
		if (eventSource != nullptr)
		{
			CEclipseCtrl* pCtrl = (CEclipseCtrl*)eventSource;
			HWND hCtrl = pCtrl->m_hWnd;
			auto it = m_mapNotifyCtrl.find(hCtrl);
			if (it == m_mapNotifyCtrl.end())
				m_mapNotifyCtrl[hCtrl] = pCtrl;
		}
	}
	return S_OK;
}

HRESULT CGalaxyCluster::Fire_GalaxyClusterLoaded(IDispatch* sender, BSTR url)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[2];
		avarParams[1] = sender;
		avarParams[1].vt = VT_DISPATCH;
		avarParams[0] = url;
		avarParams[0].vt = VT_BSTR;

		DISPPARAMS params = { avarParams, NULL, 2, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			auto it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_GalaxyClusterLoaded(sender, url);
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnGalaxyClusterLoaded(sender, OLE2T(url));
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_NodeCreated(IXobj * pXobjCreated)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[1];
		avarParams[0] = pXobjCreated;
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
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			auto it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_NodeCreated(pXobjCreated);
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnXobjCreated(pXobjCreated);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_AddInCreated(IXobj * pRootXobj, IDispatch * pAddIn, BSTR bstrID, BSTR bstrAddInXml)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[4];
		avarParams[3] = pRootXobj;
		avarParams[3].vt = VT_DISPATCH;
		avarParams[2] = pAddIn;
		avarParams[2].vt = VT_DISPATCH;
		avarParams[1] = bstrID;
		avarParams[1].vt = VT_BSTR;
		avarParams[0] = bstrAddInXml;
		avarParams[0].vt = VT_BSTR;

		DISPPARAMS params = { avarParams, NULL, 4, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			auto it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_AddInCreated(pRootXobj, pAddIn, bstrID, bstrAddInXml);
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnAddInCreated(pRootXobj, pAddIn, OLE2T(bstrID), OLE2T(bstrAddInXml));
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_BeforeOpenXml(BSTR bstrXml, LONGLONG hWnd)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{		
		CComVariant avarParams[2];
		avarParams[1] = bstrXml;
		avarParams[1].vt = VT_BSTR;
		avarParams[0] = hWnd;
		avarParams[0].vt = VT_I8;

		DISPPARAMS params = { avarParams, NULL, 2, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			auto it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_BeforeOpenXml(bstrXml, hWnd);
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnBeforeOpenXml(OLE2T(bstrXml), (HWND)hWnd);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_OpenXmlComplete(BSTR bstrXml, LONGLONG hWnd, IXobj * pRetRootNode)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[3];
		avarParams[2] = bstrXml;
		avarParams[2].vt = VT_BSTR;
		avarParams[1] = hWnd;
		avarParams[1].vt = VT_I8;
		avarParams[0] = pRetRootNode;
		avarParams[0].vt = VT_DISPATCH;

		DISPPARAMS params = { avarParams, NULL, 3, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			auto it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_OpenXmlComplete(bstrXml, hWnd, pRetRootNode);
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnOpenXmlComplete(OLE2T(bstrXml), (HWND)hWnd, pRetRootNode);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_Destroy()
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		DISPPARAMS params = { NULL, NULL, 0, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			auto it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_Destroy();
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnDestroy();
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_NodeMouseActivate(IXobj * pActiveNode)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[1];
		avarParams[0] = pActiveNode;
		avarParams[0].vt = VT_DISPATCH;
		DISPPARAMS params = { avarParams, NULL, 1, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			map<HWND, CEclipseCtrl*>::iterator it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_NodeMouseActivate(pActiveNode);
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnNodeMouseActivate(pActiveNode);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_ClrControlCreated(IXobj * Node, IDispatch * Ctrl, BSTR CtrlName, LONGLONG CtrlHandle)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[4];
		avarParams[3] = Node;
		avarParams[3].vt = VT_DISPATCH;
		avarParams[2] = Ctrl;
		avarParams[2].vt = VT_DISPATCH;
		avarParams[1] = CtrlName;
		avarParams[1].vt = VT_BSTR;
		avarParams[0] = CtrlHandle;
		avarParams[0].vt = VT_I8;
		DISPPARAMS params = { avarParams, NULL, 4, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(8, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			auto it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_ClrControlCreated(Node, Ctrl, CtrlName, CtrlHandle);
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnClrControlCreated(Node, Ctrl, OLE2T(CtrlName), (HWND)CtrlHandle);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_IPCMsg(IGalaxy* pSender, BSTR bstrType, BSTR bstrContent, BSTR bstrFeature)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[4];
		avarParams[3] = pSender;
		avarParams[3].vt = VT_DISPATCH;
		avarParams[2] = bstrType;
		avarParams[2].vt = VT_BSTR;
		avarParams[1] = bstrContent;
		avarParams[1].vt = VT_BSTR;
		avarParams[0] = bstrFeature;
		avarParams[0].vt = VT_BSTR;
		DISPPARAMS params = { avarParams, NULL, 4, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(13, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	return hr;
}

HRESULT CGalaxyCluster::Fire_TabChange(IXobj* sender, LONG ActivePage, LONG OldPage)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[3];
		avarParams[2] = sender;
		avarParams[2].vt = VT_DISPATCH;
		avarParams[1] = ActivePage;
		avarParams[1].vt = VT_I4;
		avarParams[0] = OldPage;
		avarParams[0].vt = VT_I4;
		DISPPARAMS params = { avarParams, NULL, 3, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(9, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse)
	{
		int nSize = m_mapNotifyCtrl.size();
		if (nSize)
		{
			auto it = m_mapNotifyCtrl.begin();
			map<HWND, CEclipseCtrl*> m_mapTemp;
			for (it = m_mapNotifyCtrl.begin(); it != m_mapNotifyCtrl.end(); it++)
			{
				CEclipseCtrl* pCtrl = it->second;
				if (::IsWindow(it->first))
				{
					it->second->Fire_TabChange(sender, ActivePage, OldPage);
				}
				else
				{
					m_mapTemp[it->first] = it->second;
				}
			}
			nSize = m_mapTemp.size();
			if (nSize)
			{
				for (it = m_mapTemp.begin(); it != m_mapTemp.end(); it++)
				{
					auto it2 = m_mapNotifyCtrl.find(it->first);
					if (it2 != m_mapNotifyCtrl.end())
						m_mapNotifyCtrl.erase(it2);
				}
				m_mapTemp.clear();
			}
		}
	}
	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnTabChange(sender, ActivePage, OldPage);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_CosmosEvent(ICosmosEventObj* pEventObj)
{
	CCosmosEvent* pEvent = (CCosmosEvent*)pEventObj;
	m_mapEventObj[(__int64)pEvent] = pEvent;
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
				hr = pConnection->Invoke(12, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	return hr;
}

STDMETHODIMP CGalaxyCluster::get_GalaxyClusterXML(BSTR* pVal)
{
	CString strData = m_strXmlHeader;
	if (strData == _T(""))
		strData = _T("<tangramdefaultpage>");
	CString strIndex = _T("@");
	for (auto it : m_mapGalaxy)
	{
		CComBSTR bstrXml(L"");
		strIndex += it.second->m_strGalaxyName;
		strIndex += _T("@");
		it.second->get_GalaxyXML(&bstrXml);
		strData += OLE2T(bstrXml);
	}
	map<CString, CString> m_mapTemp;
	for (auto it : m_strMapKey)
	{
		CString strKey = it.first;
		int nPos = strKey.Find(_T("@"));
		if (nPos != -1)
		{
			strKey = strKey.Mid(nPos);
			nPos = strKey.ReverseFind('@');
			if (nPos != -1)
			{
				strKey = strKey.Left(nPos + 1);
				if (strIndex.Find(strKey) == -1)
				{
					strKey.Replace(_T("@"), _T(""));
					auto it2 = m_mapTemp.find(strKey);
					if (strKey != _T(""))
					{
						if (it2 == m_mapTemp.end())
						{
							m_mapTemp[strKey] = it.second;
						}
						else
						{
							m_mapTemp[strKey] = it2->second+it.second;
						}
					}
				}
			}
		}
	}
	for (auto it : m_mapTemp)
	{
		CString strXml = _T("");
		strXml.Format(_T("<%s>%s</%s>"), it.first, it.second, it.first);
		strData += strXml;
	}
	if (m_strXmlBottom != _T(""))
		strData += m_strXmlBottom;
	else
		strData += _T("</tangramdefaultpage>");
	*pVal = CComBSTR(strData);
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::put_ConfigName(BSTR newVal)
{
	m_strConfigFileNodeName = OLE2T(newVal);
	m_strConfigFileNodeName.Trim();
	m_strConfigFileNodeName.MakeLower();
	if (m_strConfigFileNodeName == _T(""))
		m_strConfigFileNodeName = _T("default");
	if (m_strConfigFileNodeName.Find(_T(" ")))
		m_strConfigFileNodeName.Replace(_T(" "), _T("_"));

	if (m_strPageFileName == _T("")&&::GetParent(m_hWnd)==nullptr)
	{
		m_strPageFileName = g_pCosmos->m_strExeName;
		m_strPageFilePath = g_pCosmos->m_strConfigDataFile;
	}
	
	if (::PathFileExists(m_strPageFilePath))
	{
		CTangramXmlParse m_Parse2;
		if (m_Parse2.LoadFile(m_strPageFilePath))
		{
			CTangramXmlParse* m_pCosmosPageParse = m_Parse2.GetChild(_T("hubblepage"));
			if (m_pCosmosPageParse == nullptr)
			{
				m_Parse2.AddNode(_T("hubblepage"));
				m_pCosmosPageParse = m_Parse2.GetChild(_T("hubblepage"));
			}
			if (m_pCosmosPageParse)
			{
				CTangramXmlParse* m_pCosmosPageParse2 = m_pCosmosPageParse->GetChild(m_strConfigFileNodeName);
				if (m_pCosmosPageParse2 == nullptr)
				{
					if(m_pCosmosPageParse->AddNode(m_strConfigFileNodeName)!=nullptr)
						m_Parse2.SaveFile(m_strPageFilePath);
				}
				if (m_pCosmosPageParse2)
				{
					int nCount = m_pCosmosPageParse2->GetCount();
					for (int i = 0; i < nCount; i++)
					{
						CTangramXmlParse* _pParse = m_pCosmosPageParse2->GetChild(i);
						CString _str = _T("@") + _pParse->name() + _T("@") + m_strConfigFileNodeName;
						int nCount2 = _pParse->GetCount();
						for (int i = 0; i < nCount2; i++)
						{
							CTangramXmlParse* _pParse2 = _pParse->GetChild(i);
							m_strMapKey[_pParse2->name() + _str] = _pParse2->xml();
						}
					}
				}
			}
		}
	}
	else
	{
		if (::PathIsDirectory(g_pCosmos->m_strAppDataPath) == false)
		{
			if (::SHCreateDirectoryEx(NULL, g_pCosmos->m_strAppDataPath, NULL))
				return S_FALSE;
		}
		CString strXml = _T("");
		strXml.Format(_T("<%s><hubblepage><%s /></hubblepage></%s>"), m_strPageFileName, m_strConfigFileNodeName, m_strPageFileName);
		CTangramXmlParse m_Parse2;
		m_Parse2.LoadXml(strXml);
		m_Parse2.SaveFile(m_strPageFilePath);
	}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::CreateGalaxyWithDefaultNode(ULONGLONG hFrameWnd, BSTR bstrGalaxyName, BSTR bstrDefaultNodeKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfig, IXobj** ppXobj)
{
	CString strXml = OLE2T(bstrXml);
	strXml.Trim();
	if (strXml == _T(""))
		strXml = _T("<default><cluster><xobj name=\"Start\" /></cluster></default>");
	IGalaxy* pGalaxy = nullptr;
	CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)hFrameWnd), bstrGalaxyName, &pGalaxy);
	if (pGalaxy)
	{
		pGalaxy->Observe(bstrDefaultNodeKey, strXml.AllocSysString(), ppXobj);
		if (*ppXobj&&bSaveToConfig)
		{
			(*ppXobj)->put_SaveToConfigFile(true);
			//CGalaxy* pFrame2 = (CGalaxy*)::SendMessage(((CGalaxy*)pGalaxy)->m_hWnd, WM_HUBBLE_DATA, 0, 1992);
			//if (pGalaxy)
			//{
			//	pFrame2->m_nGalaxyType = EclipseWorkBenchGalaxy;
			//	//pGalaxy->m_pWorkBenchFrame = this;
			//}
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::ObserveGalaxys(BSTR bstrGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfigFile)
{
	CString strGalaxys = OLE2T(bstrGalaxys);
	CString strKey = OLE2T(bstrKey);
	CString strXml = OLE2T(bstrXml);
	if (strGalaxys == _T(""))
	{
		for (auto it : m_mapGalaxy)
		{
			if (it.second != m_pBKGalaxy)
			{
				IXobj* pXobj = nullptr;
				it.second->Observe(bstrKey, bstrXml, &pXobj);
				if (pXobj&&bSaveToConfigFile)
					pXobj->put_SaveToConfigFile(true);
			}
		}
	}
	else
	{
		strGalaxys = _T(",") + strGalaxys;
		for (auto it : m_mapGalaxy)
		{
			CString strName = _T(",") + it.second->m_strGalaxyName + _T(",");
			if (strGalaxys.Find(strName) != -1)
			{
				IXobj* pXobj = nullptr;
				it.second->Observe(bstrKey, bstrXml, &pXobj);
				if (pXobj&&bSaveToConfigFile)
					pXobj->put_SaveToConfigFile(true);
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_CurrentDesignGalaxyType(GalaxyType* pVal)
{
	if (g_pCosmos->m_pDesignXobj)
	{
		CGalaxy* pGalaxy = g_pCosmos->m_pDesignXobj->m_pXobjShareData->m_pGalaxy;
		*pVal = pGalaxy->m_nGalaxyType;
	}
	else
		*pVal = NOGalaxy;

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_CurrentDesignNode(IXobj** pVal)
{
	if (g_pCosmos->m_pDesignXobj)
		*pVal = g_pCosmos->m_pDesignXobj;

	return S_OK;
}
