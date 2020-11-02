/********************************************************************************
*					Open Universe - version 1.0.1.12							*
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

// Grid.cpp : Implementation of CGrid

#include "stdafx.h"
#include "UniverseApp.h"
#include "Hubble.h"
#include "GridHelper.h"
#include "GridWnd.h"
#include "grid.h"
#include "Galaxy.h"
#include "HubbleEvents.h"
#include "GridWnd.h"
#include "Wormhole.h"
#include "universe.c"
#include "chromium/Browser.h"
#include "chromium/WebPage.h"

CGrid::CGrid()
{
	g_pHubble->m_nTangramObj++;
	m_nID = 0;
	m_nRow = 0;
	m_nCol = 0;
	m_nRows = 1;
	m_nCols = 1;
	m_nViewType = BlankView;
	m_bTopObj = false;
	m_bWebInit = false;
	m_bCreated = false;
	m_varTag.vt = VT_EMPTY;
	m_strKey = _T("");
	m_strURL = _T("");
	m_strNodeName = _T("");
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
	m_pHostParse = nullptr;
	m_pWebBrowser = nullptr;
	m_pGridShareData = nullptr;
	g_pHubble->m_pActiveGrid = this;
}


void CGrid::InitWndGrid()
{
	m_pParentWinFormWnd = nullptr;
	m_pHubbleCloudSession = nullptr;
	m_pGridShareData = m_pRootObj->m_pGridShareData;
	ASSERT(m_pGridShareData != nullptr);
	m_nHeigh = m_pHostParse->attrInt(TGM_HEIGHT, 0);
	m_nWidth = m_pHostParse->attrInt(TGM_WIDTH, 0);
	m_strName = m_pHostParse->attr(_T("id"), _T(""));
	if (m_strName == _T(""))
	{
		m_strName.Format(_T("Grid_%p"), (LONGLONG)this);
	}
	if (m_pGridShareData->m_pGalaxy->m_pWebPageWnd)
	{
		auto it = m_pGridShareData->m_pGalaxy->m_pWebPageWnd->m_mapBindWebObj.find(m_strName);
		if (it == m_pGridShareData->m_pGalaxy->m_pWebPageWnd->m_mapBindWebObj.end())
		{
			BindWebObj* pObj = new BindWebObj;
			pObj->nType = 1;
			pObj->m_pGrid = this;
			pObj->m_strBindObjName = m_strName;
			//m_pGridShareData->m_pGalaxy->m_pWebPageWnd->m_mapBindWebObj.erase(it);
			//delete it->second;
			m_pGridShareData->m_pGalaxy->m_pWebPageWnd->m_mapBindWebObj[m_strName] = pObj;
		}
	}
	m_nActivePage = m_pHostParse->attrInt(TGM_ACTIVE_PAGE, 0);
	m_strCaption = m_pHostParse->attr(TGM_CAPTION, _T(""));
	if (m_pGridShareData->m_pGalaxy && m_pGridShareData->m_pGalaxy->m_pGalaxyCluster)
	{
		m_strNodeName = m_strName + _T("@") + g_pHubble->m_strCurrentKey + _T("@") + m_pGridShareData->m_pGalaxy->m_strGalaxyName;
		auto it2 = m_pGridShareData->m_pGalaxyCluster->m_mapGrid.find(m_strNodeName);
		if (it2 == m_pGridShareData->m_pGalaxyCluster->m_mapGrid.end())
		{
			m_pGridShareData->m_pGalaxyCluster->m_mapGrid[m_strNodeName] = this;
		}
	}

	m_strObjTypeID = m_pHostParse->attr(TGM_OBJ_ID, _T(""));
	m_strObjTypeID.MakeLower();
	m_strObjTypeID.Trim();
	if (m_strObjTypeID == TGM_NUCLEUS)
		m_strID = TGM_NUCLEUS;
	m_nRows = m_pHostParse->attrInt(TGM_ROWS, 0);
	m_nCols = m_pHostParse->attrInt(TGM_COLS, 0);
	if (m_nRows * m_nCols>1)
	{
		m_strID = _T("grid");
		m_nViewType = Grid;
	}
	else
	{
		if (m_strID != TGM_NUCLEUS)
		{
			m_strID = m_pHostParse->attr(TGM_GRID_TYPE, _T(""));
			m_strID.MakeLower();
			m_strID.Trim();
		}
		if(m_strID==_T(""))
		{ 
			if(m_strObjTypeID.Find(_T(",")) != -1)
				m_strID = _T("clrctrl");
			else if (m_pGridShareData->m_pGalaxy->m_pWebPageWnd)
			{
				auto it = m_pGridShareData->m_pGalaxy->m_pWebPageWnd->m_mapUserControlsInfo.find(m_strObjTypeID);
				if(it!= m_pGridShareData->m_pGalaxy->m_pWebPageWnd->m_mapUserControlsInfo.end())
					m_strID = _T("clrctrl");
				else
				{
					it = m_pGridShareData->m_pGalaxy->m_pWebPageWnd->m_mapFormsInfo.find(m_strObjTypeID);
					if (it != m_pGridShareData->m_pGalaxy->m_pWebPageWnd->m_mapFormsInfo.end())
						m_strID = _T("clrctrl");
				}
			}
		}
	}

	auto it = g_pHubble->m_TabWndClassInfoDictionary.find(m_strID);
	if (it != g_pHubble->m_TabWndClassInfoDictionary.end())
		m_pObjClsInfo = it->second;
	else
		m_pObjClsInfo = RUNTIME_CLASS(CGridHelper);

	for (auto it : g_pHubble->m_mapHubbleAppProxy)
	{
		CGridProxy* pHubbleWndGridProxy = it.second->OnGridInit(this);
		if (pHubbleWndGridProxy)
			m_mapWndGridProxy[it.second] = pHubbleWndGridProxy;
	}
}

CGrid::~CGrid()
{
	if (g_pHubble->m_pActiveGrid == this)
		g_pHubble->m_pActiveGrid = nullptr;
	if (m_pGridShareData->m_pOldGalaxy)
		m_pGridShareData->m_pGalaxy = m_pGridShareData->m_pOldGalaxy;
	CGalaxy * pGalaxy = m_pGridShareData->m_pGalaxy;
	if (pGalaxy->m_pGalaxyCluster)
	{
		auto it = pGalaxy->m_pGalaxyCluster->m_mapGrid.find(m_strNodeName);
		if (it != pGalaxy->m_pGalaxyCluster->m_mapGrid.end())
			pGalaxy->m_pGalaxyCluster->m_mapGrid.erase(it);
	}
	if (pGalaxy->m_pWorkGrid == this)
		pGalaxy->m_pWorkGrid = nullptr;
	if (m_strKey != _T(""))
	{
		auto it = pGalaxy->m_mapGrid.find(m_strKey);
		if (it != pGalaxy->m_mapGrid.end())
		{
			//BOOL bDeleteFrame = FALSE;
			pGalaxy->m_mapGrid.erase(it);
			if (pGalaxy->m_mapGrid.size() == 0)
			{
				if (::IsWindow(pGalaxy->m_hWnd))
				{
					pGalaxy->UnsubclassWindow(true);
					pGalaxy->m_hWnd = NULL;
				}
				delete pGalaxy;
			}
			delete m_pGridShareData;
			m_pGridShareData = nullptr;
		}
	}

	if (m_pCLREventConnector)
	{
		delete m_pCLREventConnector;
		m_pCLREventConnector = nullptr;
	}

	g_pHubble->m_nTangramObj--;
#ifdef _DEBUG
#endif
	HRESULT hr = S_OK;
	DWORD dw = 0;

	if (m_pExtender)
	{
		dw = m_pExtender->Release();
		m_pExtender = nullptr;
	}

	if (m_nViewType != TangramTreeView && m_nViewType != Grid && m_pDisp)
		CCommonFunction::ClearObject<IUnknown>(m_pDisp);

	m_vChildNodes.clear();

	if (m_pChildNodeCollection != nullptr)
	{
		dw = m_pChildNodeCollection->Release();
		while (dw)
			dw = m_pChildNodeCollection->Release();
		m_pChildNodeCollection = nullptr;
	}
	for (auto it : m_mapWndGridProxy)
	{
		if (it.second->m_bAutoDelete)
			delete it.second;
	}
	m_mapWndGridProxy.clear();
	ATLTRACE(_T("delete CGrid:%x\n"), this);
}

CString CGrid::GetNames()
{
	return _T("");
}

CWebPage* CGrid::GetHtmlWnd()
{
	if (m_pRootObj)
	{
		HWND hPWnd = m_pGridShareData->m_pGalaxy->m_pGalaxyCluster->m_hWnd;
		HWND hWnd = (HWND)::GetWindowLongPtr(hPWnd, GWLP_USERDATA);
		if (::IsWindow(hWnd))
		{
			::GetClassName(hWnd, g_pHubble->m_szBuffer, 256);
			CString strName = CString(g_pHubble->m_szBuffer);
			if (strName == _T("Chrome Extended Window Class")) {
				return (CWebPage*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			}
		}
		else
		{
			CWinForm* pForm = (CWinForm*)::SendMessage(hPWnd, WM_HUBBLE_DATA, 0, 20190214);
			if (pForm)
			{
				m_pParentWinFormWnd = pForm;
				return pForm->m_pOwnerHtmlWnd;
			}
		}
	}
	return nullptr;
}

BOOL CGrid::PreTranslateMessage(MSG * pMsg)
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

STDMETHODIMP CGrid::LoadXML(int nType, BSTR bstrXML)
{
	return S_OK;
}

STDMETHODIMP CGrid::ActiveTabPage(IGrid * _pGrid)
{
	g_pHubble->m_pActiveGrid = this;
	HWND hWnd = m_pHostWnd->m_hWnd;
	if (::IsWindow(hWnd))
	{
		::SetFocus(hWnd);
		m_pGridShareData->m_pGalaxy->HostPosChanged();
	}
	return S_OK;
}

STDMETHODIMP CGrid::Observe(BSTR bstrKey, BSTR bstrXml, IGrid * *ppRetGrid)
{
	switch (m_nViewType)
	{
	case CLRCtrl:
	case BlankView:
	{
		if (m_nViewType == BlankView)
		{
			if (m_pParentObj && m_pParentObj->m_nViewType == Grid)
			{
				HRESULT hr =  m_pParentObj->ObserveEx(m_nRow, m_nCol, bstrKey, bstrXml, ppRetGrid);
				return hr;
			}
		}
		if (m_pGridShareData->m_pGalaxyCluster)
		{
			if (m_nViewType == BlankView && m_pParentObj && m_pParentObj->m_nViewType == Grid)
			{
				return m_pParentObj->ObserveEx(m_nRow, m_nCol, bstrKey, bstrXml, ppRetGrid);
			}
			if (m_pHostGalaxy == nullptr)
			{
				CString strName = m_strNodeName;
				strName += _T("_Frame");

				if (m_nViewType == BlankView)
				{
					RECT rc;
					::GetClientRect(m_pHostWnd->m_hWnd, &rc);
					m_hHostWnd = ::CreateWindowEx(NULL, L"Hubble Grid Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_pHostWnd->m_hWnd, NULL, AfxGetInstanceHandle(), NULL);
				}
				else
				{
					m_hHostWnd = ::GetWindow(m_pHostWnd->m_hWnd, GW_CHILD);
				}
				IGalaxy* pGalaxy = nullptr;
				m_pGridShareData->m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)m_hHostWnd), strName.AllocSysString(), &pGalaxy);
				if (pGalaxy)
					m_pHostGalaxy = (CGalaxy*)pGalaxy;
			}
			if (m_pHostGalaxy && ::IsWindow(m_hHostWnd))
			{
				m_pHostGalaxy->m_pContainerNode = this;
				if (m_pGridShareData->m_pGalaxy->m_pParentGrid)
				{
					m_pHostGalaxy->m_pParentGrid = m_pGridShareData->m_pGalaxy->m_pParentGrid;
				}
				HRESULT hr = m_pHostGalaxy->Observe(bstrKey, bstrXml, ppRetGrid);
				if (m_pWebBrowser)
				{
					CGrid* pRetNode = (CGrid*)*ppRetGrid;
					CComPtr<IGridCollection> pHubbleNodeCollection;
					IGrid* _pGrid = nullptr;
					long nCount = 0;
					pRetNode->m_pRootObj->GetGrids(CComBSTR(m_strName), &_pGrid, &pHubbleNodeCollection, &nCount);
					if (_pGrid)
					{
						CGrid* pGrid2 = (CGrid*)_pGrid;
						pGrid2->m_pWebBrowser = m_pWebBrowser;
						CGrid* pOldParent = m_pWebBrowser->m_pParentGrid;
						m_pWebBrowser->m_pParentGrid = pGrid2;
						m_pHostGalaxy->m_pHostWebBrowserNode = pGrid2;
						m_pHostGalaxy->m_strHostWebBrowserNodeName = m_strName;
						m_pHostGalaxy->m_pHostWebBrowserWnd = m_pWebBrowser;
						HWND hWnd = m_pWebBrowser->m_hWnd;
						HWND h = ::GetParent(hWnd);
						CGridHelper* pGridWnd = (CGridHelper*)CWnd::FromHandlePermanent(h);
						pGridWnd->m_hFormWnd = nullptr;
						::SetParent(hWnd, ((CGridHelper*)pGrid2->m_pHostWnd)->m_hWnd);
						((CGridHelper*)pGrid2->m_pHostWnd)->m_hFormWnd = hWnd;
						if (pOldParent && pOldParent != pGrid2)
							pOldParent->m_pWebBrowser = nullptr;
						::PostMessage(pGrid2->m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20200128);
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
	return S_OK;
}

STDMETHODIMP CGrid::ObserveEx(int nRow, int nCol, BSTR bstrKey, BSTR bstrXml, IGrid * *ppRetGrid)
{
	if (m_pGridShareData->m_pGalaxyCluster && m_nViewType == Grid)
	{
		IGrid* pGrid = nullptr;
		GetGrid(nRow, nCol, &pGrid);
		if (pGrid == nullptr)
			return S_OK;
		CGrid * pWndGrid = (CGrid*)pGrid;
		if (pWndGrid->m_pHostGalaxy == nullptr)
		{
			CString strName = pWndGrid->m_strNodeName;
			strName += _T("_Frame");

			::SetWindowLong(pWndGrid->m_pHostWnd->m_hWnd, GWL_ID, 1);
			IGalaxy* pGalaxy = nullptr;
			m_pGridShareData->m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)pWndGrid->m_pHostWnd->m_hWnd), strName.AllocSysString(), &pGalaxy);
			pWndGrid->m_pHostGalaxy = (CGalaxy*)pGalaxy;
			CGalaxy* _pGalaxy = pWndGrid->m_pHostGalaxy;
			_pGalaxy->m_pWebPageWnd = m_pGridShareData->m_pGalaxy->m_pWebPageWnd;
			if (m_pGridShareData->m_pGalaxy->m_pParentGrid)
			{
				_pGalaxy->m_pParentGrid = m_pGridShareData->m_pGalaxy->m_pParentGrid;
			}
			if (m_pGridShareData->m_pGalaxy->m_pWebPageWnd)
			{
				_pGalaxy->m_pWebPageWnd = m_pGridShareData->m_pGalaxy->m_pWebPageWnd;
			}
		}

		if (pWndGrid->m_pHostGalaxy)
		{
			if (pWndGrid->m_pCurrentExNode)
			{
				::SetWindowLong(pWndGrid->m_pCurrentExNode->m_pHostWnd->m_hWnd, GWL_ID, 1);
			}

			HRESULT hr = pWndGrid->m_pHostGalaxy->Observe(bstrKey, bstrXml, ppRetGrid);

			long dwID = AFX_IDW_PANE_FIRST + nRow * 16 + nCol;
			if (hr != S_OK)
			{
				if (pWndGrid->m_pCurrentExNode)
					::SetWindowLong(pWndGrid->m_pCurrentExNode->m_pHostWnd->m_hWnd, GWL_ID, dwID);
				else
					::SetWindowLong(pWndGrid->m_pHostWnd->m_hWnd, GWL_ID, dwID);
				CWebPage* pWebWnd = pWndGrid->m_pHostGalaxy->m_pWebPageWnd;
				if (pWebWnd)
				{
					::SendMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
				}
				return S_OK;
			}
			CGrid* pRootGrid = (CGrid*)* ppRetGrid;
			CString strKey = OLE2T(bstrKey);
			strKey.MakeLower();
			m_mapExtendNode[pWndGrid] = strKey;
			pWndGrid->m_pCurrentExNode = pRootGrid;
			::SetWindowLongPtr(pRootGrid->m_pHostWnd->m_hWnd, GWLP_ID, dwID);
			CWebPage* pWebWnd = pWndGrid->m_pHostGalaxy->m_pWebPageWnd;
			if (pWebWnd)
			{
				if (pWndGrid->m_pHostGalaxy->m_pBindingGrid)
					pWebWnd->m_hWebHostWnd = pWndGrid->m_pHostGalaxy->m_pBindingGrid->m_pHostWnd->m_hWnd;
				else
					pWebWnd->m_hWebHostWnd = NULL;
				::SendMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
				//::InvalidateRect(::GetParent(pWebWnd->m_hWnd), nullptr, true);
			}
			HWND h = ::GetParent(m_pHostWnd->m_hWnd);
			if (m_nViewType==Grid)
			{
				CGalaxy* pGalaxy = m_pGridShareData->m_pGalaxy;
				pGalaxy->HostPosChanged();
			}
			return hr;
		}
	}
	return S_OK;
}

STDMETHODIMP CGrid::get_Tag(VARIANT * pVar)
{
	*pVar = m_varTag;

	if (m_varTag.vt == VT_DISPATCH && m_varTag.pdispVal)
		m_varTag.pdispVal->AddRef();
	return S_OK;
}

STDMETHODIMP CGrid::put_Tag(VARIANT var)
{
	m_varTag = var;
	return S_OK;
}

STDMETHODIMP CGrid::get_XObject(VARIANT * pVar)
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

STDMETHODIMP CGrid::get_AxPlugIn(BSTR bstrPlugInName, IDispatch * *pVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::get_Name(BSTR * pVal)
{
	*pVal = m_strName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CGrid::put_Name(BSTR bstrNewName)
{
	//CString strName = OLE2T(bstrNewName);
	//strName.Trim();
	//strName.Replace(_T(","), _T(""));
	//if (m_pHostParse != NULL)
	//{
	//	CString _strName = _T(",");
	//	_strName += GetNames();
	//	CString _strName2 = _T(",");
	//	_strName2 += strName;
	//	_strName2 += _T(",");
	//	int nPos = _strName.Find(_strName2);
	//	if (nPos == -1)
	//	{
	//		m_pHostParse->put_attr(L"id", strName);
	//		m_strName = strName;
	//	}
	//	else
	//	{
	//		::MessageBox(NULL, _T("Modify name failed!"), _T("Tangram"), MB_OK);
	//	}
	//}
	return S_OK;
}

STDMETHODIMP CGrid::get_Attribute(BSTR bstrKey, BSTR * pVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strVal = m_pHostParse->attr(OLE2T(bstrKey), _T(""));
		*pVal = strVal.AllocSysString();
	}
	return S_OK;
}

STDMETHODIMP CGrid::put_Attribute(BSTR bstrKey, BSTR bstrVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strID = OLE2T(bstrKey);
		CString strVal = OLE2T(bstrVal);
		if (strID.CompareNoCase(TGM_GRID_TYPE))
			m_strID = strVal;
		ATLTRACE(_T("Modify CGrid Attribute: ID: %s Value: %s\n"), strID, strVal);
		CGalaxy* pGalaxy = nullptr;
		if (strVal.CompareNoCase(TGM_NUCLEUS) == 0 && g_pHubble->m_pDesignGrid)
		{
			pGalaxy = g_pHubble->m_pDesignGrid->m_pRootObj->m_pGridShareData->m_pGalaxy;
			if (g_pHubble->m_pDesignGrid && pGalaxy->m_pBindingGrid)
			{
				CGrid* pOldNode = pGalaxy->m_pBindingGrid;
				if (pOldNode->m_pGridShareData->m_pOldGalaxy)
				{
					pOldNode->m_pGridShareData->m_pGalaxy = pOldNode->m_pGridShareData->m_pOldGalaxy;
					pOldNode->m_pGridShareData->m_pOldGalaxy = nullptr;
				}
				CGrid* pParent = pOldNode->m_pParentObj;
				if (pParent && pParent->m_nViewType == Grid)
				{
					if (pOldNode != this)
					{
						CGridWnd* pWnd = (CGridWnd*)pParent->m_pHostWnd;
						pWnd->m_pHostGrid = nullptr;
						if (m_pParentObj == pParent)
							pWnd->m_pHostGrid = this;
					}
				}
				if (m_pParentObj && m_pParentObj->m_nViewType == Grid)
				{
					CGridWnd* pWnd = (CGridWnd*)m_pParentObj->m_pHostWnd;
					pWnd->m_pHostGrid = this;
				}
				pOldNode->m_strID = _T("");
				if (pOldNode->m_pRootObj == g_pHubble->m_pDesignGrid->m_pRootObj)
					pOldNode->m_pHostParse->put_attr(TGM_GRID_TYPE, _T(""));
				ATLTRACE(_T("Modify CGrid HostView Attribute: ID:%s Value: %s\n"), strID, strVal);
				pOldNode->m_pHostWnd->Invalidate();
			}

			m_strID = TGM_NUCLEUS;
			CGrid* pTopGrid = m_pRootObj;
			pTopGrid->m_pGridShareData->m_pHostClientView = (CGridHelper*)m_pHostWnd;
			while (pTopGrid != pTopGrid->m_pRootObj)
			{
				pTopGrid->m_pGridShareData->m_pGalaxy->m_pBindingGrid = this;
				pTopGrid->m_pGridShareData->m_pHostClientView = pTopGrid->m_pGridShareData->m_pHostClientView;
				pTopGrid = pTopGrid->m_pRootObj;
			}
			m_pHostParse->put_attr(TGM_GRID_TYPE, TGM_NUCLEUS);
			if (g_pHubble->m_pDesignGrid)
			{
				pGalaxy->m_pBindingGrid = this;
				g_pHubble->m_pDesignGrid->m_pGridShareData->m_pOldGalaxy = g_pHubble->m_pDesignGrid->m_pGridShareData->m_pGalaxy;
				g_pHubble->m_pDesignGrid->m_pGridShareData->m_pGalaxy = m_pRootObj->m_pGridShareData->m_pGalaxy;
				g_pHubble->m_pDesignGrid->m_pGridShareData->m_pHostClientView = m_pRootObj->m_pGridShareData->m_pHostClientView;
			}

			if (m_pParentObj && m_pParentObj->m_nViewType == Grid)
				m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			m_pGridShareData->m_pGalaxy->HostPosChanged();
			if (m_pGridShareData->m_pGalaxy->m_pWebPageWnd)
			{
				CWebPage* pWebWnd = m_pGridShareData->m_pGalaxy->m_pWebPageWnd;
				auto it = g_pHubble->m_mapBrowserWnd.find(::GetParent(pWebWnd->m_hWnd));
				if (it != g_pHubble->m_mapBrowserWnd.end()) {
					((CBrowser*)it->second)->m_pBrowser->LayoutBrowser();
					((CBrowser*)it->second)->BrowserLayout();
				}
			}
		}
		m_pHostParse->put_attr(strID, strVal);
	}
	return S_OK;
}

STDMETHODIMP CGrid::get_Caption(BSTR * pVal)
{
	*pVal = m_strCaption.AllocSysString();
	return S_OK;
}

STDMETHODIMP CGrid::put_Caption(BSTR bstrCaption)
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

STDMETHODIMP CGrid::get_Handle(LONGLONG * pVal)
{
	if (m_pHostWnd)
		* pVal = (LONGLONG)m_pHostWnd->m_hWnd;
	return S_OK;
}

STDMETHODIMP CGrid::get_OuterXml(BSTR * pVal)
{
	*pVal = m_pGridShareData->m_pHubbleParse->xml().AllocSysString();
	return S_OK;
}

STDMETHODIMP CGrid::get_Key(BSTR * pVal)
{
	*pVal = m_pRootObj->m_strKey.AllocSysString();
	return S_OK;
}

STDMETHODIMP CGrid::get_XML(BSTR * pVal)
{
	*pVal = m_pHostParse->xml().AllocSysString();
	return S_OK;
}

BOOL CGrid::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext)
{
	BOOL bRet = false;

	CWebPage* pHtmlWnd = m_pGridShareData->m_pGalaxy->m_pWebPageWnd;
	HWND hWnd = 0;
	int nCol = m_pHostParse->GetCount();
	if (nCol && m_strID == _T("") && m_strObjTypeID == _T(""))
		m_strObjTypeID = _T("tabbedwnd");
	CGridHelper* pHubbleDesignView = (CGridHelper*)m_pHostWnd;
	BOOL isAppWnd = false;
	if ( m_strID == _T("clrctrl"))
	{
		g_pHubble->LoadCLR();
		m_nViewType = CLRCtrl;

		hWnd = CreateView(pParentWnd->m_hWnd, m_strObjTypeID);
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
				auto it = g_pHubble->m_mapHubbleAppProxy.find(strKey);
				if (it != g_pHubble->m_mapHubbleAppProxy.end())
				{
					pProxy = it->second;
				}
				else
				{
					CString strPath = g_pHubble->m_strAppPath;
					nPos = strKey.Find(_T("."));
					CString strAppName = strKey.Left(nPos);
					HMODULE hHandle = nullptr;
					CString strdll = strPath + strKey + _T("\\") + strAppName + _T(".dll");
					if (::PathFileExists(strdll))
						hHandle = ::LoadLibrary(strdll);
					if (hHandle == nullptr)
					{
						strdll = g_pHubble->m_strAppCommonDocPath2 + strKey + _T("\\") + strAppName + _T(".dll");
						if (::PathFileExists(strdll))
							hHandle = ::LoadLibrary(strdll);
					}
					if (hHandle)
					{
						it = g_pHubble->m_mapHubbleAppProxy.find(strKey.MakeLower());
						if (it != g_pHubble->m_mapHubbleAppProxy.end())
						{
							pProxy = it->second;
						}
					}
				}
			}

			if (m_nViewType!=CLRCtrl&&::IsWindow(hWnd) == false&&m_strObjTypeID != _T(""))
			{
				IHubbleWindowProvider* pViewFactoryDisp = nullptr;
				auto it = g_pHubble->m_mapWindowProvider.find(m_strObjTypeID);
				if (it != g_pHubble->m_mapWindowProvider.end())
				{
					pViewFactoryDisp = it->second;
				}
				else
				{
					if (it == g_pHubble->m_mapWindowProvider.end())
					{
						if (m_strID.CompareNoCase(_T("TreeView")))
						{
							CString strLib = g_pHubble->m_strAppPath + _T("TabbedWnd.dll");
							if (::PathFileExists(strLib))
							{
								::LoadLibrary(strLib);
								auto it = g_pHubble->m_mapWindowProvider.find(m_strObjTypeID);
								if (it != g_pHubble->m_mapWindowProvider.end())
								{
									pViewFactoryDisp = it->second;
								}
							}
							if (pViewFactoryDisp == nullptr)
							{
								CString strLib = _T("");
								CString strPath = g_pHubble->m_strAppPath + _T("wincomponent\\") + m_strObjTypeID + _T(".component");
								CTangramXmlParse m_Parse;
								if (::PathFileExists(strPath))
								{
									if (m_Parse.LoadFile(strPath))
									{
										strLib = g_pHubble->m_strAppPath + _T("wincomponent\\") + m_Parse.attr(_T("lib"), _T(""));
									}
								}
								if (::PathFileExists(strLib)&&::LoadLibrary(strLib))
								{
									auto it = g_pHubble->m_mapWindowProvider.find(m_strObjTypeID);
									if (it != g_pHubble->m_mapWindowProvider.end())
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
					m_nID = ::GetWindowLong(hWnd, GWL_ID);
				}
			}
		}
	}

	if (!::IsWindow(m_pHostWnd->m_hWnd) && hWnd && pHubbleDesignView->SubclassWindow(hWnd))
	{
		if (isAppWnd == false)
			::SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_CHILD | /*WS_VISIBLE | */WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		else
		{
			if (m_pParentObj && m_pParentObj->m_nViewType == Grid)
				m_pHostWnd->ModifyStyleEx(0, WS_EX_CLIENTEDGE);
		}
		::SetWindowLong(hWnd, GWL_ID, nID);

		pHubbleDesignView->m_bCreateExternal = true;
		if(m_nViewType==BlankView)
			m_nViewType = TabGrid;
		bRet = true;
	}

	if (hWnd == 0)
	{
		hWnd = CreateWindow(L"Hubble Grid Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, pParentWnd->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);
		if (::IsWindow(m_pHostWnd->m_hWnd) == false)
			bRet = m_pHostWnd->SubclassWindow(hWnd);
	}

	if (m_nViewType == BlankView && m_pHostParse != nullptr)
	{
		CString _strURL = m_pHostParse->attr(_T("url"), _T(""));
		HWND hPWnd = g_pHubble->m_hHostBrowserWnd; 

		if(_strURL ==_T("host"))
		{ 
			if (g_pHubble->m_hTempBrowserWnd)
			{
				hPWnd = g_pHubble->m_hTempBrowserWnd;
				g_pHubble->m_hTempBrowserWnd = NULL;
			}
			else if (g_pHubble->m_pHtmlWndCreated == nullptr)
			{
				hPWnd = g_pHubble->m_hHostBrowserWnd;
			}
			else
			{
				hPWnd = ::GetParent(g_pHubble->m_pHtmlWndCreated->m_hWnd);
			}
			
			if (::IsWindow(hPWnd) && (::GetWindowLongPtr(hPWnd, GWL_STYLE) & WS_CHILD)) {
				auto it = g_pHubble->m_mapBrowserWnd.find(hPWnd);
				if (it != g_pHubble->m_mapBrowserWnd.end())// && hPWnd == g_pHubble->m_hHostBrowserWnd)
				{
					m_pWebBrowser = (CBrowser*)it->second;
					::SetWindowPos(hPWnd, HWND_BOTTOM, 0,0, rect.right-rect.left,rect.bottom-rect.top, /*|SWP_SHOWWINDOW|SWP_NOSENDCHANGING*/SWP_NOREDRAW|SWP_NOACTIVATE);
					g_pHubble->m_hParent = NULL;
					m_pRootObj->m_pGridShareData->m_pGalaxy->m_strHostWebBrowserNodeName = m_strName;
					m_pRootObj->m_pGridShareData->m_pGalaxy->m_pHostWebBrowserNode = this;
					m_pRootObj->m_pGridShareData->m_pGalaxy->m_pHostWebBrowserWnd = m_pWebBrowser;
					m_pWebBrowser->m_heightfix = (hPWnd == g_pHubble->m_hHostBrowserWnd) ? 12 : 0;
					::PostMessage(hWnd, WM_COSMOSMSG, (WPARAM)m_pWebBrowser, 20201028);
				}
			}
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
							strURL = g_pHubble->m_strAppPath + strURL.Mid(nPos2 + 1);
						}
					}
					s += strURL;
					s += _T("|");
					_strURL = _strURL.Mid(nPos + 1);
					nPos = _strURL.Find(_T("|"));
				}

				if (g_pHubble->m_pBrowserFactory)
				{
					HWND hBrowser = g_pHubble->m_pBrowserFactory->CreateBrowser(hWnd, s);
					((CGridHelper*)m_pHostWnd)->m_hFormWnd = hBrowser;
					g_pHubble->m_hParent = NULL;
					auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
					if (it != g_pHubble->m_mapBrowserWnd.end())
					{
						m_pWebBrowser = (CBrowser*)it->second;
					}
				}
				else
				{
					g_pHubble->m_mapGridForHtml[this] = s;
				}
			}
		}
	}

	bRet = true;

	//Very important:
	if (m_pHostWnd && ::IsWindow(m_pHostWnd->m_hWnd))
		m_pHostWnd->SendMessage(WM_INITIALUPDATE);

	////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_strID.CompareNoCase(_T("treeview")))
	{
		m_nRows = 1;
		m_nCols = nCol;

		if (nCol)
		{
			m_nViewType = TabGrid;
			if (m_nActivePage<0 || m_nActivePage>nCol - 1)
				m_nActivePage = 0;
			CWnd * pView = nullptr;
			CGrid * pObj = nullptr;
			int j = 0;
			for (int i = 0; i < nCol; i++)
			{
				CTangramXmlParse* pChild = m_pHostParse->GetChild(i);
				CString _strName = pChild->name();
				CString strName = pChild->attr(_T("id"), _T(""));
				if (_strName.CompareNoCase(TGM_GRID) == 0)
				{
					strName.Trim();
					strName.MakeLower();

					pObj = new CComObject<CGrid>;
					pObj->m_pRootObj = m_pRootObj;
					pObj->m_pHostParse = pChild;
					AddChildNode(pObj);
					pObj->InitWndGrid();
					pObj->m_nCol = j;

					if (pObj->m_pObjClsInfo)
					{
						pContext->m_pNewViewClass = pObj->m_pObjClsInfo;
						pView = (CWnd*)pContext->m_pNewViewClass->CreateObject();
						pView->Create(NULL, _T(""), WS_CHILD, rect, m_pHostWnd, 0, pContext);
						HWND m_hChild = (HWND)::SendMessage(m_pHostWnd->m_hWnd, WM_CREATETABPAGE, (WPARAM)pView->m_hWnd, (LPARAM)LPCTSTR(pObj->m_strCaption));
					}
					j++;
				}
			}
			::SendMessage(m_pHostWnd->m_hWnd, WM_ACTIVETABPAGE, (WPARAM)m_nActivePage, (LPARAM)1);
			Fire_TabChange(m_nActivePage, -1);
		}
	}

	m_pHostWnd->SetWindowText(m_strNodeName);
	if (m_nViewType == TabGrid && m_pParentObj && m_pParentObj->m_nViewType == Grid)
	{
		if (m_pHostWnd)
			m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
	}

	NodeCreated();

	return bRet;
}

