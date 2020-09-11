/********************************************************************************
*					Open Universe - version 0.1.0								*
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

// Star.cpp : Implementation of CStar

#include "stdafx.h"
#include "UniverseApp.h"
#include "Hubble.h"
#include "StarWnd.h"
#include "Grid.h"
#include "Star.h"
#include "Quasar.h"
#include "TangramCoreEvents.h"
#include "Grid.h"
#include "Wormhole.h"
#include "universe.c"
#include "chromium/BrowserWnd.h"
#include "chromium/HtmlWnd.h"

CStar::CStar()
{
	g_pHubble->m_nTangramObj++;
	m_nID = 0;
	m_nRow = 0;
	m_nCol = 0;
	m_nRows = 1;
	m_nCols = 1;
	m_nViewType = BlankView;
	m_pChildFormsInfo = nullptr;
	m_bTopObj = false;
	m_bWebInit = false;
	m_bCreated = false;
	m_bNodeDocComplete = false;
	m_varTag.vt = VT_EMPTY;
	m_strKey = _T("");
	m_strURL = _T("");
	m_strNodeName = _T("");
	m_strExtenderID = _T("");
	m_hHostWnd = NULL;
	m_hChildHostWnd = NULL;
	m_pDisp = nullptr;
	m_pHostQuasar = nullptr;
	m_pRootObj = nullptr;
	m_pHostWnd = nullptr;
	m_pExtender = nullptr;
	m_pParentObj = nullptr;
	m_pObjClsInfo = nullptr;
	m_pVisibleXMLObj = nullptr;
	m_pCLREventConnector = nullptr;
	m_pChildNodeCollection = nullptr;
	m_pCurrentExNode = nullptr;
	m_pWindow = nullptr;
	m_pHostParse = nullptr;
	m_pDocXmlParseNode = nullptr;
	m_pWebBrowser = nullptr;
	m_pStarCommonData = nullptr;
	g_pHubble->m_pActiveStar = this;
}


void CStar::InitWndNode()
{
	m_pParentWinFormWnd = nullptr;
	m_pHubbleCloudSession = nullptr;
	m_pStarCommonData = m_pRootObj->m_pStarCommonData;
	ASSERT(m_pStarCommonData != nullptr);
	m_nHeigh = m_pHostParse->attrInt(TGM_HEIGHT, 0);
	m_nWidth = m_pHostParse->attrInt(TGM_WIDTH, 0);
	m_strName = m_pHostParse->attr(_T("id"), _T(""));
	if (m_strName == _T(""))
	{
		m_strName.Format(_T("Node_%p"), (LONGLONG)this);
	}
	if (m_pStarCommonData->m_pQuasar->m_pWebPageWnd)
	{
		auto it = m_pStarCommonData->m_pQuasar->m_pWebPageWnd->m_mapBindWebObj.find(m_strName);
		if (it == m_pStarCommonData->m_pQuasar->m_pWebPageWnd->m_mapBindWebObj.end())
		{
			BindWebObj* pObj = new BindWebObj;
			pObj->nType = 1;
			pObj->m_pNode = this;
			pObj->m_strBindObjName = m_strName;
			//m_pStarCommonData->m_pQuasar->m_pWebPageWnd->m_mapBindWebObj.erase(it);
			//delete it->second;
			m_pStarCommonData->m_pQuasar->m_pWebPageWnd->m_mapBindWebObj[m_strName] = pObj;
		}
	}
	m_nActivePage = m_pHostParse->attrInt(TGM_ACTIVE_PAGE, 0);
	m_strCaption = m_pHostParse->attr(TGM_CAPTION, _T(""));
	if (m_pStarCommonData->m_pQuasar && m_pStarCommonData->m_pQuasar->m_pGalaxyCluster)
	{
		m_strNodeName = m_strName + _T("@") + g_pHubble->m_strCurrentKey + _T("@") + m_pStarCommonData->m_pQuasar->m_strQuasarName;
		auto it2 = m_pStarCommonData->m_pGalaxyCluster->m_mapNode.find(m_strNodeName);
		if (it2 == m_pStarCommonData->m_pGalaxyCluster->m_mapNode.end())
		{
			m_pStarCommonData->m_pGalaxyCluster->m_mapNode[m_strNodeName] = this;
		}
	}
	m_strID = m_pHostParse->attr(TGM_NODE_TYPE, _T(""));
	m_strID.MakeLower();
	m_strID.Trim();
	m_strCnnID = m_pHostParse->attr(TGM_CNN_ID, _T(""));
	if(m_strCnnID==_T(""))
		m_strCnnID = m_pHostParse->attr(_T("cnnid"), _T(""));
	m_strCnnID.MakeLower();
	m_strCnnID.Trim();

	auto it = g_pHubble->m_TabWndClassInfoDictionary.find(m_strID);
	if (it != g_pHubble->m_TabWndClassInfoDictionary.end())
		m_pObjClsInfo = it->second;
	else
		m_pObjClsInfo = RUNTIME_CLASS(CStarWnd);

	for (auto it : g_pHubble->m_mapHubbleAppProxy)
	{
		CStarProxy* pTangramWndNodeProxy = it.second->OnHubbleNodeInit(this);
		if (pTangramWndNodeProxy)
			m_mapWndNodeProxy[it.second] = pTangramWndNodeProxy;
	}
}

CStar::~CStar()
{
	if (g_pHubble->m_pActiveStar == this)
		g_pHubble->m_pActiveStar = nullptr;
	if (m_pStarCommonData->m_pOldQuasar)
		m_pStarCommonData->m_pQuasar = m_pStarCommonData->m_pOldQuasar;
	CQuasar * pQuasar = m_pStarCommonData->m_pQuasar;
	if (pQuasar->m_pGalaxyCluster)
	{
		auto it = pQuasar->m_pGalaxyCluster->m_mapNode.find(m_strNodeName);
		if (it != pQuasar->m_pGalaxyCluster->m_mapNode.end())
			pQuasar->m_pGalaxyCluster->m_mapNode.erase(it);
	}
	if (pQuasar->m_pWorkNode == this)
		pQuasar->m_pWorkNode = nullptr;
	if (m_strKey != _T(""))
	{
		auto it = pQuasar->m_mapNode.find(m_strKey);
		if (it != pQuasar->m_mapNode.end())
		{
			//BOOL bDeleteFrame = FALSE;
			pQuasar->m_mapNode.erase(it);
			if (pQuasar->m_mapNode.size() == 0)
			{
				if (::IsWindow(pQuasar->m_hWnd))
				{
					pQuasar->UnsubclassWindow(true);
					pQuasar->m_hWnd = NULL;
				}
				delete pQuasar;
			}
			delete m_pStarCommonData;
			m_pStarCommonData = nullptr;
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

	if (m_nViewType != TangramTreeView && m_nViewType != Splitter && m_pDisp)
		CCommonFunction::ClearObject<IUnknown>(m_pDisp);

	m_vChildNodes.clear();

	if (m_pChildNodeCollection != nullptr)
	{
		dw = m_pChildNodeCollection->Release();
		while (dw)
			dw = m_pChildNodeCollection->Release();
		m_pChildNodeCollection = nullptr;
	}
	for (auto it : m_mapWndNodeProxy)
	{
		if (it.second->m_bAutoDelete)
			delete it.second;
	}
	m_mapWndNodeProxy.clear();
	ATLTRACE(_T("delete CStar:%x\n"), this);
}

CString CStar::GetNames()
{
	CString strRet = _T("");
	CStar* pStar = this;
	pStar = pStar->m_pRootObj;
	strRet += pStar->m_strName;
	strRet += _T(",");
	strRet += _GetNames(pStar);
	return strRet;
}

CString CStar::_GetNames(CStar * pStar)
{
	CString strRet = _T("");
	if (pStar)
	{
		for (auto it : pStar->m_vChildNodes)
		{
			CStar* pChildNode = it;
			strRet += pChildNode->m_strName;
			strRet += _T(",");
			strRet += _GetNames(pChildNode);
		}
	}
	return strRet;
}

CWebPage* CStar::GetHtmlWnd()
{
	if (m_pRootObj)
	{
		HWND hPWnd = m_pStarCommonData->m_pQuasar->m_pGalaxyCluster->m_hWnd;
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
			CWinForm* pForm = (CWinForm*)::SendMessage(hPWnd, WM_TANGRAMDATA, 0, 20190214);
			if (pForm)
			{
				m_pParentWinFormWnd = pForm;
				return pForm->m_pOwnerHtmlWnd;
			}
		}
	}
	return nullptr;
}

BOOL CStar::PreTranslateMessage(MSG * pMsg)
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

STDMETHODIMP CStar::LoadXML(int nType, BSTR bstrXML)
{
	return S_OK;
}

STDMETHODIMP CStar::ActiveTabPage(IStar * _pNode)
{
	g_pHubble->m_pActiveStar = this;
	HWND hWnd = m_pHostWnd->m_hWnd;
	if (::IsWindow(hWnd))
	{
		::SetFocus(hWnd);
		m_pStarCommonData->m_pQuasar->HostPosChanged();
		if (m_pStarCommonData->m_pQuasar->m_bDesignerState && g_pHubble->m_pDesignWindowNode)
		{
			g_pHubble->UpdareStar(g_pHubble->m_pDesignWindowNode->m_pRootObj);
			CComBSTR bstrXml(L"");
			g_pHubble->m_pDesignWindowNode->m_pRootObj->get_DocXml(&bstrXml);
			g_pHubble->m_mapValInfo[_T("tangramdesignerxml")] = CComVariant(bstrXml);
		}
	}
	return S_OK;
}

STDMETHODIMP CStar::Observe(BSTR bstrKey, BSTR bstrXml, IStar * *ppRetNode)
{
	switch (m_nViewType)
	{
	case CLRCtrl:
	case BlankView:
	{
		if (m_nViewType == BlankView)
		{
			if (m_pParentObj && m_pParentObj->m_nViewType == Splitter)
			{
				HRESULT hr =  m_pParentObj->ObserveEx(m_nRow, m_nCol, bstrKey, bstrXml, ppRetNode);
				return hr;
			}
		}
		if (m_pStarCommonData->m_pGalaxyCluster)
		{
			if (m_nViewType == BlankView && m_pParentObj && m_pParentObj->m_nViewType == Splitter)
			{
				return m_pParentObj->ObserveEx(m_nRow, m_nCol, bstrKey, bstrXml, ppRetNode);
			}
			if (m_pHostQuasar == nullptr)
			{
				CString strName = m_strNodeName;
				strName += _T("_Frame");

				if (m_nViewType == BlankView)
				{
					RECT rc;
					::GetClientRect(m_pHostWnd->m_hWnd, &rc);
					m_hHostWnd = ::CreateWindowEx(NULL, L"Tangram Node Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_pHostWnd->m_hWnd, NULL, AfxGetInstanceHandle(), NULL);
				}
				else
				{
					m_hHostWnd = ::GetWindow(m_pHostWnd->m_hWnd, GW_CHILD);
				}
				IQuasar* pQuasar = nullptr;
				m_pStarCommonData->m_pGalaxyCluster->CreateQuasar(CComVariant(0), CComVariant((long)m_hHostWnd), strName.AllocSysString(), &pQuasar);
				if (pQuasar)
					m_pHostQuasar = (CQuasar*)pQuasar;
			}
			if (m_pHostQuasar && ::IsWindow(m_hHostWnd))
			{
				m_pHostQuasar->m_pContainerNode = this;
				if (m_pStarCommonData->m_pQuasar->m_pParentStar)
				{
					m_pHostQuasar->m_pParentStar = m_pStarCommonData->m_pQuasar->m_pParentStar;
				}
				HRESULT hr = m_pHostQuasar->Observe(bstrKey, bstrXml, ppRetNode);
				if (m_pWebBrowser)
				{
					CStar* pRetNode = (CStar*)*ppRetNode;
					CComPtr<IStarCollection> pTangramNodeCollection;
					IStar* _pNode = nullptr;
					long nCount = 0;
					pRetNode->m_pRootObj->GetStars(CComBSTR(m_strName), &_pNode, &pTangramNodeCollection, &nCount);
					if (_pNode)
					{
						CStar* pNode2 = (CStar*)_pNode;
						pNode2->m_pWebBrowser = m_pWebBrowser;
						CStar* pOldParent = m_pWebBrowser->m_pParentStar;
						m_pWebBrowser->m_pParentStar = pNode2;
						m_pHostQuasar->m_pHostWebBrowserNode = pNode2;
						m_pHostQuasar->m_strHostWebBrowserNodeName = m_strName;
						m_pHostQuasar->m_pHostWebBrowserWnd = m_pWebBrowser;
						HWND hWnd = m_pWebBrowser->m_hWnd;
						HWND h = ::GetParent(hWnd);
						CStarWnd* pNodeWnd = (CStarWnd*)CWnd::FromHandlePermanent(h);
						pNodeWnd->m_hFormWnd = nullptr;
						::SetParent(hWnd, ((CStarWnd*)pNode2->m_pHostWnd)->m_hWnd);
						((CStarWnd*)pNode2->m_pHostWnd)->m_hFormWnd = hWnd;
						if (pOldParent && pOldParent != pNode2)
							pOldParent->m_pWebBrowser = nullptr;
						::PostMessage(pNode2->m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20200128);
					}
				}
				return hr;
			}
		}
	}
	break;
	case Splitter:
		break;
	}
	return S_OK;
}

STDMETHODIMP CStar::ObserveEx(int nRow, int nCol, BSTR bstrKey, BSTR bstrXml, IStar * *ppRetNode)
{
	if (m_pStarCommonData->m_pGalaxyCluster && m_nViewType == Splitter)
	{
		IStar* pNode = nullptr;
		GetStar(nRow, nCol, &pNode);
		if (pNode == nullptr)
			return S_OK;
		CStar * pWindowNode = (CStar*)pNode;
		if (pWindowNode->m_pHostQuasar == nullptr)
		{
			CString strName = pWindowNode->m_strNodeName;
			strName += _T("_Frame");

			::SetWindowLong(pWindowNode->m_pHostWnd->m_hWnd, GWL_ID, 1);
			IQuasar* pQuasar = nullptr;
			m_pStarCommonData->m_pGalaxyCluster->CreateQuasar(CComVariant(0), CComVariant((long)pWindowNode->m_pHostWnd->m_hWnd), strName.AllocSysString(), &pQuasar);
			pWindowNode->m_pHostQuasar = (CQuasar*)pQuasar;
			CQuasar* _pQuasar = pWindowNode->m_pHostQuasar;
			_pQuasar->m_pWebPageWnd = m_pStarCommonData->m_pQuasar->m_pWebPageWnd;
			if (m_pStarCommonData->m_pQuasar->m_pParentStar)
			{
				_pQuasar->m_pParentStar = m_pStarCommonData->m_pQuasar->m_pParentStar;
			}
			if (m_pStarCommonData->m_pQuasar->m_pWebPageWnd)
			{
				_pQuasar->m_pWebPageWnd = m_pStarCommonData->m_pQuasar->m_pWebPageWnd;
				if (pWindowNode->m_strID == _T("nucleus"))
					m_pStarCommonData->m_pQuasar->m_pSubQuasar = _pQuasar;
			}
		}

		if (pWindowNode->m_pHostQuasar)
		{
			if (pWindowNode->m_pCurrentExNode)
			{
				::SetWindowLong(pWindowNode->m_pCurrentExNode->m_pHostWnd->m_hWnd, GWL_ID, 1);
			}

			HRESULT hr = pWindowNode->m_pHostQuasar->Observe(bstrKey, bstrXml, ppRetNode);

			long dwID = AFX_IDW_PANE_FIRST + nRow * 16 + nCol;
			if (hr != S_OK)
			{
				if (pWindowNode->m_pCurrentExNode)
					::SetWindowLong(pWindowNode->m_pCurrentExNode->m_pHostWnd->m_hWnd, GWL_ID, dwID);
				else
					::SetWindowLong(pWindowNode->m_pHostWnd->m_hWnd, GWL_ID, dwID);
				CWebPage* pWebWnd = pWindowNode->m_pHostQuasar->m_pWebPageWnd;
				if (pWebWnd)
				{
					::SendMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 2);
					::InvalidateRect(::GetParent(pWebWnd->m_hWnd), nullptr, true);
				}
				return S_OK;
			}
			CStar* pRootNode = (CStar*)* ppRetNode;
			CString strKey = OLE2T(bstrKey);
			strKey.MakeLower();
			m_mapExtendNode[pWindowNode] = strKey;
			pWindowNode->m_pCurrentExNode = pRootNode;
			::SetWindowLongPtr(pRootNode->m_pHostWnd->m_hWnd, GWLP_ID, dwID);
			CWebPage* pWebWnd = pWindowNode->m_pHostQuasar->m_pWebPageWnd;
			if (pWebWnd)
			{
				if (pWindowNode->m_pHostQuasar->m_pBindingStar)
					pWebWnd->m_hWebHostWnd = pWindowNode->m_pHostQuasar->m_pBindingStar->m_pHostWnd->m_hWnd;
				else
					pWebWnd->m_hWebHostWnd = NULL;
				::SendMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 2);
				::InvalidateRect(::GetParent(pWebWnd->m_hWnd), nullptr, true);
			}
			HWND h = ::GetParent(m_pHostWnd->m_hWnd);
			if (m_nViewType==Splitter)
			{
				CQuasar* pQuasar = m_pStarCommonData->m_pQuasar;
				pQuasar->HostPosChanged();
			}
			return hr;
		}
	}
	return S_OK;
}

STDMETHODIMP CStar::get_Tag(VARIANT * pVar)
{
	*pVar = m_varTag;

	if (m_varTag.vt == VT_DISPATCH && m_varTag.pdispVal)
		m_varTag.pdispVal->AddRef();
	return S_OK;
}

STDMETHODIMP CStar::put_Tag(VARIANT var)
{
	m_varTag = var;
	return S_OK;
}

STDMETHODIMP CStar::get_XObject(VARIANT * pVar)
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

STDMETHODIMP CStar::get_AxPlugIn(BSTR bstrPlugInName, IDispatch * *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strObjName = OLE2T(bstrPlugInName);
	strObjName.Trim();
	strObjName.MakeLower();
	IDispatch* pDisp = nullptr;
	if (m_pStarCommonData->m_PlugInDispDictionary.Lookup(LPCTSTR(strObjName), (void*&)pDisp))
	{
		*pVal = pDisp;
		(*pVal)->AddRef();
	}
	else
		*pVal = nullptr;
	return S_OK;
}

STDMETHODIMP CStar::get_Name(BSTR * pVal)
{
	*pVal = m_strName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CStar::put_Name(BSTR bstrNewName)
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

STDMETHODIMP CStar::get_Attribute(BSTR bstrKey, BSTR * pVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strVal = m_pHostParse->attr(OLE2T(bstrKey), _T(""));
		*pVal = strVal.AllocSysString();
	}
	return S_OK;
}

STDMETHODIMP CStar::put_Attribute(BSTR bstrKey, BSTR bstrVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strID = OLE2T(bstrKey);
		CString strVal = OLE2T(bstrVal);
		if (strID.CompareNoCase(TGM_NODE_TYPE))
			m_strID = strVal;
		ATLTRACE(_T("Modify CStar Attribute: ID: %s Value: %s\n"), strID, strVal);
		CQuasar* pQuasar = nullptr;
		if (strVal.CompareNoCase(_T("nucleus")) == 0 && g_pHubble->m_pDesignWindowNode)
		{
			pQuasar = g_pHubble->m_pDesignWindowNode->m_pRootObj->m_pStarCommonData->m_pQuasar;
			if (g_pHubble->m_pDesignWindowNode && pQuasar->m_pBindingStar)
			{
				CStar* pOldNode = pQuasar->m_pBindingStar;
				if (pOldNode->m_pStarCommonData->m_pOldQuasar)
				{
					pOldNode->m_pStarCommonData->m_pQuasar = pOldNode->m_pStarCommonData->m_pOldQuasar;
					pOldNode->m_pStarCommonData->m_pOldQuasar = nullptr;
				}
				CStar* pParent = pOldNode->m_pParentObj;
				if (pParent && pParent->m_nViewType == Splitter)
				{
					if (pOldNode != this)
					{
						CGrid* pWnd = (CGrid*)pParent->m_pHostWnd;
						pWnd->m_pHostNode = nullptr;
						if (m_pParentObj == pParent)
							pWnd->m_pHostNode = this;
					}
				}
				if (m_pParentObj && m_pParentObj->m_nViewType == Splitter)
				{
					CGrid* pWnd = (CGrid*)m_pParentObj->m_pHostWnd;
					pWnd->m_pHostNode = this;
				}
				pOldNode->m_strID = _T("");
				if (pOldNode->m_pRootObj == g_pHubble->m_pDesignWindowNode->m_pRootObj)
					pOldNode->m_pHostParse->put_attr(TGM_NODE_TYPE, _T(""));
				ATLTRACE(_T("Modify CStar HostView Attribute: ID:%s Value: %s\n"), strID, strVal);
				pOldNode->m_pHostWnd->Invalidate();
				g_pHubble->UpdareStar(g_pHubble->m_pDesignWindowNode->m_pRootObj);
				g_pHubble->put_AppKeyValue(CComBSTR(L"TangramDesignerXml"), CComVariant(g_pHubble->m_pDesignWindowNode->m_pRootObj->m_pStarCommonData->m_pHubbleParse->xml()));
			}

			m_strID = _T("nucleus");
			CStar* pTopNode = m_pRootObj;
			pTopNode->m_pStarCommonData->m_pHostClientView = (CStarWnd*)m_pHostWnd;
			while (pTopNode != pTopNode->m_pRootObj)
			{
				pTopNode->m_pStarCommonData->m_pQuasar->m_pBindingStar = this;
				pTopNode->m_pStarCommonData->m_pHostClientView = pTopNode->m_pStarCommonData->m_pHostClientView;
				pTopNode = pTopNode->m_pRootObj;
			}
			m_pHostParse->put_attr(TGM_NODE_TYPE, _T("nucleus"));
			if (g_pHubble->m_pDesignWindowNode)
			{
				pQuasar->m_pBindingStar = this;
				g_pHubble->m_pDesignWindowNode->m_pStarCommonData->m_pOldQuasar = g_pHubble->m_pDesignWindowNode->m_pStarCommonData->m_pQuasar;
				g_pHubble->m_pDesignWindowNode->m_pStarCommonData->m_pQuasar = m_pRootObj->m_pStarCommonData->m_pQuasar;
				g_pHubble->m_pDesignWindowNode->m_pStarCommonData->m_pHostClientView = m_pRootObj->m_pStarCommonData->m_pHostClientView;
			}

			if (m_pParentObj && m_pParentObj->m_nViewType == Splitter)
				m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			m_pStarCommonData->m_pQuasar->HostPosChanged();
			if (m_pStarCommonData->m_pQuasar->m_pWebPageWnd)
			{
				CWebPage* pWebWnd = m_pStarCommonData->m_pQuasar->m_pWebPageWnd;
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

STDMETHODIMP CStar::get_Caption(BSTR * pVal)
{
	*pVal = m_strCaption.AllocSysString();
	return S_OK;
}

STDMETHODIMP CStar::put_Caption(BSTR bstrCaption)
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

STDMETHODIMP CStar::get_Handle(LONGLONG * pVal)
{
	if (m_pHostWnd)
		* pVal = (LONGLONG)m_pHostWnd->m_hWnd;
	return S_OK;
}

STDMETHODIMP CStar::get_OuterXml(BSTR * pVal)
{
	*pVal = m_pStarCommonData->m_pHubbleParse->xml().AllocSysString();
	return S_OK;
}

STDMETHODIMP CStar::get_Key(BSTR * pVal)
{
	*pVal = m_pRootObj->m_strKey.AllocSysString();
	return S_OK;
}

STDMETHODIMP CStar::get_XML(BSTR * pVal)
{
	*pVal = m_pHostParse->xml().AllocSysString();
	return S_OK;
}

BOOL CStar::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext)
{
	BOOL bRet = false;

	CWebPage* pHtmlWnd = m_pStarCommonData->m_pQuasar->m_pWebPageWnd;
	HWND hWnd = 0;
	CStarWnd* pTangramDesignView = (CStarWnd*)m_pHostWnd;
	BOOL isAppWnd = false;
	if (m_strID == _T("activex") || m_strID == _T("clrctrl"))
	{
		if (m_strID == _T("clrctrl") || m_strCnnID.Find(_T(",")) != -1)
		{
			g_pHubble->LoadCLR();
			m_nViewType = CLRCtrl;
		}
		else
			m_nViewType = ActiveX;
		if (m_strCnnID.Find(_T("//")) == -1 && ::PathFileExists(m_strCnnID) == false)
		{
			CString strPath = g_pHubble->m_strAppPath + _T("TangramWebPage\\") + m_strCnnID;
			if (::PathFileExists(strPath))
				m_strCnnID = strPath;
		}

		hWnd = CreateView(pParentWnd->m_hWnd, m_strCnnID);
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

			pTangramDesignView->m_bCreateExternal = true;
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
				IHubbleAppProxy* pProxy = nullptr;
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

			if (m_nViewType!=CLRCtrl&&::IsWindow(hWnd) == false&&m_strCnnID != _T(""))
			{
				IHubbleWindowProvider* pViewFactoryDisp = nullptr;
				auto it = g_pHubble->m_mapWindowProvider.find(m_strCnnID);
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
							if (g_pHubble->m_strExeName.CompareNoCase(_T("devenv")) == 0)
							{
#ifdef _WIN32
								CString strLib = g_pHubble->m_strAppPath + _T("PublicAssemblies\\TangramTabbedWnd.dll");
								if (::PathFileExists(strLib))
								{
									::LoadLibrary(strLib);
									auto it = g_pHubble->m_mapWindowProvider.find(m_strCnnID);
									if (it != g_pHubble->m_mapWindowProvider.end())
									{
										pViewFactoryDisp = it->second;
									}
								}
#endif
							}
							else
							{
								CString strLib = g_pHubble->m_strAppPath + _T("TabbedWnd.dll");
								if (::PathFileExists(strLib))
								{
									::LoadLibrary(strLib);
									auto it = g_pHubble->m_mapWindowProvider.find(m_strCnnID);
									if (it != g_pHubble->m_mapWindowProvider.end())
									{
										pViewFactoryDisp = it->second;
									}
								}
							}
							if (pViewFactoryDisp == nullptr)
							{
								CString strLib = _T("");
								CString strPath = g_pHubble->m_strAppPath + _T("tangramcomponent\\") + m_strCnnID + _T(".component");
								CTangramXmlParse m_Parse;
								if (::PathFileExists(strPath))
								{
									if (m_Parse.LoadFile(strPath))
									{
										strLib = g_pHubble->m_strAppPath + _T("tangramcomponent\\") + m_Parse.attr(_T("lib"), _T(""));
									}
								}
								else
								{
									strPath = g_pHubble->m_strProgramFilePath + _T("\\tangram\\tangramcomponent\\") + m_strCnnID + _T(".component");
									if (m_Parse.LoadFile(strPath))
									{
										strLib = g_pHubble->m_strProgramFilePath + _T("\\tangram\\tangramcomponent\\") + m_Parse.attr(_T("lib"), _T(""));
									}
									else
									{
										strPath = g_pHubble->m_strAppPath + _T("PublicAssemblies\\tangramcomponent\\") + m_strCnnID + _T(".component");
										if (m_Parse.LoadFile(strPath))
										{
											strLib = g_pHubble->m_strAppPath + _T("PublicAssemblies\\tangramcomponent\\") + m_Parse.attr(_T("lib"), _T(""));
										}
									}
								}
								if (::PathFileExists(strLib)&&::LoadLibrary(strLib))
								{
									auto it = g_pHubble->m_mapWindowProvider.find(m_strCnnID);
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
					pViewFactoryDisp->m_pCreatingNode = this;
					hWnd = pViewFactoryDisp->Create(pParentWnd ? pParentWnd->m_hWnd : 0, this);
					pViewFactoryDisp->m_pCreatingNode = nullptr;
					if (g_pHubble->m_pCreatingWindow)
					{
						m_pWindow = g_pHubble->m_pCreatingWindow;
						g_pHubble->m_pCreatingWindow = nullptr;
					}
					m_nID = ::GetWindowLong(hWnd, GWL_ID);
				}
			}
		}
	}

	if (!::IsWindow(m_pHostWnd->m_hWnd) && hWnd && pTangramDesignView->SubclassWindow(hWnd))
	{
		if (isAppWnd == false)
			::SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_CHILD | /*WS_VISIBLE | */WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		else
		{
			if (m_pParentObj && m_pParentObj->m_nViewType == Splitter)
				m_pHostWnd->ModifyStyleEx(0, WS_EX_CLIENTEDGE);
		}
		::SetWindowLong(hWnd, GWL_ID, nID);

		pTangramDesignView->m_bCreateExternal = true;
		if(m_nViewType==BlankView)
			m_nViewType = TabbedWnd;
		bRet = true;
	}

	if (hWnd == 0)
	{
		hWnd = CreateWindow(L"Tangram Node Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);
		if (::IsWindow(m_pHostWnd->m_hWnd) == false)
			bRet = m_pHostWnd->SubclassWindow(hWnd);
	}

	if (m_nViewType == BlankView && m_pHostParse != nullptr)
	{
		CString _strURL = m_pHostParse->attr(_T("url"), _T(""));
		if(_strURL ==_T("host"))
		{ 
			m_pRootObj->m_pStarCommonData->m_pQuasar->m_strHostWebBrowserNodeName = m_strName;
			::SetParent(g_pHubble->m_hHostBrowserWnd, hWnd);
			g_pHubble->m_hParent = NULL;
			auto it = g_pHubble->m_mapBrowserWnd.find(g_pHubble->m_hHostBrowserWnd);
			if (it != g_pHubble->m_mapBrowserWnd.end())
			{
				m_pWebBrowser = (CBrowser*)it->second;
				m_pRootObj->m_pStarCommonData->m_pQuasar->m_pHostWebBrowserNode = this;
				m_pRootObj->m_pStarCommonData->m_pQuasar->m_pHostWebBrowserWnd = m_pWebBrowser;
				m_pWebBrowser->m_heightfix = 12;
			}
		}
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
				((CStarWnd*)m_pHostWnd)->m_hFormWnd = hBrowser;
				g_pHubble->m_hParent = NULL;
				auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
				if (it != g_pHubble->m_mapBrowserWnd.end())
				{
					m_pWebBrowser = (CBrowser*)it->second;
				}
			}
			else
			{
				g_pHubble->m_mapNodeForHtml[this] = s;
			}
		}
	}

	bRet = true;

	//Very important:
	if (m_pHostWnd && ::IsWindow(m_pHostWnd->m_hWnd))
		m_pHostWnd->SendMessage(WM_INITIALUPDATE);

	////////////////////////////////////////////////////////////////////////////////////////////////
	m_pStarCommonData->m_mapLayoutNodes[m_strName] = this;
	if (m_strID.CompareNoCase(_T("treeview")))
	{
		int nCol = m_pHostParse->GetCount();

		m_nRows = 1;
		m_nCols = nCol;

		if (nCol)
		{
			m_nViewType = TabbedWnd;
			if (m_nActivePage<0 || m_nActivePage>nCol - 1)
				m_nActivePage = 0;
			CWnd * pView = nullptr;
			CStar * pObj = nullptr;
			int j = 0;
			for (int i = 0; i < nCol; i++)
			{
				CTangramXmlParse* pChild = m_pHostParse->GetChild(i);
				CString _strName = pChild->name();
				CString strName = pChild->attr(_T("id"), _T(""));
				if (_strName.CompareNoCase(TGM_NODE) == 0)
				{
					strName.Trim();
					strName.MakeLower();

					pObj = new CComObject<CStar>;
					pObj->m_pRootObj = m_pRootObj;
					pObj->m_pHostParse = pChild;
					AddChildNode(pObj);
					pObj->InitWndNode();
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
			m_pStarCommonData->m_pGalaxyCluster->Fire_TabChange(this, m_nActivePage, -1);
		}
	}

	m_pHostWnd->SetWindowText(m_strNodeName);
	if (m_nViewType == TabbedWnd && m_pParentObj && m_pParentObj->m_nViewType == Splitter)
	{
		if (m_pHostWnd)
			m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
	}
	if (g_pHubble->m_pActiveStar && g_pHubble->m_pActiveStar->m_pStarCommonData->m_pGalaxyCluster)
		g_pHubble->m_pActiveStar->m_pStarCommonData->m_pGalaxyCluster->Fire_NodeCreated(this);

	NodeCreated();

	return bRet;
}

void CStar::NodeCreated()
{
	CWebPage* pHtmlWnd = m_pStarCommonData->m_pQuasar->m_pWebPageWnd;
	if (pHtmlWnd == nullptr)
		pHtmlWnd = GetHtmlWnd();
	if (pHtmlWnd == nullptr)
		pHtmlWnd = g_pHubble->m_pHostHtmlWnd;
	if (pHtmlWnd == nullptr)
		pHtmlWnd = g_pHubble->m_pMainHtmlWnd;
	if (pHtmlWnd&&m_pHubbleCloudSession == nullptr)
	{
		::PostMessage(pHtmlWnd->m_hWnd, WM_COSMOSMSG, 20200310, (LPARAM)this);
	}
}

HWND CStar::CreateView(HWND hParentWnd, CString strTag)
{
	BOOL bWebCtrl = false;
	CString strURL = _T("");
	CString strID = strTag;
	CComBSTR bstr2;
	get_Attribute(CComBSTR("id"), &bstr2);
	CString strName = OLE2T(bstr2);

	CWebPage* pHtmlWnd = nullptr;
	HWND _hWnd = m_pStarCommonData->m_pQuasar->m_hWnd;
	{
		::GetClassName(_hWnd, g_pHubble->m_szBuffer, 256);
		CString strName = CString(g_pHubble->m_szBuffer);
		if (strName == _T("Chrome Extended Window Class")) {
			pHtmlWnd = (CWebPage*)::GetWindowLongPtr(_hWnd, GWLP_USERDATA);
		}
	}

	switch (m_nViewType)
	{
	case ActiveX:
	{
	}
	break;
	case CLRCtrl:
	{
		g_pHubble->m_pActiveStar = this;
		auto it = m_pStarCommonData->m_mapCLRNodes.find(strName);
		if (it == m_pStarCommonData->m_mapCLRNodes.end())
		{
			m_pStarCommonData->m_mapCLRNodes[strName] = this;
		}
		else
		{
			int nCount = m_pStarCommonData->m_mapCLRNodes.size();
			CString str = _T("");
			str.Format(_T("%s%d"), strName, nCount);
			it = m_pStarCommonData->m_mapCLRNodes.find(str);
			while (it != m_pStarCommonData->m_mapCLRNodes.end())
			{
				nCount++;
				str.Format(_T("%s%d"), strName, nCount);
				it = m_pStarCommonData->m_mapCLRNodes.find(str);
			}
			m_pStarCommonData->m_mapCLRNodes[str] = this;
			put_Attribute(CComBSTR("id"), str.AllocSysString());
		}
		
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
				LRESULT l = ::SendMessage((HWND)g_pHubble->m_hFormNodeWnd, WM_TANGRAMDATA, 0, 20190214);
				if (l&& pHtmlWnd)
				{
					auto it = pHtmlWnd->m_mapWinForm.find(g_pHubble->m_hFormNodeWnd);
					if (it == pHtmlWnd->m_mapWinForm.end())
					{
						pHtmlWnd->m_mapWinForm[g_pHubble->m_hFormNodeWnd] = (CWinForm*)l;
					}
				}
			}

			CStarWnd* pWnd = (CStarWnd*)m_pHostWnd;
			if (m_pDisp && pWnd->m_mapDockCtrl.size())
			{
				HWND hPage = m_pStarCommonData->m_pGalaxyCluster->m_hWnd;
				::SendMessage(hPage, WM_COSMOSMSG, (WPARAM)m_pHostWnd, 1963);
			}
			if (m_pDisp == nullptr)
			{
				((CStarWnd*)m_pHostWnd)->m_bCreateExternal = false;
				m_nViewType = BlankView;
			}
			if (m_strID.CollateNoCase(_T("wpfctrl")) == 0)
			{
				pWnd->m_hFormWnd = g_pHubble->m_hFormNodeWnd;
				g_pHubble->m_hFormNodeWnd = NULL;
			}
		}
	}
	break;
	}
	if (m_pDisp)
	{
		m_pStarCommonData->m_mapLayoutNodes[m_strName] = this;
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
		auto hWnd = ::CreateWindowEx(NULL, L"Tangram Node Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hParentWnd, NULL, AfxGetInstanceHandle(), NULL);
		CAxWindow m_Wnd;
		m_Wnd.Attach(hWnd);
		CComPtr<IUnknown> pUnk;
		m_Wnd.AttachControl(m_pDisp, &pUnk);

		CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pDisp);
		if (pIOleInPlaceActiveObject)
			((CStarWnd*)m_pHostWnd)->m_pOleInPlaceActiveObject = pIOleInPlaceActiveObject.Detach();
		m_Wnd.Detach();
		return hWnd;
	}

	return 0;
}

