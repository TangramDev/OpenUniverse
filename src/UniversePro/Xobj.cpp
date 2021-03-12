/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103120049           *
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

// Xobj.cpp : Implementation of CXobj

#include "stdafx.h"
#include "UniverseApp.h"
#include "Cosmos.h"
#include "XobjWnd.h"
#include "GridWnd.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "TangramHtmlTreeWnd.h"
#include "TangramHtmlTreeExWnd.h"
#include "ProgressFX.h"
#include "HourglassFX.h"
#include "TangramTreeView.h"
#include "CosmosEvents.h"
#include "GridWnd.h"
#include "EclipsePlus\EclipseAddin.h"
#include "CosmosCtrl.h"
#include "TangramListView.h"
#include "Wormhole.h"
#include "WPFView.h"
#include "universe.c"
#include "TangramTabCtrl.h"
#include "chromium/Browser.h"
#include "chromium/WebPage.h"

CXobj::CXobj()
{
	g_pCosmos->m_nTangramObj++;
	m_nID = 0;
	m_nRow = 0;
	m_nCol = 0;
	m_nRows = 1;
	m_nCols = 1;
	m_nViewType = BlankView;
	m_pChildFormsInfo = nullptr;
	m_bTopObj = false;
	m_bCreated = false;
	m_varTag.vt = VT_EMPTY;
	m_strKey = _T("");
	m_strURL = _T("");
	m_strNodeName = _T("");
	m_strExtenderID = _T("");
	m_hHostWnd = NULL;
	m_hChildHostWnd = NULL;
	m_pDisp = nullptr;
	m_pHostGalaxy = nullptr;
	m_pRootObj = nullptr;
	m_pHostWnd = nullptr;
	m_pExtender = nullptr;
	m_pParentObj = nullptr;
	m_pObjClsInfo = nullptr;
	m_pCLREventConnector = nullptr;
	m_pChildNodeCollection = nullptr;
	m_pCurrentExNode = nullptr;
	m_pWindow = nullptr;
	m_pHostParse = nullptr;
	m_pDocXmlParseNode = nullptr;
	m_pWebBrowser = nullptr;
	m_pXobjShareData = nullptr;
	g_pCosmos->m_pActiveXobj = this;
}

void CXobj::InitWndXobj()
{
	m_pParentWinFormWnd = nullptr;
	m_pWormhole = nullptr;
	m_pXobjShareData = m_pRootObj->m_pXobjShareData;
	ASSERT(m_pXobjShareData != nullptr);
	m_nHeigh = m_pHostParse->attrInt(TGM_HEIGHT, 0);
	m_nWidth = m_pHostParse->attrInt(TGM_WIDTH, 0);
	m_strObjTypeID = m_pHostParse->attr(TGM_OBJ_ID, _T(""));
	if (m_strObjTypeID.CompareNoCase(TGM_NUCLEUS) == 0)
		m_strID = TGM_NUCLEUS;
	m_nRows = m_pHostParse->attrInt(TGM_ROWS, 0);
	m_nCols = m_pHostParse->attrInt(TGM_COLS, 0);
	if (m_nRows * m_nCols > 1)
	{
		m_strID = _T("xobj");
		m_nViewType = Grid;
	}
	else
	{
		if (m_pHostParse->GetChild(TGM_XOBJ))
			m_nViewType = TabGrid;
		if (m_strID != TGM_NUCLEUS)
		{
			m_strID = m_pHostParse->attr(TGM_GRID_TYPE, _T(""));
			m_strID.MakeLower();
			m_strID.Trim();
		}
		if (m_strID == _T(""))
		{
			if (m_strObjTypeID.Find(_T(",")) != -1)
			{
				m_strID = _T("clrctrl");
				m_nViewType = CLRCtrl;
			}
			else if (m_pXobjShareData->m_pGalaxy->m_pWebPageWnd)
			{
				auto it = m_pXobjShareData->m_pGalaxy->m_pWebPageWnd->m_mapFormsInfo.find(m_strObjTypeID);
				if (it != m_pXobjShareData->m_pGalaxy->m_pWebPageWnd->m_mapFormsInfo.end())
				{
					m_strID = _T("clrctrl");
					m_nViewType = CLRCtrl;
				}
			}
		}
	}
	m_strName = m_pHostParse->attr(_T("id"), _T(""));
	if (m_strName == _T(""))
	{
		switch (m_nViewType)
		{
		case Grid:
		{
			if (this == m_pRootObj)
			{
				m_strName = _T("Xobj");
			}
		}
		break;
		case TabGrid:
			if (this == m_pRootObj)
			{
				if (m_pObjClsInfo)
					m_strName = m_pObjClsInfo->m_lpszClassName;
				else
					m_strName = _T("Tabs");
			}
			break;
		case CLRCtrl:
			if (this == m_pRootObj)
			{
				int nPos = m_strObjTypeID.Find(_T(","));
				if (nPos != -1)
				{
					m_strName = m_strObjTypeID.Left(nPos);
					m_strName.Replace(_T("."), _T("_"));
				}
			}
			break;
		default:
			if (m_pObjClsInfo && this == m_pRootObj)
				m_strName = m_pObjClsInfo->m_lpszClassName;
			break;
		}
		if (m_strName == _T(""))
		{
			if (m_pParentObj)
			{
				CString _strName = m_pParentObj->m_strName;
				if (m_pParentObj->m_nViewType == Grid)
				{
					if (m_nViewType == CLRCtrl)
					{
						int nPos = m_strObjTypeID.Find(_T(","));
						if (nPos != -1)
						{
							CString _strCtrlName = m_strObjTypeID.Left(nPos);
							_strCtrlName.Replace(_T("."), _T("_"));
							m_strName.Format(_T("%s_%02d%02d_%s"), _strName, m_nRow, m_nCol, _strCtrlName);
						}
					}
					else
						m_strName.Format(_T("%s_%02d%02d"), _strName, m_nRow, m_nCol);
				}
				else
				{
					if (m_nViewType == CLRCtrl)
					{
						int nPos = m_strObjTypeID.Find(_T(","));
						if (nPos != -1)
						{
							CString _strCtrlName = m_strObjTypeID.Left(nPos);
							_strCtrlName.Replace(_T("."), _T("_"));
							m_strName.Format(_T("%s_%s%d"), _strName, _strCtrlName, m_nCol);
						}
					}
					else
						m_strName.Format(_T("%s_Page%d"), _strName, m_nCol);
				}
			}
			else
				m_strName.Format(_T("Xobj_%p"), (LONGLONG)this);
		}
		m_pHostParse->put_attr(_T("id"), m_strName);
	}
	m_strObjTypeID.MakeLower();
	m_strObjTypeID.Trim();
	m_pRootObj->m_mapChildXobj[m_strName] = this;
	m_nActivePage = m_pHostParse->attrInt(TGM_ACTIVE_PAGE, 0);
	m_strCaption = m_pHostParse->attr(TGM_CAPTION, _T(""));
	if (m_pXobjShareData->m_pGalaxy && m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster)
	{
		m_strNodeName = m_strName + _T("@") + g_pCosmos->m_strCurrentKey + _T("@") + m_pXobjShareData->m_pGalaxy->m_strGalaxyName;
		auto it2 = m_pXobjShareData->m_pGalaxyCluster->m_mapXobj.find(m_strNodeName);
		if (it2 == m_pXobjShareData->m_pGalaxyCluster->m_mapXobj.end())
		{
			m_pXobjShareData->m_pGalaxyCluster->m_mapXobj[m_strNodeName] = this;
		}
	}

	auto it = g_pCosmos->m_mapClassInfo.find(m_strID);
	if (it != g_pCosmos->m_mapClassInfo.end())
		m_pObjClsInfo = it->second;
	else
		m_pObjClsInfo = RUNTIME_CLASS(CXobjWnd);

	for (auto it : g_pCosmos->m_mapCosmosAppProxy)
	{
		CXobjProxy* pCosmosWndXobjProxy = it.second->OnXobjInit(this);
		if (pCosmosWndXobjProxy)
			m_mapWndXobjProxy[it.second] = pCosmosWndXobjProxy;
	}
}

CXobj::~CXobj()
{
	if (g_pCosmos->m_pActiveXobj == this)
		g_pCosmos->m_pActiveXobj = nullptr;
	if (m_pXobjShareData->m_pOldGalaxy)
		m_pXobjShareData->m_pGalaxy = m_pXobjShareData->m_pOldGalaxy;
	CGalaxy * pGalaxy = m_pXobjShareData->m_pGalaxy;
	if (pGalaxy->m_pGalaxyCluster)
	{
		auto it = pGalaxy->m_pGalaxyCluster->m_mapXobj.find(m_strNodeName);
		if (it != pGalaxy->m_pGalaxyCluster->m_mapXobj.end())
			pGalaxy->m_pGalaxyCluster->m_mapXobj.erase(it);
	}
	if (pGalaxy->m_pWorkXobj == this)
		pGalaxy->m_pWorkXobj = nullptr;
	if (m_strKey != _T(""))
	{
		auto it = pGalaxy->m_mapXobj.find(m_strKey);
		if (it != pGalaxy->m_mapXobj.end())
		{
			//BOOL bDeleteFrame = FALSE;
			pGalaxy->m_mapXobj.erase(it);
			if (pGalaxy->m_mapXobj.size() == 0)
			{
				if (::IsWindow(pGalaxy->m_hWnd))
				{
					pGalaxy->UnsubclassWindow(true);
					pGalaxy->m_hWnd = NULL;
				}
				delete pGalaxy;
			}
			delete m_pXobjShareData;
			m_pXobjShareData = nullptr;
		}
	}

	if (m_pCLREventConnector)
	{
		delete m_pCLREventConnector;
		m_pCLREventConnector = nullptr;
	}

	g_pCosmos->m_nTangramObj--;
#ifdef _DEBUG
#endif
	HRESULT hr = S_OK;
	DWORD dw = 0;

	if (m_pExtender)
	{
		dw = m_pExtender->Release();
		m_pExtender = nullptr;
	}

	m_vChildNodes.clear();

	if (m_pChildNodeCollection != nullptr)
	{
		dw = m_pChildNodeCollection->Release();
		while (dw)
			dw = m_pChildNodeCollection->Release();
		m_pChildNodeCollection = nullptr;
	}
	for (auto it : m_mapWndXobjProxy)
	{
		if (it.second->m_bAutoDelete)
			delete it.second;
	}
	m_mapWndXobjProxy.clear();
	ATLTRACE(_T("delete CXobj:%x\n"), this);
}

CString CXobj::GetNames()
{
	CString strRet = _T("");
	CXobj* pXobj = this;
	if (pXobj)
	{
		pXobj = pXobj->m_pRootObj;
		strRet += pXobj->m_strName;
		strRet += _T(",");
		strRet += _GetNames(pXobj);
	}
	return strRet;
}

CString CXobj::_GetNames(CXobj * pXobj)
{
	CString strRet = _T("");
	if (pXobj)
	{
		for (auto it : pXobj->m_vChildNodes)
		{
			CXobj* pChildNode = it;
			strRet += pChildNode->m_strName;
			strRet += _T(",");
			strRet += _GetNames(pChildNode);
		}
	}
	return strRet;
}

