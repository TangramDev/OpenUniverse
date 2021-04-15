/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202104160062
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

 // Galaxy.cpp : implementation file of CGalaxy
 //

#include "stdafx.h"
#include "UniverseApp.h"
#include "Cosmos.h"
#include "GridWnd.h"
#include "XobjWnd.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "TangramHtmlTreeWnd.h"
#include "EclipsePlus\EclipseAddin.h"
#include "Wormhole.h"

/////////////////////////////////////////////////////////////////////////////
// CCosmosTreeCtrl

CCosmosTreeCtrl::CCosmosTreeCtrl() noexcept
{
}

CCosmosTreeCtrl::~CCosmosTreeCtrl()
{
	for (auto it : m_mapTreeItemData)
	{
		delete it.second;
	}
	m_mapTreeItemData.clear();
}

BEGIN_MESSAGE_MAP(CCosmosTreeCtrl, CTreeCtrl)
	ON_MESSAGE(WM_XOBJCREATED, OnXobjCreatedMsg)
	ON_NOTIFY_REFLECT_EX(NM_RCLICK, &CCosmosTreeCtrl::OnNMRClick)
	ON_NOTIFY_REFLECT_EX(TVN_SELCHANGED, &CCosmosTreeCtrl::OnTvnSelchanged)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CCosmosTreeCtrl::OnNMClick)
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK, &CCosmosTreeCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT_EX(NM_RETURN, &CCosmosTreeCtrl::OnNMReturn)
	ON_NOTIFY_REFLECT_EX(NM_TVSTATEIMAGECHANGING, &CCosmosTreeCtrl::OnNMTVStateImageChanging)
	ON_NOTIFY_REFLECT_EX(TVN_BEGINDRAG, &CCosmosTreeCtrl::OnTvnBegindrag)
	ON_NOTIFY_REFLECT_EX(TVN_DELETEITEM, &CCosmosTreeCtrl::OnTvnDeleteitem)
	ON_NOTIFY_REFLECT_EX(TVN_ITEMCHANGED, &CCosmosTreeCtrl::OnTvnItemChanged)
	ON_NOTIFY_REFLECT_EX(TVN_KEYDOWN, &CCosmosTreeCtrl::OnTvnKeydown)
	ON_MESSAGE(WM_CLOUDMSGRECEIVED, OnCloudMsgReceived)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCosmosTreeCtrl message handlers

LRESULT CCosmosTreeCtrl::OnCloudMsgReceived(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_CLOUDMSGRECEIVED, wParam, lParam);
	CSession* pSession = (CSession*)lParam;
	CWebPageImpl* m_pOwner = pSession->m_pOwner;
	CChromeRenderFrameHost* m_pChromeRenderFrameHost = m_pOwner->m_pChromeRenderFrameHost;
	CString strMsgID = pSession->GetString(L"msgID");
	return lRes;
}

HTREEITEM CCosmosTreeCtrl::FillTreeView(CTangramXmlParse* pParse, CTangramXmlParse* pParseTag, HTREEITEM hParentItem)
{
	if (pParse)
	{
		CString name = pParse->name();
		bool bItem = pParse->attrBool(_T("treeitem"), false);
		if (bItem)
		{
			CosmosUIItemData* pTreeItemData = new CosmosUIItemData;
			pTreeItemData->m_hParentItem = hParentItem;
			pTreeItemData->m_strKey = pParse->attr(_T("id"), _T("default"));
			if (pParseTag)
				pTreeItemData->m_strData = pParseTag->xml();
			HTREEITEM hItem = InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_STATE | TVIF_PARAM, pParse->attr(_T("text"), _T("tangram")), pParse->attrInt(_T("imageindex")), pParse->attrInt(_T("selectedimageindex")), 0, 0, (LPARAM)pTreeItemData, hParentItem, 0);
			m_mapTreeItemData[hItem] = pTreeItemData;
			int nCount = pParse->GetCount();
			if (nCount)
			{
				for (int i = 0; i < nCount; i++)
				{
					CTangramXmlParse* _pChild = pParse->GetChild(i);
					if (_pChild->attrBool(_T("treeitem"), false))
					{
						CString name = pParse->GetChild(i)->name() + _T("_tag");
						FillTreeView(pParse->GetChild(i), pParse->GetChild(name), hItem);
					}
				}
			}
			int nExpand = pParse->attrInt(_T("expand"));
			Expand(hItem, nExpand);
		}
		//return hItem;
	}
	return nullptr;
}

BOOL CCosmosTreeCtrl::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CWebView* pPage = nullptr;
	if (m_pGalaxy)
	{
		pPage = m_pGalaxy->m_pWebPageWnd;
	}
	return false;
}

BOOL CCosmosTreeCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosTreeCtrl::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosTreeCtrl::OnNMReturn(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosTreeCtrl::OnNMTVStateImageChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosTreeCtrl::OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	return false;
}

BOOL CCosmosTreeCtrl::OnTvnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	return false;
}

BOOL CCosmosTreeCtrl::OnTvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	return false;
}

BOOL CCosmosTreeCtrl::OnTvnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	return false;
}

BOOL CCosmosTreeCtrl::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CosmosUIItemData* pTreeItemData = (CosmosUIItemData*)pNMTreeView->itemNew.lParam;
	if (m_pGalaxy == nullptr)
	{
		DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
		auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
		if (iter != pThreadInfo->m_mapGalaxy.end())
		{
			m_pGalaxy = (CGalaxy*)iter->second;
		}
	}
	if (pTreeItemData)
	{
		CXobj* _pSender = nullptr;
		if (m_pHostObj)
			_pSender = m_pHostObj;
		else if (m_pGalaxy)
		{
			_pSender = m_pGalaxy->m_pWorkXobj;
		}
		if (_pSender && _pSender->m_pWormhole)
		{
			_pSender->m_pWormhole->InsertString(_T("msgID"), _T("TREECTRL_TvnSelchanged"));
			_pSender->m_pWormhole->InsertString(_T("treenodekey"), pTreeItemData->m_strKey);
			_pSender->m_pWormhole->InsertString(_T("treenodetag"), pTreeItemData->m_strData);
			_pSender->m_pWormhole->SendMessage();
			_pSender->m_pWormhole->InsertString(_T("msgID"), _T(""));
		}
	}
	return false;
}

LRESULT CCosmosTreeCtrl::OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_XOBJCREATED, wParam, lParam);
	switch (lParam)
	{
	case 10000:
	{
		if (m_pHostObj == nullptr)
		{
			m_pHostObj = (CXobj*)g_pCosmosImpl->GetXobj(m_hWnd);
		}
		DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);

		if (m_pGalaxy == nullptr)
		{
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
			if (iter != pThreadInfo->m_mapGalaxy.end())
			{
				m_pGalaxy = (CGalaxy*)iter->second;
			}
		}
		HANDLE hData = ::GetProp(m_hWnd, _T("CosmosData"));
		if (hData)
		{
			CTangramXmlParse* pParse = (CTangramXmlParse*)hData;
			if (pParse)
			{
				int nID = pParse->attrInt(_T("clientid"), 0);
				if (nID == ::GetDlgCtrlID(m_hWnd))
				{
					bool b = pParse->attrBool(_T("usingwebdata"), false);
					CTangramXmlParse* pUIData = pParse->GetChild(_T("uidata"));
					CTangramXmlParse* pUIDataTag = pParse->GetChild(_T("uidata_tag"));
					if (b && pUIData)
					{
						DeleteAllItems();
						FillTreeView(pUIData, pUIDataTag, NULL);
					}
				}
			}
		}
	}
	break;
	case 20210108:
	{
		IXobj* pObj = nullptr;
		g_pCosmos->GetXobjFromHandle((__int64)m_hWnd, &pObj);

		HWND _hWnd = (HWND)m_hWnd;
		CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
		while (pInfo == nullptr)
		{
			_hWnd = ::GetParent(_hWnd);
			if (_hWnd == 0)
				break;
			pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
		}
		if (pInfo)
			m_pHostObj = (CXobj*)pInfo->m_pXobj;
		if (m_pHostObj)
		{
			if (m_pHostObj->m_pHostParse)
			{
				if (m_pHostObj->m_pHostWnd->m_hWnd == m_hWnd)
				{
					CTangramXmlParse* pUIData = m_pHostObj->m_pHostParse->GetChild(_T("uidata"));
					if (pUIData)
					{
						bool b = pUIData->attrBool(_T("usingwebdata"), false);
						if (b)
						{
							DeleteAllItems();
							CTangramXmlParse* pUIDataTag = m_pHostObj->m_pHostParse->GetChild(_T("uidata_tag"));
							FillTreeView(pUIData, pUIDataTag, NULL);
						}

					}
				}
				else
				{
					int nID = ::GetDlgCtrlID(m_hWnd);
				}
			}
		}
		return S_OK;
	}
	break;
	}
	return lRes;
}

void CCosmosTreeCtrl::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CTreeCtrl::PostNcDestroy();
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CCosmosListCtrl

CCosmosListCtrl::CCosmosListCtrl() noexcept
{
}

CCosmosListCtrl::~CCosmosListCtrl()
{
	for (auto it : m_mapListItemData)
	{
		delete it.second;
	}
	m_mapListItemData.clear();
}

BEGIN_MESSAGE_MAP(CCosmosListCtrl, CListCtrl)
	ON_MESSAGE(WM_XOBJCREATED, OnXobjCreatedMsg)
	ON_MESSAGE(WM_CLOUDMSGRECEIVED, OnCloudMsgReceived)
	ON_NOTIFY_REFLECT_EX(LVN_DELETEITEM, &CCosmosListCtrl::OnLvnDeleteitem)
	ON_NOTIFY_REFLECT_EX(LVN_DELETEALLITEMS, &CCosmosListCtrl::OnLvnDeleteallitems)
	ON_NOTIFY_REFLECT_EX(LVN_INSERTITEM, &CCosmosListCtrl::OnLvnInsertitem)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, &CCosmosListCtrl::OnLvnItemchanged)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMACTIVATE, &CCosmosListCtrl::OnLvnItemActivate)
	ON_NOTIFY_REFLECT_EX(LVN_KEYDOWN, &CCosmosListCtrl::OnLvnKeydown)
	ON_NOTIFY_REFLECT_EX(LVN_LINKCLICK, &CCosmosListCtrl::OnLvnLinkClicked)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CCosmosListCtrl::OnNMClick)
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK, &CCosmosListCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT_EX(NM_HOVER, &CCosmosListCtrl::OnNMHover)
	ON_NOTIFY_REFLECT_EX(NM_KILLFOCUS, &CCosmosListCtrl::OnNMKillfocus)
	ON_NOTIFY_REFLECT_EX(NM_RCLICK, &CCosmosListCtrl::OnNMRClick)
	ON_NOTIFY_REFLECT_EX(NM_RDBLCLK, &CCosmosListCtrl::OnNMRDblclk)
	ON_NOTIFY_REFLECT_EX(NM_RETURN, &CCosmosListCtrl::OnNMReturn)
END_MESSAGE_MAP()

LRESULT CCosmosListCtrl::OnCloudMsgReceived(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_CLOUDMSGRECEIVED, wParam, lParam);
	CSession* pSession = (CSession*)lParam;
	CWebPageImpl* m_pOwner = pSession->m_pOwner;
	CChromeRenderFrameHost* m_pChromeRenderFrameHost = m_pOwner->m_pChromeRenderFrameHost;
	CString strMsgID = pSession->GetString(L"msgID");
	return lRes;
}

BOOL CCosmosListCtrl::OnLvnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnLvnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnLvnInsertitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnLvnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnLvnLinkClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosListCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnNMHover(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosListCtrl::OnNMKillfocus(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosListCtrl::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnNMRDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	return false;
}

BOOL CCosmosListCtrl::OnNMReturn(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

LRESULT CCosmosListCtrl::OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_XOBJCREATED, wParam, lParam);
	switch (lParam)
	{
	case 10000:
	{
		DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);

		if (m_pGalaxy == nullptr)
		{
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
			if (iter != pThreadInfo->m_mapGalaxy.end())
			{
				m_pGalaxy = (CGalaxy*)iter->second;
			}
		}
		HANDLE hData = ::GetProp(m_hWnd, _T("CosmosData"));
		if (hData)
		{
			CTangramXmlParse* pParse = (CTangramXmlParse*)hData;
			if (pParse)
			{
				int nID = pParse->attrInt(_T("clientid"), 0);
				if (nID == ::GetDlgCtrlID(m_hWnd))
				{
					CString xml = pParse->xml();
					bool b = pParse->attrBool(_T("usingwebdata"), false);
					CTangramXmlParse* pUIData = pParse->GetChild(_T("uidata"));
					CTangramXmlParse* pUIDataTag = pParse->GetChild(_T("uidata_tag"));
					if (b && pUIData)
					{
						DeleteAllItems();
						//FillTreeView(pUIData, pUIDataTag, NULL);
					}
				}
			}
		}
	}
	break;
	case 20210108:
	{
		IXobj* pObj = nullptr;
		g_pCosmos->GetXobjFromHandle((__int64)m_hWnd, &pObj);

		HWND _hWnd = (HWND)m_hWnd;
		CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
		while (pInfo == nullptr)
		{
			_hWnd = ::GetParent(_hWnd);
			if (_hWnd == 0)
				break;
			pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
		}
		if (pInfo)
			m_pHostObj = (CXobj*)pInfo->m_pXobj;
		if (m_pHostObj)
		{
			if (m_pHostObj->m_pHostParse)
			{
				if (m_pHostObj->m_pHostWnd->m_hWnd == m_hWnd)
				{
					CTangramXmlParse* pUIData = m_pHostObj->m_pHostParse->GetChild(_T("uidata"));
					if (pUIData)
					{

					}
				}
				else
				{
					int nID = ::GetDlgCtrlID(m_hWnd);
				}
			}
		}
		return S_OK;
	}
	break;
	}
	return lRes;
}

void CCosmosListCtrl::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CListCtrl::PostNcDestroy();
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CCosmosTabCtrl

CCosmosTabCtrl::CCosmosTabCtrl() noexcept
{
}

CCosmosTabCtrl::~CCosmosTabCtrl()
{
	for (auto it : m_mapTabItemData)
	{
		delete it.second;
	}
	m_mapTabItemData.clear();
}

BEGIN_MESSAGE_MAP(CCosmosTabCtrl, CTabCtrl)
	ON_MESSAGE(WM_CLOUDMSGRECEIVED, OnCloudMsgReceived)
	ON_MESSAGE(WM_XOBJCREATED, OnXobjCreatedMsg)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CCosmosTabCtrl::OnNMClick)
	ON_NOTIFY_REFLECT_EX(NM_RCLICK, &CCosmosTabCtrl::OnNMRClick)
	ON_NOTIFY_REFLECT_EX(TCN_KEYDOWN, &CCosmosTabCtrl::OnTcnKeydown)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, &CCosmosTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()

LRESULT CCosmosTabCtrl::OnCloudMsgReceived(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_CLOUDMSGRECEIVED, wParam, lParam);
	CSession* pSession = (CSession*)lParam;
	CWebPageImpl* m_pOwner = pSession->m_pOwner;
	CChromeRenderFrameHost* m_pChromeRenderFrameHost = m_pOwner->m_pChromeRenderFrameHost;
	CString strMsgID = pSession->GetString(L"msgID");
	return lRes;
}

