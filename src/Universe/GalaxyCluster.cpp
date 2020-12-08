/********************************************************************************
*					Open Universe - version 1.1.3.24							*
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

// GalaxyCluster.cpp : Implementation of CGalaxyCluster

#include "stdafx.h"
#include "grid.h"
#include "Galaxy.h"
#include "UniverseApp.h"
#include "Hubble.h"

CGridShareData::CGridShareData()
{
	m_pOldGalaxy		= nullptr;
	m_pHubbleParse		= nullptr;
	m_pHostClientView	= nullptr;
#ifdef _DEBUG
	g_pHubble->m_nTangramNodeCommonData++;
#endif	
};

CGridShareData::~CGridShareData()
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

	m_mapGalaxy.erase(m_mapGalaxy.begin(), m_mapGalaxy.end());
	m_mapGrid.erase(m_mapGrid.begin(), m_mapGrid.end());
	auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
	if (it != g_pHubble->m_mapWindowPage.end())
	{
		g_pHubble->m_mapWindowPage.erase(it);
	}
	if (g_pHubble->m_mapWindowPage.size() == 0)
		g_pHubble->Close();
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
	//typedef CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>>
	//	CComEnumVariant;

	//CComObject<CComEnumVariant> *pe = 0;
	//HRESULT hr = pe->CreateInstance(&pe);

	//if (SUCCEEDED(hr))
	//{
	//	pe->AddRef();
	//	long nLen = (long)m_mapGalaxy.size();
	//	VARIANT* rgvar = new VARIANT[nLen];
	//	ZeroMemory(rgvar, sizeof(VARIANT)*nLen);
	//	VARIANT* pItem = rgvar;
	//	for (auto it : m_mapGalaxy)
	//	{
	//		IUnknown* pDisp = nullptr;
	//		CGalaxy* pObj = it.second;
	//		hr = pObj->QueryInterface(IID_IUnknown, (void**)&pDisp);
	//		pItem->vt = VT_UNKNOWN;
	//		pItem->punkVal = pDisp;
	//		if (pItem->punkVal != nullptr)
	//			pItem->punkVal->AddRef();
	//		pItem++;
	//		pDisp->Release();
	//	}

	//	hr = pe->Init(rgvar, &rgvar[nLen], 0, AtlFlagTakeOwnership);
	//	if (SUCCEEDED(hr))
	//		hr = pe->QueryInterface(IID_IUnknown, (void**)ppVal);
	//	pe->Release();
	//}
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::CreateGalaxy(VARIANT ParentObj, VARIANT HostWnd, BSTR bstrGalaxyName, IGalaxy** pRetFrame)
{
	HWND h = 0; 
	CString strGalaxyName = OLE2T(bstrGalaxyName);
	BSTR bstrName = strGalaxyName.MakeLower().AllocSysString();
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
		if (g_pHubble->m_pCLRProxy)
		{
			h = g_pHubble->m_pCLRProxy->IsGalaxy(HostWnd.pdispVal);
			if (h)
			{
				CString strName = strGalaxyName;
				if (strName == _T(""))
				{
					::SysFreeString(bstrName);
					bstrGalaxyName = g_pHubble->m_pCLRProxy->GetCtrlName(HostWnd.pdispVal);
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
				CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);

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
				::GetClassName(::GetParent(_hWnd), g_pHubble->m_szBuffer, MAX_PATH);
				CString strClassName = CString(g_pHubble->m_szBuffer);
				if (strClassName.Find(_T("MDIClient")) == 0)
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
	if (g_pHubble->m_pCLRProxy)
	{
		HWND h = g_pHubble->m_pCLRProxy->IsGalaxy(CtrlObj);
		if (h)
		{
			auto it = m_mapGalaxy.find(h);
			if (it != m_mapGalaxy.end())
				* ppGalaxy = it->second;
		}
	}

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::GetGrid(BSTR bstrGalaxyName, BSTR bstrNodeName, IGrid** pRetNode)
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
				auto it2 = pGalaxy->m_mapGrid.find(strName);
				if (it2 != pGalaxy->m_mapGrid.end())
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
	return S_OK;
}


STDMETHODIMP CGalaxyCluster::put_Extender(BSTR bstrExtenderName, IDispatch* newVal)
{
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

void CGalaxyCluster::BeforeDestory()
{
	for (auto it: m_mapGalaxy)
		it.second->Destroy();

	if (g_pHubble->m_pCLRProxy)
	{
		g_pHubble->m_pCLRProxy->ReleaseHubbleObj((IGalaxyCluster*)this);
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
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::put_xtml(BSTR strKey, BSTR newVal)
{
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::Observe(IDispatch* Parent, BSTR CtrlName, BSTR GalaxyName, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid)
{
	if (g_pHubble->m_pCLRProxy)
	{
		IDispatch* pDisp = nullptr;
		pDisp =g_pHubble->m_pCLRProxy->GetCtrlByName(Parent, CtrlName, true);
		if (pDisp)
		{
			HWND h = 0;
			h = g_pHubble->m_pCLRProxy->IsGalaxy(pDisp);
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
					return pGalaxy->Observe(bstrKey, bstrXml, ppRetGrid);
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
				h = g_pHubble->m_pCLRProxy->IsGalaxy(MdiForm.pdispVal);
				if (h)
				{
					CComBSTR bstrName(L"");
					bstrName = g_pHubble->m_pCLRProxy->GetCtrlName(MdiForm.pdispVal);
					CString strKey = OLE2T(bstrKey);
					if (strKey == _T(""))
						bstrKey = CComBSTR(L"Default");
					IGalaxy* pGalaxy = nullptr;
					map<HWND, CGalaxy*>::iterator it = m_mapGalaxy.find((HWND)h);
					if (it == m_mapGalaxy.end())
					{
						CreateGalaxy(CComVariant(0), CComVariant((long)h), bstrName, &pGalaxy);
						return pGalaxy->Observe(bstrKey, bstrXml, ppRetGrid);
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
		IGalaxy* pGalaxy = nullptr;
		if (bMDI)
		{
			HRESULT hr = CreateGalaxy(CComVariant(0), CComVariant((long)h), CComBSTR(L"TangramMDIClientGalaxy"), &pGalaxy);
			if (pGalaxy)
			{
				return pGalaxy->Observe(bstrKey, bstrXml, ppRetGrid);
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
				return pGalaxy->Observe(bstrKey, bstrXml, ppRetGrid);
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


STDMETHODIMP CGalaxyCluster::get_GalaxyClusterXML(BSTR* pVal)
{
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::put_ConfigName(BSTR newVal)
{
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::CreateGalaxyWithDefaultNode(ULONGLONG hGalaxyWnd, BSTR bstrGalaxyName, BSTR bstrDefaultNodeKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfig, IGrid** ppGrid)
{
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::ObserveGalaxys(BSTR bstrGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSaveToConfigFile)
{
	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_CurrentDesignGalaxyType(GalaxyType* pVal)
{
	if (g_pHubble->m_pDesignGrid)
	{
		CGalaxy* pGalaxy = g_pHubble->m_pDesignGrid->m_pGridShareData->m_pGalaxy;
		*pVal = pGalaxy->m_nGalaxyType;
	}
	else
		*pVal = NOGalaxy;

	return S_OK;
}

STDMETHODIMP CGalaxyCluster::get_CurrentDesignNode(IGrid** pVal)
{
	if (g_pHubble->m_pDesignGrid)
		*pVal = g_pHubble->m_pDesignGrid;

	return S_OK;
}