CWebPage* CXobj::GetHtmlWnd()
{
	if (m_pRootObj)
	{
		HWND hPWnd = m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster->m_hWnd;
		HWND hWnd = (HWND)::GetWindowLongPtr(hPWnd, GWLP_USERDATA);
		if (::IsWindow(hWnd))
		{
			::GetClassName(hWnd, g_pCosmos->m_szBuffer, 256);
			CString strName = CString(g_pCosmos->m_szBuffer);
			if (strName == _T("Chrome Extended Window Class")) {
				return (CWebPage*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			}
		}
		else
		{
			if (m_pParentWinFormWnd == nullptr)
			{
				m_pXobjShareData->m_pGalaxy->GetWinForm(hPWnd);
				if (m_pRootObj->m_pParentWinFormWnd)
				{
					m_pParentWinFormWnd = m_pRootObj->m_pParentWinFormWnd;
					if(m_pRootObj->m_pParentWinFormWnd->m_pOwnerHtmlWnd)
						return m_pRootObj->m_pParentWinFormWnd->m_pOwnerHtmlWnd;
					else
					{
						hPWnd = m_pRootObj->m_pParentWinFormWnd->m_hWnd;
						if ((::GetWindowLong(hPWnd, GWL_EXSTYLE) & WS_EX_MDICHILD))
						{
							hPWnd = ::GetParent(::GetParent(hPWnd));
							CWinForm* pForm = (CWinForm*)::SendMessage(hPWnd, WM_HUBBLE_DATA, 0, 20190214);
							if (pForm)
							{
								return pForm->m_pOwnerHtmlWnd;
							}
						}
					}
				}
				CWinForm* pForm = (CWinForm*)::SendMessage(hPWnd, WM_HUBBLE_DATA, 0, 20190214);
				if (pForm)
				{
					m_pParentWinFormWnd = pForm;
					return pForm->m_pOwnerHtmlWnd;
				}
			}
		}
	}
	return nullptr;
}

BOOL CXobj::PreTranslateMessage(MSG * pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_pHostWnd && pMsg->message != WM_MOUSEMOVE)
	{
		m_pHostWnd->PreTranslateMessage(pMsg);

		for (auto it : m_vChildNodes)
		{
			it->PreTranslateMessage(pMsg);
		}
	}
	return true;
}

STDMETHODIMP CXobj::LoadXML(int nType, BSTR bstrXML)
{
	if (m_strID.CompareNoCase(_T("TreeView")) == 0)
	{
		CTangramHtmlTreeWnd* _pXHtmlTree = ((CXobjWnd*)m_pHostWnd)->m_pXHtmlTree;
		if (nType)
		{
			_pXHtmlTree->LoadXmlFromFile(OLE2T(bstrXML), CTangramHtmlTreeWnd::ConvertToUnicode);
		}
		else
		{
			_pXHtmlTree->LoadXmlFromString(OLE2T(bstrXML), CTangramHtmlTreeWnd::ConvertToUnicode);
		}
	}

	return S_OK;
}

STDMETHODIMP CXobj::ActiveTabPage(IXobj * _pXobj)
{
	g_pCosmos->m_pActiveXobj = this;
	HWND hWnd = m_pHostWnd->m_hWnd;
	if (m_pHostWnd && ::IsWindow(hWnd))
	{
		::SetFocus(hWnd);
		m_pXobjShareData->m_pGalaxy->HostPosChanged();
		m_pXobjShareData->m_pGalaxy->UpdateVisualWPFMap(::GetParent(hWnd), true);
	}
	return S_OK;
}

STDMETHODIMP CXobj::Observe(BSTR bstrKey, BSTR bstrXml, IXobj * *ppRetXobj)
{
	switch (m_nViewType)
	{
	case ActiveX:
	case CLRCtrl:
	case BlankView:
	{
		if (m_nViewType == BlankView)
		{
			if (m_pParentObj && m_pParentObj->m_nViewType == Grid)
			{
				HRESULT hr =  m_pParentObj->ObserveEx(m_nRow, m_nCol, bstrKey, bstrXml, ppRetXobj);
				return hr;
			}
		}
		if (m_pXobjShareData->m_pGalaxyCluster)
		{
			if (m_nViewType == BlankView && m_pParentObj && m_pParentObj->m_nViewType == Grid)
			{
				return m_pParentObj->ObserveEx(m_nRow, m_nCol, bstrKey, bstrXml, ppRetXobj);
			}
			if (m_pHostGalaxy == nullptr)
			{
				CString strName = m_strNodeName;
				strName += _T("_Frame");

				if (m_nViewType == BlankView)
				{
					RECT rc;
					::GetClientRect(m_pHostWnd->m_hWnd, &rc);
					m_hHostWnd = ::CreateWindowEx(NULL, L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_pHostWnd->m_hWnd, NULL, AfxGetInstanceHandle(), NULL);
				}
				else
				{
					m_hHostWnd = ::GetWindow(m_pHostWnd->m_hWnd, GW_CHILD);
				}
				IGalaxy* pGalaxy = nullptr;
				m_pXobjShareData->m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)m_hHostWnd), strName.AllocSysString(), &pGalaxy);
				if (pGalaxy)
					m_pHostGalaxy = (CGalaxy*)pGalaxy;
			}
			if (m_pHostGalaxy && ::IsWindow(m_hHostWnd))
			{
				m_pHostGalaxy->m_strDocTemplateID = m_pXobjShareData->m_pGalaxy->m_strDocTemplateID;
				m_pHostGalaxy->m_pContainerNode = this;
				if (m_pXobjShareData->m_pGalaxy->m_pParentXobj)
				{
					m_pHostGalaxy->m_pParentXobj = m_pXobjShareData->m_pGalaxy->m_pParentXobj;
				}
				HRESULT hr = m_pHostGalaxy->Observe(bstrKey, bstrXml, ppRetXobj);
				if (m_pWebBrowser)
				{
					CXobj* pRetNode = (CXobj*)*ppRetXobj;
					CComPtr<IXobjCollection> pCosmosNodeCollection;
					IXobj* _pXobj = nullptr;
					long nCount = 0;
					pRetNode->m_pRootObj->GetXobjs(CComBSTR(m_strName), &_pXobj, &pCosmosNodeCollection, &nCount);
					if (_pXobj)
					{
						CXobj* pXobj2 = (CXobj*)_pXobj;
						pXobj2->m_pWebBrowser = m_pWebBrowser;
						CXobj* pOldParent = m_pWebBrowser->m_pParentXobj;
						m_pWebBrowser->m_pParentXobj = pXobj2;
						m_pHostGalaxy->m_pHostWebBrowserNode = pXobj2;
						m_pHostGalaxy->m_strHostWebBrowserNodeName = m_strName;
						m_pHostGalaxy->m_pHostWebBrowserWnd = m_pWebBrowser;
						HWND hWnd = m_pWebBrowser->m_hWnd;
						HWND h = ::GetParent(hWnd);
						CXobjWnd* pXobjWnd = (CXobjWnd*)CWnd::FromHandlePermanent(h);
						pXobjWnd->m_hFormWnd = nullptr;
						::SetParent(hWnd, ((CXobjWnd*)pXobj2->m_pHostWnd)->m_hWnd);
						((CXobjWnd*)pXobj2->m_pHostWnd)->m_hFormWnd = hWnd;
						if (pOldParent && pOldParent != pXobj2)
							pOldParent->m_pWebBrowser = nullptr;
						::PostMessage(pXobj2->m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20200128);
					}
				}
				return hr;
			}
		}
	}
	break;
	case Grid:
		break;
	}
	m_pXobjShareData->m_pGalaxy->ModifyStyle(WS_CLIPCHILDREN, 0);
	::PostMessage(m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20210202);
	return S_OK;
}

STDMETHODIMP CXobj::ObserveEx(int nRow, int nCol, BSTR bstrKey, BSTR bstrXml, IXobj * *ppRetXobj)
{
	if (m_pXobjShareData->m_pGalaxyCluster && m_nViewType == Grid)
	{
		IXobj* pXobj = nullptr;
		GetXobj(nRow, nCol, &pXobj);
		if (pXobj == nullptr)
			return S_OK;
		CXobj * pWndXobj = (CXobj*)pXobj;
		if (pWndXobj->m_pHostGalaxy == nullptr)
		{
			CString strName = pWndXobj->m_strNodeName;
			strName += _T("_Frame");

			::SetWindowLong(pWndXobj->m_pHostWnd->m_hWnd, GWL_ID, 1);
			IGalaxy* pGalaxy = nullptr;
			m_pXobjShareData->m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)pWndXobj->m_pHostWnd->m_hWnd), strName.AllocSysString(), &pGalaxy);
			pWndXobj->m_pHostGalaxy = (CGalaxy*)pGalaxy;
			CGalaxy* _pGalaxy = pWndXobj->m_pHostGalaxy;
			_pGalaxy->m_pWebPageWnd = m_pXobjShareData->m_pGalaxy->m_pWebPageWnd;
			_pGalaxy->m_strDocTemplateID = m_pXobjShareData->m_pGalaxy->m_strDocTemplateID;
			if (m_pXobjShareData->m_pGalaxy->m_pParentXobj)
			{
				_pGalaxy->m_pParentXobj = m_pXobjShareData->m_pGalaxy->m_pParentXobj;
			}
			if (m_pXobjShareData->m_pGalaxy->m_pWebPageWnd)
			{
				_pGalaxy->m_pWebPageWnd = m_pXobjShareData->m_pGalaxy->m_pWebPageWnd;
			}
		}

		if (pWndXobj->m_pHostGalaxy)
		{
			if (pWndXobj->m_pCurrentExNode)
			{
				::SetWindowLong(pWndXobj->m_pCurrentExNode->m_pHostWnd->m_hWnd, GWL_ID, 1);
			}

			HRESULT hr = pWndXobj->m_pHostGalaxy->Observe(bstrKey, bstrXml, ppRetXobj);

			long dwID = AFX_IDW_PANE_FIRST + nRow * 16 + nCol;
			if (hr != S_OK)
			{
				if (pWndXobj->m_pCurrentExNode)
					::SetWindowLong(pWndXobj->m_pCurrentExNode->m_pHostWnd->m_hWnd, GWL_ID, dwID);
				else
					::SetWindowLong(pWndXobj->m_pHostWnd->m_hWnd, GWL_ID, dwID);
				CWebPage* pWebWnd = pWndXobj->m_pHostGalaxy->m_pWebPageWnd;
				if (pWebWnd)
				{
					::SendMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
					::PostMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
					//::InvalidateRect(::GetParent(pWebWnd->m_hWnd), nullptr, true);
				}
				return S_OK;
			}
			CXobj* pRootXobj = (CXobj*)* ppRetXobj;
			CString strKey = OLE2T(bstrKey);
			strKey.MakeLower();
			m_mapExtendNode[pWndXobj] = strKey;
			pWndXobj->m_pCurrentExNode = pRootXobj;
			::SetWindowLongPtr(pRootXobj->m_pHostWnd->m_hWnd, GWLP_ID, dwID);
			CWebPage* pWebWnd = pWndXobj->m_pHostGalaxy->m_pWebPageWnd;
			if (pWebWnd)
			{
				if (pWndXobj->m_pHostGalaxy->m_pBindingXobj)
					pWebWnd->m_hWebHostWnd = pWndXobj->m_pHostGalaxy->m_pBindingXobj->m_pHostWnd->m_hWnd;
				else
					pWebWnd->m_hWebHostWnd = NULL;
				::SendMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
				::PostMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
				//::InvalidateRect(::GetParent(pWebWnd->m_hWnd), nullptr, true);
			}
			HWND h = ::GetParent(m_pHostWnd->m_hWnd);
			if (m_nViewType==Grid)
			{
				CGalaxy* pGalaxy = m_pXobjShareData->m_pGalaxy;
				pGalaxy->HostPosChanged();
			}
			m_pXobjShareData->m_pGalaxy->ModifyStyle(WS_CLIPCHILDREN, 0);
			::PostMessage(m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20210202);
			return hr;
		}
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_Tag(VARIANT * pVar)
{
	*pVar = m_varTag;

	if (m_varTag.vt == VT_DISPATCH && m_varTag.pdispVal)
		m_varTag.pdispVal->AddRef();
	return S_OK;
}