void CGrid::NodeCreated()
{
	CWebPage* pHtmlWnd = m_pGridShareData->m_pGalaxy->m_pWebPageWnd;
	if (pHtmlWnd == nullptr)
		pHtmlWnd = GetHtmlWnd();
	if (pHtmlWnd == nullptr)
		pHtmlWnd = g_pHubble->m_pHostHtmlWnd;
	if (pHtmlWnd&&m_pHubbleCloudSession == nullptr)
	{
		::PostMessage(pHtmlWnd->m_hWnd, WM_COSMOSMSG, 20200310, (LPARAM)this);
	}
}

HWND CGrid::CreateView(HWND hParentWnd, CString strTag)
{
	BOOL bWebCtrl = false;
	CString strURL = _T("");
	CString strID = strTag;
	CComBSTR bstr2;
	get_Attribute(CComBSTR("id"), &bstr2);
	CString strName = OLE2T(bstr2);

	CWebPage* pHtmlWnd = nullptr;
	HWND _hWnd = m_pGridShareData->m_pGalaxy->m_hWnd;
	{
		::GetClassName(_hWnd, g_pHubble->m_szBuffer, 256);
		CString strName = CString(g_pHubble->m_szBuffer);
		if (strName == _T("Chrome Extended Window Class")) {
			pHtmlWnd = (CWebPage*)::GetWindowLongPtr(_hWnd, GWLP_USERDATA);
		}
	}

	switch (m_nViewType)
	{
	case CLRCtrl:
	{
		g_pHubble->m_pActiveGrid = this;
		
		CString strUIKey = strTag;
		if (g_pHubble->m_pCLRProxy)
		{
			if (pHtmlWnd)
			{
				g_pHubble->m_pCLRProxy->m_strCurrentWinFormTemplate = _T("");
				strUIKey.MakeLower();
				auto it = pHtmlWnd->m_mapUserControlsInfo.find(strUIKey);
				if(it != pHtmlWnd->m_mapUserControlsInfo.end())
				{
					g_pHubble->m_mapControlScript[this] = it->second;
					if (it->second != _T(""))
					{
						CTangramXmlParse parse;
						if (parse.LoadXml(it->second))
						{
							CString _strTag = parse.attr(_T("objid"), _T(""));
							if (_strTag != _T(""))
								strTag = _strTag;
						}
					}
				}
				else
				{
					it = pHtmlWnd->m_mapFormsInfo.find(strUIKey);
					if (it != pHtmlWnd->m_mapFormsInfo.end())
						g_pHubble->m_pCLRProxy->m_strCurrentWinFormTemplate = it->second;
					if (g_pHubble->m_pCLRProxy->m_strCurrentWinFormTemplate != _T(""))
					{
						CTangramXmlParse parse;
						if (parse.LoadXml(g_pHubble->m_pCLRProxy->m_strCurrentWinFormTemplate))
						{
							CString _strTag = parse.attr(_T("objid"), _T(""));
							if (_strTag != _T(""))
								strTag = _strTag;
						}
					}
				}
			}
			m_pDisp = g_pHubble->m_pCLRProxy->CreateObject(strTag.AllocSysString(), hParentWnd, this);
			if (g_pHubble->m_hFormNodeWnd)
			{
				LRESULT l = ::SendMessage((HWND)g_pHubble->m_hFormNodeWnd, WM_HUBBLE_DATA, 0, 20190214);
				if (l&& pHtmlWnd)
				{
					auto it = pHtmlWnd->m_mapWinForm.find(g_pHubble->m_hFormNodeWnd);
					if (it == pHtmlWnd->m_mapWinForm.end())
					{
						pHtmlWnd->m_mapWinForm[g_pHubble->m_hFormNodeWnd] = (CWinForm*)l;
					}
				}
			}

			if (m_pDisp == nullptr)
			{
				((CGridHelper*)m_pHostWnd)->m_bCreateExternal = false;
				m_nViewType = BlankView;
			}
		}
	}
	break;
	}
	if (m_pDisp)
	{
		if (m_nViewType == CLRCtrl)
		{
			HWND hCtrl = NULL;
			if (g_pHubble->m_pCLRProxy)
				hCtrl = g_pHubble->m_pCLRProxy->GetCtrlHandle(m_pDisp);
			if (g_pHubble->m_hFormNodeWnd&& hCtrl == g_pHubble->m_hFormNodeWnd &&(::GetWindowLongPtr(g_pHubble->m_hFormNodeWnd, GWL_STYLE) & WS_CHILD))
			{
				HWND hWnd = g_pHubble->m_hFormNodeWnd;
				g_pHubble->m_hFormNodeWnd = nullptr;
				return hWnd;
			}
		}
		auto hWnd = ::CreateWindowEx(NULL, L"Hubble Grid Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hParentWnd, NULL, AfxGetInstanceHandle(), NULL);
		CAxWindow m_Wnd;
		m_Wnd.Attach(hWnd);
		CComPtr<IUnknown> pUnk;
		m_Wnd.AttachControl(m_pDisp, &pUnk);

		CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pDisp);
		if (pIOleInPlaceActiveObject)
			((CGridHelper*)m_pHostWnd)->m_pOleInPlaceActiveObject = pIOleInPlaceActiveObject.Detach();
		m_Wnd.Detach();
		return hWnd;
	}

	return 0;
}

