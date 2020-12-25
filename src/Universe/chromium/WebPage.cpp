/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202012250002
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser;
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

#include "../stdafx.h"
#include "../Cosmos.h"
#include "../UniverseApp.h"
#include "../Xobj.h"
#include "../Wormhole.h"
#include "../Galaxy.h"
#include "../XobjHelper.h"
#include "../GridWnd.h"
#include "../Markup.h" 
#include "WebPage.h"
#include "Browser.h"

namespace Browser {
	CWebPage::CWebPage() {
		m_pWebWnd = nullptr;
		m_pDevToolWnd = nullptr;
		m_pAppProxy = nullptr;
		m_bDevToolWnd = false;
		m_bWebContentVisible = true;
		m_strCurKey = _T("");
		m_strCurXml = _T("");
		m_strDocXml = _T("");
		m_pParentXobj = nullptr;
		m_strAppProxyID = _T("");
		m_pGalaxyCluster = nullptr;
		m_pGalaxy = nullptr;
		m_hWebHostWnd = m_hExtendWnd = m_hChildWnd = NULL;
		m_pChromeRenderFrameHost = g_pCosmos->m_pCreatingChromeRenderFrameHostBase;
		g_pCosmos->m_pCreatingChromeRenderFrameHostBase = nullptr;
	}

	CWebPage::~CWebPage() {
	}

