/********************************************************************************
*					Open Universe - version 1.0.1.20							*
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
********************************************************************************/

// TangramTreeView.cpp : Implementation of CTangramTreeView

#include "stdafx.h"
#include "TangramHtmlTreeWnd.h"
#include "TangramTreeView.h"


// CTangramTreeView
CTangramTreeView::CTangramTreeView()
{
	m_pHubbleHtmlTreeWnd = NULL;
}


STDMETHODIMP CTangramTreeView::put_TangramTreeViewCallBack(BSTR bstrKey, IHubbleTreeViewCallBack* newVal)
{
	CString strKey = OLE2T(bstrKey);
	strKey.Trim();
	strKey.MakeLower();
	auto it = m_mapHubbleTreeViewCallBack.find(strKey);
	if(it==m_mapHubbleTreeViewCallBack.end())
	{
		m_mapHubbleTreeViewCallBack[strKey] = newVal;
		newVal->AddRef();
	}

	return S_OK;
}

void CTangramTreeView::FinalRelease()
{
	for(auto it : m_mapHubbleTreeViewCallBack)
	{
		it.second->Release();
	}
	m_mapHubbleTreeViewCallBack.erase(m_mapHubbleTreeViewCallBack.begin(),m_mapHubbleTreeViewCallBack.end());
}

STDMETHODIMP CTangramTreeView::AddTreeNode(long hItem, BSTR bstrXml)
{
	if(m_pHubbleHtmlTreeWnd)
	{
		CTangramXmlParse m_Parse;
		if(m_Parse.LoadXml(OLE2T(bstrXml)))
		{
			int nCount = 0;
			if((HTREEITEM)hItem)
			{
				HTREEITEM hChild = m_pHubbleHtmlTreeWnd->GetChildItem((HTREEITEM)hItem);
				CTangramXHtmlTreeNode *pXTCD = m_pHubbleHtmlTreeWnd->GetItemDataStruct((HTREEITEM)hChild);
				if(pXTCD->m_bWaitingFor)
					m_pHubbleHtmlTreeWnd->DeleteItem(hChild);
			}
				
			CTangramXHtmlTreeNode *pXTCD = m_pHubbleHtmlTreeWnd->GetItemDataStruct((HTREEITEM)hItem);
			if(pXTCD)
				pXTCD->m_strTangramXML = _T("");

			m_pHubbleHtmlTreeWnd->LoadXml(&m_Parse,(HTREEITEM)hItem,nCount);
			m_pHubbleHtmlTreeWnd->Expand((HTREEITEM)hItem,TVE_EXPAND);
		}
	}

	return S_OK;
}

STDMETHODIMP CTangramTreeView::InsertNode(BSTR bstrXml, long* hItem)
{
	if(m_pHubbleHtmlTreeWnd)
	{
		CTangramXmlParse m_Parse;
		if(m_Parse.LoadXml(OLE2T(bstrXml)))
		{
			int nCount = 0;
			HTREEITEM _hItem = m_pHubbleHtmlTreeWnd->InsertXmlItem(&m_Parse,0);
			* hItem = (long)_hItem;
		}
	}
	return S_OK;
}

STDMETHODIMP CTangramTreeView::get_FirstRoot(long* pVal)
{
	if(m_pHubbleHtmlTreeWnd&&m_pHubbleHtmlTreeWnd->m_hFirstRoot)
	{
		* pVal = (long)m_pHubbleHtmlTreeWnd->m_hFirstRoot;
	}

	return S_OK;
}