STDMETHODIMP CXobj::put_Tag(VARIANT var)
{
	m_varTag = var;
	return S_OK;
}

STDMETHODIMP CXobj::get_XObject(VARIANT * pVar)
{
	pVar->vt = VT_EMPTY;
	if (m_pDisp)
	{
		pVar->vt = VT_DISPATCH;
		pVar->pdispVal = m_pDisp;
		pVar->pdispVal->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_AxPlugIn(BSTR bstrPlugInName, IDispatch * *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strObjName = OLE2T(bstrPlugInName);
	strObjName.Trim();
	strObjName.MakeLower();
	IDispatch* pDisp = nullptr;
	if (m_pXobjShareData->m_PlugInDispDictionary.Lookup(LPCTSTR(strObjName), (void*&)pDisp))
	{
		*pVal = pDisp;
		(*pVal)->AddRef();
	}
	else
		*pVal = nullptr;
	return S_OK;
}

STDMETHODIMP CXobj::get_Name(BSTR * pVal)
{
	*pVal = m_strName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CXobj::put_Name(BSTR bstrNewName)
{
	CString strName = OLE2T(bstrNewName);
	strName.Trim();
	strName.Replace(_T(","), _T(""));
	if (m_pHostParse != NULL)
	{
		CString _strName = _T(",");
		_strName += GetNames();
		CString _strName2 = _T(",");
		_strName2 += strName;
		_strName2 += _T(",");
		int nPos = _strName.Find(_strName2);
		if (nPos == -1)
		{
			m_pHostParse->put_attr(L"id", strName);
			m_strName = strName;
		}
		else
		{
			::MessageBox(NULL, _T("Modify name failed!"), _T("Tangram"), MB_OK);
		}
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_Attribute(BSTR bstrKey, BSTR * pVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strVal = m_pHostParse->attr(OLE2T(bstrKey), _T(""));
		*pVal = strVal.AllocSysString();
	}
	return S_OK;
}

STDMETHODIMP CXobj::put_Attribute(BSTR bstrKey, BSTR bstrVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strID = OLE2T(bstrKey);
		CString strVal = OLE2T(bstrVal);
		ATLTRACE(_T("Modify CXobj Attribute: ID: %s Value: %s\n"), strID, strVal);
		CGalaxy* pGalaxy = nullptr;
		if (strVal.CompareNoCase(TGM_NUCLEUS) == 0)
		{
			pGalaxy = m_pRootObj->m_pXobjShareData->m_pGalaxy;
			m_strID = TGM_NUCLEUS;
			CXobj* pTopXobj = m_pRootObj;
			pTopXobj->m_pXobjShareData->m_pHostClientView = (CXobjWnd*)m_pHostWnd;
			m_pHostParse->put_attr(TGM_OBJ_ID, TGM_NUCLEUS);

			pGalaxy->m_pBindingXobj = this;

			if (m_pParentObj && m_pParentObj->m_nViewType == Grid)
				m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			m_pXobjShareData->m_pGalaxy->HostPosChanged();
		}
		m_pHostParse->put_attr(strID, strVal);
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_Caption(BSTR * pVal)
{
	*pVal = m_strCaption.AllocSysString();
	return S_OK;
}

STDMETHODIMP CXobj::put_Caption(BSTR bstrCaption)
{
	CString str(bstrCaption);

	m_strCaption = str;

	if (m_pParentObj != nullptr && m_pParentObj->m_pHostWnd != nullptr)
	{
		m_pParentObj->m_pHostWnd->SendMessage(WM_TGM_SET_CAPTION, m_nCol, (LPARAM)str.GetBuffer());
	}

	if (m_pHostParse != nullptr)
	{
		m_pHostParse->put_attr(L"caption", str);
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_Handle(LONGLONG * pVal)
{
	if (m_pHostWnd)
		* pVal = (LONGLONG)m_pHostWnd->m_hWnd;
	return S_OK;
}

STDMETHODIMP CXobj::get_OuterXml(BSTR * pVal)
{
	*pVal = m_pXobjShareData->m_pCosmosParse->xml().AllocSysString();
	return S_OK;
}

STDMETHODIMP CXobj::get_Key(BSTR * pVal)
{
	*pVal = m_pRootObj->m_strKey.AllocSysString();
	return S_OK;
}

STDMETHODIMP CXobj::get_XML(BSTR * pVal)
{
	*pVal = m_pHostParse->xml().AllocSysString();
	return S_OK;
}

BOOL CXobj::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext)
{
	BOOL bRet = false;

	CWebPage* pHtmlWnd = m_pXobjShareData->m_pGalaxy->m_pWebPageWnd;
	HWND hWnd = 0;
	CXobjWnd* pCosmosDesignView = (CXobjWnd*)m_pHostWnd;
	int nCol = m_pHostParse->GetCount();
	if (nCol && m_strID == _T("") && m_strObjTypeID == _T("") && m_pHostParse->GetChild(TGM_XOBJ))
	{
		m_strObjTypeID = _T("tabbedwnd");
	}
	BOOL isAppWnd = false;
	if (m_strID == _T("activex") || m_strID == _T("clrctrl"))
	{
		if (m_strID == _T("clrctrl") || m_strObjTypeID.Find(_T(",")) != -1)
		{
			g_pCosmos->LoadCLR();
			m_nViewType = CLRCtrl;
		}
		else
			m_nViewType = ActiveX;
		if (m_strObjTypeID.Find(_T("//")) == -1 && ::PathFileExists(m_strObjTypeID) == false)
		{
			CString strPath = g_pCosmos->m_strAppPath + _T("TangramWebPage\\") + m_strObjTypeID;
			if (::PathFileExists(strPath))
				m_strObjTypeID = strPath;
		}

		hWnd = CreateView(pParentWnd->m_hWnd, m_strObjTypeID);
		if (m_pDisp)
		{
			CComBSTR bstrExtenderID(L"");
			get_Attribute(_T("extender"), &bstrExtenderID);
			m_strExtenderID = OLE2T(bstrExtenderID);
			m_strExtenderID.Trim();
			if (m_strExtenderID != _T(""))
			{
				CComPtr<IDispatch> pDisp;
				pDisp.CoCreateInstance(bstrExtenderID);
				if (pDisp)
				{
					m_pExtender = pDisp.Detach();
					m_pExtender->AddRef();
				}
			}

			pCosmosDesignView->m_bCreateExternal = true;
		}
		bRet = true;
	}
	else
	{
		if (hWnd == NULL)
		{
			int nPos = m_strID.Find(_T("@"));
			if (nPos != -1)
			{
				IUniverseAppProxy* pProxy = nullptr;
				CString strKey = m_strID.Mid(nPos + 1);
				auto it = g_pCosmos->m_mapCosmosAppProxy.find(strKey);
				if (it != g_pCosmos->m_mapCosmosAppProxy.end())
				{
					pProxy = it->second;
				}
				else
				{
					CString strPath = g_pCosmos->m_strAppPath;
					nPos = strKey.Find(_T("."));
					CString strAppName = strKey.Left(nPos);
					HMODULE hHandle = nullptr;
					CString strdll = strPath + strKey + _T("\\") + strAppName + _T(".dll");
					if (::PathFileExists(strdll))
						hHandle = ::LoadLibrary(strdll);
					if (hHandle)
					{
						it = g_pCosmos->m_mapCosmosAppProxy.find(strKey.MakeLower());
						if (it != g_pCosmos->m_mapCosmosAppProxy.end())
						{
							pProxy = it->second;
						}
					}
				}
			}

			if (m_nViewType!=CLRCtrl&&::IsWindow(hWnd) == false&&m_strObjTypeID != _T(""))
			{
				ICosmosWindowProvider* pViewFactoryDisp = nullptr;
				auto it = g_pCosmos->m_mapWindowProvider.find(m_strObjTypeID);
				if (it != g_pCosmos->m_mapWindowProvider.end())
				{
					pViewFactoryDisp = it->second;
				}
				else
				{
					if (it == g_pCosmos->m_mapWindowProvider.end())
					{
						if (m_strID.CompareNoCase(_T("TreeView")))
						{
							if (g_pCosmos->m_strExeName.CompareNoCase(_T("devenv")) == 0)
							{
#ifdef _WIN32
								CString strLib = g_pCosmos->m_strAppPath + _T("PublicAssemblies\\TabbedWnd.dll");
								if (::PathFileExists(strLib))
								{
									::LoadLibrary(strLib);
									auto it = g_pCosmos->m_mapWindowProvider.find(m_strObjTypeID);
									if (it != g_pCosmos->m_mapWindowProvider.end())
									{
										pViewFactoryDisp = it->second;
									}
								}
#endif
							}
							else
							{
								CString strLib = g_pCosmos->m_strAppPath + _T("TangramTabbedWnd.dll");
								if (::PathFileExists(strLib))
								{
									::LoadLibrary(strLib);
									auto it = g_pCosmos->m_mapWindowProvider.find(m_strObjTypeID);
									if (it != g_pCosmos->m_mapWindowProvider.end())
									{
										pViewFactoryDisp = it->second;
									}
								}
							}
							if (pViewFactoryDisp == nullptr)
							{
								CString strLib = _T("");
								CString strPath = g_pCosmos->m_strAppPath + _T("wincomponent\\") + m_strObjTypeID + _T(".component");
								CTangramXmlParse m_Parse;
								if (::PathFileExists(strPath))
								{
									if (m_Parse.LoadFile(strPath))
									{
										strLib = g_pCosmos->m_strAppPath + _T("wincomponent\\") + m_Parse.attr(_T("lib"), _T(""));
									}
								}
								else
								{
									strPath = g_pCosmos->m_strProgramFilePath + _T("\\tangram\\wincomponent\\") + m_strObjTypeID + _T(".component");
									if (m_Parse.LoadFile(strPath))
									{
										strLib = g_pCosmos->m_strProgramFilePath + _T("\\tangram\\wincomponent\\") + m_Parse.attr(_T("lib"), _T(""));
									}
									else
									{
										strPath = g_pCosmos->m_strAppPath + _T("PublicAssemblies\\wincomponent\\") + m_strObjTypeID + _T(".component");
										if (m_Parse.LoadFile(strPath))
										{
											strLib = g_pCosmos->m_strAppPath + _T("PublicAssemblies\\wincomponent\\") + m_Parse.attr(_T("lib"), _T(""));
										}
									}
								}
								if (::PathFileExists(strLib)&&::LoadLibrary(strLib))
								{
									auto it = g_pCosmos->m_mapWindowProvider.find(m_strObjTypeID);
									if (it != g_pCosmos->m_mapWindowProvider.end())
									{
										pViewFactoryDisp = it->second;
									}
								}
							}
						}
					}
				}

				if (pViewFactoryDisp)
				{
					hWnd = pViewFactoryDisp->Create(pParentWnd ? pParentWnd->m_hWnd : 0, this);
					if (g_pCosmos->m_pCreatingWindow)
					{
						m_pWindow = g_pCosmos->m_pCreatingWindow;
						g_pCosmos->m_pCreatingWindow = nullptr;
					}

					//::PostMessage(hWnd, WM_XOBJCREATED, (WPARAM)((IXobj*)this), 10000);
					m_nID = ::GetWindowLong(hWnd, GWL_ID);
				}
			}
		}
	}
	CWnd* pTemp = CWnd::FromHandlePermanent(hWnd);
	if (!::IsWindow(m_pHostWnd->m_hWnd) && hWnd && pCosmosDesignView->SubclassWindow(hWnd))
	{
		if (isAppWnd == false)
			::SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_CHILD | /*WS_VISIBLE | */WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		else
		{
			if (m_pParentObj && m_pParentObj->m_nViewType == Grid)
				m_pHostWnd->ModifyStyleEx(0, WS_EX_CLIENTEDGE);
		}
		::SetWindowLong(hWnd, GWL_ID, nID);

		pCosmosDesignView->m_bCreateExternal = true;
		if(m_nViewType==BlankView)
			m_nViewType = TabGrid;
		bRet = true;
	}

	HWND hPWnd = NULL;
	CString _strURL = m_pHostParse->attr(_T("url"), _T(""));
	if (_strURL == _T("host"))
	{
		if (g_pCosmos->m_hTempBrowserWnd)
		{
			hPWnd = g_pCosmos->m_hTempBrowserWnd;
			::SetWindowPos(hPWnd, HWND_BOTTOM, 0, 0, rect.right - rect.left, rect.bottom - rect.top, /*|SWP_SHOWWINDOW|SWP_NOSENDCHANGING*/SWP_NOREDRAW | SWP_NOACTIVATE);
			hWnd = ::GetParent(hPWnd);
			::SetWindowLongPtr(hWnd, GWLP_ID, nID);
			::SetParent(hWnd, pParentWnd->m_hWnd);
			::SetWindowLongPtr(hWnd, GWL_STYLE, ::GetWindowLongPtr(hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_BORDER | WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION) | WS_CHILD | WS_VISIBLE);
			g_pCosmos->m_hTempBrowserWnd = NULL;
			if (::IsWindow(m_pHostWnd->m_hWnd) == false)
				bRet = m_pHostWnd->SubclassWindow(hWnd);
		}
		else if (g_pCosmos->m_pHtmlWndCreated == nullptr)
		{
			hPWnd = g_pCosmos->m_hHostBrowserWnd;
		}
		else
		{
			hPWnd = ::GetParent(g_pCosmos->m_pHtmlWndCreated->m_hWnd);
		}
		if (::IsWindow(hPWnd) && (::GetWindowLongPtr(hPWnd, GWL_STYLE) & WS_CHILD)) {
			auto it = g_pCosmos->m_mapBrowserWnd.find(hPWnd);
			if (it != g_pCosmos->m_mapBrowserWnd.end())
			{
				m_pWebBrowser = (CBrowser*)it->second;
				if (m_pWebBrowser->m_pVisibleWebWnd)
				{
					::SetParent(m_pWebBrowser->m_pVisibleWebWnd->m_hExtendWnd, hPWnd);
					::ShowWindow(m_pWebBrowser->m_pVisibleWebWnd->m_hExtendWnd, SW_SHOW);
					if(m_pWebBrowser->m_pVisibleWebWnd->m_pChromeRenderFrameHost)
						m_pWebBrowser->m_pVisibleWebWnd->m_pChromeRenderFrameHost->ShowWebPage(true);
				}
				m_pWebBrowser->BrowserLayout();
				g_pCosmos->m_hParent = NULL;
				m_pRootObj->m_pXobjShareData->m_pGalaxy->m_strHostWebBrowserNodeName = m_strName;
				m_pRootObj->m_pXobjShareData->m_pGalaxy->m_pHostWebBrowserNode = this;
				m_pRootObj->m_pXobjShareData->m_pGalaxy->m_pHostWebBrowserWnd = m_pWebBrowser;
			}
		}
	}

	if (hWnd == 0)
	{
		hWnd = CreateWindow(L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);
		if (::IsWindow(m_pHostWnd->m_hWnd) == false)
		{
			bRet = m_pHostWnd->SubclassWindow(hWnd);
			if (m_nViewType == BlankView &&
				m_strID.CompareNoCase(_T("mdiclient")) == 0 &&
				g_pCosmos->m_pMDIMainWnd &&
				::IsChild(g_pCosmos->m_pMDIMainWnd->m_hWnd, hWnd)
				&& !::IsChild(g_pCosmos->m_pMDIMainWnd->m_hMDIClient, hWnd))
			{
				g_pCosmos->m_pMDIMainWnd->m_vMdiClientXobjs.push_back(this);
			}
		}
	}

	if (m_nViewType == BlankView && m_pHostParse != nullptr)
	{
		CString _strURL = m_pHostParse->attr(_T("url"), _T(""));
		HWND hPWnd = NULL;
		if (_strURL == _T("host"))
		{
			::PostMessage(hWnd, WM_COSMOSMSG, (WPARAM)m_pWebBrowser, 20201028);
		}
		else
		{
			if (m_pWebBrowser == nullptr && _strURL != _T(""))
			{
				_strURL += _T("|");
				CString s = _T("");
				int nPos = _strURL.Find(_T("|"));
				while (nPos != -1) {
					CString strURL = _strURL.Left(nPos);
					int nPos2 = strURL.Find(_T(":"));
					if (nPos2 != -1)
					{
						CString strURLHeader = strURL.Left(nPos2);
						if (strURLHeader.CompareNoCase(_T("host")) == 0)
						{
							strURL = g_pCosmos->m_strAppPath + strURL.Mid(nPos2 + 1);
						}
					}
					s += strURL;
					s += _T("|");
					_strURL = _strURL.Mid(nPos + 1);
					nPos = _strURL.Find(_T("|"));
				}

				if (g_pCosmos->m_pBrowserFactory)
				{
					HWND hBrowser = g_pCosmos->m_pBrowserFactory->CreateBrowser(hWnd, s);
					((CXobjWnd*)m_pHostWnd)->m_hFormWnd = hBrowser;
					g_pCosmos->m_hParent = NULL;
					auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
					if (it != g_pCosmos->m_mapBrowserWnd.end())
					{
						m_pWebBrowser = (CBrowser*)it->second;
					}
				}
				else
				{
					g_pCosmos->m_mapXobjForHtml[this] = s;
				}
			}
		}
	}
	if (m_strObjTypeID.CompareNoCase(_T("docwebhost")) == 0)
	{
		::PostMessage(m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20210225);
	}

	if (m_strID.CompareNoCase(_T("TreeView")) == 0)
	{
		CComBSTR bstrStyle(L"");
		get_Attribute(CComBSTR(L"Style"), &bstrStyle);
		m_nViewType = TangramTreeView;
		CString _strStyle = OLE2T(bstrStyle);
		if (_strStyle != _T(""))
		{
			pCosmosDesignView->m_pXHtmlTree = new CProgressFX< CHourglassFX< CTangramHtmlTreeEx2Wnd > >;
		}
		else
			pCosmosDesignView->m_pXHtmlTree = new CTangramHtmlTreeWnd();
		m_pDisp = pCosmosDesignView->m_pXHtmlTree->m_pObj;
		m_pDisp->AddRef();
		pCosmosDesignView->m_pXHtmlTree->m_pHostWnd = pCosmosDesignView;
		bRet = true;

		DWORD dwStyle = TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT |
			TVS_SHOWSELALWAYS | /*TVS_EDITLABELS |TVS_NOTOOLTIPS |*/
			WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP;

		CRect rect(0, 0, 100, 100);
		VERIFY(pCosmosDesignView->m_pXHtmlTree->Create(dwStyle, rect, pCosmosDesignView, 100));

		CComBSTR bstrCheckBox(L"");
		get_Attribute(CComBSTR(L"CheckBoxes"), &bstrCheckBox);
		CString strCheckBox = OLE2T(bstrCheckBox);
		CComBSTR bstrSmartCheckBox(L"");
		get_Attribute(CComBSTR(L"SmartCheckBox"), &bstrSmartCheckBox);
		CString strSmartCheckBox = OLE2T(bstrSmartCheckBox);
		CComBSTR bstrSetHtml(L"");
		get_Attribute(CComBSTR(L"SetHtml"), &bstrSetHtml);
		CString strSetHtml = OLE2T(bstrSetHtml);
		CComBSTR bstrStripHtml(L"");
		get_Attribute(CComBSTR(L"StripHtml"), &bstrStripHtml);
		CString strStripHtml = OLE2T(bstrStripHtml);
		CComBSTR bstrImages(L"");
		get_Attribute(CComBSTR(L"Images"), &bstrImages);
		CString strImages = OLE2T(bstrImages);
		CComBSTR bstrReadOnly(L"");
		get_Attribute(CComBSTR(L"ReadOnly"), &bstrReadOnly);
		CString strReadOnly = OLE2T(bstrReadOnly);

		int r = 0, g = 0, b = 0;
		CComBSTR bstrBKColor(L"");
		get_Attribute(CComBSTR(L"BKColor"), &bstrBKColor);
		CString strBKColor = OLE2T(bstrBKColor);

		COLORREF colorBK = RGB(255, 255, 255);
		if (strBKColor != _T(""))
		{
			_stscanf_s(strBKColor, _T("RGB(%d,%d,%d)"), &r, &g, &b);
			colorBK = RGB(r, g, b);
		}

		CComBSTR bstrSeparatorColor(L"");
		get_Attribute(CComBSTR(L"SeparatorColor"), &bstrSeparatorColor);
		CString strSeparatorColor = OLE2T(bstrSeparatorColor);

		COLORREF colorSeparator = RGB(0, 0, 255);
		if (bstrSeparatorColor != _T(""))
		{
			_stscanf_s(strSeparatorColor, _T("RGB(%d,%d,%d)"), &r, &g, &b);
			colorSeparator = RGB(r, g, b);
		}
		pCosmosDesignView->m_pXHtmlTree->Initialize(strCheckBox != _T("") ? true : false, true)
			.SetSmartCheckBox(strSmartCheckBox != _T("") ? true : false)
			.SetHtml(true)
			//.SetHtml(strSetHtml != _T("") ? true : false)
			.SetStripHtml(strStripHtml != _T("") ? true : false)
			.SetImages(strImages != _T("") ? true : false)
			.SetReadOnly(strReadOnly != _T("") ? true : false)
			.SetDragOps(XHTMLTREE_DO_DEFAULT)
			.SetSeparatorColor(colorSeparator).SetBkColor(colorBK);
		//.SetDropCursors(IDC_NODROP, IDC_DROPCOPY, IDC_DROPMOVE);
		if (strImages != _T(""))
		{
			CComBSTR bstrImageURL(L"");
			get_Attribute(CComBSTR(L"ImageURL"), &bstrImageURL);
			pCosmosDesignView->m_pXHtmlTree->m_strImageURL = OLE2T(bstrImageURL);
			get_Attribute(CComBSTR(L"ImageTarget"), &bstrImageURL);
			CString strImage = OLE2T(bstrImageURL);
			if (strImage != _T(""))
			{
				::GetTempPath(MAX_PATH, g_pCosmos->m_szBuffer);
				CString strPath = CString(g_pCosmos->m_szBuffer);
				strPath += _T("TangramTreeNode");
				strPath += strImage;
				pCosmosDesignView->m_pXHtmlTree->m_strImageTarget = strPath;
				int nPos = strPath.ReverseFind('\\');
				CString strDir = strPath.Left(nPos);
				SHCreateDirectory(NULL, strDir);
				if (::PathFileExists(strPath) == false)
				{
				}
				else
				{
					CImage image;
					image.Load(strPath);
					int nWidet = image.GetWidth();
					int nHeight = image.GetHeight();

					COLORREF color = image.GetTransparentColor();

					HBITMAP hBitmap;
					CBitmap* pBitmap;
					hBitmap = (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), (LPCTSTR)strPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					pBitmap = new CBitmap;
					pBitmap->Attach(hBitmap);
					pCosmosDesignView->m_pXHtmlTree->m_Images.Add(pBitmap, color);
					delete pBitmap;
				}
			}
		}

		if (nCol)
		{
			CTangramXmlParse* pTreeNodeParse = m_pHostParse->GetChild(_T("TreeNode"));
			if (pTreeNodeParse)
			{
				CComBSTR bstrTag(L"");
				get_Attribute(CComBSTR(L"doctag"), &bstrTag);
				CString strTag = OLE2T(bstrTag);
				if (strTag.CompareNoCase(_T("tangramdesigner")) == 0)
				{
					CString strKey = L"tangramdesignerxml";
					auto it = g_pCosmos->m_mapValInfo.find(strKey);
					if (it != g_pCosmos->m_mapValInfo.end())
					{
						CString strXml = OLE2T(it->second.bstrVal);
						::VariantClear(&it->second);
						g_pCosmos->m_mapValInfo.erase(it);
						CTangramXmlParse* pParse = new CTangramXmlParse();
						if (pParse->LoadXml(strXml))
						{
							pCosmosDesignView->m_pXHtmlTree->m_pHostXmlParse = pParse;
							m_pRootObj->m_pDocXmlParseNode = pParse;
							pCosmosDesignView->m_pXHtmlTree->m_hFirstRoot = pCosmosDesignView->m_pXHtmlTree->LoadXmlFromXmlParse(pParse);
						}
					}
				}
				else
				{
					CComBSTR bstrType(L"");
					get_Attribute(CComBSTR(L"doctype"), &bstrType);
					CString strType = OLE2T(bstrType);
					if (bstrType != _T(""))
					{
						CXHtmlDraw::XHTMLDRAW_APP_COMMAND AppCommands[] =
						{
							{ pCosmosDesignView->m_pXHtmlTree->m_hWnd, WM_HUBBLE_DESIGNERCMD, 1992, _T("WM_HUBBLE__DESIGNERCMD") },
							{ pCosmosDesignView->m_pXHtmlTree->m_hWnd, WM_HUBBLE_DESIGNERCMD, 1963, _T("WM_HUBBLE__DESIGNERCMD2") },
						};

						pCosmosDesignView->m_pXHtmlTree->m_Links.SetAppCommands(AppCommands, sizeof(AppCommands) / sizeof(AppCommands[0]));
					}
					else
					{
						CString strXml = pTreeNodeParse->xml();
						CTangramXmlParse* pParse = new CTangramXmlParse();
						pParse->LoadXml(strXml);
						pCosmosDesignView->m_pXHtmlTree->m_hFirstRoot = pCosmosDesignView->m_pXHtmlTree->LoadXmlFromXmlParse(pParse);
					}
				}
			}
		}
	}
	bRet = true;

	//Very important:
	if (m_pHostWnd && ::IsWindow(m_pHostWnd->m_hWnd))
		m_pHostWnd->SendMessage(WM_INITIALUPDATE);

	////////////////////////////////////////////////////////////////////////////////////////////////
	NodeCreated();
	m_pXobjShareData->m_mapLayoutNodes[m_strName] = this;
	if (m_strID.CompareNoCase(_T("treeview")))
	{
		m_nRows = 1;
		m_nCols = nCol;

		if (nCol&&m_pHostParse->GetChild(TGM_XOBJ))
		{
			m_nViewType = TabGrid;
			if (m_nActivePage<0 || m_nActivePage>nCol - 1)
				m_nActivePage = 0;
			CWnd * pView = nullptr;
			CXobj * pObj = nullptr;
			int j = 0;
			for (int i = 0; i < nCol; i++)
			{
				CTangramXmlParse* pChild = m_pHostParse->GetChild(i);
				CString _strName = pChild->name();
				CString strName = pChild->attr(_T("id"), _T(""));
				if (_strName.CompareNoCase(TGM_XOBJ) == 0)
				{
					strName.Trim();
					strName.MakeLower();

					pObj = new CComObject<CXobj>;
					pObj->m_pRootObj = m_pRootObj;
					pObj->m_pHostParse = pChild;
					AddChildNode(pObj);
					pObj->m_nCol = j;
					pObj->InitWndXobj();

					if (pObj->m_pObjClsInfo)
					{
						pContext->m_pNewViewClass = pObj->m_pObjClsInfo;
						pView = (CWnd*)pContext->m_pNewViewClass->CreateObject();
						if (pObj->m_strID.CompareNoCase(_T("tabctrl")) == 0)
						{
							CTangramTabCtrl* pTabCtrl = (CTangramTabCtrl*)pView;
							pTabCtrl->Create(WS_CHILD, rect, m_pHostWnd, 0);
						}
						else
							pView->Create(NULL, _T(""), WS_CHILD, rect, m_pHostWnd, 0, pContext);
						HWND m_hChild = (HWND)::SendMessage(m_pHostWnd->m_hWnd, WM_CREATETABPAGE, (WPARAM)pView->m_hWnd, (LPARAM)LPCTSTR(pObj->m_strCaption));
					}
					j++;
				}
			}
			::SendMessage(m_pHostWnd->m_hWnd, WM_ACTIVETABPAGE, (WPARAM)m_nActivePage, (LPARAM)1);
			Fire_TabChange(m_nActivePage, -1);
			m_pXobjShareData->m_pGalaxyCluster->Fire_TabChange(this, m_nActivePage, -1);
		}
	}

	m_pHostWnd->SetWindowText(m_strNodeName);
	if (m_nViewType == TabGrid && m_pParentObj && m_pParentObj->m_nViewType == Grid)
	{
		if (m_pHostWnd)
			m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
	}
	if (g_pCosmos->m_pActiveXobj && g_pCosmos->m_pActiveXobj->m_pXobjShareData->m_pGalaxyCluster)
		g_pCosmos->m_pActiveXobj->m_pXobjShareData->m_pGalaxyCluster->Fire_NodeCreated(this);

	return bRet;
}

CXobj* CXobj::GetMdiclientObj()
{
	if (g_pCosmos->m_pMDIMainWnd == nullptr)
		return nullptr;
	HWND hWnd = m_pHostWnd->m_hWnd;
	if (::IsChild(g_pCosmos->m_pMDIMainWnd->m_hWnd, hWnd) == false)
		return nullptr;
	if (m_pHostGalaxy && m_pHostGalaxy->m_nGalaxyType != CtrlBarGalaxy)
	{
		auto it = m_pHostGalaxy->m_pWorkXobj->m_mapChildXobj.find(_T("mdiclient"));
		if (it != m_pHostGalaxy->m_pWorkXobj->m_mapChildXobj.end())
		{
			CXobj* pObj = it->second->GetMdiclientObj();
			if (pObj == nullptr)
				return it->second;
			while (pObj)
			{
				pObj = it->second->GetMdiclientObj();
				if (pObj == nullptr)
					return it->second;
			}
		}
		return nullptr;
	}
	else
		return nullptr;
}

void CXobj::NodeCreated()
{
	CosmosInfo* pInfo = new CosmosInfo();
	pInfo->m_pXobj = this;
	pInfo->m_pGalaxy = m_pXobjShareData->m_pGalaxy;
	pInfo->m_pGalaxyCluster = m_pXobjShareData->m_pGalaxyCluster;
	pInfo->m_strName = m_strName;
	pInfo->m_strNodeName = m_strNodeName;
	::SetProp(m_pHostWnd->m_hWnd, _T("CosmosInfo"), pInfo);
	m_pHostParse->put_attr(_T("name"), (__int64)m_pHostWnd->m_hWnd);
	CWebPage* pHtmlWnd = GetHtmlWnd();
	if (m_pXobjShareData->m_pGalaxy->m_pWebPageWnd == nullptr && pHtmlWnd)
		m_pXobjShareData->m_pGalaxy->m_pWebPageWnd = pHtmlWnd;
	if (pHtmlWnd == nullptr)
		pHtmlWnd = m_pXobjShareData->m_pGalaxy->m_pWebPageWnd;
	if (pHtmlWnd == nullptr)
		pHtmlWnd = g_pCosmos->m_pHostHtmlWnd;
	if (pHtmlWnd && m_pWormhole == nullptr)
	{
		::PostMessage(pHtmlWnd->m_hWnd, WM_COSMOSMSG, 20200310, (LPARAM)this);
	}
}

HWND CXobj::CreateView(HWND hParentWnd, CString strTag)
{
	BOOL bWebCtrl = false;
	CString strURL = _T("");
	CString strID = strTag;
	CString strName = m_strName;

	CWebPage* pHtmlWnd = nullptr;
	HWND _hWnd = m_pXobjShareData->m_pGalaxy->m_hWnd;
	{
		::GetClassName(_hWnd, g_pCosmos->m_szBuffer, 256);
		CString strName = CString(g_pCosmos->m_szBuffer);
		if (strName == _T("Chrome Extended Window Class")) {
			pHtmlWnd = (CWebPage*)::GetWindowLongPtr(_hWnd, GWLP_USERDATA);
		}
	}
	if (pHtmlWnd == nullptr)
	{
		pHtmlWnd = m_pXobjShareData->m_pGalaxy->m_pWebPageWnd;
	}
	switch (m_nViewType)
	{
	case ActiveX:
	{
		strID.MakeLower();

		if (m_pDisp == nullptr)
		{
			CComPtr<IDispatch> pDisp;
			HRESULT hr = pDisp.CoCreateInstance(CComBSTR(strID));
			if (hr == S_OK)
			{
				m_pDisp = pDisp.Detach();
			}
		}
	}
	break;
	case CLRCtrl:
	{
		g_pCosmos->m_pActiveXobj = this;
		
		if (g_pCosmos->m_pCLRProxy)
		{
			if (pHtmlWnd)
			{
				g_pCosmos->m_pCLRProxy->m_strCurrentWinFormTemplate = m_pHostParse->xml();
			}
			m_pDisp = g_pCosmos->m_pCLRProxy->CreateObject(strTag.AllocSysString(), hParentWnd, this);
			if (g_pCosmos->m_hFormNodeWnd)
			{
				LRESULT l = ::SendMessage((HWND)g_pCosmos->m_hFormNodeWnd, WM_HUBBLE_DATA, 0, 20190214);
				if (l&& pHtmlWnd)
				{
					auto it = pHtmlWnd->m_mapWinForm.find(g_pCosmos->m_hFormNodeWnd);
					if (it == pHtmlWnd->m_mapWinForm.end())
					{
						pHtmlWnd->m_mapWinForm[g_pCosmos->m_hFormNodeWnd] = (CWinForm*)l;
					}
				}
			}

			CXobjWnd* pWnd = (CXobjWnd*)m_pHostWnd;
			if (m_pDisp == nullptr)
			{
				pWnd->m_bCreateExternal = false;
				m_nViewType = BlankView;
			}
			if (m_strID.CollateNoCase(_T("wpfctrl")) == 0)
			{
				pWnd->m_hFormWnd = g_pCosmos->m_hFormNodeWnd;
				g_pCosmos->m_hFormNodeWnd = NULL;
			}
		}
	}
	break;
	}
	if (m_pDisp)
	{
		m_pXobjShareData->m_mapLayoutNodes[m_strName] = this;
		if (m_nViewType == CLRCtrl)
		{
			if (g_pCosmos->m_hFormNodeWnd && (::GetWindowLongPtr(g_pCosmos->m_hFormNodeWnd, GWL_STYLE) & WS_CHILD))
			{
				HWND hCtrl = NULL;
				if (g_pCosmos->m_pCLRProxy)
					hCtrl = g_pCosmos->m_pCLRProxy->GetCtrlHandle(m_pDisp);
				if (hCtrl == g_pCosmos->m_hFormNodeWnd)
				{
					HWND hWnd = g_pCosmos->m_hFormNodeWnd;
					g_pCosmos->m_hFormNodeWnd = nullptr;
					return hWnd;
				}
			}
		}
		auto hWnd = ::CreateWindowEx(NULL, L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hParentWnd, NULL, AfxGetInstanceHandle(), NULL);
		CAxWindow m_Wnd;
		m_Wnd.Attach(hWnd);
		CComPtr<IUnknown> pUnk;
		m_Wnd.AttachControl(m_pDisp, &pUnk);
		if (m_nViewType == ActiveX)
		{
			((CXobjWnd*)m_pHostWnd)->m_pXobj = this;
			HWND hPage = m_pXobjShareData->m_pGalaxyCluster->m_hWnd;
			::SendMessage(hPage, WM_COSMOSMSG, (WPARAM)((CXobjWnd*)m_pHostWnd), 1963);
		}

		CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pDisp);
		if (pIOleInPlaceActiveObject)
			((CXobjWnd*)m_pHostWnd)->m_pOleInPlaceActiveObject = pIOleInPlaceActiveObject.Detach();
		m_Wnd.Detach();
		return hWnd;
	}

	return 0;
}

STDMETHODIMP CXobj::get_ChildNodes(IXobjCollection * *pXobjColletion)
{
	if (m_pChildNodeCollection == nullptr)
	{
		CComObject<CXobjCollection>::CreateInstance(&m_pChildNodeCollection);
		m_pChildNodeCollection->AddRef();
		m_pChildNodeCollection->m_pXobjs = &m_vChildNodes;
	}
	return m_pChildNodeCollection->QueryInterface(IID_IXobjCollection, (void**)pXobjColletion);
}

int CXobj::_getNodes(CXobj * pXobj, CString & strName, CXobj * *ppRetXobj, CXobjCollection * pXobjs)
{
	int iCount = 0;
	if (pXobj->m_strName.CompareNoCase(strName) == 0)
	{
		if (pXobjs != nullptr)
			pXobjs->m_pXobjs->push_back(pXobj);

		if (ppRetXobj != nullptr && (*ppRetXobj) == nullptr)
			* ppRetXobj = pXobj;
		return 1;
	}

	for (auto it : pXobj->m_vChildNodes)
	{
		iCount += _getNodes(it, strName, ppRetXobj, pXobjs);
	}
	return iCount;
}

STDMETHODIMP CXobj::Show()
{
	CXobj* pChild = this;
	CXobj* pParent = pChild->m_pParentObj;

	while (pParent != nullptr)
	{
		pParent->m_pHostWnd->SendMessage(WM_ACTIVETABPAGE, (WPARAM)pChild->m_nCol, (LPARAM)1);

		pChild = pParent;
		pParent = pChild->m_pParentObj;
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_RootXobj(IXobj * *ppXobj)
{
	if (m_pRootObj != nullptr)
		* ppXobj = m_pRootObj;
	return S_OK;
}

STDMETHODIMP CXobj::get_ParentXobj(IXobj * *ppXobj)
{
	*ppXobj = nullptr;
	if (m_pParentObj != nullptr)
		* ppXobj = m_pParentObj;

	return S_OK;
}

STDMETHODIMP CXobj::get_XobjType(XobjType* nType)
{
	*nType = m_nViewType;
	return S_OK;
}

void CXobj::_get_Objects(CXobj * pXobj, UINT32 & nType, CXobjCollection * pXobjColletion)
{
	if (pXobj->m_nViewType & nType)
	{
		pXobjColletion->m_pXobjs->push_back(pXobj);
	}

	CXobj* pChildNode = nullptr;
	for (auto it : pXobj->m_vChildNodes)
	{
		pChildNode = it;
		_get_Objects(pChildNode, nType, pXobjColletion);
	}
}

STDMETHODIMP CXobj::get_Objects(long nType, IXobjCollection * *ppXobjColletion)
{
	CComObject<CXobjCollection>* pXobjs = nullptr;
	CComObject<CXobjCollection>::CreateInstance(&pXobjs);

	pXobjs->AddRef();

	UINT32 u = nType;
	_get_Objects(this, u, pXobjs);
	HRESULT hr = pXobjs->QueryInterface(IID_IXobjCollection, (void**)ppXobjColletion);

	pXobjs->Release();

	return hr;
}

STDMETHODIMP CXobj::get_Rows(long* nRows)
{
	*nRows = m_nRows;
	return S_OK;
}

STDMETHODIMP CXobj::get_Cols(long* nCols)
{
	*nCols = m_nCols;
	if (m_vChildNodes.size() && m_nRows * m_nCols == 0)
	{
		*nCols = m_vChildNodes.size();
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_Row(long* nRow)
{
	*nRow = m_nRow;
	return S_OK;
}

STDMETHODIMP CXobj::get_Col(long* nCol)
{
	*nCol = m_nCol;
	return S_OK;
}

STDMETHODIMP CXobj::GetXobj(long nRow, long nCol, IXobj** ppXobj)
{
	CXobj* pRet = nullptr;

	*ppXobj = nullptr;
	if (nRow < 0 || nCol < 0 || nRow >= m_nRows || nCol >= m_nCols) return E_INVALIDARG;
	//if (m_nViewType == Grid)
	//{
	//	CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
	//	HWND hWnd = ::GetDlgItem(pSplitter->m_hWnd, pSplitter->IdFromRowCol(nRow, nCol));
	//	LRESULT lRes = ::SendMessage(hWnd, WM_HUBBLE_GETNODE, 0, 0);
	//	if (lRes)
	//	{
	//		pRet = (CXobj*)lRes;
	//		pRet->QueryInterface(IID_IXobj, (void**)ppXobj);
	//		return S_OK;
	//	}
	//	return S_FALSE;
	//}

	for (auto it : m_vChildNodes)
	{
		if (it->m_nCol == nCol && it->m_nRow == nRow)
		{
			pRet = it;
			break;
		}
	}

	HRESULT hr = S_OK;
	if (pRet)
	{
		hr = pRet->QueryInterface(IID_IXobj, (void**)ppXobj);
	}
	return hr;
}

STDMETHODIMP CXobj::GetChildXobjByName(BSTR bstrName, IXobj** ppXobj)
{
	CString strName = OLE2T(bstrName);
	auto it = m_pRootObj->m_mapChildXobj.find(strName);
	if (it != m_pRootObj->m_mapChildXobj.end())
	{
		*ppXobj = it->second;
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CXobj::GetXobjByName(BSTR bstrName, IXobjCollection * *ppXobjs)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strName(bstrName);

	CXobj* pRetNode = nullptr;

	CComObject<CXobjCollection>* pXobjs = nullptr;
	if (ppXobjs != nullptr)
	{
		*ppXobjs = nullptr;
		CComObject<CXobjCollection>::CreateInstance(&pXobjs);
		pXobjs->AddRef();
	}

	int iCount = _getNodes(this, strName, &pRetNode, pXobjs);

	if (ppXobjs != nullptr)
		pXobjs->QueryInterface(IID_IXobjCollection, (void**)ppXobjs);

	if (pXobjs != nullptr)
		pXobjs->Release();

	return S_OK;
}

STDMETHODIMP CXobj::GetUIScript(BSTR bstrCtrlName, BSTR* bstrVal)
{
	*bstrVal = CComBSTR(m_pHostParse->xml());
	return S_OK;
}

STDMETHODIMP CXobj::GetXobjs(BSTR bstrName, IXobj * *ppXobj, IXobjCollection * *ppXobjs, long* pCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strName(bstrName);

	CXobj* pRetNode = nullptr;

	if (ppXobj != nullptr)
		* ppXobj = nullptr;

	CComObject<CXobjCollection> * pXobjs = nullptr;
	if (ppXobjs != nullptr)
	{
		*ppXobjs = nullptr;
		CComObject<CXobjCollection>::CreateInstance(&pXobjs);
		pXobjs->AddRef();
	}

	int iCount = _getNodes(this, strName, &pRetNode, pXobjs);

	*pCount = iCount;

	if ((iCount > 0) && (ppXobj != nullptr))
		pRetNode->QueryInterface(IID_IXobj, (void**)ppXobj);

	if (ppXobjs != nullptr)
		pXobjs->QueryInterface(IID_IXobjCollection, (void**)ppXobjs);

	if (pXobjs != nullptr)
		pXobjs->Release();

	return S_OK;
}

BOOL CXobj::AddChildNode(CXobj * pXobj)
{
	m_vChildNodes.push_back(pXobj);
	pXobj->m_pParentObj = this;
	pXobj->m_pRootObj = m_pRootObj;
	return true;
}

BOOL CXobj::RemoveChildNode(CXobj * pXobj)
{
	auto it = find(m_vChildNodes.begin(), m_vChildNodes.end(), pXobj);
	if (it != m_vChildNodes.end())
	{
		m_vChildNodes.erase(it);
		return true;
	}
	return false;
}

STDMETHODIMP CXobj::get_Galaxy(IGalaxy * *pVal)
{
	if (m_pXobjShareData->m_pGalaxy)
		* pVal = m_pXobjShareData->m_pGalaxy;

	return S_OK;
}

STDMETHODIMP CXobj::get_HostGalaxy(IGalaxy * *pVal)
{
	if (m_pHostGalaxy)
		* pVal = m_pHostGalaxy;

	return S_OK;
}

STDMETHODIMP CXobj::get_Height(LONG * pVal)
{
	RECT rc;
	::GetClientRect(m_pHostWnd->m_hWnd, &rc);
	*pVal = rc.bottom;
	return S_OK;
}

STDMETHODIMP CXobj::get_Width(LONG * pVal)
{
	RECT rc;
	::GetClientRect(m_pHostWnd->m_hWnd, &rc);
	*pVal = rc.right;

	return S_OK;
}

STDMETHODIMP CXobj::get_WebPage(IWebPage * *pVal)
{
	if (m_pWebPage == nullptr)
	{
		m_pWebPage = GetHtmlWnd();
	}
	if (m_pWebPage)
	{
		*pVal = m_pWebPage;
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_OfficeObj(IDispatch * *pVal)
{
	if (m_pXobjShareData->m_pOfficeObj)
	{
		*pVal = m_pXobjShareData->m_pOfficeObj;
		(*pVal)->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_Extender(IDispatch * *pVal)
{
	if (m_pExtender)
	{
		*pVal = m_pExtender;
		(*pVal)->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CXobj::put_Extender(IDispatch * newVal)
{
	if (m_pExtender)
		m_pExtender->Release();
	m_pExtender = newVal;
	m_pExtender->AddRef();

	return S_OK;
}

STDMETHODIMP CXobj::get_GalaxyCluster(IGalaxyCluster * *pVal)
{
	*pVal = (IGalaxyCluster*)m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster;
	return S_OK;
}

STDMETHODIMP CXobj::get_NameAtWindowPage(BSTR * pVal)
{
	*pVal = m_strNodeName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CXobj::GetCtrlByName(BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch * *ppRetDisp)
{
	if (g_pCosmos->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
		* ppRetDisp = g_pCosmos->m_pCLRProxy->GetCtrlByName(m_pDisp, bstrName, bFindInChild ? true : false);

	return S_OK;
}

STDMETHODIMP CXobj::GetCtrlValueByName(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR * bstrVal)
{
	if (g_pCosmos->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
	{
		*bstrVal = g_pCosmos->m_pCLRProxy->GetCtrlValueByName(m_pDisp, bstrName, bFindInChild ? true : false);
	}
	return S_OK;
}

STDMETHODIMP CXobj::SetCtrlValueByName(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal)
{
	if (g_pCosmos->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
	{
		g_pCosmos->m_pCLRProxy->SetCtrlValueByName(m_pDisp, bstrName, bFindInChild ? true : false, bstrVal);
	}
	return S_OK;
}

CXobjCollection::CXobjCollection()
{
	m_pXobjs = &m_vXobjs;
	g_pCosmos->m_mapWndXobjCollection[(__int64)this] = this;
}

CXobjCollection::~CXobjCollection()
{
	auto it = g_pCosmos->m_mapWndXobjCollection.find((__int64)this);
	if (it != g_pCosmos->m_mapWndXobjCollection.end())
	{
		g_pCosmos->m_mapWndXobjCollection.erase(it);
	}
	m_vXobjs.clear();
}

STDMETHODIMP CXobjCollection::get_XobjCount(long* pCount)
{
	*pCount = (int)m_pXobjs->size();
	return S_OK;
}

STDMETHODIMP CXobjCollection::get_Item(long iIndex, IXobj * *ppXobj)
{
	if (iIndex < 0 || iIndex >= (int)m_pXobjs->size()) return E_INVALIDARG;

	CXobj * pXobj = m_pXobjs->operator [](iIndex);

	return pXobj->QueryInterface(IID_IXobj, (void**)ppXobj);
}

STDMETHODIMP CXobjCollection::get__NewEnum(IUnknown * *ppVal)
{
	*ppVal = nullptr;

	struct _CopyVariantFromIUnkown
	{
		static HRESULT copy(VARIANT* p1, CXobj* const* p2)
		{
			CXobj* pXobj = *p2;
			p1->vt = VT_UNKNOWN;
			return pXobj->QueryInterface(IID_IUnknown, (void**) & (p1->punkVal));
		}

		static void init(VARIANT* p) { VariantInit(p); }
		static void destroy(VARIANT* p) { VariantClear(p); }
	};

	typedef CComEnumOnSTL<IEnumVARIANT, & IID_IEnumVARIANT, VARIANT, _CopyVariantFromIUnkown, CXobjVector>
		CComEnumVariantOnVector;

	CComObject<CComEnumVariantOnVector>* pe = 0;
	HRESULT hr = CComObject<CComEnumVariantOnVector>::CreateInstance(&pe);

	if (SUCCEEDED(hr))
	{
		hr = pe->AddRef();
		hr = pe->Init(GetUnknown(), *m_pXobjs);

		if (SUCCEEDED(hr))
			hr = pe->QueryInterface(ppVal);

		hr = pe->Release();
	}

	return hr;
}

STDMETHODIMP CXobj::get_DocXml(BSTR * pVal)
{
	g_pCosmos->UpdateXobj(m_pRootObj);
	CString strXml = m_pXobjShareData->m_pCosmosParse->xml();
	strXml.Replace(_T("/><"), _T("/>\r\n<"));
	strXml.Replace(_T("/>"), _T("></xobj>"));
	*pVal = strXml.AllocSysString();
	strXml.ReleaseBuffer();

	return S_OK;
}

STDMETHODIMP CXobj::get_rgbMiddle(OLE_COLOR * pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = OLE_COLOR(pSplitter->rgbMiddle);
	}
	else
	{
		*pVal = OLE_COLOR(RGB(240, 240, 240));
	}
	return S_OK;
}

STDMETHODIMP CXobj::put_rgbMiddle(OLE_COLOR newVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		OleTranslateColor(newVal, NULL, &pSplitter->rgbMiddle);
		BYTE Red = GetRValue(pSplitter->rgbMiddle);
		BYTE Green = GetGValue(pSplitter->rgbMiddle);
		BYTE Blue = GetBValue(pSplitter->rgbMiddle);
		CString strRGB = _T("");
		strRGB.Format(_T("RGB(%d,%d,%d)"), Red, Green, Blue);
		put_Attribute(CComBSTR(L"middlecolor"), strRGB.AllocSysString());
		pSplitter->Invalidate();
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_rgbLeftTop(OLE_COLOR * pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = OLE_COLOR(pSplitter->rgbLeftTop);
	}
	else
	{
		*pVal = OLE_COLOR(RGB(240, 240, 240));
	}
	return S_OK;
}

STDMETHODIMP CXobj::put_rgbLeftTop(OLE_COLOR newVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		OleTranslateColor(newVal, NULL, &pSplitter->rgbLeftTop);
		CString strRGB = _T("");
		strRGB.Format(_T("RGB(%d,%d,%d)"), GetRValue(pSplitter->rgbLeftTop), GetGValue(pSplitter->rgbLeftTop), GetBValue(pSplitter->rgbLeftTop));
		put_Attribute(CComBSTR(L"lefttopcolor"), strRGB.AllocSysString());
		pSplitter->Invalidate();
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_rgbRightBottom(OLE_COLOR * pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = OLE_COLOR(pSplitter->rgbRightBottom);
	}
	else
		*pVal = OLE_COLOR(RGB(240, 240, 240));

	return S_OK;
}

STDMETHODIMP CXobj::put_rgbRightBottom(OLE_COLOR newVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		OleTranslateColor(newVal, NULL, &pSplitter->rgbRightBottom);
		BYTE Red = GetRValue(pSplitter->rgbRightBottom);
		BYTE Green = GetGValue(pSplitter->rgbRightBottom);
		BYTE Blue = GetBValue(pSplitter->rgbRightBottom);
		CString strRGB = _T("");
		strRGB.Format(_T("RGB(%d,%d,%d)"), Red, Green, Blue);
		put_Attribute(CComBSTR(L"rightbottomcolor"), strRGB.AllocSysString());
		pSplitter->Invalidate();
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_Hmin(int* pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = pSplitter->m_Hmin;
	}
	return S_OK;
}

STDMETHODIMP CXobj::put_Hmin(int newVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		pSplitter->m_Hmin = min(pSplitter->m_Hmax, newVal);
		CString strVal = _T("");
		strVal.Format(_T("%d"), pSplitter->m_Hmin);
		put_Attribute(CComBSTR(L"hmin"), strVal.AllocSysString());
	}

	return S_OK;
}

STDMETHODIMP CXobj::get_Hmax(int* pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = pSplitter->m_Hmax;
	}
	return S_OK;
}

STDMETHODIMP CXobj::put_Hmax(int newVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		pSplitter->m_Hmax = max(pSplitter->m_Hmin, newVal);
		CString strVal = _T("");
		strVal.Format(_T("%d"), pSplitter->m_Hmax);
		put_Attribute(CComBSTR(L"hmax"), strVal.AllocSysString());
	}

	return S_OK;
}

STDMETHODIMP CXobj::get_Vmin(int* pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = pSplitter->m_Vmin;
	}

	return S_OK;
}

STDMETHODIMP CXobj::put_Vmin(int newVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		pSplitter->m_Vmin = min(pSplitter->m_Vmax, newVal);
		CString strVal = _T("");
		strVal.Format(_T("%d"), pSplitter->m_Vmin);
		put_Attribute(CComBSTR(L"vmin"), strVal.AllocSysString());
	}

	return S_OK;
}

STDMETHODIMP CXobj::get_Vmax(int* pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = pSplitter->m_Vmax;
	}

	return S_OK;
}

STDMETHODIMP CXobj::put_Vmax(int newVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		pSplitter->m_Vmax = max(pSplitter->m_Vmin, newVal);
		CString strVal = _T("");
		strVal.Format(_T("%d"), pSplitter->m_Vmax);
		put_Attribute(CComBSTR(L"vmax"), strVal.AllocSysString());
	}

	return S_OK;
}


STDMETHODIMP CXobj::get_HostXobj(IXobj * *pVal)
{
	if (m_pXobjShareData->m_pHostClientView)
		* pVal = m_pXobjShareData->m_pHostClientView->m_pXobj;

	return S_OK;
}


STDMETHODIMP CXobj::put_HostXobj(IXobj * newVal)
{
	return S_OK;
}


STDMETHODIMP CXobj::get_ActivePage(int* pVal)
{
	if (m_nViewType == TabGrid)
	{
		CComBSTR bstr(L"");
		get_Attribute(CComBSTR(L"activepage"), &bstr);
		*pVal = _wtoi(OLE2T(bstr));
	}
	return S_OK;
}


STDMETHODIMP CXobj::put_ActivePage(int newVal)
{
	if (this->m_nViewType == TabGrid && newVal < m_nCols)
	{
		HWND hwnd = nullptr;
		int nOldPage = 0;
		get_ActivePage(&nOldPage);
		if (nOldPage == newVal)
			return S_OK;
		IXobj * pOldNode = nullptr;
		GetXobj(0, newVal, &pOldNode);
		if (pOldNode)
		{
			LONGLONG h = 0;
			pOldNode->get_Handle(&h);
			hwnd = (HWND)h;
			if (::IsWindow(hwnd))
			{
				::ShowWindow(hwnd, SW_HIDE);
			}
		}
		m_pHostWnd->SendMessage(WM_ACTIVETABPAGE, (WPARAM)newVal, (LPARAM)1);
		IXobj* pXobj = nullptr;
		this->GetXobj(0, newVal, &pXobj);
		if (pXobj)
		{
			::ShowWindow(hwnd, SW_HIDE);
			ActiveTabPage(pXobj);
		}
	}

	return S_OK;
}

STDMETHODIMP CXobj::get_MasterRow(int* pVal)
{
	return S_OK;
}

STDMETHODIMP CXobj::put_MasterRow(int newVal)
{
	return S_OK;
}

STDMETHODIMP CXobj::get_MasterCol(int* pVal)
{
	return S_OK;
}

STDMETHODIMP CXobj::put_MasterCol(int newVal)
{
	return S_OK;
}

HRESULT CXobj::Fire_ObserveComplete()
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		DISPPARAMS params = { NULL, NULL, 0, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapWndXobjProxy)
	{
		it.second->OnObserverComplete();
	}

	return hr;
}

HRESULT CXobj::Fire_Destroy()
{
	if (m_pWebBrowser)
	{
		if(::IsChild(m_pHostWnd->m_hWnd,m_pWebBrowser->m_hWnd))
			m_pWebBrowser->DestroyWindow();
		m_pWebBrowser = nullptr;
	}
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		DISPPARAMS params = { NULL, NULL, 0, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	for (auto it : m_mapWndXobjProxy)
	{
		it.second->OnDestroy();
	}

	if (g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_pCLRProxy->ReleaseCosmosObj((IXobj*)this);
	}
	if (m_nViewType != TangramTreeView && m_nViewType != Grid && m_pDisp)
	{
		if (m_nViewType == CLRCtrl)
			g_pCosmos->m_pCLRProxy->ReleaseCosmosObj(m_pDisp);
		else
			CCommonFunction::ClearObject<IUnknown>(m_pDisp);
	}
	return hr;
}

HRESULT CXobj::Fire_XobjAddInCreated(IDispatch * pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[3];
		avarParams[2] = pAddIndisp;
		avarParams[2].vt = VT_DISPATCH;
		avarParams[1] = bstrAddInID;
		avarParams[1].vt = VT_BSTR;
		avarParams[0] = bstrAddInXml;
		avarParams[0].vt = VT_BSTR;
		DISPPARAMS params = { avarParams, NULL, 3, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapWndXobjProxy)
	{
		it.second->OnXobjAddInCreated(pAddIndisp, OLE2T(bstrAddInID), OLE2T(bstrAddInXml));
	}

	return hr;
}

HRESULT CXobj::Fire_XobjAddInsCreated()
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		DISPPARAMS params = { NULL, NULL, 0, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	for (auto it : m_mapWndXobjProxy)
	{
		it.second->OnXobjAddInsCreated();
	}
	return hr;
}

HRESULT CXobj::Fire_XobjDocumentComplete(IDispatch * ExtenderDisp, BSTR bstrURL)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[2];
		avarParams[1] = ExtenderDisp;
		avarParams[1].vt = VT_DISPATCH;
		avarParams[0] = bstrURL;
		avarParams[0].vt = VT_BSTR;
		DISPPARAMS params = { avarParams, NULL, 2, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapWndXobjProxy)
	{
		it.second->OnXobjDocumentComplete(ExtenderDisp, OLE2T(bstrURL));
	}

	return hr;
}

HRESULT CXobj::Fire_ControlNotify(IXobj * sender, LONG NotifyCode, LONG CtrlID, LONGLONG CtrlHandle, BSTR CtrlClassName)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[5];
		avarParams[4] = sender;
		avarParams[4].vt = VT_DISPATCH;
		avarParams[3] = NotifyCode;
		avarParams[3].vt = VT_I4;
		avarParams[2] = CtrlID;
		avarParams[2].vt = VT_I4;
		avarParams[1] = CtrlHandle;
		avarParams[1].vt = VT_I8;
		avarParams[0] = CtrlClassName;
		avarParams[0].vt = VT_BSTR;
		DISPPARAMS params = { avarParams, NULL, 5, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	if (g_pCosmos->m_bEclipse && m_pXobjShareData->m_pGalaxyCluster && m_pXobjShareData->m_pGalaxyCluster->m_mapNotifyCtrl.size())
	{
		map<HWND, CEclipseCtrl*> m_mapTemp;
		for (auto it : m_pXobjShareData->m_pGalaxyCluster->m_mapNotifyCtrl)
		{
			CEclipseCtrl* pCtrl = it.second;
			if (::IsWindow(it.first))
				pCtrl->Fire_ControlNotify(sender, NotifyCode, CtrlID, CtrlHandle, CtrlClassName);
			else
				m_mapTemp[it.first] = pCtrl;
		}
		if (m_mapTemp.size())
		{
			for (auto it : m_mapTemp)
				m_pXobjShareData->m_pGalaxyCluster->m_mapNotifyCtrl.erase(m_pXobjShareData->m_pGalaxyCluster->m_mapNotifyCtrl.find(it.first));
			m_mapTemp.clear();
		}
	}
	for (auto it : m_mapWndXobjProxy)
	{
		it.second->OnControlNotify(sender, NotifyCode, CtrlID, (HWND)CtrlHandle, OLE2T(CtrlClassName));
	}
	return hr;
}

HRESULT CXobj::Fire_TabChange(LONG ActivePage, LONG OldPage)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[2];
		avarParams[1] = ActivePage;
		avarParams[1].vt = VT_I4;
		avarParams[0] = OldPage;
		avarParams[0].vt = VT_I4;
		DISPPARAMS params = { avarParams, NULL, 2, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();
			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	for (auto it : m_mapWndXobjProxy)
	{
		it.second->OnTabChange(ActivePage, OldPage);
	}
	return hr;
}

HRESULT CXobj::Fire_IPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		CComVariant avarParams[5];
		avarParams[4] = bstrFrom;
		avarParams[4].vt = VT_BSTR;
		avarParams[3] = bstrTo;
		avarParams[3].vt = VT_BSTR;
		avarParams[2] = bstrMsgId;
		avarParams[2].vt = VT_BSTR;
		avarParams[1] = bstrPayload;
		avarParams[1].vt = VT_BSTR;
		avarParams[0] = bstrExtra;
		avarParams[0].vt = VT_BSTR;
		DISPPARAMS params = { avarParams, NULL, 5, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pCosmos->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pCosmos->Unlock();
			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(8, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	//for (auto it : m_mapWndXobjProxy)
	//{
	//	it.second->OnTabChange(ActivePage, OldPage);
	//}
	return hr;
}

STDMETHODIMP CXobj::NavigateURL(BSTR bstrURL, IDispatch * dispObjforScript)
{
	if (g_pCosmos->m_pBrowserFactory == nullptr)
		return S_OK;
	if (m_pWebBrowser == nullptr)
	{
		CString _strXml = OLE2T(bstrURL);
		_strXml += _T("|");
		_strXml.Replace(_T("||"), _T("|"));
		CString s = _T("");
		int nPos = _strXml.Find(_T("|"));
		while (nPos != -1) {
			CString strURL = _strXml.Left(nPos);
			int nPos2 = strURL.Find(_T(":"));
			if (nPos2 != -1)
			{
				CString strURLHeader = strURL.Left(nPos2);
				if (strURLHeader.CompareNoCase(_T("host")) == 0)
				{
					strURL = g_pCosmos->m_strAppPath + strURL.Mid(nPos2 + 1);
				}
			}
			s += strURL;
			s += _T("|");
			_strXml = _strXml.Mid(nPos + 1);
			nPos = _strXml.Find(_T("|"));
		}

		HWND hBrowser = g_pCosmos->m_pBrowserFactory->CreateBrowser(((CXobjWnd*)m_pHostWnd)->m_hWnd, s);
		((CXobjWnd*)m_pHostWnd)->m_hFormWnd = hBrowser;
		g_pCosmos->m_hParent = NULL;
		auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
		if (it != g_pCosmos->m_mapBrowserWnd.end())
		{
			m_pWebBrowser = (CBrowser*)it->second;
			//m_pWebBrowser->m_pXobj = this;
		}
		//g_pCosmos->m_pCurWebNode = nullptr;
		return S_OK;
	}
	else if (m_pWebBrowser)
	{
		m_pWebBrowser->OpenURL(bstrURL, BrowserWndOpenDisposition::SWITCH_TO_TAB, CComBSTR(""), CComBSTR(""));
		return S_OK;
	}
	return S_OK;
}

STDMETHODIMP CXobj::get_URL(BSTR * pVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strVal = m_pHostParse->attr(_T("url"), _T(""));
		*pVal = strVal.AllocSysString();
		strVal.ReleaseBuffer();
	}
	return S_OK;
}

STDMETHODIMP CXobj::put_URL(BSTR newVal)
{
	if (m_pWebBrowser)
	{
		m_pWebBrowser->DestroyWindow();
		m_pWebBrowser = nullptr;
	}
	if (m_pWebBrowser == nullptr)
	{
		CString _strXml = OLE2T(newVal);
		_strXml += _T("|");
		_strXml.Replace(_T("||"), _T("|"));
		CString s = _T("");
		int nPos = _strXml.Find(_T("|"));
		while (nPos != -1) {
			CString strURL = _strXml.Left(nPos);
			int nPos2 = strURL.Find(_T(":"));
			if (nPos2 != -1)
			{
				CString strURLHeader = strURL.Left(nPos2);
				if (strURLHeader.CompareNoCase(_T("host")) == 0)
				{
					strURL = g_pCosmos->m_strAppPath + strURL.Mid(nPos2 + 1);
				}
			}
			s += strURL;
			s += _T("|");
			_strXml = _strXml.Mid(nPos + 1);
			nPos = _strXml.Find(_T("|"));
		}

		HWND hBrowser = g_pCosmos->m_pBrowserFactory->CreateBrowser(m_pHostWnd->m_hWnd, s);
		((CXobjWnd*)m_pHostWnd)->m_hFormWnd = hBrowser;
		g_pCosmos->m_hParent = NULL;
		auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
		if (it != g_pCosmos->m_mapBrowserWnd.end())
		{
			m_pWebBrowser = (CBrowser*)it->second;
			m_pWebBrowser->m_pParentXobj = this;
		}

		return S_OK;
	}
	return S_OK;
}

STDMETHODIMP CXobj::SendIPCMessage(BSTR bstrTo, BSTR bstrPayload, BSTR bstrExtra, BSTR bstrMsgId, BSTR* bstrRet)
{
	if (m_pXobjShareData->m_pGalaxy->m_pWebPageWnd)
	{
		m_pXobjShareData->m_pGalaxy->m_pWebPageWnd->SendChromeIPCMessage(_T("I_DONT_KNOW"), OLE2T(bstrTo), OLE2T(bstrMsgId), OLE2T(bstrExtra), L"", L"");
	}
	else
	{
		if (m_pXobjShareData->m_pGalaxy->m_pHostWebBrowserNode)
		{
			if (m_pXobjShareData->m_pGalaxy->m_pHostWebBrowserWnd)
			{
				HWND hPWnd = m_pXobjShareData->m_pGalaxy->m_pHostWebBrowserWnd->m_hWnd;
				for (auto it : g_pCosmos->m_mapHtmlWnd)
				{
					if (::IsChild(hPWnd,it.first))
					{
						CWebPage* pWnd = (CWebPage*)it.second;
						pWnd->SendChromeIPCMessage(_T("bstrMsgId"), OLE2T(bstrTo), OLE2T(bstrMsgId), OLE2T(bstrExtra), OLE2T(bstrMsgId), _T(""));
					}
				}
			}
		}
	}
	return S_OK;
}
