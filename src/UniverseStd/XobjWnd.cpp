/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103090047
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

 // XobjHelper.cpp : implementation file
 //

#include "stdafx.h"
#include "UniverseApp.h"
#include "XobjWnd.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "GridWnd.h"
#include "Wormhole.h"

#include "chromium/WebPage.h"

// CXobjWnd

IMPLEMENT_DYNCREATE(CXobjWnd, CWnd)

CXobjWnd::CXobjWnd()
{
	m_hFormWnd = NULL;
	m_bNoMove = false;
	m_bBKWnd = false;
	m_bCreateExternal = false;
	m_bEraseBkgnd = true;
	m_pXobj = nullptr;
	m_pParentXobj = nullptr;
	m_pOleInPlaceActiveObject = nullptr;
	m_strKey = m_strXml = _T("");
}

CXobjWnd::~CXobjWnd()
{
}

BEGIN_MESSAGE_MAP(CXobjWnd, CWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE(WM_TABCHANGE, OnTabChange)
	ON_MESSAGE(WM_COSMOSMSG, OnCosmosMsg)
	ON_MESSAGE(WM_HUBBLE_DATA, OnCosmosData)
	ON_MESSAGE(WM_HUBBLE_GETNODE, OnGetCosmosObj)
	ON_MESSAGE(WM_TGM_SETACTIVEPAGE, OnActiveTangramObj)

	ON_MESSAGE(WM_SPLITTERREPOSITION, OnSplitterReposition)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CXobjWnd diagnostics

#ifdef _DEBUG
void CXobjWnd::AssertValid() const
{
	///CView::AssertValid();
}

#ifndef _WIN32_WCE
void CXobjWnd::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif
#endif //_DEBUG

//CXobjWnd message handlers
BOOL CXobjWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_pXobj = g_pCosmos->m_pActiveXobj;
	m_pXobj->m_nID = nID;
	m_pXobj->m_pHostWnd = this;

	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		CGalaxy* pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
		pGalaxy->m_pBindingXobj = m_pXobj;
		m_pXobj->m_pXobjShareData->m_pHostClientView = this;
		CGalaxyCluster* pGalaxyCluster = pGalaxy->m_pGalaxyCluster;
		HWND hWnd = CreateWindow(L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);
		BOOL bRet = SubclassWindow(hWnd);
		if (m_pXobj->m_pParentObj)
		{
			if (m_pXobj->m_pParentObj->m_nViewType == Grid)
			{
				::PostMessage(::GetParent(m_hWnd), WM_HOSTNODEFORSPLITTERCREATED, m_pXobj->m_nRow, m_pXobj->m_nCol);
				ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
		}
		m_pXobj->NodeCreated();
		return bRet;
	}
	return m_pXobj->Create(dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CXobjWnd::OnSplitterReposition(WPARAM wParam, LPARAM lParam)
{
	switch (m_pXobj->m_nViewType)
	{
	case CLRCtrl:
	case ActiveX:
	case TabGrid:
		m_pXobj->m_pXobjShareData->m_pGalaxy->HostPosChanged();
		break;
		break;
	default:
		break;
	}

	return CWnd::DefWindowProc(WM_SPLITTERREPOSITION, wParam, lParam);
}

int CXobjWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pCosmos->m_pCLRProxy)
		g_pCosmos->m_pCLRProxy->HideMenuStripPopup();
	if (g_pCosmos->m_pActiveHtmlWnd)
	{
		::PostMessage(g_pCosmos->m_pActiveHtmlWnd->m_hWnd, WM_COSMOSMSG, 20190331, 0);
		g_pCosmos->m_pActiveHtmlWnd = nullptr;
	}

	CGalaxy* pGalaxy = m_pXobj->m_pRootObj->m_pXobjShareData->m_pGalaxy;
	HWND hWnd = pGalaxy->m_pGalaxyCluster->m_hWnd;
	if (((::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD)) || ::GetParent(hWnd) == NULL)
		::BringWindowToTop(hWnd);
	if (pGalaxy->m_pGalaxyCluster->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = pGalaxy->m_pGalaxyCluster->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	else if (g_pCosmos->m_pActiveAppProxy)
	{
		HWND hMenuWnd = g_pCosmos->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}

	if (m_pXobj && m_pXobj->m_pXobjShareData->m_pGalaxyCluster)
		m_pXobj->m_pXobjShareData->m_pGalaxyCluster->Fire_NodeMouseActivate(m_pXobj);

	if ((m_pXobj->m_nViewType == TabGrid || m_pXobj->m_nViewType == Grid))
	{
		//if (g_pCosmos->m_pGalaxy && g_pCosmos->m_pGalaxy != m_pXobj->m_pXobjShareData->m_pGalaxy)
		::SetFocus(m_hWnd);
		g_pCosmos->m_pActiveXobj = m_pXobj;
		g_pCosmos->m_bWinFormActived = false;
		return MA_ACTIVATE;
	}

	if (!m_bCreateExternal)
		Invalidate(true);
	else
	{
		if ((m_pXobj->m_nViewType != TabGrid && m_pXobj->m_nViewType != Grid))
		{
			if (g_pCosmos->m_pGalaxy != m_pXobj->m_pXobjShareData->m_pGalaxy || g_pCosmos->m_pActiveXobj != m_pXobj)
				::SetFocus(m_hWnd);
		}
	}
	g_pCosmos->m_pActiveXobj = m_pXobj;
	g_pCosmos->m_bWinFormActived = false;
	g_pCosmos->m_pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;

	CWebPage* pHtmlWnd = g_pCosmos->m_pGalaxy->m_pWebPageWnd;
	CString strID = m_pXobj->m_strName;

	if ((m_pXobj->m_nViewType == ActiveX || m_pXobj->m_nViewType == CLRCtrl))
	{
		if (pHtmlWnd)
		{
			CWormhole* pSession = m_pXobj->m_pWormhole;
			if (pSession)
			{
				pSession->InsertString(_T("msgID"), IPC_NODE_ONMOUSEACTIVATE_ID);
				pHtmlWnd->m_pChromeRenderFrameHost->SendCosmosMessage(pSession->m_pSession);
			}
		}
		return MA_NOACTIVATE;
	}

	if (m_bCreateExternal == false)
	{
		if (pHtmlWnd)
		{
			IPCMsg pIPCInfo;
			pIPCInfo.m_strId = IPC_NODE_ONMOUSEACTIVATE_ID;
			pIPCInfo.m_strParam1 = strID;
			CString strHandle = _T("");
			strHandle.Format(_T("%d"), m_hWnd);
			pIPCInfo.m_strParam2 = strHandle;
			strHandle.Format(_T("%d"), m_pXobj->m_nViewType);
			pIPCInfo.m_strParam3 = strHandle;
			strHandle.Format(_T("%d"), pGalaxy->m_hWnd);
			pIPCInfo.m_strParam4 = strHandle;
			//strHandle.Format(_T("%d"), m_pRootObj->m_pHostWnd->m_hWnd);
			pIPCInfo.m_strParam5 = _T("wndnode");
			pHtmlWnd->m_pChromeRenderFrameHost->SendCosmosMessage(&pIPCInfo);
		}
		return MA_ACTIVATEANDEAT;
	}
	else
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CXobjWnd::OnEraseBkgnd(CDC* pDC)
{
	if (m_pXobj->m_nViewType != BlankView)
		return true;
	CGalaxy* pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
	CBitmap bit;
	RECT rt;
	GetClientRect(&rt);
	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		HWND hWnd = pGalaxy->m_hWnd;
		if (::IsWindow(hWnd) && !::IsWindowVisible(hWnd))
		{
			//pGalaxy->HostPosChanged();
			return false;
		}
	}
	//else if ((m_pXobj->m_nViewType == ActiveX || m_pXobj->m_nViewType == CLRCtrl) && m_pXobj->m_pDisp == nullptr)
	//{
	//	bit.LoadBitmap(IDB_BITMAP_DESIGNER);
	//	CBrush br(&bit);
	//	pDC->FillRect(&rt, &br);
	//	pDC->SetBkMode(TRANSPARENT);
	//	CComBSTR bstrCaption(L"");
	//	m_pXobj->get_Attribute(CComBSTR(L"caption"), &bstrCaption);
	//	CString strInfo = _T("");
	//	strInfo = strInfo +
	//		_T("\n  ----.NET Assembly don't exists or AxtiveX Control not Support!----");

	//	pDC->SetTextColor(RGB(255, 0, 255));
	//	pDC->DrawText(strInfo, &rt, DT_LEFT);
	//}

	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pXobj->m_pDisp == NULL) && m_bEraseBkgnd)
	{
		CString strText = _T("");
		bit.LoadBitmap(IDB_BITMAP_DESIGNER);
		CBrush br(&bit);
		pDC->FillRect(&rt, &br);

		{
			CComBSTR bstrCaption(L"");
			m_pXobj->get_Attribute(CComBSTR(L"caption"), &bstrCaption);
			CString strInfo = _T("\n\n  ");
			strText.Format(strInfo, m_pXobj->m_strName, CString(OLE2T(bstrCaption)));
			pDC->SetTextColor(RGB(255, 255, 255));
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strText, &rt, DT_LEFT);
	}
	return true;
}