STDMETHODIMP CStar::get_ChildNodes(IStarCollection * *pNodeColletion)
{
	if (m_pChildNodeCollection == nullptr)
	{
		CComObject<CStarCollection>::CreateInstance(&m_pChildNodeCollection);
		m_pChildNodeCollection->AddRef();
		m_pChildNodeCollection->m_pNodes = &m_vChildNodes;
	}
	return m_pChildNodeCollection->QueryInterface(IID_IStarCollection, (void**)pNodeColletion);
}

int CStar::_getNodes(CStar * pNode, CString & strName, CStar * *ppRetNode, CStarCollection * pNodes)
{
	int iCount = 0;
	if (pNode->m_strName.CompareNoCase(strName) == 0)
	{
		if (pNodes != nullptr)
			pNodes->m_pNodes->push_back(pNode);

		if (ppRetNode != nullptr && (*ppRetNode) == nullptr)
			* ppRetNode = pNode;
		return 1;
	}

	for (auto it : pNode->m_vChildNodes)
	{
		iCount += _getNodes(it, strName, ppRetNode, pNodes);
	}
	return iCount;
}

STDMETHODIMP CStar::Show()
{
	CStar* pChild = this;
	CStar* pParent = pChild->m_pParentObj;

	while (pParent != nullptr)
	{
		pParent->m_pHostWnd->SendMessage(WM_ACTIVETABPAGE, (WPARAM)pChild->m_nCol, (LPARAM)1);

		pChild = pParent;
		pParent = pChild->m_pParentObj;
	}
	return S_OK;
}