STDMETHODIMP CGrid::get_ChildNodes(IGridCollection * *pGridColletion)
{
	if (m_pChildNodeCollection == nullptr)
	{
		CComObject<CGridCollection>::CreateInstance(&m_pChildNodeCollection);
		m_pChildNodeCollection->AddRef();
		m_pChildNodeCollection->m_pGrids = &m_vChildNodes;
	}
	return m_pChildNodeCollection->QueryInterface(IID_IGridCollection, (void**)pGridColletion);
}

int CGrid::_getNodes(CGrid * pGrid, CString & strName, CGrid * *ppRetGrid, CGridCollection * pGrids)
{
	int iCount = 0;
	if (pGrid->m_strName.CompareNoCase(strName) == 0)
	{
		if (pGrids != nullptr)
			pGrids->m_pGrids->push_back(pGrid);

		if (ppRetGrid != nullptr && (*ppRetGrid) == nullptr)
			* ppRetGrid = pGrid;
		return 1;
	}

	for (auto it : pGrid->m_vChildNodes)
	{
		iCount += _getNodes(it, strName, ppRetGrid, pGrids);
	}
	return iCount;
}

STDMETHODIMP CGrid::Show()
{
	CGrid* pChild = this;
	CGrid* pParent = pChild->m_pParentObj;

	while (pParent != nullptr)
	{
		pParent->m_pHostWnd->SendMessage(WM_ACTIVETABPAGE, (WPARAM)pChild->m_nCol, (LPARAM)1);

		pChild = pParent;
		pParent = pChild->m_pParentObj;
	}
	return S_OK;
}