BOOL CXobjWnd::PreTranslateMessage(MSG* pMsg)
{
	if (m_pOleInPlaceActiveObject)
	{
		LRESULT hr = m_pOleInPlaceActiveObject->TranslateAccelerator((LPMSG)pMsg);
		if (hr == S_OK)
			return true;
		else
		{
			if (m_pXobj->m_nViewType == CLRCtrl)
			{
				VARIANT_BOOL bShiftKey = false;
				if (::GetKeyState(VK_SHIFT) < 0)  // shift pressed
					bShiftKey = true;
				hr = g_pCosmos->m_pCLRProxy->ProcessCtrlMsg(::GetWindow(m_hWnd, GW_CHILD), bShiftKey ? true : false);
				if (hr == S_OK)
					return true;
				return false;
			}
			else
			{
				pMsg->hwnd = 0;
				return true;
			}
		}
	}
	else
	{
		if (m_pXobj->m_nViewType == CLRCtrl)
		{
			if (g_pCosmos->m_pCLRProxy->ProcessFormMsg(m_hWnd, pMsg, 0))
				return true;
			return false;
		}
	}
	if (IsDialogMessage(pMsg))
		return true;
	return CWnd::PreTranslateMessage(pMsg);
}

void CXobjWnd::OnDestroy()
{
	m_pXobj->Fire_Destroy();
	HANDLE hData = RemoveProp(m_hWnd, _T("CosmosInfo"));
	if (hData)
	{
		CosmosInfo* pInfo = (CosmosInfo*)hData;
		delete pInfo;
	}
	CWnd::OnDestroy();
}