STDMETHODIMP CStar::get_RootStar(IStar * *ppNode)
{
	if (m_pRootObj != nullptr)
		* ppNode = m_pRootObj;
	return S_OK;
}

STDMETHODIMP CStar::get_ParentStar(IStar * *ppStar)
{
	*ppStar = nullptr;
	if (m_pParentObj != nullptr)
		* ppStar = m_pParentObj;

	return S_OK;
}

STDMETHODIMP CStar::get_StarType(StarType* nType)
{
	*nType = m_nViewType;
	return S_OK;
}

void CStar::_get_Objects(CStar * pNode, UINT32 & nType, CStarCollection * pNodeColletion)
{
	if (pNode->m_nViewType & nType)
	{
		pNodeColletion->m_pNodes->push_back(pNode);
	}

	CStar* pChildNode = nullptr;
	for (auto it : pNode->m_vChildNodes)
	{
		pChildNode = it;
		_get_Objects(pChildNode, nType, pNodeColletion);
	}
}

STDMETHODIMP CStar::get_Objects(long nType, IStarCollection * *ppNodeColletion)
{
	CComObject<CStarCollection>* pNodes = nullptr;
	CComObject<CStarCollection>::CreateInstance(&pNodes);

	pNodes->AddRef();

	UINT32 u = nType;
	_get_Objects(this, u, pNodes);
	HRESULT hr = pNodes->QueryInterface(IID_IStarCollection, (void**)ppNodeColletion);

	pNodes->Release();

	return hr;
}