STDMETHODIMP CGrid::get_RootGrid(IGrid * *ppGrid)
{
	if (m_pRootObj != nullptr)
		* ppGrid = m_pRootObj;
	return S_OK;
}

STDMETHODIMP CGrid::get_ParentGrid(IGrid * *ppGrid)
{
	*ppGrid = nullptr;
	if (m_pParentObj != nullptr)
		* ppGrid = m_pParentObj;

	return S_OK;
}

STDMETHODIMP CGrid::get_GridType(GridType* nType)
{
	*nType = m_nViewType;
	return S_OK;
}

//void CGrid::_get_Objects(CGrid * pGrid, UINT32 & nType, CGridCollection * pGridColletion)
//{
//	if (pGrid->m_nViewType & nType)
//	{
//		pGridColletion->m_pGrids->push_back(pGrid);
//	}
//
//	CGrid* pChildNode = nullptr;
//	for (auto it : pGrid->m_vChildNodes)
//	{
//		pChildNode = it;
//		_get_Objects(pChildNode, nType, pGridColletion);
//	}
//}

STDMETHODIMP CGrid::get_Objects(long nType, IGridCollection * *ppGridColletion)
{
	//CComObject<CGridCollection>* pGrids = nullptr;
	//CComObject<CGridCollection>::CreateInstance(&pGrids);

	//pGrids->AddRef();

	//UINT32 u = nType;
	//_get_Objects(this, u, pGrids);
	//HRESULT hr = pGrids->QueryInterface(IID_IGridCollection, (void**)ppGridColletion);

	//pGrids->Release();

	return S_OK;
}