void CXobjWnd::PostNcDestroy()
{
	delete m_pXobj;
	CWnd::PostNcDestroy();
	delete this;
}

LRESULT CXobjWnd::OnTabChange(WPARAM wParam, LPARAM lParam)
{
	int nOldPage = m_pXobj->m_nActivePage;
	m_pXobj->m_nActivePage = (int)wParam;
	IXobj* pXobj = nullptr;
	m_pXobj->GetXobj(0, wParam, &pXobj);

	CGalaxy* pGalaxy = m_pXobj->m_pXobjShareData->m_pGalaxy;
	::PostMessage(pGalaxy->m_hWnd, WM_COSMOSMSG, 0, 20180115);
	if (pXobj)
	{
		CXobj* _pXobj = (CXobj*)pXobj;
		CString str = _T("");
		str.Format(_T("%d"), wParam);
		m_pXobj->put_Attribute(CComBSTR(L"activepage"), str.AllocSysString());
		if (_pXobj->m_nViewType == Grid)
		{
			((CSplitterWnd*)_pXobj->m_pHostWnd)->RecalcLayout();
		}
		else
		{
			pGalaxy->HostPosChanged();
		}
		if (_pXobj->m_pWebBrowser) {
			g_pCosmos->m_pActiveHtmlWnd = _pXobj->m_pWebBrowser->m_pVisibleWebWnd;
		}
		if (nOldPage != wParam)
		{
			::PostMessage(pGalaxy->m_hWnd, WM_HUBBLE_ACTIVEPAGE, wParam, lParam);
			::SendMessage(_pXobj->m_pHostWnd->m_hWnd, WM_HUBBLE_ACTIVEPAGE, wParam, lParam);
		}
		if (m_pXobj->m_nViewType == TabGrid)
		{
			for (auto it : m_pXobj->m_vChildNodes)
			{
				CXobj* pXobj = it;
				if (pXobj->m_nCol != m_pXobj->m_nActivePage)
				{
					pXobj->m_pHostWnd->ShowWindow(SW_HIDE);
				}
			}
		}
		HWND h = ::GetParent(m_hWnd);
		if (h)
		{
			LRESULT lRes = ::SendMessage(h, WM_HUBBLE_GETNODE, 0, 0);
			if (lRes)
			{
				CXobj* pRetNode = (CXobj*)lRes;
				if (pRetNode && pRetNode->m_nViewType == Grid)
				{
					pRetNode->m_pXobjShareData->m_pGalaxy->HostPosChanged();
				}
			}
		}
	}
	if (nOldPage != wParam)
	{
		m_pXobj->Fire_TabChange(wParam, lParam);
		m_pXobj->m_pXobjShareData->m_pGalaxyCluster->Fire_TabChange(m_pXobj, wParam, lParam);
		if (pGalaxy->m_pWebPageWnd)
		{
			HWND hWnd = ::GetParent(pGalaxy->m_pWebPageWnd->m_hWnd);
			if (::IsWindow(hWnd))
			{
				::SendMessage(hWnd, WM_BROWSERLAYOUT, 0, 5);
			}
		}
		m_pXobj->m_pXobjShareData->m_pGalaxy->ModifyStyle(WS_CLIPCHILDREN, 0);
		::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20210202);
	}
	LRESULT lRes = CWnd::DefWindowProc(WM_TABCHANGE, wParam, lParam);
	return lRes;
}