STDMETHODIMP CStar::get_Rows(long* nRows)
{
	*nRows = m_nRows;
	return S_OK;
}

STDMETHODIMP CStar::get_Cols(long* nCols)
{
	*nCols = m_nCols;
	return S_OK;
}

STDMETHODIMP CStar::get_Row(long* nRow)
{
	*nRow = m_nRow;
	return S_OK;
}

STDMETHODIMP CStar::get_Col(long* nCol)
{
	*nCol = m_nCol;
	return S_OK;
}

STDMETHODIMP CStar::GetStar(long nRow, long nCol, IStar * *ppTangramNode)
{
	CStar* pRet = nullptr;
	auto bFound = false;

	*ppTangramNode = nullptr;
	if (nRow < 0 || nCol < 0 || nRow >= m_nRows || nCol >= m_nCols) return E_INVALIDARG;

	for (auto it : m_vChildNodes)
	{
		pRet = it;
		if (pRet->m_nCol == nCol && pRet->m_nRow == nRow)
		{
			bFound = true;
			break;
		}
	}

	HRESULT hr = S_OK;
	if (bFound)
	{
		hr = pRet->QueryInterface(IID_IStar, (void**)ppTangramNode);
	}
	return hr;
}

STDMETHODIMP CStar::GetStarByName(BSTR bstrName, IStarCollection * *ppNodes)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strName(bstrName);

	CStar* pRetNode = nullptr;

	CComObject<CStarCollection>* pNodes = nullptr;
	if (ppNodes != nullptr)
	{
		*ppNodes = nullptr;
		CComObject<CStarCollection>::CreateInstance(&pNodes);
		pNodes->AddRef();
	}

	int iCount = _getNodes(this, strName, &pRetNode, pNodes);

	if (ppNodes != nullptr)
		pNodes->QueryInterface(IID_IStarCollection, (void**)ppNodes);

	if (pNodes != nullptr)
		pNodes->Release();

	return S_OK;
}