STDMETHODIMP CGrid::get_Rows(long* nRows)
{
	*nRows = m_nRows;
	return S_OK;
}

STDMETHODIMP CGrid::get_Cols(long* nCols)
{
	*nCols = m_nCols;
	return S_OK;
}

STDMETHODIMP CGrid::get_Row(long* nRow)
{
	*nRow = m_nRow;
	return S_OK;
}

STDMETHODIMP CGrid::get_Col(long* nCol)
{
	*nCol = m_nCol;
	return S_OK;
}

STDMETHODIMP CGrid::GetGrid(long nRow, long nCol, IGrid * *ppGrid)
{
	CGrid* pRet = nullptr;

	*ppGrid = nullptr;
	if (nRow < 0 || nCol < 0 || nRow >= m_nRows || nCol >= m_nCols) return E_INVALIDARG;
	//if (m_nViewType == Grid)
	//{
	//	CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
	//	HWND hWnd = ::GetDlgItem(pSplitter->m_hWnd, pSplitter->IdFromRowCol(nRow, nCol));
	//	LRESULT lRes = ::SendMessage(hWnd, WM_HUBBLE_GETNODE, 0, 0);
	//	if (lRes)
	//	{
	//		pRet = (CGrid*)lRes;
	//		pRet->QueryInterface(IID_IGrid, (void**)ppGrid);
	//		return S_OK;
	//	}
	//	return S_FALSE;
	//}

	for (auto it : m_vChildNodes)
	{
		pRet = it;
		if (pRet->m_nCol == nCol && pRet->m_nRow == nRow)
		{
			break;
		}
	}

	HRESULT hr = S_OK;
	if (pRet)
	{
		hr = pRet->QueryInterface(IID_IGrid, (void**)ppGrid);
	}
	return hr;
}