LRESULT CXobjWnd::OnCosmosData(WPARAM wParam, LPARAM lParam)
{
	return CWnd::DefWindowProc(WM_HUBBLE_DATA, wParam, lParam);
}

LRESULT CXobjWnd::OnCosmosMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam && lParam == 20201028)
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		m_pXobj->m_pWebBrowser = (CBrowser*)wParam;
		::SetParent(m_pXobj->m_pWebBrowser->m_hWnd, m_hWnd);
		return -1;
	}
	if (wParam == 0 && lParam)//Create CLRCtrl Node
	{
		switch (lParam)
		{
		case 20201028:
		case 20200609:
		case 20200606:
		case 19631222:
		case 20191031:
		case 20180115:
		case 19820911:
			return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
			break;
		case 20210202:
		{
			HWND hWnd = g_pCosmos->m_pUniverseAppProxy->QueryWndInfo(RecalcLayout, m_hWnd);
			if (::IsWindow(hWnd))
			{
				CosmosFrameWndInfo* pCosmosFrameWndInfo = (CosmosFrameWndInfo*)::GetProp(hWnd, _T("CosmosFrameWndInfo"));
				if (pCosmosFrameWndInfo)
				{
					HWND hClient = pCosmosFrameWndInfo->m_hClient;
					IGalaxy* pGalaxy = nullptr;
					g_pCosmos->GetGalaxy((__int64)hClient, &pGalaxy);
					if (pGalaxy)
					{
						CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
						_pGalaxy->HostPosChanged();
					}
				}
			}
			int nPage = -1;
			m_pXobj->get_ActivePage(&nPage);
			IXobj* pObj = nullptr;
			m_pXobj->GetXobj(0, nPage, &pObj);
			if (pObj)
			{
				CXobj* _pObj = (CXobj*)pObj;
				if (_pObj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
					m_pXobj->m_pXobjShareData->m_pGalaxy->SetFocus();
				else
					_pObj->m_pHostWnd->SetFocus();
			}
			m_pXobj->m_pXobjShareData->m_pGalaxy->ModifyStyle(0, WS_CLIPCHILDREN);
			return 0;
		}
		break;
		case 20190602:
		{
			CWinForm* pCosmosWinFormWnd = (CWinForm*)::SendMessage(m_hWnd, WM_HUBBLE_DATA, 0, 20190214);
			if (pCosmosWinFormWnd)
				return (LRESULT)pCosmosWinFormWnd->m_pChildFormsInfo;
			return 0;
		}
		break;
		case 20200128:
		{
			if (m_pXobj && m_pXobj->m_pWebBrowser)
			{
				::SetWindowPos(m_pXobj->m_pWebBrowser->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW);
			}
		}
		break;
		default:
		{
			CString strObjTypeID = (LPCTSTR)lParam;
			if (strObjTypeID.Find(_T(",")) != -1 && g_pCosmos->m_pCLRProxy)
			{
				m_pXobj->m_pDisp = g_pCosmos->m_pCLRProxy->CreateObject(strObjTypeID.AllocSysString(), m_hWnd, m_pXobj);

				if (m_pXobj->m_pDisp)
				{
					m_pXobj->m_pXobjShareData->m_mapLayoutNodes[m_pXobj->m_strName] = m_pXobj;
					m_pXobj->m_nViewType = CLRCtrl;
					if (g_pCosmos->m_hFormNodeWnd)
					{
						m_hFormWnd = g_pCosmos->m_hFormNodeWnd;
						RECT rc;
						::GetClientRect(m_hWnd, &rc);
						::SetWindowPos(m_hFormWnd, HWND_BOTTOM, 0, 0, rc.right, rc.bottom, SWP_NOACTIVATE | SWP_NOREDRAW);
						g_pCosmos->m_hFormNodeWnd = nullptr;
						return 0;
					}
					CAxWindow m_Wnd;
					m_Wnd.Attach(m_hWnd);
					CComPtr<IUnknown> pUnk;
					m_Wnd.AttachControl(m_pXobj->m_pDisp, &pUnk);
					CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pXobj->m_pDisp);
					if (pIOleInPlaceActiveObject)
						m_pOleInPlaceActiveObject = pIOleInPlaceActiveObject.Detach();
					m_Wnd.Detach();
				}
				else
				{
					m_bCreateExternal = true;
				}
			}
			else
			{
				CString strName = strObjTypeID;
				int _nPos = strName.Find(_T("."));
				if (_nPos != -1)
					strName = strName.Mid(_nPos + 1);
				_nPos = strName.ReverseFind('.');
				if (_nPos != -1)
					strName = strName.Left(_nPos);
				CXobj* pXobj = m_pXobj->m_pRootObj;
				CXobj* pParent = m_pXobj->m_pParentObj;
				if (pParent)
				{
					strName = pParent->m_strName + _T("_") + strName;
				}
				auto it = pXobj->m_pXobjShareData->m_mapAxNodes.find(strName);
				if (it != pXobj->m_pXobjShareData->m_mapAxNodes.end())
				{
					BOOL bGetNew = false;
					int nIndex = 0;
					while (bGetNew == false)
					{
						CString strNewName = _T("");
						strNewName.Format(_T("%s%d"), strName, nIndex);
						it = pXobj->m_pXobjShareData->m_mapAxNodes.find(strNewName);
						if (it == pXobj->m_pXobjShareData->m_mapAxNodes.end())
						{
							pXobj->m_pXobjShareData->m_mapAxNodes[strNewName] = m_pXobj;
							strName = strNewName;
							bGetNew = true;
							break;
						}
						nIndex++;
					}
				}
				else
				{
					pXobj->m_pXobjShareData->m_mapAxNodes[strName] = m_pXobj;
				}
				m_pXobj->put_Attribute(CComBSTR(L"id"), strName.AllocSysString());
				m_pXobj->m_strName = strName;
				BOOL bWebCtrl = false;
				CString strURL = _T("");
				strObjTypeID.MakeLower();
				auto nPos = strObjTypeID.Find(_T("http:"));
				if (nPos == -1)
					nPos = strObjTypeID.Find(_T("https:"));
				if (m_pXobj->m_pXobjShareData->m_pGalaxy)
				{
					CComBSTR bstr;
					m_pXobj->get_Attribute(CComBSTR("InitInfo"), &bstr);
					CString str = _T("");
					str += bstr;
					if (str != _T("") && m_pXobj->m_pXobjShareData->m_pGalaxyCluster)
					{
						LRESULT hr = ::SendMessage(m_pXobj->m_pXobjShareData->m_pGalaxyCluster->m_hWnd, WM_GETNODEINFO, (WPARAM)OLE2T(bstr), (LPARAM)::GetParent(m_hWnd));
						if (hr)
						{
							CString strInfo = _T("");
							CWindow m_wnd;
							m_wnd.Attach(::GetParent(m_hWnd));
							m_wnd.GetWindowText(strInfo);
							strObjTypeID += strInfo;
							m_wnd.Detach();
						}
					}
				}

				if (strObjTypeID.Find(_T("http://")) != -1 || strObjTypeID.Find(_T("https://")) != -1)
				{
					bWebCtrl = true;
					strURL = strObjTypeID;
					strObjTypeID = _T("shell.explorer.2");
				}

				if (strObjTypeID.Find(_T("res://")) != -1 || ::PathFileExists(strObjTypeID))
				{
					bWebCtrl = true;
					strURL = strObjTypeID;
					strObjTypeID = _T("shell.explorer.2");
					if (strObjTypeID.Find(_T("res://")) != -1)
					{
						CString strTemp = strURL;
						strTemp.Replace(_T("res://"), _T(""));
						int nPos = strTemp.ReverseFind('/');
						CString strTemp2 = strTemp.Mid(nPos);
						strTemp = strTemp.Left(nPos);
						if (::PathFileExists(strTemp) == false)
						{
							strTemp = g_pCosmos->m_strAppPath + strTemp;
							if (::PathFileExists(strTemp) == false)
							{
								strTemp = g_pCosmos->m_strProgramFilePath + _T("Tangram\\") + strTemp;
								if (::PathFileExists(strTemp) == false)
								{
									strURL = _T("about:blank");
								}
								else
									strURL = _T("res://") + strTemp + strTemp2;
							}
							else
								strURL = _T("res://") + strTemp + strTemp2;
						}
					}
				}

				if (strObjTypeID.CompareNoCase(_T("about:blank")) == 0)
				{
					bWebCtrl = true;
					strURL = strObjTypeID;
					strObjTypeID = _T("shell.explorer.2");
				}

				if (m_pXobj->m_pDisp == NULL)
				{
					CComPtr<IDispatch> pDisp;
					HRESULT hr = pDisp.CoCreateInstance(CComBSTR(strObjTypeID));
					if (hr == S_OK)
					{
						m_pXobj->m_pDisp = pDisp.Detach();
					}
				}
				if (m_pXobj->m_pDisp)
				{
					m_pXobj->m_pXobjShareData->m_mapLayoutNodes[m_pXobj->m_strName] = m_pXobj;
					m_pXobj->m_nViewType = ActiveX;
					CAxWindow m_Wnd;
					m_Wnd.Attach(m_hWnd);
					CComPtr<IUnknown> pUnk;
					m_Wnd.AttachControl(m_pXobj->m_pDisp, &pUnk);
					((CXobjWnd*)m_pXobj->m_pHostWnd)->m_pXobj = m_pXobj;
					HWND hPage = m_pXobj->m_pXobjShareData->m_pGalaxyCluster->m_hWnd;
					::SendMessage(hPage, WM_COSMOSMSG, (WPARAM)((CXobjWnd*)m_pXobj->m_pHostWnd), 1963);
					CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pXobj->m_pDisp);
					if (pIOleInPlaceActiveObject)
						m_pOleInPlaceActiveObject = pIOleInPlaceActiveObject.Detach();
					m_Wnd.Detach();
				}
			}
		}
		break;
		}
		return 0;
	}
	if (lParam == 20191031 || lParam == 20200130)
		return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
	if (lParam == 20200208)
		return 0;
	CString str = (LPCTSTR)lParam;
	IXobj* _pXobj = nullptr;
	CXobj* pOldNode = (CXobj*)m_pXobj;
	if (m_pXobj->m_hHostWnd == 0)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		m_pXobj->m_hHostWnd = ::CreateWindowEx(NULL, L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_hWnd, NULL, AfxGetInstanceHandle(), NULL);
		m_pXobj->m_hChildHostWnd = ::CreateWindowEx(NULL, L"Cosmos Xobj Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_pXobj->m_hHostWnd, NULL, AfxGetInstanceHandle(), NULL);
		IGalaxy* pGalaxy = nullptr;
		m_pXobj->m_pXobjShareData->m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)pOldNode->m_hChildHostWnd), CComBSTR(L"Design"), &pGalaxy);
		IXobj* pXobj = nullptr;
		pGalaxy->Observe(CComBSTR(L""), str.AllocSysString(), &pXobj);
		m_bEraseBkgnd = false;

		((CXobj*)pXobj)->m_pRootObj = m_pXobj->m_pRootObj;
		((CXobj*)pXobj)->m_pParentObj = m_pXobj->m_pParentObj;
		((CXobj*)pXobj)->m_pXobjShareData->m_pOfficeObj = m_pXobj->m_pXobjShareData->m_pOfficeObj;
		m_pXobj->m_pXobjShareData->m_mapLayoutNodes[((CXobj*)pXobj)->m_strName] = (CXobj*)pXobj;
		CString strXml = ((CXobj*)pXobj)->m_pHostParse->xml();
		CTangramXmlParse* pNew = ((CXobj*)pXobj)->m_pHostParse;
		CTangramXmlParse* pOld = pOldNode->m_pHostParse;
		CTangramXmlParse* pParent = m_pXobj->m_pHostParse->m_pParentParse;
		CTangramXmlParse* pRet = nullptr;
		if (pParent)
		{
			pRet = pParent->ReplaceNode(pOld, pNew, _T(""));
			CString str = pRet->xml();
			int nCount = pRet->GetCount();
			((CXobj*)pXobj)->m_pHostParse = pRet;
			m_pXobj->m_pHostParse = pRet;

			CXobj* pChildNode = nullptr;
			for (auto it2 : ((CXobj*)pXobj)->m_vChildNodes)
			{
				pChildNode = it2;
				pChildNode->m_pRootObj = m_pXobj->m_pRootObj;
				CString strName = pChildNode->m_strName;
				for (int i = 0; i < nCount; i++)
				{
					CTangramXmlParse* child = pRet->GetChild(i);
					CString _strName = child->attr(_T("id"), _T(""));
					if (_strName.CompareNoCase(strName) == 0)
					{
						pChildNode->m_pHostParse = child;
						break;
					}
				}
			}
			m_pXobj->m_vChildNodes.push_back(((CXobj*)pXobj));
		}

		auto it = m_pXobj->m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster->m_mapGalaxy.find(pOldNode->m_hChildHostWnd);
		if (it != m_pXobj->m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster->m_mapGalaxy.end())
			m_pXobj->m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster->m_mapGalaxy.erase(it);
	}
	return -1;
}