STDMETHODIMP CStar::GetStars(BSTR bstrName, IStar * *ppNode, IStarCollection * *ppNodes, long* pCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strName(bstrName);

	CStar* pRetNode = nullptr;

	if (ppNode != nullptr)
		* ppNode = nullptr;

	CComObject<CStarCollection> * pNodes = nullptr;
	if (ppNodes != nullptr)
	{
		*ppNodes = nullptr;
		CComObject<CStarCollection>::CreateInstance(&pNodes);
		pNodes->AddRef();
	}

	int iCount = _getNodes(this, strName, &pRetNode, pNodes);

	*pCount = iCount;

	if ((iCount > 0) && (ppNode != nullptr))
		pRetNode->QueryInterface(IID_IStar, (void**)ppNode);

	if (ppNodes != nullptr)
		pNodes->QueryInterface(IID_IStarCollection, (void**)ppNodes);

	if (pNodes != nullptr)
		pNodes->Release();

	return S_OK;
}

BOOL CStar::AddChildNode(CStar * pNode)
{
	m_vChildNodes.push_back(pNode);
	pNode->m_pParentObj = this;
	pNode->m_pRootObj = m_pRootObj;
	return true;
}

BOOL CStar::RemoveChildNode(CStar * pNode)
{
	auto it = find(m_vChildNodes.begin(), m_vChildNodes.end(), pNode);
	if (it != m_vChildNodes.end())
	{
		m_vChildNodes.erase(it);
		return true;
	}
	return false;
}

