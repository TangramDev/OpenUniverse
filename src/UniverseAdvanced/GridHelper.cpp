/********************************************************************************
*					Open Universe - version 1.0.1.10							*
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
*
********************************************************************************/

// TangramView.cpp : implementation file
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "GridHelper.h"
#include "Grid.h"
#include "Galaxy.h"
#include "GridWnd.h"
#include "Wormhole.h"
#include "TangramHtmlTreeWnd.h"
#include "OfficePlus\ExcelPlus\ExcelAddin.h"
#include "OfficePlus\ExcelPlus\ExcelPlusWnd.h"

#include "chromium/WebPage.h"

// CGridHelper

IMPLEMENT_DYNCREATE(CGridHelper, CWnd)

CGridHelper::CGridHelper()
{
	m_hFormWnd = NULL;
	m_bNoMove = false;
	m_bBKWnd = false;
	m_bCreateExternal = false;
	m_bEraseBkgnd = true;
	m_pGrid = nullptr;
	m_pXHtmlTree = nullptr;
	m_pParentGrid = nullptr;
	m_pOleInPlaceActiveObject = nullptr;
	m_strKey = m_strXml = _T("");
}

CGridHelper::~CGridHelper()
{
}

BEGIN_MESSAGE_MAP(CGridHelper, CWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE(WM_TABCHANGE, OnTabChange)
	ON_MESSAGE(WM_COSMOSMSG, OnHubbleMsg)
	ON_MESSAGE(WM_HUBBLE_DATA, OnHubbleData)
	ON_MESSAGE(WM_HUBBLE_GETNODE, OnGetHubbleObj)
	ON_MESSAGE(WM_TGM_SETACTIVEPAGE, OnActiveTangramObj)

	ON_MESSAGE(WM_SPLITTERREPOSITION, OnSplitterReposition)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CGridHelper diagnostics

#ifdef _DEBUG
void CGridHelper::AssertValid() const
{
	///CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGridHelper::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif
#endif //_DEBUG

//CGridHelper message handlers
BOOL CGridHelper::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_pGrid = g_pHubble->m_pActiveGrid;
	m_pGrid->m_nID = nID;
	m_pGrid->m_pHostWnd = this;

	if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		CGalaxy* pGalaxy = m_pGrid->m_pGridShareData->m_pGalaxy;
		pGalaxy->m_pBindingGrid = m_pGrid;

		m_pGrid->m_pGridShareData->m_pHostClientView = this;
		CGalaxyCluster* pGalaxyCluster = pGalaxy->m_pGalaxyCluster;
		HWND hWnd = CreateWindow(L"Hubble Grid Class", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, pParentWnd->m_hWnd, (HMENU)nID, AfxGetInstanceHandle(), NULL);
		BOOL bRet = SubclassWindow(hWnd);
		if (m_pGrid->m_pParentObj)
		{
			if (m_pGrid->m_pParentObj->m_nViewType == Grid)
			{
				::PostMessage(::GetParent(m_hWnd), WM_HOSTNODEFORSPLITTERCREATED, m_pGrid->m_nRow, m_pGrid->m_nCol);
				ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
			else if (g_pHubble->m_pMDIMainWnd && pGalaxy->m_nGalaxyType == GalaxyType::MDIClientGalaxy && pGalaxy->m_pBindingGrid)
			{
				RECT rc = { 0,0,0,0 };
				if (::SendMessage(g_pHubble->m_pMDIMainWnd->m_hWnd, WM_QUERYAPPPROXY, (WPARAM)&rc, 19921989) == 19921989)
				{
					::SetWindowPos(pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd, m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,  /*SWP_NOSENDCHANGING| SWP_NOZORDER |*/ SWP_NOACTIVATE | SWP_FRAMECHANGED);
				}
			}
		}
		m_pGrid->NodeCreated();
		return bRet;
	}
	return m_pGrid->Create(dwStyle, rect, pParentWnd, nID, pContext);
}

LRESULT CGridHelper::OnSplitterReposition(WPARAM wParam, LPARAM lParam)
{
	switch (m_pGrid->m_nViewType)
	{
	case CLRCtrl:
	case ActiveX:
	case TabGrid:
		m_pGrid->m_pGridShareData->m_pGalaxy->HostPosChanged();
		break;
		break;
	default:
		break;
	}

	return CWnd::DefWindowProc(WM_SPLITTERREPOSITION, wParam, lParam);
}

int CGridHelper::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pHubble->m_pCLRProxy)
		g_pHubble->m_pCLRProxy->HideMenuStripPopup();
	if (g_pHubble->m_pActiveHtmlWnd)
	{
		::PostMessage(g_pHubble->m_pActiveHtmlWnd->m_hWnd, WM_COSMOSMSG, 20190331, 0);
		g_pHubble->m_pActiveHtmlWnd = nullptr;
	}

	CGalaxy* pGalaxy = m_pGrid->m_pRootObj->m_pGridShareData->m_pGalaxy;
	if (pGalaxy->m_pGalaxyCluster->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = pGalaxy->m_pGalaxyCluster->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	else if (g_pHubble->m_pActiveAppProxy)
	{
		HWND hMenuWnd = g_pHubble->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	BOOL b = pGalaxy->m_bDesignerState;
	if (g_pHubble->m_pDesignerGalaxyCluster)
		if (pGalaxy->m_pGalaxyCluster == g_pHubble->m_pDesignerGalaxyCluster || m_pGrid->m_strObjTypeID != _T(""))
			b = false;
	if (m_pGrid->m_nViewType == BlankView && m_pGrid->m_strObjTypeID == _T(""))
		b = true;
	if (g_pHubble->m_pDocDOMTree && g_pHubble->m_pCLRProxy)
	{
		if (b)
		{
			if (g_pHubble->m_hVSToolBoxWnd)
			{
				if (::IsChild(g_pHubble->m_hVSToolBoxWnd, m_hWnd) == false)
					g_pHubble->m_pCLRProxy->SelectGrid(m_pGrid);
			}
			else if (::IsChild(g_pHubble->m_hHostWnd, m_hWnd) == false)
				g_pHubble->m_pCLRProxy->SelectGrid(m_pGrid);
		}
	}
	if (m_pGrid && m_pGrid->m_pGridShareData->m_pGalaxyCluster)
		m_pGrid->m_pGridShareData->m_pGalaxyCluster->Fire_NodeMouseActivate(m_pGrid);

	if ((m_pGrid->m_nViewType == TabGrid || m_pGrid->m_nViewType == Grid))
	{
		if (g_pHubble->m_pGalaxy != m_pGrid->m_pGridShareData->m_pGalaxy)
			::SetFocus(m_hWnd);
		g_pHubble->m_pActiveGrid = m_pGrid;
		g_pHubble->m_bWinFormActived = false;
		return MA_ACTIVATE;
	}

	if (!m_bCreateExternal)
		Invalidate(true);
	else
	{
		if ((m_pGrid->m_nViewType != TabGrid && m_pGrid->m_nViewType != Grid))
		{
			if (g_pHubble->m_pGalaxy != m_pGrid->m_pGridShareData->m_pGalaxy || g_pHubble->m_pActiveGrid != m_pGrid)
				::SetFocus(m_hWnd);
		}
	}
	g_pHubble->m_pActiveGrid = m_pGrid;
	g_pHubble->m_bWinFormActived = false;
	g_pHubble->m_pGalaxy = m_pGrid->m_pGridShareData->m_pGalaxy;

	CWebPage* pHtmlWnd = g_pHubble->m_pGalaxy->m_pWebPageWnd;
	CString strID = m_pGrid->m_strName;

	if ((m_pGrid->m_nViewType == ActiveX || m_pGrid->m_nViewType == CLRCtrl))
	{
		if (pHtmlWnd)
		{
			CWormhole* pSession = m_pGrid->m_pHubbleCloudSession;
			if (pSession)
			{
				pSession->InsertString(_T("msgID"), IPC_NODE_ONMOUSEACTIVATE_ID);
				pHtmlWnd->m_pChromeRenderFrameHost->SendHubbleMessage(pSession->m_pSession);
			}
		}
		return MA_NOACTIVATE;
	}

	if (b && m_bCreateExternal == false && m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS))
	{
		g_pHubble->CreateCommonDesignerToolBar();
		if (g_pHubble->m_pHostViewDesignerNode)
		{
			HWND hwnd = pGalaxy->m_hWnd;
			::GetClassName(::GetParent(hwnd), g_pHubble->m_szBuffer, MAX_PATH);
			CString strName = CString(g_pHubble->m_szBuffer);
			if (strName.Find(_T("Afx:ControlBar:")) != -1)
			{
				pGalaxy->m_nGalaxyType = CtrlBarGalaxy;
			}
			::GetClassName(hwnd, g_pHubble->m_szBuffer, MAX_PATH);
			strName = CString(g_pHubble->m_szBuffer);
			CUniverseMDIChild* pWnd = (CUniverseMDIChild*)::SendMessage(::GetParent(pGalaxy->m_hWnd), WM_COSMOSMSG, 0, 19631222);
			if (pWnd)
			{
				g_pHubble->m_pHostViewDesignerNode->put_Caption(CComBSTR(L"Show Host MFC View for MFC Document"));
			}
			else if (strName == _T("MDIClient"))
			{
				g_pHubble->m_pHostViewDesignerNode->put_Caption(CComBSTR(L"Show MDI Client"));
			}
			else if (pGalaxy->m_pGalaxyInfo)
			{
				CString s = _T("");
				s.Format(_T("Show .NET Ctrl \"%s\" at Control '%s'"), pGalaxy->m_pGalaxyInfo->m_strCtrlName, pGalaxy->m_pGalaxyInfo->m_strParentCtrlName);
				g_pHubble->m_pHostViewDesignerNode->put_Caption(CComBSTR(s));
			}
			else if (pGalaxy->m_nGalaxyType == GalaxyType::CtrlBarGalaxy)
			{
				g_pHubble->m_pHostViewDesignerNode->put_Caption(CComBSTR(L"Show Control Bar Host Object"));
			}
			else
			{
				g_pHubble->m_pHostViewDesignerNode->put_Caption(CComBSTR(L"Create Host Node"));
			}
		}
		if (m_pGrid->m_pDisp == NULL)
		{
			if (::IsWindow(g_pHubble->m_hHostWnd) == false)
			{
				g_pHubble->m_hHostWnd = ::CreateWindowEx(NULL, L"Hubble Grid Class", _T("Tangram Designer Helper Window"), WS_OVERLAPPED | WS_CAPTION, 0, 0, 0, 0, NULL, NULL, theApp.m_hInstance, NULL);
				g_pHubble->m_mapValInfo[_T("hostwindow")] = CComVariant((LONGLONG)g_pHubble->m_hHostWnd);
			}
			if (g_pHubble->m_pDesignGrid && g_pHubble->m_pDesignGrid != m_pGrid)
			{
				CGridHelper* pWnd = ((CGridHelper*)g_pHubble->m_pDesignGrid->m_pHostWnd);
				if (pWnd && ::IsWindow(pWnd->m_hWnd))
				{
					pWnd->Invalidate(true);
				}
			}
			g_pHubble->m_pDesignGrid = m_pGrid;
			Invalidate(true);
		}

		if (g_pHubble->m_pDesigningFrame != pGalaxy)
		{
			g_pHubble->m_pHostDesignUINode = m_pGrid->m_pRootObj;
			g_pHubble->m_pDesigningFrame = pGalaxy;
			pGalaxy->UpdateDesignerTreeInfo();
		}
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
			strHandle.Format(_T("%d"), m_pGrid->m_nViewType);
			pIPCInfo.m_strParam3 = strHandle;
			strHandle.Format(_T("%d"), pGalaxy->m_hWnd);
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

BOOL CGridHelper::OnEraseBkgnd(CDC* pDC)
{
	if (m_pGrid->m_nViewType != BlankView)
		return true;
	CGalaxy* pGalaxy = m_pGrid->m_pGridShareData->m_pGalaxy;
	BOOL bInDesignState = pGalaxy->m_bDesignerState;
	CBitmap bit;
	RECT rt;
	GetClientRect(&rt);
	if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
	{
		HWND hWnd = pGalaxy->m_hWnd;
		if (::IsWindow(hWnd) && !::IsWindowVisible(hWnd))
		{
			//pGalaxy->HostPosChanged();
			return false;
		}
	}
	//else if ((m_pGrid->m_nViewType == ActiveX || m_pGrid->m_nViewType == CLRCtrl) && m_pGrid->m_pDisp == nullptr)
	//{
	//	bit.LoadBitmap(IDB_BITMAP_DESIGNER);
	//	CBrush br(&bit);
	//	pDC->FillRect(&rt, &br);
	//	pDC->SetBkMode(TRANSPARENT);
	//	CComBSTR bstrCaption(L"");
	//	m_pGrid->get_Attribute(CComBSTR(L"caption"), &bstrCaption);
	//	CString strInfo = _T("");
	//	strInfo = strInfo +
	//		_T("\n  ----.NET Assembly don't exists or AxtiveX Control not Support!----");

	//	pDC->SetTextColor(RGB(255, 0, 255));
	//	pDC->DrawText(strInfo, &rt, DT_LEFT);
	//}

	if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pGrid->m_pDisp == NULL) && m_bEraseBkgnd)
	{
		CString strText = _T("");
		bit.LoadBitmap(IDB_BITMAP_DESIGNER);
		CBrush br(&bit);
		pDC->FillRect(&rt, &br);
		if (bInDesignState && g_pHubble->m_pDesignGrid == m_pGrid)
		{
			pDC->SetTextColor(RGB(255, 0, 255));
			CString str = g_pHubble->GetDesignerInfo(_T("SelectedText"));
			if (str == _T(""))
				strText = _T("\n\n  ") + g_pHubble->m_strGridSelectedText;
			else
				strText = _T("\n\n  ") + str;
		}
		else
		{
			CComBSTR bstrCaption(L"");
			m_pGrid->get_Attribute(CComBSTR(L"caption"), &bstrCaption);
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
			if (pGalaxy->m_pParentGrid)
			{
				CString strTip = _T("");
				if (pGalaxy->m_pParentGrid->m_nViewType == CLRCtrl)
				{
					strTip.Format(_T("Sub Frame for .NET CLRCtrl: %s"), pGalaxy->m_pParentGrid->m_strName);
					strInfo = strInfo + _T("\n  ") + strTip;
					strTip.Format(_T("CLRCtrl Type: %s"), pGalaxy->m_pParentGrid->m_strObjTypeID);
					strInfo = strInfo + _T("\n  ") + strTip;
				}
				else if (m_pGrid->m_pGridShareData->m_pGalaxy->m_pParentGrid->m_nViewType == ActiveX)
				{
					strTip.Format(_T("Sub Frame for ActiveX Control: %s"), pGalaxy->m_pParentGrid->m_strName);
					strInfo = strInfo + _T("\n  ") + strTip;
					strTip.Format(_T("ActiveX ID: %s"), pGalaxy->m_pParentGrid->m_strObjTypeID);
					strInfo = strInfo + _T("\n  ") + strTip;
				}
			}
			strText.Format(strInfo, m_pGrid->m_strName, CString(OLE2T(bstrCaption)));
			pDC->SetTextColor(RGB(255, 255, 255));
		}

		//CString strKey = m_pGrid->m_pRootObj->m_strKey;
		//switch (pGalaxy->m_nGalaxyType)
		//{
		//case MDIClientGalaxy:
		//{
		//	CGalaxyCluster* pGalaxyCluster = pGalaxy->m_pGalaxyCluster;
		//	if (pGalaxyCluster)
		//	{
		//		LRESULT lRes = ::SendMessage(pGalaxyCluster->m_hWnd, WM_QUERYAPPPROXY, 0, 0);
		//		if (lRes > 0)
		//		{
		//			IUniverseAppProxy* pProxy = (IUniverseAppProxy*)lRes;
		//			auto it = g_pHubble->m_mapTemplateInfo.find(strKey);
		//			if (it != g_pHubble->m_mapTemplateInfo.end())
		//			{
		//				CString strInfo = _T("");
		//				strInfo.Format(_T("  MDIClientGalaxy for Document Template : %s"), strKey);
		//				strText += _T("  ") + strInfo;
		//			}
		//		}
		//	}
		//}
		//break;
		//case MDIChildGalaxy:
		//{
		//	if (pGalaxy->m_pDoc)
		//	{
		//		CString strInfo = _T("");
		//		strInfo.Format(_T("  MDIChildGalaxy for Document Template : %s"), pGalaxy->m_pDoc->m_strDocID);
		//		strText += _T("  ") + strInfo;

		//	}
		//}
		//break;
		//case SDIGalaxy:
		//{
		//	strText += _T("\n  ") + strKey;
		//}
		//break;
		//case CtrlBarGalaxy:
		//{
		//	CGalaxyCluster* pGalaxyCluster = pGalaxy->m_pGalaxyCluster;
		//	if (pGalaxyCluster)
		//	{
		//		LRESULT lRes = ::SendMessage(pGalaxyCluster->m_hWnd, WM_QUERYAPPPROXY, 0, 0);
		//		if (lRes > 0)
		//		{
		//			IUniverseAppProxy* pProxy = (IUniverseAppProxy*)lRes;
		//			auto it = g_pHubble->m_mapTemplateInfo.find(strKey);
		//			if (it != g_pHubble->m_mapTemplateInfo.end())
		//			{
		//				CString strInfo = _T("");
		//				strInfo.Format(_T("  Control Bar for Document Template: %s"), strKey);
		//				strText += _T("\n  ") + strInfo;
		//			}
		//		}
		//	}
		//}
		//break;
		//case WinFormMDIClientGalaxy:
		//{
		//}
		//break;
		//case WinFormMDIChildGalaxy:
		//{
		//}
		//break;
		//case WinFormGalaxy:
		//{
		//}
		//break;
		//case EclipseWorkBenchGalaxy:
		//{
		//	strText += _T("\n  ") + strKey;
		//}
		//break;
		//case EclipseViewGalaxy:
		//{
		//}
		//break;
		//case EclipseSWTGalaxy:
		//{
		//}
		//break;
		//case WinFormControlGalaxy:
		//{
		//}
		//break;
		//default:
		//	break;
		//}
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strText, &rt, DT_LEFT);
	}
	return true;
}

