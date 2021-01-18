/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101180012
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
#include "XobjHelper.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "TangramHtmlTreeWnd.h"
#include "EclipsePlus\EclipseAddin.h"
#include "DocTemplateDlg.h"
#include "Wormhole.h"

class CDockPaneWnd : public CWnd
{
	DECLARE_DYNAMIC(CDockPaneWnd)
public:
	CDockPaneWnd() {}
	virtual ~CDockPaneWnd() {}
	CGalaxy* m_pGalaxy = nullptr;
	HWND m_hClient = nullptr;
	void PostNcDestroy()
	{
		CWnd::PostNcDestroy();
		delete this;
	}
	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnCosmosMsg(WPARAM wParam, LPARAM lParam);
};

IMPLEMENT_DYNAMIC(CDockPaneWnd, CWnd)

BEGIN_MESSAGE_MAP(CDockPaneWnd, CWnd)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_COSMOSMSG, OnCosmosMsg)
END_MESSAGE_MAP()

void CDockPaneWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if (bShow && ::IsWindow(m_hClient))
	{
		if (m_pGalaxy == nullptr)
		{
			DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
			CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
			auto iter = pThreadInfo->m_mapGalaxy.find(m_hClient);
			if (iter != pThreadInfo->m_mapGalaxy.end())
			{
				m_pGalaxy = (CGalaxy*)iter->second;
			}
		}
		//::ShowWindow(m_hClient, SW_SHOW);
		::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20210107);
	}
}

LRESULT CDockPaneWnd::OnCosmosMsg(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 20210107)
	{
		if (m_pGalaxy)
			m_pGalaxy->HostPosChanged();
	}
	return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
}

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
	CWebPage* pPage = nullptr;
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
	CWebPage* pPage = nullptr;
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
	if (pTreeItemData&&m_pGalaxy)
	{
		pPage = m_pGalaxy->m_pWebPageWnd;
		if (m_pGalaxy->m_pWormhole)
		{
			m_pGalaxy->m_pWormhole->InsertString(_T("msgID"), _T("TREECTRL_TvnSelchanged"));
			m_pGalaxy->m_pWormhole->InsertString(_T("treenodekey"), pTreeItemData->m_strKey);
			m_pGalaxy->m_pWormhole->InsertString(_T("treenodetag"), pTreeItemData->m_strData);
			m_pGalaxy->m_pWormhole->SendMessage();
			m_pGalaxy->m_pWormhole->InsertString(_T("msgID"), _T(""));
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
	ON_MESSAGE(WM_XOBJCREATED, OnXobjCreatedMsg)
	ON_MESSAGE(WM_CLOUDMSGRECEIVED, OnCloudMsgReceived)
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

CMDIChildHelperWnd::CMDIChildHelperWnd(void)
{
	m_hClient = nullptr;
	m_hParent = nullptr;
	m_strKey = _T("");
}

CMDIChildHelperWnd::~CMDIChildHelperWnd(void)
{
}

LRESULT CMDIChildHelperWnd::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	if (g_pCosmos->m_nAppType == 1992 && ::IsIconic(m_hWnd))
		::SendMessage(::GetParent(m_hWnd), WM_MDIICONARRANGE, 0, 0);
	return l;
}

LRESULT CMDIChildHelperWnd::OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (lParam != wParam)
	{
		HWND hActivedWnd = (HWND)lParam;
		CMDIChildHelperWnd* _pWnd = nullptr;
		CString strKey = _T("");
		if (hActivedWnd)
		{
			_pWnd = (CMDIChildHelperWnd*)::SendMessage(hActivedWnd, WM_COSMOSMSG, 0, 19631222);
			if (_pWnd)
			{
				strKey = _pWnd->m_strKey;
			}
		}
		if (_pWnd == nullptr)
			return DefWindowProc(uMsg, wParam, lParam);

		IXobj* _pXobj = nullptr;
		CComBSTR bstrKey(strKey);
		BSTR bstrXml = ::SysAllocString(L"");
		CosmosFrameWndInfo* pCosmosFrameWndInfo = nullptr;
		HANDLE hHandle = ::GetProp(::GetParent(::GetParent(m_hWnd)), _T("CosmosFrameWndInfo"));;
		if (hHandle)
		{
			pCosmosFrameWndInfo = (CosmosFrameWndInfo*)hHandle;
			for (auto& it : pCosmosFrameWndInfo->m_mapControlBarGalaxys)
			{
				IGalaxy* _pGalaxy = it.second;
				IXobj* pXobj = nullptr;
				_pGalaxy->Observe(bstrKey, bstrXml, &pXobj);
			}
		}
		::SysFreeString(bstrXml);
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CMDIChildHelperWnd::OnCosmosMg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (lParam == 19631222 && wParam == 0)
	{
		return (LRESULT)this;
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	if (wParam)
	{
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
		case 19650601:
		{
			::PostMessage(::GetWindow(m_hWnd, GW_CHILD), WM_QUERYAPPPROXY, (WPARAM)m_hWnd, 19650601);
		}
		break;
		default:
			break;
		}
	}
	return l;
}

void CMDIChildHelperWnd::OnFinalMessage(HWND hWnd)
{
	auto it = g_pCosmos->m_pMDIMainWnd->m_mapMDIChildHelperWnd.find(hWnd);
	if (it != g_pCosmos->m_pMDIMainWnd->m_mapMDIChildHelperWnd.end())
		g_pCosmos->m_pMDIMainWnd->m_mapMDIChildHelperWnd.erase(it);
	if (g_pCosmos->m_pMDIMainWnd->m_mapMDIChildHelperWnd.size() == 0)
	{

	}
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CMDTFrameHelperWnd::CMDTFrameHelperWnd(void)
{
}

CMDTFrameHelperWnd::~CMDTFrameHelperWnd(void)
{
}

LRESULT CMDTFrameHelperWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CMDTFrameHelperWnd* pHelperWnd = nullptr;
	auto it = g_pCosmos->m_mapMDTFrameHelperWnd.find(m_hWnd);
	if (it != g_pCosmos->m_mapMDTFrameHelperWnd.end())
	{
		for (auto& itX : g_pCosmos->m_mapMDTFrameHelperWnd)
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
		g_pCosmos->m_pCosmosDelegate->QueryWndInfo(QueryDestroy, pHelperWnd->m_hWnd);
		g_pCosmos->m_hMainWnd = pHelperWnd->m_hWnd;
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

LRESULT CMDTFrameHelperWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CMDTFrameHelperWnd* pHelperWnd = nullptr;
	if (g_pCosmos->m_mapMDTFrameHelperWnd.size() == 1)
	{
		::SendMessage(g_pCosmos->m_hHostBrowserWnd, WM_DESTROY, 0, 0);
	}
	else
	{
		auto it = g_pCosmos->m_mapMDTFrameHelperWnd.find(m_hWnd);
		if (it != g_pCosmos->m_mapMDTFrameHelperWnd.end())
		{
			g_pCosmos->m_mapMDTFrameHelperWnd.erase(it);
		}
	}
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	return l;
}

void CMDTFrameHelperWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CUniverseMDIMain::CUniverseMDIMain(void)
{
	m_pGalaxyCluster = nullptr;
	m_hMDIClient = nullptr;
	m_pDocTemplate = new CComObject<CCosmosDocTemplate>;
}

CUniverseMDIMain::~CUniverseMDIMain(void)
{
	if (m_pDocTemplate)
		delete m_pDocTemplate;
	m_pDocTemplate = nullptr;
}

LRESULT CUniverseMDIMain::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	::InvalidateRect(m_hWnd, nullptr, false);
	return lRes;

}

LRESULT CUniverseMDIMain::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	m_pDocTemplate->SaveXmlData();
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

void CUniverseMDIMain::NavigateKey(CString strKey)
{
	CCosmosDocTemplate* pTemplate = nullptr;
	if (strKey == _T(""))
	{
		strKey = _T("default");
		pTemplate = m_pDocTemplate;
	}
	if (pTemplate == nullptr)
	{
		auto it = m_mapCosmosDocTemplate.find(strKey);
		if (it != m_mapCosmosDocTemplate.end())
		{
			pTemplate = it->second;
		}
	}
	if (pTemplate)
	{
		vector<HWND> m_vec;
		for (auto it : m_pGalaxyCluster->m_mapGalaxy)
		{
			auto it2 = m_mapDesignableWnd.find(it.first);
			if (::IsWindow(it.first))
			{
				IXobj* pXobj = nullptr;
				auto it3 = pTemplate->m_mapConnectedFrame.find(it.first);
				if (it3 == pTemplate->m_mapConnectedFrame.end())
				{
					it.second->m_pCosmosDocTemplate = pTemplate;
					it.second->Observe(CComBSTR(strKey), CComBSTR(""), &pXobj);
					pTemplate->m_mapMainPageNode[it.first] = (CXobj*)pXobj;
				}
			}
			else
			{
				m_vec.push_back(it.first);
			}
		}
		int nSize = m_vec.size();
		if (nSize)
		{
			auto it = m_vec.begin();
			auto it2 = m_pGalaxyCluster->m_mapGalaxy.find(*it);
			if (it2 != m_pGalaxyCluster->m_mapGalaxy.end())
			{
				m_pGalaxyCluster->m_mapGalaxy.erase(it2);
				m_vec.erase(it);
			}
			nSize = m_vec.size();
		}
	}
}

void CUniverseMDIMain::OnCreateDoc(CString strDocData)
{
	if (g_pCosmos->m_pActiveMDIChildWnd)
	{
		CString strXml = _T("");
		CString _strKey = _T("Document_");
		CString _strClientKey = _T("Client_");
		CString strCaption = _T("");
		CTangramXmlParse m_Parse;
		if (strDocData != _T(""))
		{
			if (m_Parse.LoadXml(strDocData))
			{
				strXml = m_Parse.xml();
				_strKey += m_Parse.attr(_T("key"), _T("default"));
				_strClientKey += m_Parse.attr(_T("key"), _T("default"));
				strCaption = m_Parse.attr(_T("caption"), _T(""));
			}
		}
		else
		{
			CString strFile = g_pCosmos->GetDocTemplateXml(_T("Please select New Doc Template:"), g_pCosmos->m_strAppDataPath + _T("Doctemplate\\"), _T(".appxml"));
			if (::PathFileExists(strFile) && m_Parse.LoadFile(strFile))
			{
				CString strKey = m_Parse.attr(_T("key"), _T("default"));
				_strKey += strKey;
				_strClientKey += strKey;
				strXml = m_Parse.xml();
				strCaption = m_Parse.attr(_T("caption"), _T(""));
			}
			else
			{
				_strKey += _T("default");
				_strClientKey += _T("default");
				strXml = _T("<default><cluster><xobj name=\"Start\" =\"nucleus\"/></cluster></default>");
			}
		}
		CUniverseMDIChild* pWnd = g_pCosmos->m_pActiveMDIChildWnd;
		CCosmosDocTemplate* pCosmosDocTemplate = nullptr;
		auto it2 = m_mapCosmosDocTemplate.find(_strKey);
		if (it2 == m_mapCosmosDocTemplate.end())
		{
			pCosmosDocTemplate = new CComObject<CCosmosDocTemplate>;
			pWnd->m_pDocTemplate = pCosmosDocTemplate;
			pCosmosDocTemplate->m_strKey = _strKey;
			pCosmosDocTemplate->m_strClientKey = _strClientKey;
			if (g_pCosmos->m_strTemplatePath.Find(g_pCosmos->m_strAppCommonDocPath) == 0)
			{
				pCosmosDocTemplate->m_strDocTemplatePath = g_pCosmos->m_strAppCommonDocPath + _T("CommonMFCAppTemplate\\MDI\\");
			}

			pCosmosDocTemplate->InitXmlData();
			m_mapCosmosDocTemplate[_strKey] = pCosmosDocTemplate;
		}
		else
			pCosmosDocTemplate = it2->second;
		pCosmosDocTemplate->m_mapCosmosMDIChildWnd[pWnd->m_hWnd] = pWnd;
		g_pCosmos->m_pActiveTemplate = pWnd->m_pDocTemplate = pCosmosDocTemplate;
		pWnd->m_strXml = strXml;
		pWnd->m_strCaption = strCaption;
		::SetWindowText(pWnd->m_hChild, strCaption);
		::SendMessage(pWnd->m_hWnd, WM_COSMOSMSG, (WPARAM)pWnd->m_pDocTemplate, TANGRAM_CONST_OPENFILE);
		::PostMessage(pWnd->m_hWnd, WM_COSMOSMSG, 0, 19922017);
	}
}

void CUniverseMDIMain::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CUniverseMDIMain::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pCosmos->m_strDefaultTemplate == _T("") && g_pCosmos->m_strDefaultTemplate2 == _T(""))
		return DefWindowProc(uMsg, wParam, lParam);

	switch (wParam)
	{
	case ID_FILE_NEW:
	{
		IXobj* pXobj = nullptr;
		CGalaxy* pGalaxy = (CGalaxy*)::SendMessage(m_hMDIClient, WM_HUBBLE_DATA, 0, 1992);
		if (pGalaxy && g_pCosmos->m_strNewDocXml != _T(""))
		{
			pGalaxy->Observe(CComBSTR(L"newdocument"), g_pCosmos->m_strNewDocXml.AllocSysString(), &pXobj);
			g_pCosmos->m_bNewFile = TRUE;
			return 0;
		}
	}
	break;
	//case ID_FILE_OPEN:
	//{
	//	g_pCosmos->m_pActiveMDIChildWnd = nullptr;
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//	LRESULT hr = ::SendMessage(g_pCosmos->m_hCosmosWnd, WM_COSMOSMSG, (WPARAM)m_hWnd, 0);
	//	return hr;
	//}
	//break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CUniverseMDIMain::OnControlBarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HWND hWnd = (HWND)lParam;
	HWND hPWnd = (HWND)wParam;
	::GetWindowText(hPWnd, g_pCosmos->m_szBuffer, MAX_PATH);
	if (::IsChild(m_hMDIClient, hPWnd))
	{
		//for mdi child:
	}
	else if (::IsChild(m_hWnd, hPWnd))
	{
		//for mdi main window
		LRESULT lRes = ::SendMessage(hWnd, WM_HUBBLE_GETNODE, 0, 0);
		if (lRes == 0)
			m_mapDesignableWnd[hWnd] = _T("");
	}
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	return lRes;
}