STDMETHODIMP CStar::get_Quasar(IQuasar * *pVal)
{
	if (m_pStarCommonData->m_pQuasar)
		* pVal = m_pStarCommonData->m_pQuasar;

	return S_OK;
}

STDMETHODIMP CStar::get_HostQuasar(IQuasar * *pVal)
{
	if (m_pHostQuasar)
		* pVal = m_pHostQuasar;

	return S_OK;
}

STDMETHODIMP CStar::Refresh(void)
{
	if (m_pDisp)
	{
		CComQIPtr<IWebBrowser2> pWebCtrl(m_pDisp);
		if (pWebCtrl)
			pWebCtrl->Refresh();
	}

	return S_OK;
}

STDMETHODIMP CStar::get_Height(LONG * pVal)
{
	RECT rc;
	::GetClientRect(m_pHostWnd->m_hWnd, &rc);
	*pVal = rc.bottom;
	return S_OK;
}

STDMETHODIMP CStar::get_Width(LONG * pVal)
{
	RECT rc;
	::GetClientRect(m_pHostWnd->m_hWnd, &rc);
	*pVal = rc.right;

	return S_OK;
}

STDMETHODIMP CStar::get_OfficeObj(IDispatch * *pVal)
{
	return S_OK;
}

STDMETHODIMP CStar::get_Extender(IDispatch * *pVal)
{
	if (m_pExtender)
	{
		*pVal = m_pExtender;
		(*pVal)->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CStar::put_Extender(IDispatch * newVal)
{
	if (m_pExtender)
		m_pExtender->Release();
	m_pExtender = newVal;
	m_pExtender->AddRef();

	return S_OK;
}

STDMETHODIMP CStar::get_GalaxyCluster(IGalaxyCluster * *pVal)
{
	*pVal = (IGalaxyCluster*)m_pStarCommonData->m_pQuasar->m_pGalaxyCluster;
	return S_OK;
}

STDMETHODIMP CStar::get_NameAtWindowPage(BSTR * pVal)
{
	*pVal = m_strNodeName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CStar::GetCtrlByName(BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch * *ppRetDisp)
{
	if (g_pHubble->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
		* ppRetDisp = g_pHubble->m_pCLRProxy->GetCtrlByName(m_pDisp, bstrName, bFindInChild ? true : false);

	return S_OK;
}

STDMETHODIMP CStar::GetCtrlValueByName(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR * bstrVal)
{
	if (g_pHubble->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
	{
		*bstrVal = g_pHubble->m_pCLRProxy->GetCtrlValueByName(m_pDisp, bstrName, bFindInChild ? true : false);
	}
	return S_OK;
}

STDMETHODIMP CStar::SetCtrlValueByName(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal)
{
	if (g_pHubble->m_pCLRProxy && m_nViewType == CLRCtrl && m_pDisp)
	{
		g_pHubble->m_pCLRProxy->SetCtrlValueByName(m_pDisp, bstrName, bFindInChild ? true : false, bstrVal);
	}
	return S_OK;
}

CStarCollection::CStarCollection()
{
	m_pNodes = &m_vNodes;
	g_pHubble->m_mapWndNodeCollection[(__int64)this] = this;
}

CStarCollection::~CStarCollection()
{
	auto it = g_pHubble->m_mapWndNodeCollection.find((__int64)this);
	if (it != g_pHubble->m_mapWndNodeCollection.end())
	{
		g_pHubble->m_mapWndNodeCollection.erase(it);
	}
	m_vNodes.clear();
}

STDMETHODIMP CStarCollection::get_StarCount(long* pCount)
{
	*pCount = (int)m_pNodes->size();
	return S_OK;
}

STDMETHODIMP CStarCollection::get_Item(long iIndex, IStar * *ppNode)
{
	if (iIndex < 0 || iIndex >= (int)m_pNodes->size()) return E_INVALIDARG;

	CStar * pNode = m_pNodes->operator [](iIndex);

	return pNode->QueryInterface(IID_IStar, (void**)ppNode);
}

STDMETHODIMP CStarCollection::get__NewEnum(IUnknown * *ppVal)
{
	*ppVal = nullptr;

	struct _CopyVariantFromIUnkown
	{
		static HRESULT copy(VARIANT* p1, CStar* const* p2)
		{
			CStar* pNode = *p2;
			p1->vt = VT_UNKNOWN;
			return pNode->QueryInterface(IID_IUnknown, (void**) & (p1->punkVal));
		}

		static void init(VARIANT* p) { VariantInit(p); }
		static void destroy(VARIANT* p) { VariantClear(p); }
	};

	typedef CComEnumOnSTL<IEnumVARIANT, & IID_IEnumVARIANT, VARIANT, _CopyVariantFromIUnkown, CStarVector>
		CComEnumVariantOnVector;

	CComObject<CComEnumVariantOnVector>* pe = 0;
	HRESULT hr = CComObject<CComEnumVariantOnVector>::CreateInstance(&pe);

	if (SUCCEEDED(hr))
	{
		hr = pe->AddRef();
		hr = pe->Init(GetUnknown(), *m_pNodes);

		if (SUCCEEDED(hr))
			hr = pe->QueryInterface(ppVal);

		hr = pe->Release();
	}

	return hr;
}

STDMETHODIMP CStar::get_DocXml(BSTR * pVal)
{
	g_pHubble->UpdareStar(m_pRootObj);
	CString strXml = m_pStarCommonData->m_pHubbleParse->xml();
	strXml.Replace(_T("/><"), _T("/>\r\n<"));
	strXml.Replace(_T("/>"), _T("></node>"));
	*pVal = strXml.AllocSysString();
	strXml.ReleaseBuffer();

	return S_OK;
}

STDMETHODIMP CStar::get_rgbMiddle(OLE_COLOR * pVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		*pVal = OLE_COLOR(pSplitter->rgbMiddle);
	}
	else
	{
		*pVal = OLE_COLOR(RGB(240, 240, 240));
	}
	return S_OK;
}

STDMETHODIMP CStar::put_rgbMiddle(OLE_COLOR newVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
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

STDMETHODIMP CStar::get_rgbLeftTop(OLE_COLOR * pVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		*pVal = OLE_COLOR(pSplitter->rgbLeftTop);
	}
	else
	{
		*pVal = OLE_COLOR(RGB(240, 240, 240));
	}
	return S_OK;
}

STDMETHODIMP CStar::put_rgbLeftTop(OLE_COLOR newVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		OleTranslateColor(newVal, NULL, &pSplitter->rgbLeftTop);
		CString strRGB = _T("");
		strRGB.Format(_T("RGB(%d,%d,%d)"), GetRValue(pSplitter->rgbLeftTop), GetGValue(pSplitter->rgbLeftTop), GetBValue(pSplitter->rgbLeftTop));
		put_Attribute(CComBSTR(L"lefttopcolor"), strRGB.AllocSysString());
		pSplitter->Invalidate();
	}
	return S_OK;
}

STDMETHODIMP CStar::get_rgbRightBottom(OLE_COLOR * pVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		*pVal = OLE_COLOR(pSplitter->rgbRightBottom);
	}
	else
		*pVal = OLE_COLOR(RGB(240, 240, 240));

	return S_OK;
}

STDMETHODIMP CStar::put_rgbRightBottom(OLE_COLOR newVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
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

STDMETHODIMP CStar::get_Hmin(int* pVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		*pVal = pSplitter->m_Hmin;
	}
	return S_OK;
}

STDMETHODIMP CStar::put_Hmin(int newVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		pSplitter->m_Hmin = min(pSplitter->m_Hmax, newVal);
		CString strVal = _T("");
		strVal.Format(_T("%d"), pSplitter->m_Hmin);
		put_Attribute(CComBSTR(L"hmin"), strVal.AllocSysString());
	}

	return S_OK;
}

STDMETHODIMP CStar::get_Hmax(int* pVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		*pVal = pSplitter->m_Hmax;
	}
	return S_OK;
}