BOOL CGridHelper::PreTranslateMessage(MSG* pMsg)
{
	if (m_pXHtmlTree)
	{
		return m_pXHtmlTree->PreTranslateMessage(pMsg);
	}

	if (m_pOleInPlaceActiveObject)
	{
		LRESULT hr = m_pOleInPlaceActiveObject->TranslateAccelerator((LPMSG)pMsg);
		if (hr == S_OK)
			return true;
		else
		{
			if (m_pGrid->m_nViewType == CLRCtrl)
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
		if (m_pGrid->m_nViewType == CLRCtrl)
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

void CGridHelper::OnDestroy()
{
	if (g_pHubble->m_pDesignGrid == m_pGrid)
	{
		if (g_pHubble->m_pCLRProxy)
			g_pHubble->m_pCLRProxy->SelectGrid(NULL);
		g_pHubble->m_pDesignGrid = NULL;
	}

	m_pGrid->Fire_Destroy();
	CWnd::OnDestroy();
}

void CGridHelper::PostNcDestroy()
{
	delete m_pGrid;
	CWnd::PostNcDestroy();
	delete this;
}

LRESULT CGridHelper::OnTabChange(WPARAM wParam, LPARAM lParam)
{
	int nOldPage = m_pGrid->m_nActivePage;
	m_pGrid->m_nActivePage = (int)wParam;
	IGrid* pGrid = nullptr;
	m_pGrid->GetGrid(0, wParam, &pGrid);

	CGalaxy* pGalaxy = m_pGrid->m_pGridShareData->m_pGalaxy;
	if (pGrid)
	{
		CString str = _T("");
		str.Format(_T("%d"), wParam);
		m_pGrid->put_Attribute(CComBSTR(L"activepage"), str.AllocSysString());
		CGrid* _pGrid = (CGrid*)pGrid;
		if (_pGrid->m_nViewType == Grid)
		{
			((CSplitterWnd*)_pGrid->m_pHostWnd)->RecalcLayout();
		}
		else
		{
			pGalaxy->HostPosChanged();
		}
		if (_pGrid->m_pWebBrowser) {
			g_pHubble->m_pActiveHtmlWnd = _pGrid->m_pWebBrowser->m_pVisibleWebWnd;
		}
		if (nOldPage != wParam)
		{
			::PostMessage(pGalaxy->m_hWnd, WM_HUBBLE_ACTIVEPAGE, wParam, lParam);
			::SendMessage(_pGrid->m_pHostWnd->m_hWnd, WM_HUBBLE_ACTIVEPAGE, wParam, lParam);
		}
		if (m_pGrid->m_nViewType == TabGrid)
		{
			for (auto it : m_pGrid->m_vChildNodes)
			{
				CGrid* pGrid = it;
				if (pGrid->m_nCol != m_pGrid->m_nActivePage)
				{
					pGrid->m_pHostWnd->ShowWindow(SW_HIDE);
				}
			}
		}
		HWND h = ::GetParent(m_hWnd);
		if (h)
		{
			LRESULT lRes = ::SendMessage(h, WM_HUBBLE_GETNODE, 0, 0);
			if (lRes)
			{
				CGrid* pRetNode = (CGrid*)lRes;
				if (pRetNode && pRetNode->m_nViewType == Grid)
				{
					pRetNode->m_pGridShareData->m_pGalaxy->HostPosChanged();
				}
			}
		}
	}
	if (nOldPage != wParam)
	{
		m_pGrid->Fire_TabChange(wParam, lParam);
		m_pGrid->m_pGridShareData->m_pGalaxyCluster->Fire_TabChange(m_pGrid, wParam, lParam);
		if (pGalaxy->m_pWebPageWnd)
		{
			::SendMessage(::GetParent(pGalaxy->m_pWebPageWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
		}
	}

	LRESULT lRes = CWnd::DefWindowProc(WM_TABCHANGE, wParam, lParam);
	return lRes;
}

LRESULT CGridHelper::OnHubbleData(WPARAM wParam, LPARAM lParam)
{
	return CWnd::DefWindowProc(WM_HUBBLE_DATA, wParam, lParam);
}

LRESULT CGridHelper::OnHubbleMsg(WPARAM wParam, LPARAM lParam)
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
		case 19820911:
			return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
			break;
		case 20190602:
		{
			CWinForm* pHubbleWinFormWnd = (CWinForm*)::SendMessage(m_hWnd, WM_HUBBLE_DATA, 0, 20190214);
			if (pHubbleWinFormWnd)
				return (LRESULT)pHubbleWinFormWnd->m_pChildFormsInfo;
			return 0;
		}
		case 20200128:
		{
			if (m_pGrid && m_pGrid->m_pWebBrowser)
			{
				::SetWindowPos(m_pGrid->m_pWebBrowser->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW);
			}
		}
		break;
		case 20200601:
		{
			for (auto it : m_pGrid->m_vChildNodes)
			{
				//if (it->m_nViewType == Grid)
				//{
				//	CGridWnd* pWnd = (CGridWnd*)it->m_pHostWnd;
				//	pWnd->RecalcLayout();
				//}
				::PostMessage(it->m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20200601);//for webruntimeVS Dockabe ToolWindow
			}
			return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
		}
		break;
		default:
		{
			if (lParam == 20022017)
			{
				break;
			}
			CString strObjTypeID = (LPCTSTR)lParam;
			if (strObjTypeID.Find(_T(",")) != -1 && g_pHubble->m_pCLRProxy)
			{
				m_pGrid->m_pDisp = g_pHubble->m_pCLRProxy->CreateObject(strObjTypeID.AllocSysString(), m_hWnd, m_pGrid);

				if (m_pGrid->m_pDisp)
				{
					m_pGrid->m_pGridShareData->m_mapLayoutNodes[m_pGrid->m_strName] = m_pGrid;
					m_pGrid->m_nViewType = CLRCtrl;
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
					m_Wnd.AttachControl(m_pGrid->m_pDisp, &pUnk);
					CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pGrid->m_pDisp);
					if (pIOleInPlaceActiveObject)
						m_pOleInPlaceActiveObject = pIOleInPlaceActiveObject.Detach();
					m_Wnd.Detach();
					if (m_mapDockCtrl.size())
					{
						HWND hPage = m_pGrid->m_pGridShareData->m_pGalaxyCluster->m_hWnd;
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
				CString strName = strObjTypeID;
				int _nPos = strName.Find(_T("."));
				if (_nPos != -1)
					strName = strName.Mid(_nPos + 1);
				_nPos = strName.ReverseFind('.');
				if (_nPos != -1)
					strName = strName.Left(_nPos);
				CGrid* pGrid = m_pGrid->m_pRootObj;
				CGrid* pParent = m_pGrid->m_pParentObj;
				if (pParent)
				{
					strName = pParent->m_strName + _T("_") + strName;
				}
				auto it = pGrid->m_pGridShareData->m_mapAxNodes.find(strName);
				if (it != pGrid->m_pGridShareData->m_mapAxNodes.end())
				{
					BOOL bGetNew = false;
					int nIndex = 0;
					while (bGetNew == false)
					{
						CString strNewName = _T("");
						strNewName.Format(_T("%s%d"), strName, nIndex);
						it = pGrid->m_pGridShareData->m_mapAxNodes.find(strNewName);
						if (it == pGrid->m_pGridShareData->m_mapAxNodes.end())
						{
							pGrid->m_pGridShareData->m_mapAxNodes[strNewName] = m_pGrid;
							strName = strNewName;
							bGetNew = true;
							break;
						}
						nIndex++;
					}
				}
				else
				{
					pGrid->m_pGridShareData->m_mapAxNodes[strName] = m_pGrid;
				}
				m_pGrid->put_Attribute(CComBSTR(L"id"), strName.AllocSysString());
				m_pGrid->m_strName = strName;
				BOOL bWebCtrl = false;
				CString strURL = _T("");
				strObjTypeID.MakeLower();
				auto nPos = strObjTypeID.Find(_T("http:"));
				if (nPos == -1)
					nPos = strObjTypeID.Find(_T("https:"));
				if (m_pGrid->m_pGridShareData->m_pGalaxy)
				{
					CComBSTR bstr;
					m_pGrid->get_Attribute(CComBSTR("InitInfo"), &bstr);
					CString str = _T("");
					str += bstr;
					if (str != _T("") && m_pGrid->m_pGridShareData->m_pGalaxyCluster)
					{
						LRESULT hr = ::SendMessage(m_pGrid->m_pGridShareData->m_pGalaxyCluster->m_hWnd, WM_GETNODEINFO, (WPARAM)OLE2T(bstr), (LPARAM)::GetParent(m_hWnd));
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
							strTemp = g_pHubble->m_strAppPath + strTemp;
							if (::PathFileExists(strTemp) == false)
							{
								strTemp = g_pHubble->m_strProgramFilePath + _T("Tangram\\") + strTemp;
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

				if (m_pGrid->m_pDisp == NULL)
				{
					CComPtr<IDispatch> pDisp;
					HRESULT hr = pDisp.CoCreateInstance(CComBSTR(strObjTypeID));
					if (hr == S_OK)
					{
						m_pGrid->m_pDisp = pDisp.Detach();
					}
				}
				if (m_pGrid->m_pDisp)
				{
					m_pGrid->m_pGridShareData->m_mapLayoutNodes[m_pGrid->m_strName] = m_pGrid;
					m_pGrid->m_nViewType = ActiveX;
					CAxWindow m_Wnd;
					m_Wnd.Attach(m_hWnd);
					CComPtr<IUnknown> pUnk;
					m_Wnd.AttachControl(m_pGrid->m_pDisp, &pUnk);
					CComQIPtr<IWebBrowser2> pWebDisp(m_pGrid->m_pDisp);
					if (pWebDisp)
					{
						bWebCtrl = true;
						m_pGrid->m_strURL = strURL;
						if (m_pGrid->m_strURL == _T(""))
							m_pGrid->m_strURL = strObjTypeID;

						CComPtr<IAxWinAmbientDispatch> spHost;
						LRESULT hr = m_Wnd.QueryHost(&spHost);
						if (SUCCEEDED(hr))
						{
							CComBSTR bstr;
							m_pGrid->get_Attribute(CComBSTR("scrollbar"), &bstr);
							CString str = OLE2T(bstr);
							if (str == _T("1"))
								spHost->put_DocHostFlags(DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE | DOCHOSTUIFLAG_THEME);//DOCHOSTUIFLAG_DIALOG|
							else
								spHost->put_DocHostFlags(/*DOCHOSTUIFLAG_DIALOG|*/DOCHOSTUIFLAG_SCROLL_NO | DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE | DOCHOSTUIFLAG_THEME);

							if (m_pGrid->m_strURL != _T(""))
							{
								pWebDisp->Navigate2(&CComVariant(m_pGrid->m_strURL), &CComVariant(navNoReadFromCache | navNoWriteToCache), NULL, NULL, NULL);
								m_pGrid->m_bWebInit = true;
							}
						}
					}
					((CGridHelper*)m_pGrid->m_pHostWnd)->m_pGrid = m_pGrid;
					HWND hPage = m_pGrid->m_pGridShareData->m_pGalaxyCluster->m_hWnd;
					::SendMessage(hPage, WM_COSMOSMSG, (WPARAM)((CGridHelper*)m_pGrid->m_pHostWnd), 1963);
					CComQIPtr<IOleInPlaceActiveObject> pIOleInPlaceActiveObject(m_pGrid->m_pDisp);
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
	if (lParam == 20191031 || lParam == 20200130|| lParam == 20200609|| lParam == 20200606)
		return CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
	if (lParam == 20200208)
		return 0;
	CString str = (LPCTSTR)lParam;
	IGrid* _pGrid = nullptr;
	CGrid* pOldNode = (CGrid*)m_pGrid;
	if (m_pGrid->m_hHostWnd == 0)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		m_pGrid->m_hHostWnd = ::CreateWindowEx(NULL, L"Hubble Grid Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_hWnd, NULL, AfxGetInstanceHandle(), NULL);
		m_pGrid->m_hChildHostWnd = ::CreateWindowEx(NULL, L"Hubble Grid Class", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, rc.right, rc.bottom, m_pGrid->m_hHostWnd, NULL, AfxGetInstanceHandle(), NULL);
		IGalaxy* pGalaxy = nullptr;
		m_pGrid->m_pGridShareData->m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)pOldNode->m_hChildHostWnd), CComBSTR(L"Design"), &pGalaxy);
		((CGalaxy*)pGalaxy)->m_pDoc = m_pGrid->m_pGridShareData->m_pGalaxy->m_pDoc;
		IGrid* pGrid = nullptr;
		pGalaxy->Observe(CComBSTR(L""), str.AllocSysString(), &pGrid);
		m_bEraseBkgnd = false;

		((CGrid*)pGrid)->m_pRootObj = m_pGrid->m_pRootObj;
		((CGrid*)pGrid)->m_pParentObj = m_pGrid->m_pParentObj;
		((CGrid*)pGrid)->m_pGridShareData->m_pOfficeObj = m_pGrid->m_pGridShareData->m_pOfficeObj;
		m_pGrid->m_pGridShareData->m_mapLayoutNodes[((CGrid*)pGrid)->m_strName] = (CGrid*)pGrid;
		((CGalaxy*)pGalaxy)->m_bDesignerState = m_pGrid->m_pGridShareData->m_pGalaxy->m_bDesignerState;
		CString strXml = ((CGrid*)pGrid)->m_pHostParse->xml();
		CTangramXmlParse* pNew = ((CGrid*)pGrid)->m_pHostParse;
		CTangramXmlParse* pOld = pOldNode->m_pHostParse;
		CTangramXmlParse* pParent = m_pGrid->m_pHostParse->m_pParentParse;
		CTangramXmlParse* pRet = nullptr;
		if (pParent)
		{
			pRet = pParent->ReplaceNode(pOld, pNew, _T(""));
			CString str = pRet->xml();
			int nCount = pRet->GetCount();
			((CGrid*)pGrid)->m_pHostParse = pRet;
			m_pGrid->m_pHostParse = pRet;

			CGrid* pChildNode = nullptr;
			for (auto it2 : ((CGrid*)pGrid)->m_vChildNodes)
			{
				pChildNode = it2;
				pChildNode->m_pRootObj = m_pGrid->m_pRootObj;
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
			m_pGrid->m_vChildNodes.push_back(((CGrid*)pGrid));
		}

		strXml = m_pGrid->m_pGridShareData->m_pHubbleParse->xml();
		g_pHubble->m_pDesignGrid = m_pGrid;
		g_pHubble->m_pHostDesignUINode = m_pGrid->m_pRootObj;
		if (g_pHubble->m_pHostDesignUINode)
		{
			CTangramHtmlTreeWnd* pTreeCtrl = g_pHubble->m_pDocDOMTree;
			if (pTreeCtrl)
			{
				pTreeCtrl->DeleteItem(g_pHubble->m_pDocDOMTree->m_hFirstRoot);

				if (pTreeCtrl->m_pHostXmlParse)
				{
					delete pTreeCtrl->m_pHostXmlParse;
				}
				pTreeCtrl->m_pHostXmlParse = new CTangramXmlParse();
				pTreeCtrl->m_pHostXmlParse->LoadXml(strXml);
				pTreeCtrl->m_hFirstRoot = pTreeCtrl->LoadXmlFromXmlParse(pTreeCtrl->m_pHostXmlParse);
				pTreeCtrl->ExpandAll();
			}
		}
		auto it = m_pGrid->m_pGridShareData->m_pGalaxy->m_pGalaxyCluster->m_mapGalaxy.find(pOldNode->m_hChildHostWnd);
		if (it != m_pGrid->m_pGridShareData->m_pGalaxy->m_pGalaxyCluster->m_mapGalaxy.end())
			m_pGrid->m_pGridShareData->m_pGalaxy->m_pGalaxyCluster->m_mapGalaxy.erase(it);
	}
	return -1;
}

LRESULT CGridHelper::OnActiveTangramObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrid->m_nViewType == CLRCtrl)
		::SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_pGrid->m_pGridShareData->m_pGalaxy->HostPosChanged();
	::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return CWnd::DefWindowProc(WM_TGM_SETACTIVEPAGE, wParam, lParam);
}

LRESULT CGridHelper::OnGetHubbleObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pGrid)
	{
		HWND hBrowser = (HWND)lParam;
		if (::IsWindow(hBrowser))
		{
			auto it = g_pHubble->m_mapBrowserWnd.find(hBrowser);
			if (it != g_pHubble->m_mapBrowserWnd.end())
			{
				m_pGrid->m_pWebBrowser = (CBrowser*)it->second;
			}
		}
		return (LRESULT)m_pGrid;
	}
	return CWnd::DefWindowProc(WM_HUBBLE_GETNODE, wParam, lParam);;
}

LRESULT CGridHelper::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
			if (::IsWindow(hWnd) && m_pGrid)
			{
				::GetClassName(hWnd, g_pHubble->m_szBuffer, MAX_PATH);
				m_pGrid->Fire_ControlNotify(m_pGrid, HIWORD(wParam), LOWORD(wParam), lParam, CComBSTR(g_pHubble->m_szBuffer));
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
			if (wParam && m_pGrid->m_strURL != _T(""))
			{
				CComQIPtr<IWebBrowser2> pWebCtrl(m_pGrid->m_pDisp);
				if (pWebCtrl)
				{
					if (m_pGrid->m_bWebInit == false)
					{
						pWebCtrl->Navigate2(&CComVariant(m_pGrid->m_strURL), &CComVariant(navNoReadFromCache | navNoWriteToCache), NULL, NULL, NULL);
						m_pGrid->m_bWebInit = true;
					}
				}
			}
			break;
		}
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

CBKWnd::CBKWnd(void)
{
	m_pGalaxy = nullptr;
	m_hChild = nullptr;
	m_pGrid = nullptr;
	m_strURL = _T("");
}

void CBKWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl<CBKWnd, CWindow>::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CBKWnd::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pGrid)
	{
		g_pHubble->m_pActiveGrid = m_pGrid;
		g_pHubble->m_bWinFormActived = false;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CBKWnd::OnMdiClientCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_hChild)
	{
		CGalaxyCluster* pGalaxyCluster = m_pGrid->m_pGridShareData->m_pGalaxyCluster;
		IGalaxy* pGalaxy = nullptr;
		pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)m_hChild), CComBSTR(L"ClientFrame"), &pGalaxy);
		CString strXml = _T("");
		strXml.Format(_T("<mdiclient><layout><grid name=\"mdiclient\" gridtype=\"activex\" objid=\"%s\" /></layout></mdiclient>"), m_strURL);
		IGrid* pGrid = nullptr;
		pGalaxy->Observe(CComBSTR(L"default"), strXml.AllocSysString(), &pGrid);
		m_pGalaxy = (CGalaxy*)pGalaxy;
		g_pHubble->m_mapBKFrame[m_hChild] = m_pGalaxy;
	}
	return 1;
}

