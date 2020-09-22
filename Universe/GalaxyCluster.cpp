/********************************************************************************
*					Open Universe - version 0.9.9								*
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

// TangramObject.cpp : Implementation of CGalaxyCluster

#include "stdafx.h"
#include "grid.h"
#include "Quasar.h"
#include "UniverseApp.h"
#include "Hubble.h"

CGridCommonData::CGridCommonData()
{
	m_pOldQuasar		= nullptr;
	m_pHubbleParse		= nullptr;
	m_pHostClientView	= nullptr;
#ifdef _DEBUG
	g_pHubble->m_nTangramNodeCommonData++;
#endif	
};

CGridCommonData::~CGridCommonData()
{
	if (m_pHubbleParse)
		delete m_pHubbleParse;
#ifdef _DEBUG
	g_pHubble->m_nTangramNodeCommonData--;
#endif	
};

// CGalaxyCluster
CGalaxyCluster::CGalaxyCluster()
{
	m_hWnd								= 0;
	m_bIsBlank							= false;
	m_bPageDataLoaded					= false;
	m_bIsDestory						= false;
	m_bDocComplete						= false;
	m_bDoc								= false;
	m_strXmlHeader						= _T("");
	m_strPageFileName					= _T("");
	m_strPageFilePath					= _T("");
	m_strConfigFileNodeName				= _T("tangramdefaultpage");
	m_pBKFrame							= nullptr;
	g_pHubble->m_pGalaxyCluster			= this;
	m_pUniverseAppProxy					= nullptr;
#ifdef _DEBUG
	g_pHubble->m_nTangram++;
#endif	
}

CGalaxyCluster::~CGalaxyCluster()
{
#ifdef _DEBUG
	g_pHubble->m_nTangram--;
#endif	

	for (auto it2 : m_mapExternalObj)
	{
		it2.second->Release();
	}
	m_mapExternalObj.erase(m_mapExternalObj.begin(), m_mapExternalObj.end());

	m_mapQuasar.erase(m_mapQuasar.begin(), m_mapQuasar.end());
	m_mapGrid.erase(m_mapGrid.begin(), m_mapGrid.end());
	auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
	if (it != g_pHubble->m_mapWindowPage.end())
	{
		g_pHubble->m_mapWindowPage.erase(it);
	}
	if (g_pHubble->m_mapWindowPage.size() == 0)
		g_pHubble->Close();

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
	*pCount = (long)m_mapQuasar.size();
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_Quasar(VARIANT vIndex, IQuasar ** ppQuasar)
{
	if (vIndex.vt == VT_I4)
	{
		long lCount = m_mapQuasar.size();
		int iIndex = vIndex.lVal;
		HWND hWnd = (HWND)iIndex;
		if (::IsWindow(hWnd))
		{
			auto it = m_mapQuasar.find(hWnd);
			if (it != m_mapQuasar.end())
			{
				*ppQuasar = it->second;
				return S_OK;
			}
		}
		if (iIndex < 0 || iIndex >= lCount) return E_INVALIDARG;

		auto it = m_mapQuasar.begin();
		int iPos = 0;
		while (it != m_mapQuasar.end())
		{
			if (iPos == iIndex)
			{
				*ppQuasar = it->second;
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
			auto it2 = m_mapQuasar.find(it->second);
			if (it2 != m_mapQuasar.end())
			{
				*ppQuasar = it2->second;
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
		long nLen = (long)m_mapQuasar.size();
		VARIANT* rgvar = new VARIANT[nLen];
		ZeroMemory(rgvar, sizeof(VARIANT)*nLen);
		VARIANT* pItem = rgvar;
		for (auto it : m_mapQuasar)
		{
			IUnknown* pDisp = nullptr;
			CQuasar* pObj = it.second;
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

STDMETHODIMP CGalaxyCluster::CreateQuasar(VARIANT ParentObj, VARIANT HostWnd, BSTR bstrFrameName, IQuasar** pRetFrame)
{
	HWND h = 0; 
	CString strFrameName = OLE2T(bstrFrameName);
	//auto it = m_mapWnd.find(strFrameName);
	//if (it != m_mapWnd.end())
	//{
	//	int i = 0;
	//	CString s = _T("");
	//	s.Format(_T("%s%d"), strFrameName,i);
	//	auto it = m_mapWnd.find(s);
	//	while (it != m_mapWnd.end())
	//	{
	//		i++;
	//		s.Format(_T("%s%d"), strFrameName, i);
	//		it = m_mapWnd.find(s);
	//		if (it == m_mapWnd.end())
	//			break;
	//	}
	//	strFrameName = s;
	//}
	BSTR bstrName = strFrameName.MakeLower().AllocSysString();
	if (ParentObj.vt == VT_DISPATCH&&HostWnd.vt == VT_BSTR)
	{
		if (g_pHubble->m_pCLRProxy)
		{
			IDispatch* pDisp = nullptr;
			pDisp = g_pHubble->m_pCLRProxy->GetCtrlByName(ParentObj.pdispVal, HostWnd.bstrVal, true);
			if (pDisp)
			{
				h = g_pHubble->m_pCLRProxy->GetCtrlHandle(pDisp);
				if (h)
				{
					HRESULT hr = CreateQuasar(CComVariant(0), CComVariant((long)h), bstrName, pRetFrame);
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
		if (g_pHubble->m_pCLRProxy)
		{
			h = g_pHubble->m_pCLRProxy->IsCtrlCanNavigate(HostWnd.pdispVal);
			if (h)
			{
				CString strName = strFrameName;
				if (strName == _T(""))
				{
					::SysFreeString(bstrName);
					bstrFrameName = g_pHubble->m_pCLRProxy->GetCtrlName(HostWnd.pdispVal);
					strName = OLE2T(bstrFrameName);
					if (strName == _T(""))
						bstrFrameName = CComBSTR(L"Default");
					
					strFrameName = OLE2T(bstrFrameName);
					auto it = m_mapWnd.find(strFrameName);
					if (it != m_mapWnd.end())
					{
						int i = 0;
						CString s = _T("");
						s.Format(_T("%s%d"), strFrameName, i);
						auto it = m_mapWnd.find(s);
						while (it != m_mapWnd.end())
						{
							i++;
							s.Format(_T("%s%d"), strFrameName, i);
							it = m_mapWnd.find(s);
							if (it == m_mapWnd.end())
								break;
						}
						strFrameName = s;
					}

				}
				auto it = m_mapQuasar.find((HWND)h);
				if (it == m_mapQuasar.end())
					return CreateQuasar(CComVariant(0), CComVariant((long)h), CComBSTR(strFrameName.MakeLower()), pRetFrame);
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
			auto it = m_mapQuasar.find(_hWnd);
			if (it == m_mapQuasar.end())
			{
				DWORD dwID = ::GetWindowThreadProcessId(_hWnd, NULL);
				CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);

				CQuasar* m_pExtenderQuasar = new CComObject<CQuasar>();
				CString strName = strFrameName;
				if (strName == _T(""))
					strName = _T("default");
				strName.MakeLower();
				m_pExtenderQuasar->m_strQuasarName = strName;
				if (strName.CompareNoCase(_T("System.Windows.Forms.MdiClient")) == 0)
					m_pExtenderQuasar->m_nQuasarType = WinFormMDIClientQuasar;
				else if(bIsMDI)
					m_pExtenderQuasar->m_nQuasarType = MDIClientQuasar;
				::GetClassName(::GetParent(_hWnd), g_pHubble->m_szBuffer, MAX_PATH);
				CString strClassName = CString(g_pHubble->m_szBuffer);
				if (strClassName.Find(_T("MDIClient")) == 0)
				{
					m_pExtenderQuasar->m_nQuasarType = MDIClientQuasar;
				}
				m_pExtenderQuasar->m_pGalaxyCluster = this;
				m_pExtenderQuasar->m_hHostWnd = _hWnd;
				if (ParentObj.vt == VT_I8 || ParentObj.vt == VT_I4)
				{
					HWND hPWnd = (HWND)ParentObj.llVal;
					if (::IsWindow(hPWnd))
					{
						m_pExtenderQuasar->m_hPWnd = hPWnd;
					}
				}
				pThreadInfo->m_mapQuasar[_hWnd] = m_pExtenderQuasar;
				m_mapQuasar[_hWnd] = m_pExtenderQuasar;
				m_mapWnd[strName] = _hWnd;

				for (auto it : g_pHubble->m_mapHubbleAppProxy)
				{
					CQuasarProxy* pQuasarProxy = it.second->OnQuasarCreated(m_pExtenderQuasar);
					if (pQuasarProxy)
					{
						m_pExtenderQuasar->m_mapQuasarProxy[it.second] = pQuasarProxy;
					}
				}

				*pRetFrame = m_pExtenderQuasar;
			}
			else
				*pRetFrame = it->second;
		}
	}
		
	return S_OK;
}


STDMETHODIMP CGalaxyCluster::GetQuasarFromCtrl(IDispatch* CtrlObj, IQuasar** ppQuasar)
{
	if (g_pHubble->m_pCLRProxy)
	{
		HWND h = g_pHubble->m_pCLRProxy->IsCtrlCanNavigate(CtrlObj);
		if (h)
		{
			auto it = m_mapQuasar.find(h);
			if (it != m_mapQuasar.end())
				* ppQuasar = it->second;
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::GetGrid(BSTR bstrFrameName, BSTR bstrNodeName, IGrid** pRetNode)
{
	CString strKey = OLE2T(bstrFrameName);
	CString strName = OLE2T(bstrNodeName);
	if (strKey == _T("") || strName == _T(""))
		return S_FALSE;
	auto it = m_mapWnd.find(strKey);
	if (it != m_mapWnd.end())
	{
		HWND hWnd = it->second;
		if (::IsWindow(hWnd))
		{
			auto it = m_mapQuasar.find(hWnd);
			if (it != m_mapQuasar.end())
			{
				CQuasar* pQuasar = it->second;
				strName = strName.MakeLower();
				auto it2 = pQuasar->m_mapGrid.find(strName);
				if (it2 != pQuasar->m_mapGrid.end())
					*pRetNode = (IGrid*)it2->second;
				else
				{
					it2 = m_mapGrid.find(strName);
					if (it2 != m_mapGrid.end())
						*pRetNode = (IGrid*)it2->second;
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

STDMETHODIMP CGalaxyCluster::get_QuasarName(LONGLONG hHwnd, BSTR* pVal)
{
	HWND _hWnd = (HWND)hHwnd;
	if (_hWnd)
	{
		auto it = m_mapQuasar.find(_hWnd);
		if (it!=m_mapQuasar.end())
			*pVal = it->second->m_strQuasarName.AllocSysString();
	}

	return S_OK;
}

//STDMETHODIMP CGalaxyCluster::get_Quasar(BSTR bstrFrameName, IQuasar** pVal)
//{
//	CString strName = OLE2T(bstrFrameName);
//	if (strName != _T(""))
//	{
//		auto it = m_mapWnd.find(strName);
//		if (it != m_mapWnd.end())
//		{
//			HWND h = it->second;
//			auto it2 = m_mapQuasar.find(h);
//			if (it2 != m_mapQuasar.end())
//				*pVal = it2->second;
//		}
//	}
//	return S_OK;
//}

void CGalaxyCluster::OnNodeDocComplete(WPARAM wParam)
{
	bool bState = false;
	for (auto it1 : m_mapQuasar)
	{
		for (auto it2 : it1.second->m_mapGrid)
		{
			if (it2.second->m_bCreated == false)
			{
				::PostMessage(m_hWnd, WM_TANGRAM_WEBNODEDOCCOMPLETE, wParam, 0);
				return;
			}
		}
	}

	switch (wParam)
	{
	case 0:
		break;
	case 1:
	default:
		break;
	}
}

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
			CString strFrameName = pChild->name();
			int nCount = pChild->GetCount();
			for (int i = 0; i < nCount; i++)
			{
				CTangramXmlParse* _pChild = pChild->GetChild(i);
				CString strKey = _pChild->name();
				if (strKey.CompareNoCase(_T("tangram")) == 0)
					strKey = _T("default");
				CString _strKey = strKey + _T("@") + strFrameName + _T("@") + strMainKey;
				m_strMapKey[_strKey] = _pChild->xml();
			}
		}

		if (m_bPageDataLoaded == false)
		{
			m_bPageDataLoaded = true;
			CTangramXmlParse* pParse = (CTangramXmlParse*)m_Parse.GetChild(_T("pagedata"));
			if (pParse&&::PathFileExists(g_pHubble->m_strTemplatePath) == TRUE)
			{
				int nCount = pParse->GetCount();
				if (nCount)
				{
					for (int i = 0; i < nCount; i++)
					{
						CString strPath = g_pHubble->m_strTemplatePath;
						CTangramXmlParse* pChild = pParse->GetChild(i);
						CString strName = pChild->name();
						m_mapXtml[strName] = pChild->xml();
						int nPos = strPath.ReverseFind('\\');
						CString strFileName = strPath.Mid(nPos + 1);
						int nPos1 = strFileName.ReverseFind('.');
						strFileName = strFileName.Left(nPos1);
						strPath = strPath.Left(nPos + 1) + strFileName + _T("_") + strName + _T(".bmp");
						if (::PathFileExists(strPath))
						{
							m_mapXtml[strName + _T("_imagePath")] = strPath;
							CString strData = g_pHubble->EncodeFileToBase64(strPath);
							m_mapXtml[strName + _T("_image")] = strData;
						}
					}
				}
			}
		}
	}
}

void CGalaxyCluster::BeforeDestory()
{
	if (!m_bIsDestory)
	{
		m_bIsDestory = true;
		Fire_Destroy();

		for (auto it: m_mapQuasar)
			it.second->Destroy();

		if (g_pHubble->m_pCLRProxy)
		{
			g_pHubble->m_pCLRProxy->ReleaseHubbleObj((IGalaxyCluster*)this);
		}
	}
}

STDMETHODIMP CGalaxyCluster::get_Grid(BSTR bstrNodeName, IGrid** pVal)
{
	CString strName = OLE2T(bstrNodeName);
	if (strName == _T(""))
		return S_OK;
	auto it2 = m_mapGrid.find(strName);
	if (it2 == m_mapGrid.end())
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
	auto it2 = m_mapGrid.find(strName);
	if (it2 == m_mapGrid.end())
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

STDMETHODIMP CGalaxyCluster::get_GridNames(BSTR* pVal)
{
	CString strNames = _T("");
	for (auto it : m_mapGrid)
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

	auto it = g_pHubble->m_mapWindowPage.find(hWnd);
	while (it == g_pHubble->m_mapWindowPage.end())
	{
		hWnd = ::GetParent(hWnd);
		if (hWnd == NULL)
			return S_OK;
		it = g_pHubble->m_mapWindowPage.find(hWnd);
		if (it != g_pHubble->m_mapWindowPage.end())
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

STDMETHODIMP CGalaxyCluster::GetCtrlInGrid(BSTR NodeName, BSTR CtrlName, IDispatch** ppCtrl)
{
	CString strName = OLE2T(NodeName);
	if (strName == _T(""))
		return S_OK;
	auto it2 = m_mapGrid.find(strName);
	if (it2 == m_mapGrid.end())
		return S_OK;

	CGrid* pGrid = it2->second;
	if (pGrid)
		pGrid->GetCtrlByName(CtrlName, true, ppCtrl);

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

STDMETHODIMP CGalaxyCluster::Observe(IDispatch* Parent, BSTR CtrlName, BSTR FrameName, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid)
{
	if (g_pHubble->m_pCLRProxy)
	{
		IDispatch* pDisp = nullptr;
		pDisp =g_pHubble->m_pCLRProxy->GetCtrlByName(Parent, CtrlName, true);
		if (pDisp)
		{
			HWND h = 0;
			h = g_pHubble->m_pCLRProxy->IsCtrlCanNavigate(pDisp);
			if (h)
			{
				CString strFrameName = OLE2T(FrameName);
				CString strKey = OLE2T(bstrKey);
				if (strFrameName == _T(""))
					FrameName = CtrlName;
				if (strKey == _T(""))
					bstrKey = CComBSTR(L"Default");
				auto it = m_mapQuasar.find((HWND)h);
				if (it == m_mapQuasar.end())
				{
					IQuasar* pQuasar = nullptr;
					CreateQuasar(CComVariant(0), CComVariant((long)h), FrameName, &pQuasar);
					return pQuasar->Observe(bstrKey, bstrXml, ppRetGrid);
				}
				else
				{
					return it->second->Observe(bstrKey, bstrXml, ppRetGrid);
				}
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::ObserveCtrl(VARIANT MdiForm, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid)
{
	HWND h = 0;
	bool bMDI = false;
	if (MdiForm.vt == VT_DISPATCH)
	{
		if (g_pHubble->m_pCLRProxy)
		{
			h = g_pHubble->m_pCLRProxy->GetMDIClientHandle(MdiForm.pdispVal);
			if (h)
				bMDI = true;
			else
			{
				h = g_pHubble->m_pCLRProxy->IsCtrlCanNavigate(MdiForm.pdispVal);
				if (h)
				{
					CComBSTR bstrName(L"");
					bstrName = g_pHubble->m_pCLRProxy->GetCtrlName(MdiForm.pdispVal);
					CString strKey = OLE2T(bstrKey);
					if (strKey == _T(""))
						bstrKey = CComBSTR(L"Default");
					IQuasar* pQuasar = nullptr;
					map<HWND, CQuasar*>::iterator it = m_mapQuasar.find((HWND)h);
					if (it == m_mapQuasar.end())
					{
						CreateQuasar(CComVariant(0), CComVariant((long)h), bstrName, &pQuasar);
						return pQuasar->Observe(bstrKey, bstrXml, ppRetGrid);
					}
					else
					{
						return it->second->Observe(bstrKey, bstrXml, ppRetGrid);
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
		IQuasar* pQuasar = nullptr;
		if (bMDI)
		{
			HRESULT hr = CreateQuasar(CComVariant(0), CComVariant((long)h), CComBSTR(L"TangramMDIClientQuasar"), &pQuasar);
			if (pQuasar)
			{
				return pQuasar->Observe(bstrKey, bstrXml, ppRetGrid);
			}
		}
		else
		{
			CString strKey = OLE2T(bstrKey);
			if (strKey == _T(""))
				bstrKey = CComBSTR(L"Default");

			IQuasar* pQuasar = nullptr;
			auto it = m_mapQuasar.find((HWND)h);
			if (it == m_mapQuasar.end())
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
				CreateQuasar(CComVariant(0), CComVariant((long)h), CComBSTR(strText), &pQuasar);
				return pQuasar->Observe(bstrKey, bstrXml, ppRetGrid);
			}
			else
			{
				return it->second->Observe(bstrKey, bstrXml, ppRetGrid);
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::ConnectHubbleCtrl(IHubbleCtrl* eventSource)
{
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
			g_pHubble->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnGalaxyClusterLoaded(sender, OLE2T(url));
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_NodeCreated(IGrid * pGridCreated)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[1];
		avarParams[0] = pGridCreated;
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
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnGridCreated(pGridCreated);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_AddInCreated(IGrid * pRootGrid, IDispatch * pAddIn, BSTR bstrID, BSTR bstrAddInXml)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[4];
		avarParams[3] = pRootGrid;
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnAddInCreated(pRootGrid, pAddIn, OLE2T(bstrID), OLE2T(bstrAddInXml));
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnBeforeOpenXml(OLE2T(bstrXml), (HWND)hWnd);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_OpenXmlComplete(BSTR bstrXml, LONGLONG hWnd, IGrid * pRetRootNode)
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnDestroy();
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_NodeMouseActivate(IGrid * pActiveNode)
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
			g_pHubble->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnNodeMouseActivate(pActiveNode);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_ClrControlCreated(IGrid * Node, IDispatch * Ctrl, BSTR CtrlName, LONGLONG CtrlHandle)
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(8, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnClrControlCreated(Node, Ctrl, OLE2T(CtrlName), (HWND)CtrlHandle);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_IPCMsg(IQuasar* pSender, BSTR bstrType, BSTR bstrContent, BSTR bstrFeature)
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

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

HRESULT CGalaxyCluster::Fire_TabChange(IGrid* sender, LONG ActivePage, LONG OldPage)
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection);
			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(9, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapGalaxyClusterProxy)
	{
		it.second->OnTabChange(sender, ActivePage, OldPage);
	}
	return hr;
}

HRESULT CGalaxyCluster::Fire_TangramEvent(IHubbleEventObj* pEventObj)
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
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

STDMETHODIMP CGalaxyCluster::get_NewVersion(VARIANT_BOOL* bNewVersion)
{
	*bNewVersion = m_bNewVersion;
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::put_NewVersion(VARIANT_BOOL bNewVersion)
{
	m_bNewVersion = bNewVersion;
	return S_OK;
}


STDMETHODIMP CGalaxyCluster::get_GalaxyClusterXML(BSTR* pVal)
{
	CString strData = m_strXmlHeader;
	if (strData == _T(""))
		strData = _T("<tangramdefaultpage>");
	CString strIndex = _T("@");
	for (auto it : m_mapQuasar)
	{
		CComBSTR bstrXml(L"");
		strIndex += it.second->m_strQuasarName;
		strIndex += _T("@");
		it.second->get_QuasarXML(&bstrXml);
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
		m_strPageFileName = g_pHubble->m_strExeName;
		m_strPageFilePath = g_pHubble->m_strConfigDataFile;
	}
	
	if (::PathFileExists(m_strPageFilePath))
	{
		CTangramXmlParse m_Parse2;
		if (m_Parse2.LoadFile(m_strPageFilePath))
		{
			CTangramXmlParse* m_pHubblePageParse = m_Parse2.GetChild(_T("hubblepage"));
			if (m_pHubblePageParse == nullptr)
			{
				m_Parse2.AddNode(_T("hubblepage"));
				m_pHubblePageParse = m_Parse2.GetChild(_T("hubblepage"));
			}
			if (m_pHubblePageParse)
			{
				CTangramXmlParse* m_pHubblePageParse2 = m_pHubblePageParse->GetChild(m_strConfigFileNodeName);
				if (m_pHubblePageParse2 == nullptr)
				{
					if(m_pHubblePageParse->AddNode(m_strConfigFileNodeName)!=nullptr)
						m_Parse2.SaveFile(m_strPageFilePath);
				}
				if (m_pHubblePageParse2)
				{
					int nCount = m_pHubblePageParse2->GetCount();
					for (int i = 0; i < nCount; i++)
					{
						CTangramXmlParse* _pParse = m_pHubblePageParse2->GetChild(i);
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
		if (::PathIsDirectory(g_pHubble->m_strAppDataPath) == false)
		{
			if (::SHCreateDirectoryEx(NULL, g_pHubble->m_strAppDataPath, NULL))
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

STDMETHODIMP CGalaxyCluster::CreateQuasarWithDefaultNode(ULONGLONG hFrameWnd, BSTR bstrFrameName, BSTR bstrDefaultNodeKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfig, IGrid** ppGrid)
{
	CString strXml = OLE2T(bstrXml);
	strXml.Trim();
	if (strXml == _T(""))
		strXml = _T("<default><layout><grid name=\"Start\" /></layout></default>");
	IQuasar* pQuasar = nullptr;
	CreateQuasar(CComVariant(0), CComVariant((LONGLONG)hFrameWnd), bstrFrameName, &pQuasar);
	if (pQuasar)
	{
		pQuasar->Observe(bstrDefaultNodeKey, strXml.AllocSysString(), ppGrid);
		if (*ppGrid&&bSaveToConfig)
		{
			(*ppGrid)->put_SaveToConfigFile(true);
			//CQuasar* pFrame2 = (CQuasar*)::SendMessage(((CQuasar*)pQuasar)->m_hWnd, WM_TANGRAMDATA, 0, 1992);
			//if (pQuasar)
			//{
			//	pFrame2->m_nQuasarType = EclipseWorkBenchQuasar;
			//	//pQuasar->m_pWorkBenchFrame = this;
			//}
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::ObserveQuasars(BSTR bstrFrames, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfigFile)
{
	CString strFrames = OLE2T(bstrFrames);
	CString strKey = OLE2T(bstrKey);
	CString strXml = OLE2T(bstrXml);
	if (strFrames == _T(""))
	{
		for (auto it : m_mapQuasar)
		{
			if (it.second != m_pBKFrame)
			{
				IGrid* pGrid = nullptr;
				it.second->Observe(bstrKey, bstrXml, &pGrid);
				if (pGrid&&bSaveToConfigFile)
					pGrid->put_SaveToConfigFile(true);
			}
		}
	}
	else
	{
		strFrames = _T(",") + strFrames;
		for (auto it : m_mapQuasar)
		{
			CString strName = _T(",") + it.second->m_strQuasarName + _T(",");
			if (strFrames.Find(strName) != -1)
			{
				IGrid* pGrid = nullptr;
				it.second->Observe(bstrKey, bstrXml, &pGrid);
				if (pGrid&&bSaveToConfigFile)
					pGrid->put_SaveToConfigFile(true);
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_CurrentDesignQuasarType(QuasarType* pVal)
{
	if (g_pHubble->m_pDesignGrid)
	{
		CQuasar* pQuasar = g_pHubble->m_pDesignGrid->m_pGridCommonData->m_pQuasar;
		*pVal = pQuasar->m_nQuasarType;
	}
	else
		*pVal = NOQuasar;

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_CurrentDesignNode(IGrid** pVal)
{
	if (g_pHubble->m_pDesignGrid)
		*pVal = g_pHubble->m_pDesignGrid;

	return S_OK;
}