STDMETHODIMP CStar::put_Hmax(int newVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		pSplitter->m_Hmax = max(pSplitter->m_Hmin, newVal);
		CString strVal = _T("");
		strVal.Format(_T("%d"), pSplitter->m_Hmax);
		put_Attribute(CComBSTR(L"hmax"), strVal.AllocSysString());
	}

	return S_OK;
}

STDMETHODIMP CStar::get_Vmin(int* pVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		*pVal = pSplitter->m_Vmin;
	}

	return S_OK;
}

STDMETHODIMP CStar::put_Vmin(int newVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		pSplitter->m_Vmin = min(pSplitter->m_Vmax, newVal);
		CString strVal = _T("");
		strVal.Format(_T("%d"), pSplitter->m_Vmin);
		put_Attribute(CComBSTR(L"vmin"), strVal.AllocSysString());
	}

	return S_OK;
}

STDMETHODIMP CStar::get_Vmax(int* pVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		*pVal = pSplitter->m_Vmax;
	}

	return S_OK;
}

STDMETHODIMP CStar::put_Vmax(int newVal)
{
	if (m_nViewType == Splitter)
	{
		CGrid* pSplitter = (CGrid*)m_pHostWnd;
		pSplitter->m_Vmax = max(pSplitter->m_Vmin, newVal);
		CString strVal = _T("");
		strVal.Format(_T("%d"), pSplitter->m_Vmax);
		put_Attribute(CComBSTR(L"vmax"), strVal.AllocSysString());
	}

	return S_OK;
}


STDMETHODIMP CStar::get_HostStar(IStar * *pVal)
{
	if (m_pStarCommonData->m_pHostClientView)
		* pVal = m_pStarCommonData->m_pHostClientView->m_pStar;

	return S_OK;
}


STDMETHODIMP CStar::put_HostStar(IStar * newVal)
{
	return S_OK;
}


STDMETHODIMP CStar::get_ActivePage(int* pVal)
{
	if (this->m_nViewType == StarType::TabbedWnd)
	{
		CComBSTR bstr(L"");
		get_Attribute(CComBSTR(L"activepage"), &bstr);
		*pVal = _wtoi(OLE2T(bstr));
	}
	return S_OK;
}


STDMETHODIMP CStar::put_ActivePage(int newVal)
{
	if (this->m_nViewType == StarType::TabbedWnd && newVal < m_nCols)
	{
		HWND hwnd = nullptr;
		int nOldPage = 0;
		get_ActivePage(&nOldPage);
		if (nOldPage == newVal)
			return S_OK;
		IStar * pOldNode = nullptr;
		GetStar(0, newVal, &pOldNode);
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
		IStar* pNode = nullptr;
		this->GetStar(0, newVal, &pNode);
		if (pNode)
		{
			::ShowWindow(hwnd, SW_HIDE);
			ActiveTabPage(pNode);
		}
	}

	return S_OK;
}

STDMETHODIMP CStar::get_MasterRow(int* pVal)
{
	return S_OK;
}

STDMETHODIMP CStar::put_MasterRow(int newVal)
{
	return S_OK;
}

STDMETHODIMP CStar::get_MasterCol(int* pVal)
{
	return S_OK;
}

STDMETHODIMP CStar::put_MasterCol(int newVal)
{
	return S_OK;
}

HRESULT CStar::Fire_OpenComplete()
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

	for (auto it : m_mapWndNodeProxy)
	{
		it.second->OnOpenComplete();
	}

	return hr;
}

HRESULT CStar::Fire_Destroy()
{
	if (m_pWebBrowser)
	{
		if(::IsChild(m_pHostWnd->m_hWnd,m_pWebBrowser->m_hWnd))
			m_pWebBrowser->DestroyWindow();
		m_pWebBrowser = nullptr;
	}
	if (m_pRootObj == this)
	{
		if (m_pStarCommonData->m_pQuasar->m_pGalaxyCluster)
		{
			CString strKey = m_strKey + _T("@") + m_pStarCommonData->m_pQuasar->m_strQuasarName + _T("@") + m_pStarCommonData->m_pQuasar->m_pGalaxyCluster->m_strConfigFileNodeName;
			auto it = m_pStarCommonData->m_pQuasar->m_mapNeedSaveToConfigNode.find(m_strKey);
			if (it != m_pStarCommonData->m_pQuasar->m_mapNeedSaveToConfigNode.end())
			{
				if (it->second == this)
				{
					CString strFile = m_pStarCommonData->m_pQuasar->m_pGalaxyCluster->m_strPageFilePath;
					CString strXml = _T("");
					CTangramXmlParse m_Parse;
					CTangramXmlParse xml;
					CTangramXmlParse* m_pHubblePageParse = nullptr;
					if (::PathFileExists(strFile))
					{
						if (m_Parse.LoadFile(strFile))
						{
							m_pHubblePageParse = m_Parse.GetChild(_T("tangrampage"));
							if (m_pHubblePageParse == nullptr)
							{
								m_Parse.AddNode(_T("tangrampage"));
								m_pHubblePageParse = m_Parse.GetChild(_T("tangrampage"));
							}
							if (m_pHubblePageParse)
							{
								CTangramXmlParse* pTangramPageParse = m_pHubblePageParse->GetChild(m_pStarCommonData->m_pGalaxyCluster->m_strConfigFileNodeName);
								if (pTangramPageParse == nullptr)
								{
									pTangramPageParse = m_pHubblePageParse->AddNode(m_pStarCommonData->m_pGalaxyCluster->m_strConfigFileNodeName);
								}
								if (pTangramPageParse)
								{
									CString strFrameName = m_pStarCommonData->m_pQuasar->m_strQuasarName;

									CTangramXmlParse* pTangramFrameParse = pTangramPageParse->GetChild(strFrameName);
									if (pTangramFrameParse == nullptr)
										pTangramFrameParse = pTangramPageParse->AddNode(strFrameName);
									if (pTangramFrameParse)
									{
										if (m_pWindow)
										{
											if (m_nActivePage > 0)
											{
												CString strVal = _T("");
												strVal.Format(_T("%d"), m_nActivePage);
												m_pHostParse->put_attr(_T("activepage"), strVal);
											}
											m_pWindow->Save();
										}
										if (m_nViewType == Splitter)
										{
											((CGrid*)m_pHostWnd)->Save();
										}

										for (auto it2 : m_vChildNodes)
										{
											g_pHubble->UpdareStar(it2);
										}
										CTangramXmlParse* pParse = pTangramFrameParse->GetChild(m_strKey);
										if (pParse)
											pTangramFrameParse->RemoveNode(m_strKey);

										strXml = m_pStarCommonData->m_pHubbleParse->xml();
										CString _strName = m_pStarCommonData->m_pHubbleParse->name();
										if (_strName != m_strKey)
										{
											CString strName = _T("<") + _strName;
											int nPos = strXml.ReverseFind('<');
											CString str = strXml.Left(nPos);
											nPos = str.Find(strName);
											str = str.Mid(nPos + strName.GetLength());
											strXml = _T("<");
											strXml += m_strKey;
											strXml += str;
											strXml += _T("</");
											strXml += m_strKey;
											strXml += _T(">");
										}
										xml.LoadXml(strXml);
										if (pTangramFrameParse->AddNode(&xml, _T("")))
											m_Parse.SaveFile(strFile);
									}
								}
							}
						}
					}
					if (strXml != _T(""))
					{
						CTangramXmlParse m_Parse;
						if (m_Parse.LoadFile(strFile))
						{
							m_pHubblePageParse = m_Parse.GetChild(_T("tangrampage"));
							if (m_pHubblePageParse == nullptr)
							{
								m_Parse.AddNode(_T("tangrampage"));
								m_pHubblePageParse = m_Parse.GetChild(_T("tangrampage"));
							}
							if (m_pHubblePageParse)
							{
								CTangramXmlParse* pPageParse = m_pHubblePageParse->GetChild(m_pStarCommonData->m_pGalaxyCluster->m_strConfigFileNodeName);
								if (pPageParse)
								{
									CString strFrameName = m_pStarCommonData->m_pQuasar->m_strQuasarName;

									CTangramXmlParse* pFrameParse = pPageParse->GetChild(strFrameName);
									if (pFrameParse == nullptr)
										pFrameParse = pPageParse->AddNode(strFrameName);
									if (pFrameParse)
									{
										CTangramXmlParse* pParse = pFrameParse->GetChild(m_strKey);
										if (pParse)
											pFrameParse->RemoveNode(m_strKey);
										if (pFrameParse->AddNode(&xml, _T("")))
											m_Parse.SaveFile(strFile);
									}
								}
							}
						}
					}
				}
			}
		}
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
	for (auto it : m_mapWndNodeProxy)
	{
		it.second->OnDestroy();
	}

	if (g_pHubble->m_pCLRProxy)
	{
		g_pHubble->m_pCLRProxy->ReleaseHubbleObj((IStar*)this);
	}
	//if (m_pHubbleCloudSession)
	//	delete m_pHubbleCloudSession;
	//m_pHubbleCloudSession = nullptr;
	return hr;
}

HRESULT CStar::Fire_NodeAddInCreated(IDispatch * pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
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
			g_pHubble->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapWndNodeProxy)
	{
		it.second->OnNodeAddInCreated(pAddIndisp, OLE2T(bstrAddInID), OLE2T(bstrAddInXml));
	}

	return hr;
}