BOOL CCosmosTabCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosTabCtrl::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

BOOL CCosmosTabCtrl::OnTcnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTCKEYDOWN* pTCKeyDown = reinterpret_cast<NMTCKEYDOWN*>(pNMHDR);
	return false;
}

BOOL CCosmosTabCtrl::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	return false;
}

LRESULT CCosmosTabCtrl::OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_XOBJCREATED, wParam, lParam);
	switch (lParam)
	{
	case 10000:
	{
		DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);

		if (m_pGalaxy == nullptr)
		{
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
			if (iter != pThreadInfo->m_mapGalaxy.end())
			{
				m_pGalaxy = (CGalaxy*)iter->second;
			}
		}
		HANDLE hData = ::GetProp(m_hWnd, _T("CosmosData"));
		if (hData)
		{
			CTangramXmlParse* pParse = (CTangramXmlParse*)hData;
			if (pParse)
			{
				int nID = pParse->attrInt(_T("clientid"), 0);
				if (nID == ::GetDlgCtrlID(m_hWnd))
				{
					CString xml = pParse->xml();
					bool b = pParse->attrBool(_T("usingwebdata"), false);
					CTangramXmlParse* pUIData = pParse->GetChild(_T("uidata"));
					CTangramXmlParse* pUIDataTag = pParse->GetChild(_T("uidata_tag"));
					if (b && pUIData)
					{
						DeleteAllItems();
						//FillTreeView(pUIData, pUIDataTag, NULL);
					}
				}
			}
		}
	}
	break;
	case 20210108:
	{
		IXobj* pObj = nullptr;
		g_pCosmos->GetXobjFromHandle((__int64)m_hWnd, &pObj);

		HWND _hWnd = (HWND)m_hWnd;
		CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
		while (pInfo == nullptr)
		{
			_hWnd = ::GetParent(_hWnd);
			if (_hWnd == 0)
				break;
			pInfo = (CosmosInfo*)::GetProp((HWND)_hWnd, _T("CosmosInfo"));
		}
		if (pInfo)
			m_pHostObj = (CXobj*)pInfo->m_pXobj;
		if (m_pHostObj)
		{
			if (m_pHostObj->m_pHostParse)
			{
				if (m_pHostObj->m_pHostWnd->m_hWnd == m_hWnd)
				{
					CTangramXmlParse* pUIData = m_pHostObj->m_pHostParse->GetChild(_T("uidata"));
					if (pUIData)
					{

					}
				}
				else
				{
					int nID = ::GetDlgCtrlID(m_hWnd);
				}
			}
		}
		return S_OK;
	}
	break;
	}
	return lRes;
}

void CCosmosTabCtrl::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CTabCtrl::PostNcDestroy();
	delete this;
}

CMDIChild::CMDIChild(void)
{
	m_hClient = nullptr;
	m_strKey = _T("");
	m_pParent = nullptr;
}

CMDIChild::~CMDIChild(void)
{
}

LRESULT CMDIChild::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	if (::IsIconic(m_hWnd))
		::SendMessage(::GetParent(m_hWnd), WM_MDIICONARRANGE, 0, 0);
	return l;
}

LRESULT CMDIChild::OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	if (m_pParent->m_bDestroy)
		return l;
	if (m_pParent)
	{
		if (m_pParent->m_pHostBrowser)
			m_pParent->m_pHostBrowser->m_bSZMode = true;
		if (m_hWnd == (HWND)lParam)
		{
			::PostMessage(m_pParent->m_hWnd, WM_COSMOSMSG, (WPARAM)this, 20210202);
		}
	}
	return l;
}

LRESULT CMDIChild::OnCosmosDocObserved(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(ObserveComplete, 0);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CMDIChild::OnCosmosMg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	switch (lParam)
	{
	case 1965:
	{
		CGalaxy* pGalaxy = (CGalaxy*)wParam;
		if (pGalaxy)
		{
			::ShowWindow(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd, SW_SHOW);
			pGalaxy->HostPosChanged();
		}
	}
	break;
	case 10000:
		return (LRESULT)LPCTSTR(m_strDocTemplateKey);
		break;
	default:
		break;
	}
	return l;
}

void CMDIChild::OnFinalMessage(HWND hWnd)
{
	if (m_pParent)
	{
		auto it = m_pParent->m_mapMDIChild.find(hWnd);
		if (it != m_pParent->m_mapMDIChild.end())
			m_pParent->m_mapMDIChild.erase(it);
		if (m_pParent->m_mapMDIChild.size() == 0)
		{
			if (m_pParent->m_pHostBrowser)
				m_pParent->m_pHostBrowser->m_bSZMode = true;
			::SendMessage(m_pParent->m_hWnd, WM_COSMOSMSG, 0, 20210126);
		}
	}
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CMDTWnd::CMDTWnd(void)
{
}

CMDTWnd::~CMDTWnd(void)
{
}

LRESULT CMDTWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CMDTWnd* pHelperWnd = nullptr;
	auto it = g_pCosmos->m_mapMDTWindow.find(m_hWnd);
	if (it != g_pCosmos->m_mapMDTWindow.end())
	{
		for (auto& itX : g_pCosmos->m_mapMDTWindow)
		{
			if (itX.second->m_hWnd != m_hWnd)
			{
				pHelperWnd = itX.second;
				break;
			}
		}
	}
	if (pHelperWnd)
	{
		g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(QueryDestroy, pHelperWnd->m_hWnd);
		g_pCosmos->m_hMainWnd = pHelperWnd->m_hWnd;
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

LRESULT CMDTWnd::OnExitSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_bSZMode = false;
	if (m_pBrowser)
		m_pBrowser->m_bSZMode = false;
	for (auto& it : g_pCosmos->m_mapSizingBrowser)
	{
		if (::IsWindow(it.first))
		{
			it.second->m_bSZMode = false;
			it.second->m_pBrowser->LayoutBrowser();
		}
	}
	g_pCosmos->m_mapSizingBrowser.erase(g_pCosmos->m_mapSizingBrowser.begin(), g_pCosmos->m_mapSizingBrowser.end());

	CosmosFrameWndInfo* pCosmosFrameWndInfo = (CosmosFrameWndInfo*)::GetProp(m_hWnd, _T("CosmosFrameWndInfo"));
	if (pCosmosFrameWndInfo)
	{
		g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(QueryDestroy, pCosmosFrameWndInfo->m_hClient);
	}

	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMDTWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam)
	{
	case SC_SIZE:
		m_bSZMode = true;
		if (m_pBrowser)
			m_pBrowser->m_bSZMode = true;
		break;
	case SC_MOVE:
		m_bSZMode = false;
		if (m_pBrowser)
			m_pBrowser->m_bSZMode = false;
		break;
	}
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMDTWnd::OnEnterSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_bSZMode = true;
	if (m_pBrowser)
		m_pBrowser->m_bSZMode = true;
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMDTWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CMDTWnd* pHelperWnd = nullptr;
	if (g_pCosmos->m_mapMDTWindow.size() == 1)
	{
		::SendMessage(g_pCosmos->m_hHostBrowserWnd, WM_DESTROY, 0, 0);
	}
	else
	{
		auto it = g_pCosmos->m_mapMDTWindow.find(m_hWnd);
		if (it != g_pCosmos->m_mapMDTWindow.end())
		{
			g_pCosmos->m_mapMDTWindow.erase(it);
		}
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

LRESULT CMDTWnd::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(QueryType::RecalcLayout, m_hWnd);
	return l;
}

LRESULT CMDTWnd::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 10000:
		return (LRESULT)LPCTSTR(m_strDocTemplateKey);
		break;
	case 20210328:
	{
		if (m_bCreateNewDoc)
		{
			if (m_pBrowser && m_pBrowser->m_pParentXobj)
			{
				RECT rc;
				::GetClientRect(m_pBrowser->m_pParentXobj->m_pHostWnd->m_hWnd, &rc);
				::SetWindowPos(m_pBrowser->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOSENDCHANGING);
			}
			m_bCreateNewDoc = false;
		}
	}
	break;
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

void CMDTWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CMDIParent::CMDIParent(void)
{
	m_hMDIClient = nullptr;

	m_pGalaxy = nullptr;
	m_pHostBrowser = nullptr;
	m_pActiveMDIChild = nullptr;
	m_pCosmosFrameWndInfo = nullptr;
}

CMDIParent::~CMDIParent(void)
{
	OutputDebugString(_T("------------------Release CMDIParent------------------------\n"));
}

void CMDIParent::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CMDIParent::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pHostBrowser)
	{
		switch (wParam)
		{
		case SC_MOVE:
			m_pHostBrowser->m_bSZMode = false;
			break;
		case SC_SIZE:
			break;
		}
	}
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMDIParent::OnExitSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	if (m_pHostBrowser)
	{
		m_pHostBrowser->m_bSZMode = false;
		::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20210213);
	}

	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMDIParent::OnEnterSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	if (m_pHostBrowser)
		m_pHostBrowser->m_bSZMode = true;
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMDIParent::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	if (LOWORD(wParam) != WA_INACTIVE) {
		g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(QueryType::MainWnd, m_hWnd);
	}
	return l;
}