CUniverseMDIChild::CUniverseMDIChild(void)
{
	m_bNoDocView = false;
	m_strXml = _T("");
	m_strCaption = _T("");
	m_strTemplatePath = _T("");
	if (g_pCosmos->m_strStartupCLRObj != _T(""))
	{
		m_strTemplatePath = g_pCosmos->m_strStartupCLRObj;
		g_pCosmos->m_strStartupCLRObj = _T("");
	}
	m_hChild = nullptr;
	m_pDocTemplate = nullptr;
	g_pCosmos->m_pActiveMDIChildWnd = this;
}

CUniverseMDIChild::~CUniverseMDIChild(void)
{
	if (g_pCosmos->m_pActiveMDIChildWnd && g_pCosmos->m_pActiveMDIChildWnd == this)
		g_pCosmos->m_pActiveMDIChildWnd = nullptr;
}

LRESULT CUniverseMDIChild::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pCosmos->m_nAppType == 1963)
	{
		for (auto it : g_pCosmos->m_mapCosmosMDIChildWnd)
		{
			if (it.first != m_hWnd)
			{
				::SendMessage(it.first, WM_COSMOSMSG, 0, 19650601);
				break;
			}
		}
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CUniverseMDIChild::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	if (g_pCosmos->m_nAppType == 1992 && ::IsIconic(m_hWnd))
		::SendMessage(::GetParent(m_hWnd), WM_MDIICONARRANGE, 0, 0);
	return lRes;
}