STDMETHODIMP CGrid::GetGridByName(BSTR bstrName, IGridCollection * *ppGrids)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strName(bstrName);

	CGrid* pRetNode = nullptr;

	CComObject<CGridCollection>* pGrids = nullptr;
	if (ppGrids != nullptr)
	{
		*ppGrids = nullptr;
		CComObject<CGridCollection>::CreateInstance(&pGrids);
		pGrids->AddRef();
	}

	int iCount = _getNodes(this, strName, &pRetNode, pGrids);

	if (ppGrids != nullptr)
		pGrids->QueryInterface(IID_IGridCollection, (void**)ppGrids);

	if (pGrids != nullptr)
		pGrids->Release();

	return S_OK;
}

STDMETHODIMP CGrid::GetGrids(BSTR bstrName, IGrid * *ppGrid, IGridCollection * *ppGrids, long* pCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strName(bstrName);

	CGrid* pRetNode = nullptr;

	if (ppGrid != nullptr)
		* ppGrid = nullptr;

	CComObject<CGridCollection> * pGrids = nullptr;
	if (ppGrids != nullptr)
	{
		*ppGrids = nullptr;
		CComObject<CGridCollection>::CreateInstance(&pGrids);
		pGrids->AddRef();
	}

	int iCount = _getNodes(this, strName, &pRetNode, pGrids);

	*pCount = iCount;

	if ((iCount > 0) && (ppGrid != nullptr))
		pRetNode->QueryInterface(IID_IGrid, (void**)ppGrid);

	if (ppGrids != nullptr)
		pGrids->QueryInterface(IID_IGridCollection, (void**)ppGrids);

	if (pGrids != nullptr)
		pGrids->Release();

	return S_OK;
}

