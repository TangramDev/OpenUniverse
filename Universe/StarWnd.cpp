/********************************************************************************
*					Open Universe - version 0.2.0								*
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

// TangramView.cpp : implementation file
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "Hubble.h"
#include "StarWnd.h"
#include "Star.h"
#include "Quasar.h"
#include "Grid.h"
#include "Wormhole.h"

#include "chromium/HtmlWnd.h"

// CStarWnd

IMPLEMENT_DYNCREATE(CStarWnd, CWnd)

CStarWnd::CStarWnd()
{
	m_hFormWnd = NULL;
	m_bNoMove = false;
	m_bBKWnd = false;
	m_bCreateExternal = false;
	m_bEraseBkgnd = true;
	m_pStar = nullptr;
	m_pParentStar = nullptr;
	m_pOleInPlaceActiveObject = nullptr;
	m_strKey = m_strXml = _T("");
}

CStarWnd::~CStarWnd()
{
}

BEGIN_MESSAGE_MAP(CStarWnd, CWnd)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_TABCHANGE, OnTabChange)
	ON_MESSAGE(WM_COSMOSMSG, OnHubbleMsg)
	ON_MESSAGE(WM_TANGRAMGETNODE, OnGetTangramObj)
	ON_MESSAGE(WM_TGM_SETACTIVEPAGE, OnActiveTangramObj)
	ON_MESSAGE(WM_SPLITTERREPOSITION, OnSplitterReposition)
END_MESSAGE_MAP()

// CStarWnd diagnostics

#ifdef _DEBUG
void CStarWnd::AssertValid() const
{
	///CView::AssertValid();
}

#ifndef _WIN32_WCE
void CStarWnd::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif
#endif //_DEBUG

//CStarWnd message handlers
BOOL CStarWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_pStar = g_pHubble->m_pActiveStar;
	m_pStar->m_nID = nID;
	m_pStar->m_pHostWnd = this;

	if (m_pStar->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		CQuasar* pQuasar = m_pStar->m_pStarCommonData->m_pQuasar;
		pQuasar->m_pBindingStar = m_pStar;

		m_pStar->m_pStarCommonData->m_pHostClientView = this;
		CGalaxyCluster* pGalaxyCluster = pQuasar->m_pGalaxyCluster;
		HWND hWnd = CreateWindow(L"Tangram Node Class", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);
		BOOL bRet = SubclassWindow(hWnd);
		if (m_pStar->m_pParentObj)
		{
			if (m_pStar->m_pParentObj->m_nViewType == Splitter)
			{
				::PostMessage(::GetParent(m_hWnd), WM_HOSTNODEFORSPLITTERCREATED, m_pStar->m_nRow, m_pStar->m_nCol);
				ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
		}
		m_pStar->NodeCreated();
		return bRet;
	}
	return m_pStar->Create(dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CStarWnd::OnSplitterReposition(WPARAM wParam, LPARAM lParam)
{
	switch (m_pStar->m_nViewType)
	{
	case CLRCtrl:
	case ActiveX:
	case TabbedWnd:
		m_pStar->m_pStarCommonData->m_pQuasar->HostPosChanged();
		break;
	default:
		break;
	}

	return CWnd::DefWindowProc(WM_SPLITTERREPOSITION, wParam, lParam);
}

int CStarWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pHubble->m_pCLRProxy)
		g_pHubble->m_pCLRProxy->HideMenuStripPopup();
	if (g_pHubble->m_pActiveHtmlWnd)
	{
		::PostMessage(g_pHubble->m_pActiveHtmlWnd->m_hWnd, WM_COSMOSMSG, 20190331, 0);
		g_pHubble->m_pActiveHtmlWnd = nullptr;
	}

	CQuasar* pQuasar = m_pStar->m_pRootObj->m_pStarCommonData->m_pQuasar;
	if (pQuasar->m_pGalaxyCluster->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = pQuasar->m_pGalaxyCluster->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	else if (g_pHubble->m_pActiveAppProxy)
	{
		HWND hMenuWnd = g_pHubble->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	BOOL b = pQuasar->m_bDesignerState;
	if (m_pStar->m_nViewType == BlankView && m_pStar->m_strCnnID == _T(""))
		b = true;
	if (m_pStar && m_pStar->m_pStarCommonData->m_pGalaxyCluster)
		m_pStar->m_pStarCommonData->m_pGalaxyCluster->Fire_NodeMouseActivate(m_pStar);

	if ((m_pStar->m_nViewType == TabbedWnd || m_pStar->m_nViewType == Splitter))
	{
		if (g_pHubble->m_pQuasar != m_pStar->m_pStarCommonData->m_pQuasar)
			::SetFocus(m_hWnd);
		g_pHubble->m_pActiveStar = m_pStar;
		g_pHubble->m_bWinFormActived = false;
		return MA_ACTIVATE;
	}

	if (!m_bCreateExternal)
		Invalidate(true);
	else
	{
		if ((m_pStar->m_nViewType != TabbedWnd && m_pStar->m_nViewType != Splitter))
		{
			if (g_pHubble->m_pQuasar != m_pStar->m_pStarCommonData->m_pQuasar || g_pHubble->m_pActiveStar != m_pStar)
				::SetFocus(m_hWnd);
		}
	}
	g_pHubble->m_pActiveStar = m_pStar;
	g_pHubble->m_bWinFormActived = false;
	if (m_pStar->m_pParentObj)
	{
		if (m_pStar->m_pParentObj->m_nViewType & TabbedWnd)
			m_pStar->m_pParentObj->m_pVisibleXMLObj = m_pStar;
	}
	g_pHubble->m_pQuasar = m_pStar->m_pStarCommonData->m_pQuasar;

	CWebPage* pHtmlWnd = g_pHubble->m_pQuasar->m_pWebPageWnd;
	CString strID = m_pStar->m_strName;

	if ((m_pStar->m_nViewType == ActiveX || m_pStar->m_nViewType == CLRCtrl))
	{
		if (pHtmlWnd)
		{
			CWormhole* pSession = m_pStar->m_pHubbleCloudSession;
			if (pSession)
			{
				pSession->InsertString(_T("msgID"), IPC_NODE_ONMOUSEACTIVATE_ID);
				pHtmlWnd->m_pChromeRenderFrameHost->SendHubbleMessage(pSession->m_pSession);
			}
		}
		return MA_NOACTIVATE;
	}

	if (b && m_bCreateExternal == false 
		&& m_pStar->m_strID.CompareNoCase(TGM_NUCLEUS)
		&&m_pStar->m_pDisp == NULL)
	{
		if (g_pHubble->m_pDesignWindowNode && g_pHubble->m_pDesignWindowNode != m_pStar)
		{
			CStarWnd* pWnd = ((CStarWnd*)g_pHubble->m_pDesignWindowNode->m_pHostWnd);
			if (pWnd && ::IsWindow(pWnd->m_hWnd))
			{
				pWnd->Invalidate(true);
			}
		}
		g_pHubble->m_pDesignWindowNode = m_pStar;
		Invalidate(true);
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
			strHandle.Format(_T("%d"), m_pStar->m_nViewType);
			pIPCInfo.m_strParam3 = strHandle;
			strHandle.Format(_T("%d"), pQuasar->m_hWnd);
			pIPCInfo.m_strParam4 = strHandle;
			//strHandle.Format(_T("%d"), m_pRootObj->m_pHostWnd->m_hWnd);
			pIPCInfo.m_strParam5 = _T("wndnode");
			pHtmlWnd->m_pChromeRenderFrameHost->SendHubbleMessage(&pIPCInfo);
		}
		return MA_ACTIVATEANDEAT;
	}
	else
		return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CStarWnd::OnEraseBkgnd(CDC* pDC)
{
	if (m_pStar->m_nViewType != BlankView)
		return true;
	CQuasar* pQuasar = m_pStar->m_pStarCommonData->m_pQuasar;
	BOOL bInDesignState = pQuasar->m_bDesignerState;
	CBitmap bit;
	RECT rt;
	GetClientRect(&rt);
	if (m_pStar->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		HWND hWnd = pQuasar->m_hWnd;
		if (::IsWindow(hWnd) && !::IsWindowVisible(hWnd))
		{
			//pQuasar->HostPosChanged();
			return false;
		}
	}
	else if ((m_pStar->m_nViewType == ActiveX || m_pStar->m_nViewType == CLRCtrl) && m_pStar->m_pDisp == nullptr)
	{
		bit.LoadBitmap(IDB_BITMAP_DESIGNER);
		CBrush br(&bit);
		pDC->FillRect(&rt, &br);
		pDC->SetBkMode(TRANSPARENT);
		CComBSTR bstrCaption(L"");
		m_pStar->get_Attribute(CComBSTR(L"caption"), &bstrCaption);
		CString strInfo = _T("");
		strInfo = strInfo +
			_T("\n  ----.NET Assembly don't exists or AxtiveX Control not Support!----");

		pDC->SetTextColor(RGB(255, 0, 255));
		pDC->DrawText(strInfo, &rt, DT_LEFT);
	}

	if (m_pStar->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pStar->m_pDisp == NULL) && m_bEraseBkgnd)
	{
		CString strText = _T("");
		if (bInDesignState && g_pHubble->m_pDesignWindowNode == m_pStar)
		{
			bit.LoadBitmap(IDB_BITMAP_DESIGNER);
			CBrush br(&bit);
			pDC->FillRect(&rt, &br);
			pDC->SetTextColor(RGB(255, 0, 255));
			CString str = g_pHubble->GetDesignerInfo(_T("SelectedText"));
			if (str == _T(""))
				strText = _T("\n\n  ") + g_pHubble->m_strNodeSelectedText;
			else
				strText = _T("\n\n  ") + str;
		}
		else
		{
			bit.LoadBitmap(IDB_BITMAP_GRID);
			CBrush br(&bit);
			pDC->FillRect(&rt, &br);
			CComBSTR bstrCaption(L"");
			m_pStar->get_Attribute(CComBSTR(L"caption"), &bstrCaption);
			CString strInfo = _T("\n\n  ");
			if (bInDesignState)
			{
				CString str = g_pHubble->GetDesignerInfo(_T("DesignerTip1"));
				if (str == _T(""))
					strInfo = strInfo + g_pHubble->m_strDesignerTip1;
				else
					strInfo = strInfo + str;
			}
			CString str = g_pHubble->GetDesignerInfo(_T("DesignerTip2"));
			if (str == _T(""))
				strInfo = strInfo + _T("\n  ") + g_pHubble->m_strDesignerTip2;
			else
				strInfo = strInfo + _T("\n  ") + str;
			if (pQuasar->m_pParentStar)
			{
				CString strTip = _T("");
				if (pQuasar->m_pParentStar->m_nViewType == CLRCtrl)
				{
					strTip.Format(_T("Sub Frame for .NET CLRCtrl: %s"), pQuasar->m_pParentStar->m_strName);
					strInfo = strInfo + _T("\n  ") + strTip;
					strTip.Format(_T("CLRCtrl Type: %s"), pQuasar->m_pParentStar->m_strCnnID);
					strInfo = strInfo + _T("\n  ") + strTip;
				}
				else if (m_pStar->m_pStarCommonData->m_pQuasar->m_pParentStar->m_nViewType == ActiveX)
				{
					strTip.Format(_T("Sub Frame for ActiveX Control: %s"), pQuasar->m_pParentStar->m_strName);
					strInfo = strInfo + _T("\n  ") + strTip;
					strTip.Format(_T("ActiveX ID: %s"), pQuasar->m_pParentStar->m_strCnnID);
					strInfo = strInfo + _T("\n  ") + strTip;
				}
			}
			strText.Format(strInfo, m_pStar->m_strName, CString(OLE2T(bstrCaption)));
			pDC->SetTextColor(RGB(255, 255, 255));
		}

		CString strKey = m_pStar->m_pRootObj->m_strKey;
		switch (pQuasar->m_nQuasarType)
		{
		case MDIClientQuasar:
		{
		}
		break;
		case SDIQuasar:
		{
			strText += _T("\n  ") + strKey;
		}
		break;
		case CtrlBarQuasar:
		{
		}
		break;
		case WinFormMDIClientQuasar:
		{
		}
		break;
		case WinFormMDIChildQuasar:
		{
		}
		break;
		case WinFormQuasar:
		{
		}
		break;
		case EclipseWorkBenchQuasar:
		{
			strText += _T("\n  ") + strKey;
		}
		break;
		case EclipseViewQuasar:
		{
		}
		break;
		case EclipseSWTQuasar:
		{
		}
		break;
		case WinFormControlQuasar:
		{
		}
		break;
		default:
			break;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strText, &rt, DT_LEFT);
	}
	return true;
}

BOOL CStarWnd::PreTranslateMessage(MSG* pMsg)
{
	if (m_pOleInPlaceActiveObject)
	{
		LRESULT hr = m_pOleInPlaceActiveObject->TranslateAccelerator((LPMSG)pMsg);
		if (hr == S_OK)
			return true;
		else
		{
			if (m_pStar->m_nViewType == CLRCtrl)
			{
				VARIANT_BOOL bShiftKey = false;
				if (::GetKeyState(VK_SHIFT) < 0)  // shift pressed
					bShiftKey = true;
				hr = g_pHubble->m_pCLRProxy->ProcessCtrlMsg(::GetWindow(m_hWnd, GW_CHILD), bShiftKey ? true : false);
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
		if (m_pStar->m_nViewType == CLRCtrl)
		{
			if (g_pHubble->m_pCLRProxy->ProcessFormMsg(m_hWnd, pMsg, 0))
				return true;
			return false;
		}
	}
	if (IsDialogMessage(pMsg))
		return true;
	return CWnd::PreTranslateMessage(pMsg);
}

void CStarWnd::OnDestroy()
{
	if (g_pHubble->m_pDesignWindowNode == m_pStar)
	{
		g_pHubble->m_pDesignWindowNode = NULL;
	}

	m_pStar->Fire_Destroy();
	CWnd::OnDestroy();
}

void CStarWnd::PostNcDestroy()
{
	delete m_pStar;
	CWnd::PostNcDestroy();
	delete this;
}

LRESULT CStarWnd::OnTabChange(WPARAM wParam, LPARAM lParam)
{
	int nOldPage = m_pStar->m_nActivePage;
	m_pStar->m_nActivePage = (int)wParam;
	IStar* pNode = nullptr;
	m_pStar->GetStar(0, wParam, &pNode);

	CQuasar* pQuasar = m_pStar->m_pStarCommonData->m_pQuasar;
	if (pNode)
	{
		CString str = _T("");
		str.Format(_T("%d"), wParam);
		m_pStar->put_Attribute(CComBSTR(L"activepage"), str.AllocSysString());
		CStar* _pNode = (CStar*)pNode;
		if (_pNode->m_nViewType == Splitter)
		{
			((CSplitterWnd*)_pNode->m_pHostWnd)->RecalcLayout();
		}
		else
		{
			pQuasar->HostPosChanged();
		}
		if (_pNode->m_pWebBrowser) {
			g_pHubble->m_pActiveHtmlWnd = _pNode->m_pWebBrowser->m_pVisibleWebWnd;
		}
		if (nOldPage != wParam)
		{
			::PostMessage(pQuasar->m_hWnd, WM_TANGRAMACTIVEPAGE, wParam, lParam);
			::SendMessage(_pNode->m_pHostWnd->m_hWnd, WM_TANGRAMACTIVEPAGE, wParam, lParam);
		}
		if (m_pStar->m_nViewType == TabbedWnd)
		{
			for (auto it : m_pStar->m_vChildNodes)
			{
				CStar* pNode = it;
				if (pNode->m_nCol != m_pStar->m_nActivePage)
				{
					pNode->m_pHostWnd->ShowWindow(SW_HIDE);
				}
			}
		}
		HWND h = ::GetParent(m_hWnd);
		if (h)
		{
			LRESULT lRes = ::SendMessage(h, WM_TANGRAMGETNODE, 0, 0);
			if (lRes)
			{
				CStar* pRetNode = (CStar*)lRes;
				if (pRetNode && pRetNode->m_nViewType == Splitter)
				{
					CQuasar* pQuasar = pRetNode->m_pStarCommonData->m_pQuasar;
					pQuasar->HostPosChanged();
				}
			}
		}
	}
	if (nOldPage != wParam)
	{
		m_pStar->Fire_TabChange(wParam, lParam);
		m_pStar->m_pStarCommonData->m_pGalaxyCluster->Fire_TabChange(m_pStar, wParam, lParam);
		if (pQuasar->m_pWebPageWnd)
		{
			::SendMessage(::GetParent(pQuasar->m_pWebPageWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
		}
	}

	LRESULT lRes = CWnd::DefWindowProc(WM_TABCHANGE, wParam, lParam);
	return lRes;
}

LRESULT CStarWnd::OnHubbleMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0 && lParam)//Create CLRCtrl Node
	{
		switch (lParam)
		{
		case 19920612:
		case 20200609:
		case 20200606:
		case 20200531:
		case 19631222:
		case 20191031:
		case 20180115:
			return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
			break;
		case 20190602:
		{
			CWinForm* pWinFormWnd = (CWinForm*)::SendMessage(m_hWnd, WM_TANGRAMDATA, 0, 20190214);
			if (pWinFormWnd)
				return (LRESULT)pWinFormWnd->m_pChildFormsInfo;
			return 0;
		}
		case 20200128:
		{
			if (m_pStar && m_pStar->m_pWebBrowser)
			{
				::SetWindowPos(m_pStar->m_pWebBrowser->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW);
			}
		}
		break;
		default:
		{
			if (lParam == 20022017)
			{
				break;
			}
			CString strCnnID = (LPCTSTR)lParam;
			if (strCnnID.Find(_T(",")) != -1 && g_pHubble->m_pCLRProxy)
			{
				m_pStar->m_pDisp = g_pHubble->m_pCLRProxy->CreateObject(strCnnID.AllocSysString(), m_hWnd, m_pStar);

				if (m_pStar->m_pDisp)
				{
					m_pStar->m_pStarCommonData->m_mapLayoutNodes[m_pStar->m_strName] = m_pStar;
					m_pStar->m_nViewType = CLRCtrl;
					if (g_pHubble->m_hFormNodeWnd)
					{
						m_hFormWnd = g_pHubble->m_hFormNodeWnd;
						RECT rc;
						::GetClientRect(m_hWnd, &rc);
						::SetWindowPos(m_hFormWnd, HWND_BOTTOM, 0, 0, rc.right, rc.bottom, SWP_NOACTIVATE | SWP_NOREDRAW);
						g_pHubble->m_hFormNodeWnd = nullptr;
						return 0;
					}
					CAxWindow m_Wnd;
					m_Wnd.Attach(m_hWnd);
					CComPtr<IUnknown> pUnk;
					m_Wnd.AttachControl(m_pStar->m_pDisp, &pUnk);
					CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pStar->m_pDisp);
					if (pIOleInPlaceActiveObject)
						m_pOleInPlaceActiveObject = pIOleInPlaceActiveObject.Detach();
					m_Wnd.Detach();
					if (m_mapDockCtrl.size())
					{
						HWND hPage = m_pStar->m_pStarCommonData->m_pGalaxyCluster->m_hWnd;
						::SendMessage(hPage, WM_COSMOSMSG, (WPARAM)this, 1963);
					}
				}
				else
				{
					m_bCreateExternal = true;
				}
			}
			else
			{
				CString strName = strCnnID;
				int _nPos = strName.Find(_T("."));
				if (_nPos != -1)
					strName = strName.Mid(_nPos + 1);
				_nPos = strName.ReverseFind('.');
				if (_nPos != -1)
					strName = strName.Left(_nPos);
				CStar* pNode = m_pStar->m_pRootObj;
				CStar* pParent = m_pStar->m_pParentObj;
				if (pParent)
				{
					strName = pParent->m_strName + _T("_") + strName;
				}
				auto it = pNode->m_pStarCommonData->m_mapAxNodes.find(strName);
				if (it != pNode->m_pStarCommonData->m_mapAxNodes.end())
				{
					BOOL bGetNew = false;
					int nIndex = 0;
					while (bGetNew == false)
					{
						CString strNewName = _T("");
						strNewName.Format(_T("%s%d"), strName, nIndex);
						it = pNode->m_pStarCommonData->m_mapAxNodes.find(strNewName);
						if (it == pNode->m_pStarCommonData->m_mapAxNodes.end())
						{
							pNode->m_pStarCommonData->m_mapAxNodes[strNewName] = m_pStar;
							strName = strNewName;
							bGetNew = true;
							break;
						}
						nIndex++;
					}
				}
				else
				{
					pNode->m_pStarCommonData->m_mapAxNodes[strName] = m_pStar;
				}
				m_pStar->put_Attribute(CComBSTR(L"id"), strName.AllocSysString());
				m_pStar->m_strName = strName;
				BOOL bWebCtrl = false;
				CString strURL = _T("");
				strCnnID.MakeLower();
				auto nPos = strCnnID.Find(_T("http:"));
				if (nPos == -1)
					nPos = strCnnID.Find(_T("https:"));
				if (m_pStar->m_pStarCommonData->m_pQuasar)
				{
					CComBSTR bstr;
					m_pStar->get_Attribute(CComBSTR("InitInfo"), &bstr);
					CString str = _T("");
					str += bstr;
					if (str != _T("") && m_pStar->m_pStarCommonData->m_pGalaxyCluster)
					{
						LRESULT hr = ::SendMessage(m_pStar->m_pStarCommonData->m_pGalaxyCluster->m_hWnd, WM_GETNODEINFO, (WPARAM)OLE2T(bstr), (LPARAM)::GetParent(m_hWnd));
						if (hr)
						{
							CString strInfo = _T("");
							CWindow m_wnd;
							m_wnd.Attach(::GetParent(m_hWnd));
							m_wnd.GetWindowText(strInfo);
							strCnnID += strInfo;
							m_wnd.Detach();
						}
					}
				}

				if (strCnnID.Find(_T("http://")) != -1 || strCnnID.Find(_T("https://")) != -1)
				{
					bWebCtrl = true;
					strURL = strCnnID;
					strCnnID = _T("shell.explorer.2");
				}

				if (strCnnID.Find(_T("res://")) != -1 || ::PathFileExists(strCnnID))
				{
					bWebCtrl = true;
					strURL = strCnnID;
					strCnnID = _T("shell.explorer.2");
					if (strCnnID.Find(_T("res://")) != -1)
					{
						CString strTemp = strURL;
						strTemp.Replace(_T("res://"), _T(""));
						int nPos = strTemp.ReverseFind('/');
						CString strTemp2 = strTemp.Mid(nPos);
						strTemp = strTemp.Left(nPos);
						if (::PathFileExists(strTemp) == false)
						{
							strTemp = g_pHubble->m_strAppPath + strTemp;
							if (::PathFileExists(strTemp) == false)
							{
								strTemp = g_pHubble->m_strProgramFilePath + _T("Cosmos\\") + strTemp;
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

				if (strCnnID.CompareNoCase(_T("about:blank")) == 0)
				{
					bWebCtrl = true;
					strURL = strCnnID;
					strCnnID = _T("shell.explorer.2");
				}

				if (m_pStar->m_pDisp == NULL)
				{
					CComPtr<IDispatch> pDisp;
					HRESULT hr = pDisp.CoCreateInstance(CComBSTR(strCnnID));
					if (hr == S_OK)
					{
						m_pStar->m_pDisp = pDisp.Detach();
					}
				}
				if (m_pStar->m_pDisp)
				{
					m_pStar->m_pStarCommonData->m_mapLayoutNodes[m_pStar->m_strName] = m_pStar;
					m_pStar->m_nViewType = ActiveX;
					CAxWindow m_Wnd;
					m_Wnd.Attach(m_hWnd);
					CComPtr<IUnknown> pUnk;
					m_Wnd.AttachControl(m_pStar->m_pDisp, &pUnk);
					CComQIPtr<IWebBrowser2> pWebDisp(m_pStar->m_pDisp);
					if (pWebDisp)
					{
						bWebCtrl = true;
						m_pStar->m_strURL = strURL;
						if (m_pStar->m_strURL == _T(""))
							m_pStar->m_strURL = strCnnID;

						CComPtr<IAxWinAmbientDispatch> spHost;
						LRESULT hr = m_Wnd.QueryHost(&spHost);
						if (SUCCEEDED(hr))
						{
							CComBSTR bstr;
							m_pStar->get_Attribute(CComBSTR("scrollbar"), &bstr);
							CString str = OLE2T(bstr);
							if (str == _T("1"))
								spHost->put_DocHostFlags(DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE | DOCHOSTUIFLAG_THEME);//DOCHOSTUIFLAG_DIALOG|
							else
								spHost->put_DocHostFlags(/*DOCHOSTUIFLAG_DIALOG|*/DOCHOSTUIFLAG_SCROLL_NO | DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE | DOCHOSTUIFLAG_THEME);

							if (m_pStar->m_strURL != _T(""))
							{
								pWebDisp->Navigate2(&CComVariant(m_pStar->m_strURL), &CComVariant(navNoReadFromCache | navNoWriteToCache), NULL, NULL, NULL);
								m_pStar->m_bWebInit = true;
							}
						}
					}
					((CStarWnd*)m_pStar->m_pHostWnd)->m_pStar = m_pStar;
					HWND hPage = m_pStar->m_pStarCommonData->m_pGalaxyCluster->m_hWnd;
					::SendMessage(hPage, WM_COSMOSMSG, (WPARAM)((CStarWnd*)m_pStar->m_pHostWnd), 1963);
					CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pStar->m_pDisp);
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
	if (lParam == 20191031 || lParam == 20200609|| lParam == 20200606)
		return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
	if (lParam == 20200208)
		return 0;
	CString str = (LPCTSTR)lParam;
	IStar* _pNode = nullptr;
	CStar* pOldNode = (CStar*)m_pStar;
	if (m_pStar->m_hHostWnd == 0)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		m_pStar->m_hHostWnd = ::CreateWindowEx(NULL, L"Tangram Node Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_hWnd, NULL, AfxGetInstanceHandle(), NULL);
		m_pStar->m_hChildHostWnd = ::CreateWindowEx(NULL, L"Tangram Node Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_pStar->m_hHostWnd, NULL, AfxGetInstanceHandle(), NULL);
		IQuasar* pQuasar = nullptr;
		m_pStar->m_pStarCommonData->m_pGalaxyCluster->CreateQuasar(CComVariant(0), CComVariant((long)pOldNode->m_hChildHostWnd), CComBSTR(L"Design"), &pQuasar);
		IStar* pNode = nullptr;
		pQuasar->Observe(CComBSTR(L""), str.AllocSysString(), &pNode);
		m_bEraseBkgnd = false;

		((CStar*)pNode)->m_pRootObj = m_pStar->m_pRootObj;
		((CStar*)pNode)->m_pParentObj = m_pStar->m_pParentObj;
		m_pStar->m_pStarCommonData->m_mapLayoutNodes[((CStar*)pNode)->m_strName] = (CStar*)pNode;
		((CQuasar*)pQuasar)->m_bDesignerState = m_pStar->m_pStarCommonData->m_pQuasar->m_bDesignerState;
		CString strXml = ((CStar*)pNode)->m_pHostParse->xml();
		CTangramXmlParse* pNew = ((CStar*)pNode)->m_pHostParse;
		CTangramXmlParse* pOld = pOldNode->m_pHostParse;
		CTangramXmlParse* pParent = m_pStar->m_pHostParse->m_pParentParse;
		CTangramXmlParse* pRet = nullptr;
		if (pParent)
		{
			pRet = pParent->ReplaceNode(pOld, pNew, _T(""));
			CString str = pRet->xml();
			int nCount = pRet->GetCount();
			((CStar*)pNode)->m_pHostParse = pRet;
			m_pStar->m_pHostParse = pRet;

			CStar* pChildNode = nullptr;
			for (auto it2 : ((CStar*)pNode)->m_vChildNodes)
			{
				pChildNode = it2;
				pChildNode->m_pRootObj = m_pStar->m_pRootObj;
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
			m_pStar->m_vChildNodes.push_back(((CStar*)pNode));
		}

		strXml = m_pStar->m_pStarCommonData->m_pHubbleParse->xml();
		g_pHubble->m_pDesignWindowNode = m_pStar;
		auto it = m_pStar->m_pStarCommonData->m_pQuasar->m_pGalaxyCluster->m_mapQuasar.find(pOldNode->m_hChildHostWnd);
		if (it != m_pStar->m_pStarCommonData->m_pQuasar->m_pGalaxyCluster->m_mapQuasar.end())
			m_pStar->m_pStarCommonData->m_pQuasar->m_pGalaxyCluster->m_mapQuasar.erase(it);
	}
	return -1;
}

LRESULT CStarWnd::OnActiveTangramObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pStar->m_nViewType == CLRCtrl)
		::SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_pStar->m_pStarCommonData->m_pQuasar->HostPosChanged();
	::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return CWnd::DefWindowProc(WM_TGM_SETACTIVEPAGE, wParam, lParam);
}