LRESULT CUniverseMDIChild::OnDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	RECT* const prcNewWindow = (RECT*)lParam;
	::SetWindowPos(m_hWnd,
		NULL,
		prcNewWindow->left,
		prcNewWindow->top,
		prcNewWindow->right - prcNewWindow->left,
		prcNewWindow->bottom - prcNewWindow->top,
		SWP_NOZORDER | SWP_NOACTIVATE);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CUniverseMDIChild::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (lParam == 19631222)
	{
		if (wParam == 0)
			return (LRESULT)this;
	}
	if (g_pCosmos->m_nAppType == 1965 && g_pCosmos->m_strTemplatePath != _T(""))
	{
		BOOL bNeedInit = false;
		CGalaxyCluster* pGalaxyCluster = nullptr;
		if (g_pCosmos->m_mapWindowPage.size() > 0)
		{
			auto it = g_pCosmos->m_mapWindowPage.find(m_hWnd);
			if (it != g_pCosmos->m_mapWindowPage.end())
			{
				m_strXml = _T("");
				pGalaxyCluster = (CGalaxyCluster*)it->second;
				auto it2 = pGalaxyCluster->m_mapGalaxy.begin();
				while (it2 != pGalaxyCluster->m_mapGalaxy.end())
				{
					CGalaxy* pGalaxy = it2->second;
					pGalaxyCluster->m_mapGalaxy.erase(it2);
					if (pGalaxy)
					{
						RECT rc;
						bool deleteframe = false;
						HWND hwnd = pGalaxy->m_hWnd;
						bNeedInit = true;
						if (hwnd == nullptr)
						{
							hwnd = pGalaxy->m_hHostWnd;
							deleteframe = true;
						}
						if (pGalaxy->m_pWorkXobj)
						{
							::GetWindowRect(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd, &rc);
							if (pGalaxy->m_hWnd)
								pGalaxy->GetParent().ScreenToClient(&rc);
							else
							{
								HWND h = ::GetParent(pGalaxy->m_hHostWnd);
								CWindow m_Wnd;
								m_Wnd.Attach(h);
								m_Wnd.ScreenToClient(&rc);
								m_Wnd.Detach();
							}
							pGalaxy->m_pGalaxyCluster->m_mapXobj.erase(pGalaxy->m_pGalaxyCluster->m_mapXobj.begin(), pGalaxy->m_pGalaxyCluster->m_mapXobj.end());
							pGalaxy->m_pGalaxyCluster = nullptr;
							::DestroyWindow(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd);
							::SetWindowPos(hwnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
						}
						pGalaxy->m_pGalaxyCluster = nullptr;
						if (deleteframe)
							delete pGalaxy;
					}
					auto it = g_pCosmos->m_mapWindowPage.find(m_hWnd);
					if (it == g_pCosmos->m_mapWindowPage.end())
					{
						AtlTrace(_T("*************CGalaxyCluster Deleted:  %x\n****************\n"), m_hWnd);
						pGalaxyCluster = nullptr;
						break;
					}
					it2 = pGalaxyCluster->m_mapGalaxy.begin();
				}
				if (pGalaxyCluster)
				{
					pGalaxyCluster->m_bPageDataLoaded = false;
					pGalaxyCluster->m_strXmlHeader = _T("");
					pGalaxyCluster->m_strMapKey.erase(pGalaxyCluster->m_strMapKey.begin(), pGalaxyCluster->m_strMapKey.end());
					pGalaxyCluster->m_mapXtml.erase(pGalaxyCluster->m_mapXtml.begin(), pGalaxyCluster->m_mapXtml.end());
					pGalaxyCluster->m_mapWnd.erase(pGalaxyCluster->m_mapWnd.begin(), pGalaxyCluster->m_mapWnd.end());
				}
			}
		}
		if (pGalaxyCluster == nullptr)
		{
			IGalaxyCluster* _pGalaxyCluster = nullptr;
			g_pCosmos->CreateGalaxyCluster((__int64)m_hWnd, &_pGalaxyCluster);
			pGalaxyCluster = (CGalaxyCluster*)_pGalaxyCluster;
		}
		IXobj* pXobj = nullptr;
		m_strXml = g_pCosmos->m_strTemplatePath;

		if (m_strXml != _T(""))
		{
			CTangramXmlParse m_Parse;
			bool bLoad = m_Parse.LoadXml(m_strXml);
			if (bLoad == false)
			{
				bLoad = m_Parse.LoadFile(m_strXml);
			}
			else
				g_pCosmos->m_strTemplatePath = g_pCosmos->m_strStartupCLRObj;
			((CGalaxyCluster*)pGalaxyCluster)->UpdateMapKey(m_strXml);
			if (bLoad)
			{
				IGalaxy* pGalaxy = nullptr;
				CString strXml = _T("");
				CTangramXmlParse* pParse = m_Parse.GetChild(_T("client"));
				if (pParse)
					strXml = pParse->xml();
				bool bAddCfg = m_bNoDocView;
				if (g_pCosmos->m_strDefaultTemplate2 != _T(""))
					bAddCfg = false;
				pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR(_T("default")), CComBSTR(_T("")), bAddCfg, &pXobj);
				if (pXobj)
				{
					CXobj* _pXobj = (CXobj*)pXobj;
					m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
				}
				for (auto it : m_mapDesignableWnd)
				{
					CString strXml = _T("");
					CTangramXmlParse* pParse = m_Parse.GetChild(it.second);
					if (pParse)
						strXml = pParse->xml();
					IXobj* pXobj = nullptr;
					pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(_T("")), bAddCfg, &pXobj);
				}
			}
		}
		g_pCosmos->m_strTemplatePath = _T("");

		return DefWindowProc(uMsg, wParam, lParam);
	}
	if (lParam == 19922017)
	{
		BOOL bNeedInit = false;
		IGalaxyCluster* pGalaxyCluster = nullptr;
		IXobj* pXobj = nullptr;
		g_pCosmos->CreateGalaxyCluster((__int64)m_hWnd, &pGalaxyCluster);
		if (pGalaxyCluster)
		{
			((CGalaxyCluster*)pGalaxyCluster)->m_bDoc = true;
			if ((::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD))
			{
				if (m_strXml != _T(""))
				{
					if (::PathFileExists(m_strXml))
						g_pCosmos->m_strTemplatePath = m_strXml;
					((CGalaxyCluster*)pGalaxyCluster)->UpdateMapKey(m_strXml);
				}
				pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR("default"), CComBSTR(L""), false, &pXobj);
				if (pXobj)
				{
					CXobj* _pXobj = (CXobj*)pXobj;
					m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
				}

				if (g_pCosmos->m_pMDIMainWnd && g_pCosmos->m_pMDIMainWnd->m_pGalaxyCluster && m_pDocTemplate && m_pDocTemplate->m_strKey != _T(""))
				{
					g_pCosmos->m_pMDIMainWnd->NavigateKey(m_pDocTemplate->m_strKey);
				}
			}
			else
			{
				if (g_pCosmos->m_nAppType == 1965 && m_strXml != _T(""))
				{
					CGalaxyCluster* pGalaxyCluster = nullptr;
					TRACE(_T("%s\n"), g_pCosmos->m_strTemplatePath);
					HWND hPage = (HWND)m_hWnd;
					auto it = g_pCosmos->m_mapWindowPage.find(hPage);
					if (it != g_pCosmos->m_mapWindowPage.end())
					{
						pGalaxyCluster = (CGalaxyCluster*)it->second;
						auto it2 = pGalaxyCluster->m_mapGalaxy.begin();
						while (it2 != pGalaxyCluster->m_mapGalaxy.end())
						{
							CGalaxy* pGalaxy = it2->second;
							pGalaxyCluster->m_mapGalaxy.erase(it2);
							if (pGalaxy)
							{
								RECT rc;
								bool deleteframe = false;
								HWND hwnd = pGalaxy->m_hWnd;
								bNeedInit = true;
								if (hwnd == nullptr)
								{
									hwnd = pGalaxy->m_hHostWnd;
									deleteframe = true;
								}
								if (pGalaxy->m_pWorkXobj)
								{
									::GetWindowRect(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd, &rc);
									if (pGalaxy->m_hWnd)
										pGalaxy->GetParent().ScreenToClient(&rc);
									else
									{
										HWND h = ::GetParent(pGalaxy->m_hHostWnd);
										CWindow m_Wnd;
										m_Wnd.Attach(h);
										m_Wnd.ScreenToClient(&rc);
										m_Wnd.Detach();
									}
									pGalaxy->m_pGalaxyCluster->m_mapXobj.erase(pGalaxy->m_pGalaxyCluster->m_mapXobj.begin(), pGalaxy->m_pGalaxyCluster->m_mapXobj.end());
									pGalaxy->m_pGalaxyCluster = nullptr;
									::DestroyWindow(pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd);
									::SetWindowPos(hwnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
								}
								pGalaxy->m_pGalaxyCluster = nullptr;
								if (deleteframe)
									delete pGalaxy;
							}
							auto it = g_pCosmos->m_mapWindowPage.find(m_hWnd);
							if (it == g_pCosmos->m_mapWindowPage.end())
							{
								AtlTrace(_T("*************CGalaxyCluster Deleted:  %x\n****************\n"), m_hWnd);
								pGalaxyCluster = nullptr;
								break;
							}
							it2 = pGalaxyCluster->m_mapGalaxy.begin();
						}
					}
					if (pGalaxyCluster)
					{
						pGalaxyCluster->m_bPageDataLoaded = false;
						pGalaxyCluster->m_strXmlHeader = _T("");
						pGalaxyCluster->m_strMapKey.erase(pGalaxyCluster->m_strMapKey.begin(), pGalaxyCluster->m_strMapKey.end());
						pGalaxyCluster->m_mapXtml.erase(pGalaxyCluster->m_mapXtml.begin(), pGalaxyCluster->m_mapXtml.end());
						pGalaxyCluster->m_mapWnd.erase(pGalaxyCluster->m_mapWnd.begin(), pGalaxyCluster->m_mapWnd.end());
					}
					if (m_strXml != _T(""))
					{
						if (::PathFileExists(m_strXml))
							g_pCosmos->m_strTemplatePath = m_strXml;
						((CGalaxyCluster*)pGalaxyCluster)->UpdateMapKey(m_strXml);
					}
					IXobj* pXobj = nullptr;
					if (m_strXml != _T(""))
					{
						IGalaxy* pGalaxy = nullptr;
						pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR(_T("default")), CComBSTR(L""), false, &pXobj);
						if (pXobj)
						{
							CXobj* _pXobj = (CXobj*)pXobj;
							m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
						}
						for (auto it : m_mapDesignableWnd)
						{
							IXobj* pXobj = nullptr;
							pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(L""), false, &pXobj);
						}
					}
					return DefWindowProc(uMsg, wParam, lParam);
				}
				if (m_strXml != _T(""))
				{
					if (::PathFileExists(m_strXml))
						g_pCosmos->m_strTemplatePath = m_strXml;
					((CGalaxyCluster*)pGalaxyCluster)->UpdateMapKey(m_strXml);
				}
				CString strTemplatePath = g_pCosmos->m_strTemplatePath;
				if (m_strXml == _T("") && strTemplatePath != _T(""))
				{
					m_strXml = strTemplatePath;
					g_pCosmos->m_strTemplatePath = _T("");
				}
				if (m_strXml != _T(""))
				{
					CTangramXmlParse m_Parse;
					if (m_Parse.LoadXml(m_strXml))
					{
						CString strXml = _T("");
						CTangramXmlParse* pParse = (CTangramXmlParse*)m_Parse.GetChild(_T("client"));
						if (pParse)
							strXml = pParse->xml();
						pParse = (CTangramXmlParse*)m_Parse.GetChild(_T("pagedata"));
						if (pParse)
						{
							int nCount = pParse->GetCount();
							if (nCount)
							{
								for (int i = 0; i < nCount; i++)
								{
									CTangramXmlParse* pChild = pParse->GetChild(i);
									CGalaxyCluster* _pGalaxyCluster = (CGalaxyCluster*)pGalaxyCluster;
									CString strName = pChild->name();
									_pGalaxyCluster->m_mapXtml[strName] = pChild->xml();
									CString strPath = m_strTemplatePath;
									int nPos = strPath.ReverseFind('\\');
									CString strFileName = strPath.Mid(nPos + 1);
									int nPos1 = strFileName.ReverseFind('.');
									strFileName = strFileName.Left(nPos1);
									strPath = strPath.Left(nPos + 1) + strFileName + _T("_") + strName + _T(".bmp");
									if (::PathFileExists(strPath))
									{
										_pGalaxyCluster->m_mapXtml[strName + _T("_imagePath")] = strPath;
										CString strData = g_pCosmos->EncodeFileToBase64(strPath);
										_pGalaxyCluster->m_mapXtml[strName + _T("_image")] = strData;
									}
								}
							}
						}
						IGalaxy* pGalaxy = nullptr;
						pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR(_T("default")), CComBSTR(strXml), false, &pXobj);
						if (pXobj)
						{
							CXobj* _pXobj = (CXobj*)pXobj;
							m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
						}
						for (auto it : m_mapDesignableWnd)
						{
							CString strXml = _T("");
							CTangramXmlParse* pParse = m_Parse.GetChild(it.second);
							if (pParse)
								strXml = m_Parse.GetChild(it.second)->xml();
							IXobj* pXobj = nullptr;
							pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(strXml), false, &pXobj);
						}
					}
				}
				else
				{
					if (g_pCosmos->m_nAppType == 1963 && m_bNoDocView)
					{
						if (g_pCosmos->m_strDefaultTemplate2 != _T(""))//Fist Frame for APP_TYPE_MTLD Application
						{
							pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"defaultframe"), CComBSTR(_T("defaultframe")), CComBSTR(""), true, &pXobj);
							if (pXobj)
							{
								CXobj* _pXobj = (CXobj*)pXobj;
								m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
							}
						}
						else
						{
							CString strPath = g_pCosmos->GetDocTemplateXml(_T("Please select Document Template:"), g_pCosmos->m_strAppDataPath + _T("Doctemplate\\"), _T(".AppXml"));
							if (::PathFileExists(strPath))
							{
								CTangramXmlParse m_Parse;
								if (m_Parse.LoadFile(strPath))
								{
									pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR(_T("default")), CComBSTR(m_Parse.xml()), false, &pXobj);
									if (pXobj)
									{
										CXobj* _pXobj = (CXobj*)pXobj;
										m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
									}
								}
							}
						}
						return DefWindowProc(uMsg, wParam, lParam);
					}
					bool bAddCfg = m_bNoDocView;
					if (g_pCosmos->m_strDefaultTemplate2 != _T(""))
						bAddCfg = false;
					CString strKey = _T("default");
					if (g_pCosmos->m_nAppType == 1965 && m_bNoDocView)// First View for SDI Application 
					{
						strKey = _T("defaultclient");
						pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(strKey), CComBSTR(strKey), CComBSTR(L""), true, &pXobj);
						if (pXobj)
						{
							CXobj* _pXobj = (CXobj*)pXobj;
							m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
						}
						for (auto it : m_mapDesignableWnd)
						{
							IXobj* pXobj = nullptr;
							pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(L""), bAddCfg, &pXobj);
						}
					}
					else
					{
						OnCreateDoc(_T(""));
					}
					//	pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(strKey), CComBSTR(strKey), CComBSTR(L""), bAddCfg, &pXobj);
					//if (pXobj)
					//{
					//	CXobj* _pXobj = (CXobj*)pXobj;
					//	m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
					//}
					//for (auto it : m_mapDesignableWnd)
					//{
					//	IXobj* pXobj = nullptr;
					//	pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(L""), bAddCfg, &pXobj);
					//}
				}
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CUniverseMDIChild::OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pDocTemplate == nullptr)
		return DefWindowProc(uMsg, wParam, lParam);
	if (lParam != wParam)
	{
		HWND hActivedWnd = (HWND)lParam;
		CString strKey = _T("");
		CUniverseMDIChild* pWnd = (CUniverseMDIChild*)::SendMessage(hActivedWnd, WM_COSMOSMSG, 0, 19631222);
		if (pWnd && pWnd->m_pDocTemplate)
		{
			strKey = pWnd->m_pDocTemplate->m_strKey;
			g_pCosmos->m_pActiveTemplate = pWnd->m_pDocTemplate;
		}
		g_pCosmos->m_pMDIMainWnd->NavigateKey(strKey);
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

void CUniverseMDIChild::OnCreateDoc(CString strDocData)
{
	m_strXml = strDocData;
	if (m_strXml == _T(""))
	{
		CString strPath = g_pCosmos->m_strAppDataPath + _T("DocTemplate\\Normal\\default.xml");
		bool bDefaultExists = ::PathFileExists(strPath);
		if (bDefaultExists == false)
		{
			CString strDir = g_pCosmos->m_pMDIMainWnd ? _T("MDI") : _T("SDI");
			strPath = g_pCosmos->m_strAppCommonDocPath + _T("CommonMFCAppTemplate\\") + strDir + _T("\\DocTemplate\\default.xml");
			bDefaultExists = ::PathFileExists(strPath);
		}
		if (bDefaultExists)
		{
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadFile(strPath))
			{
				m_strTemplatePath = strPath;
				m_strXml = m_Parse.xml();
			}
			g_pCosmos->m_strTemplatePath = strPath;
		}
		else
		{
			bool bAddCfg = m_bNoDocView;
			if (g_pCosmos->m_strDefaultTemplate2 != _T(""))
				bAddCfg = false;
			CString strKey = _T("default");
			IGalaxyCluster* pGalaxyCluster = nullptr;
			IXobj* pXobj = nullptr;
			g_pCosmos->CreateGalaxyCluster((__int64)m_hWnd, &pGalaxyCluster);
			pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(strKey), CComBSTR(strKey), CComBSTR(L""), bAddCfg, &pXobj);
			if (pXobj)
			{
				CXobj* _pXobj = (CXobj*)pXobj;
				m_pGalaxy = _pXobj->m_pXobjShareData->m_pGalaxy;
			}
			for (auto it : m_mapDesignableWnd)
			{
				IXobj* pXobj = nullptr;
				pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(L""), bAddCfg, &pXobj);
			}
			return;
		}
	}
	::SendMessage(m_hWnd, WM_COSMOSMSG, 0, 19922017);
}