LRESULT CBKWnd::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
	if (lpwndpos->cx == 0 && lpwndpos->cy == 0)
	{
		RECT rc;
		::GetClientRect(::GetParent(m_hWnd), &rc);
		lpwndpos->cx = rc.right;
		lpwndpos->cy = rc.bottom;
	}
	lpwndpos->flags |= SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW;
	if (::IsWindow(m_hChild))
	{
		::SetWindowPos(m_hChild, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOREDRAW | SWP_NOACTIVATE);
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

void CGridHelper::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
	if (m_pGrid && m_pGrid->m_nViewType == CLRCtrl && m_pGrid->m_hHostWnd)
	{
		if (m_bNoMove)
		{
			lpwndpos->flags |= SWP_NOMOVE;
			CGridHelper* pGridWnd = (CGridHelper*)m_pGrid->m_pHostWnd;
			if (m_pGrid->m_pParentObj->m_nViewType == Grid)
			{
				CGridWnd* pWnd = (CGridWnd*)m_pGrid->m_pParentObj->m_pHostWnd;
				::PostMessage(pWnd->m_hWnd, WM_COSMOSMSG, 3, 1993);
			}
		}
	}
	if (m_pGrid && m_pGrid->m_hHostWnd)
	{
		if (m_pGrid->m_nViewType == CLRCtrl)
		{
			if (m_bNoMove)
			{
				m_bNoMove = false;
			}
			return;
		}
		else if (m_pGrid->m_nViewType == BlankView)
		{
			::SetWindowPos(m_pGrid->m_hHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
			if (m_pGrid->m_hChildHostWnd)
				::SetWindowPos(m_pGrid->m_hChildHostWnd, HWND_BOTTOM, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		}
	}

	if (m_pGrid->m_pWebBrowser)
	{
		::SetWindowPos(m_pGrid->m_pWebBrowser->m_hWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE);
	}
	else {
		if (m_hFormWnd)
			::SetWindowPos(m_hFormWnd, HWND_TOP, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE | SWP_NOREDRAW);
		else if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
		{
			m_pGrid->m_pGridShareData->m_pGalaxy->HostPosChanged();
		}
	}
	if (m_pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) && (m_bCreateExternal == false && m_pGrid->m_pDisp == NULL) && m_pGrid != m_pGrid->m_pRootObj)
	{
		return;
	}
	if (m_pGrid->m_nViewType == TangramTreeView)
	{
		lpwndpos->flags &= ~SWP_NOREDRAW;
		::SetWindowPos(m_pXHtmlTree->m_hWnd, NULL, 0, 0, lpwndpos->cx, lpwndpos->cy, SWP_NOACTIVATE);
	}

	if (m_bCreateExternal)
	{
		Invalidate(true);
	}
}

void CGridHelper::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_pGrid->m_nViewType == TangramTreeView && m_pGrid->m_pParentObj == NULL)
	{
		::SetWindowPos(m_pXHtmlTree->m_hWnd, NULL, 0, 0, cx, cy,/*SWP_NOREDRAW|*/SWP_NOACTIVATE);
	}
}


void CGridHelper::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	//if (m_pGrid->m_nViewType == TabGrid)
	//{
	//	IGrid* pGrid = nullptr;
	//	m_pGrid->GetNode(0, m_pGrid->m_nActivePage, &pGrid);
	//	CGrid* _pGrid = (CGrid*)pGrid;
	//	if (_pGrid&&_pGrid->m_nViewType == Grid)
	//	{
	//		((CSplitterWnd*)_pGrid->m_pHostWnd)->RecalcLayout();
	//	}
	//}
}