LRESULT CStarWnd::OnGetTangramObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pStar)
	{
		HWND hBrowser = (HWND)lParam;
		if (::IsWindow(hBrowser))
		{
			auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
			if (it != g_pHubble->m_mapBrowserWnd.end())
			{
				m_pStar->m_pWebBrowser = (CBrowser*)it->second;
			}
		}
		return (LRESULT)m_pStar;
	}
	return CWnd::DefWindowProc(WM_TANGRAMGETNODE, wParam, lParam);;
}

LRESULT CStarWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_bCreateExternal)
	{
		switch (message)
		{
		case WM_COMMAND:
		{
			WNDPROC* lplpfn = GetSuperWndProcAddr();
			LRESULT res = CallWindowProc(*lplpfn, m_hWnd, message, wParam, lParam);
			HWND hWnd = (HWND)lParam;
			if (::IsWindow(hWnd) && m_pStar)
			{
				::GetClassName(hWnd, g_pHubble->m_szBuffer, MAX_PATH);
				m_pStar->Fire_ControlNotify(m_pStar, HIWORD(wParam), LOWORD(wParam), lParam, CComBSTR(g_pHubble->m_szBuffer));
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
		case WM_SHOWWINDOW:
		{
			if (wParam && m_pStar->m_strURL != _T(""))
			{
				CComQIPtr<IWebBrowser2> pWebCtrl(m_pStar->m_pDisp);
				if (pWebCtrl)
				{
					if (m_pStar->m_bWebInit == false)
					{
						pWebCtrl->Navigate2(&CComVariant(m_pStar->m_strURL), &CComVariant(navNoReadFromCache | navNoWriteToCache), NULL, NULL, NULL);
						m_pStar->m_bWebInit = true;
					}
				}
			}
			break;
		}
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CStarWnd::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	if (m_pStar && m_pStar->m_nViewType == CLRCtrl && m_pStar->m_hHostWnd)
	{
		if (m_bNoMove)
		{
			lpwndpos->flags |= SWP_NOMOVE;
			CStarWnd* pNodeWnd = (CStarWnd*)m_pStar->m_pHostWnd;
			if (m_pStar->m_pParentObj->m_nViewType == Splitter)
			{
				CGrid* pWnd = (CGrid*)m_pStar->m_pParentObj->m_pHostWnd;
				::PostMessage(pWnd->m_hWnd, WM_COSMOSMSG, 3, 1993);
			}
		}
	}
	if (m_pStar && m_pStar->m_hHostWnd)
	{
		if (m_pStar->m_nViewType == CLRCtrl)
		{
			if (m_bNoMove)
			{
				m_bNoMove = false;
			}
			return;
		}
		else if (m_pStar->m_nViewType == BlankView)
		{
			::SetWindowPos(m_pStar->m_hHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
			if (m_pStar->m_hChildHostWnd)
				::SetWindowPos(m_pStar->m_hChildHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
	}

	if (m_pStar->m_pWebBrowser)
	{
		::SetWindowPos(m_pStar->m_pWebBrowser->m_hWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE);// | SWP_NOREDRAW);
	}
	else {
		if (m_hFormWnd)
			::SetWindowPos(m_hFormWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		else if (m_pStar->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
		{
			m_pStar->m_pStarCommonData->m_pQuasar->HostPosChanged();
		}
	}
	if (m_pStar->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pStar->m_pDisp == NULL) && m_pStar != m_pStar->m_pRootObj)
	{
		return;
	}

	if (m_bCreateExternal)// || m_pStar == m_pStar->m_pRootObj)
	{
		Invalidate(true);
	}
	CWnd::OnWindowPosChanged(lpwndpos);
}