HRESULT CStar::Fire_NodeAddInsCreated()
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
				hr = pConnection->Invoke(4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	for (auto it : m_mapWndNodeProxy)
	{
		it.second->OnNodeAddInsCreated();
	}
	return hr;
}

HRESULT CStar::Fire_NodeDocumentComplete(IDispatch * ExtenderDisp, BSTR bstrURL)
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
			g_pHubble->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}

	for (auto it : m_mapWndNodeProxy)
	{
		it.second->OnNodeDocumentComplete(ExtenderDisp, OLE2T(bstrURL));
	}

	return hr;
}

HRESULT CStar::Fire_ControlNotify(IStar * sender, LONG NotifyCode, LONG CtrlID, LONGLONG CtrlHandle, BSTR CtrlClassName)
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
			g_pHubble->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			g_pHubble->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;
				hr = pConnection->Invoke(6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
	}
	for (auto it : m_mapWndNodeProxy)
	{
		it.second->OnControlNotify(sender, NotifyCode, CtrlID, (HWND)CtrlHandle, OLE2T(CtrlClassName));
	}
	return hr;
}

HRESULT CStar::Fire_TabChange(LONG ActivePage, LONG OldPage)
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
	for (auto it : m_mapWndNodeProxy)
	{
		it.second->OnTabChange(ActivePage, OldPage);
	}
	return hr;
}

HRESULT CStar::Fire_IPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
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
	//for (auto it : m_mapWndNodeProxy)
	//{
	//	it.second->OnTabChange(ActivePage, OldPage);
	//}
	return hr;
}

STDMETHODIMP CStar::put_SaveToConfigFile(VARIANT_BOOL newVal)
{
	if (m_pRootObj == this)
	{
		CString strKey = m_strKey + _T("@") + m_pStarCommonData->m_pQuasar->m_strQuasarName + _T("@") + m_pStarCommonData->m_pQuasar->m_pGalaxyCluster->m_strConfigFileNodeName;
		auto it = m_pStarCommonData->m_pQuasar->m_mapNeedSaveToConfigNode.find(m_strKey);
		if (newVal)
		{
			if (it == m_pStarCommonData->m_pQuasar->m_mapNeedSaveToConfigNode.end())
			{
				m_pStarCommonData->m_pQuasar->m_mapNeedSaveToConfigNode[m_strKey] = this;
				if (m_pStarCommonData->m_pGalaxyCluster->m_strConfigFileNodeName == _T(""))
				{
					m_pStarCommonData->m_pGalaxyCluster->put_ConfigName(CComBSTR(L""));
				}
				auto it2 = m_pStarCommonData->m_pGalaxyCluster->m_mapNeedSaveQuasar.find(m_pStarCommonData->m_pQuasar->m_hWnd);
				if (it2 == m_pStarCommonData->m_pGalaxyCluster->m_mapNeedSaveQuasar.end())
					m_pStarCommonData->m_pGalaxyCluster->m_mapNeedSaveQuasar[m_pStarCommonData->m_pQuasar->m_hWnd] = m_pStarCommonData->m_pQuasar;
			}
		}
		else if (it != m_pStarCommonData->m_pQuasar->m_mapNeedSaveToConfigNode.end())
		{
			m_pStarCommonData->m_pQuasar->m_mapNeedSaveToConfigNode.erase(it);
			if (m_pStarCommonData->m_pQuasar->m_mapNeedSaveToConfigNode.size() == 0)
			{
				auto it2 = m_pStarCommonData->m_pGalaxyCluster->m_mapNeedSaveQuasar.find(m_pStarCommonData->m_pQuasar->m_hWnd);
				if (it2 != m_pStarCommonData->m_pGalaxyCluster->m_mapNeedSaveQuasar.end())
					m_pStarCommonData->m_pGalaxyCluster->m_mapNeedSaveQuasar.erase(it2);
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CStar::get_DockObj(BSTR bstrName, LONGLONG * pVal)
{
	CString strName = OLE2T(bstrName);
	if (m_nViewType == CLRCtrl)
	{
		CStarWnd* pWnd = (CStarWnd*)m_pHostWnd;
		auto it = pWnd->m_mapDockCtrl.find(strName);
		if (it != pWnd->m_mapDockCtrl.end())
		{
			*pVal = (LONGLONG)it->second;
		}
	}
	return S_OK;
}

STDMETHODIMP CStar::put_DockObj(BSTR bstrName, LONGLONG newVal)
{
	CString strName = OLE2T(bstrName);
	if (/*m_nViewType == CLRCtrl&&*/::IsWindow((HWND)newVal) && strName != _T(""))
	{
		CStarWnd* pWnd = (CStarWnd*)m_pHostWnd;
		auto it = pWnd->m_mapDockCtrl.find(strName);
		if (it == pWnd->m_mapDockCtrl.end())
		{
			pWnd->m_mapDockCtrl[strName] = (HWND)newVal;
		}
	}
	return S_OK;
}

STDMETHODIMP CStar::NavigateURL(BSTR bstrURL, IDispatch * dispObjforScript)
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

		HWND hBrowser = g_pHubble->m_pBrowserFactory->CreateBrowser(((CStarWnd*)m_pHostWnd)->m_hWnd, s);
		((CStarWnd*)m_pHostWnd)->m_hFormWnd = hBrowser;
		g_pHubble->m_hParent = NULL;
		auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
		if (it != g_pHubble->m_mapBrowserWnd.end())
		{
			m_pWebBrowser = (CBrowser*)it->second;
			//m_pWebBrowser->m_pStar = this;
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

STDMETHODIMP CStar::get_URL(BSTR * pVal)
{
	if (m_pHostParse != nullptr)
	{
		CString strVal = m_pHostParse->attr(_T("url"), _T(""));
		*pVal = strVal.AllocSysString();
		strVal.ReleaseBuffer();
	}
	return S_OK;
}

STDMETHODIMP CStar::put_URL(BSTR newVal)
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
		((CStarWnd*)m_pHostWnd)->m_hFormWnd = hBrowser;
		g_pHubble->m_hParent = NULL;
		auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
		if (it != g_pHubble->m_mapBrowserWnd.end())
		{
			m_pWebBrowser = (CBrowser*)it->second;
			m_pWebBrowser->m_pParentStar = this;
		}

		return S_OK;
	}
	//if (m_pWebBrowser)
	//{
	//	m_pWebBrowser->DestroyWindow();
	//	m_pWebBrowser = nullptr;
	//	CString strURL = OLE2T(newVal);
	//	strURL += _T("|");

	//	HWND hBrowser = g_pHubble->m_pBrowserFactory->CreateBrowser(((CStarWnd*)m_pHostWnd)->m_hWnd, strURL);
	//	((CStarWnd*)m_pHostWnd)->m_hFormWnd = hBrowser;
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

STDMETHODIMP CStar::SendIPCMessage(BSTR bstrTo, BSTR bstrPayload, BSTR bstrExtra, BSTR bstrMsgId, BSTR* bstrRet)
{
	if (m_pStarCommonData->m_pQuasar->m_pWebPageWnd)
	{
		m_pStarCommonData->m_pQuasar->m_pWebPageWnd->SendChromeIPCMessage(_T("I_DONT_KNOW"), OLE2T(bstrTo), OLE2T(bstrMsgId), OLE2T(bstrExtra), L"", L"");
	}
	else
	{
		if (m_pStarCommonData->m_pQuasar->m_pHostWebBrowserNode)
		{
			if (m_pStarCommonData->m_pQuasar->m_pHostWebBrowserWnd)
			{
				HWND hPWnd = m_pStarCommonData->m_pQuasar->m_pHostWebBrowserWnd->m_hWnd;
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