BOOL CGrid::AddChildNode(CGrid * pGrid)
{
	m_vChildNodes.push_back(pGrid);
	pGrid->m_pParentObj = this;
	pGrid->m_pRootObj = m_pRootObj;
	return true;
}

BOOL CGrid::RemoveChildNode(CGrid * pGrid)
{
	auto it = find(m_vChildNodes.begin(), m_vChildNodes.end(), pGrid);
	if (it != m_vChildNodes.end())
	{
		m_vChildNodes.erase(it);
		return true;
	}
	return false;
}

STDMETHODIMP CGrid::get_Galaxy(IGalaxy * *pVal)
{
	if (m_pGridShareData->m_pGalaxy)
		* pVal = m_pGridShareData->m_pGalaxy;

	return S_OK;
}

STDMETHODIMP CGrid::get_HostGalaxy(IGalaxy * *pVal)
{
	if (m_pHostGalaxy)
		* pVal = m_pHostGalaxy;

	return S_OK;
}

STDMETHODIMP CGrid::get_Height(LONG * pVal)
{
	RECT rc;
	::GetClientRect(m_pHostWnd->m_hWnd, &rc);
	*pVal = rc.bottom;
	return S_OK;
}

STDMETHODIMP CGrid::get_Width(LONG * pVal)
{
	RECT rc;
	::GetClientRect(m_pHostWnd->m_hWnd, &rc);
	*pVal = rc.right;

	return S_OK;
}

