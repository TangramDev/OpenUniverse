/********************************************************************************
 *           Web Runtime for Application - Version 1.0.1.202105020001           *
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

#include "../stdafx.h"
 //#include <afxMDIFrameWndEx.h>
#include "../UniverseApp.h"
#include "../Xobj.h"
#include "../Wormhole.h"
#include "../Galaxy.h"
#include "../XobjWnd.h"
#include "../GridWnd.h"
#include "../Markup.h" 
#include "WebPage.h"
#include "Browser.h"

namespace Browser {
	CExtendWnd::CExtendWnd(void)
	{
	}

	void CExtendWnd::OnFinalMessage(HWND hWnd)
	{
		CWindowImpl<CExtendWnd, CWindow>::OnFinalMessage(hWnd);
		delete this;
	}

	LRESULT CExtendWnd::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		CDC* dc = CDC::FromHandle((HDC)wParam);
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		dc->FillSolidRect(0, 0, rc.right, rc.bottom, RGB(255, 255, 255));

		//HDC hDC = ::GetDC(m_pHostPage->m_hWnd);
		//CDC* pDC = CDC::FromHandle(hDC);

		//RECT rc;
		//::GetClientRect(m_pHostPage->m_hWnd, &rc);

		////CDC memDC;
		////CBitmap bitmap;
		////memDC.CreateCompatibleDC(pDC);
		////bitmap.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
		////dc->SelectObject(&bitmap);
		////CDC dcMemory;
		////dcMemory.CreateCompatibleDC(pDC);

		////// Select the bitmap into the in-memory DC
		////CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);
		//dc->BitBlt(0, 0, rc.right, rc.bottom, pDC, 0, 0, SRCCOPY);

		//CBrush br(&bitmap);
		//RECT rt;
		//GetClientRect(&rt);
		//dc->FillRect(&rt, &br);

		//memDC.DeleteDC();
		//bitmap.DeleteObject();

		//::ReleaseDC(m_pHostPage->m_hWnd, hDC);

		//HDC hDC = ::GetDC(m_pHostPage->m_hWnd);
		//CDC* pDC = CDC::FromHandle(hDC);
		//RECT rc;
		//::GetClientRect(m_pHostPage->m_hWnd, &rc);
		//CDC memDC;
		//CBitmap bitmap;
		//memDC.CreateCompatibleDC(pDC);
		//bitmap.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
		//memDC.SelectObject(&bitmap);
		//memDC.BitBlt(0, 0, rc.right, rc.bottom, pDC, 0, 0, SRCCOPY);

		//CBrush br(&bitmap);
		//RECT rt;
		//GetClientRect(&rt);
		//dc->FillRect(&rt, &br);

		//memDC.DeleteDC();
		//bitmap.DeleteObject();

		//RECT rc;
		//::GetClientRect(m_hWnd, &rc);
		//dc->FillSolidRect(0, 0, rc.right, rc.bottom, RGB(255, 255, 255));
		return true;
	}

	CWebView::CWebView() {
		m_pWebWnd = nullptr;
		m_pDevToolWnd = nullptr;
		m_pBindWinForm = nullptr;
		m_pAppProxy = nullptr;
		m_bDevToolWnd = false;
		m_strCurKey = _T("");
		m_strCurXml = _T("");
		m_strDocXml = _T("");
		m_pParentXobj = nullptr;
		m_strAppProxyID = _T("");
		m_strLoadingURLs = _T("");
		m_pBindXobj = nullptr;
		m_pGalaxyCluster = nullptr;
		m_pGalaxy = nullptr;
		m_hWebHostWnd = m_hExtendWnd = m_hChildWnd = NULL;
		m_pChromeRenderFrameHost = g_pCosmos->m_pCreatingChromeRenderFrameHostBase;
		g_pCosmos->m_pCreatingChromeRenderFrameHostBase = nullptr;
	}

	CWebView::~CWebView() {
		for (auto it : m_mapChildFormsInfo)
		{
			delete it.second;
		}
	}

	LRESULT CWebView::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
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
			return lRes;
		}
		else
			g_pCosmos->m_pActiveHtmlWnd = nullptr;
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CWebView::OnCosmosMsg(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL&) {
		switch (wParam)
		{
		case 20210411:
		{
			if (lParam)
			{
				g_pCosmos->m_nWaitTabCounts = 0;
				g_pCosmos->m_hWaitTabWebPageWnd = NULL;
				m_strLoadingURLs = _T("");
				theApp.m_bAppStarting = false;
				m_bCanShow = true;
				::SendMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 1);
				break;
			}
			if (m_strLoadingURLs != _T(""))
			{
				HWND hPWnd = ::GetParent(m_hWnd);
				auto it = g_pCosmos->m_mapBrowserWnd.find(hPWnd);
				if (it != g_pCosmos->m_mapBrowserWnd.end())
				{
					CBrowser* pWebBrowser = (CBrowser*)it->second;
					pWebBrowser->m_bSZMode = true;
					m_bCanShow = false;
					pWebBrowser->m_bInTabChange = true;
					theApp.m_bAppStarting = true;
					CTangramXmlParse m_Parse;
					if (m_Parse.LoadXml(m_strLoadingURLs))
					{
						CString strUrls = _T("");
						int nCount = m_Parse.GetCount();
						for (int i = 0; i < nCount; i++)
						{
							CString strURL = m_Parse.GetChild(i)->attr(_T("url"), _T(""));
							int nPos2 = strURL.Find(_T(":"));
							if (nPos2 != -1)
							{
								CString strURLHeader = strURL.Left(nPos2);
								if (strURLHeader.CompareNoCase(_T("host")) == 0)
								{
									strURL = g_pCosmos->m_strAppPath + strURL.Mid(nPos2 + 1);
								}
							}
							if (strURL != _T(""))
							{
								strUrls += strURL;
								if (i < nCount - 1)
									strUrls += _T("|");
							}
						}
						if (strUrls != _T(""))
						{
							g_pCosmos->m_nWaitTabCounts = nCount;
							g_pCosmos->m_hWaitTabWebPageWnd = m_hWnd;
							CString strDisposition = _T("");
							strDisposition.Format(_T("%d"), NEW_BACKGROUND_TAB);
							if (m_pChromeRenderFrameHost)
							{
								IPCMsg msg;
								msg.m_strId = L"ADD_URL";
								msg.m_strParam1 = strUrls;
								msg.m_strParam2 = strDisposition;
								m_pChromeRenderFrameHost->SendCosmosMessage(&msg);
							}
						}
					}
				}
			}
		}
		break;
		case 20201109:
		{
			if (lParam)
			{
				if (::IsWindow(m_hExtendWnd))
				{
					if (::IsChild(m_hWnd, m_hExtendWnd))
					{
						::SetParent(m_hExtendWnd, ::GetParent(m_hWnd));
						::ShowWindow(m_hExtendWnd, SW_SHOW);
					}
					//if (m_pChromeRenderFrameHost)
					//	m_pChromeRenderFrameHost->ShowWebPage(true);
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
				if (pXobj->m_pXobjShareData->m_pGalaxy->m_pWormhole == nullptr)
				{
					pXobj->m_pXobjShareData->m_pGalaxy->m_pWormhole = pXobj->m_pWormhole;
				}
				CWormhole* pSession = pXobj->m_pWormhole;
				if (pSession)
				{
					pSession->InsertString(_T("msgID"), IPC_NODE_CREARED_ID);
					CString strDocType = pXobj->m_pXobjShareData->m_pGalaxy->m_strDocTemplateID;
					if (strDocType == _T(""))
					{
						strDocType = g_pCosmos->m_pUniverseAppProxy->QueryDocType(pXobj->m_pHostWnd->m_hWnd);
						pXobj->m_pXobjShareData->m_pGalaxy->m_strDocTemplateID = strDocType;
					}
					pSession->InsertString(_T("msgID"), IPC_NODE_CREARED_ID);
					pSession->InsertString(_T("DocTypeID"), strDocType);
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
					if (pXobj->m_pXobjShareData->m_pGalaxy->m_strGalaxyName == _T("default"))
					{
						CString strName = pXobj->m_pRootObj->m_pHostParse->attr(_T("galaxy"), _T(""));
						if (strName != _T(""))
							pXobj->m_pXobjShareData->m_pGalaxy->m_strGalaxyName = strName;
					}
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

					CCloudWinForm* pMDIPForm = nullptr;
					bool bMDIChild = false;
					{
						pXobj->m_pRootObj->m_pXobjShareData->m_pGalaxy->GetWinForm(pXobj->m_pRootObj->m_pXobjShareData->m_pGalaxy->m_hWnd);
						if (pXobj->m_pRootObj->m_pParentWinFormWnd)
						{
							pXobj->m_pParentWinFormWnd = pXobj->m_pRootObj->m_pParentWinFormWnd;
							{
								HWND hPWnd = pXobj->m_pRootObj->m_pParentWinFormWnd->m_hWnd;
								if ((::GetWindowLong(hPWnd, GWL_EXSTYLE) & WS_EX_MDICHILD))
								{
									bMDIChild = true;
									hPWnd = ::GetParent(::GetParent(hPWnd));
									pMDIPForm = (CCloudWinForm*)::SendMessage(hPWnd, WM_HUBBLE_DATA, 0, 20190214);
								}
							}
						}
					}

					if (pXobj->m_pParentWinFormWnd)
					{
						pSession->Insertint64(_T("parentFormHandle"), (__int64)pXobj->m_pParentWinFormWnd->m_hWnd);
					}
					if (bMDIChild && pMDIPForm)
					{
						pSession->Insertint64(_T("parentMDIFormHandle"), (__int64)pMDIPForm->m_hWnd);
						IGalaxy* pGalaxy = nullptr;
						CCosmosImpl* _pCosmosImpl = static_cast<CCosmosImpl*>(g_pCosmos);
						auto it = _pCosmosImpl->m_mapGalaxyCluster.find(pMDIPForm->m_hWnd);
						if (it != _pCosmosImpl->m_mapGalaxyCluster.end())
						{
							it->second->get_Galaxy(CComVariant(L"mdiclient"), &pGalaxy);
							if (pGalaxy)
							{
								IXobj* _pXobj = nullptr;
								pGalaxy->get_VisibleXobj(&_pXobj);
								__int64 nHandle = 0;
								if (_pXobj)
								{
									_pXobj->get_Handle(&nHandle);
									pSession->Insertint64(_T("BindMdiXobjHandle"), nHandle);
									if (pXobj->m_pParentWinFormWnd)
									{
										pXobj->m_pParentWinFormWnd->m_pBindMDIXobj = (CXobj*)_pXobj;
									}
								}
								if (m_pGalaxy && m_pGalaxy->m_pWorkXobj && pXobj->m_pParentWinFormWnd)
								{
									CString strKey = pXobj->m_pParentWinFormWnd->m_strKey;
									auto it = m_pGalaxy->m_mapXobj.find(strKey);// 
									if (it != m_pGalaxy->m_mapXobj.end())
									{
										it->second->get_Handle(&nHandle);
										if (nHandle)
											pSession->Insertint64(_T("mdiwebbindgridhandle"), nHandle);
										if (pXobj->m_pParentWinFormWnd)
										{
											pXobj->m_pParentWinFormWnd->m_pWebBindMDIXobj = it->second;
										}
									}
								}
							}
						}
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
			if (m_pGalaxy && m_pGalaxy->m_pWorkXobj)
			{
				if (m_pGalaxy->m_pWorkXobj->m_pWormhole)
				{
					m_pGalaxy->m_pWorkXobj->m_pWormhole->InsertString(_T("msgID"), _T("TANGRAMAPP_READY"));
					m_pGalaxy->m_pWorkXobj->m_pWormhole->SendMessage();
				}
			}
		}
		break;
		case 20200131:
		{
			if (m_pChromeRenderFrameHost)
			{
				OutputDebugString(_T("------------------WebPage Begin PBT_APMRESUMEAUTOMATIC------------------------\n"));
				HWND hBrowser = m_pChromeRenderFrameHost->GetHostBrowserWnd();
				::SetParent(m_hExtendWnd, hBrowser);
				m_pChromeRenderFrameHost->ShowWebPage(true);
				//::SendMessage(hBrowser, WM_BROWSERLAYOUT, 0, 4);
				OutputDebugString(_T("------------------WebPage End PBT_APMRESUMEAUTOMATIC------------------------\n"));
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
				CCloudWinForm* pWnd = (CCloudWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
				if (pWnd)
				{
					m_mapSubWinForm[hWnd] = pWnd;
				}
			}
		}
		break;
		case 19820911:
		{
			Show(_T(""));
		}
		break;
		case 20190115:
		{
			if (g_pCosmos->m_mapCreatingWorkBenchInfo.size())
			{
				::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 20191114);
			}
		}
		break;
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebView::OnParentChanged(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL&) {
		if (g_pCosmos->m_bChromeNeedClosed == false && lParam) {
			HWND hNewPWnd = (HWND)lParam;
			::GetClassName(hNewPWnd, g_pCosmos->m_szBuffer, 256);
			CString strName = g_pCosmos->m_szBuffer;
			if (strName.Find(_T("Chrome_WidgetWin_0")) == 0)
			{
				if (m_hExtendWnd)
				{
					::SetParent(m_hExtendWnd, m_hWnd);
					::ShowWindow(m_hExtendWnd, SW_HIDE);
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
					pChromeBrowserWnd->m_bSZMode = true;
					g_pCosmos->m_pActiveBrowser = pChromeBrowserWnd->m_pBrowser;
					if (pChromeBrowserWnd && m_hExtendWnd) {
						if (::IsWindowVisible(m_hWnd)) {
							pChromeBrowserWnd->m_pVisibleWebView = this;
							if (bNewParent)
							{
								g_pCosmos->m_pActiveBrowser->m_pProxy = pChromeBrowserWnd;
								::PostMessageW(hNewPWnd, WM_BROWSERLAYOUT, 0, 7);
							}
							else
							{
								if (pChromeBrowserWnd->m_bInTabChange)
								{
									::SendMessageW(pChromeBrowserWnd->m_hWnd, WM_BROWSERLAYOUT, 0, 1000);
								}
							}
						}
					}
					if (pChromeBrowserWnd->m_pVisibleWebView && m_bDevToolWnd && pChromeBrowserWnd->m_pVisibleWebView->m_bDevToolWnd == false)
					{
						if (pChromeBrowserWnd->m_pVisibleWebView->m_pDevToolWnd == nullptr)
						{
							pChromeBrowserWnd->m_pVisibleWebView->m_pDevToolWnd = this;
							::ShowWindow(m_hWnd, SW_SHOW);
						}
					}
				}
			}
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebView::OnDestroy(UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL& /*bHandled*/) {
		if (m_hExtendWnd)
		{
			::DestroyWindow(m_hExtendWnd);
		}

		if (g_pCosmos->m_pCLRProxy)
			g_pCosmos->m_pCLRProxy->OnWebPageCreated(m_hWnd, (CWebPageImpl*)this, (IWebPage*)this, 1);

		m_hExtendWnd = nullptr;

		if (m_bDevToolWnd) {
			if (m_pWebWnd) {
				m_pWebWnd->m_pDevToolWnd = nullptr;
			}
		}
		if (m_mapSubWinForm.size())
		{
			for (auto& it : m_mapSubWinForm)
			{
				::DestroyWindow(it.first);
			}
			m_mapSubWinForm.erase(m_mapSubWinForm.begin(), m_mapSubWinForm.end());
		}
		if (m_mapWinForm.size())
		{
			while (m_mapWinForm.size())
			{
				auto it = m_mapWinForm.begin();
				CCloudWinForm* pForm = it->second;
				if (pForm->m_bMainForm == false)
				{
					if (::IsChild(pForm->m_hWnd, m_hWnd)||::IsWindow(pForm->m_hWnd)==false)
					{
						m_mapWinForm.erase(it);
					}
					else
						::DestroyWindow(pForm->m_hWnd);
				}
				else
				{
					pForm->m_pOwnerHtmlWnd = nullptr;
					m_mapWinForm.erase(it);
				}
				if (m_mapWinForm.size() == 0)
					break;
			}
			m_mapWinForm.erase(m_mapWinForm.begin(), m_mapWinForm.end());
		}
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CWebView::OnShowWindow(UINT uMsg,
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
						pBrowserWnd->m_pVisibleWebView = this;
					}
				}
				::ShowWindow(m_hExtendWnd, SW_SHOW);
				::SetParent(m_hExtendWnd, hPWnd);
				if (pBrowserWnd)
				{
					//if (m_pChromeRenderFrameHost)
					//	m_pChromeRenderFrameHost->ShowWebPage(true);
					if (::GetParent(hPWnd) == nullptr)
					{
						pBrowserWnd->BrowserLayout();
						::PostMessage(hPWnd, WM_BROWSERLAYOUT, 0, 2);
					}
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

	void CWebView::OnFinalMessage(HWND hWnd) {
		CBrowser* pPWnd = nullptr;
		auto it2 = g_pCosmos->m_mapBrowserWnd.find(::GetParent(hWnd));
		if (it2 != g_pCosmos->m_mapBrowserWnd.end())
		{
			pPWnd = (CBrowser*)it2->second;
			if (pPWnd->m_pVisibleWebView == this)
				pPWnd->m_pVisibleWebView = nullptr;
			auto it3 = pPWnd->m_mapChildPage.find(hWnd);
			if (it3 != pPWnd->m_mapChildPage.end())
			{
				pPWnd->m_mapChildPage.erase(it3);
			}
		}
		auto it = g_pCosmos->m_mapWebView.find(hWnd);
		if (it != g_pCosmos->m_mapWebView.end())
		{
			g_pCosmos->m_mapWebView.erase(it);
		}
		if (g_pCosmos->m_pActiveHtmlWnd == this)
			g_pCosmos->m_pActiveHtmlWnd = nullptr;
		CWindowImpl::OnFinalMessage(hWnd);
		delete this;
	}

	void CWebView::Show(CString strID2)
	{
		LONG_PTR data = 0;
		if (::IsWindow(m_hChildWnd))
			data = ::GetWindowLongPtr(m_hChildWnd, GWLP_USERDATA);

		if (data == 0 && g_pCosmos->m_pUniverseAppProxy)
		{
			if (::IsChild(m_hExtendWnd, m_hChildWnd))
			{
				m_strAppProxyID = _T("");
				auto it = g_pCosmos->m_mapGalaxyCluster.find(m_hExtendWnd);
				if (it != g_pCosmos->m_mapGalaxyCluster.end())
					m_pGalaxyCluster = (CGalaxyCluster*)it->second;
				else
				{
					m_pGalaxyCluster = new CComObject<CGalaxyCluster>();
					m_pGalaxyCluster->m_hWnd = m_hWnd;
					g_pCosmos->m_mapGalaxyCluster[m_hWnd] = m_pGalaxyCluster;

					for (auto it : g_pCosmos->m_mapCosmosAppProxy)
					{
						CGalaxyClusterProxy* pCosmosProxy = it.second->OnGalaxyClusterCreated(m_pGalaxyCluster);
						if (pCosmosProxy)
							m_pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pCosmosProxy;
					}
				}

				if (m_strAppProxyID != _T(""))
				{
					m_pAppProxy = nullptr;
					auto it = g_pCosmos->m_mapCosmosAppProxy.find(m_strAppProxyID.MakeLower());
					if (it != g_pCosmos->m_mapCosmosAppProxy.end())
						m_pAppProxy = it->second;
					else
					{
						CString strPath = g_pCosmos->m_strAppPath;
						HMODULE hHandle = nullptr;
						CString strAppName = _T("");
						int nPos = m_strAppProxyID.Find(_T("."));
						if (nPos != -1)
							strAppName = m_strAppProxyID.Left(nPos);
						CString strdll = strPath + m_strAppProxyID + _T("\\") + strAppName + _T(".dll");
						if (::PathFileExists(strdll))
							hHandle = ::LoadLibrary(strdll);
						if (hHandle)
						{
							it = g_pCosmos->m_mapCosmosAppProxy.find(m_strAppProxyID.MakeLower());
							if (it != g_pCosmos->m_mapCosmosAppProxy.end())
							{
								m_pAppProxy = it->second;
							}
						}
					}
					if (m_pAppProxy)
					{
						g_pCosmos->m_pActiveAppProxy = m_pAppProxy;
						m_pAppProxy->m_hCreatingView = m_hChildWnd;
						::SetWindowText(m_hChildWnd, g_pCosmos->m_strAppKey);
					}
				}
				else
				{
					IGalaxy* pGalaxy = nullptr;
					m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)m_hChildWnd), CComBSTR(L"default"), &pGalaxy);
					if (pGalaxy)
					{
						m_pGalaxy = (CGalaxy*)pGalaxy;
						//m_pGalaxyCluster->m_mapNeedSaveGalaxy[m_hChildWnd] = m_pGalaxy;
						IXobj* pXobj = nullptr;
						pGalaxy->Observe(CComBSTR("default"), CComBSTR(L""), &pXobj);
					}
				}
			}
		}
	}

	void CWebView::SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5)
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

	LRESULT CWebView::OnChromeIPCMsgReceived(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	CChromeBrowserBase* CWebView::GetChromeBrowserBase(HWND hHostWnd)
	{
		return nullptr;
	}

	void CWebView::LoadDocument2Viewport(CString strName, CString strXML)
	{
		HWND hBrowser = ::GetParent(m_hWnd);
		HWND hPPWnd = ::GetParent(hBrowser);
		CCloudMDTFrame* pMDTWnd = nullptr;
		CCloudMDIFrame* pMDIWnd = nullptr;
		CBrowser* pBrowserWnd = nullptr;
		auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
		if (it != g_pCosmos->m_mapBrowserWnd.end())
		{
			pBrowserWnd = (CBrowser*)it->second;
			if (pBrowserWnd->m_pBrowser->GetActiveWebContentWnd() != m_hWnd)
				::ShowWindow(m_hWnd, SW_HIDE);
			if (pBrowserWnd->m_pCosmosFrameWndInfo == nullptr)
			{
				CosmosInfo* pInfo = (CosmosInfo*)::GetProp(hPPWnd, _T("CosmosInfo"));
				if (pInfo)
				{
					pBrowserWnd->m_pParentXobj = (CXobj*)pInfo->m_pXobj;
					HWND hPWnd = pBrowserWnd->m_pParentXobj->m_pXobjShareData->m_pGalaxy->m_hWnd;
					hPWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(DocView, hPWnd);
					if (hPWnd)
					{
						pBrowserWnd->m_pCosmosFrameWndInfo = (CosmosFrameWndInfo*)::GetProp(hPWnd, _T("CosmosFrameWndInfo"));;
						switch (pBrowserWnd->m_pCosmosFrameWndInfo->m_nFrameType)
						{
						case 1:
						{
							auto it = g_pCosmos->m_mapMDTWindow.find(hPWnd);
							if (it != g_pCosmos->m_mapMDTWindow.end())
							{
								pMDTWnd = it->second;
								pMDTWnd->m_pBrowser = pBrowserWnd;
							}
						}
						break;
						case 2:
						{
							auto it = g_pCosmos->m_mapMDIParent.find(hPWnd);
							if (it != g_pCosmos->m_mapMDIParent.end())
							{
								it->second->m_pHostBrowser = pBrowserWnd;
								pMDIWnd = it->second;
							}
						}
						break;
						}
					}
				}
			}
		}
		if (m_pCosmosFrameWndInfo == nullptr)
		{
			m_pCosmosFrameWndInfo = pBrowserWnd->m_pCosmosFrameWndInfo;
		}

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

			m_hExtendWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, hParent, NULL, theApp.m_hInstance, NULL);
			m_hChildWnd = ::CreateWindowEx(NULL, _T("Chrome Extended Window Class"), L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, m_hExtendWnd, (HMENU)1, theApp.m_hInstance, NULL);
			CExtendWnd* pExtendWnd = new CExtendWnd();
			pExtendWnd->m_pHostPage = this;
			pExtendWnd->SubclassWindow(m_hChildWnd);
			::SetWindowLongPtr(m_hExtendWnd, GWLP_USERDATA, (LONG_PTR)m_hChildWnd);
			::SetWindowLongPtr(m_hChildWnd, GWLP_USERDATA, (LONG_PTR)this);
		}
		if (m_hExtendWnd)
		{
			if (m_pGalaxy == nullptr) {
				CGalaxyCluster* pGalaxyCluster = nullptr;
				auto it = g_pCosmos->m_mapGalaxyCluster.find(m_hExtendWnd);
				if (it != g_pCosmos->m_mapGalaxyCluster.end())
					pGalaxyCluster = (CGalaxyCluster*)it->second;
				else
				{
					pGalaxyCluster = new CComObject<CGalaxyCluster>();
					pGalaxyCluster->m_hWnd = m_hExtendWnd;
					g_pCosmos->m_mapGalaxyCluster[m_hExtendWnd] = pGalaxyCluster;

					for (auto& it : g_pCosmos->m_mapCosmosAppProxy)
					{
						CGalaxyClusterProxy* pProxy = it.second->OnGalaxyClusterCreated(pGalaxyCluster);
						if (pProxy)
							pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pProxy;
					}
				}
				if (pGalaxyCluster) {
					IGalaxy* pGalaxy = nullptr;
					pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)m_hChildWnd), CComBSTR("webviewport"), &pGalaxy);
					if (pGalaxy)
					{
						m_pGalaxy = (CGalaxy*)pGalaxy;
						m_pGalaxy->m_pWebPageWnd = this;
					}
				}
			}
			if (m_pGalaxy)
			{
				if (pMDIWnd && m_pGalaxy->m_pMDIParent == nullptr)
					m_pGalaxy->m_pMDIParent = pMDIWnd;
				IXobj* pXobj = nullptr;
				CComBSTR bstrKey(strName);
				m_pGalaxy->m_pWebPageWnd = this;
				m_pGalaxy->Observe(bstrKey, CComBSTR(strXML), &pXobj);
				if (pXobj)
				{
					m_strCurKey = strName;
					m_hWebHostWnd = NULL;
					if (m_pGalaxy->m_pBindingXobj)
					{
						m_hWebHostWnd = m_pGalaxy->m_pBindingXobj->m_pHostWnd->m_hWnd;
					}
					if (pMDTWnd && pMDTWnd->m_bCreateNewDoc)
					{
						::PostMessage(pMDTWnd->m_hWnd, WM_COSMOSMSG, 0, 20210328);
						return;
					}
					if (m_pGalaxy->m_pParentMDIWinForm && m_pGalaxy->m_pParentMDIWinForm->m_pClientGalaxy)
					{
						CXobj* pMDIClientObj = m_pGalaxy->m_pWorkXobj->GetVisibleChildByName(_T("mdiclient"));
						if (m_pGalaxy->m_pParentMDIWinForm->m_pClientGalaxy->m_pBindingXobj != pMDIClientObj)
						{
							m_pGalaxy->m_pParentMDIWinForm->m_pClientGalaxy->m_pBindingXobj = pMDIClientObj;
							m_pGalaxy->m_pParentMDIWinForm->m_pClientGalaxy->HostPosChanged();
						}
					}
				}
			}
		}

		g_pCosmos->m_mapSizingBrowser[hBrowser] = pBrowserWnd;
		if (hPPWnd == nullptr)
			pBrowserWnd->BrowserLayout();
		::PostMessage(hBrowser, WM_BROWSERLAYOUT, 0, 7);
	}

	void CWebView::HandleChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5)
	{
		if (strId.CompareNoCase(_T("RENDER_ELEMENT")) == 0)
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
				g_pCosmos->m_pUniverseAppProxy->OnWebAppData(this, g_pCosmos->m_strAppXml);
				g_pCosmos->m_pHostHtmlWnd = this;

				g_pCosmos->TangramInitFromeWeb();
				//g_pCosmos->m_pUniverseAppProxy->OnMainWindow(this, g_pCosmos->m_strAppXml);
				CustomizedMainWindowElement(g_pCosmos->m_strMainWndXml);
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
		else if (strId.CompareNoCase(_T("Client_UI_MESSAGE")) == 0)
		{
			HWND hMainWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(MainWnd, NULL);
			if (hMainWnd)
			{
				CTangramXmlParse xmlParse;
				if (xmlParse.LoadFile(strParam3))
				{
					CosmosFrameWndInfo* pCosmosFrameWndInfo = nullptr;
					HANDLE hHandle = ::GetProp(hMainWnd, _T("CosmosFrameWndInfo"));
					if (hHandle)
					{
						pCosmosFrameWndInfo = (CosmosFrameWndInfo*)hHandle;
						pCosmosFrameWndInfo->m_pWebPage = this;
						pCosmosFrameWndInfo->m_strData = g_pCosmos->m_strMainWndXml;
						CTangramXmlParse* pParse = xmlParse.GetChild(_T("mainframe"));
						if (pParse)
						{
							CTangramXmlParse* pParseClient = pParse->GetChild(_T("client"));
							if (pParseClient)
							{
								HWND hClient = pCosmosFrameWndInfo->m_hClient;
								CString strXml = pParseClient->xml();
								IGalaxyCluster* pCluster = nullptr;
								if (pCluster == nullptr)
								{
									g_pCosmos->CreateGalaxyCluster((__int64)::GetParent(hClient), &pCluster);
								}
								if (pCluster)
								{
									IGalaxy* pGalaxy = nullptr;
									CString strKey = _T("client");
									pCluster->CreateGalaxy(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(strKey), &pGalaxy);
									if (pGalaxy)
									{
										CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
										_pGalaxy->m_pWebPageWnd = this;
										IXobj* pXobj = nullptr;
										_pGalaxy->Observe(CComBSTR(strParam1), CComBSTR(strXml), &pXobj);
									}
								}
							}
							pParse = pParse->GetChild(_T("controlbars"));
							if (pParse)
							{
								int nCount = pParse->GetCount();
								for (int i = 0; i < nCount; i++)
								{
									CTangramXmlParse* pParse2 = pParse->GetChild(i);
									int nBarID = pParse2->attrInt(_T("ctrlbarid"), 0);
									if (nBarID)
									{
										auto it = pCosmosFrameWndInfo->m_mapCtrlBarWnd.find(nBarID);
										if (it != pCosmosFrameWndInfo->m_mapCtrlBarWnd.end())
										{
											HWND hWnd = it->second;
											int nID = pParse2->attrInt(_T("clientid"), 0);
											CString strCaption = pParse2->attr(_T("caption"), _T(""));
											HWND hClient = ::GetDlgItem(hWnd, nID);
											if (hClient)
											{
												CString strXml = pParse2->xml();
												IGalaxyCluster* pCluster = nullptr;
												if (pCluster == nullptr)
												{
													g_pCosmos->CreateGalaxyCluster((__int64)hWnd, &pCluster);
												}
												if (pCluster)
												{
													if (strCaption != _T(""))
														::SetWindowText(::GetParent(hClient), strCaption);
													IGalaxy* pGalaxy = nullptr;
													CString strKey = _T("");
													strKey.Format(_T("ControlBar_%d"), nBarID);
													pCluster->CreateGalaxy(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(strKey), &pGalaxy);
													if (pGalaxy)
													{
														CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
														_pGalaxy->m_pWebPageWnd = this;
														IXobj* pXobj = nullptr;
														_pGalaxy->Observe(CComBSTR(strParam1), CComBSTR(strXml), &pXobj);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if (g_pCosmos->m_pUniverseAppProxy)
				g_pCosmos->m_pUniverseAppProxy->OnIPCMsg((CWebPageImpl*)this, strId, strParam1, strParam2, strParam3, strParam4, strParam5); // TODO: Missing parameters
			HWND hPPWnd = ::GetParent(::GetParent(m_hWnd));
			if (m_pRemoteCosmos)
			{
				CString strMsg = _T("");
				CString strMsgID = _T("");
				strMsg.Format(_T("%s@IPCMessage@%s@IPCMessage@%s@IPCMessage@%s@IPCMessage@%s@IPCMessage@%s"), strId, strParam1, strParam2, strParam3, strParam4, strParam5);
				strMsgID.Format(_T("HandleChromeIPCMessage:%I64d"), hPPWnd);
				m_pRemoteCosmos->put_AppKeyValue(CComBSTR(strMsgID), CComVariant(strMsg));
			}
		}
	}

	void CWebView::HandleAggregatedMessage(CString strParam1, CString strParam2)
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

	CString CWebView::FindToken(CString pszContent, CString pszDelimiter, int& nStart)
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

	void CWebView::HandleSingleMessage(CString strParam)
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

	void CWebView::CustomizedDOMElement(CString strRuleName, CString strHTML)
	{
		if (strRuleName.CompareNoCase(_T("application")) == 0)
		{
			if (g_pCosmos->m_strAppXml == _T(""))
			{
				g_pCosmos->m_strAppXml = strHTML;
			}
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(strHTML))
			{
				m_strPageName = m_Parse.attr(_T("pagename"), _T("default"));
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
			g_pCosmos->m_pUniverseAppProxy->CustomizedDOMElement(m_hWnd, strRuleName, strHTML);
		}
	}

	void CWebView::CustomizedMainWindowElement(CString strHTML)
	{
		CBrowser* pBrowserWnd = nullptr;
		if (m_pChromeRenderFrameHost)
		{
			HWND hBrowser = m_pChromeRenderFrameHost->GetHostBrowserWnd();
			auto itBrowser = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
			if (itBrowser != g_pCosmos->m_mapBrowserWnd.end())
			{
				pBrowserWnd = (CBrowser*)itBrowser->second;
			}
		}
		CTangramXmlParse xmlParse;
		if (xmlParse.LoadXml(strHTML))
		{
			CTangramXmlParse* urlsParse = xmlParse.GetChild(_T("urls"));
			CString strObjID = xmlParse.attr(_T("objid"), _T(""));
			IDispatch* pDisp = nullptr;
			int nPos = strObjID.Find(_T(","));
			if (strObjID != _T("") && nPos != -1)
			{
				//g_pCosmos->m_bCLRApp = true;
				CTangramXmlParse* pMdiChildXmlParse = xmlParse.GetChild(_T("mdichild"));
				if (pMdiChildXmlParse)
				{
					CTangramXmlParse* pMdiClientXmlParse = xmlParse.GetChild(_T("mdiclient"));
					int nCount = pMdiChildXmlParse->GetCount();
					if (nCount && pMdiClientXmlParse)
					{
						CMDIChildFormInfo* pInfo = new CMDIChildFormInfo();
						g_pCosmos->m_pCurMDIChildFormInfo = pInfo;
						for (int i = 0; i < nCount; i++)
						{
							CString strName = pMdiChildXmlParse->GetChild(i)->name().MakeLower();
							if (pMdiClientXmlParse->GetChild(strName))
							{
								CWebPageImpl* pChromeRenderFrameHostProxyBase = (CWebPageImpl*)this;
								pMdiChildXmlParse->GetChild(i)->put_attr(_T("renderframehostproxy"), (__int64)pChromeRenderFrameHostProxyBase);
								IWebPage* pChromeWebPage = (IWebPage*)this;
								pMdiChildXmlParse->GetChild(i)->put_attr(_T("webpage"), (__int64)pChromeWebPage);
								pMdiChildXmlParse->GetChild(i)->put_attr(_T("webpagehandle"), (__int64)m_hWnd);
								pInfo->m_mapFormsInfo[strName] = pMdiChildXmlParse->GetChild(i)->xml();
							}
						}
					}
				}
				if (g_pCosmos->m_pCLRProxy == nullptr)
					g_pCosmos->LoadCLR();
				if (g_pCosmos->m_pCLRProxy)
				{
					pBrowserWnd->m_bSZMode = true;
					m_bCanShow = false;
					IWebPage* pChromeWebPage = (IWebPage*)this;
					xmlParse.put_attr(_T("webpage"), (__int64)pChromeWebPage);
					xmlParse.put_attr(_T("webpagehandle"), (__int64)m_hWnd);
					CWebPageImpl* pChromeRenderFrameHostProxyBase = (CWebPageImpl*)this;
					xmlParse.put_attr(_T("renderframehostproxy"), (__int64)pChromeRenderFrameHostProxyBase);
					xmlParse.put_attr(_T("IsMainCosmosWnd"), (__int64)1);
					pDisp = g_pCosmos->m_pCLRProxy->CreateCLRObj(xmlParse.xml());
					if (pBrowserWnd->m_pParentXobj)
					{
						if (pBrowserWnd->m_pParentXobj->m_pParentWinFormWnd)
						{
							pBrowserWnd->m_pParentXobj->m_pParentWinFormWnd->m_pBrowser = pBrowserWnd;
						}
					}
				}
			}
			if (pDisp == nullptr)
			{
				if (g_pCosmos->m_pUniverseAppProxy->CreateCosmosApp(this, pBrowserWnd, strObjID, strHTML))
				{
					HWND hMainWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(MainWnd, NULL);
					if (hMainWnd)
					{
						theApp.m_bAppStarting = true;
						pBrowserWnd->m_bSZMode = true;
						IGalaxyCluster* pCluster = nullptr;
						CosmosFrameWndInfo* pCosmosFrameWndInfo = nullptr;
						HANDLE hHandle = ::GetProp(hMainWnd, _T("CosmosFrameWndInfo"));
						if (hHandle)
						{
							pCosmosFrameWndInfo = (CosmosFrameWndInfo*)hHandle;
							pCosmosFrameWndInfo->m_pWebPage = this;
							m_pCosmosFrameWndInfo = pCosmosFrameWndInfo;
							CCloudMDIFrame* pMdiParent = nullptr;
							pBrowserWnd->m_pCosmosFrameWndInfo = m_pCosmosFrameWndInfo;
							auto it = g_pCosmos->m_mapMDIParent.find(hMainWnd);
							if (it != g_pCosmos->m_mapMDIParent.end())
							{
								pMdiParent = it->second;
								pBrowserWnd->m_pMDIParent = pMdiParent;
								pMdiParent->m_pHostBrowser = pBrowserWnd;
								pMdiParent->m_pCosmosFrameWndInfo = pCosmosFrameWndInfo;
								RECT rc;
								::GetClientRect(pCosmosFrameWndInfo->m_hClient, &rc);
								::SetWindowPos(pBrowserWnd->m_hWnd, nullptr, 0, 0, rc.right, rc.bottom, SWP_DRAWFRAME);
							}
							pCosmosFrameWndInfo->m_strData = g_pCosmos->m_strMainWndXml;
							CString strWndXml = _T("");
							CString strTemplateID = _T("");
							CString strMainClient = _T("");
							if (g_pCosmos->m_pUniverseAppProxy->m_nShellCmd == CCommandLineInfo::FileNothing)
							{
								strTemplateID = _T("mainclient");
							}
							else
							{
								strTemplateID = xmlParse.attr(_T("initdocid"), _T("default"));
								auto itDoc = g_pCosmos->m_mapDocTemplate.find(_T("mainclient"));
								if (itDoc != g_pCosmos->m_mapDocTemplate.end())
									strMainClient = itDoc->second;
							}
							auto itDoc = g_pCosmos->m_mapDocTemplate.find(strTemplateID);
							if (itDoc != g_pCosmos->m_mapDocTemplate.end())
							{
								CTangramXmlParse m_DocParse, m_DocParse2;
								CString strWebPage = _T("");
								bool bNeedClientInfo = false;
								if (strMainClient != _T(""))
									bNeedClientInfo = m_DocParse2.LoadXml(strMainClient);
								if (m_DocParse.LoadXml(itDoc->second))
								{
									CTangramXmlParse* pParse = m_DocParse.GetChild(_T("hostpage"));
									if (pParse)
									{
										strWebPage = pParse->xml();
										if (bNeedClientInfo)
										{
											CTangramXmlParse* pParse = m_DocParse2.GetChild(_T("hostpage"));
											pMdiParent->m_mapClientCtrlBarData[_T("hostpage")] = pParse->xml();;
										}
										LoadDocument2Viewport(strTemplateID, strWebPage);
									}
									CString strCaption = xmlParse.attr(_T("caption"), _T(""));
									if (strCaption != _T(""))
									{
										g_pCosmos->m_pUniverseAppProxy->SetFrameCaption(hMainWnd, strCaption);
									}
									CTangramXmlParse* pParseClient = nullptr;
									if (pCosmosFrameWndInfo->m_nFrameType == 2)
										pParseClient = m_DocParse.GetChild(_T("mdiclient"));
									else
										pParseClient = m_DocParse.GetChild(_T("client"));
									if (pParseClient)
									{
										HWND hClient = pCosmosFrameWndInfo->m_hClient;
										CString strXml = pParseClient->xml();
										if (bNeedClientInfo)
										{
											CTangramXmlParse* pParse = m_DocParse2.GetChild(_T("mdiclient"));
											pMdiParent->m_mapClientCtrlBarData[_T("client")] = pParse->xml();;
										}
										if (pCluster == nullptr)
										{
											g_pCosmos->CreateGalaxyCluster((__int64)::GetParent(hClient), &pCluster);
										}
										if (pCluster)
										{
											IGalaxy* pGalaxy = nullptr;
											CString strKey = _T("client");
											pCluster->CreateGalaxy(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(strKey), &pGalaxy);
											if (pGalaxy)
											{
												CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
												pCosmosFrameWndInfo->m_mapCtrlBarGalaxys[10000] = _pGalaxy;
												_pGalaxy->m_pWebPageWnd = this;
												if (pCosmosFrameWndInfo->m_nFrameType == 2)
												{
													IXobj* pXobj = nullptr;
													_pGalaxy->Observe(CComBSTR(strTemplateID), CComBSTR(strXml), &pXobj);
												}
											}
										}
									}
									pParse = m_DocParse.GetChild(_T("controlbars"));
									CTangramXmlParse* pParseControlBars = nullptr;
									if (bNeedClientInfo)
									{
										pParseControlBars = m_DocParse2.GetChild(_T("controlbars"));
									}
									if (pParse)
									{
										int nCount = pParse->GetCount();
										for (int i = 0; i < nCount; i++)
										{
											CTangramXmlParse* pParse2 = pParse->GetChild(i);
											CTangramXmlParse* pCtrlBarParse2 = nullptr;
											if (pParseControlBars)
												pCtrlBarParse2 = pParseControlBars->GetChild(i);
											int nBarID = pParse2->attrInt(_T("ctrlbarid"), 0);
											if (nBarID)
											{
												auto it = pCosmosFrameWndInfo->m_mapCtrlBarWnd.find(nBarID);
												if (it != pCosmosFrameWndInfo->m_mapCtrlBarWnd.end())
												{
													HWND hWnd = it->second;
													int nID = pParse2->attrInt(_T("clientid"), 0);
													HWND hClient = ::GetDlgItem(hWnd, nID);
													if (hClient)
													{
														CString strXml = pParse2->xml();
														if (pCluster)
														{
															IGalaxy* pGalaxy = nullptr;
															CString strKey = _T("");
															strKey.Format(_T("ControlBar_%d"), nBarID);
															if (bNeedClientInfo && pCtrlBarParse2)
															{
																pMdiParent->m_mapClientCtrlBarData[strKey.MakeLower()] = pCtrlBarParse2->xml();;
															}

															pCluster->CreateGalaxy(CComVariant((__int64)::GetParent(hClient)), CComVariant((__int64)hClient), CComBSTR(strKey), &pGalaxy);
															if (pGalaxy)
															{
																CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
																pCosmosFrameWndInfo->m_mapCtrlBarGalaxys[nBarID] = _pGalaxy;
																_pGalaxy->m_pWebPageWnd = this;
																IXobj* pXobj = nullptr;
																_pGalaxy->Observe(CComBSTR(strTemplateID), CComBSTR(strXml), &pXobj);
															}
															CString strCaption = pParse2->attr(_T("caption"), _T(""));
															if (strCaption != _T(""))
																::SetWindowText(::GetParent(hClient), strCaption);
														}
													}
												}
											}
										}
									}
									if (pBrowserWnd->m_pClientGalaxy == nullptr)
									{
										if (m_pCosmosFrameWndInfo)
										{
											HWND hClient = m_pCosmosFrameWndInfo->m_hClient;
											IGalaxy* pGalaxy = nullptr;
											g_pCosmos->GetGalaxy((__int64)hClient, &pGalaxy);
											if (pGalaxy)
											{
												pBrowserWnd->m_pClientGalaxy = (CGalaxy*)pGalaxy;
											}
										}
									}
									if (g_pCosmos->m_pUniverseAppProxy->m_nShellCmd != CCommandLineInfo::FileNothing)
									{
										HWND hWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(DocView, g_pCosmos->m_hFirstView);
										if (hWnd)
										{
											pParseClient = m_DocParse.GetChild(_T("client"));
											IGalaxy* pGalaxy = nullptr;
											IXobj* _pXobj = nullptr;
											CGalaxyCluster* pGalaxyCluster = new CComObject<CGalaxyCluster>();
											pGalaxyCluster->m_hWnd = hWnd;
											g_pCosmos->m_mapGalaxyCluster[hWnd] = pGalaxyCluster;

											for (auto& it2 : g_pCosmos->m_mapCosmosAppProxy)
											{
												CGalaxyClusterProxy* pCosmosProxy = it2.second->OnGalaxyClusterCreated(pGalaxyCluster);
												if (pCosmosProxy)
													pGalaxyCluster->m_mapGalaxyClusterProxy[it2.second] = pCosmosProxy;
											}
											pGalaxyCluster->CreateGalaxy(CComVariant((__int64)hWnd), CComVariant((__int64)g_pCosmos->m_hFirstView), CComBSTR(""), &pGalaxy);
											if (pGalaxy)
											{
												pGalaxy->Observe(CComBSTR(strTemplateID), CComBSTR(pParseClient->xml()), &_pXobj);
											}
											pParseClient = m_DocParse.GetChild(_T("client"));
											CCloudMDIChild* pWnd = new CCloudMDIChild();
											pWnd->SubclassWindow(hWnd);
											pWnd->m_strDocTemplateKey = strTemplateID;
											pWnd->m_pParent = pMdiParent;
											pMdiParent->m_pActiveMDIChild = pWnd;
											pWnd->m_pGalaxy = (CGalaxy*)pGalaxy;
											pWnd->m_pGalaxy->m_nGalaxyType = GalaxyType::MDIChildGalaxy;
											pWnd->m_strKey = strTemplateID;
											pWnd->m_hClient = g_pCosmos->m_hFirstView;
											g_pCosmos->m_hFirstView = nullptr;
											pWnd->m_pParent->m_mapMDIChild[hWnd] = pWnd;
											CString strDefaultName = _T("");
											auto itName = g_pCosmos->m_mapDocDefaultName.find(strTemplateID);
											if (itName != g_pCosmos->m_mapDocDefaultName.end())
												strDefaultName = itName->second;
											g_pCosmos->m_pUniverseAppProxy->SetFrameCaption(pWnd->m_hWnd, strDefaultName);
											theApp.m_bAppStarting = false;
										}
									}
									else
										theApp.m_bAppStarting = false;
								}
							}
						}
					}
				}
				else
				{
					g_pCosmosImpl->m_pUniverseAppProxy->OpenDocFile(_T(""), _T(""), _T("default"));
				}
			}
			if (urlsParse)
			{
				theApp.m_bAppStarting = true;
				m_strLoadingURLs = urlsParse->xml();
				::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)m_hWnd, 20210411);
			}
			else
			{
				::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, (WPARAM)m_hWnd, 20210418);
			}
		}
	}

	void CWebView::CustomizedIndWindowElement(CString strHTML)
	{
		CTangramXmlParse xmlParse;
		if (xmlParse.LoadXml(strHTML))
		{
			CTangramXmlParse* pMdiChildXmlParse = xmlParse.GetChild(_T("mdichild"));
			if (pMdiChildXmlParse)
			{
				CTangramXmlParse* pMdiClientXmlParse = xmlParse.GetChild(_T("mdiclient"));
				int nCount = pMdiChildXmlParse->GetCount();
				if (nCount && pMdiClientXmlParse)
				{
					CMDIChildFormInfo* pInfo = new CMDIChildFormInfo();
					g_pCosmos->m_pCurMDIChildFormInfo = pInfo;
					for (int i = 0; i < nCount; i++)
					{
						CString strName = pMdiChildXmlParse->GetChild(i)->name().MakeLower();
						if (pMdiClientXmlParse->GetChild(strName))
						{
							CWebPageImpl* pChromeRenderFrameHostProxyBase = (CWebPageImpl*)this;
							pMdiChildXmlParse->GetChild(i)->put_attr(_T("renderframehostproxy"), (__int64)pChromeRenderFrameHostProxyBase);
							IWebPage* pChromeWebPage = (IWebPage*)this;
							pMdiChildXmlParse->GetChild(i)->put_attr(_T("webpage"), (__int64)pChromeWebPage);
							pMdiChildXmlParse->GetChild(i)->put_attr(_T("webpagehandle"), (__int64)m_hWnd);
							pInfo->m_mapFormsInfo[strName] = pMdiChildXmlParse->GetChild(i)->xml();
						}
					}
				}
			}
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

	void CWebView::CustomizedWebBrowserElement(CString strHTML)
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

	void CWebView::CustomizedNodeDetailsElement(CString strHTML)
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

	void CWebView::CustomizedObjectElement(CString strHTML)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strHTML))
		{
			CString strType = m_Parse.attr(_T("type"), _T(""));
			if (strType.CompareNoCase(_T("winform")) == 0)
			{
				CString strStartup = _T("");
				CTangramXmlParse* pChild3 = m_Parse.GetChild(_T("mdichild"));
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
				if (pChild3)
				{
					CTangramXmlParse* pChild4 = m_Parse.GetChild(_T("mdiclient"));
					int nCount = pChild3->GetCount();
					if (nCount && pChild4)
					{
						CMDIChildFormInfo* pInfo = new CMDIChildFormInfo();
						g_pCosmos->m_pCurMDIChildFormInfo = pInfo;
						for (int i = 0; i < nCount; i++)
						{
							CString strName = pChild3->GetChild(i)->name().MakeLower();
							if (pChild4->GetChild(strName))
							{
								CWebPageImpl* pChromeRenderFrameHostProxyBase = (CWebPageImpl*)this;
								pChild3->GetChild(i)->put_attr(_T("renderframehostproxy"), (__int64)pChromeRenderFrameHostProxyBase);
								IWebPage* pChromeWebPage = (IWebPage*)this;
								pChild3->GetChild(i)->put_attr(_T("webpage"), (__int64)pChromeWebPage);
								pChild3->GetChild(i)->put_attr(_T("webpagehandle"), (__int64)m_hWnd);
								pInfo->m_mapFormsInfo[strName] = pChild3->GetChild(i)->xml();
							}
						}
					}
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
							auto it2 = m_mapChildFormsInfo.find(strID);
							if (it2 != m_mapChildFormsInfo.end())
								g_pCosmos->m_pCurMDIChildFormInfo = it2->second;
							IDispatch* pDisp = g_pCosmos->m_pCLRProxy->CreateCLRObj(it->second);
							HWND hwnd = g_pCosmos->m_pCLRProxy->GetCtrlHandle(pDisp);
							if (hwnd)
							{
								CCloudWinForm* pForm = (CCloudWinForm*)::SendMessage(hwnd, WM_HUBBLE_DATA, 0, 20190214);
								if (pForm)
								{
									HWND hBrowser = m_pChromeRenderFrameHost->GetHostBrowserWnd();
									auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
									if (it != g_pCosmos->m_mapBrowserWnd.end())
									{
										pForm->m_pOwnerHtmlWnd = this;
										pForm->m_pBrowser = (CBrowser*)it->second;
									}
								}
							}
						}
					}
				}
			}
			if (strType.CompareNoCase(_T("workbench")) == 0)
			{
				bool bSkip = false;
				if (::IsChild(g_pCosmos->m_hHostBrowserWnd, m_hWnd))
				{
					if (g_pCosmos->m_nAppType != APP_ECLIPSE && g_pCosmos->m_nAppType != APP_BROWSER_ECLIPSE)
					{
						bSkip = true;
					}
				}
				if (bSkip == false)
				{
					CString strStartup = _T("");
					CString strID = m_Parse.attr(_T("workbenchkey"), _T(""));
					CString strName = m_Parse.name();
					if (strID != _T(""))
					{
						g_pCosmos->m_mapCreatingWorkBenchInfo[strID] = m_mapWorkBenchInfo[strID] = m_Parse.xml();
					}
					if (m_Parse.attrBool(_T("showstartup")))
					{
						strStartup = strID;
					}
					if (strStartup != _T(""))
					{
						CString strID = strStartup;
						g_pCosmos->m_strWorkBenchStrs = strID;
						g_pCosmos->m_strWorkBenchStrs += _T("|");
						g_pCosmos->m_nAppType = APP_BROWSER_ECLIPSE;
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

	void CWebView::CustomizedExtraElement(CString strHTML)
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

	void CWebView::CustomizedDataElement(CString strHTML)
	{
		CMarkup rootXML;
		if (rootXML.SetDoc(strHTML) && rootXML.FindElem())
		{
			CString strKey = rootXML.GetAttrib(_T("key"));
			g_pCosmos->m_mapValInfo[strKey] = CComVariant(strHTML);
		}
	}

	void CWebView::CustomizedDocElement(CString strHTML)
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

	void CWebView::OnNTPLoaded()
	{
		if (g_pCosmos->m_strNtpXml != _T(""))
		{
			LoadDocument2Viewport(_T("__NTP_DEFAULT__"), g_pCosmos->m_strNtpXml);
		}
	}

	void CWebView::OnWinFormCreated(HWND hwnd)
	{
		if (hwnd)
		{
			LRESULT l = ::SendMessage(hwnd, WM_HUBBLE_DATA, 0, 20190214);
			if (l == 0)
			{
				CCloudWinForm* pWnd = new CCloudWinForm();
				g_pCosmos->m_hFormNodeWnd = (HWND)hwnd;
				pWnd->SubclassWindow(hwnd);
				HWND hBrowser = m_pChromeRenderFrameHost->GetHostBrowserWnd();
				auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
				if (it != g_pCosmos->m_mapBrowserWnd.end())
				{
					pWnd->m_pOwnerHtmlWnd = this;
					pWnd->m_pBrowser = (CBrowser*)it->second;
				}
				g_pCosmos->m_mapFormWebPage[hwnd] = this;
				m_mapWinForm[hwnd] = pWnd;
				::PostMessage(g_pCosmos->m_hFormNodeWnd, WM_WINFORMCREATED, 0, 0);
			}
		}
	}

	void CWebView::OnCloudMsgReceived(CSession* pSession)
	{
		CString strMsgID = pSession->GetString(L"msgID");
		IXobj* pXobj = (IXobj*)pSession->Getint64(_T("xobj"));
		CXobj* pObj = nullptr;
		if (pXobj)
		{
			pObj = (CXobj*)pXobj;
			if (pObj->m_pWormhole == nullptr)
				pObj->m_pWormhole = (CWormhole*)pSession;
			if (pObj->m_pWebPage == nullptr)
				pObj->m_pWebPage = this;
			if (pObj->m_pXobjShareData->m_pGalaxy->m_pWebPageWnd == nullptr)
				pObj->m_pXobjShareData->m_pGalaxy->m_pWebPageWnd = this;
		}
		if (pObj)
		{
			::SendMessage(pObj->m_pXobjShareData->m_pGalaxy->m_hWnd, WM_CLOUDMSGRECEIVED, 0, (LPARAM)pSession);
			::SendMessage(pObj->m_pHostWnd->m_hWnd, WM_CLOUDMSGRECEIVED, 0, (LPARAM)pSession);
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
				CTangramXmlParse* pMdiChildXmlParse = xmlParse.GetChild(_T("mdichild"));
				if (pMdiChildXmlParse)
				{
					CTangramXmlParse* pMdiClientXmlParse = xmlParse.GetChild(_T("mdiclient"));
					int nCount = pMdiChildXmlParse->GetCount();
					if (nCount && pMdiClientXmlParse)
					{
						CMDIChildFormInfo* pInfo = new CMDIChildFormInfo();
						g_pCosmos->m_pCurMDIChildFormInfo = pInfo;
						for (int i = 0; i < nCount; i++)
						{
							CString strName = pMdiChildXmlParse->GetChild(i)->name().MakeLower();
							if (pMdiClientXmlParse->GetChild(strName))
							{
								CWebPageImpl* pChromeRenderFrameHostProxyBase = (CWebPageImpl*)this;
								pMdiChildXmlParse->GetChild(i)->put_attr(_T("renderframehostproxy"), (__int64)pChromeRenderFrameHostProxyBase);
								IWebPage* pChromeWebPage = (IWebPage*)this;
								pMdiChildXmlParse->GetChild(i)->put_attr(_T("webpage"), (__int64)pChromeWebPage);
								pMdiChildXmlParse->GetChild(i)->put_attr(_T("webpagehandle"), (__int64)m_hWnd);
								pInfo->m_mapFormsInfo[strName] = pMdiChildXmlParse->GetChild(i)->xml();
							}
						}
					}
				}
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
		else if (strMsgID == _T("OPEN_URL"))
		{
			CString strPath = g_pCosmos->m_strAppPath;
			CString strUrl = pSession->GetString(_T("openurl"));
			long nPos = pSession->GetLong(_T("BrowserWndOpenDisposition"));
			int _nPos = strUrl.Find(_T("host:"));
			if (nPos == 2 && _nPos != -1)
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
						HWND hWnd = ::CreateWindow(L"Cosmos Xobj Class", NULL, /*WS_OVERLAPPED |*/ WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 200, 200, g_pCosmos->m_hCosmosWnd, 0, AfxGetInstanceHandle(), NULL);
						g_pCosmos->m_hTempBrowserWnd = g_pCosmos->m_pBrowserFactory->CreateBrowser(hWnd, strUrl);
						::SetWindowPos(g_pCosmos->m_hTempBrowserWnd, HWND_BOTTOM, 0, 0, ::GetSystemMetrics(SM_CXSCREEN), 100, SWP_NOACTIVATE);// | SWP_HIDEWINDOW);
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

	STDMETHODIMP CWebView::get_HostWnd(LONGLONG* Val)
	{
		*Val = (LONGLONG)m_hWebHostWnd;
		return S_OK;
	}

	STDMETHODIMP CWebView::put_HostWnd(LONGLONG newVal)
	{
		HWND hWnd = (HWND)newVal;
		if (::IsWindow(hWnd))
			m_hWebHostWnd = hWnd;
		return S_OK;
	}

	STDMETHODIMP CWebView::Observe(BSTR bstrKey, BSTR bstrXml, IXobj** pRetXobj)
	{
		if (m_pGalaxy == nullptr) {
			CGalaxyCluster* pGalaxyCluster = nullptr;
			auto it = g_pCosmos->m_mapGalaxyCluster.find(m_hExtendWnd);
			if (it != g_pCosmos->m_mapGalaxyCluster.end())
				pGalaxyCluster = (CGalaxyCluster*)it->second;
			else
			{
				pGalaxyCluster = new CComObject<CGalaxyCluster>();
				pGalaxyCluster->m_hWnd = m_hExtendWnd;
				g_pCosmos->m_mapGalaxyCluster[m_hExtendWnd] = pGalaxyCluster;

				for (auto it : g_pCosmos->m_mapCosmosAppProxy)
				{
					CGalaxyClusterProxy* pProxy = it.second->OnGalaxyClusterCreated(pGalaxyCluster);
					if (pProxy)
						pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pProxy;
				}
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
			CString strNewKey = OLE2T(bstrKey);
			if (m_strCurKey != strNewKey)
				m_pGalaxy->Observe(bstrKey, bstrXml, pRetXobj);
			else
			{
				*pRetXobj = m_pGalaxy->m_pWorkXobj;
				return S_OK;
			}
			if (*pRetXobj)
			{
				m_strCurKey = strNewKey;
				m_hWebHostWnd = NULL;
				if (m_pGalaxy->m_pBindingXobj)
				{
					m_hWebHostWnd = m_pGalaxy->m_pBindingXobj->m_pHostWnd->m_hWnd;
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
			//::SendMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 2);
			//::PostMessage(::GetParent(m_hWnd), WM_BROWSERLAYOUT, 0, 4);
		}
		return S_OK;
	}

	STDMETHODIMP CWebView::SendXmlMessage(IXobj* sender, BSTR bstrXml)
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

	STDMETHODIMP CWebView::CreateForm(BSTR bstrKey, LONGLONG hParent, IDispatch** pRetForm)
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
}  // namespace Browser