	LRESULT CWebPage::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		CBrowser* pParent = nullptr;
		auto it = g_pCosmos->m_mapBrowserWnd.find(::GetParent(m_hWnd));
		if (it != g_pCosmos->m_mapBrowserWnd.end())
		{
			pParent = (CBrowser*)it->second;
		}
		HWND hPWnd = ::GetParent(pParent->m_hWnd);
		if (hPWnd != NULL)
		{
			g_pCosmos->m_pActiveHtmlWnd = this;
			g_pCosmos->m_pGalaxy = nullptr;
			g_pCosmos->m_bWinFormActived = false;
			g_pCosmos->m_hActiveWnd = nullptr;
			LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
			::PostMessage(m_hWnd, WM_COSMOSMSG, 20190331, 1);
			m_pChromeRenderFrameHost->ShowWebPage(true);
			return lRes;
		}
		else
			g_pCosmos->m_pActiveHtmlWnd = nullptr;
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CWebPage::OnCosmosMsg(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL&) {
		bool bChild = ::GetWindowLongPtr(::GetParent(m_hWnd), GWL_STYLE) & WS_CHILD;
		switch (wParam)
		{
		case 20201109:
		{
			if (lParam)
			{
				if (::IsWindow(m_hExtendWnd))
				{
					::SetParent(m_hExtendWnd, ::GetParent(m_hWnd));
					::ShowWindow(m_hExtendWnd, SW_SHOW);
				}
			}
			else
			{
				::SetParent(m_hExtendWnd, m_hWnd);
				::ShowWindow(m_hExtendWnd, SW_HIDE);
			}
		}
		break;
		case 20200429:
		{
			CSession* pSession = (CSession*)lParam;
			pSession->SendMessage();
		}
		break;
		case 20200310:
		{
			CXobj* pXobj = (CXobj*)lParam;
			if (pXobj && pXobj->m_pWormhole == nullptr)
			{
				pXobj->m_pWormhole = (CWormhole*)((CCosmosImpl*)g_pCosmos)->CreateCloudSession(this);
				CWormhole* pSession = pXobj->m_pWormhole;
				if (pSession)
				{
					pSession->InsertString(_T("msgID"), IPC_NODE_CREARED_ID);
					pSession->InsertLong(_T("autodelete"), 0);
					pSession->InsertLong(_T("gridtype"), pXobj->m_nViewType);
					pSession->InsertLong(_T("rows"), pXobj->m_nRows);
					pSession->InsertLong(_T("cols"), pXobj->m_nCols);
					pSession->InsertLong(_T("row"), pXobj->m_nRow);
					pSession->InsertLong(_T("col"), pXobj->m_nCol);
					pSession->InsertString(_T("objtype"), pXobj->m_strObjTypeID);
					if (pXobj->m_strCosmosXml != _T(""))
					{
						pSession->InsertString(_T("cosmosxml"), pXobj->m_strCosmosXml);
					}
					pSession->InsertString(_T("gridxml"), pXobj->m_pHostParse->xml());
					pSession->InsertString(_T("name@page"), pXobj->m_strName);
					pSession->Insertint64(_T("xobjhandle"), (__int64)pXobj->m_pHostWnd->m_hWnd);
					pSession->Insertint64(_T("xobj"), (__int64)(IXobj*)pXobj);
					pSession->Insertint64(_T("Galaxyhandle"), (__int64)pXobj->m_pXobjShareData->m_pGalaxy->m_hWnd);
					pSession->InsertString(_T("galaxy"), pXobj->m_pXobjShareData->m_pGalaxy->m_strGalaxyName);
					pSession->InsertString(_T("cluster"), pXobj->m_pRootObj->m_strKey);
					pSession->Insertint64(_T("rootgridhandle"), (__int64)pXobj->m_pRootObj->m_pHostWnd->m_hWnd);
					pSession->Insertint64(_T("domhandle"), (__int64)pXobj->m_pWormhole);
					if (pXobj->m_pXobjShareData->m_pHostClientView)
						pSession->Insertint64(_T("nucleushandle"), (__int64)pXobj->m_pXobjShareData->m_pHostClientView->m_hWnd);
					pSession->InsertString(_T("objID"), _T("wndnode"));
					switch (pXobj->m_nViewType)
					{
					case Grid:
					{
						CGridWnd* pWnd = (CGridWnd*)pXobj->m_pHostWnd;
						for (int i = 0; i < pXobj->m_nRows; i++)
						{
							for (int j = 0; j < pXobj->m_nCols; j++)
							{
								int nIndex = j + i * pXobj->m_nRows;
								CString strIndex = _T("");
								strIndex.Format(_T("%d"), nIndex);
								pSession->Insertint64(strIndex, (__int64)::GetDlgItem(pWnd->m_hWnd, pWnd->IdFromRowCol(i, j)));
							}
						}
					}
					break;
					case TabGrid:
					{
						for (auto it : pXobj->m_vChildNodes)
						{
							CString strIndex = _T("");
							strIndex.Format(_T("%d"), it->m_nCol);
							pSession->Insertint64(strIndex, (__int64)it->m_pHostWnd->m_hWnd);
						}
					}
					break;
					default:
						break;
					}
					if (pXobj->m_pParentObj)
					{
						pSession->Insertint64(_T("parenthandle"), (__int64)pXobj->m_pParentObj->m_pHostWnd->m_hWnd);
					}
					if (pXobj->m_pParentWinFormWnd)
					{
						pSession->Insertint64(_T("parentFormHandle"), (__int64)pXobj->m_pParentWinFormWnd->m_hWnd);
					}
					if (pXobj->m_pDisp)
					{
						pXobj->m_pWormhole->Insertint64(_T("objectdisp"), (__int64)pXobj->m_pDisp);
						if (g_pCosmos->m_pCLRProxy)
						{
							g_pCosmos->m_pCLRProxy->ConnectXobjToWebPage(pXobj, true);
						}
					}
					m_pChromeRenderFrameHost->SendCosmosMessage(pSession->m_pSession);
				}
			}
		}
		break;
		case 20200311:
		{
			if (m_pGalaxy&&m_pGalaxy->m_pWorkXobj->m_pWormhole)
			{
				m_pGalaxy->m_pWorkXobj->m_pWormhole->InsertString(_T("msgID"), _T("TANGRAMAPP_READY"));
				m_pGalaxy->m_pWorkXobj->m_pWormhole->SendMessage();
			}
		}
		break;
		case 20200131:
		{
			if (bChild && m_pChromeRenderFrameHost)
			{
				::SetParent(m_hExtendWnd, ::GetParent(m_hWnd));
				m_pChromeRenderFrameHost->ShowWebPage(true);
			}
		}
		break;
		case 20200221:
		{
			IPCMsg* pMsg = (IPCMsg*)lParam;
			if (pMsg && m_pChromeRenderFrameHost)
			{
				m_pChromeRenderFrameHost->SendCosmosMessage(pMsg);
				g_pCosmos->m_pCurrentIPCMsg = nullptr;
			}
		}
		break;
		case 20200213:
		{
			HWND hWnd = (HWND)lParam;
			if (::IsWindow(hWnd))
			{
				CWinForm* pWnd = (CWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
				if (pWnd)
				{
					m_mapSubWinForm[hWnd] = pWnd;
				}
			}
		}
		break;
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebPage::OnParentChanged(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL&) {
		if (g_pCosmos->m_bChromeNeedClosed == false && lParam) {
			HWND hNewPWnd = (HWND)lParam;
			::GetClassName(hNewPWnd, g_pCosmos->m_szBuffer, 256);
			CString strName = CString(g_pCosmos->m_szBuffer);
			if (strName.Find(_T("Chrome_WidgetWin_"))==0)
			{
				if (m_hExtendWnd)
				{
					::SetParent(m_hExtendWnd, hNewPWnd);
					::ShowWindow(m_hExtendWnd, SW_HIDE);
				}
				HWND hNewPWnd2 = ::GetParent(m_hWnd);
				bool bNewParent = true;
				if (hNewPWnd != hNewPWnd2)
				{
					hNewPWnd = hNewPWnd2;
					bNewParent = true;
				}
				CBrowser* pBrowserWnd = nullptr;
				auto it = g_pCosmos->m_mapBrowserWnd.find(hNewPWnd);
				if (it == g_pCosmos->m_mapBrowserWnd.end())
				{
					if (::IsWindowVisible(hNewPWnd)) {
						pBrowserWnd = new CComObject<CBrowser>();
						pBrowserWnd->SubclassWindow(hNewPWnd);
						g_pCosmos->m_mapBrowserWnd[hNewPWnd] = pBrowserWnd;
						pBrowserWnd->m_pBrowser = g_pCosmos->m_pActiveBrowser;
						if (pBrowserWnd->m_pBrowser)
							pBrowserWnd->m_pBrowser->m_pProxy = pBrowserWnd;
						if (pBrowserWnd && m_hExtendWnd) {
							::SetParent(m_hExtendWnd, hNewPWnd);
							if (::IsWindowVisible(m_hWnd)) {
								pBrowserWnd->m_pVisibleWebWnd = this;
								if (bNewParent)
								{
									::PostMessageW(hNewPWnd, WM_BROWSERLAYOUT, 0, 4);
								}
							}
						}
					}
				}
			}
			else
			{
				if (m_hExtendWnd)
				{
					::SetParent(m_hExtendWnd, hNewPWnd);
					::ShowWindow(m_hExtendWnd, SW_SHOW);
				}
				HWND hNewPWnd2 = ::GetParent(m_hWnd);
				bool bNewParent = false;
				if (hNewPWnd != hNewPWnd2)
				{
					hNewPWnd = hNewPWnd2;
					bNewParent = true;
				}
				CBrowser* pChromeBrowserWnd = nullptr;
				auto it = g_pCosmos->m_mapBrowserWnd.find(hNewPWnd);
				if (it != g_pCosmos->m_mapBrowserWnd.end())
				{
					pChromeBrowserWnd = (CBrowser*)it->second;
					g_pCosmos->m_pActiveBrowser = pChromeBrowserWnd->m_pBrowser;
					if (pChromeBrowserWnd && m_hExtendWnd) {
						if (::IsWindowVisible(m_hWnd)) {
							pChromeBrowserWnd->m_pVisibleWebWnd = this;
							if (bNewParent)
							{
								g_pCosmos->m_pActiveBrowser->m_pProxy = pChromeBrowserWnd;
								pChromeBrowserWnd->BrowserLayout();
								::PostMessageW(hNewPWnd, WM_BROWSERLAYOUT, 0, 2);
							}
						}
					}
					if (pChromeBrowserWnd->m_pVisibleWebWnd && m_bDevToolWnd && pChromeBrowserWnd->m_pVisibleWebWnd->m_bDevToolWnd == false)
					{
						if (pChromeBrowserWnd->m_pVisibleWebWnd->m_pDevToolWnd == nullptr)
						{
							pChromeBrowserWnd->m_pVisibleWebWnd->m_pDevToolWnd = this;
							::ShowWindow(m_hWnd, SW_SHOW);
						}
					}
				}
				else
				{
					if (::IsWindowVisible(hNewPWnd)) {
						pChromeBrowserWnd = new CComObject<CBrowser>();
						pChromeBrowserWnd->SubclassWindow(hNewPWnd);
						g_pCosmos->m_mapBrowserWnd[hNewPWnd] = pChromeBrowserWnd;
						pChromeBrowserWnd->m_pBrowser = g_pCosmos->m_pActiveBrowser;
						pChromeBrowserWnd->m_pBrowser->m_pProxy = pChromeBrowserWnd;
						if (pChromeBrowserWnd && m_hExtendWnd) {
							::SetParent(m_hExtendWnd, hNewPWnd);
							if (::IsWindowVisible(m_hWnd)) {
								pChromeBrowserWnd->m_pVisibleWebWnd = this;
								if (bNewParent)
								{
									pChromeBrowserWnd->BrowserLayout();
									::PostMessageW(hNewPWnd, WM_BROWSERLAYOUT, 0, 2);
								}
							}
						}
					}
				}
			}
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebPage::OnDestroy(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL& /*bHandled*/) {
		if (m_hExtendWnd)
		{
			::DestroyWindow(m_hExtendWnd);
		}
		for (auto it : m_mapSubWinForm)
		{
			::DestroyWindow(it.second->m_hWnd);
		}
		if (g_pCosmos->m_pCLRProxy)
			g_pCosmos->m_pCLRProxy->OnWebPageCreated(m_hWnd, (CWebPageImpl*)this, (IWebPage*)this, 1);

		m_hExtendWnd = nullptr;

		if (m_bDevToolWnd) {
			if (m_pWebWnd) {
				m_pWebWnd->m_pDevToolWnd = nullptr;
			}
		}
		for (auto it : m_mapSubWinForm)
		{
			::DestroyWindow(it.first);
		}
		m_mapSubWinForm.erase(m_mapSubWinForm.begin(), m_mapSubWinForm.end());
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebPage::OnShowWindow(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL&) {
		if (g_pCosmos->m_bChromeNeedClosed == false && ::IsWindow(m_hExtendWnd))
		{
			if (wParam) {
				CBrowser* pBrowserWnd = nullptr;
				HWND hPWnd = ::GetParent(m_hWnd);
				if (!m_bDevToolWnd)
				{
					auto it = g_pCosmos->m_mapBrowserWnd.find(hPWnd);
					if (it != g_pCosmos->m_mapBrowserWnd.end()) {
						pBrowserWnd = (CBrowser*)it->second;
						pBrowserWnd->m_pVisibleWebWnd = this;
					}
				}
				::ShowWindow(m_hExtendWnd, SW_SHOW);
				::SetParent(m_hExtendWnd, hPWnd);
				if (pBrowserWnd)
				{
					m_pChromeRenderFrameHost->ShowWebPage(true);
					pBrowserWnd->BrowserLayout();
					::PostMessage(hPWnd, WM_BROWSERLAYOUT, 0, 2);
				}
			}
			else
			{
				::SetParent(m_hExtendWnd, m_hWnd);
			}
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	void CWebPage::OnFinalMessage(HWND hWnd) {
		CBrowser* pPWnd = nullptr;
		auto it2 = g_pCosmos->m_mapBrowserWnd.find(::GetParent(hWnd));
		if (it2 != g_pCosmos->m_mapBrowserWnd.end())
		{
			pPWnd = (CBrowser*)it2->second;
			if (pPWnd->m_pVisibleWebWnd == this)
				pPWnd->m_pVisibleWebWnd = nullptr;
		}
		auto it = g_pCosmos->m_mapHtmlWnd.find(hWnd);
		if (it != g_pCosmos->m_mapHtmlWnd.end())
		{
			g_pCosmos->m_mapHtmlWnd.erase(it);
		}
		if (g_pCosmos->m_pActiveHtmlWnd == this)
			g_pCosmos->m_pActiveHtmlWnd = nullptr;
		CWindowImpl::OnFinalMessage(hWnd);
		delete this;
	}

	void CWebPage::SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5)
	{
		if (m_pChromeRenderFrameHost != nullptr)
		{
			IPCMsg pIPCInfo;
			pIPCInfo.m_strId = strId;
			pIPCInfo.m_strParam1 = strParam1;
			pIPCInfo.m_strParam2 = strParam2;
			pIPCInfo.m_strParam3 = strParam3;
			pIPCInfo.m_strParam4 = strParam4;
			pIPCInfo.m_strParam5 = strParam5;
			m_pChromeRenderFrameHost->SendCosmosMessage(&pIPCInfo);
		}
		g_pCosmos->m_pCurrentIPCMsg = nullptr;
	}

	LRESULT CWebPage::OnChromeIPCMsgReceived(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (lParam)
		{
			if (wParam && m_pChromeRenderFrameHost == nullptr)
			{
				m_pChromeRenderFrameHost = (CChromeRenderFrameHost*)wParam;
				m_pChromeRenderFrameHost->m_pProxy = this;
			}
			IPCMsg* pIPCInfo = (IPCMsg*)lParam;
			CString strId = pIPCInfo->m_strId;
			CString strParam1 = pIPCInfo->m_strParam1;
			CString strParam2 = pIPCInfo->m_strParam2;
			CString strParam3 = pIPCInfo->m_strParam3;
			CString strParam4 = pIPCInfo->m_strParam4;
			CString strParam5 = pIPCInfo->m_strParam5;
			if (pIPCInfo->m_strId.CompareNoCase(_T("TANGRAM_CTRL_VALUE_MESSAGE")) == 0)
			{
				HWND hCtrl = (HWND)pIPCInfo->m_nHandleFrom;
				if (g_pCosmos->m_pCLRProxy)
				{
					IDispatch* pCtrl = g_pCosmos->m_pCLRProxy->GetCtrlFromHandle(hCtrl);
					if (pCtrl)
						g_pCosmos->m_pCLRProxy->SetCtrlValueByName(pCtrl, CComBSTR(""), true, CComBSTR(strParam4));
				}
			}
			else
			{
				HandleChromeIPCMessage(strId, strParam1, strParam2, strParam3, strParam4, strParam5);
			}
			g_pCosmos->m_pCurrentIPCMsg = nullptr;
		}

		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	CChromeBrowserBase* CWebPage::GetChromeBrowserBase(HWND hHostWnd)
	{
		return nullptr;
	}

	void CWebPage::LoadDocument2Viewport(CString strName, CString strXML)
	{
		HWND hPPWnd = ::GetParent(::GetParent(m_hWnd));
		if (m_pRemoteCosmos)
		{
			CString strInfo = _T("");
			strInfo.Format(_T("loaddocument2viewport:%I64d"), hPPWnd);
			CString strData = strName;
			strData += _T("|");
			strData += strXML;
			m_pRemoteCosmos->put_AppKeyValue(CComBSTR(strInfo), CComVariant(strData));
		}
		if (m_hExtendWnd == nullptr)
		{
			HWND hParent = NULL;
			if (::IsWindowVisible(m_hWnd))
				hParent = ::GetParent(m_hWnd);
			else
				hParent = m_hWnd;

			m_hExtendWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hParent, NULL, theUniverse.m_hInstance, NULL);
			m_hChildWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, m_hExtendWnd, (HMENU)1, theUniverse.m_hInstance, NULL);

			::SetWindowLongPtr(m_hExtendWnd, GWLP_USERDATA, (LONG_PTR)m_hChildWnd);
			::SetWindowLongPtr(m_hChildWnd, GWLP_USERDATA, (LONG_PTR)this);
		}
		if (m_hExtendWnd)
		{
			if (m_pGalaxy == nullptr) {
				CGalaxyCluster* pGalaxyCluster = nullptr;
				auto it = g_pCosmos->m_mapWindowPage.find(m_hExtendWnd);
				if (it != g_pCosmos->m_mapWindowPage.end())
					pGalaxyCluster = (CGalaxyCluster*)it->second;
				else
				{
					pGalaxyCluster = new CComObject<CGalaxyCluster>();
					pGalaxyCluster->m_hWnd = m_hExtendWnd;
					g_pCosmos->m_mapWindowPage[m_hExtendWnd] = pGalaxyCluster;
				}
				if (pGalaxyCluster) {
					IGalaxy* pGalaxy = nullptr;
					pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)m_hChildWnd), CComBSTR("default"), &pGalaxy);
					if (pGalaxy)
					{
						m_pGalaxy = (CGalaxy*)pGalaxy;
						m_pGalaxy->m_pWebPageWnd = this;
					}
				}
			}
			if (m_pGalaxy)
			{
				IXobj* pXobj = nullptr;
				m_pGalaxy->Observe(CComBSTR(strName), CComBSTR(strXML), &pXobj);
				if (pXobj)
				{
					m_strCurKey = strName;
					m_hWebHostWnd = NULL;
					if (m_pGalaxy->m_pBindingXobj)
					{
						m_hWebHostWnd = m_pGalaxy->m_pBindingXobj->m_pHostWnd->m_hWnd;
					}
				}
			}
		}
		CBrowser* pBrowserWnd = nullptr;
		auto it = g_pCosmos->m_mapBrowserWnd.find(::GetParent(m_hWnd));
		if (it != g_pCosmos->m_mapBrowserWnd.end())
		{
			pBrowserWnd = (CBrowser*)it->second;
			if (pBrowserWnd->m_pBrowser->GetActiveWebContentWnd() != m_hWnd)
				::ShowWindow(m_hWnd, SW_HIDE);
		}
		if (::IsWindowVisible(m_hWnd))
		{
			::SendMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 2);
			::PostMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 4);
		}
	}

	//void CWebPage::LoadDocument2Viewport(CString strName, CString strXML)
	//{
	//	HWND hPPWnd = ::GetParent(::GetParent(m_hWnd));
	//	if (m_hExtendWnd == nullptr)
	//	{
	//		HWND hParent = NULL;
	//		if (::IsWindowVisible(m_hWnd))
	//			hParent = ::GetParent(m_hWnd);
	//		else
	//			hParent = m_hWnd;

	//		m_hExtendWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hParent, NULL, theUniverse.m_hInstance, NULL);
	//		m_hChildWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, m_hExtendWnd, (HMENU)1, theUniverse.m_hInstance, NULL);

	//		::SetWindowLongPtr(m_hExtendWnd, GWLP_USERDATA, (LONG_PTR)m_hChildWnd);
	//		::SetWindowLongPtr(m_hChildWnd, GWLP_USERDATA, (LONG_PTR)this);
	//	}
	//	if (m_hExtendWnd)
	//	{
	//		if (m_pGalaxy == nullptr) {
	//			CGalaxyCluster* pGalaxyCluster = nullptr;
	//			auto it = g_pCosmos->m_mapWindowPage.find(m_hExtendWnd);
	//			if (it != g_pCosmos->m_mapWindowPage.end())
	//				pGalaxyCluster = (CGalaxyCluster*)it->second;
	//			else
	//			{
	//				pGalaxyCluster = new CComObject<CGalaxyCluster>();
	//				pGalaxyCluster->m_hWnd = m_hExtendWnd;
	//				g_pCosmos->m_mapWindowPage[m_hExtendWnd] = pGalaxyCluster;
	//			}
	//			if (pGalaxyCluster) {
	//				IGalaxy* pGalaxy = nullptr;
	//				pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)m_hChildWnd), CComBSTR("default"), &pGalaxy);
	//				if (pGalaxy)
	//				{
	//					m_pGalaxy = (CGalaxy*)pGalaxy;
	//					m_pGalaxy->m_pWebPageWnd = this;
	//				}
	//			}
	//		}
	//		if (m_pGalaxy)
	//		{
	//			IXobj* pXobj = nullptr;
	//			m_pGalaxy->Observe(CComBSTR(strName), CComBSTR(strXML), &pXobj);
	//			if (pXobj)
	//			{
	//				m_strCurKey = strName;
	//				m_hWebHostWnd = NULL;
	//				if (m_pGalaxy->m_pBindingXobj)
	//				{
	//					m_hWebHostWnd = m_pGalaxy->m_pBindingXobj->m_pHostWnd->m_hWnd;
	//				}
	//			}
	//		}
	//	}
	//	CBrowser* pBrowserWnd = nullptr;
	//	auto it = g_pCosmos->m_mapBrowserWnd.find(::GetParent(m_hWnd));
	//	if (it != g_pCosmos->m_mapBrowserWnd.end())
	//	{
	//		pBrowserWnd = (CBrowser*)it->second;
	//		if (pBrowserWnd->m_pBrowser->GetActiveWebContentWnd() != m_hWnd)
	//			::ShowWindow(m_hWnd, SW_HIDE);
	//	}
	//	if (::IsWindowVisible(m_hWnd))
	//	{
	//		::SendMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 2);
	//		::PostMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 2);
	//	}
	//}

	void CWebPage::HandleChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5)
	{
		if (strId.CompareNoCase(_T("__Browser_Layout__")) == 0)
		{
			CBrowser* pBrowserWnd = nullptr;
			auto it = g_pCosmos->m_mapBrowserWnd.find(::GetParent(m_hWnd));
			if (it != g_pCosmos->m_mapBrowserWnd.end())
			{
				pBrowserWnd = (CBrowser*)it->second;
				if (pBrowserWnd->m_pBrowser->GetActiveWebContentWnd() != m_hWnd)
					::ShowWindow(m_hWnd, SW_HIDE);
			}
			if (::IsWindowVisible(m_hWnd))
			{
				::SendMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 2);
				::PostMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 2);
			}
		}
		else if (strId.CompareNoCase(_T("RENDER_ELEMENT")) == 0)
		{
			CustomizedDOMElement(strParam1, strParam2);
		}
		else if (strId.CompareNoCase(_T("AGGREGATED_MESSAGE")) == 0)
		{
			HandleAggregatedMessage(strParam1, strParam2);
			if (m_strDocXml != _T(""))
			{
				CustomizedDocElement(m_strDocXml);
			}
			if (g_pCosmos->m_pHostHtmlWnd == nullptr && g_pCosmos->m_strAppXml != _T(""))
			{
				g_pCosmos->m_pHostHtmlWnd = this;
				g_pCosmos->CosmosInitFromeWeb();
				if (g_pCosmos->m_strMainWndXml != _T(""))
				{
					CustomizedMainWindowElement(g_pCosmos->m_strMainWndXml);
				}
				auto t = create_task([this]()
					{
						SleepEx(200, true);
						try
						{
							::PostMessage(::GetParent(m_hWnd), WM_COSMOSMSG, 20200214, 0);
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
		else if (strId.CompareNoCase(_T("NEW_TAB_PAGE_LOADED")) == 0)
		{
			OnNTPLoaded();
		}
		else if (strId.CompareNoCase(_T("TANGRAM_UI_MESSAGE")) == 0)
		{ 
			// Param1: Frame Name, Param2: HWND, Param3: XML, Param4: BindWebObj Name
			HWND hWndSource = (HWND)_wtoi(LPCTSTR(strParam2));
			if (::IsWindow(hWndSource))
			{
				auto it = m_mapWinForm.find(hWndSource);
				if (it != m_mapWinForm.end())
				{
					LoadDocument2Viewport(strParam1, strParam3);
					return;
				}
				LRESULT lRes = ::SendMessage(hWndSource, WM_HUBBLE_DATA, 0, 20190214);
				if (lRes)
				{
					LoadDocument2Viewport(strParam2, strParam3);
					return;
				}
			}
			LoadDocument2Viewport(strParam1, strParam3);
		}
		else
		{
			if (g_pCosmos->m_pCosmosDelegate)
				g_pCosmos->m_pCosmosDelegate->IPCMsg(m_hWnd, strId, strParam1, strParam2); // TODO: Missing parameters
		}
	}

	void CWebPage::HandleAggregatedMessage(CString strParam1, CString strParam2)
	{
		int nTokenPos = 0;
		CString strToken = strParam1.Tokenize(_T("$$$"), nTokenPos);
		while (!strToken.IsEmpty())
		{
			// Process a single message.
			HandleSingleMessage(strToken);
			strToken = strParam1.Tokenize(_T("$$$"), nTokenPos);
		}
	}

	CString CWebPage::FindToken(CString pszContent, CString pszDelimiter, int& nStart)
	{
		if (nStart == -1) {
			return _T("");
		}
		int nNextDelimiter = pszContent.Find(pszDelimiter, nStart);
		if (nNextDelimiter == -1)
		{
			CString pszToken = pszContent.Mid(nStart);
			nStart = -1;
			return pszToken;
		}
		else
		{
			int nCount = nNextDelimiter - nStart;
			CString pszToken = pszContent.Mid(nStart, nCount);
			nStart = nNextDelimiter + pszDelimiter.GetLength();
			if (nStart >= pszContent.GetLength()) {
				nStart = -1;
			}
			return pszToken;
		}
	}

	void CWebPage::HandleSingleMessage(CString strParam)
	{
		int nStart = 0;
		CString strId = FindToken(strParam, _T("%%%"), nStart);
		CString strParam1 = FindToken(strParam, _T("%%%"), nStart);
		CString strParam2 = FindToken(strParam, _T("%%%"), nStart);
		CString strParam3 = FindToken(strParam, _T("%%%"), nStart);
		CString strParam4 = FindToken(strParam, _T("%%%"), nStart);
		CString strParam5 = FindToken(strParam, _T("%%%"), nStart);
		HandleChromeIPCMessage(strId, strParam1, strParam2, strParam3, strParam4, strParam5);
	}

	void CWebPage::CustomizedDOMElement(CString strRuleName, CString strHTML)
	{
		if (strRuleName.CompareNoCase(_T("application")) == 0)
		{
			if (g_pCosmos->m_strAppXml == _T(""))
			{
				g_pCosmos->m_strAppXml = strHTML;
			}
		}
		else if (strRuleName.CompareNoCase(_T("mainWindow")) == 0)
		{
			if (g_pCosmos->m_strMainWndXml == _T(""))
				g_pCosmos->m_strMainWndXml = strHTML;
		}
		else if (strRuleName.CompareNoCase(_T("webBrowser")) == 0)
		{
			CustomizedWebBrowserElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("cloud-form")) == 0)
		{
			CustomizedIndWindowElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("gridTemplate")) == 0)
		{
			CustomizedNodeDetailsElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("object")) == 0)
		{
			CustomizedObjectElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("data")) == 0)
		{
			CustomizedDataElement(strHTML);
		}
		else if (strRuleName.CompareNoCase(_T("webViewPort")) == 0)
		{
			m_strDocXml = strHTML;
		}
		else if (strRuleName.CompareNoCase(_T("extra")) == 0)
		{
			CustomizedExtraElement(strHTML);
		}
		else
		{
			g_pCosmos->m_pCosmosDelegate->CustomizedDOMElement(m_hWnd, strRuleName, strHTML);
		}
	}

	void CWebPage::CustomizedMainWindowElement(CString strHTML)
	{
		CTangramXmlParse xmlParse;
		if (xmlParse.LoadXml(strHTML))
		{
			if (g_pCosmos->m_pCLRProxy == nullptr)
				g_pCosmos->LoadCLR();
			if (g_pCosmos->m_pCLRProxy)
			{
				IWebPage* pChromeWebPage = (IWebPage*)this;
				xmlParse.put_attr(_T("webpage"), (__int64)pChromeWebPage);
				xmlParse.put_attr(_T("webpagehandle"), (__int64)m_hWnd);
				CWebPageImpl* pChromeRenderFrameHostProxyBase = (CWebPageImpl*)this;
				xmlParse.put_attr(_T("renderframehostproxy"), (__int64)pChromeRenderFrameHostProxyBase);
				xmlParse.put_attr(_T("IsMainCosmosWnd"), (__int64)1);
				IDispatch* pDisp = g_pCosmos->m_pCLRProxy->CreateCLRObj(xmlParse.xml());
			}
		}
	}

	void CWebPage::CustomizedIndWindowElement(CString strHTML)
	{
		CTangramXmlParse xmlParse;
		if (xmlParse.LoadXml(strHTML))
		{
			if (g_pCosmos->m_pCLRProxy == nullptr)
				g_pCosmos->LoadCLR();
			if (g_pCosmos->m_pCLRProxy)
			{
				CWebPageImpl* pProxyBase = (CWebPageImpl*)this;
				xmlParse.put_attr(_T("renderframehostproxy"), (__int64)pProxyBase);
				IWebPage* pChromeWebPage = (IWebPage*)this;
				xmlParse.put_attr(_T("webpage"), (__int64)pChromeWebPage);
				xmlParse.put_attr(_T("webpagehandle"), (__int64)m_hWnd);
				IDispatch* pDisp = g_pCosmos->m_pCLRProxy->CreateCLRObj(xmlParse.xml());
			}
		}
	}

	void CWebPage::CustomizedWebBrowserElement(CString strHTML)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strHTML))
		{
			CString strUrls = _T("");
			int nCount = m_Parse.GetCount();
			if (nCount > 0)
			{
				for (int i = 0; i < nCount; i++)
				{
					CString strUrl = m_Parse.GetChild(i)->val();
					int nPos2 = strUrl.Find(_T(":"));
					if (nPos2 != -1)
					{
						CString strURLHeader = strUrl.Left(nPos2);
						if (strURLHeader.CompareNoCase(_T("host")) == 0)
						{
							strUrl = g_pCosmos->m_strAppPath + strUrl.Mid(nPos2 + 1);
						}
					}
					strUrls = strUrls + strUrl + _T("|");
				}
			}
			else
			{
				strUrls = "chrome://newtab|";
			}

			g_pCosmos->m_pBrowserFactory->CreateBrowser(NULL, strUrls);
		}
	}

	void CWebPage::CustomizedNodeDetailsElement(CString strHTML)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strHTML))
		{
			CString strType = m_Parse.attr(_T("type"), _T(""));
			CString strUiKey = m_Parse.attr(_T("uikey"), _T("")).MakeLower();
			if (strUiKey != _T(""))
			{
				if (strType.CompareNoCase(_T("winform")) == 0)
				{
					m_mapFormsInfo[strUiKey] = m_Parse.xml();
				}
			}
		}
	}

	void CWebPage::CustomizedObjectElement(CString strHTML)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strHTML))
		{
			CString strType = m_Parse.attr(_T("type"), _T(""));
			if (strType.CompareNoCase(_T("winform")) == 0)
			{
				CString strStartup = _T("");
				CString strID = m_Parse.attr(_T("uikey"), _T("")).MakeLower();
				CString strName = m_Parse.name();
				if (strID != _T(""))
				{
					m_mapFormsInfo[strID] = m_Parse.xml();
				}
				if (m_Parse.attrBool(_T("showstartup")))
				{
					strStartup = strID;
				}
				if (strStartup != _T(""))
				{
					CString strID = strStartup.MakeLower();
					if (g_pCosmos->m_pCLRProxy == nullptr)
						g_pCosmos->LoadCLR();
					if (g_pCosmos->m_pCLRProxy)
					{
						auto it = m_mapFormsInfo.find(strID);
						if (it != m_mapFormsInfo.end())
						{
							IDispatch* pDisp = g_pCosmos->m_pCLRProxy->CreateCLRObj(it->second);
							HWND hwnd = g_pCosmos->m_pCLRProxy->GetCtrlHandle(pDisp);
							if (hwnd)
							{
								CWinForm* pForm = (CWinForm*)::SendMessage(hwnd, WM_HUBBLE_DATA, 0, 20190214);
								if (pForm)
								{
									pForm->m_pOwnerHtmlWnd = this;
								}
							}
						}
					}
				}
			}
			else if (strType.CompareNoCase(_T("browser")) == 0)
			{
				CString strUrls = _T("");
				int nCount = m_Parse.GetCount();
				for (int i = 0; i < nCount; i++)
				{
					CString strUrl = m_Parse.GetChild(i)->attr(_T("url"), _T(""));
					int nPos2 = strUrl.Find(_T(":"));
					if (nPos2 != -1)
					{
						CString strURLHeader = strUrl.Left(nPos2);
						if (strURLHeader.CompareNoCase(_T("host")) == 0)
						{
							strUrl = g_pCosmos->m_strAppPath + strUrl.Mid(nPos2 + 1);
						}
					}
					strUrls = strUrls + strUrl + _T("|");
				}
				g_pCosmos->m_pBrowserFactory->CreateBrowser(NULL, strUrls);
			}
		}
	}

	void CWebPage::CustomizedExtraElement(CString strHTML)
	{
		CMarkup rootXML;
		if (rootXML.SetDoc(strHTML) && rootXML.FindElem())
		{
			CString strTarget = rootXML.GetAttrib(_T("target"));
			if (strTarget.CompareNoCase(_T("ntp")) == 0)
			{
				g_pCosmos->m_strNtpXml = strHTML;
			}
		}
	}

	void CWebPage::CustomizedDataElement(CString strHTML)
	{
		CMarkup rootXML;
		if (rootXML.SetDoc(strHTML) && rootXML.FindElem())
		{
			CString strKey = rootXML.GetAttrib(_T("key"));
			g_pCosmos->m_mapValInfo[strKey] = CComVariant(strHTML);
		}
	}

	void CWebPage::CustomizedDocElement(CString strHTML)
	{
		CMarkup rootXML;
		if (rootXML.SetDoc(strHTML) && rootXML.FindElem())
		{
			CString strName = rootXML.GetAttrib(_T("name"));
			if (strName == _T(""))
			{
				strName = _T("__VIEWPORT_DEFAULT__");
			}
			LoadDocument2Viewport(strName, strHTML);
			::PostMessage(m_hWnd, WM_COSMOSMSG, 20200311, 0);
		}
	}

	void CWebPage::OnNTPLoaded()
	{
		if (g_pCosmos->m_strNtpXml != _T(""))
		{
			LoadDocument2Viewport(_T("__NTP_DEFAULT__"), g_pCosmos->m_strNtpXml);
		}
	}

	void CWebPage::OnWinFormCreated(HWND hwnd)
	{
		if (hwnd)
		{
			LRESULT l = ::SendMessage(hwnd, WM_HUBBLE_DATA, 0, 20190214);
			if (l == 0)
			{
				CWinForm* pWnd = new CWinForm();
				g_pCosmos->m_hFormNodeWnd = NULL;
				g_pCosmos->m_hFormNodeWnd = (HWND)hwnd;
				pWnd->SubclassWindow(hwnd);
				pWnd->m_pOwnerHtmlWnd = this;
				g_pCosmos->m_mapFormWebPage[hwnd] = this;
				m_mapWinForm[hwnd] = pWnd;
				::PostMessage(g_pCosmos->m_hFormNodeWnd, WM_WINFORMCREATED, 0, 0);
			}
		}
	}

	void CWebPage::OnCloudMsgReceived(CSession* pSession)
	{
		CString strMsgID = pSession->GetString(L"msgID");
		IXobj* pXobj = (IXobj*)pSession->Getint64(_T("xobj"));
		if (pXobj)
		{
			CXobj* pObj = (CXobj*)pXobj;
			if (pObj->m_pWormhole == nullptr)
				pObj->m_pWormhole = (CWormhole*)pSession;
			if (pObj->m_pWebPage == nullptr)
				pObj->m_pWebPage = this;
			if (pObj->m_pXobjShareData->m_pGalaxy->m_pWebPageWnd == nullptr)
				pObj->m_pXobjShareData->m_pGalaxy->m_pWebPageWnd = this;
		}
		if (strMsgID == _T("CREATE_WINFORM"))
		{
			CString strFormXml = pSession->GetString(_T("formXml"));
			pSession->InsertString(_T("formXml"), _T(""));
			// formType: 0, normal form; 1 subform; 2: dlgform
			long formType = pSession->GetLong(_T("formType"));
			CTangramXmlParse xmlParse;
			if (xmlParse.LoadXml(strFormXml))
			{
				if (g_pCosmos->m_pCLRProxy == nullptr)
					g_pCosmos->LoadCLR();
				if (g_pCosmos->m_pCLRProxy)
				{
					CWebPageImpl* pProxyBase = (CWebPageImpl*)this;
					xmlParse.put_attr(_T("renderframehostproxy"), (__int64)pProxyBase);
					xmlParse.put_attr(_T("addsubform"), (int)1);
					xmlParse.put_attr(_T("ipcsession"), (__int64)pSession);
					pSession->Insertint64(_T("domhandle"), (__int64)pSession);
					pSession->InsertLong(_T("autodelete"), 0);
					IWebPage* pChromeWebPage = (IWebPage*)this;
					xmlParse.put_attr(_T("webpage"), (__int64)pChromeWebPage);
					xmlParse.put_attr(_T("webpagehandle"), (__int64)m_hWnd);
					if (formType == 2)
					{
						xmlParse.put_attr(_T("model"), 1);
					}
					pSession->m_pOwner = this;
					IDispatch* pDisp = g_pCosmos->m_pCLRProxy->CreateCLRObj(xmlParse.xml());
				}
			}
		}
		else if(strMsgID == _T("OPEN_URL"))
		{ 
			CString strPath = g_pCosmos->m_strAppPath;
			CString strUrl = pSession->GetString(_T("openurl"));
			long nPos = pSession->GetLong(_T("BrowserWndOpenDisposition"));
			int _nPos = strUrl.Find(_T("host:"));
			if (nPos==2&&_nPos != -1)
				strUrl.Replace(_T("host:"), strPath);
			else
			{
				strUrl.Replace(_T("host:"), strPath);
				strUrl += _T("|");
				strUrl.Replace(_T("||"), _T("|"));
			}

			if (m_pChromeRenderFrameHost)
			{
				if (nPos > 0x000a || nPos == 0)
				{
					if (nPos == 200)
					{
						HWND hWnd = ::CreateWindow(L"Cosmos Xobj Class", NULL, /*WS_OVERLAPPED |*/ WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 200, 200, g_pCosmos->m_hChildHostWnd, 0, AfxGetInstanceHandle(), NULL);
						g_pCosmos->m_hTempBrowserWnd = g_pCosmos->m_pBrowserFactory->CreateBrowser(hWnd, strUrl);
						::SetWindowPos(g_pCosmos->m_hTempBrowserWnd, HWND_BOTTOM, 0, 0, 200, 200, SWP_NOACTIVATE);
					}
					else
						g_pCosmos->m_pBrowserFactory->CreateBrowser(0, strUrl);
				}
				else
				{
					IPCMsg msg;
					msg.m_strId = L"OPEN_URL";
					msg.m_strParam1 = strUrl;
					CString strDisposition = _T("");
					strDisposition.Format(_T("%d"), nPos);
					msg.m_strParam2 = strDisposition;
					m_pChromeRenderFrameHost->SendCosmosMessage(&msg);
				}
			}
		}
		else if (strMsgID == _T("OPEN_XML"))
		{
			CString strKey = pSession->GetString(_T("openkey"));
			CString strXml = pSession->GetString(_T("openxml"));
			if (pXobj)
			{
				IXobj* _pXobj = nullptr;
				pXobj->Observe(CComBSTR(strKey), CComBSTR(strXml), &_pXobj);
				if (_pXobj)
				{
					__int64 nHandle = 0;
					_pXobj->get_Handle(&nHandle);
					pSession->Insertint64(_T("openxmlreturnhandle"), nHandle);
					::PostMessage(m_hWnd, WM_COSMOSMSG, 20200429, (LPARAM)pSession);
				}
			}
		}
		else if (strMsgID == _T("OPEN_XML_SPLITTER"))
		{
			CString strKey = pSession->GetString(_T("openkey"));
			CString strXml = pSession->GetString(_T("openxml"));
			int nCol = pSession->GetLong(_T("opencol"));
			int nRow = pSession->GetLong(_T("openrow"));
			HWND hWnd = (HWND)pSession->Getint64(_T("xobjhandle"));
			if (pXobj)
			{
				IXobj* _pXobj = nullptr;
				pXobj->ObserveEx(nRow, nCol, CComBSTR(strKey), CComBSTR(strXml), &_pXobj);
				if (_pXobj)
				{
					__int64 nHandle = 0;
					_pXobj->get_Handle(&nHandle);
					pSession->Insertint64(_T("openxmlreturnhandle"), nHandle);
					::PostMessage(m_hWnd, WM_COSMOSMSG, 20200429, (LPARAM)pSession);
				}
			}
		}
		else if (strMsgID == _T("OPEN_XML_CTRL"))
		{
			CString strName = pSession->GetString(_T("ctrlName"));
			CString strKey = pSession->GetString(_T("openkey"));
			CString strXml = pSession->GetString(_T("openxml"));
		}
		else if (strMsgID == _T("SET_REFGRIDS_IPC_MSG"))
		{
			CString strXml = pSession->GetString(_T("RefInfo"));
			((CXobj*)pXobj)->m_strXmlRefXobjInfo = strXml;
		}
	}

	STDMETHODIMP CWebPage::get_HostWnd(LONGLONG* Val)
	{
		*Val = (LONGLONG)m_hWebHostWnd;
		return S_OK;
	}

	STDMETHODIMP CWebPage::put_HostWnd(LONGLONG newVal)
	{
		HWND hWnd = (HWND)newVal;
		if (::IsWindow(hWnd))
			m_hWebHostWnd = hWnd;
		return S_OK;
	}

	STDMETHODIMP CWebPage::CreateForm(BSTR bstrKey, LONGLONG hParent, IDispatch** pRetForm)
	{
		if (g_pCosmos->m_pCLRProxy == nullptr)
			g_pCosmos->LoadCLR();
		if (hParent == 0)
			hParent = (__int64)m_hWnd;
		CString strKey = OLE2T(bstrKey);
		auto it = this->m_mapFormsInfo.find(strKey.MakeLower());
		if (it != m_mapFormsInfo.end())
		{
			*pRetForm = g_pCosmos->m_pCLRProxy->CreateWinForm((HWND)hParent, CComBSTR(it->second));
		}
		return S_OK;
	}

	STDMETHODIMP CWebPage::SendXmlMessage(IXobj* sender, BSTR bstrXml)
	{
		CXobj* pXobj = (CXobj*)sender;
		if (pXobj)
		{
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(OLE2T(bstrXml)))
			{
				CTangramXmlParse* pChild = m_Parse.GetChild(_T("webpage"));
				if (pChild)
				{
					CString strMsgID = m_Parse.attr(_T("msgID"), _T(""));
					int nCount = pChild->GetCount();
					if (nCount)
					{
						for (int i = 0; i < nCount; i++)
						{
							CString strName = pChild->GetChild(i)->name();
							auto it = pXobj->m_pRootObj->m_mapChildXobj.find(strName);
							if (it != pXobj->m_pRootObj->m_mapChildXobj.end())
							{
								CXobj* pTarget = it->second;
								if (pTarget)
								{
									if (pXobj->m_pWormhole == nullptr)
									{
										pXobj->m_pWormhole = (CWormhole*)((CCosmosImpl*)g_pCosmos)->CreateCloudSession(this);
										CWormhole* pSession = pXobj->m_pWormhole;
										if (pSession)
										{
											pSession->InsertString(_T("msgID"), strMsgID);
											pSession->InsertLong(_T("autodelete"), 0);
											pSession->InsertLong(_T("gridtype"), pXobj->m_nViewType);
											pSession->InsertLong(_T("rows"), pXobj->m_nRows);
											pSession->InsertLong(_T("cols"), pXobj->m_nCols);
											pSession->InsertLong(_T("row"), pXobj->m_nRow);
											pSession->InsertLong(_T("col"), pXobj->m_nCol);
											pSession->InsertString(_T("objtype"), pXobj->m_strObjTypeID);
											pSession->InsertString(_T("name@page"), pXobj->m_strName);
											pSession->Insertint64(_T("xobjhandle"), (__int64)pXobj->m_pHostWnd->m_hWnd);
											pSession->Insertint64(_T("xobj"), (__int64)(IXobj*)pXobj);
											pSession->Insertint64(_T("Galaxyhandle"), (__int64)pXobj->m_pXobjShareData->m_pGalaxy->m_hWnd);
											pSession->InsertString(_T("galaxy"), pXobj->m_pXobjShareData->m_pGalaxy->m_strGalaxyName);
											pSession->InsertString(_T("cluster"), pXobj->m_pRootObj->m_strKey);
											pSession->Insertint64(_T("rootgridhandle"), (__int64)pXobj->m_pRootObj->m_pHostWnd->m_hWnd);
											pSession->Insertint64(_T("domhandle"), (__int64)pXobj->m_pWormhole);
											pSession->InsertString(_T("objID"), _T("wndnode"));
											switch (pXobj->m_nViewType)
											{
											case Grid:
											{
												CGridWnd* pWnd = (CGridWnd*)pXobj->m_pHostWnd;
												for (int i = 0; i < pXobj->m_nRows; i++)
												{
													for (int j = 0; j < pXobj->m_nCols; j++)
													{
														int nIndex = j + i * pXobj->m_nRows;
														CString strIndex = _T("");
														strIndex.Format(_T("%d"), nIndex);
														pSession->Insertint64(strIndex, (__int64)::GetDlgItem(pWnd->m_hWnd, pWnd->IdFromRowCol(i, j)));
													}
												}
											}
											break;
											case TabGrid:
											{
												for (auto it : pXobj->m_vChildNodes)
												{
													CString strIndex = _T("");
													strIndex.Format(_T("%d"), it->m_nCol);
													pSession->Insertint64(strIndex, (__int64)it->m_pHostWnd->m_hWnd);
												}
											}
											break;
											default:
												break;
											}
											if (pXobj->m_pParentObj)
											{
												pSession->Insertint64(_T("parenthandle"), (__int64)pXobj->m_pParentObj->m_pHostWnd->m_hWnd);
											}
											if (pXobj->m_pParentWinFormWnd)
											{
												pSession->Insertint64(_T("parentFormHandle"), (__int64)pXobj->m_pParentWinFormWnd->m_hWnd);
											}
											if (pXobj->m_pDisp)
											{
												pXobj->m_pWormhole->Insertint64(_T("objectdisp"), (__int64)pXobj->m_pDisp);
												if (g_pCosmos->m_pCLRProxy)
												{
													g_pCosmos->m_pCLRProxy->ConnectXobjToWebPage(pXobj, true);
												}
											}
											pXobj->m_pWormhole->InsertString(_T("msgData"), pChild->GetChild(i)->xml());
											m_pChromeRenderFrameHost->SendCosmosMessage(pSession->m_pSession);
										}
									}
									else
									{
										pXobj->m_pWormhole->InsertString(_T("msgID"), strMsgID);
										pXobj->m_pWormhole->InsertString(_T("msgData"), pChild->GetChild(i)->xml());
										m_pChromeRenderFrameHost->SendCosmosMessage(pXobj->m_pWormhole->m_pSession);
									}
								}
							}
						}
					}
				}
				pChild = m_Parse.GetChild(_T("parentform"));
				if (pChild)
				{
					int nCount = pChild->GetCount();
					if (nCount)
					{
						for (int i = 0; i < nCount; i++)
						{

						}
					}
				}
			}
		}
		return S_FALSE;
	}

	STDMETHODIMP CWebPage::Observe(BSTR bstrKey, BSTR bstrXml, IXobj** pRetXobj)
	{
		if (m_pGalaxy == nullptr) {
			CGalaxyCluster* pGalaxyCluster = nullptr;
			auto it = g_pCosmos->m_mapWindowPage.find(m_hExtendWnd);
			if (it != g_pCosmos->m_mapWindowPage.end())
				pGalaxyCluster = (CGalaxyCluster*)it->second;
			else
			{
				pGalaxyCluster = new CComObject<CGalaxyCluster>();
				pGalaxyCluster->m_hWnd = m_hExtendWnd;
				g_pCosmos->m_mapWindowPage[m_hExtendWnd] = pGalaxyCluster;
			}
			if (pGalaxyCluster) {
				IGalaxy* pGalaxy = nullptr;
				pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)m_hChildWnd), CComBSTR("default"), &pGalaxy);
				if (pGalaxy)
				{
					m_pGalaxy = (CGalaxy*)pGalaxy;
					m_pGalaxy->m_pWebPageWnd = this;
				}
			}
		}
		if (m_pGalaxy)
			return m_pGalaxy->Observe(bstrKey, bstrXml, pRetXobj);
		return S_OK;
	}
}  