STDMETHODIMP CGrid::get_OfficeObj(IDispatch * *pVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::get_Extender(IDispatch * *pVal)
{
	if (m_pExtender)
	{
		*pVal = m_pExtender;
		(*pVal)->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CGrid::put_Extender(IDispatch * newVal)
{
	if (m_pExtender)
		m_pExtender->Release();
	m_pExtender = newVal;
	m_pExtender->AddRef();

	return S_OK;
}

STDMETHODIMP CGrid::get_GalaxyCluster(IGalaxyCluster * *pVal)
{
	*pVal = (IGalaxyCluster*)m_pGridShareData->m_pGalaxy->m_pGalaxyCluster;
	return S_OK;
}

STDMETHODIMP CGrid::get_NameAtWindowPage(BSTR * pVal)
{
	*pVal = m_strNodeName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CGrid::GetCtrlByName(BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch * *ppRetDisp)
{
	if (g_pHubble->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
		* ppRetDisp = g_pHubble->m_pCLRProxy->GetCtrlByName(m_pDisp, bstrName, bFindInChild ? true : false);

	return S_OK;
}

STDMETHODIMP CGrid::GetCtrlValueByName(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR * bstrVal)
{
	//if (g_pHubble->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
	//{
	//	*bstrVal = g_pHubble->m_pCLRProxy->GetCtrlValueByName(m_pDisp, bstrName, bFindInChild ? true : false);
	//}
	return S_OK;
}

STDMETHODIMP CGrid::SetCtrlValueByName(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal)
{
	//if (g_pHubble->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
	//{
	//	g_pHubble->m_pCLRProxy->SetCtrlValueByName(m_pDisp, bstrName, bFindInChild ? true : false, bstrVal);
	//}
	return S_OK;
}

CGridCollection::CGridCollection()
{
	m_pGrids = &m_vGrids;
	g_pHubble->m_mapWndGridCollection[(__int64)this] = this;
}

CGridCollection::~CGridCollection()
{
	auto it = g_pHubble->m_mapWndGridCollection.find((__int64)this);
	if (it != g_pHubble->m_mapWndGridCollection.end())
	{
		g_pHubble->m_mapWndGridCollection.erase(it);
	}
	m_vGrids.clear();
}

STDMETHODIMP CGridCollection::get_GridCount(long* pCount)
{
	*pCount = (int)m_pGrids->size();
	return S_OK;
}

STDMETHODIMP CGridCollection::get_Item(long iIndex, IGrid * *ppGrid)
{
	if (iIndex < 0 || iIndex >= (int)m_pGrids->size()) return E_INVALIDARG;

	CGrid * pGrid = m_pGrids->operator [](iIndex);

	return pGrid->QueryInterface(IID_IGrid, (void**)ppGrid);
}

STDMETHODIMP CGridCollection::get__NewEnum(IUnknown * *ppVal)
{
	*ppVal = nullptr;

	struct _CopyVariantFromIUnkown
	{
		static HRESULT copy(VARIANT* p1, CGrid* const* p2)
		{
			CGrid* pGrid = *p2;
			p1->vt = VT_UNKNOWN;
			return pGrid->QueryInterface(IID_IUnknown, (void**) & (p1->punkVal));
		}

		static void init(VARIANT* p) { VariantInit(p); }
		static void destroy(VARIANT* p) { VariantClear(p); }
	};

	typedef CComEnumOnSTL<IEnumVARIANT, & IID_IEnumVARIANT, VARIANT, _CopyVariantFromIUnkown, CGridVector>
		CComEnumVariantOnVector;

	CComObject<CComEnumVariantOnVector>* pe = 0;
	HRESULT hr = CComObject<CComEnumVariantOnVector>::CreateInstance(&pe);

	if (SUCCEEDED(hr))
	{
		hr = pe->AddRef();
		hr = pe->Init(GetUnknown(), *m_pGrids);

		if (SUCCEEDED(hr))
			hr = pe->QueryInterface(ppVal);

		hr = pe->Release();
	}

	return hr;
}

STDMETHODIMP CGrid::get_DocXml(BSTR * pVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::get_rgbMiddle(OLE_COLOR * pVal)
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

STDMETHODIMP CGrid::put_rgbMiddle(OLE_COLOR newVal)
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

STDMETHODIMP CGrid::get_rgbLeftTop(OLE_COLOR * pVal)
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

STDMETHODIMP CGrid::put_rgbLeftTop(OLE_COLOR newVal)
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

STDMETHODIMP CGrid::get_rgbRightBottom(OLE_COLOR * pVal)
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

STDMETHODIMP CGrid::put_rgbRightBottom(OLE_COLOR newVal)
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

STDMETHODIMP CGrid::get_Hmin(int* pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = pSplitter->m_Hmin;
	}
	return S_OK;
}

STDMETHODIMP CGrid::put_Hmin(int newVal)
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

STDMETHODIMP CGrid::get_Hmax(int* pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = pSplitter->m_Hmax;
	}
	return S_OK;
}

STDMETHODIMP CGrid::put_Hmax(int newVal)
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

STDMETHODIMP CGrid::get_Vmin(int* pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = pSplitter->m_Vmin;
	}

	return S_OK;
}

STDMETHODIMP CGrid::put_Vmin(int newVal)
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

STDMETHODIMP CGrid::get_Vmax(int* pVal)
{
	if (m_nViewType == Grid)
	{
		CGridWnd* pSplitter = (CGridWnd*)m_pHostWnd;
		*pVal = pSplitter->m_Vmax;
	}

	return S_OK;
}

STDMETHODIMP CGrid::put_Vmax(int newVal)
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


STDMETHODIMP CGrid::get_HostGrid(IGrid * *pVal)
{
	if (m_pGridShareData->m_pHostClientView)
		* pVal = m_pGridShareData->m_pHostClientView->m_pGrid;

	return S_OK;
}


STDMETHODIMP CGrid::put_HostGrid(IGrid * newVal)
{
	return S_OK;
}


STDMETHODIMP CGrid::get_ActivePage(int* pVal)
{
	if (this->m_nViewType == GridType::TabGrid)
	{
		CComBSTR bstr(L"");
		get_Attribute(CComBSTR(L"activepage"), &bstr);
		*pVal = _wtoi(OLE2T(bstr));
	}
	return S_OK;
}


STDMETHODIMP CGrid::put_ActivePage(int newVal)
{
	if (this->m_nViewType == GridType::TabGrid && newVal < m_nCols)
	{
		HWND hwnd = nullptr;
		int nOldPage = 0;
		get_ActivePage(&nOldPage);
		if (nOldPage == newVal)
			return S_OK;
		IGrid * pOldNode = nullptr;
		GetGrid(0, newVal, &pOldNode);
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
		IGrid* pGrid = nullptr;
		this->GetGrid(0, newVal, &pGrid);
		if (pGrid)
		{
			::ShowWindow(hwnd, SW_HIDE);
			ActiveTabPage(pGrid);
		}
	}

	return S_OK;
}

STDMETHODIMP CGrid::get_MasterRow(int* pVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::put_MasterRow(int newVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::get_MasterCol(int* pVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::put_MasterCol(int newVal)
{
	return S_OK;
}

HRESULT CGrid::Fire_ObserveComplete()
{
	HRESULT hr = S_OK;
	int cConnections = m_vec.GetSize();
	if (cConnections)
	{
		DISPPARAMS params = { NULL, NULL, 0, 0 };
		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			g_pHubble->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapWndGridProxy)
	{
		it.second->OnObserverComplete();
	}

	return hr;
}

HRESULT CGrid::Fire_Destroy()
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
			g_pHubble->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	for (auto it : m_mapWndGridProxy)
	{
		it.second->OnDestroy();
	}

	if (g_pHubble->m_pCLRProxy)
	{
		g_pHubble->m_pCLRProxy->ReleaseHubbleObj((IGrid*)this);
	}
	return hr;
}

HRESULT CGrid::Fire_TabChange(LONG ActivePage, LONG OldPage)
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	for (auto it : m_mapWndGridProxy)
	{
		it.second->OnTabChange(ActivePage, OldPage);
	}
	return hr;
}

HRESULT CGrid::Fire_IPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
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
			g_pHubble->Lock();
			IUnknown* punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();
			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(8, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	return hr;
}

STDMETHODIMP CGrid::put_SaveToConfigFile(VARIANT_BOOL newVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::get_DockObj(BSTR bstrName, LONGLONG * pVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::put_DockObj(BSTR bstrName, LONGLONG newVal)
{
	return S_OK;
}

STDMETHODIMP CGrid::NavigateURL(BSTR bstrURL, IDispatch * dispObjforScript)
{
	if (g_pHubble->m_pBrowserFactory == nullptr)
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
					strURL = g_pHubble->m_strAppPath + strURL.Mid(nPos2 + 1);
				}
			}
			s += strURL;
			s += _T("|");
			_strXml = _strXml.Mid(nPos + 1);
			nPos = _strXml.Find(_T("|"));
		}

		HWND hBrowser = g_pHubble->m_pBrowserFactory->CreateBrowser(((CGridHelper*)m_pHostWnd)->m_hWnd, s);
		((CGridHelper*)m_pHostWnd)->m_hFormWnd = hBrowser;
		g_pHubble->m_hParent = NULL;
		auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
		if (it != g_pHubble->m_mapBrowserWnd.end())
		{
			m_pWebBrowser = (CBrowser*)it->second;
			//m_pWebBrowser->m_pGrid = this;
		}
		//g_pHubble->m_pCurWebNode = nullptr;
		return S_OK;
	}
	else if (m_pWebBrowser)
	{
		m_pWebBrowser->OpenURL(bstrURL, BrowserWndOpenDisposition::SWITCH_TO_TAB, CComBSTR(""), CComBSTR(""));
		return S_OK;
	}
	return S_OK;
}

STDMETHODIMP CGrid::get_URL(BSTR * pVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strVal = m_pHostParse->attr(_T("url"), _T(""));
		*pVal = strVal.AllocSysString();
		strVal.ReleaseBuffer();
	}
	return S_OK;
}

STDMETHODIMP CGrid::put_URL(BSTR newVal)
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
					strURL = g_pHubble->m_strAppPath + strURL.Mid(nPos2 + 1);
				}
			}
			s += strURL;
			s += _T("|");
			_strXml = _strXml.Mid(nPos + 1);
			nPos = _strXml.Find(_T("|"));
		}

		HWND hBrowser = g_pHubble->m_pBrowserFactory->CreateBrowser(m_pHostWnd->m_hWnd, s);
		((CGridHelper*)m_pHostWnd)->m_hFormWnd = hBrowser;
		g_pHubble->m_hParent = NULL;
		auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
		if (it != g_pHubble->m_mapBrowserWnd.end())
		{
			m_pWebBrowser = (CBrowser*)it->second;
			m_pWebBrowser->m_pParentGrid = this;
		}

		return S_OK;
	}
	//if (m_pWebBrowser)
	//{
	//	m_pWebBrowser->DestroyWindow();
	//	m_pWebBrowser = nullptr;
	//	CString strURL = OLE2T(newVal);
	//	strURL += _T("|");

	//	HWND hBrowser = g_pHubble->m_pBrowserFactory->CreateBrowser(((CGridHelper*)m_pHostWnd)->m_hWnd, strURL);
	//	((CGridHelper*)m_pHostWnd)->m_hFormWnd = hBrowser;
	//	g_pHubble->m_hParent = NULL;
	//	auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
	//	if (it != g_pHubble->m_mapBrowserWnd.end())
	//	{
	//		m_pWebBrowser = (CBrowser*)it->second;
	//	}
	//	return S_OK;
	//}
	return S_OK;
}

STDMETHODIMP CGrid::SendIPCMessage(BSTR bstrTo, BSTR bstrPayload, BSTR bstrExtra, BSTR bstrMsgId, BSTR* bstrRet)
{
	if (m_pGridShareData->m_pGalaxy->m_pWebPageWnd)
	{
		m_pGridShareData->m_pGalaxy->m_pWebPageWnd->SendChromeIPCMessage(_T("I_DONT_KNOW"), OLE2T(bstrTo), OLE2T(bstrMsgId), OLE2T(bstrExtra), L"", L"");
	}
	else
	{
		if (m_pGridShareData->m_pGalaxy->m_pHostWebBrowserNode)
		{
			if (m_pGridShareData->m_pGalaxy->m_pHostWebBrowserWnd)
			{
				HWND hPWnd = m_pGridShareData->m_pGalaxy->m_pHostWebBrowserWnd->m_hWnd;
				for (auto it : g_pHubble->m_mapHtmlWnd)
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