LRESULT CUniverseMDIChild::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (LOWORD(wParam) != WA_INACTIVE)
	{
		g_pCosmos->m_pActiveMDIChildWnd = this;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CUniverseMDIChild::OnControlBarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HWND hWnd = (HWND)lParam;
	HWND hPWnd = (HWND)wParam;
	::GetWindowText(hPWnd, g_pCosmos->m_szBuffer, MAX_PATH);
	CString strText = CString(g_pCosmos->m_szBuffer);
	strText.Replace(_T(" "), _T("_"));
	LRESULT lRes = ::SendMessage(hWnd, WM_HUBBLE_GETNODE, 0, 0);
	if (lRes == 0)
	{
		m_mapDesignableWnd[hWnd] = strText;
		auto it = g_pCosmos->m_mapWindowPage.find(m_hWnd);
		if (it != g_pCosmos->m_mapWindowPage.end())
		{
			IXobj* pXobj = nullptr;
			bool bAddCfg = m_bNoDocView;
			if (g_pCosmos->m_strDefaultTemplate2 != _T(""))
				bAddCfg = false;
			it->second->CreateGalaxyWithDefaultNode((__int64)hWnd, CComBSTR(strText), CComBSTR(_T("default")), CComBSTR(L""), bAddCfg, &pXobj);
			CGalaxy* pGalaxy = (CGalaxy*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 1992);
			if (pGalaxy)
			{
				pGalaxy->m_nGalaxyType = CtrlBarGalaxy;
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CUniverseMDIChild::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pCosmos->m_strDefaultTemplate == _T("") && g_pCosmos->m_strDefaultTemplate2 == _T(""))
		return DefWindowProc(uMsg, wParam, lParam);

	switch (wParam)
	{
	case ID_FILE_NEW:
	case ID_FILE_NEW_FRAME:
	{
		if (g_pCosmos->m_hTemplateWnd == nullptr)
		{
			if (g_pCosmos->m_pDesignerGalaxyCluster == nullptr)
			{
				auto it = g_pCosmos->m_mapWindowPage.find(g_pCosmos->m_hHostWnd);
				if (it != g_pCosmos->m_mapWindowPage.end())
					g_pCosmos->m_pDesignerGalaxyCluster = (CGalaxyCluster*)it->second;
				else
				{
					g_pCosmos->m_pDesignerGalaxyCluster = new CComObject<CGalaxyCluster>();
					g_pCosmos->m_pDesignerGalaxyCluster->m_hWnd = g_pCosmos->m_hHostWnd;
					g_pCosmos->m_mapWindowPage[g_pCosmos->m_hHostWnd] = g_pCosmos->m_pDesignerGalaxyCluster;
				}
			}
			g_pCosmos->m_hTemplateWnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, g_pCosmos->m_hHostWnd, 0, theApp.m_hInstance, NULL);
			g_pCosmos->m_hTemplateChildWnd = ::CreateWindowEx(NULL, _T("Cosmos Xobj Class"), _T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, g_pCosmos->m_hTemplateWnd, 0, theApp.m_hInstance, NULL);
			IGalaxy* pGalaxy = nullptr;
			g_pCosmos->m_pDesignerGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((__int64)g_pCosmos->m_hTemplateChildWnd), CComBSTR(L"doctemplate"), &pGalaxy);
			g_pCosmos->m_pDocTemplateFrame = (CGalaxy*)pGalaxy;
		}
		if (g_pCosmos->m_pDocTemplateFrame)
		{
			if (m_hChild)
			{
				RECT rc;
				CUniverseMDIChild* pWnd = (CUniverseMDIChild*)::SendMessage(::GetParent(g_pCosmos->m_hTemplateWnd), WM_COSMOSMSG, 0, 19631222);
				if (pWnd)
				{
					::GetWindowRect(g_pCosmos->m_pDocTemplateFrame->m_pWorkXobj->m_pHostWnd->m_hWnd, &rc);
					pWnd->ScreenToClient(&rc);
					::ShowWindow(pWnd->m_pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd, SW_SHOW);
					::SetWindowLongPtr(pWnd->m_hChild, GWLP_ID, AFX_IDW_PANE_FIRST);
					::SetWindowPos(pWnd->m_hChild, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED);
				}
				::SetParent(g_pCosmos->m_hTemplateWnd, m_hWnd);
				::GetWindowRect(m_pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd, &rc);
				ScreenToClient(&rc);
				m_pGalaxy->m_pWorkXobj->m_pHostWnd->ShowWindow(SW_HIDE);
				::SetWindowPos(m_hChild, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED);
				::SetWindowLongPtr(m_hChild, GWLP_ID, 19631222);
				::SetWindowLongPtr(g_pCosmos->m_hTemplateWnd, GWLP_ID, AFX_IDW_PANE_FIRST);
				::ShowWindow(g_pCosmos->m_hTemplateWnd, SW_SHOW);
				::SetWindowPos(g_pCosmos->m_hTemplateWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED);
			}
			IXobj* pXobj = nullptr;
			g_pCosmos->m_pDocTemplateFrame->Observe(CComBSTR(L"newdocument"), g_pCosmos->m_strNewDocXml.AllocSysString(), &pXobj);
			g_pCosmos->m_bNewFile = TRUE;
			return 0;
		}
	}
	break;
	//case ID_FILE_OPEN:
	//{
	//	LRESULT hr = ::SendMessage(g_pCosmos->m_hCosmosWnd, WM_COSMOSMSG, (WPARAM)m_hWnd, 0);
	//	return hr;
	//}
	//break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CUniverseMDIChild::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pDocTemplate)
	{
		auto it = m_pDocTemplate->m_mapCosmosMDIChildWnd.find(m_hWnd);
		if (it != m_pDocTemplate->m_mapCosmosMDIChildWnd.end())
		{
			m_pDocTemplate->m_mapCosmosMDIChildWnd.erase(it);
			if (m_pDocTemplate->m_mapCosmosMDIChildWnd.size() == 0)
			{
				auto it = g_pCosmos->m_pMDIMainWnd->m_mapCosmosDocTemplate.find(m_pDocTemplate->m_strKey);
				if (it != g_pCosmos->m_pMDIMainWnd->m_mapCosmosDocTemplate.end())
				{
					g_pCosmos->m_pMDIMainWnd->m_mapCosmosDocTemplate.erase(it);
					m_pDocTemplate->SaveXmlData();
					if (g_pCosmos->m_pMDIMainWnd->m_mapCosmosDocTemplate.size() == 0)
					{
						g_pCosmos->m_pActiveTemplate = nullptr;
					}
				}
			}
		}
	}
	else
	{
		if (::IsChild(m_hWnd, g_pCosmos->m_hTemplateWnd))
		{
			::ShowWindow(g_pCosmos->m_hTemplateWnd, SW_HIDE);
			::SetParent(g_pCosmos->m_hTemplateWnd, g_pCosmos->m_hHostWnd);
		}
		auto it = g_pCosmos->m_mapCosmosMDIChildWnd.find(m_hWnd);
		if (it != g_pCosmos->m_mapCosmosMDIChildWnd.end())
		{
			g_pCosmos->m_mapCosmosMDIChildWnd.erase(it);
		}
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

void CUniverseMDIChild::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
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
	if (g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_pCLRProxy->OnWinFormActivate(m_hWnd, LOWORD(wParam));
	}
	if (LOWORD(wParam) != WA_INACTIVE) {
		g_pCosmos->m_pActiveWinFormWnd = this;
	}
	else
	{
		HWND hMenuWnd = g_pCosmos->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
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
			if (m_pOwnerHtmlWnd && m_pOwnerHtmlWnd->m_pGalaxy)
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
	case 20190214:
		return (LRESULT)this;
		break;
	case 20201225:
		::SetActiveWindow(m_hWnd);
		::SetForegroundWindow(m_hWnd);
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

LRESULT CWinForm::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	auto it = g_pCosmos->m_mapMainForm.find(m_hWnd);
	if (it != g_pCosmos->m_mapMainForm.end())
	{
		g_pCosmos->m_mapMainForm.erase(it);
		if (m_hWnd == g_pCosmos->m_hMainWnd)
		{
			if (g_pCosmos->m_mapMainForm.size())
			{
				it = g_pCosmos->m_mapMainForm.begin();
				if (it != g_pCosmos->m_mapMainForm.end())
					g_pCosmos->m_hMainWnd = it->first;
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20201114:
	{
		if(m_bReady)
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
			auto it = g_pCosmos->m_mapHtmlWnd.find(hWnd);
			if (it != g_pCosmos->m_mapHtmlWnd.end())
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
					auto it = g_pCosmos->m_mapHtmlWnd.find(hWnd);
					if (it != g_pCosmos->m_mapHtmlWnd.end())
					{
						if (m_pOwnerHtmlWnd == nullptr)
							m_pOwnerHtmlWnd = (CWebPage*)it->second;
						pSession->InsertString(_T("msgID"), _T("MdiWinForm_ActiveMdiChild"));
						pSession->Insertint64(_T("active_mdichildhandle"), (__int64)m_hWnd);
						pSession->InsertString(_T("active_mdichildkey"), m_strKey);
						pSession->SendMessage();
						if (!m_bReady)
						{
							::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20201114);
						}
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
	case 20200419:
	{
		auto it = g_pCosmos->m_mapMainForm.find(m_hWnd);
		if (it == g_pCosmos->m_mapMainForm.end())
			g_pCosmos->m_mapMainForm[m_hWnd] = this;
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

LRESULT CWinForm::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_pActiveHtmlWnd = nullptr;
	g_pCosmos->m_pActiveWinFormWnd = this;
	::BringWindowToTop(m_hWnd);
		
	//if ((long)(g_pCosmos->m_pUniverseAppProxy) > 1)
	//{
	//	HWND hMenuWnd = g_pCosmos->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
	//	if (::IsWindow(hMenuWnd))
	//		::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	//}

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
				strXml.Format(_T("<mdiclient><cluster><xobj name=\"mdiclient\" objid=\"%s\" /></cluster></mdiclient>"), m_strBKID);
			else
			{
				m_strBKID = m_strBKID.Mid(nPos + 1);
				strXml.Format(_T("<mdiclient><cluster><xobj name='mdiclient' url='%s' /></cluster></mdiclient>"), m_strBKID);
			}
			IXobj* pXobj = nullptr;
			pGalaxy->Observe(CComBSTR(L"default"), strXml.AllocSysString(), &pXobj);
			auto it = pGalaxyCluster->m_mapGalaxy.find((HWND)wParam);
			if (it != pGalaxyCluster->m_mapGalaxy.end())
			{
				it->second->m_pBKWnd = m_pBKWnd;
			}
			pGalaxyCluster->m_pBKGalaxy = m_pBKWnd->m_pGalaxy = (CGalaxy*)pGalaxy;
			g_pCosmos->m_mapBKFrame[m_pBKWnd->m_hChild] = m_pBKWnd->m_pGalaxy;
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
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnFormCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_hFormNodeWnd = nullptr;
	return DefWindowProc(uMsg, wParam, lParam);
}

void CWinForm::OnFinalMessage(HWND hWnd)
{
	auto it = g_pCosmos->m_mapFormWebPage.find(hWnd);
	if (it != g_pCosmos->m_mapFormWebPage.end())
		g_pCosmos->m_mapFormWebPage.erase(it);
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CCosmosDocWnd::CCosmosDocWnd(void)
{
	m_hView = nullptr;
	m_strKey = _T("");
	m_strWndID = _T("default");
	m_pGalaxy = nullptr;
	m_pDocFrame = nullptr;
	m_pParentPage = nullptr;
	auto it = g_pCosmos->m_mapValInfo.find(_T("newmdtframe"));
	if (it != g_pCosmos->m_mapValInfo.end())
	{
		m_strKey = OLE2T(it->second.bstrVal);
		::VariantClear(&it->second);
		g_pCosmos->m_mapValInfo.erase(it);
	}
}

CCosmosDocWnd::~CCosmosDocWnd(void)
{
	::SendMessage(g_pCosmos->m_hCosmosWnd, WM_COSMOSMSG, 0, 10001000);
}

LRESULT CCosmosDocWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pDocFrame && m_pDocFrame->m_pCosmosDoc)
	{
		CString strPath = g_pCosmos->m_strAppDataPath + _T("default.tangramdoc");
		if (m_pDocFrame->m_pCosmosDoc->m_strPath.CompareNoCase(strPath) == 0)
		{
			::SendMessage(m_hWnd, WM_QUERYAPPPROXY, 0, TANGRAM_CONST_NEWDOC);
		}
		m_pDocFrame->m_pCosmosDoc->m_pAppProxy->m_strClosingFrameID = m_strKey;
		auto it = g_pCosmos->m_mapMDTFrame.find(m_hWnd);
		if (it != g_pCosmos->m_mapMDTFrame.end())
		{
			if (m_pDocFrame->m_pCosmosDoc->m_pAppProxy != g_pCosmos->m_pUniverseAppProxy)
			{
				g_pCosmos->m_pUniverseAppProxy->OnDestroyMainFrame(m_strKey, g_pCosmos->m_mapMDTFrame.size(), 1);
			}
			m_pDocFrame->m_pCosmosDoc->m_pAppProxy->OnDestroyMainFrame(m_strKey, g_pCosmos->m_mapMDTFrame.size(), 1);
		}
	}
	TRACE(_T("Close Tangram Document :%x\n"), this);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCosmosDocWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam)
	{
	case ID_FILE_NEW:
	case ID_FILE_NEW_FRAME:
	{
		if (m_pDocFrame)
		{
			IXobj* pXobj = nullptr;
			if (m_pDocFrame->m_pHostGalaxy)
			{
				m_pDocFrame->m_pHostGalaxy->Observe(CComBSTR(L"newdocument"), g_pCosmos->m_strNewDocXml.AllocSysString(), &pXobj);
				g_pCosmos->m_bNewFile = TRUE;
				return 0;
			}
		}
	}
	break;
	//case ID_FILE_OPEN:
	//{
	//	if (g_pCosmos)
	//	{
	//		return ::SendMessage(g_pCosmos->m_hCosmosWnd, WM_COSMOSMSG, (WPARAM)m_hWnd, 0);
	//	}
	//	return 0;
	//}

	break;
	case ID_FILE_SAVE:
	{
		if (m_pDocFrame)
		{
			m_pDocFrame->m_pCosmosDoc->m_pDocProxy->SaveDoc();
			return 0;
		}
		return 0;

	}
	case ID_FILE_SAVE_AS:
	{
		if (m_pDocFrame)
		{
			CString strID = m_pDocFrame->m_pCosmosDoc->m_strDocID;
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCosmosDocWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	auto it = g_pCosmos->m_mapMDTFrame.find(m_hWnd);
	if (it != g_pCosmos->m_mapMDTFrame.end())
	{
		g_pCosmos->m_mapMDTFrame.erase(it);
		if (m_pDocFrame && m_pDocFrame->m_pCosmosDoc)
		{
			m_pDocFrame->m_pCosmosDoc->m_pAppProxy->m_strClosingFrameID = m_strKey;
			CString strKey = m_strKey + _T("-ondestroy");
			m_pDocFrame->m_pCosmosDoc->m_pAppProxy->m_hMainWnd = nullptr;
			if (m_pDocFrame->m_pCosmosDoc->m_pAppProxy != g_pCosmos->m_pUniverseAppProxy)
			{
				g_pCosmos->m_pUniverseAppProxy->OnDestroyMainFrame(strKey, g_pCosmos->m_mapMDTFrame.size(), 1);
			}
			m_pDocFrame->m_pCosmosDoc->m_pAppProxy->OnDestroyMainFrame(strKey, g_pCosmos->m_mapMDTFrame.size(), 1);
		}
	}

	if (m_pDocFrame)
	{
		if (m_pDocFrame->m_pCosmosDoc->m_pActiveWnd == this)
			m_pDocFrame->m_pCosmosDoc->m_pActiveWnd = nullptr;
		auto it = m_pDocFrame->m_mapWnd.find(m_hWnd);
		if (it != m_pDocFrame->m_mapWnd.end())
			m_pDocFrame->m_mapWnd.erase(it);
		if (m_pDocFrame->m_mapWnd.size() == 0)
		{
			delete m_pDocFrame;
			m_pDocFrame = nullptr;
		}
		else
		{
			m_pDocFrame->m_pCurrentWnd = m_pDocFrame->m_mapWnd.begin()->second;
			m_pDocFrame->m_pHostGalaxy->ModifyHost((LONGLONG)m_pDocFrame->m_pCurrentWnd->m_hView);
		}
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

void CCosmosDocWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
	if (g_pCosmos->m_bEclipse == false && g_pCosmos->m_pMDIMainWnd == nullptr && g_pCosmos->m_mapMDTFrame.size() == 0)
	{
		if (g_pCosmos->m_mapBrowserWnd.size() == 1)
		{
			g_pCosmos->m_bChromeNeedClosed = true;
			for (auto it : g_pCosmos->m_mapBrowserWnd)
			{
				CBrowser* pWnd = (CBrowser*)it.second;
				pWnd->PostMessageW(WM_CLOSE, 0, 0);
			}
		}
	}
}

LRESULT CCosmosDocWnd::OnCtrlBarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HWND hWnd = (HWND)wParam;
	HWND hClient = ::GetDlgItem(hWnd, AFX_IDW_PANE_FIRST);
	if (::IsWindow(hClient))
	{
		::GetWindowText(hClient, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strText = CString(g_pCosmos->m_szBuffer);
		CGalaxyCluster* pGalaxyCluster = nullptr;
		IXobj* pXobj = nullptr;
		HWND hPWnd = (HWND)lParam;
		auto it = g_pCosmos->m_mapWindowPage.find(hPWnd);
		if (it == g_pCosmos->m_mapWindowPage.end())
		{
			pGalaxyCluster = new CComObject<CGalaxyCluster>;
			pGalaxyCluster->m_hWnd = hPWnd;
			g_pCosmos->m_mapWindowPage[hPWnd] = pGalaxyCluster;
			for (auto it : g_pCosmos->m_mapCosmosAppProxy)
			{
				CGalaxyClusterProxy* pCosmosProxy = it.second->OnGalaxyClusterCreated(pGalaxyCluster);
				if (pCosmosProxy)
					pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pCosmosProxy;
			}
		}
		else
			pGalaxyCluster = (CGalaxyCluster*)it->second;

		auto it2 = m_mapCtrlBar.find(strText);
		if (it2 == m_mapCtrlBar.end())
		{
			m_mapCtrlBar[strText] = hClient;
			IGalaxy* pGalaxy = nullptr;
			pGalaxyCluster->CreateGalaxy(CComVariant((LONGLONG)0), CComVariant((LONGLONG)hClient), CComBSTR(strText), &pGalaxy);
			if (pGalaxy)
			{
				((CGalaxy*)pGalaxy)->m_pDoc = m_pDocFrame->m_pCosmosDoc;
				CString strXml = _T("");
				auto it = m_pDocFrame->m_pCosmosDoc->m_mapWndScript.find(strText);
				if (it != m_pDocFrame->m_pCosmosDoc->m_mapWndScript.end())
				{
					strXml = it->second;
				}
				else
				{
					CString strPath = m_pDocFrame->m_pCosmosDoc->m_strPath;
					if (strPath == _T(""))
					{
						CString s = _T("");
						s.Format(_T("Do you want to create Control Bar \"%s\" from exists Common Template?"), strText);
						CString strCaption = s;
						s.Format(_T("Creating Control Bar: \"%s\""), strText);
						if (::MessageBox(::GetActiveWindow(), strCaption, s, MB_YESNO) == IDYES)
						{
							CString _strPath = g_pCosmos->m_strAppCommonDocPath + _T("CommonCtrlBar\\");
							s.Format(_T("Please select Control Bar \"%s\" Template"), strText);
							strXml = g_pCosmos->GetDocTemplateXml(s, _strPath, _T(".AppXml"));
						}
					}
				}
				pGalaxy->Observe(CComBSTR(strText), CComBSTR(strXml), &pXobj);
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCosmosDocWnd::OnCosmosGetXml(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strGalaxyName = (LPCTSTR)wParam;
	strGalaxyName += _T("_");
	strGalaxyName += (LPCTSTR)lParam;
	auto it = this->m_pDocFrame->m_pCosmosDoc->m_mapWndScript.find(strGalaxyName);
	if (it != m_pDocFrame->m_pCosmosDoc->m_mapWndScript.end())
		return (LRESULT)LPCTSTR(it->second);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CCosmosDocWnd::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 19631222:
		return 0;
	case 1963://for Uer Control
	{
		CXobjHelper* pWnd = (CXobjHelper*)wParam;
		if (pWnd)
		{
			CGalaxyCluster* pGalaxyCluster = pWnd->m_pXobj->m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster;
			for (auto it : pWnd->m_mapDockCtrl)
			{
				TRACE(_T("DockCtrlName:%s\n"), it.first);
				TRACE(_T("DockCtrlHandle:%x\n"), it.second);
				CString strName = pWnd->m_pXobj->m_strName;
				strName += CComBSTR(L"_");
				strName += it.first;
				CString strKey = strName;
				strKey += CComBSTR(L"_default");
				CString strXml = _T("");
				auto it2 = m_pDocFrame->m_pCosmosDoc->m_mapWndScript.find(strKey);
				if (it2 != m_pDocFrame->m_pCosmosDoc->m_mapWndScript.end())
					strXml = it2->second;

				IGalaxy* pGalaxy = nullptr;
				pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)it.second), strName.AllocSysString(), &pGalaxy);
				CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
				_pGalaxy->m_pParentXobj = pWnd->m_pXobj;
				IXobj* pXobj = nullptr;
				pGalaxy->Observe(CComBSTR(L"default"), CComBSTR(strXml), &pXobj);
				if (pXobj)
				{
					_pGalaxy->m_nGalaxyType = CtrlBarGalaxy;
					pWnd->m_pXobj->m_mapSubFrame[strName] = (CGalaxy*)pGalaxy;
				}
			}
		}
		return 0;
	}
	}
	switch (wParam)
	{
	case 2016:
		return (LRESULT)this;
	case TANGRAM_CONST_PANE_FIRST:
		return DefWindowProc(uMsg, wParam, lParam);
	}

	g_pCosmos->m_pActiveDocWnd = this;
	LRESULT lRes = 0;
	HWND hPWnd = (HWND)lParam;
	if (::IsWindow(hPWnd))
		lRes = ::SendMessage(::GetParent(m_hWnd), WM_HUBBLE_DATA, 0, 1992);
	if (lRes)
	{
		m_pGalaxy = (CGalaxy*)lRes;
		IXobj* pXobj = nullptr;
		m_pGalaxy->Observe(m_pDocFrame->m_pCosmosDoc->m_strDocID.AllocSysString(), CComBSTR(_T("")), &pXobj);
		if (pXobj)
		{
			pXobj->put_SaveToConfigFile(true);
		}
	}
	if (m_pDocFrame)
	{
		IGalaxyCluster* pGalaxyCluster = nullptr;
		CComBSTR bstrTemplate(L"");
		auto it = m_pDocFrame->m_pCosmosDoc->m_mapWndScript.find(m_strWndID);
		if (it != m_pDocFrame->m_pCosmosDoc->m_mapWndScript.end())
			bstrTemplate = it->second.AllocSysString();
		//else
		//{
		//	if (::GetParent(m_hWnd) == NULL)
		//	{
		//		CString s = _T("");
		//		s.Format(_T("Do you want to create Tangram Document: \"%s\" from exists Common Document Template?"), m_pDocFrame->m_pCosmosDoc->m_strDocID);
		//		CString strCaption = s;
		//		s.Format(_T("Creating Tangram Document: \"%s\""), m_pDocFrame->m_pCosmosDoc->m_strDocID);
		//		if (::MessageBox(::GetActiveWindow(), strCaption, s, MB_YESNO) == IDYES)
		//		{
		//			CString _strPath = g_pCosmos->m_strAppCommonDocPath + _T("CommonDocument\\");
		//			s.Format(_T("Please select Tangram Document \"%s\" Template"), m_pDocFrame->m_pCosmosDoc->m_strDocID);
		//			bstrTemplate = g_pCosmos->GetDocTemplateXml(s, _strPath, _T(".AppXml")).AllocSysString();
		//		}
		//	}
		//}

		LRESULT lRes = 0;
		HWND h = ::GetParent(m_hWnd);
		if (h != NULL)
		{
			h = ::GetAncestor(m_hWnd, GA_ROOT);
			lRes = ::SendMessage(h, WM_COSMOSMSG, 20190305, 0);
		}
		else
			h = m_hWnd;

		g_pCosmos->CreateGalaxyCluster((LONGLONG)h, &pGalaxyCluster);
		if (pGalaxyCluster)
		{
			IXobj* pXobj = nullptr;
			CComBSTR bstrKey(m_pDocFrame->m_pCosmosDoc->m_strDocID);
			pGalaxyCluster->CreateGalaxyWithDefaultNode((LONGLONG)m_hView, bstrKey, m_strWndID.AllocSysString(), bstrTemplate, false, &pXobj);
			if (pXobj)
			{
				IGalaxy* pGalaxy = nullptr;
				pXobj->get_Galaxy(&pGalaxy);
				m_pDocFrame->m_pHostGalaxy = (CGalaxy*)pGalaxy;
				if (lRes != 0)
				{
					CGalaxyCluster* _pGalaxyCluster = (CGalaxyCluster*)pGalaxyCluster;
					_pGalaxyCluster->m_mapNeedSaveGalaxy[m_hView] = m_pDocFrame->m_pHostGalaxy;
				}
				m_pDocFrame->m_pHostGalaxy->m_pDoc = m_pDocFrame->m_pCosmosDoc;
				m_pDocFrame->m_pCosmosDoc->m_mapGalaxy[m_strWndID] = m_pDocFrame;
				m_pDocFrame->m_mapWnd[m_hWnd] = this;
				m_pDocFrame->m_pCurrentWnd = this;
				m_pDocFrame->m_pCosmosDoc->m_pActiveWnd = this;
				m_pDocFrame->m_pHostGalaxy->m_nGalaxyType = lParam ? MDIChildGalaxy : SDIGalaxy;
				if (lParam == 0)
				{
					CString strFix = g_pCosmos->m_strAppName;
					if (strFix == _T(""))
						strFix = g_pCosmos->m_strExeName;
					CString strAppTitle = m_pDocFrame->m_pCosmosDoc->m_pDocProxy->m_strAppName + _T(" - ") + strFix;
					if (strAppTitle != _T(""))
					{
						::SendMessage(m_hWnd, WM_HUBBLE_SETAPPTITLE, (WPARAM)strAppTitle.GetBuffer(), 0);
					}
					strAppTitle.ReleaseBuffer();
				}
			}

			if (m_pGalaxy)
			{
				for (auto it2 : m_pGalaxy->m_pGalaxyCluster->m_mapCtrlBarGalaxy)
				{
					pXobj = nullptr;
					it2.second->Observe(bstrKey, CComBSTR(L""), &pXobj);
					if (pXobj)
					{
						pXobj->put_SaveToConfigFile(true);
						CtrlBarDocActiveNotify(m_pDocFrame->m_pCosmosDoc, m_pDocFrame->m_pHostGalaxy->m_pWorkXobj, pXobj, it2.first);
					}
				}
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

void CCosmosDocWnd::CtrlBarDocActiveNotify(ICosmosDoc* pDoc, IXobj* pXobjInDoc, IXobj* pXobjInCtrlBar, HWND hCtrlBar)
{
	if (pDoc != nullptr)
	{
		CXobj* pXobj = (CXobj*)pXobjInCtrlBar;
		switch (pXobj->m_nViewType)
		{
		case Grid:
		case TabGrid:
			for (auto it : pXobj->m_vChildNodes)
			{
				CtrlBarDocActiveNotify(pDoc, pXobjInDoc, it, hCtrlBar);
			}
			break;
		default:
		{
			LRESULT lRes = ::SendMessage(m_pGalaxy->m_pGalaxyCluster->m_hWnd, WM_QUERYAPPPROXY, 0, 0);
			if (lRes > 0)
			{
				IUniverseAppProxy* pProxy = (IUniverseAppProxy*)lRes;
				pProxy->OnActiveDocument(pDoc, pXobjInDoc, pXobjInCtrlBar, hCtrlBar);
			}
			if (g_pCosmos->m_pCosmosAppProxy)
			{
				g_pCosmos->m_pCosmosAppProxy->OnActiveDocument(pDoc, pXobjInDoc, pXobjInCtrlBar, hCtrlBar);
			}
		}
		break;
		}
	}
}

// CGalaxy
CGalaxy::CGalaxy()
{
	m_pCurrentIPCMsg = nullptr;
	m_OldRect = { 0,0,0,0 };
	m_strAsynKeys = _T("");
	m_strLastKey = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentXml = _T("");
	m_strGalaxyName = _T("");
	m_bMDIChild = false;
	m_bDetached = false;
	m_pWebPageWnd = nullptr;
	m_pSubGalaxy = nullptr;
	m_pWorkBenchFrame = nullptr;
	m_bTabbedMDIClient = false;
	m_bDesignerState = true; 
	m_hPWnd = nullptr;
	m_pBKWnd = nullptr;
	m_pGalaxyCluster = nullptr;
	m_pWorkXobj = nullptr;
	m_pRootNodes = nullptr;
	m_pBindingXobj = nullptr;
	m_pContainerNode = nullptr;
	m_pDoc = nullptr;
	m_pGalaxyInfo = nullptr;
	m_pParentXobj = nullptr;
	m_pCosmosDocTemplate = nullptr;
	m_nGalaxyType = NOGalaxy;
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
	if (m_mapVal.size()) {
		for (auto it : m_mapVal) {
			::VariantClear(&it.second);
		}
		m_mapVal.clear();
	}
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
	if (::IsWindow(m_hWnd) == false)
		return;
	HWND hwnd = m_hWnd;
	CXobj* pTopXobj = m_pWorkXobj;
	CGalaxy* _pGalaxy = this;
	if (!_pGalaxy->m_bDesignerState)
		while (pTopXobj && pTopXobj->m_pRootObj != pTopXobj)
		{
			_pGalaxy = pTopXobj->m_pRootObj->m_pXobjShareData->m_pGalaxy;
			hwnd = _pGalaxy->m_hWnd;
			pTopXobj = _pGalaxy->m_pWorkXobj;
		}
	if (::IsWindow(hwnd) == false)
		return;
	HWND hPWnd = ::GetParent(m_hWnd);
	if (::IsWindow(_pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd))
	{
		RECT rt1;
		_pGalaxy->m_pWorkXobj->m_pHostWnd->GetWindowRect(&rt1);

		::ScreenToClient(hPWnd, (LPPOINT)&rt1);
		::ScreenToClient(hPWnd, ((LPPOINT)&rt1) + 1);

		HDWP dwh = BeginDeferWindowPos(1);
		UINT flag = SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_SHOWWINDOW;
		m_bObserve = !m_bDockPane;
		if (m_bObserve)
			flag |= SWP_NOREDRAW;
		if (m_bTabbedMDIClient)//&&m_nGalaxyType == GalaxyType::MDIClientGalaxy)
			flag &= ~SWP_NOREDRAW;
		dwh = ::DeferWindowPos(dwh, hwnd, HWND_TOP,
			rt1.left,
			rt1.top,
			rt1.right - rt1.left,
			rt1.bottom - rt1.top,
			flag
		);
		EndDeferWindowPos(dwh);
		UpdateVisualWPFMap(hPWnd, false);
		if (m_pBKWnd && ::IsWindow(m_pBKWnd->m_hWnd))
		{
			::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 0, 0, rt1.right - rt1.left, rt1.bottom - rt1.top, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
		::SendMessage(hPWnd, WM_QUERYAPPPROXY, 0, 19651965);
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

void CGalaxy::UpdateDesignerTreeInfo()
{
	if (m_bDesignerState && g_pCosmos->m_hChildHostWnd) {
		g_pCosmos->m_pDesigningFrame = this;
		if (g_pCosmos->m_pDocDOMTree && ::IsWindow(g_pCosmos->m_pDocDOMTree->m_hWnd)) {
			g_pCosmos->m_pDocDOMTree->DeleteItem(g_pCosmos->m_pDocDOMTree->m_hFirstRoot);
			if (g_pCosmos->m_pDocDOMTree->m_pHostXmlParse) {
				delete g_pCosmos->m_pDocDOMTree->m_pHostXmlParse;
				g_pCosmos->m_pDocDOMTree->m_pHostXmlParse = nullptr;
			}
			CXobj* pXobj = g_pCosmos->m_pDesigningFrame->m_pWorkXobj;
			if (pXobj == nullptr) {
				return;
			}

			CString _strName = pXobj->m_strName;
			_strName += _T("-indesigning");
			_strName.MakeLower();
			CTangramXmlParse* pParse = nullptr;
			auto it = m_mapXobj.find(_strName);
			if (it != m_mapXobj.end())
				pParse = it->second->m_pXobjShareData->m_pCosmosParse;
			else
				pParse = pXobj->m_pXobjShareData->m_pCosmosParse;
			g_pCosmos->InitDesignerTreeCtrl(pParse->xml());
		}
	}
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

	CreateGalaxyCluster();
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
		if(hForm)
			m_pWorkXobj->m_pParentWinFormWnd = (CWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
	}
	return m_pWorkXobj;
}

BOOL CGalaxy::CreateGalaxyCluster()
{
	if (::IsWindow(m_hWnd) == false)
		SubclassWindow(m_hHostWnd);
	HWND hPWnd = ::GetParent(m_hWnd);
	CWnd* pWnd = CWnd::FromHandlePermanent(hPWnd);
	if (pWnd == nullptr)
	{
		::GetClassName(hPWnd, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strClassName = g_pCosmos->m_szBuffer;
		if ((strClassName.Find(_T("Afx:ControlBar")) == 0) ||
			(strClassName.Find(_T("SysTreeView32")) == 0 ||
				strClassName.Find(_T("SysTabControl32")) == 0 ||
				strClassName.Find(_T("SysListView32")) == 0))
		{
			CDockPaneWnd* _pWnd = new CDockPaneWnd();
			_pWnd->SubclassWindow(hPWnd);
			_pWnd->m_hClient = m_hWnd;
		}
	}
	if (m_hPWnd && ::IsWindow(m_hPWnd))
		hPWnd = m_pGalaxyCluster->m_hWnd;
	else
		hPWnd = ::GetParent(m_hWnd);

	//m_pWorkXobj->m_strName.Trim();
	//m_pWorkXobj->m_strName.MakeLower();
	m_pWorkXobj->InitWndXobj();
	HWND hWnd = NULL;
	if (m_pWorkXobj->m_pObjClsInfo) {
		RECT rc;
		CWnd* pParentWnd = CWnd::FromHandle(::GetParent(m_hWnd));
		//CWnd* pParentWnd = CWnd::FromHandle(hPWnd);
		m_pWorkXobj->m_pRootObj = m_pWorkXobj;
		CCreateContext	m_Context;
		m_Context.m_pNewViewClass = m_pWorkXobj->m_pObjClsInfo;
		CWnd* pWnd = (CWnd*)m_pWorkXobj->m_pObjClsInfo->CreateObject();
		GetWindowRect(&rc);
		if (pParentWnd)
			pParentWnd->ScreenToClient(&rc);

		pWnd->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rc, pParentWnd, 0, &m_Context);
		hWnd = pWnd->m_hWnd;
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

	m_pRootNodes->m_pXobjs->clear();
	for (auto it : m_mapXobj) {
		m_pRootNodes->m_pXobjs->push_back(it.second);
	}

	return m_pRootNodes->QueryInterface(IID_IXobjCollection, (void**)pXobjColletion);
}

STDMETHODIMP CGalaxy::get_GalaxyData(BSTR bstrKey, VARIANT* pVal)
{
	CString strKey = OLE2T(bstrKey);

	if (strKey != _T("")) {
		::SendMessage(m_hWnd, WM_COSMOSMSG, (WPARAM)strKey.GetBuffer(), 0);
		strKey.Trim();
		strKey.MakeLower();
		auto it = m_mapVal.find(strKey);
		if (it != m_mapVal.end())
			*pVal = it->second;
		strKey.ReleaseBuffer();
	}
	return S_OK;
}

STDMETHODIMP CGalaxy::put_GalaxyData(BSTR bstrKey, VARIANT newVal)
{
	CString strKey = OLE2T(bstrKey);

	if (strKey == _T(""))
		return S_OK;
	strKey.Trim();
	strKey.MakeLower();
	m_mapVal[strKey] = newVal;
	return S_OK;
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
	//::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20180115);
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
	for (auto it : m_mapXobj)
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
	if (m_bDockPane == false)
	{
		CWnd* _pWnd = CWnd::FromHandlePermanent(::GetParent(m_hWnd));
		if (_pWnd && _pWnd->IsKindOf(RUNTIME_CLASS(CDockPaneWnd)))
			m_bDockPane = true;
	}
	if (::GetWindowLong(m_hWnd, GWL_STYLE) & MDIS_ALLCHILDSTYLES)
		m_nGalaxyType = GalaxyType::MDIClientGalaxy;
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
	auto it = m_mapXobj.find(m_strCurrentKey);

	CXobj* pOldNode = m_pWorkXobj;
	if (it != m_mapXobj.end())
		m_pWorkXobj = it->second;
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
				CUniverseMDIChild* pWnd = (CUniverseMDIChild*)::SendMessage(m_pGalaxyCluster->m_hWnd, WM_COSMOSMSG, 0, 19631222);
				if (pWnd == nullptr && m_pCosmosDocTemplate && m_strCurrentKey != _T("newdocument"))
				{
					CString strKey = m_strGalaxyName;
					strKey.Replace(_T("@"), _T("_"));
					auto it = m_pCosmosDocTemplate->m_mapXml.find(strKey);
					if (it != m_pCosmosDocTemplate->m_mapXml.end())
					{
						CTangramXmlParse parse;
						if (parse.LoadXml(it->second))
						{
							CTangramXmlParse* pChild = parse.GetChild(m_strCurrentKey);
							if (pChild)
							{
								bAtTemplate = true;
								strXml = pChild->xml();
							}
							else
							{
								if (parse.GetChild(TGM_CLUSTER))
								{
									bAtTemplate = true;
									strXml = parse.xml();
								}
							}
						}
					}
				}
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
							strXml = _T("<default><cluster><xobj name=\"Start\" /></cluster></default>");;
					}
					else
						strXml = _strXml;
				}
			}
		}

		Unlock();
		m_pGalaxyCluster->Fire_BeforeOpenXml(CComBSTR(strXml), (long)m_hHostWnd);

		m_bObserve = true;

		m_pWorkXobj = g_pCosmos->ObserveEx((long)m_hHostWnd, _T(""), strXml);
		if (m_pWorkXobj == nullptr)
			return S_FALSE;
		if (::GetWindowLong(::GetParent(m_hWnd), GWL_EXSTYLE) & WS_EX_MDICHILD)
			m_bMDIChild = true;
	}
	m_pBindingXobj = m_pWorkXobj->m_pXobjShareData->m_pHostClientView ? m_pWorkXobj->m_pXobjShareData->m_pHostClientView->m_pXobj : nullptr;
	if (m_strGalaxyName == _T("default"))
	{
		CString strName = m_pWorkXobj->m_pHostParse->attr(_T("galaxy"), _T(""));
		if (strName != _T(""))
			m_strGalaxyName = strName;
	}

	g_pCosmos->m_strCurrentKey = _T("");
	*ppRetXobj = (IXobj*)m_pWorkXobj;
	for (auto it : g_pCosmos->m_mapCosmosAppProxy)
	{
		it.second->OnObserverComplete(m_hHostWnd, strXml, m_pWorkXobj);
	}
	if (g_pCosmos->m_pCosmosAppProxy)
		g_pCosmos->m_pCosmosAppProxy->OnObserverComplete(m_hHostWnd, strXml, m_pWorkXobj);

	if (pOldNode && pOldNode != m_pWorkXobj)
	{
		RECT  rc;
		if (::IsWindow(pOldNode->m_pHostWnd->m_hWnd))
			::GetWindowRect(pOldNode->m_pHostWnd->m_hWnd, &rc);
		CWnd* pWnd = m_pWorkXobj->m_pHostWnd;
		HWND hParent = ::GetParent(m_hWnd);

		CWnd::FromHandle(hParent)->ScreenToClient(&rc);
		for (auto it : m_mapXobj)
		{
			HWND hwnd = it.second->m_pHostWnd->m_hWnd;
			BOOL bTop = (it.second == m_pWorkXobj);
			it.second->m_bTopObj = bTop;
			::SetWindowLongPtr(hwnd, GWLP_ID, bTop ? m_pWorkXobj->m_nID : 0);
			::SetParent(hwnd, bTop ? hParent : pWnd->m_hWnd);
			if (!bTop)
			{
				::ShowWindow(hwnd, SW_HIDE);
			}
		}
		::SetWindowPos(pWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);

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

		for (auto it : m_mapGalaxyProxy)
		{
			it.second->OnExtend(m_pWorkXobj, m_strCurrentKey, strXml);
		}

		if (m_pBKWnd && m_pBKWnd->m_pGalaxy)
		{
			IXobj* pXobj = nullptr;
			m_pBKWnd->m_pGalaxy->Observe(CComBSTR(L"default"), CComBSTR(L""), &pXobj);
		}
	}

	m_bObserve = false;

	HostPosChanged();
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
		if (_pHostNode&&m_pWebPageWnd)
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
	for (auto it : m_pWorkXobj->m_mapExtendNode)
	{
		IXobj* pXobj = nullptr;
		it.first->Observe(CComBSTR(it.second), CComBSTR(""), &pXobj);
	}
	if (m_pWebPageWnd)
	{
		ATLTRACE(L"\n");
	}
	if (m_pHostWebBrowserWnd)
	{
		IXobj* pXobj = nullptr;
		CComPtr<IXobjCollection> pCol;
		long nCount = 0;
		m_pWorkXobj->GetXobjs(CComBSTR(m_strHostWebBrowserNodeName), &pXobj, &pCol, &nCount);
		if (pXobj)
		{
			CXobj* _pXobj = (CXobj*)pXobj;
			if (_pXobj->m_nViewType == BlankView)
			{
				CGalaxy* _pGalaxy = nullptr;
				if (_pXobj->m_pHostGalaxy)
				{
					_pGalaxy = _pXobj->m_pHostGalaxy;
					while (_pGalaxy)
					{
						if(_pGalaxy->m_pHostWebBrowserNode)
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
		}
	}
	::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20180115);
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
			::SetWindowPos(m_pBKWnd->m_hWnd, HWND_BOTTOM, 0, 0, rect.right, rect.bottom, SWP_NOREPOSITION | SWP_NOSENDCHANGING | SWP_NOACTIVATE);
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
						m_pDoc->m_pDocProxy->m_bCanDestroyFrame = false;
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
	else
	{
		if (wParam && g_pCosmos->m_pMDIMainWnd && g_pCosmos->m_pMDIMainWnd->m_hMDIClient == m_hWnd)
		{
			TRACE(_T("\n"));
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
	if (m_mapXobjScript.size())
	{
		this->UpdateXobj();
		for (auto it : m_mapXobjScript)
		{
			CString strPath = it.first;
			CString s = it.second->m_pHostParse->xml();
			CString str = _T("");
			str.Format(_T("<nodexml><cluster>%s</cluster></nodexml>"), s);
			CTangramXmlParse parse;
			parse.LoadXml(str);
			parse.SaveFile(it.first);
		}
	}
	if (m_pBKWnd)
		m_pBKWnd->DestroyWindow();
	if (g_pCosmos->m_pDesigningFrame && g_pCosmos->m_pDesigningFrame == this)
		g_pCosmos->m_pDesigningFrame = nullptr;
	m_pGalaxyCluster->BeforeDestory();
	m_pGalaxyCluster->m_strConfigFileNodeName.MakeLower();//20190116
	//auto it = g_pCosmos->m_mapWindowPage.find(m_pGalaxyCluster->m_hWnd);
	//if (it != g_pCosmos->m_mapWindowPage.end())
	//{
	//	auto it2 = m_pGalaxyCluster->m_mapNeedSaveGalaxy.find(m_hWnd);
	//	if (it2 != m_pGalaxyCluster->m_mapNeedSaveGalaxy.end())
	//	{
	//		if (!m_pGalaxyCluster->m_bNewVersion)
	//		{
	//			m_pGalaxyCluster->put_ConfigName(m_pGalaxyCluster->m_strConfigFileNodeName.AllocSysString());
	//		}
	//	}
	//}

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
	case 2048:
	{
		if (m_hWnd != g_pCosmos->m_hChildHostWnd)
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
	case 20180115:
	{
		HostPosChanged();
	}
	break;
	case 20200601:
	{
		HostPosChanged();
		//for webruntimeVS Dockabe ToolWindow
		if (m_pWorkXobj)
		{
			::PostMessage(m_pWorkXobj->m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20200601);
		}
	}
	break;
	case 20200531:
	{
		//for webruntimeVS Dockabe ToolWindow
		if (m_pWorkXobj)
		{
			auto t = create_task([this]()
				{
					//SleepEx(200, true);
					try
					{
						::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20200601);
					}
					catch (...)
					{
						ATLASSERT(false);
						return 0;
					}
					return 1;
				});
		}
	}
	break;
	case WM_BROWSERLAYOUT:
	{
		CWebPage* pWebWnd = (CWebPage*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
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
	case 20170929:
	{
		TangramFrameInfo* pCosmosFrameInfo = (TangramFrameInfo*)wParam;
		CString _strKey = _T(",");
		_strKey += pCosmosFrameInfo->m_strKey;
		_strKey += _T(",");
		if (m_strAsynKeys.Find(_strKey) != -1)
		{
			m_strAsynKeys.Replace(_strKey, _T(""));
		}

		IXobj* pXobj = nullptr;
		int nPos = pCosmosFrameInfo->m_strXml.ReverseFind('>');
		CString strXml = pCosmosFrameInfo->m_strXml;
		this->Observe(pCosmosFrameInfo->m_strKey.AllocSysString(), CComBSTR(strXml), &pXobj);
		delete pCosmosFrameInfo;
		return 0;
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
	//HostPosChanged();
	//::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20180115);
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
	if (g_pCosmos->m_pMDIMainWnd && lParam == 19651965)
	{
		m_bTabbedMDIClient = true;
		LPRECT lpRECT = (LPRECT)wParam;
		if (lpRECT && m_pWorkXobj && ::IsWindowVisible(m_pWorkXobj->m_pHostWnd->m_hWnd))
		{
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpRECT->left, lpRECT->top, lpRECT->right - lpRECT->left, lpRECT->bottom - lpRECT->top, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_FRAMECHANGED);/*SWP_FRAMECHANGED| SWP_HIDEWINDOW| SWP_NOZORDER | SWP_NOREDRAW */
			if (m_pBindingXobj && ::IsWindowVisible(m_pBindingXobj->m_pHostWnd->m_hWnd))
			{
				m_pBindingXobj->m_pHostWnd->GetWindowRect(lpRECT);
				g_pCosmos->m_pMDIMainWnd->ScreenToClient(lpRECT);
			}
		}
		return m_pWorkXobj ? ((LRESULT)m_pWorkXobj->m_pHostWnd->m_hWnd) : 0;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT hr = DefWindowProc(uMsg, wParam, lParam);

	if (g_pCosmos->m_pBrowserFactory && g_pCosmos->m_bChromeNeedClosed)
		return hr;

	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
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
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOACTIVATE| SWP_FRAMECHANGED );// |SWP_NOREDRAW); 
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
						CXobjHelper* pWnd = (CXobjHelper*)_pParentNode->m_pHostWnd;
						::InvalidateRect(pWnd->m_hWnd, nullptr, true);
					}
					break;
					}
				}
				if (m_pContainerNode && m_pContainerNode->m_pParentObj && m_pContainerNode->m_pParentObj->m_nViewType == Grid)
				{
					if (m_pContainerNode->m_nViewType == CLRCtrl)
					{
						CXobjHelper* pXobjWnd = (CXobjHelper*)m_pContainerNode->m_pHostWnd;
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
			::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_TOP, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOSENDCHANGING | /*SWP_NOZORDER |*/ SWP_NOACTIVATE | SWP_FRAMECHANGED);
			lpwndpos->flags &= ~SWP_SHOWWINDOW;
			lpwndpos->flags |= SWP_HIDEWINDOW;
		}
	}

	if (m_bMDIChild)
		lpwndpos->flags |= SWP_NOZORDER;

	::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return hr;
}

LRESULT CGalaxy::OnParentNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_pGalaxy = nullptr;
	return DefWindowProc(uMsg, wParam, lParam);
}

STDMETHODIMP CGalaxy::get_DesignerState(VARIANT_BOOL* pVal)
{
	if (m_bDesignerState)
		*pVal = true;
	else
		*pVal = false;

	return S_OK;
}

STDMETHODIMP CGalaxy::put_DesignerState(VARIANT_BOOL newVal)
{
	m_bDesignerState = newVal;
	return S_OK;
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
		for (auto it : m_mapXobj)
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

STDMETHODIMP CGalaxy::get_CosmosDoc(ICosmosDoc** pVal)
{
	if (m_pDoc == nullptr)
	{
		CCosmosDocWnd* pWnd = (CCosmosDocWnd*)::SendMessage(::GetParent(m_hWnd), WM_COSMOSMSG, 2016, 0);
		if (pWnd)
		{
			m_pDoc = pWnd->m_pDocFrame->m_pCosmosDoc;
		}
	}

	*pVal = m_pDoc;
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