LRESULT CXobjWnd::OnActiveTangramObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pXobj->m_nViewType == CLRCtrl)
		::SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_pXobj->m_pXobjShareData->m_pGalaxy->HostPosChanged();
	::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return CWnd::DefWindowProc(WM_TGM_SETACTIVEPAGE, wParam, lParam);
}

LRESULT CXobjWnd::OnGetCosmosObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pXobj)
	{
		HWND hBrowser = (HWND)lParam;
		if (::IsWindow(hBrowser))
		{
			auto it = g_pCosmos->m_mapBrowserWnd.find(hBrowser);
			if (it != g_pCosmos->m_mapBrowserWnd.end())
			{
				m_pXobj->m_pWebBrowser = (CBrowser*)it->second;
			}
		}
		return (LRESULT)m_pXobj;
	}
	return CWnd::DefWindowProc(WM_HUBBLE_GETNODE, wParam, lParam);;
}

LRESULT CXobjWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_bCreateExternal)
	{
		switch (message)
		{
		case WM_COMMAND:
		{
			LRESULT res = CWnd::WindowProc(message, wParam, lParam);// CallWindowProc(*lplpfn, m_hWnd, message, wParam, lParam);
			HWND hWnd = (HWND)lParam;
			if (::IsWindow(hWnd) && m_pXobj)
			{
				::GetClassName(hWnd, g_pCosmos->m_szBuffer, MAX_PATH);
				CString strName = g_pCosmos->m_szBuffer;
				m_pXobj->Fire_ControlNotify(m_pXobj, HIWORD(wParam), LOWORD(wParam), lParam, CComBSTR(g_pCosmos->m_szBuffer));
				if (m_pXobj->m_pWormhole)
				{
					HWND hCtrl = (HWND)lParam;
					CString strVal = _T("");
					if (strName == _T("Edit"))
					{
						CEdit edit;
						edit.Attach(hCtrl);
						edit.GetWindowText(strVal);
						edit.Detach();
					}
					else
					{
						::GetWindowText((HWND)lParam, g_pCosmos->m_szBuffer, MAX_PATH);
						strVal = g_pCosmos->m_szBuffer;
					}
					m_pXobj->m_pWormhole->InsertString(_T("msgID"), _T("WM_COMMAND"));
					m_pXobj->m_pWormhole->InsertString(_T("CtrlClass"), strName);
					m_pXobj->m_pWormhole->InsertString(_T("CtrlValue"), strVal);
					m_pXobj->m_pWormhole->InsertLong(_T("NotifyCode"), HIWORD(wParam));
					m_pXobj->m_pWormhole->InsertLong(_T("CtrlID"), LOWORD(wParam));
					m_pXobj->m_pWormhole->Insertint64(_T("CtrlHandle"), lParam);
					m_pXobj->m_pWormhole->SendMessage();
					m_pXobj->m_pWormhole->InsertString(_T("msgID"), _T(""));
				}
			}
			return res;
		}
		case WM_ACTIVATE:
		case WM_ERASEBKGND:
		case WM_SETFOCUS:
		case WM_CONTEXTMENU:
		case WM_LBUTTONDOWN:
			//case WM_NOTIFY:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONDBLCLK:
		{
			WNDPROC* lplpfn = GetSuperWndProcAddr();
			return CallWindowProc(*lplpfn, m_hWnd, message, wParam, lParam);
		}
		case WM_MOUSEACTIVATE:
		{
			break;
		}
		case WM_SETREDRAW:
		{
			if (wParam == 0 && m_pXobj->m_nViewType == TabGrid)
				return 1;
			break;
		}
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CXobjWnd::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
	CGalaxy* pGalaxy = nullptr;
	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) == 0 || m_pXobj->m_strID.CompareNoCase(_T("mdiclient")) == 0)
	{
		m_pXobj->m_pXobjShareData->m_pGalaxy->HostPosChanged();
		return;
	}
	if (m_pXobj->m_pWebBrowser)
	{
		::SetWindowPos(m_pXobj->m_pWebBrowser->m_hWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		return;
	}
	if (m_pXobj && m_pXobj->m_nViewType == CLRCtrl && m_pXobj->m_hHostWnd)
	{
		if (m_bNoMove)
		{
			lpwndpos->flags |= SWP_NOMOVE;
			CXobjWnd* pXobjWnd = (CXobjWnd*)m_pXobj->m_pHostWnd;
			if (m_pXobj->m_pParentObj->m_nViewType == Grid)
			{
				CGridWnd* pWnd = (CGridWnd*)m_pXobj->m_pParentObj->m_pHostWnd;
				::PostMessage(pWnd->m_hWnd, WM_COSMOSMSG, 3, 1993);
			}
		}
	}
	if (m_pXobj && m_pXobj->m_hHostWnd)
	{
		if (m_pXobj->m_nViewType == CLRCtrl)
		{
			if (m_bNoMove)
			{
				m_bNoMove = false;
			}
			return;
		}
		else if (m_pXobj->m_nViewType == BlankView)
		{
			::SetWindowPos(m_pXobj->m_hHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
			if (m_pXobj->m_hChildHostWnd)
				::SetWindowPos(m_pXobj->m_hChildHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
	}
	if (m_hFormWnd)
		::SetWindowPos(m_hFormWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
	if (m_pXobj->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pXobj->m_pDisp == NULL) && m_pXobj != m_pXobj->m_pRootObj)
	{
		return;
	}

	if (m_bCreateExternal)
	{
		Invalidate(true);
	}
}

void CXobjWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

void CXobjWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if (bShow && m_pXobj->m_pWebBrowser)
	{
		::PostMessage(m_pXobj->m_pWebBrowser->m_hWnd, WM_BROWSERLAYOUT, 0, 5);
	}
}