LRESULT CMDIParent::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_bDestroy = true;
	auto it = g_pCosmos->m_mapMDIParent.find(m_hWnd);
	if (it != g_pCosmos->m_mapMDIParent.end())
		g_pCosmos->m_mapMDIParent.erase(it);
	if (m_pHostBrowser)
		m_pHostBrowser->m_bDestroy = true;
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CMDIParent::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20210325:
	{
		if (m_bCreateNewDoc)
		{
			::SendMessage(m_hWnd, WM_COSMOSMSG, (WPARAM)m_pActiveMDIChild, 20210202);
		}
	}
	break;
	case 20210213:
	{
		if (m_bDestroy)
			break;
		switch (wParam)
		{
		case 0:
		{
			for (auto& it : g_pCosmos->m_mapSizingBrowser)
			{
				if (::IsWindow(it.first))
				{
					it.second->m_pBrowser->LayoutBrowser();
				}
			}
			::SendMessage(m_hWnd, WM_QUERYAPPPROXY, 0, 20210215);
		}
		break;
		case 1:
			::PostMessage(m_hWnd, WM_QUERYAPPPROXY, 0, 20210215);
			break;
		}
	}
	break;
	case 20210126:
	{
		if (m_bDestroy)
			break;
		CString strXml = _T("");
		if (m_pCosmosFrameWndInfo)
		{
			CComBSTR bstrKey("mainclient");
			for (auto& it : m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys)
			{
				CGalaxy* _pGalaxy = (CGalaxy*)it.second;
				CString strName = _pGalaxy->m_strGalaxyName;
				auto it = m_mapClientCtrlBarData.find(strName);
				if (it != m_mapClientCtrlBarData.end())
					strXml = it->second;
				IXobj* pXobj = nullptr;
				_pGalaxy->Observe(bstrKey, CComBSTR(strXml), &pXobj);
			}
		}

		if (m_pGalaxy && m_pGalaxy->m_pWebPageWnd)
		{
			m_pActiveMDIChild = nullptr;
			auto it = m_mapClientCtrlBarData.find(_T("hostpage"));
			if (it != m_mapClientCtrlBarData.end())
				strXml = it->second;
			m_pGalaxy->m_pWebPageWnd->LoadDocument2Viewport(_T("mainclient"), strXml);
		}
		if (m_pHostBrowser)
		{
			m_pHostBrowser->m_bSZMode = false;
			m_pHostBrowser->OpenURL(CComBSTR(g_pCosmos->m_strStartupURL), BrowserWndOpenDisposition::SWITCH_TO_TAB, CComBSTR(""), CComBSTR(""));
			g_pCosmos->m_mapSizingBrowser.erase(g_pCosmos->m_mapSizingBrowser.begin(), g_pCosmos->m_mapSizingBrowser.end());
		}
		::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20210213);
	}
	break;
	case 20210202:
	{
		if (m_bDestroy)
			break;

		if (wParam)
		{
			if (m_bCreateNewDoc == false)
				m_pActiveMDIChild = (CMDIChild*)wParam;
			if (m_pActiveMDIChild == nullptr)
				break;
			if (!::IsWindow(m_pActiveMDIChild->m_hWnd))
			{
				m_pActiveMDIChild = nullptr;
				break;
			}
			if (m_pCosmosFrameWndInfo)
			{
				if (m_pHostBrowser)
				{
					m_pHostBrowser->m_bInTabChange = false;
					m_pHostBrowser->m_bSZMode = false;
					BSTR bstrXml = ::SysAllocString(L"");
					CString strKey = m_pActiveMDIChild->m_strKey;
					CWebView* pVisiblePage = m_pHostBrowser->m_pVisibleWebView;
					if (pVisiblePage != m_pGalaxy->m_pWebPageWnd)
					{
						strKey = pVisiblePage->m_pGalaxy->m_strCurrentKey;
						CComBSTR bstrKey(strKey);
						pVisiblePage->LoadDocument2Viewport(strKey, _T(""));
						for (auto& it : m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys)
						{
							CGalaxy* _pGalaxy = (CGalaxy*)it.second;
							IXobj* pXobj = nullptr;
							if (_pGalaxy != m_pGalaxy)
							{
								_pGalaxy->Observe(bstrKey, bstrXml, &pXobj);
							}
						}
						::SysFreeString(bstrXml);
						break;
					}
					CComBSTR bstrKey(strKey);
					bool bNewKey = m_pGalaxy->m_strCurrentKey != strKey;
					for (auto& it : m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys)
					{
						CGalaxy* _pGalaxy = (CGalaxy*)it.second;
						IXobj* pXobj = nullptr;
						if (m_bCreateNewDoc || _pGalaxy != m_pGalaxy || bNewKey)
						{
							if (it.first == 10000)
							{
								CString strXml = _T("");
								auto it = g_pCosmos->m_mapDocTemplate.find(strKey);
								if (it != g_pCosmos->m_mapDocTemplate.end())
								{
									strXml = it->second;
									CTangramXmlParse m_Parse;
									m_Parse.LoadXml(strXml);
									CTangramXmlParse* pClient = m_Parse.GetChild(_T("mdiclient"));
									IXobj* _pXobj = nullptr;
									m_pGalaxy->Observe(CComBSTR(strKey), CComBSTR(pClient->xml()), &_pXobj);
									pVisiblePage->m_bCanShow = true;
								}
							}
							else if (_pGalaxy->m_strCurrentKey != strKey)
							{
								_pGalaxy->Observe(bstrKey, bstrXml, &pXobj);
							}
						}
					}
					::SysFreeString(bstrXml);
					if (pVisiblePage->m_pGalaxy->m_strCurrentKey != strKey)
						pVisiblePage->LoadDocument2Viewport(strKey, _T(""));
					CXobj* pClientObj = m_pGalaxy->m_pWorkXobj->GetVisibleChildByName(_T("mdiclient"));
					if (pClientObj)
						m_pGalaxy->m_pBindingXobj = pClientObj;
					if (m_bCreateNewDoc)
					{
						m_pHostBrowser->m_bSZMode = true;
						m_pHostBrowser->m_bTabChange = false;
						m_bCreateNewDoc = false;
						theApp.m_bAppStarting = false;

						if (m_pHostBrowser->m_pParentXobj)
						{
							if (!::IsChild(m_pHostBrowser->m_pParentXobj->m_pHostWnd->m_hWnd, m_pHostBrowser->m_hWnd))
							{
								RECT rc;
								::GetClientRect(m_pHostBrowser->m_pParentXobj->m_pHostWnd->m_hWnd, &rc);
								::SetParent(m_pHostBrowser->m_hWnd, m_pHostBrowser->m_pParentXobj->m_pHostWnd->m_hWnd);
								::SetWindowPos(m_pHostBrowser->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_NOACTIVATE | SWP_NOREDRAW | SWP_SHOWWINDOW | SWP_NOSENDCHANGING);
							}
						}
						::PostMessage(m_pHostBrowser->m_hWnd, WM_BROWSERLAYOUT, 1, 7);
					}
				}
			}
		}
		else if (m_bCreateNewDoc)
		{
			RECT rc;
			::GetClientRect(m_pHostBrowser->m_hWnd, &rc);
			if ((rc.right < rc.left) || (rc.bottom < rc.top))
			{
				::GetClientRect(::GetParent(m_pHostBrowser->m_hWnd), &rc);
				::SetWindowPos(m_pHostBrowser->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
			}
			m_bCreateNewDoc = false;
			m_pHostBrowser->m_bSZMode = true;
		}
	}
	break;
	default:
		break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

CWinForm::CWinForm(void)
{
	m_nState = -1;
	m_bMdiForm = false;
	m_pBKWnd = nullptr;
	m_pWormhole = nullptr;
	m_pOwnerHtmlWnd = nullptr;
	m_strChildFormPath = m_strXml = m_strKey = m_strBKID = _T("");
	if (g_pCosmos->m_pCurMDIChildFormInfo)
	{
		m_pChildFormsInfo = g_pCosmos->m_pCurMDIChildFormInfo;
		if (g_pCosmos->m_pActiveXobj)
			g_pCosmos->m_pActiveXobj->m_pChildFormsInfo = m_pChildFormsInfo;
		g_pCosmos->m_pCurMDIChildFormInfo = nullptr;
	}
	else
	{
		m_pChildFormsInfo = nullptr;
	}
}

CWinForm::~CWinForm(void)
{
	if (m_pChildFormsInfo)
		delete m_pChildFormsInfo;
}

void CWinForm::SendMessage()
{
	if (m_pWormhole == nullptr)
	{
		m_pWormhole = (CWormhole*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
	}
	if (m_pWormhole)
	{
		m_pWormhole->SendMessage();
	}
}

LRESULT CWinForm::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	g_pCosmos->m_pActiveWinFormWnd = this;
	return lRes;
}

LRESULT CWinForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pCosmos->m_mapBrowserWnd.size())
	{
		CSession* pSession = (CSession*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		if (pSession)
		{
			int nCount = 0;
			if (m_bMainForm)
			{
				nCount = g_pCosmos->m_mapNeedQueryOnClose.size();
				if (nCount > 1)
				{
					pSession->InsertLong(_T("FormAppNeedClosed"), nCount);
				}
				for (auto it : g_pCosmos->m_mapNeedQueryOnClose)
				{
					if (it.second != this)
					{

					}
				}
			}
			bool bQueryonclose = false;
			long nQueryOnClose = pSession->GetLong(_T("queryonclose"));
			bQueryonclose = nQueryOnClose ? true : false;
			pSession->InsertString(_T("msgID"), _T("WINFORM_ONCLOSE"));
			pSession->SendMessage();
			if (bQueryonclose)
			{
				MSG msg;
				while (pSession->Getint64(_T("CloseInfo")) != 19921963)
				{
					GetMessage(&msg, NULL, 0, 0);
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					if (pSession->Getint64(_T("CloseInfo")) == 19921965)
					{
						pSession->Insertint64(_T("CloseInfo"), 0);
						return 1;
						break;
					}
					else if (pSession->Getint64(_T("CloseInfo")) == 19921963)
					{
						break;
					}
				}
			}
		}
	}
	if (g_pCosmos->m_pActiveWinFormWnd == this)
		g_pCosmos->m_pActiveWinFormWnd = nullptr;
	auto it = g_pCosmos->m_mapNeedQueryOnClose.find(m_hWnd);
	if (it != g_pCosmos->m_mapNeedQueryOnClose.end())
		g_pCosmos->m_mapNeedQueryOnClose.erase(it);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnGetMe(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 0:
	{
		m_nState = 0;//new
		m_strPath = (LPCTSTR)wParam;
		m_strPath.MakeLower();
		return (LRESULT)this;
	}
	break;
	case 1:
	{
		m_nState = 1;//design
		m_strPath = (LPCTSTR)wParam;
		m_strPath.MakeLower();
		if (m_bMdiForm)
		{
			int nPos = m_strPath.ReverseFind('.');
			if (nPos != -1)
			{
				CString strDir = m_strPath.Left(nPos) + _T("\\");
				m_strChildFormPath = strDir;
				if (::PathIsDirectory(strDir) == false)
				{
					::SHCreateDirectory(nullptr, strDir);
				}
			}
		}
	}
	break;
	case 2:
	{
		if (m_strKey != _T(""))
		{
			CWinForm* pParent = nullptr;
			CXobj* pTopObj = nullptr;
			if (m_pOwnerHtmlWnd && m_pOwnerHtmlWnd->m_pGalaxy)
			{
				CBrowser* pBrowser = nullptr;
				auto it = g_pCosmos->m_mapBrowserWnd.find(::GetParent(m_pOwnerHtmlWnd->m_hWnd));
				if (it != g_pCosmos->m_mapBrowserWnd.end())
				{
					pBrowser = (CBrowser*)it->second;
					pBrowser->m_bSZMode = true;
				}
				m_pOwnerHtmlWnd->LoadDocument2Viewport(m_strKey, _T(""));
				pParent = m_pOwnerHtmlWnd->m_pGalaxy->m_pParentMDIWinForm;
				if (pParent)
				{
					CString strOldKey = _T("");
					strOldKey = pParent->m_pClientGalaxy->m_strCurrentKey;
					if (strOldKey != m_strKey)
					{
						HWND hClient = ::GetParent(m_hWnd);
						IXobj* pObj = nullptr;
						pParent->m_pClientGalaxy->Observe(CComBSTR(m_strKey), CComBSTR(""), &pObj);
						pTopObj = (CXobj*)pObj;
						pParent->m_pClientGalaxy->m_pBindingXobj = pTopObj->GetVisibleChildByName(_T("mdiclient"));
						pParent->m_mapMDIClientXobj[m_strKey] = pParent->m_pClientGalaxy->m_pBindingXobj;
					}
				}
			}
			::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20200216);
		}
		return (LRESULT)m_strKey.GetBuffer();
	}
	break;
	case 3:
	{
		m_nState = 3;//design
		m_strXml = (LPCTSTR)wParam;
		if (::PathFileExists(m_strXml))
		{
			m_strPath = m_strXml;
		}
		else
		{
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(m_strXml))
			{
				m_strKey = m_Parse.name();
				if (m_pOwnerHtmlWnd)
				{
					CTangramXmlParse* pChild = m_Parse.GetChild(_T("webui"));
					if (pChild)
					{
						IXobj* pXobj = nullptr;
						m_pOwnerHtmlWnd->Observe(CComBSTR(m_strKey), CComBSTR(pChild->xml()), &pXobj);
					}
				}
			}
		}
	}
	break;
	case 4:
	{
		m_bMdiForm = true;//design
		if (m_bMdiForm)
		{
			//int nPos = m_strPath.ReverseFind('.');
			//if (nPos != -1)
			//{
			//	CString strDir = m_strPath.Left(nPos) + _T("\\");
			//	m_strChildFormPath = strDir;
			//	if (::PathIsDirectory(strDir) == false)
			//	{
			//		::SHCreateDirectory(nullptr, strDir);
			//	}
			//}
		}
	}
	break;
	case 20190214:
		return (LRESULT)this;
		break;
	case 20201029:
	{
		if (wParam == 0)
			m_bMainForm = true;
		else
		{
			g_pCosmos->m_mapNeedQueryOnClose[m_hWnd] = this;
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam)
	{
	case SC_MOVE:
		m_bSZMode = false;
		break;
	case SC_SIZE:
		break;
	}
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CWinForm::OnExitSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_bSZMode = false;
	for (auto& it : g_pCosmos->m_mapSizingBrowser)
	{
		if (::IsWindow(it.first) && it.second)
		{
			it.second->m_bSZMode = false;
			it.second->m_pBrowser->LayoutBrowser();
		}
	}
	g_pCosmos->m_mapSizingBrowser.erase(g_pCosmos->m_mapSizingBrowser.begin(), g_pCosmos->m_mapSizingBrowser.end());
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CWinForm::OnEnterSZ(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_bSZMode = true;
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CWinForm::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_hWnd != g_pCosmos->m_hMainWnd)
	{
		if (::IsWindow(m_hOwnerWebView) && m_pOwnerHtmlWnd)
		{
			auto it = m_pOwnerHtmlWnd->m_mapWinForm.find(m_hWnd);
			if (it != m_pOwnerHtmlWnd->m_mapWinForm.end())
			{
				m_pOwnerHtmlWnd->m_mapWinForm.erase(it);
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20210415:
	{
		if (m_pOwnerHtmlWnd)
		{
			HWND hBrowser = m_pOwnerHtmlWnd->m_pChromeRenderFrameHost->GetHostBrowserWnd();
			auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
			if (it != g_pCosmos->m_mapBrowserWnd.end())
			{
				it->second->OpenURL(CComBSTR(m_pOwnerHtmlWnd->m_pChromeRenderFrameHost->GetRenderFrameURL(2)), BrowserWndOpenDisposition::SWITCH_TO_TAB, CComBSTR(""), CComBSTR(""));
			}
		}
	}
	break;
	case 20210331:
	{
		HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
		CosmosInfo* pInfo = (CosmosInfo*)::GetProp(::GetParent(m_hWnd), _T("CosmosInfo"));
		if (pInfo)
		{
			::InvalidateRect(::GetParent(m_hWnd), nullptr, false);
		}
	}
	break;
	case 20201114:
	{
		if (m_bReady)
			return DefWindowProc(uMsg, wParam, lParam);
		m_bReady = true;
		CSession* pSession = (CSession*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		HWND hWnd = ::GetParent(m_hWnd);
		DWORD dwID = ::GetWindowThreadProcessId(hWnd, NULL);
		CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);

		CGalaxy* pGalaxy = nullptr;
		auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
		if (iter != pThreadInfo->m_mapGalaxy.end())
		{
			pGalaxy = (CGalaxy*)iter->second;
		}
		if (pGalaxy->m_pHostWebBrowserWnd)
		{
			HWND hWnd = pGalaxy->m_pHostWebBrowserWnd->m_pBrowser->GetActiveWebContentWnd();
			auto it = g_pCosmos->m_mapWebView.find(hWnd);
			if (it != g_pCosmos->m_mapWebView.end())
			{
				pSession->InsertString(_T("msgID"), _T("MdiWinForm_Ready"));
				pSession->Insertint64(_T("ready_mdichildhandle"), (__int64)m_hWnd);
				pSession->InsertString(_T("mdichildkey"), m_strKey);
				pSession->SendMessage();
			}
		}
	}
	break;
	case 20200216:
	{
		CSession* pSession = (CSession*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		if (pSession)
		{
			if (m_bMdiForm)
			{
				//if (m_pOwnerHtmlWnd)
				//{
				//	ATLTRACE(_T("\n"));
				//	CString strHandle = _T("");
				//	strHandle.Format(_T("%d"), m_hWnd);
				//	m_pOwnerHtmlWnd->SendChromeIPCMessage(_T("MESSAGE"), m_strKey, strHandle, _T("MainMdiForm:ActiveClient"), m_strKey, L"");
				//}
			}
			else
			{
				HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
				::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
				HWND hWnd = ::GetParent(m_hWnd);
				DWORD dwID = ::GetWindowThreadProcessId(hWnd, NULL);
				CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);

				CGalaxy* pGalaxy = nullptr;
				auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
				if (iter != pThreadInfo->m_mapGalaxy.end())
				{
					pGalaxy = (CGalaxy*)iter->second;
					if (pGalaxy->m_pParentMDIWinForm && pGalaxy->m_pParentMDIWinForm->m_pClientGalaxy)
					{
						if (pGalaxy->m_pHostWebBrowserWnd)
						{
							pGalaxy->m_pHostWebBrowserWnd->m_pBrowser->LayoutBrowser();
						}
					}
				}
				if (pGalaxy->m_pHostWebBrowserWnd)
				{
					HWND hWnd = pGalaxy->m_pHostWebBrowserWnd->m_pBrowser->GetActiveWebContentWnd();
					auto it = g_pCosmos->m_mapWebView.find(hWnd);
					if (it != g_pCosmos->m_mapWebView.end())
					{
						if (m_pOwnerHtmlWnd == nullptr)
							m_pOwnerHtmlWnd = (CWebView*)it->second;
						pSession->InsertString(_T("msgID"), _T("MdiWinForm_ActiveMdiChild"));
						pSession->Insertint64(_T("active_mdichildhandle"), (__int64)m_hWnd);
						pSession->InsertString(_T("active_mdichildkey"), m_strKey);
						pSession->SendMessage();
						if (!m_bReady)
						{
							::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20201114);
						}
						if (m_pOwnerHtmlWnd)
							m_pOwnerHtmlWnd->m_pChromeRenderFrameHost->ShowWebPage(true);
					}
				}
			}
		}
		return 0;
	}
	break;
	case 20200130:
	{
		HWND hWnd = (HWND)wParam;
		IXobj* pXobj = nullptr;
		g_pCosmos->GetXobjFromHandle((__int64)m_hWnd, &pXobj);
		if (pXobj)
		{
			IGalaxy* pGalaxy = nullptr;
			pXobj->get_Galaxy(&pGalaxy);
			if (pGalaxy)
			{
				CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
				IWebPage* pWebPage = nullptr;
				if (_pGalaxy->m_pWebPageWnd)
				{
					_pGalaxy->m_pWebPageWnd->m_hWebHostWnd = hWnd;
					CWebHelperWnd* pWnd = new CWebHelperWnd();
					pWnd->SubclassWindow(hWnd);
					pWnd->m_hWebHost = _pGalaxy->m_pWebPageWnd->m_hWnd;
				}
			}
		}
	}
	break;
	case 20190601:
	{
		if (m_strKey == _T(""))
		{
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(m_strXml))
			{
				m_strKey = m_Parse.name();
			}
		}
		CTangramXmlParse* pChild = (CTangramXmlParse*)wParam;
		if (pChild)
		{
			int nCount = pChild->GetCount();
		}
	}
	break;
	case 20190602:
	{
		return (LRESULT)m_pChildFormsInfo;
	}
	break;
	case 20200115:
	{
		if (m_pBKWnd)
		{
			m_pBKWnd->m_pGalaxy->HostPosChanged();
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnCosmosGetXml(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strGalaxyName = (LPCTSTR)wParam;
	CString currentKey = (LPCTSTR)lParam;
	CString strIndex = strGalaxyName + L"_" + currentKey;
	if (m_bMdiForm)
	{
		//auto it = m_mapKey.find(currentKey);
		//if (it != m_mapKey.end())
		//{
		//	CString strXml = it->second;
		//	CTangramXmlParse parse;
		//	if (parse.LoadXml(strXml))
		//	{
		//		CTangramXmlParse* pParse = parse.GetChild(strGalaxyName);
		//		if (pParse)
		//		{
		//			CTangramXmlParse* pParse2 = pParse->GetChild(currentKey);
		//			if (pParse2)
		//			{
		//				CString s = pParse2->xml();
		//				//LRESULT res = (LRESULT)LPSTR(LPCTSTR(s));
		//				auto it = g_pCosmos->m_mapValInfo.find(strIndex);
		//				if (it != g_pCosmos->m_mapValInfo.end())
		//				{
		//					g_pCosmos->m_mapValInfo.erase(it);
		//				}
		//				g_pCosmos->m_mapValInfo[strIndex] = CComVariant(s);
		//				return 1;
		//			}
		//		}
		//	}
		//}
	}
	CTangramXmlParse parse;
	if (parse.LoadXml(m_strXml) || parse.LoadFile(m_strXml))
	{
		CTangramXmlParse* pParse = parse.GetChild(strGalaxyName);
		if (pParse)
		{
			CTangramXmlParse* pParse2 = pParse->GetChild(currentKey);
			if (pParse2)
			{
				auto it = g_pCosmos->m_mapValInfo.find(strIndex);
				if (it != g_pCosmos->m_mapValInfo.end())
				{
					g_pCosmos->m_mapValInfo.erase(it);
				}
				g_pCosmos->m_mapValInfo[strIndex] = CComVariant(pParse2->xml());
				return 1;
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	DWORD dwY = HIWORD(wParam);
	DWORD dwX = LOWORD(wParam);
	RECT* const prcNewWindow = (RECT*)lParam;
	float fScale = (float)dwX / USER_DEFAULT_SCREEN_DPI;

	//::SetWindowPos(m_hWnd,
	//	NULL,
	//	prcNewWindow->left,
	//	prcNewWindow->top,
	//	prcNewWindow->right - prcNewWindow->left,
	//	prcNewWindow->bottom - prcNewWindow->top,
	//	SWP_NOZORDER | SWP_NOACTIVATE);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnGetDPIScaledSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	DWORD dwY = HIWORD(wParam);
	DWORD dwX = LOWORD(wParam);
	RECT* const prcNewWindow = (RECT*)lParam;
	//::SetWindowPos(m_hWnd,
	//	NULL,
	//	prcNewWindow->left,
	//	prcNewWindow->top,
	//	prcNewWindow->right - prcNewWindow->left,
	//	prcNewWindow->bottom - prcNewWindow->top,
	//	SWP_NOZORDER | SWP_NOACTIVATE);
	return  false;//DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnMdiChildMin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
	{
		::PostMessage(::GetParent(m_hWnd), WM_COSMOSMSG, 0, 20180115);
	}
	return  DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_hWnd == (HWND)lParam)
	{
		HWND hPWnd = ::GetParent(m_hWnd);
		HWND hPPWnd = ::GetParent(hPWnd);
		if (hPPWnd)
		{
			CWinForm* pWnd = (CWinForm*)::SendMessage(hPPWnd, WM_HUBBLE_DATA, 0, 20190214);
			if (pWnd && pWnd->m_bMdiForm)
			{
				auto it = pWnd->m_mapMDIClientXobj.find(m_strKey);
				if (it != pWnd->m_mapMDIClientXobj.end())
				{
					pWnd->m_pClientGalaxy->m_pBindingXobj = it->second;
				}
			}
		}
	}
	return  DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_pActiveHtmlWnd = nullptr;
	g_pCosmos->m_pActiveWinFormWnd = this;
	::BringWindowToTop(m_hWnd);
	HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
	::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return  DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnMdiClientCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strID = (LPCTSTR)lParam;
	if (m_strBKID == _T(""))
		m_strBKID = strID;
	if (lParam)
	{
		::PostMessage(m_hWnd, WM_MDICLIENTCREATED, wParam, 0);
	}
	if (lParam == 0 && m_pBKWnd == nullptr && m_strBKID != _T(""))
	{
		m_pBKWnd = new CBKWnd();
		HWND hwnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T("mdibk"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, (HWND)wParam, 0, theApp.m_hInstance, nullptr);
		m_pBKWnd->SubclassWindow(hwnd);
		m_pBKWnd->m_hChild = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, hwnd, 0, theApp.m_hInstance, nullptr);
		CGalaxyCluster* pGalaxyCluster = nullptr;
		auto it = g_pCosmos->m_mapWindowPage.find(m_hWnd);
		if (it != g_pCosmos->m_mapWindowPage.end())
			pGalaxyCluster = (CGalaxyCluster*)it->second;
		if (pGalaxyCluster)
		{
			IGalaxy* pGalaxy = nullptr;
			pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)m_pBKWnd->m_hChild), CComBSTR(L"ClientFrame"), &pGalaxy);
			CString strXml = _T("");
			int nPos = m_strBKID.Find(_T(":"));
			if (nPos == -1)
				strXml.Format(_T("<mdiclient><cluster><xobj name=\"mdiclient\" gridtype=\"clrctrl\" objid=\"%s\" /></cluster></mdiclient>"), m_strBKID);
			else
			{
				m_strBKID = m_strBKID.Mid(nPos + 1);
				strXml.Format(_T("<mdiclient><cluster><xobj name='mdiclient' gridtype='' url='%s' /></cluster></mdiclient>"), m_strBKID);
			}
			IXobj* pXobj = nullptr;
			pGalaxy->Observe(CComBSTR(L"default"), strXml.AllocSysString(), &pXobj);
			auto it = pGalaxyCluster->m_mapGalaxy.find((HWND)wParam);
			if (it != pGalaxyCluster->m_mapGalaxy.end())
			{
				it->second->m_pBKWnd = m_pBKWnd;
			}
			pGalaxyCluster->m_pBKGalaxy = m_pBKWnd->m_pGalaxy = (CGalaxy*)pGalaxy;
			//g_pCosmos->m_mapBKFrame[m_pBKWnd->m_hChild] = m_pBKWnd->m_pGalaxy;
		}
	}

	return  DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	WINDOWPLACEMENT wndPlacement;
	GetWindowPlacement(&wndPlacement);
	if (wndPlacement.showCmd == SW_RESTORE)
	{
		::PostMessage(m_hWnd, WM_COSMOSMSG, 1, 20200115);
	};
	if (g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_pCLRProxy->PreWindowPosChanging(m_hWnd, (WINDOWPOS*)lParam, 1);
	}
	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnFormCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_mapWinForm[m_hWnd] = this;
	g_pCosmos->m_hFormNodeWnd = nullptr;
	if (m_pOwnerHtmlWnd == nullptr)
	{
		if (::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)
		{
			HWND hPWnd = ::GetParent(::GetParent(m_hWnd));
			auto it = g_pCosmos->m_mapWinForm.find(hPWnd);
			if (it != g_pCosmos->m_mapWinForm.end())
			{
				CWinForm* pMdiParent = it->second;
				m_pOwnerHtmlWnd = it->second->m_pOwnerHtmlWnd;
			}
		}
	}
	if (m_pOwnerHtmlWnd)
	{
		m_hOwnerWebView = m_pOwnerHtmlWnd->m_hWnd;
		HWND hBrowser = ::GetParent(m_pOwnerHtmlWnd->m_hWnd);
		if (::IsChild(m_hWnd, hBrowser))
		{
			auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
			if (it != g_pCosmos->m_mapBrowserWnd.end())
			{
				CBrowser* pBrowser = (CBrowser*)it->second;
				if (pBrowser->m_pParentXobj)
				{
					pBrowser->m_pParentXobj->m_pXobjShareData->m_pGalaxy->m_pParentWinForm = this;
					if (m_bMdiForm)
						pBrowser->m_pParentXobj->m_pXobjShareData->m_pGalaxy->m_pParentMDIWinForm = this;
				}
				pBrowser->m_pVisibleWebView->m_bCanShow = false;
				if (m_hWnd != g_pCosmos->m_hMainWnd)
				{
					pBrowser->m_bSZMode = true;
					g_pCosmos->m_mapSizingBrowser[hBrowser] = pBrowser;
				}
				pBrowser->m_pBrowser->LayoutBrowser();
				pBrowser->BrowserLayout();
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

void CWinForm::OnFinalMessage(HWND hWnd)
{
	auto it = g_pCosmos->m_mapFormWebPage.find(hWnd);
	if (it != g_pCosmos->m_mapFormWebPage.end())
		g_pCosmos->m_mapFormWebPage.erase(it);
	auto it2 = g_pCosmos->m_mapWinForm.find(hWnd);
	if (it2 != g_pCosmos->m_mapWinForm.end())
		g_pCosmos->m_mapWinForm.erase(it2);
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CCosmosWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SHOWWINDOW:
	{
		if (m_hClient)
		{
			::PostMessage(m_hClient, WM_COSMOSMSG, 1, 20180115);
		}
	}
	break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// CGalaxy
CGalaxy::CGalaxy()
{
	m_pCurrentIPCMsg = nullptr;
	m_strLastKey = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentXml = _T("");
	m_strGalaxyName = _T("");
	m_bMDIChild = false;
	m_bDetached = false;
	m_pWebPageWnd = nullptr;
	m_pWorkBenchFrame = nullptr;
	m_bTabbedMDIClient = false;
	m_pBKWnd = nullptr;
	m_pGalaxyCluster = nullptr;
	m_pWorkXobj = nullptr;
	m_pRootNodes = nullptr;
	m_pBindingXobj = nullptr;
	m_pContainerNode = nullptr;
	m_pGalaxyInfo = nullptr;
	m_pParentXobj = nullptr;
	m_nGalaxyType = NOGalaxy;
	m_pParentMDIWinForm = nullptr;
#ifdef _DEBUG
	g_pCosmos->m_nTangramFrame++;
#endif
}

CGalaxy::~CGalaxy()
{
#ifdef _DEBUG
	g_pCosmos->m_nTangramFrame--;
#endif	
	//if (m_pGalaxyInfo)
	//	delete m_pGalaxyInfo;
	if (g_pCosmos->m_pGalaxy == this)
		g_pCosmos->m_pGalaxy = nullptr;
	for (auto it : g_pCosmos->m_mapThreadInfo)
	{
		if (it.second)
		{
			auto it2 = it.second->m_mapGalaxy.find(m_hHostWnd);
			if (it2 != it.second->m_mapGalaxy.end())
			{
				it.second->m_mapGalaxy.erase(it2);
				break;
			}
		}
	}
	if (m_pRootNodes)
		CCommonFunction::ClearObject<CXobjCollection>(m_pRootNodes);
	if (m_pGalaxyCluster) {
		auto it = m_pGalaxyCluster->m_mapGalaxy.find(m_hHostWnd);
		if (it != m_pGalaxyCluster->m_mapGalaxy.end()) {
			m_pGalaxyCluster->m_mapGalaxy.erase(it);
			if (m_pGalaxyCluster->m_mapGalaxy.size() == 0)
				delete m_pGalaxyCluster;
		}
	}
	for (auto it : m_mapGalaxyProxy)
	{
		if (it.second->m_bAutoDelete)
			delete it.second;
	}
	m_mapGalaxyProxy.clear();
	m_hWnd = NULL;
}

void CGalaxy::HostPosChanged()
{
	if (::IsWindow(m_hWnd) == false || m_pWorkXobj == nullptr || m_pWorkXobj->m_pHostWnd == nullptr)
		return;

	HWND hPWnd = ::GetParent(m_hWnd);
	if (::IsWindow(m_pWorkXobj->m_pHostWnd->m_hWnd))
	{
		RECT rt1;
		m_pWorkXobj->m_pHostWnd->GetWindowRect(&rt1);

		::ScreenToClient(hPWnd, (LPPOINT)&rt1);
		::ScreenToClient(hPWnd, ((LPPOINT)&rt1) + 1);

		HDWP dwh = BeginDeferWindowPos(1);
		UINT flag = SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_SHOWWINDOW;
		//CWnd* _pWnd = CWnd::FromHandlePermanent(hPWnd);

		if (m_bNoRedrawState == false)
		{
			HWND hRoot = ::GetAncestor(m_hWnd, GA_ROOT);
			::GetClassName(hRoot, g_pCosmos->m_szBuffer, MAX_PATH);
			CString strClassName = g_pCosmos->m_szBuffer;
			if (strClassName.Find(_T("Afx:MiniFrame:")) == 0)
				::PostMessage(m_hWnd, WM_COSMOSMSG, 1, 20210228);
			else
			{
				m_bNoRedrawState = true;
				flag |= SWP_NOREDRAW;
			}
		}
		else
			flag |= SWP_NOREDRAW;

		if (m_bTabbedMDIClient || m_bObserveState)
			flag &= ~SWP_NOREDRAW;
		dwh = ::DeferWindowPos(dwh, m_hWnd, HWND_TOP,
			rt1.left,
			rt1.top,
			rt1.right - rt1.left,
			rt1.bottom - rt1.top,
			flag
		);
		EndDeferWindowPos(dwh);
		//UpdateVisualWPFMap(hPWnd, false);
		//if (m_pBKWnd && ::IsWindow(m_pBKWnd->m_hWnd))
		//{
		//	::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 0, 0, rt1.right - rt1.left, rt1.bottom - rt1.top, SWP_NOACTIVATE | SWP_NOREDRAW);
		//}
		if (m_bObserveState)
		{
			m_bObserveState = false;
		}
	}
}

CTangramXmlParse* CGalaxy::UpdateXobj()
{
	for (auto it : m_mapXobj) {
		CXobj* pWndXobj = (CXobj*)it.second;
		if (pWndXobj)
		{
			if (pWndXobj->m_pWindow) {
				if (pWndXobj->m_nActivePage > 0) {
					CString strVal = _T("");
					strVal.Format(_T("%d"), pWndXobj->m_nActivePage);
					pWndXobj->m_pHostParse->put_attr(_T("activepage"), strVal);
				}
				pWndXobj->m_pWindow->Save();
			}
			if (pWndXobj->m_nViewType == Grid)
			{
				((CGridWnd*)pWndXobj->m_pHostWnd)->Save();
			}

			for (auto it2 : pWndXobj->m_vChildNodes) {
				g_pCosmos->UpdateXobj(it2);
			}

			if (pWndXobj == pWndXobj->m_pRootObj && pWndXobj->m_pXobjShareData->m_pOfficeObj) {
				g_pCosmos->UpdateOfficeObj(pWndXobj->m_pXobjShareData->m_pOfficeObj, pWndXobj->m_pXobjShareData->m_pCosmosParse->GetChild(TGM_CLUSTER)->xml(), pWndXobj->m_pXobjShareData->m_pCosmosParse->name());
			}
		}
	}
	if (m_mapXobj.size())
		return m_mapXobj.begin()->second->m_pXobjShareData->m_pCosmosParse;
	return nullptr;
}

HWND CGalaxy::GetWinForm(HWND hForm)
{
	if (g_pCosmos->m_pCLRProxy)
	{
		int nForm = g_pCosmos->m_pCLRProxy->IsWinForm(hForm);
		while (nForm >= 1)
		{
			m_pWorkXobj->m_pParentWinFormWnd = (CWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
			LRESULT lRes = ::SendMessage(hForm, WM_HUBBLE_GETNODE, 0, 0);
			if (lRes)
			{
				CXobj* pXobj = (CXobj*)lRes;
				hForm = pXobj->m_pRootObj->m_pXobjShareData->m_pGalaxyCluster->m_hWnd;
				nForm = g_pCosmos->m_pCLRProxy->IsWinForm(hForm);
			}
			else
			{
				break;
			}
		}
		if (nForm == 0)
		{
			hForm = ::GetParent(hForm);
			while (g_pCosmos->m_pCLRProxy->IsWinForm(hForm) == 0)
			{
				hForm = ::GetParent(hForm);
				if (hForm == NULL)
				{
					return hForm;
					break;
				}
			}
			hForm = GetWinForm(hForm);
		}
	}
	return hForm;
}

CXobj* CGalaxy::ObserveXtmlDocument(CTangramXmlParse* _pParse, CString strKey)
{
	m_pWorkXobj = new CComObject<CXobj>;
	m_pWorkXobj->m_pRootObj = m_pWorkXobj;
	CXobjShareData* pCommonData = new CXobjShareData();
	m_pWorkXobj->m_pXobjShareData = pCommonData;
	pCommonData->m_pGalaxy = this;
	pCommonData->m_pGalaxyCluster = m_pGalaxyCluster;
	pCommonData->m_pCosmosParse = _pParse;
	CTangramXmlParse* pParse = _pParse->GetChild(TGM_CLUSTER);
	m_pWorkXobj->m_pHostParse = pParse->GetChild(TGM_XOBJ);
	for (auto it : m_mapXobj)
	{
		::ShowWindow(it.second->m_pHostWnd->m_hWnd, SW_HIDE);
	}
	Create();
	m_mapXobj[strKey] = m_pWorkXobj;
	if (strKey.CompareNoCase(_T("default")) == 0)
	{
		::SetProp(m_hWnd, _T("CosmosData"), _pParse);
		::SendMessage(m_hWnd, WM_XOBJCREATED, 0, 10000);
	}

	m_pWorkXobj->m_strCosmosXml = _pParse->xml();
	if (m_pGalaxyCluster)
		m_pGalaxyCluster->Fire_OpenXmlComplete(CComBSTR(m_pWorkXobj->m_strCosmosXml), (long)m_hHostWnd, m_pWorkXobj);
	m_pWorkXobj->m_strKey = strKey;
	m_pWorkXobj->Fire_ObserveComplete();
	if (g_pCosmos->m_pCLRProxy)
	{
		CGalaxyCluster* pGalaxyCluster = m_pGalaxyCluster;
		HWND hForm = pGalaxyCluster->m_hWnd;
		hForm = GetWinForm(hForm);
		if (hForm)
			m_pWorkXobj->m_pParentWinFormWnd = (CWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
	}
	if (g_pCosmos->m_pUniverseAppProxy && m_nGalaxyType != CtrlBarGalaxy)
	{
		//CString strCaption = _pParse->attr(_T("caption"), _T(""));
		//if (strCaption != _T(""))
		//{
		//	g_pCosmos->m_pUniverseAppProxy->SetFrameCaption(m_hWnd, strCaption);
		//}
		::PostMessage(::GetParent(m_hWnd), WM_COSMOSOBSERVED, 0, 0);
	}

	return m_pWorkXobj;
}

BOOL CGalaxy::Create()
{
	if (::IsWindow(m_hWnd) == false)
	{
		SubclassWindow(m_hHostWnd);
	}

	m_pWorkXobj->InitWndXobj();
	if (m_pWorkXobj->m_pObjClsInfo) {
		RECT rc;
		HWND hPWnd = ::GetParent(m_hWnd);
		CWnd* pParentWnd = CWnd::FromHandle(hPWnd);
		m_pWorkXobj->m_pRootObj = m_pWorkXobj;
		CCreateContext	m_Context;
		m_Context.m_pNewViewClass = m_pWorkXobj->m_pObjClsInfo;
		CWnd* pWnd = (CWnd*)m_pWorkXobj->m_pObjClsInfo->CreateObject();
		GetWindowRect(&rc);
		if (pParentWnd)
			pParentWnd->ScreenToClient(&rc);

		pWnd->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rc, pParentWnd, 0, &m_Context);
		pWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		::SetWindowPos(pWnd->m_hWnd, HWND_BOTTOM, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_NOACTIVATE);//|SWP_NOREDRAWSWP_NOZORDER);
	}

	CTangramXmlParse* pWndParse = m_pWorkXobj->m_pXobjShareData->m_pCosmosParse->GetChild(_T("docplugin"));
	if (pWndParse)
	{
		CString strPlugID = _T("");
		HRESULT hr = S_OK;
		BOOL bHavePlugin = false;
		int nCount = pWndParse->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CTangramXmlParse* pChild = pWndParse->GetChild(i);
			CComBSTR bstrXml(pChild->xml());
			strPlugID = pChild->text();
			strPlugID.Trim();
			strPlugID.MakeLower();
			if (strPlugID != _T(""))
			{
				int nPos = strPlugID.Find(_T(","));
				CComBSTR bstrPlugIn(strPlugID);
				CComPtr<IDispatch> pDisp;
				//for COM Component:
				if (nPos == -1) {
					hr = pDisp.CoCreateInstance(strPlugID.AllocSysString());
					if (hr == S_OK)
					{
						m_pWorkXobj->m_pXobjShareData->m_PlugInDispDictionary[strPlugID] = pDisp.p;
						pDisp.p->AddRef();
					}

					m_pWorkXobj->Fire_XobjAddInCreated(pDisp.p, bstrPlugIn, bstrXml);
				}
				else //for .NET Component
				{
					hr = g_pCosmos->CreateCLRObj(bstrPlugIn, &pDisp);
					if (hr == S_OK)
					{
						m_pWorkXobj->m_pXobjShareData->m_PlugInDispDictionary[strPlugID] = pDisp.p;

						bstrPlugIn = strPlugID.AllocSysString();
						m_pWorkXobj->Fire_XobjAddInCreated(pDisp, bstrPlugIn, bstrXml);
					}
				}
				if (m_pGalaxyCluster && pDisp)
					m_pGalaxyCluster->Fire_AddInCreated(m_pWorkXobj, pDisp, bstrPlugIn, bstrXml);
				::SysFreeString(bstrPlugIn);
				bHavePlugin = true;
			}
			::SysFreeString(bstrXml);
		}

		if (bHavePlugin)
			m_pWorkXobj->Fire_XobjAddInsCreated();
	}
	m_pWorkXobj->m_bCreated = true;

	return true;
}

STDMETHODIMP CGalaxy::get_RootXobjs(IXobjCollection** pXobjColletion)
{
	if (m_pRootNodes == nullptr) {
		CComObject<CXobjCollection>::CreateInstance(&m_pRootNodes);
		m_pRootNodes->AddRef();
	}
	if (m_pRootNodes)
	{
		m_pRootNodes->m_pXobjs->clear();
		for (auto& it : m_mapXobj) {
			m_pRootNodes->m_pXobjs->push_back(it.second);
		}
	}

	return m_pRootNodes->QueryInterface(IID_IXobjCollection, (void**)pXobjColletion);
}

STDMETHODIMP CGalaxy::Detach(void)
{
	if (::IsWindow(m_hWnd))
	{
		m_bDetached = true;
		::ShowWindow(m_pWorkXobj->m_pHostWnd->m_hWnd, SW_HIDE);
		RECT rect;
		m_pWorkXobj->m_pHostWnd->GetWindowRect(&rect);
		m_hHostWnd = UnsubclassWindow(true);
		if (::IsWindow(m_hHostWnd)) {
			HWND m_hParentWnd = ::GetParent(m_hHostWnd);
			CWnd* pParent = CWnd::FromHandle(m_hParentWnd);
			pParent->ScreenToClient(&rect);
			::SetWindowPos(m_hHostWnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}
	}
	return S_OK;
}

STDMETHODIMP CGalaxy::Attach(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!::IsWindow(m_hWnd)) {
		m_bDetached = false;
		::ShowWindow(m_pWorkXobj->m_pHostWnd->m_hWnd, SW_SHOW);
		SubclassWindow(m_hHostWnd);
	}
	HostPosChanged();
	return S_OK;
}

STDMETHODIMP CGalaxy::ModifyHost(LONGLONG hHostWnd)
{
	HWND _hHostWnd = (HWND)hHostWnd;
	if (!::IsWindow(_hHostWnd) || m_hWnd == _hHostWnd)
	{
		return S_OK;
	}
	if (m_pWorkXobj == nullptr)
		return S_FALSE;
	HWND hParent = (HWND)::GetParent(_hHostWnd);
	CWindow m_Parent;
	RECT rc;
	m_pWorkXobj->m_pHostWnd->GetWindowRect(&rc);
	if (::IsWindow(m_hWnd)) {
		HWND hTangramWnd = m_pGalaxyCluster->m_hWnd;
		auto it = g_pCosmos->m_mapWindowPage.find(hTangramWnd);
		if (it != g_pCosmos->m_mapWindowPage.end())
			g_pCosmos->m_mapWindowPage.erase(it);
		m_pGalaxyCluster->m_hWnd = hTangramWnd;
		g_pCosmos->m_mapWindowPage[hTangramWnd] = m_pGalaxyCluster;

		auto it2 = m_pGalaxyCluster->m_mapGalaxy.find(m_hWnd);
		if (it2 != m_pGalaxyCluster->m_mapGalaxy.end()) {
			m_pGalaxyCluster->m_mapGalaxy.erase(it2);
			m_pGalaxyCluster->m_mapGalaxy[_hHostWnd] = this;
			m_pGalaxyCluster->m_mapWnd[m_strGalaxyName] = _hHostWnd;
			DWORD dwID = ::GetWindowThreadProcessId(_hHostWnd, NULL);
			TRACE(_T("ObserveEx ThreadInfo:%x\n"), dwID);
			CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
			theApp.SetHook(dwID);
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
			if (iter != pThreadInfo->m_mapGalaxy.end()) {
				pThreadInfo->m_mapGalaxy.erase(iter);
				pThreadInfo->m_mapGalaxy[_hHostWnd] = this;
			}
		}

		m_hHostWnd = UnsubclassWindow(true);
		if (::IsWindow(m_hHostWnd)) {
			HWND m_hParentWnd = ::GetParent(m_hHostWnd);
			m_Parent.Attach(m_hParentWnd);
			m_Parent.ScreenToClient(&rc);
			m_Parent.Detach();
			::MoveWindow(m_hHostWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, true);
			::ShowWindow(m_hHostWnd, SW_SHOW);
		}
	}

	SubclassWindow(_hHostWnd);
	m_hHostWnd = _hHostWnd;
	::GetWindowRect(_hHostWnd, &rc);
	m_Parent.Attach(hParent);
	m_Parent.ScreenToClient(&rc);
	m_Parent.Detach();
	for (auto& it : m_mapXobj)
	{
		if (it.second != m_pWorkXobj)
		{
			::SetParent(it.second->m_pHostWnd->m_hWnd, m_pWorkXobj->m_pHostWnd->m_hWnd);
		}
	}
	::SetParent(m_pWorkXobj->m_pHostWnd->m_hWnd, ::GetParent(_hHostWnd));
	::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	HostPosChanged();
	return S_OK;
}

STDMETHODIMP CGalaxy::get_HWND(LONGLONG* pVal)
{
	if (m_hWnd)
		*pVal = (LONGLONG)m_hWnd;
	else
		*pVal = (LONGLONG)m_hHostWnd;
	return S_OK;
}

STDMETHODIMP CGalaxy::get_VisibleXobj(IXobj** pVal)
{
	if (m_pWorkXobj)
		*pVal = m_pWorkXobj;
	return S_OK;
}

STDMETHODIMP CGalaxy::get_GalaxyCluster(IGalaxyCluster** pVal)
{
	if (m_pGalaxyCluster)
		*pVal = m_pGalaxyCluster;

	return S_OK;
}

STDMETHODIMP CGalaxy::Observe(BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj)
{
	CString _strXml = OLE2T(bstrXml);
	if (m_pGalaxyCluster->m_strPageFileName == _T(""))
	{
		m_pGalaxyCluster->m_strPageFileName = g_pCosmos->m_strExeName;
		m_pGalaxyCluster->m_strPageFilePath = g_pCosmos->m_strConfigDataFile;
	}
	DWORD dwID = ::GetWindowThreadProcessId(m_hHostWnd, NULL);
	TRACE(_T("ObserveEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
	theApp.SetHook(dwID);

	CString strCurrentKey = OLE2T(bstrKey);
	if (strCurrentKey == _T(""))
		strCurrentKey = _T("default");
	if (m_strCurrentKey != strCurrentKey)
	{
		m_strLastKey = m_strCurrentKey;
		m_strCurrentKey = strCurrentKey;
	}
	g_pCosmos->m_pGalaxyCluster = m_pGalaxyCluster;
	g_pCosmos->m_pGalaxy = this;

	m_strCurrentKey = m_strCurrentKey.MakeLower();
	g_pCosmos->m_strCurrentKey = m_strCurrentKey;
	CString strXml = _T("");
	CXobj* pOldNode = m_pWorkXobj;

	bool bExists = false;
	auto it = m_mapXobj.find(m_strCurrentKey);
	if (it != m_mapXobj.end())
	{
		bExists = true;
		m_pWorkXobj = it->second;
		m_pWorkXobj->m_bTopObj = true;
		if (pOldNode)
		{
			RECT rc;
			::GetWindowRect(pOldNode->m_pHostWnd->m_hWnd, &rc);
			pOldNode->m_pHostWnd->ShowWindow(SW_HIDE);
			GetParent().ScreenToClient(&rc);
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			m_pWorkXobj->m_pHostWnd->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		bool bAtTemplate = false;

		LRESULT l = ::SendMessage(m_pGalaxyCluster->m_hWnd, WM_HUBBLE_GETXML, (WPARAM)LPCTSTR(m_strGalaxyName), (WPARAM)LPCTSTR(m_strCurrentKey));
		if (l)
		{
			if (m_strCurrentXml != _T(""))
			{
				strXml = m_strCurrentXml;
				m_strCurrentXml = _T("");
			}
			else
			{
				auto it = g_pCosmos->m_mapValInfo.find(m_strGalaxyName + L"_" + m_strCurrentKey);
				if (it != g_pCosmos->m_mapValInfo.end())
				{
					strXml = OLE2T(it->second.bstrVal);
				}
				else
				{
					strXml = (LPCTSTR)l;
				}
			}
		}
		else
		{
			if (m_strCurrentXml != _T(""))
			{
				strXml = m_strCurrentXml;
				m_strCurrentXml = _T("");
			}
			else
			{
				if (bAtTemplate == false)
				{
					if (m_strCurrentKey != _T("newdocument"))
					{
						CString _str = _T("@") + m_strGalaxyName + _T("@") + m_pGalaxyCluster->m_strConfigFileNodeName;
						CString strKey2 = OLE2T(bstrKey);
						strKey2.MakeLower();
						if (strKey2 == _T(""))
							strKey2 = _T("default");
						CString _strKey = strKey2 + _str;
						auto itKey = m_pGalaxyCluster->m_strMapKey.find(_strKey);
						if (itKey != m_pGalaxyCluster->m_strMapKey.end()) {
							strXml = itKey->second;
						}
						else
						{
							CTangramXmlParse* m_pCosmosPageParse = nullptr;
							CTangramXmlParse* m_pCosmosPageParse2 = nullptr;
							if (m_pGalaxyCluster->m_bDoc == false && ::PathFileExists(m_pGalaxyCluster->m_strPageFilePath))
							{
								CTangramXmlParse m_Parse;
								if (m_Parse.LoadFile(m_pGalaxyCluster->m_strPageFilePath))
								{
									m_pCosmosPageParse = m_Parse.GetChild(_T("hubblepage"));
									if (m_pCosmosPageParse)
									{
										m_pCosmosPageParse2 = m_pCosmosPageParse->GetChild(m_pGalaxyCluster->m_strConfigFileNodeName);
										if (m_pCosmosPageParse2)
										{
											int nCount = m_pCosmosPageParse2->GetCount();
											for (int i = 0; i < nCount; i++)
											{
												CTangramXmlParse* _pParse = m_pCosmosPageParse2->GetChild(i);
												CString _str = _T("@") + _pParse->name() + _T("@") + m_pGalaxyCluster->m_strConfigFileNodeName;
												int nCount2 = _pParse->GetCount();
												for (int i = 0; i < nCount2; i++)
												{
													CTangramXmlParse* _pParse2 = _pParse->GetChild(i);
													m_pGalaxyCluster->m_strMapKey[_pParse2->name() + _str] = _pParse2->xml();
												}
											}
										}
									}
								}

								auto itKey = m_pGalaxyCluster->m_strMapKey.find(_strKey);
								if (strXml == _T("") && itKey != m_pGalaxyCluster->m_strMapKey.end()) {
									strXml = itKey->second;
								}
							}
						}
						if (strXml == _T(""))
							strXml = _strXml;
						if (strXml == _T(""))
							strXml = _T("<default><cluster><xobj  objid='nucleus' /></cluster></default>");;
					}
					else
						strXml = _strXml;
				}
			}
		}

		Unlock();
		m_pGalaxyCluster->Fire_BeforeOpenXml(CComBSTR(strXml), (long)m_hHostWnd);

		m_bNoRedrawState = false;
		m_pWorkXobj = g_pCosmos->ObserveEx((long)m_hHostWnd, _T(""), strXml);
		if (m_pWorkXobj == nullptr)
		{
			return S_FALSE;
		}
		if (::GetWindowLong(::GetParent(m_hWnd), GWL_EXSTYLE) & WS_EX_MDICHILD)
			m_bMDIChild = true;
	}
	m_pBindingXobj = m_pWorkXobj->m_pXobjShareData->m_pHostClientView ? m_pWorkXobj->m_pXobjShareData->m_pHostClientView->m_pXobj : nullptr;
	CXobj* _pXobj = nullptr;
	auto itHostBrowser = m_pWorkXobj->m_mapChildXobj.find(m_strHostWebBrowserNodeName);
	if (itHostBrowser != m_pWorkXobj->m_mapChildXobj.end())
	{
		_pXobj = itHostBrowser->second;
	}

	if (bExists)
	{
		if (_pXobj && m_pHostWebBrowserWnd)
		{
			if (_pXobj->m_nViewType == BlankView)
			{
				CGalaxy* _pGalaxy = nullptr;
				if (_pXobj->m_pHostGalaxy)
				{
					_pGalaxy = _pXobj->m_pHostGalaxy;
					while (_pGalaxy)
					{
						if (_pGalaxy->m_pHostWebBrowserNode)
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						if (_pXobj && _pXobj->m_pHostGalaxy)
						{
							_pGalaxy = _pXobj->m_pHostGalaxy;
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						}
						else
							break;
					}
				}
				if (m_pHostWebBrowserWnd->m_pParentXobj == nullptr)
				{
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
				}
				else if (m_pHostWebBrowserWnd->m_pParentXobj != _pXobj)
				{
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = nullptr;
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					_pXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
					::ShowWindow(m_pHostWebBrowserWnd->m_hWnd, SW_SHOW);
					RECT rc;
					::GetClientRect(_pXobj->m_pHostWnd->m_hWnd, &rc);
					::SetWindowPos(m_pHostWebBrowserWnd->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_NOACTIVATE | SWP_NOREDRAW);
					if (::IsChild(_pXobj->m_pHostWnd->m_hWnd, m_pHostWebBrowserWnd->m_hWnd) == false)
					{
						::SetParent(m_pHostWebBrowserWnd->m_hWnd, _pXobj->m_pHostWnd->m_hWnd);
					}
					//::SetWindowPos(m_pHostWebBrowserWnd->m_hWnd, HWND_TOP, -12, -6, rc.right + 24, rc.bottom + 18, SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
			::PostMessage(m_pHostWebBrowserWnd->m_hWnd, WM_BROWSERLAYOUT, 0, 2);
		}
	}
	if (m_pCosmosFrameWndInfo && m_pCosmosFrameWndInfo->m_nFrameType == 2 && m_nGalaxyType != CtrlBarGalaxy)
	{
		if (m_pMDIParent == nullptr)
		{
			auto itFrame = g_pCosmos->m_mapMDIParent.find(::GetParent(m_pCosmosFrameWndInfo->m_hClient));
			if (itFrame != g_pCosmos->m_mapMDIParent.end())
				m_pMDIParent = itFrame->second;
		}
		if (m_pMDIParent)
		{
			CXobj* pMdiClientObj = m_pWorkXobj->GetVisibleChildByName(_T("mdiclient"));
			if (pMdiClientObj)
				m_pMDIParent->m_pGalaxy->m_pBindingXobj = pMdiClientObj;
		}
	}
	else if (m_pParentMDIWinForm)
	{
		CXobj* pMdiClientObj = m_pWorkXobj->GetVisibleChildByName(_T("mdiclient"));
		if (pMdiClientObj)
		{
			HWND hClient = m_pParentMDIWinForm->m_hMDIClient;
			CGalaxy* pClientGalaxy = (CGalaxy*)g_pCosmos->GetGalaxy(hClient);
			if (pClientGalaxy)
			{
				m_pParentMDIWinForm->m_pClientGalaxy = pClientGalaxy;
				::PostMessage(m_hWnd, WM_COSMOSMSG, (WPARAM)pMdiClientObj, 20210331);
				pClientGalaxy->m_pBindingXobj = pMdiClientObj;
			}
		}
	}

	if (m_strGalaxyName == _T("default"))
	{
		CString strName = m_pWorkXobj->m_pHostParse->attr(_T("galaxy"), _T(""));
		if (strName != _T(""))
			m_strGalaxyName = strName;
	}

	g_pCosmos->m_strCurrentKey = _T("");
	*ppRetXobj = (IXobj*)m_pWorkXobj;
	for (auto& it : g_pCosmos->m_mapCosmosAppProxy)
	{
		it.second->OnObserverComplete(m_hHostWnd, strXml, m_pWorkXobj);
	}
	if (g_pCosmos->m_pCosmosAppProxy)
		g_pCosmos->m_pCosmosAppProxy->OnObserverComplete(m_hHostWnd, strXml, m_pWorkXobj);

	HostPosChanged();
	HWND hParent = ::GetParent(m_hWnd);
	::ShowWindow(m_pWorkXobj->m_pHostWnd->m_hWnd, SW_SHOW);
	//::SetParent(m_pWorkXobj->m_pHostWnd->m_hWnd, hParent);
	m_pWorkXobj->m_bTopObj = true;
	if (m_pWorkXobj->m_nViewType == Grid)
		::SetWindowLongPtr(m_pWorkXobj->m_pHostWnd->m_hWnd, GWLP_ID, m_pWorkXobj->m_nID);
	if (pOldNode && pOldNode != m_pWorkXobj)
	{
		RECT  rc;
		if (::IsWindow(pOldNode->m_pHostWnd->m_hWnd))
			::GetWindowRect(pOldNode->m_pHostWnd->m_hWnd, &rc);
		CWnd* pWnd = m_pWorkXobj->m_pHostWnd;

		CWnd::FromHandle(hParent)->ScreenToClient(&rc);

		for (auto& it : m_mapXobj)
		{
			if (it.second != m_pWorkXobj)
			{
				HWND hwnd = it.second->m_pHostWnd->m_hWnd;
				it.second->m_bTopObj = false;
				if (m_pWorkXobj->m_nViewType == Grid)
					::SetWindowLongPtr(hwnd, GWLP_ID, 0);
				::ShowWindow(hwnd, SW_HIDE);
				::SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE);
				//HWND _hParent = pWnd->m_hWnd;
				//::SetParent(hwnd, pWnd->m_hWnd);
			}
		}
		::SetWindowPos(pWnd->m_hWnd, HWND_BOTTOM, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);

		if (m_pWorkXobj != nullptr) {
			if (m_pWorkXobj->m_nViewType != Grid) {
				if (m_pWorkXobj->m_pHostWnd)
					m_pWorkXobj->m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
			HRESULT hr = S_OK;
			int cConnections = g_pCosmos->m_vec.GetSize();
			if (cConnections) {
				CComVariant avarParams[3];
				avarParams[2] = (long)m_hHostWnd;
				avarParams[2].vt = VT_I4;
				avarParams[1] = strXml.AllocSysString();
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = (IXobj*)m_pWorkXobj;
				avarParams[0].vt = VT_DISPATCH;
				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				for (int iConnection = 0; iConnection < cConnections; iConnection++) {
					g_pCosmos->Lock();
					CComPtr<IUnknown> punkConnection = g_pCosmos->m_vec.GetAt(iConnection);
					g_pCosmos->Unlock();
					IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);
					if (pConnection) {
						CComVariant varResult;
						hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
					}
				}
			}
		}

		for (auto& it : m_mapGalaxyProxy)
		{
			it.second->OnExtend(m_pWorkXobj, m_strCurrentKey, strXml);
		}

		if (m_pBKWnd && m_pBKWnd->m_pGalaxy && m_pBKWnd->m_pGalaxy->m_pWorkXobj == nullptr)
		{
			IXobj* pXobj = nullptr;
			m_pBKWnd->m_pGalaxy->Observe(CComBSTR(L"default"), CComBSTR(L""), &pXobj);
		}
	}
	if (m_nGalaxyType == GalaxyType::CtrlBarGalaxy)
	{
		if (m_pWorkXobj->m_strCaption != _T(""))
		{
			::SetWindowText(::GetParent(m_hWnd), m_pWorkXobj->m_strCaption);
			m_bObserveState = true;
			//g_pCosmos->m_bSZMode = true;
		}
	}

	//Add 20200218
	if (m_pBindingXobj)
	{
		CXobj* _pHostNode = m_pBindingXobj;
		if (_pHostNode->m_pHostGalaxy)
		{
			CGalaxy* _pGalaxy = _pHostNode->m_pHostGalaxy;
			while (_pGalaxy)
			{
				_pHostNode = _pGalaxy->m_pBindingXobj;
				if (_pHostNode && _pHostNode->m_pHostGalaxy)
					_pGalaxy = _pHostNode->m_pHostGalaxy;
				else
					break;
			}
		}
		if (_pHostNode && m_pWebPageWnd)
		{
			CXobj* pXobj = _pHostNode->m_pRootObj;
			if (pXobj->m_strLastIPCMsgID != _T(""))
			{
				IPCMsg pIPCInfo;
				pIPCInfo.m_strId = pXobj->m_strLastIPCMsgID;
				pIPCInfo.m_strParam1 = pXobj->m_strLastIPCParam1;
				pIPCInfo.m_strParam2 = pXobj->m_strLastIPCParam2;
				pIPCInfo.m_strParam3 = pXobj->m_strLastIPCParam5;
				pIPCInfo.m_strParam4 = pXobj->m_strLastIPCParam4;
				pIPCInfo.m_strParam5 = pXobj->m_strLastIPCParam3;
				m_pWebPageWnd->m_pChromeRenderFrameHost->SendCosmosMessage(&pIPCInfo);
			}
			g_pCosmos->m_pCurrentIPCMsg = nullptr;
		}
	}
	//end Add 20200218

	if (m_pWorkXobj->m_pHostGalaxy)
	{
		IXobj* pXobj = nullptr;
		m_pWorkXobj->m_pHostGalaxy->Observe(CComBSTR(m_pWorkXobj->m_pHostGalaxy->m_strCurrentKey), CComBSTR(""), &pXobj);
	}
	for (auto& it : m_pWorkXobj->m_mapExtendNode)
	{
		IXobj* pXobj = nullptr;
		it.first->Observe(CComBSTR(it.second), CComBSTR(""), &pXobj);
	}
	if (bExists == false)
	{
		if (_pXobj && m_pHostWebBrowserWnd)
		{
			if (_pXobj->m_nViewType == BlankView)
			{
				CGalaxy* _pGalaxy = nullptr;
				if (_pXobj->m_pHostGalaxy)
				{
					_pGalaxy = _pXobj->m_pHostGalaxy;
					while (_pGalaxy)
					{
						if (_pGalaxy->m_pHostWebBrowserNode)
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						if (_pXobj && _pXobj->m_pHostGalaxy)
						{
							_pGalaxy = _pXobj->m_pHostGalaxy;
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						}
						else
							break;
					}
				}
				if (m_pHostWebBrowserWnd->m_pParentXobj == nullptr)
				{
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
				}
				else if (m_pHostWebBrowserWnd->m_pParentXobj != _pXobj)//&&_pXobj->m_pHostWnd->IsWindowVisible())
				{
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = nullptr;
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					_pXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
					::SetParent(m_pHostWebBrowserWnd->m_hWnd, _pXobj->m_pHostWnd->m_hWnd);
					::SetWindowPos(m_pHostWebBrowserWnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE);
				}
			}
			::PostMessage(m_pHostWebBrowserWnd->m_hWnd, WM_BROWSERLAYOUT, 0, 2);
		}
	}
	if (m_pCosmosFrameWndInfo == nullptr)
	{
		HWND hFrameWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(DocView, m_hWnd);
		if (hFrameWnd == NULL)
		{
			if (m_pWebPageWnd)
			{
				CGalaxy* pGalaxy = nullptr;
				HWND hBrowser = ::GetParent(m_pWebPageWnd->m_hWnd);
				HWND hPPWnd = ::GetParent(hBrowser);
				CosmosInfo* pInfo = (CosmosInfo*)::GetProp(hPPWnd, _T("CosmosInfo"));
				if (pInfo)
				{
					CXobj* pXobj = (CXobj*)pInfo->m_pXobj;
					pGalaxy = pXobj->m_pXobjShareData->m_pGalaxy;
					hFrameWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(DocView, pGalaxy->m_hWnd);
				}
				else
				{
					DWORD dwID = ::GetWindowThreadProcessId(hPPWnd, NULL);
					CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
					auto iter = pThreadInfo->m_mapGalaxy.find(hPPWnd);
					if (iter != pThreadInfo->m_mapGalaxy.end())
					{
						pGalaxy = (CGalaxy*)iter->second;
					}
					hFrameWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(DocView, hPPWnd);
				}
			}
		}
		if (hFrameWnd)
		{
			m_pCosmosFrameWndInfo = (CosmosFrameWndInfo*)::GetProp(hFrameWnd, _T("CosmosFrameWndInfo"));
		}
	}
	if (m_pCosmosFrameWndInfo && m_pWebPageWnd)
	{
		CString _strKey = m_pWebPageWnd->m_strPageName + _T("_") + strCurrentKey;
		CComBSTR _bstrKey(_strKey);
		CTangramXmlParse* pParse = m_pWorkXobj->m_pXobjShareData->m_pCosmosParse;
		CTangramXmlParse* pClient = pParse->GetChild(m_pCosmosFrameWndInfo->m_nFrameType == 2 ? _T("mdiclient") : _T("client"));
		if (pClient)
		{
			CString strKey = _T("client");
			IGalaxy* pGalaxy = nullptr;
			auto it = m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.find(10000);
			if (it == m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.end())
			{
				HWND hClient = m_pCosmosFrameWndInfo->m_hClient;
				CString strXml = _T("");
				IGalaxyCluster* pCluster = nullptr;
				g_pCosmos->CreateGalaxyCluster((__int64)::GetParent(hClient), &pCluster);
				if (pCluster)
				{
					pCluster->CreateGalaxy(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(strKey), &pGalaxy);
					m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys[10000] = pGalaxy;
					strXml = pClient->xml();
					if (pGalaxy)
					{
						CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
						_pGalaxy->m_pWebPageWnd = m_pWebPageWnd;
						if (_strKey != _pGalaxy->m_strCurrentKey)
						{
							IXobj* pXobj = nullptr;
							_pGalaxy->Observe(_bstrKey, CComBSTR(strXml), &pXobj);
						}
					}
				}
			}
			else
			{
				pGalaxy = it->second;
				CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
				if (_strKey != _pGalaxy->m_strCurrentKey)
				{
					IXobj* pXobj = nullptr;
					_pGalaxy->Observe(_bstrKey, CComBSTR(strXml), &pXobj);
				}
			}
		}
		if (m_pCosmosFrameWndInfo->m_nFrameType == 2)
		{
			pClient = pParse->GetChild(_T("hostpage"));
			CGalaxy* pGalaxy = nullptr;
			auto it = m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.find(10000);
			if (it != m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.end())
			{
				pGalaxy = (CGalaxy*)it->second;
				if (pClient && pGalaxy && pGalaxy->m_pWebPageWnd)
				{
					pGalaxy->m_pWebPageWnd->LoadDocument2Viewport(_strKey, pClient->xml());
				}
			}
		}
		pClient = pParse->GetChild(_T("controlbars"));
		if (pClient)
		{
			int nCount = pClient->GetCount();
			for (int i = 0; i < nCount; i++)
			{
				CTangramXmlParse* pParse2 = pClient->GetChild(i);
				int nBarID = pParse2->attrInt(_T("ctrlbarid"), 0);
				if (nBarID)
				{
					auto it = m_pCosmosFrameWndInfo->m_mapCtrlBarWnd.find(nBarID);
					if (it != m_pCosmosFrameWndInfo->m_mapCtrlBarWnd.end())
					{
						HWND hWnd = it->second;
						int nID = pParse2->attrInt(_T("clientid"), 0);
						HWND hClient = ::GetDlgItem(hWnd, nID);
						if (hClient)
						{
							CString strXml = _T("");
							IGalaxy* pGalaxy = nullptr;
							CString strKey = _T("");
							strKey.Format(_T("ControlBar_%d"), nBarID);
							auto it = m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.find(nBarID);
							if (it != m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.end())
							{
								pGalaxy = it->second;
							}
							else
							{
								m_pGalaxyCluster->CreateGalaxy(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(strKey), &pGalaxy);
								strXml = pParse2->xml();
							}
							if (pGalaxy)
							{
								CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
								m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys[nBarID] = _pGalaxy;
								_pGalaxy->m_pWebPageWnd = m_pWebPageWnd;
								IXobj* pXobj = nullptr;
								_pGalaxy->Observe(_bstrKey, CComBSTR(strXml), &pXobj);
							}
							CString strCaption = pParse2->attr(_T("caption"), _T(""));
							if (strCaption != _T(""))
								::SetWindowText(::GetParent(hClient), strCaption);
						}
					}
				}
			}
		}
		m_pCosmosFrameWndInfo->bControlBarProessed = true;
	}

	::PostMessage(m_hWnd, WM_COSMOSMSG, 2, 20180115);
	return S_OK;
}

STDMETHODIMP CGalaxy::get_CurrentNavigateKey(BSTR* pVal)
{
	*pVal = m_strCurrentKey.AllocSysString();
	return S_OK;
}

void CGalaxy::UpdateVisualWPFMap(HWND hParent, BOOL bSized)
{
	for (auto it : m_mapWPFView)
	{
		HWND hWnd = it.first;
		if (::IsChild(hParent, hWnd))
		{
			auto it2 = m_mapVisibleWPFView.find(hWnd);
			if (::IsWindowVisible(hWnd) == TRUE)
			{
				if (it2 == m_mapVisibleWPFView.end())
				{
					m_mapVisibleWPFView[hWnd] = it.second;
				}
				it.second->m_pCosmosWPFObj->ShowVisual(false);
				it.second->m_pCosmosWPFObj->ShowVisual(true);
				it.second->m_pCosmosWPFObj->InvalidateVisual();
				long nIndex = (long)::GetWindowLongPtr(it.second->m_hWnd, GWLP_USERDATA);
				if (bSized || nIndex == 1963)
				{
					::SetWindowLongPtr(it.second->m_hWnd, GWLP_USERDATA, 0);

					RECT rc;
					::GetWindowRect(it.second->m_hWnd, &rc);
					CWnd* pParent = it.second->GetParent();
					pParent->ScreenToClient(&rc);
					::SetWindowPos(it.second->m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + 1, SWP_DRAWFRAME);
					::SetWindowPos(it.second->m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
			else
			{
				if (it2 != m_mapVisibleWPFView.end())
				{
					it.second->m_pCosmosWPFObj->ShowVisual(false);
					m_mapVisibleWPFView.erase(it2);
				}
			}
		}
	}
}

void CGalaxy::Destroy()
{
	CXobj* pWndXobj = nullptr;
	CString strPlugID = _T("");
	for (auto it : m_mapXobj)
	{
		pWndXobj = it.second;
		if (pWndXobj->m_pXobjShareData->m_pCosmosParse)
		{
			CTangramXmlParse* pParse = pWndXobj->m_pXobjShareData->m_pCosmosParse->GetChild(_T("docplugin"));
			if (pParse)
			{
				int nCount = pParse->GetCount();
				for (int i = 0; i < nCount; i++)
				{
					CTangramXmlParse* pChild = pParse->GetChild(i);
					strPlugID = pChild->text();
					strPlugID.Trim();
					if (strPlugID != _T(""))
					{
						if (strPlugID.Find(_T(",")) == -1)
						{
							strPlugID.MakeLower();
							IDispatch* pDisp = (IDispatch*)pWndXobj->m_pXobjShareData->m_PlugInDispDictionary[strPlugID];
							if (pDisp)
							{
								pWndXobj->m_pXobjShareData->m_PlugInDispDictionary.RemoveKey(LPCTSTR(strPlugID));
								pDisp->Release();
							}
						}
					}
				}
			}

			pWndXobj->m_pXobjShareData->m_PlugInDispDictionary.RemoveAll();
		}
	}
}

void CGalaxy::OnFinalMessage(HWND hWnd)
{
	HWND hParent = ::GetParent(hWnd);
	if (::IsWindow(hParent))
		::PostMessage(hParent, WM_COSMOSMSG, 4096, 0);
	__super::OnFinalMessage(hWnd);
}

LRESULT CGalaxy::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT hr = DefWindowProc(uMsg, wParam, lParam);
	if (m_pBKWnd)
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		if (::IsWindow(m_pBKWnd->m_hWnd))
			::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 2, 2, rect.right - 4, rect.bottom - 4, SWP_NOREPOSITION | SWP_NOSENDCHANGING | SWP_NOACTIVATE);
		else
			::InvalidateRect(m_hWnd, &rect, true);
		return hr;
	}
	return hr;
}

LRESULT CGalaxy::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pCosmos->m_pActiveHtmlWnd)
	{
		g_pCosmos->m_pActiveHtmlWnd = nullptr;
	}

	if ((long)(g_pCosmos->m_pActiveAppProxy) > 1)
	{
		HWND hMenuWnd = g_pCosmos->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	if (m_pBKWnd == nullptr)
	{
		g_pCosmos->m_hActiveWnd = 0;
		g_pCosmos->m_bWinFormActived = false;
		g_pCosmos->m_pActiveXobj = nullptr;
		g_pCosmos->m_pGalaxy = nullptr;
	}
	//bug fix for winform:
	//++++++++++++++++begin+++++++++++
	HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
	if (hTop != nullptr && g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_bWinFormActived = g_pCosmos->m_pCLRProxy->IsWinForm(hTop);
		if (g_pCosmos->m_bWinFormActived)
		{
			g_pCosmos->m_hActiveWnd = hTop;
			g_pCosmos->m_pGalaxy = this;
		}
	}
	//++++++++++++++++end+++++++++++

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	if (wParam == 0 && m_pWorkBenchFrame && ::GetWindow(m_hWnd, GW_CHILD) == nullptr)
	{
		HWND hFirst = ::GetWindow(m_hWnd, GW_HWNDFIRST);
		if (hFirst && hFirst != m_hWnd)
		{
			::GetClassName(hFirst, g_pCosmos->m_szBuffer, MAX_PATH);
			CString strName = CString(g_pCosmos->m_szBuffer);
			if (strName == _T("SWT_Window0"))
			{
				HWND hWnd = UnsubclassWindow(true);
				if (hWnd)
				{
					SubclassWindow(hFirst);
					auto it = m_pGalaxyCluster->m_mapGalaxy.find(hWnd);
					if (it != m_pGalaxyCluster->m_mapGalaxy.end())
					{
						m_pWorkBenchFrame->m_hClient = m_hWnd;
						::SetWindowLongPtr(hWnd, GWLP_ID, 0);
						::SetWindowLongPtr(m_hWnd, GWLP_ID, AFX_IDW_PANE_FIRST);
						m_hHostWnd = m_hWnd;
						m_pGalaxyCluster->m_mapGalaxy.erase(it);
						m_pGalaxyCluster->m_mapGalaxy[m_hWnd] = this;
						m_pGalaxyCluster->m_mapWnd[m_strGalaxyName] = m_hWnd;
						CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(::GetWindowThreadProcessId(m_hWnd, NULL));
						auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
						if (iter != pThreadInfo->m_mapGalaxy.end()) {
							pThreadInfo->m_mapGalaxy.erase(iter);
							pThreadInfo->m_mapGalaxy[m_hWnd] = this;
						}
					}
				}
			}
		}
	}
	return lRes;
}

LRESULT CGalaxy::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	::RemoveProp(m_hWnd, _T("CosmosData"));
	if (g_pCosmos->m_pCLRProxy) {
		g_pCosmos->m_pCLRProxy->ReleaseCosmosObj((IGalaxy*)this);
	}
	if (m_pBKWnd)
		m_pBKWnd->DestroyWindow();
	m_pGalaxyCluster->BeforeDestory();
	m_pGalaxyCluster->m_strConfigFileNodeName.MakeLower();//20190116

	DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
	TRACE(_T("ObserveEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
	if (pThreadInfo)
	{
		//theApp.SetHook(dwID);
		auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
		if (iter != pThreadInfo->m_mapGalaxy.end())
		{
			pThreadInfo->m_mapGalaxy.erase(iter);
			TRACE(_T("Tangram Frame Removed from ThreadInfo Manager:%x\n"), m_hWnd);
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20210411:
	{
		if (theApp.m_bAppStarting == false)
		{
			if (m_pMDIParent)
			{
				if (m_pMDIParent->m_bCreateNewDoc || m_pMDIParent->m_pHostBrowser->m_pVisibleWebView->m_bCanShow == false)
					break;
			}
			HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
			::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN /*| RDW_UPDATENOW*/);
		}
	}
	break;
	case 20210331:
	{
		CXobj* pMdiClientObj = (CXobj*)wParam;
		m_pParentMDIWinForm->m_pClientGalaxy->m_pBindingXobj = pMdiClientObj;
	}
	break;
	case 2048:
	{
		if (m_hWnd)
		{
			CtrlInfo* pCtrlInfo = (CtrlInfo*)wParam;
			if (pCtrlInfo && pCtrlInfo->m_pGalaxyCluster)
			{
				CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)pCtrlInfo->m_pGalaxyCluster;
				pGalaxyCluster->Fire_ClrControlCreated(pCtrlInfo->m_pXobj, pCtrlInfo->m_pCtrlDisp, pCtrlInfo->m_strName.AllocSysString(), (long)pCtrlInfo->m_hWnd);
			}
		}
	}
	break;
	case 20210228:
	{
		m_bNoRedrawState = true;
	}
	break;
	case 20180115:
	{
		if (wParam == 1 && m_nGalaxyType == GalaxyType::CtrlBarGalaxy)
		{
			::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)m_hWnd, 20210309);
		}
		//if (m_pParentMDIWinForm)
		//{
		//	if (m_pParentMDIWinForm->m_pClientGalaxy)
		//	{
		//		if (m_pHostWebBrowserWnd)
		//		{
		//			if (m_pHostWebBrowserWnd->m_pVisibleWebView && m_pHostWebBrowserWnd->m_pVisibleWebView->m_pGalaxy)
		//			{
		//				CXobj* pObj = m_pHostWebBrowserWnd->m_pVisibleWebView->m_pGalaxy->m_pWorkXobj->GetVisibleChildByName(_T("mdiclient"));
		//				m_pBindingXobj = pObj;
		//			}
		//		}
		//	}
		//}
		HostPosChanged();
		if (m_pCosmosFrameWndInfo && m_pCosmosFrameWndInfo->m_nFrameType == 2)
		{
			CGalaxy* pGalaxy = nullptr;
			auto it = m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.find(100000);
			if (it != m_pCosmosFrameWndInfo->m_mapCtrlBarGalaxys.end())
			{
				if (::IsChild(::GetParent(m_pCosmosFrameWndInfo->m_hClient), m_hWnd))
				{
					pGalaxy = (CGalaxy*)it->second;
					pGalaxy->HostPosChanged();
				}
			}
		}

		if (wParam == 3)
		{
			HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
			::RedrawWindow(hTop, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN /*| RDW_UPDATENOW*/);
		}
	}
	break;
	case WM_BROWSERLAYOUT:
	{
		CWebView* pWebWnd = (CWebView*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		::PostMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 1);
	}
	break;
	case 19651965:
	{
		for (auto it : m_pGalaxyCluster->m_mapGalaxy)
		{
			it.second->UpdateVisualWPFMap((HWND)wParam, false);
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnTabChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD);
	if (hWnd == nullptr)
		return 0;
	if (m_pWorkBenchFrame && ::IsWindowVisible(m_hWnd))
	{
		RECT rc, rc0;
		::GetClientRect(m_hWnd, &rc0);
		::GetClientRect(hWnd, &rc);
		if (rc.right * rc.bottom == 0)
		{
			::MoveWindow(hWnd, 0, 0, rc0.right, rc0.bottom, 1);
		}
		HWND hWnd2 = ::GetWindow(hWnd, GW_CHILD);
		if (hWnd2 == nullptr)
			return 0;
		::GetClassName(hWnd2, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strName = CString(g_pCosmos->m_szBuffer);
		if (strName == _T("SWT_Window0"))
		{
			::GetClientRect(hWnd2, &rc);
			if (rc.right * rc.bottom == 0)
				::MoveWindow(hWnd2, 0, 0, rc0.right, rc0.bottom, 1);
			hWnd2 = ::GetWindow(hWnd2, GW_CHILD);
			if (hWnd2 == nullptr)
				return 0;
			::GetClassName(hWnd2, g_pCosmos->m_szBuffer, MAX_PATH);
			strName = CString(g_pCosmos->m_szBuffer);
			if (strName == _T("SWT_Window0"))
			{
				::GetClientRect(hWnd2, &rc);
				if (rc.right * rc.bottom == 0)
					::MoveWindow(hWnd2, 0, 0, rc0.right, rc0.bottom, 1);
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	//RECT* const prcNewWindow = (RECT*)lParam;
	//::SetWindowPos(m_hWnd,
	//	NULL,
	//	prcNewWindow->left,
	//	prcNewWindow->top,
	//	prcNewWindow->right - prcNewWindow->left,
	//	prcNewWindow->bottom - prcNewWindow->top,
	//	SWP_NOZORDER | SWP_NOACTIVATE);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnGetDPIScaledSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	//RECT* const prcNewWindow = (RECT*)lParam;
	//::SetWindowPos(m_hWnd,
	//	NULL,
	//	prcNewWindow->left,
	//	prcNewWindow->top,
	//	prcNewWindow->right - prcNewWindow->left,
	//	prcNewWindow->bottom - prcNewWindow->top,
	//	SWP_NOZORDER | SWP_NOACTIVATE);
	return false;// DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnBeforeParentDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnAfterParentDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnGetMe(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (lParam == 1992)
	{
		return (LRESULT)this;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LONG_PTR pfnWndProc = ::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
	LRESULT lr = DefWindowProc(uMsg, wParam, lParam);
	if (m_pfnSuperWindowProc != ::DefWindowProc && ::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC) == pfnWndProc)
		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_pfnSuperWindowProc);

	// mark window as destryed
	m_dwState |= WINSTATE_DESTROYED;
	return lr;
}

LRESULT CGalaxy::OnQueryAppProxy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pCosmosFrameWndInfo)
	{
		CMDIParent* pMDIParent = nullptr;
		auto it = g_pCosmos->m_mapMDIParent.find(::GetParent(m_pCosmosFrameWndInfo->m_hClient));
		if (it != g_pCosmos->m_mapMDIParent.end())
		{
			pMDIParent = it->second;
			if (lParam == 19651965)
			{
				m_bTabbedMDIClient = true;
				LPRECT lpRECT = (LPRECT)wParam;
				if (lpRECT && m_pWorkXobj && ::IsWindowVisible(m_pWorkXobj->m_pHostWnd->m_hWnd))
				{
					::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpRECT->left, lpRECT->top, lpRECT->right - lpRECT->left, lpRECT->bottom - lpRECT->top, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOZORDER);/*SWP_FRAMECHANGED| SWP_HIDEWINDOW | SWP_NOREDRAW */
					if (m_pBindingXobj && ::IsWindowVisible(m_pBindingXobj->m_pHostWnd->m_hWnd))
					{
						m_pBindingXobj->m_pHostWnd->GetWindowRect(lpRECT);
						pMDIParent->ScreenToClient(lpRECT);
					}
				}
				return m_pWorkXobj ? ((LRESULT)m_pWorkXobj->m_pHostWnd->m_hWnd) : 0;
			}
		}
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT hr = DefWindowProc(uMsg, wParam, lParam);

	if (g_pCosmos->m_pBrowserFactory && g_pCosmos->m_bChromeNeedClosed)
		return hr;
	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
	//if(m_pParentMDIWinForm&& m_pParentMDIWinForm->m_pClientGalaxy)
	//{
	//	CXobj* _pObj = m_pWorkXobj->GetVisibleChildByName(_T("mdiclient"));
	//	if (_pObj)
	//	{
	//		m_pParentMDIWinForm->m_pClientGalaxy->m_pBindingXobj = _pObj;
	//	}
	//}
	if (m_pWorkXobj)
	{
		if (m_pBindingXobj)
		{
			RECT rect = { 0,0,0,0 };
			HWND hPWnd = ::GetParent(m_hWnd);
			if (::SendMessage(hPWnd, WM_QUERYAPPPROXY, (WPARAM)&rect, 19921989) == 19921989)
			{
				lpwndpos->x = rect.left;
				lpwndpos->y = rect.top;
				lpwndpos->cx = rect.right - rect.left;
				lpwndpos->cy = rect.bottom - rect.top;
			}
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOACTIVATE | SWP_FRAMECHANGED);// |SWP_NOREDRAW); 

			CXobj* _pHostNode = m_pBindingXobj;
			if (_pHostNode->m_pHostGalaxy)
			{
				CGalaxy* _pGalaxy = _pHostNode->m_pHostGalaxy;
				while (_pGalaxy)
				{
					if (_pGalaxy->m_pBindingXobj)
						_pHostNode = _pGalaxy->m_pBindingXobj;
					else
						break;
					if (_pHostNode && _pHostNode->m_pHostGalaxy)
					{
						_pGalaxy = _pHostNode->m_pHostGalaxy;
						if (_pGalaxy == nullptr)
							break;
					}
					else
						break;
				}
			}
			HWND hHost = _pHostNode->m_pHostWnd->m_hWnd;
			bool bVisible = ::IsWindowVisible(hHost);
			if (bVisible)
			{
				RECT rc;
				::GetClientRect(hHost, &rc);
				if (rc.bottom * rc.right == 0)
					bVisible = false;
			}
			if (bVisible)
			{
				::GetWindowRect(hHost, &rect);
				::ScreenToClient(hPWnd, (LPPOINT)&rect);
				::ScreenToClient(hPWnd, ((LPPOINT)&rect) + 1);
				lpwndpos->x = rect.left;
				lpwndpos->y = rect.top;
				lpwndpos->cx = rect.right - rect.left;
				lpwndpos->cy = rect.bottom - rect.top;
				lpwndpos->flags &= ~SWP_HIDEWINDOW;
				lpwndpos->flags |= SWP_SHOWWINDOW | SWP_NOZORDER;
				//begin fix bug for .net Control or Window Form
				CXobj* _pParentNode = m_pBindingXobj->m_pParentObj;
				if (_pParentNode)
				{
					switch (_pParentNode->m_nViewType)
					{
					case Grid:
					{
						CGridWnd* pWnd = (CGridWnd*)_pParentNode->m_pHostWnd;
						if (pWnd->bInited == false)
							::SendMessage(pWnd->m_hWnd, WM_COSMOSMSG, 1, 1993);
					}
					break;
					case TabGrid:
					{
						CXobjWnd* pWnd = (CXobjWnd*)_pParentNode->m_pHostWnd;
						::InvalidateRect(pWnd->m_hWnd, nullptr, true);
					}
					break;
					}
				}
				if (m_pContainerNode && m_pContainerNode->m_pParentObj && m_pContainerNode->m_pParentObj->m_nViewType == Grid)
				{
					if (m_pContainerNode->m_nViewType == CLRCtrl)
					{
						CXobjWnd* pXobjWnd = (CXobjWnd*)m_pContainerNode->m_pHostWnd;
						pXobjWnd->m_bNoMove = true;
					}
				}
				//end fix bug for .net Control or Window Form
			}
			else
			{
				lpwndpos->x = lpwndpos->y = lpwndpos->cx = lpwndpos->cy = 0;
				lpwndpos->flags |= SWP_HIDEWINDOW | SWP_NOZORDER;
			}
		}
		else
		{
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOSENDCHANGING | /*SWP_NOZORDER |*/ SWP_NOACTIVATE | SWP_FRAMECHANGED);
			lpwndpos->flags &= ~SWP_SHOWWINDOW;
			lpwndpos->flags |= SWP_HIDEWINDOW;
		}
	}

	if (m_bMDIChild)
		lpwndpos->flags |= SWP_NOZORDER;
	::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	if (::IsWindowVisible(m_hWnd))
		::InvalidateRect(m_hWnd, nullptr, true);
	if (m_pBKWnd)
	{
		::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 2, 2, lpwndpos->cx - 4, lpwndpos->cy - 4, SWP_NOZORDER | SWP_NOACTIVATE);
	}
	return hr;
}

LRESULT CGalaxy::OnParentNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_pGalaxy = nullptr;
	return DefWindowProc(uMsg, wParam, lParam);
}

STDMETHODIMP CGalaxy::GetXml(BSTR bstrRootName, BSTR* bstrRet)
{
	CString strRootName = OLE2T(bstrRootName);
	if (strRootName == _T(""))
		strRootName = _T("DocumentUI");
	CString strXmlData = _T("<Default><cluster><xobj name=\"Start\"/></cluster></Default>");
	CString strName = _T("");
	CString strXml = _T("");

	map<CString, CString> m_mapTemp;
	map<CString, CString>::iterator it2;
	for (auto it : m_mapXobj)
	{
		g_pCosmos->UpdateXobj(it.second);
		strName = it.first;
		int nPos = strName.Find(_T("-"));
		CString str = strName.Mid(nPos + 1);
		if (str.CompareNoCase(_T("inDesigning")) == 0)
		{
			strName = strName.Left(nPos);
			m_mapTemp[strName] = it.second->m_pXobjShareData->m_pCosmosParse->xml();
		}
	}

	for (auto it : m_mapXobj)
	{
		strName = it.first;
		if (strName.Find(_T("-indesigning")) == -1)
		{
			it2 = m_mapTemp.find(strName);
			if (it2 != m_mapTemp.end())
				strXml = it2->second;
			else
				strXml = it.second->m_pXobjShareData->m_pCosmosParse->xml();
			strXmlData += strXml;
		}
	}

	strXml = _T("<");
	strXml += strRootName;
	strXml += _T(">");
	strXml += strXmlData;
	strXml += _T("</");
	strXml += strRootName;
	strXml += _T(">");
	*bstrRet = strXml.AllocSysString();
	return S_OK;
}

STDMETHODIMP CGalaxy::get_Count(long* pCount)
{
	*pCount = (long)m_mapXobj.size();
	return S_OK;
}

STDMETHODIMP CGalaxy::get_Xobj(VARIANT vIndex, IXobj** ppXobj)
{
	if (vIndex.vt == VT_I4)
	{
		long lCount = m_mapXobj.size();
		int iIndex = vIndex.lVal;
		if (iIndex < 0 || iIndex >= lCount) return E_INVALIDARG;

		auto it = m_mapXobj.begin();
		int iPos = 0;
		while (it != m_mapXobj.end())
		{
			if (iPos == iIndex) break;
			iPos++;
			it++;
		}

		CXobj* pXobj = it->second;
		*ppXobj = pXobj;
		return S_OK;
	}

	if (vIndex.vt == VT_BSTR)
	{
		CString strKey = OLE2T(vIndex.bstrVal);
		auto it = m_mapXobj.find(strKey);
		if (it != m_mapXobj.end())
		{
			*ppXobj = it->second;
			return S_OK;
		}
		return E_INVALIDARG;
	}

	return S_OK;
}

STDMETHODIMP CGalaxy::get__NewEnum(IUnknown** ppVal)
{
	typedef CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>>
		CComEnumVariant;

	CComObject<CComEnumVariant>* pe = 0;
	HRESULT hr = pe->CreateInstance(&pe);

	if (SUCCEEDED(hr))
	{
		pe->AddRef();
		long nLen = (long)m_mapXobj.size();
		VARIANT* rgvar = new VARIANT[nLen];
		ZeroMemory(rgvar, sizeof(VARIANT) * nLen);
		VARIANT* pItem = rgvar;
		for (auto& it : m_mapXobj)
		{
			IUnknown* pDisp = nullptr;
			CXobj* pObj = it.second;
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

STDMETHODIMP CGalaxy::get_GalaxyXML(BSTR* pVal)
{
	CString strData = _T("<");
	CString strName = m_strGalaxyName;
	strName.Replace(_T("@"), _T("_"));
	strData += strName;
	strData += _T(">");
	for (auto it : m_mapXobj)
	{
		CXobj* pWndXobj = (CXobj*)it.second;
		if (pWndXobj)
		{
			if (pWndXobj->m_pWindow)
			{
				if (pWndXobj->m_nActivePage > 0)
				{
					CString strVal = _T("");
					strVal.Format(_T("%d"), pWndXobj->m_nActivePage);
					pWndXobj->m_pHostParse->put_attr(_T("activepage"), strVal);
				}
				pWndXobj->m_pWindow->Save();
			}
			if (pWndXobj->m_nViewType == Grid)
			{
				((CGridWnd*)pWndXobj->m_pHostWnd)->Save();
			}

			for (auto it2 : pWndXobj->m_vChildNodes)
			{
				g_pCosmos->UpdateXobj(it2);
			}

			if (pWndXobj == pWndXobj->m_pRootObj && pWndXobj->m_pXobjShareData->m_pOfficeObj)
			{
				g_pCosmos->UpdateOfficeObj(pWndXobj->m_pXobjShareData->m_pOfficeObj, pWndXobj->m_pXobjShareData->m_pCosmosParse->GetChild(TGM_CLUSTER)->xml(), pWndXobj->m_pXobjShareData->m_pCosmosParse->name());
			}
		}
		CString strXml = pWndXobj->m_pXobjShareData->m_pCosmosParse->GetChild(TGM_CLUSTER)->xml();
		CString s = _T("");
		s.Format(_T("<%s>%s</%s>"), it.first, strXml, it.first);
		CString strKey = it.second->m_strKey + _T("@") + this->m_strGalaxyName + _T("@") + _T("tangramdefaultpage");
		auto it = m_pGalaxyCluster->m_strMapKey.find(strKey);
		if (it != m_pGalaxyCluster->m_strMapKey.end())
		{
		}
		m_pGalaxyCluster->m_strMapKey[strKey] = s;
		strData += s;
	}
	strData += _T("</");
	strData += strName;
	strData += _T(">");
	CString strXml = _T("<");
	strXml += strName;
	strXml += _T(">");
	for (auto it : m_pGalaxyCluster->m_strMapKey)
	{
		CString strKey = it.first;
		if (strKey.Find(_T("@") + m_strGalaxyName + _T("@")) != -1)
		{
			strXml += it.second;
		}
	}
	strXml += _T("</");
	strXml += strName;
	strXml += _T(">");
	*pVal = strXml.AllocSysString();
	return S_OK;
}

STDMETHODIMP CGalaxy::get_GalaxyType(GalaxyType* pVal)
{
	*pVal = m_nGalaxyType;
	return S_OK;
}

STDMETHODIMP CGalaxy::get_Name(BSTR* pVal)
{
	*pVal = m_strGalaxyName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CGalaxy::get_HostBrowser(IBrowser** ppChromeWebBrowser)
{
	if (m_pWebPageWnd != nullptr)
	{
		HWND hPWnd = ::GetParent(m_pWebPageWnd->m_hWnd);
		if (::IsWindow(hPWnd))
		{
			auto it = g_pCosmos->m_mapBrowserWnd.find(hPWnd);
			if (it != g_pCosmos->m_mapBrowserWnd.end())
			{
				*ppChromeWebBrowser = it->second;
				return S_OK;
			}
		}
	}
	return S_FALSE;
}

STDMETHODIMP CGalaxy::get_HostWebPage(IWebPage** ppChromeWebPage)
{
	if (m_pWebPageWnd != nullptr)
	{
		*ppChromeWebPage = (IWebPage*)m_pWebPageWnd;
	}
	return S_FALSE;
}

