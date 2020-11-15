/********************************************************************************
*					Open Universe - version 1.0.1.15							*
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

// Galaxy.cpp : implementation file of CGalaxy
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "Hubble.h"
#include "GridWnd.h"
#include "GridHelper.h"
#include "Grid.h"
#include "Galaxy.h"
#include "TangramHtmlTreeWnd.h"
#include "EclipsePlus\EclipseAddin.h"
#include "DocTemplateDlg.h"
#include "Wormhole.h"

CCommonCtrl::CCommonCtrl()
{

}

CCommonCtrl::~CCommonCtrl()
{

}

LRESULT CCommonCtrl::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	NMHDR* pNMHDR = (NMHDR*)lParam;
	//ASSERT(pNMHDR != NULL);
	//if (pNMHDR->code)
	//{
	//	switch (pNMHDR->code)
	//	{
	//		case NM_CLICK:
	//		{
	//			TRACE(_T("NM_CLICK\n"));
	//		}
	//		break;
	//	case NM_DBLCLK:
	//		{
	//			TRACE(_T("NM_DBLCLK\n"));
	//		}
	//		break;
	//	case TVN_SELCHANGED:
	//		{
	//			TRACE(_T("TVN_SELCHANGED \n"));
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//}
	TRACE(_T("======== Notify=========hwndFrom:%x,idFrom:%x,code:%x\n"), pNMHDR->hwndFrom, pNMHDR->idFrom, pNMHDR->code);
	return DefWindowProc(uMsg, wParam, lParam);
}

void CCommonCtrl::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CAFXHelperWnd::CAFXHelperWnd(void)
{
	m_hFrame = nullptr;
	m_hParent = nullptr;
}

CAFXHelperWnd::~CAFXHelperWnd(void)
{
}

LRESULT CAFXHelperWnd::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	if (m_hParent)
	{
		HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
		if (g_pHubble->m_pMDIMainWnd && hTop == g_pHubble->m_pMDIMainWnd->m_hWnd)
			::SetWindowPos(m_hParent, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE);
		else
		{
			auto it = g_pHubble->m_mapHubbleMDIChildWnd.find(hTop);
			if (it != g_pHubble->m_mapHubbleMDIChildWnd.end())
				::SetWindowPos(m_hParent, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE);
		}
	}
	return l;
}

LRESULT CAFXHelperWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT l = DefWindowProc(uMsg, wParam, lParam);
	if (wParam && ::IsWindow(m_hFrame))
	{
		CGalaxy* pGalaxy = (CGalaxy*)::SendMessage(m_hFrame, WM_HUBBLE_DATA, 0, 1992);
		if (pGalaxy)
		{
			CGrid* pGrid = pGalaxy->m_pWorkGrid;
			if (::IsWindowVisible(pGrid->m_pHostWnd->m_hWnd) == FALSE)
			{
				HWND hPWnd = ::GetParent(m_hWnd);
				::PostMessage(m_hWnd, WM_QUERYAPPPROXY, (WPARAM)m_hWnd, 19650601);
			}
		}
	}
	return l;
}

LRESULT CAFXHelperWnd::OnHubbleMg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
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
				::ShowWindow(pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd, SW_SHOW);
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

void CAFXHelperWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CUniverseMDIMain::CUniverseMDIMain(void)
{
	m_pGalaxyCluster = nullptr;
	m_hMDIClient = nullptr;
	m_pDocTemplate = new CComObject<CHubbleDocTemplate>;
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
	CHubbleDocTemplate* pTemplate = nullptr;
	if (strKey == _T(""))
	{
		strKey = _T("default");
		pTemplate = m_pDocTemplate;
	}
	if (pTemplate == nullptr)
	{
		auto it = m_mapHubbleDocTemplate.find(strKey);
		if (it != m_mapHubbleDocTemplate.end())
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
				IGrid* pGrid = nullptr;
				auto it3 = pTemplate->m_mapConnectedFrame.find(it.first);
				if (it3 == pTemplate->m_mapConnectedFrame.end())
				{
					it.second->m_pHubbleDocTemplate = pTemplate;
					it.second->Observe(CComBSTR(strKey), CComBSTR(""), &pGrid);
					pTemplate->m_mapMainPageNode[it.first] = (CGrid*)pGrid;
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
	if (g_pHubble->m_pActiveMDIChildWnd)
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
			CString strFile = _T("");
			if (g_pHubble->m_pHubbleDocTemplateInfo)
			{
				strFile = g_pHubble->m_strTemplatePath;
				g_pHubble->m_pHubbleDocTemplateInfo = nullptr;
			}
			if (strFile == _T(""))
				strFile = g_pHubble->GetDocTemplateXml(_T("Please select New Doc Template:"), g_pHubble->m_strAppDataPath + _T("Doctemplate\\"), _T(".appxml"));
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
				strXml = _T("<default><layout><grid name=\"Start\" =\"nucleus\"/></layout></default>");
			}
		}
		CUniverseMDIChild* pWnd = g_pHubble->m_pActiveMDIChildWnd;
		CHubbleDocTemplate* pHubbleDocTemplate = nullptr;
		auto it2 = m_mapHubbleDocTemplate.find(_strKey);
		if (it2 == m_mapHubbleDocTemplate.end())
		{
			pHubbleDocTemplate = new CComObject<CHubbleDocTemplate>;
			pWnd->m_pDocTemplate = pHubbleDocTemplate;
			pHubbleDocTemplate->m_strKey = _strKey;
			pHubbleDocTemplate->m_strClientKey = _strClientKey;
			if (g_pHubble->m_strTemplatePath.Find(g_pHubble->m_strAppCommonDocPath) == 0)
			{
				pHubbleDocTemplate->m_strDocTemplatePath = g_pHubble->m_strAppCommonDocPath + _T("CommonMFCAppTemplate\\MDI\\");
			}

			pHubbleDocTemplate->InitXmlData();
			m_mapHubbleDocTemplate[_strKey] = pHubbleDocTemplate;
		}
		else
			pHubbleDocTemplate = it2->second;
		pHubbleDocTemplate->m_mapHubbleMDIChildWnd[pWnd->m_hWnd] = pWnd;
		g_pHubble->m_pActiveTemplate = pWnd->m_pDocTemplate = pHubbleDocTemplate;
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
	if (g_pHubble->m_strDefaultTemplate == _T("") && g_pHubble->m_strDefaultTemplate2 == _T(""))
		return DefWindowProc(uMsg, wParam, lParam);

	switch (wParam)
	{
	case ID_FILE_NEW:
	{
		IGrid* pGrid = nullptr;
		CGalaxy* pGalaxy = (CGalaxy*)::SendMessage(m_hMDIClient, WM_HUBBLE_DATA, 0, 1992);
		if (pGalaxy && g_pHubble->m_strNewDocXml != _T(""))
		{
			pGalaxy->Observe(CComBSTR(L"newdocument"), g_pHubble->m_strNewDocXml.AllocSysString(), &pGrid);
			g_pHubble->m_bNewFile = TRUE;
			return 0;
		}
	}
	break;
	//case ID_FILE_OPEN:
	//{
	//	g_pHubble->m_pActiveMDIChildWnd = nullptr;
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//	LRESULT hr = ::SendMessage(g_pHubble->m_hHubbleWnd, WM_COSMOSMSG, (WPARAM)m_hWnd, 0);
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
	::GetWindowText(hPWnd, g_pHubble->m_szBuffer, MAX_PATH);
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
	if (g_pHubble->m_strStartupCLRObj != _T(""))
	{
		m_strTemplatePath = g_pHubble->m_strStartupCLRObj;
		g_pHubble->m_strStartupCLRObj = _T("");
	}
	m_hChild = nullptr;
	m_pDocTemplate = nullptr;
	g_pHubble->m_pActiveMDIChildWnd = this;
}

CUniverseMDIChild::~CUniverseMDIChild(void)
{
	if (g_pHubble->m_pActiveMDIChildWnd && g_pHubble->m_pActiveMDIChildWnd == this)
		g_pHubble->m_pActiveMDIChildWnd = nullptr;
}

LRESULT CUniverseMDIChild::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pHubble->m_nAppType == 1963)
	{
		for (auto it : g_pHubble->m_mapHubbleMDIChildWnd)
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
	if (g_pHubble->m_nAppType == 1992 && ::IsIconic(m_hWnd))
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

LRESULT CUniverseMDIChild::OnHubbleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (lParam == 19631222)
	{
		if (wParam == 0)
			return (LRESULT)this;
	}
	if (g_pHubble->m_nAppType == 1965 && g_pHubble->m_strTemplatePath != _T(""))
	{
		BOOL bNeedInit = false;
		CGalaxyCluster* pGalaxyCluster = nullptr;
		for (auto it : g_pHubble->m_mapHubbleCommonCtrl)
		{
			//clear common control data:
			if (::IsChild(m_hWnd, it.first))
				::SendMessage(it.first, WM_COSMOSMSG, 19820911, 19820911);
		}
		if (g_pHubble->m_mapWindowPage.size() > 0)
		{
			auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
			if (it != g_pHubble->m_mapWindowPage.end())
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
						if (pGalaxy->m_pWorkGrid)
						{
							::GetWindowRect(pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd, &rc);
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
							pGalaxy->m_pGalaxyCluster->m_mapGrid.erase(pGalaxy->m_pGalaxyCluster->m_mapGrid.begin(), pGalaxy->m_pGalaxyCluster->m_mapGrid.end());
							pGalaxy->m_pGalaxyCluster = nullptr;
							::DestroyWindow(pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd);
							::SetWindowPos(hwnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
						}
						pGalaxy->m_pGalaxyCluster = nullptr;
						if (deleteframe)
							delete pGalaxy;
					}
					auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
					if (it == g_pHubble->m_mapWindowPage.end())
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
			g_pHubble->CreateGalaxyCluster((__int64)m_hWnd, &_pGalaxyCluster);
			pGalaxyCluster = (CGalaxyCluster*)_pGalaxyCluster;
		}
		IGrid* pGrid = nullptr;
		m_strXml = g_pHubble->m_strTemplatePath;

		if (m_strXml != _T(""))
		{
			CTangramXmlParse m_Parse;
			bool bLoad = m_Parse.LoadXml(m_strXml);
			if (bLoad == false)
			{
				bLoad = m_Parse.LoadFile(m_strXml);
			}
			else
				g_pHubble->m_strTemplatePath = g_pHubble->m_strStartupCLRObj;
			((CGalaxyCluster*)pGalaxyCluster)->UpdateMapKey(m_strXml);
			if (bLoad)
			{
				IGalaxy* pGalaxy = nullptr;
				CString strXml = _T("");
				CTangramXmlParse* pParse = m_Parse.GetChild(_T("client"));
				if (pParse)
					strXml = pParse->xml();
				bool bAddCfg = m_bNoDocView;
				if (g_pHubble->m_strDefaultTemplate2 != _T(""))
					bAddCfg = false;
				pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR(_T("default")), CComBSTR(_T("")), bAddCfg, &pGrid);
				if (pGrid)
				{
					CGrid* _pGrid = (CGrid*)pGrid;
					m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
				}
				for (auto it : m_mapDesignableWnd)
				{
					CString strXml = _T("");
					CTangramXmlParse* pParse = m_Parse.GetChild(it.second);
					if (pParse)
						strXml = pParse->xml();
					IGrid* pGrid = nullptr;
					pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(_T("")), bAddCfg, &pGrid);
				}
			}
		}
		if (g_pHubble->m_bNewFile)
		{
			for (auto it : g_pHubble->m_mapHubbleCommonCtrl)
			{
				//fill common control data:
				if (::IsChild(m_hWnd, it.first))
					::PostMessage(it.first, WM_COSMOSMSG, 0, 19820911);
			}
		}
		g_pHubble->m_strTemplatePath = _T("");

		return DefWindowProc(uMsg, wParam, lParam);
	}
	if (lParam == 19922017)
	{
		BOOL bNeedInit = false;
		IGalaxyCluster* pGalaxyCluster = nullptr;
		IGrid* pGrid = nullptr;
		g_pHubble->CreateGalaxyCluster((__int64)m_hWnd, &pGalaxyCluster);
		if (pGalaxyCluster)
		{
			((CGalaxyCluster*)pGalaxyCluster)->m_bDoc = true;
			if ((::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD))
			{
				if (m_strXml != _T(""))
				{
					if (::PathFileExists(m_strXml))
						g_pHubble->m_strTemplatePath = m_strXml;
					((CGalaxyCluster*)pGalaxyCluster)->UpdateMapKey(m_strXml);
				}
				pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR("default"), CComBSTR(L""), false, &pGrid);
				if (pGrid)
				{
					CGrid* _pGrid = (CGrid*)pGrid;
					m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
				}

				if (g_pHubble->m_pMDIMainWnd && g_pHubble->m_pMDIMainWnd->m_pGalaxyCluster && m_pDocTemplate && m_pDocTemplate->m_strKey != _T(""))
				{
					g_pHubble->m_pMDIMainWnd->NavigateKey(m_pDocTemplate->m_strKey);
				}
			}
			else
			{
				if (g_pHubble->m_nAppType == 1965 && m_strXml != _T(""))
				{
					CGalaxyCluster* pGalaxyCluster = nullptr;
					TRACE(_T("%s\n"), g_pHubble->m_strTemplatePath);
					HWND hPage = (HWND)m_hWnd;
					auto it = g_pHubble->m_mapWindowPage.find(hPage);
					if (it != g_pHubble->m_mapWindowPage.end())
					{
						pGalaxyCluster = (CGalaxyCluster*)it->second;
						for (auto it : g_pHubble->m_mapHubbleCommonCtrl)
						{
							//clear common control data:
							if (::IsChild(m_hWnd, it.first))
								::SendMessage(it.first, WM_COSMOSMSG, 19820911, 19820911);
						}
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
								if (pGalaxy->m_pWorkGrid)
								{
									::GetWindowRect(pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd, &rc);
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
									pGalaxy->m_pGalaxyCluster->m_mapGrid.erase(pGalaxy->m_pGalaxyCluster->m_mapGrid.begin(), pGalaxy->m_pGalaxyCluster->m_mapGrid.end());
									pGalaxy->m_pGalaxyCluster = nullptr;
									::DestroyWindow(pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd);
									::SetWindowPos(hwnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
								}
								pGalaxy->m_pGalaxyCluster = nullptr;
								if (deleteframe)
									delete pGalaxy;
							}
							auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
							if (it == g_pHubble->m_mapWindowPage.end())
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
							g_pHubble->m_strTemplatePath = m_strXml;
						((CGalaxyCluster*)pGalaxyCluster)->UpdateMapKey(m_strXml);
					}
					IGrid* pGrid = nullptr;
					if (m_strXml != _T(""))
					{
						IGalaxy* pGalaxy = nullptr;
						pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR(_T("default")), CComBSTR(L""), false, &pGrid);
						if (pGrid)
						{
							CGrid* _pGrid = (CGrid*)pGrid;
							m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
						}
						for (auto it : m_mapDesignableWnd)
						{
							IGrid* pGrid = nullptr;
							pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(L""), false, &pGrid);
						}
					}
					for (auto it : g_pHubble->m_mapHubbleCommonCtrl)
					{
						//fill common control data:
						if (::IsChild(m_hWnd, it.first))
							::PostMessage(it.first, WM_COSMOSMSG, 0, 19820911);
					}
					return DefWindowProc(uMsg, wParam, lParam);
				}
				if (m_strXml != _T(""))
				{
					if (::PathFileExists(m_strXml))
						g_pHubble->m_strTemplatePath = m_strXml;
					((CGalaxyCluster*)pGalaxyCluster)->UpdateMapKey(m_strXml);
				}
				CString strTemplatePath = g_pHubble->m_strTemplatePath;
				if (m_strXml == _T("") && strTemplatePath != _T(""))
				{
					m_strXml = strTemplatePath;
					g_pHubble->m_strTemplatePath = _T("");
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
										CString strData = g_pHubble->EncodeFileToBase64(strPath);
										_pGalaxyCluster->m_mapXtml[strName + _T("_image")] = strData;
									}
								}
							}
						}
						IGalaxy* pGalaxy = nullptr;
						pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR(_T("default")), CComBSTR(strXml), false, &pGrid);
						if (pGrid)
						{
							CGrid* _pGrid = (CGrid*)pGrid;
							m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
						}
						for (auto it : m_mapDesignableWnd)
						{
							CString strXml = _T("");
							CTangramXmlParse* pParse = m_Parse.GetChild(it.second);
							if (pParse)
								strXml = m_Parse.GetChild(it.second)->xml();
							IGrid* pGrid = nullptr;
							pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(strXml), false, &pGrid);
						}
					}
				}
				else
				{
					if (g_pHubble->m_nAppType == 1963 && m_bNoDocView)
					{
						if (g_pHubble->m_strDefaultTemplate2 != _T(""))//Fist Frame for APP_TYPE_MTLD Application
						{
							pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"defaultframe"), CComBSTR(_T("defaultframe")), CComBSTR(""), true, &pGrid);
							if (pGrid)
							{
								CGrid* _pGrid = (CGrid*)pGrid;
								m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
							}
						}
						else
						{
							CString strPath = g_pHubble->GetDocTemplateXml(_T("Please select Document Template:"), g_pHubble->m_strAppDataPath + _T("Doctemplate\\"), _T(".AppXml"));
							if (::PathFileExists(strPath))
							{
								CTangramXmlParse m_Parse;
								if (m_Parse.LoadFile(strPath))
								{
									pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(L"default"), CComBSTR(_T("default")), CComBSTR(m_Parse.xml()), false, &pGrid);
									if (pGrid)
									{
										CGrid* _pGrid = (CGrid*)pGrid;
										m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
									}
								}
							}
						}
						return DefWindowProc(uMsg, wParam, lParam);
					}
					bool bAddCfg = m_bNoDocView;
					if (g_pHubble->m_strDefaultTemplate2 != _T(""))
						bAddCfg = false;
					CString strKey = _T("default");
					if (g_pHubble->m_nAppType == 1965 && m_bNoDocView)// First View for SDI Application 
					{
						strKey = _T("defaultclient");
						pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(strKey), CComBSTR(strKey), CComBSTR(L""), true, &pGrid);
						if (pGrid)
						{
							CGrid* _pGrid = (CGrid*)pGrid;
							m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
						}
						for (auto it : m_mapDesignableWnd)
						{
							IGrid* pGrid = nullptr;
							pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(L""), bAddCfg, &pGrid);
						}
					}
					else
					{
						OnCreateDoc(_T(""));
					}
					//	pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(strKey), CComBSTR(strKey), CComBSTR(L""), bAddCfg, &pGrid);
					//if (pGrid)
					//{
					//	CGrid* _pGrid = (CGrid*)pGrid;
					//	m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
					//}
					//for (auto it : m_mapDesignableWnd)
					//{
					//	IGrid* pGrid = nullptr;
					//	pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(L""), bAddCfg, &pGrid);
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
			g_pHubble->m_pActiveTemplate = pWnd->m_pDocTemplate;
		}
		g_pHubble->m_pMDIMainWnd->NavigateKey(strKey);
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

void CUniverseMDIChild::OnCreateDoc(CString strDocData)
{
	m_strXml = strDocData;
	if (m_strXml == _T(""))
	{
		CString strPath = g_pHubble->m_strAppDataPath + _T("DocTemplate\\Normal\\default.xml");
		bool bDefaultExists = ::PathFileExists(strPath);
		if (bDefaultExists == false)
		{
			CString strDir = g_pHubble->m_pMDIMainWnd ? _T("MDI") : _T("SDI");
			strPath = g_pHubble->m_strAppCommonDocPath + _T("CommonMFCAppTemplate\\") + strDir + _T("\\DocTemplate\\default.xml");
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
			g_pHubble->m_strTemplatePath = strPath;
		}
		else
		{
			bool bAddCfg = m_bNoDocView;
			if (g_pHubble->m_strDefaultTemplate2 != _T(""))
				bAddCfg = false;
			CString strKey = _T("default");
			IGalaxyCluster* pGalaxyCluster = nullptr;
			IGrid* pGrid = nullptr;
			g_pHubble->CreateGalaxyCluster((__int64)m_hWnd, &pGalaxyCluster);
			pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)m_hChild, CComBSTR(strKey), CComBSTR(strKey), CComBSTR(L""), bAddCfg, &pGrid);
			if (pGrid)
			{
				CGrid* _pGrid = (CGrid*)pGrid;
				m_pGalaxy = _pGrid->m_pGridShareData->m_pGalaxy;
			}
			for (auto it : m_mapDesignableWnd)
			{
				IGrid* pGrid = nullptr;
				pGalaxyCluster->CreateGalaxyWithDefaultNode((__int64)it.first, CComBSTR(it.second), CComBSTR(_T("default")), CComBSTR(L""), bAddCfg, &pGrid);
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
		g_pHubble->m_pActiveMDIChildWnd = this;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CUniverseMDIChild::OnControlBarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HWND hWnd = (HWND)lParam;
	HWND hPWnd = (HWND)wParam;
	::GetWindowText(hPWnd, g_pHubble->m_szBuffer, MAX_PATH);
	CString strText = CString(g_pHubble->m_szBuffer);
	strText.Replace(_T(" "), _T("_"));
	LRESULT lRes = ::SendMessage(hWnd, WM_HUBBLE_GETNODE, 0, 0);
	if (lRes == 0)
	{
		m_mapDesignableWnd[hWnd] = strText;
		auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
		if (it != g_pHubble->m_mapWindowPage.end())
		{
			IGrid* pGrid = nullptr;
			bool bAddCfg = m_bNoDocView;
			if (g_pHubble->m_strDefaultTemplate2 != _T(""))
				bAddCfg = false;
			it->second->CreateGalaxyWithDefaultNode((__int64)hWnd, CComBSTR(strText), CComBSTR(_T("default")), CComBSTR(L""), bAddCfg, &pGrid);
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
	if (g_pHubble->m_strDefaultTemplate == _T("") && g_pHubble->m_strDefaultTemplate2 == _T(""))
		return DefWindowProc(uMsg, wParam, lParam);

	switch (wParam)
	{
	case ID_FILE_NEW:
	case ID_FILE_NEW_FRAME:
	{
		if (g_pHubble->m_hTemplateWnd == nullptr)
		{
			if (g_pHubble->m_pDesignerGalaxyCluster == nullptr)
			{
				auto it = g_pHubble->m_mapWindowPage.find(g_pHubble->m_hHostWnd);
				if (it != g_pHubble->m_mapWindowPage.end())
					g_pHubble->m_pDesignerGalaxyCluster = (CGalaxyCluster*)it->second;
				else
				{
					g_pHubble->m_pDesignerGalaxyCluster = new CComObject<CGalaxyCluster>();
					g_pHubble->m_pDesignerGalaxyCluster->m_hWnd = g_pHubble->m_hHostWnd;
					g_pHubble->m_mapWindowPage[g_pHubble->m_hHostWnd] = g_pHubble->m_pDesignerGalaxyCluster;
				}
			}
			g_pHubble->m_hTemplateWnd = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, g_pHubble->m_hHostWnd, 0, theApp.m_hInstance, NULL);
			g_pHubble->m_hTemplateChildWnd = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, g_pHubble->m_hTemplateWnd, 0, theApp.m_hInstance, NULL);
			IGalaxy* pGalaxy = nullptr;
			g_pHubble->m_pDesignerGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((__int64)g_pHubble->m_hTemplateChildWnd), CComBSTR(L"doctemplate"), &pGalaxy);
			g_pHubble->m_pDocTemplateFrame = (CGalaxy*)pGalaxy;
		}
		if (g_pHubble->m_pDocTemplateFrame)
		{
			if (m_hChild)
			{
				RECT rc;
				CUniverseMDIChild* pWnd = (CUniverseMDIChild*)::SendMessage(::GetParent(g_pHubble->m_hTemplateWnd), WM_COSMOSMSG, 0, 19631222);
				if (pWnd)
				{
					::GetWindowRect(g_pHubble->m_pDocTemplateFrame->m_pWorkGrid->m_pHostWnd->m_hWnd, &rc);
					pWnd->ScreenToClient(&rc);
					::ShowWindow(pWnd->m_pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd, SW_SHOW);
					::SetWindowLongPtr(pWnd->m_hChild, GWLP_ID, AFX_IDW_PANE_FIRST);
					::SetWindowPos(pWnd->m_hChild, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED);
				}
				::SetParent(g_pHubble->m_hTemplateWnd, m_hWnd);
				::GetWindowRect(m_pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd, &rc);
				ScreenToClient(&rc);
				m_pGalaxy->m_pWorkGrid->m_pHostWnd->ShowWindow(SW_HIDE);
				::SetWindowPos(m_hChild, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED);
				::SetWindowLongPtr(m_hChild, GWLP_ID, 19631222);
				::SetWindowLongPtr(g_pHubble->m_hTemplateWnd, GWLP_ID, AFX_IDW_PANE_FIRST);
				::ShowWindow(g_pHubble->m_hTemplateWnd, SW_SHOW);
				::SetWindowPos(g_pHubble->m_hTemplateWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED);
			}
			IGrid* pGrid = nullptr;
			g_pHubble->m_pDocTemplateFrame->Observe(CComBSTR(L"newdocument"), g_pHubble->m_strNewDocXml.AllocSysString(), &pGrid);
			g_pHubble->m_bNewFile = TRUE;
			return 0;
		}
	}
	break;
	//case ID_FILE_OPEN:
	//{
	//	LRESULT hr = ::SendMessage(g_pHubble->m_hHubbleWnd, WM_COSMOSMSG, (WPARAM)m_hWnd, 0);
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
		auto it = m_pDocTemplate->m_mapHubbleMDIChildWnd.find(m_hWnd);
		if (it != m_pDocTemplate->m_mapHubbleMDIChildWnd.end())
		{
			m_pDocTemplate->m_mapHubbleMDIChildWnd.erase(it);
			if (m_pDocTemplate->m_mapHubbleMDIChildWnd.size() == 0)
			{
				auto it = g_pHubble->m_pMDIMainWnd->m_mapHubbleDocTemplate.find(m_pDocTemplate->m_strKey);
				if (it != g_pHubble->m_pMDIMainWnd->m_mapHubbleDocTemplate.end())
				{
					g_pHubble->m_pMDIMainWnd->m_mapHubbleDocTemplate.erase(it);
					m_pDocTemplate->SaveXmlData();
					if (g_pHubble->m_pMDIMainWnd->m_mapHubbleDocTemplate.size() == 0)
					{
						g_pHubble->m_pActiveTemplate = nullptr;
					}
				}
			}
		}
	}
	else
	{
		if (::IsChild(m_hWnd, g_pHubble->m_hTemplateWnd))
		{
			::ShowWindow(g_pHubble->m_hTemplateWnd, SW_HIDE);
			::SetParent(g_pHubble->m_hTemplateWnd, g_pHubble->m_hHostWnd);
		}
		auto it = g_pHubble->m_mapHubbleMDIChildWnd.find(m_hWnd);
		if (it != g_pHubble->m_mapHubbleMDIChildWnd.end())
		{
			g_pHubble->m_mapHubbleMDIChildWnd.erase(it);
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
	if (g_pHubble->m_pCurMDIChildFormInfo)
	{
		m_pChildFormsInfo = g_pHubble->m_pCurMDIChildFormInfo;
		if (g_pHubble->m_pActiveGrid)
			g_pHubble->m_pActiveGrid->m_pChildFormsInfo = m_pChildFormsInfo;
		g_pHubble->m_pCurMDIChildFormInfo = nullptr;
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
	g_pHubble->m_pActiveWinFormWnd = this;
	if (g_pHubble->m_pCLRProxy)
	{
		g_pHubble->m_pCLRProxy->OnWinFormActivate(m_hWnd, LOWORD(wParam));
	}
	return lRes;
}

LRESULT CWinForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pHubble->m_mapBrowserWnd.size())
	{
		CSession* pSession = (CSession*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		if (pSession)
		{
			int nCount = 0;
			if (m_bMainForm)
			{
				nCount = g_pHubble->m_mapNeedQueryOnClose.size();
				if (nCount > 1)
				{
					pSession->InsertLong(_T("FormAppNeedClosed"), nCount);
				}
				for (auto it : g_pHubble->m_mapNeedQueryOnClose)
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
	switch (m_nState)
	{
	case 0:
	case 1:
	case 3:
	{
		if (!::PathFileExists(m_strPath))
		{
			int nPos = m_strPath.ReverseFind('\\');
			CString strPath = m_strPath.Left(nPos);
			::SHCreateDirectory(nullptr, strPath);
		}
		CGalaxyCluster* pGalaxyCluster = nullptr;
		auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
		if (it != g_pHubble->m_mapWindowPage.end())
		{
			pGalaxyCluster = (CGalaxyCluster*)it->second;
			CString strData = _T("<winform>");
			CString strIndex = _T("@");
			for (auto it2 : pGalaxyCluster->m_mapGalaxy)
			{
				CComBSTR bstrXml(L"");
				strIndex += it2.second->m_strGalaxyName;
				strIndex += _T("@");
				it2.second->get_GalaxyXML(&bstrXml);
				strData += OLE2T(bstrXml);
			}
			DWORD dw = ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
			if (dw & WS_EX_MDICHILD)
			{
				HWND h = ::GetParent(::GetParent(m_hWnd));
				if (h)
				{
					CWinForm* pParent = (CWinForm*)::SendMessage(h, WM_HUBBLE_DATA, 0, 20190214);
					if (pParent)
					{
						auto it = pParent->m_mapKey.find(m_strKey);
						if (it != pParent->m_mapKey.end())
						{
							strData += it->second;
						}
					}
				}
			}
			strData += _T("</winform>");
			CTangramXmlParse xml;
			if (xml.LoadXml(strData))
				xml.SaveFile(m_strPath);
			// TODO Refresh ListCtrl
		}
	}
	break;
	}
	if (g_pHubble->m_pActiveWinFormWnd == this)
		g_pHubble->m_pActiveWinFormWnd = nullptr;
	auto it = g_pHubble->m_mapNeedQueryOnClose.find(m_hWnd);
	if (it != g_pHubble->m_mapNeedQueryOnClose.end())
		g_pHubble->m_mapNeedQueryOnClose.erase(it);
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
		if (m_strKey == _T(""))
		{
			CString strPath = m_strPath;
			strPath.MakeLower();
			int nPos = strPath.ReverseFind('.');
			strPath = strPath.Left(nPos);
			CString strForms = g_pHubble->m_strAppFormsTemplatePath;
			strForms.MakeLower();
			strPath.Replace(strForms, _T(""));
			nPos = strPath.Find(_T("\\"));
			strPath = strPath.Mid(nPos + 1);
			nPos = strPath.Find(_T("\\"));
			strPath = strPath.Mid(nPos + 1);
			strPath.Replace(_T("\\"), _T("_"));
			strPath.Replace(_T(" "), _T("_"));
			m_strKey = strPath;
			DWORD dw = ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
			if (dw & WS_EX_MDICHILD)
			{
				HWND h = ::GetParent(::GetParent(m_hWnd));
				if (h)
				{
					CWinForm* pParent = (CWinForm*)::SendMessage(h, WM_HUBBLE_DATA, 0, 20190214);
					if (pParent)
					{
						auto it = pParent->m_mapKey.find(m_strKey);
						if (it == pParent->m_mapKey.end())
						{
							CTangramXmlParse m_Parse;
							if (m_Parse.LoadFile(m_strPath))
							{
								CTangramXmlParse* pChild = m_Parse.GetChild(m_strKey);
								if (pChild)
									pParent->m_mapKey[m_strKey] = pChild->xml();
							}
						}
					}
				}
			}
		}
		else
		{
			if (m_pOwnerHtmlWnd && m_pOwnerHtmlWnd->m_pGalaxy)
			{
				auto it = m_pOwnerHtmlWnd->m_pGalaxy->m_mapGrid.find(m_strKey);
				if (it != m_pOwnerHtmlWnd->m_pGalaxy->m_mapGrid.end())
				{
					IGrid* pGrid = nullptr;
					m_pOwnerHtmlWnd->m_pGalaxy->Observe(CComBSTR(m_strKey), CComBSTR(""), &pGrid);
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
						IGrid* pGrid = nullptr;
						m_pOwnerHtmlWnd->Observe(CComBSTR(m_strKey), CComBSTR(pChild->xml()), &pGrid);
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
	case 5:
	{
		BindWebObj* pObj = (BindWebObj*)wParam;
		if ((::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_MDICHILD))
		{
			auto it = m_mapBindWebObj.find(pObj->m_strBindObjName);
			if (it != m_mapBindWebObj.end())
			{
				m_mapBindWebObj.erase(it);
			}
			m_mapBindWebObj[pObj->m_strBindObjName] = pObj;
		}
		else if (m_bMdiForm)
		{
			auto it = m_mapBindWebObj.find(pObj->m_strBindObjName);
			if (it != m_mapBindWebObj.end())
			{
				m_mapBindWebObj.erase(it);
			}
			m_mapBindWebObj[pObj->m_strBindObjName] = pObj;
		}

		if (m_pOwnerHtmlWnd)
		{
			auto it = m_pOwnerHtmlWnd->m_mapBindWebObj.find(pObj->m_strBindObjName);
			if (it != m_pOwnerHtmlWnd->m_mapBindWebObj.end())
			{
				delete it->second;
				m_pOwnerHtmlWnd->m_mapBindWebObj.erase(it);
			}
			m_pOwnerHtmlWnd->m_mapBindWebObj[pObj->m_strBindObjName] = pObj;
		}
		else
		{
			IGrid* pGrid = nullptr;
			g_pHubble->GetGridFromHandle((__int64)m_hWnd, &pGrid);
			if (pGrid)
			{
				CGrid* pWndGrid = (CGrid*)pGrid;
				HWND hWnd = pWndGrid->m_pGridShareData->m_pGalaxy->m_hWnd;
				BindWebObj* pObj = (BindWebObj*)wParam;
				CWebPage* m_pHtmlWnd = (CWebPage*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
				if (m_pHtmlWnd == nullptr)
				{
					delete pObj;
					//for (auto it : g_pHubble->m_mapHtmlWnd)
					//{
					//	HWND hHtmlWnd = it.first;
					//	CWebPage* pWebWnd = (CWebPage*)it.second;
					//}
				}
				else
				{
					auto it = m_pHtmlWnd->m_mapBindWebObj.find(pObj->m_strBindObjName);
					if (it != m_pHtmlWnd->m_mapBindWebObj.end())
					{
						delete it->second;
						m_pHtmlWnd->m_mapBindWebObj.erase(it);
					}
					m_pHtmlWnd->m_mapBindWebObj[pObj->m_strBindObjName] = pObj;
				}
			}
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
			g_pHubble->m_mapNeedQueryOnClose[m_hWnd] = this;
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	auto it = g_pHubble->m_mapMainForm.find(m_hWnd);
	if (it != g_pHubble->m_mapMainForm.end())
	{
		g_pHubble->m_mapMainForm.erase(it);
		if (m_hWnd == g_pHubble->m_hMainWnd)
		{
			if (g_pHubble->m_mapMainForm.size())
			{
				it = g_pHubble->m_mapMainForm.begin();
				if (it != g_pHubble->m_mapMainForm.end())
					g_pHubble->m_hMainWnd = it->first;
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnHubbleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
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
		CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);

		CGalaxy* pGalaxy = nullptr;
		auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
		if (iter != pThreadInfo->m_mapGalaxy.end())
		{
			pGalaxy = (CGalaxy*)iter->second;
		}
		if (pGalaxy->m_pHostWebBrowserWnd)
		{
			HWND hWnd = pGalaxy->m_pHostWebBrowserWnd->m_pBrowser->GetActiveWebContentWnd();
			auto it = g_pHubble->m_mapHtmlWnd.find(hWnd);
			if (it != g_pHubble->m_mapHtmlWnd.end())
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
				if (m_pOwnerHtmlWnd)
				{
					ATLTRACE(_T("\n"));
					CString strHandle = _T("");
					strHandle.Format(_T("%d"), m_hWnd);
					m_pOwnerHtmlWnd->SendChromeIPCMessage(_T("MESSAGE"), m_strKey, strHandle, _T("MainMdiForm:ActiveClient"), m_strKey, L"");
				}
			}
			else
			{
				HWND hWnd = ::GetParent(m_hWnd);
				DWORD dwID = ::GetWindowThreadProcessId(hWnd, NULL);
				CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);

				CGalaxy* pGalaxy = nullptr;
				auto iter = pThreadInfo->m_mapGalaxy.find(hWnd);
				if (iter != pThreadInfo->m_mapGalaxy.end())
				{
					pGalaxy = (CGalaxy*)iter->second;
				}
				if (pGalaxy->m_pHostWebBrowserWnd)
				{
					HWND hWnd = pGalaxy->m_pHostWebBrowserWnd->m_pBrowser->GetActiveWebContentWnd();
					auto it = g_pHubble->m_mapHtmlWnd.find(hWnd);
					if (it != g_pHubble->m_mapHtmlWnd.end())
					{
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
		IGrid* pGrid = nullptr;
		g_pHubble->GetGridFromHandle((__int64)m_hWnd, &pGrid);
		if (pGrid)
		{
			IGalaxy* pGalaxy = nullptr;
			pGrid->get_Galaxy(&pGalaxy);
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
		auto it = g_pHubble->m_mapMainForm.find(m_hWnd);
		if (it == g_pHubble->m_mapMainForm.end())
			g_pHubble->m_mapMainForm[m_hWnd] = this;
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnHubbleGetXml(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strGalaxyName = (LPCTSTR)wParam;
	CString currentKey = (LPCTSTR)lParam;
	CString strIndex = strGalaxyName + L"_" + currentKey;
	if (m_bMdiForm)
	{
		auto it = m_mapKey.find(currentKey);
		if (it != m_mapKey.end())
		{
			CString strXml = it->second;
			CTangramXmlParse parse;
			if (parse.LoadXml(strXml))
			{
				CTangramXmlParse* pParse = parse.GetChild(strGalaxyName);
				if (pParse)
				{
					CTangramXmlParse* pParse2 = pParse->GetChild(currentKey);
					if (pParse2)
					{
						CString s = pParse2->xml();
						//LRESULT res = (LRESULT)LPSTR(LPCTSTR(s));
						auto it = g_pHubble->m_mapValInfo.find(strIndex);
						if (it != g_pHubble->m_mapValInfo.end())
						{
							g_pHubble->m_mapValInfo.erase(it);
						}
						g_pHubble->m_mapValInfo[strIndex] = CComVariant(s);
						return 1;
					}
				}
			}
		}
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
				auto it = g_pHubble->m_mapValInfo.find(strIndex);
				if (it != g_pHubble->m_mapValInfo.end())
				{
					g_pHubble->m_mapValInfo.erase(it);
				}
				g_pHubble->m_mapValInfo[strIndex] = CComVariant(pParse2->xml());
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
	g_pHubble->m_pActiveHtmlWnd = nullptr;
	g_pHubble->m_pActiveWinFormWnd = this;
	::BringWindowToTop(m_hWnd);
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
		HWND hwnd = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T("mdibk"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, (HWND)wParam, 0, theApp.m_hInstance, nullptr);
		m_pBKWnd->SubclassWindow(hwnd);
		m_pBKWnd->m_hChild = ::CreateWindowEx(NULL, _T("Hubble Grid Class"), _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 0, 0, hwnd, 0, theApp.m_hInstance, nullptr);
		CGalaxyCluster* pGalaxyCluster = nullptr;
		auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
		if (it != g_pHubble->m_mapWindowPage.end())
			pGalaxyCluster = (CGalaxyCluster*)it->second;
		if (pGalaxyCluster)
		{
			IGalaxy* pGalaxy = nullptr;
			pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)m_pBKWnd->m_hChild), CComBSTR(L"ClientFrame"), &pGalaxy);
			CString strXml = _T("");
			int nPos = m_strBKID.Find(_T(":"));
			if (nPos == -1)
				strXml.Format(_T("<mdiclient><layout><grid name=\"mdiclient\" objid=\"%s\" /></layout></mdiclient>"), m_strBKID);
			else
			{
				m_strBKID = m_strBKID.Mid(nPos + 1);
				strXml.Format(_T("<mdiclient><layout><grid name='mdiclient' url='%s' /></layout></mdiclient>"), m_strBKID);
			}
			IGrid* pGrid = nullptr;
			pGalaxy->Observe(CComBSTR(L"default"), strXml.AllocSysString(), &pGrid);
			auto it = pGalaxyCluster->m_mapGalaxy.find((HWND)wParam);
			if (it != pGalaxyCluster->m_mapGalaxy.end())
			{
				it->second->m_pBKWnd = m_pBKWnd;
			}
			pGalaxyCluster->m_pBKGalaxy = m_pBKWnd->m_pGalaxy = (CGalaxy*)pGalaxy;
			g_pHubble->m_mapBKFrame[m_pBKWnd->m_hChild] = m_pBKWnd->m_pGalaxy;
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
	if (g_pHubble->m_pCLRProxy)
	{
		g_pHubble->m_pCLRProxy->PreWindowPosChanging(m_hWnd, (WINDOWPOS*)lParam, 1);
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnFormCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pHubble->m_hFormNodeWnd = nullptr;
	return DefWindowProc(uMsg, wParam, lParam);
}

void CWinForm::OnFinalMessage(HWND hWnd)
{
	auto it = g_pHubble->m_mapFormWebPage.find(hWnd);
	if (it != g_pHubble->m_mapFormWebPage.end())
		g_pHubble->m_mapFormWebPage.erase(it);
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

CHubbleDocWnd::CHubbleDocWnd(void)
{
	m_hView = nullptr;
	m_strKey = _T("");
	m_strWndID = _T("default");
	m_pGalaxy = nullptr;
	m_pDocFrame = nullptr;
	m_pParentPage = nullptr;
	auto it = g_pHubble->m_mapValInfo.find(_T("newmdtframe"));
	if (it != g_pHubble->m_mapValInfo.end())
	{
		m_strKey = OLE2T(it->second.bstrVal);
		::VariantClear(&it->second);
		g_pHubble->m_mapValInfo.erase(it);
	}
}

CHubbleDocWnd::~CHubbleDocWnd(void)
{
	::SendMessage(g_pHubble->m_hHubbleWnd, WM_COSMOSMSG, 0, 10001000);
}

LRESULT CHubbleDocWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (m_pDocFrame && m_pDocFrame->m_pHubbleDoc)
	{
		CString strPath = g_pHubble->m_strAppDataPath + _T("default.tangramdoc");
		if (m_pDocFrame->m_pHubbleDoc->m_strPath.CompareNoCase(strPath) == 0)
		{
			::SendMessage(m_hWnd, WM_QUERYAPPPROXY, 0, TANGRAM_CONST_NEWDOC);
		}
		m_pDocFrame->m_pHubbleDoc->m_pAppProxy->m_strClosingFrameID = m_strKey;
		auto it = g_pHubble->m_mapMDTFrame.find(m_hWnd);
		if (it != g_pHubble->m_mapMDTFrame.end())
		{
			if (m_pDocFrame->m_pHubbleDoc->m_pAppProxy != g_pHubble->m_pUniverseAppProxy)
			{
				g_pHubble->m_pUniverseAppProxy->OnDestroyMainFrame(m_strKey, g_pHubble->m_mapMDTFrame.size(), 1);
			}
			m_pDocFrame->m_pHubbleDoc->m_pAppProxy->OnDestroyMainFrame(m_strKey, g_pHubble->m_mapMDTFrame.size(), 1);
		}
	}
	TRACE(_T("Close Tangram Document :%x\n"), this);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CHubbleDocWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (wParam)
	{
	case ID_FILE_NEW:
	case ID_FILE_NEW_FRAME:
	{
		if (m_pDocFrame)
		{
			IGrid* pGrid = nullptr;
			if (m_pDocFrame->m_pHostGalaxy)
			{
				m_pDocFrame->m_pHostGalaxy->Observe(CComBSTR(L"newdocument"), g_pHubble->m_strNewDocXml.AllocSysString(), &pGrid);
				g_pHubble->m_bNewFile = TRUE;
				return 0;
			}
		}
	}
	break;
	//case ID_FILE_OPEN:
	//{
	//	if (g_pHubble)
	//	{
	//		return ::SendMessage(g_pHubble->m_hHubbleWnd, WM_COSMOSMSG, (WPARAM)m_hWnd, 0);
	//	}
	//	return 0;
	//}

	break;
	case ID_FILE_SAVE:
	{
		if (m_pDocFrame)
		{
			m_pDocFrame->m_pHubbleDoc->m_pDocProxy->SaveDoc();
			return 0;
		}
		return 0;

	}
	case ID_FILE_SAVE_AS:
	{
		if (m_pDocFrame)
		{
			CString strID = m_pDocFrame->m_pHubbleDoc->m_strDocID;
		}
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CHubbleDocWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	auto it = g_pHubble->m_mapMDTFrame.find(m_hWnd);
	if (it != g_pHubble->m_mapMDTFrame.end())
	{
		g_pHubble->m_mapMDTFrame.erase(it);
		if (m_pDocFrame && m_pDocFrame->m_pHubbleDoc)
		{
			m_pDocFrame->m_pHubbleDoc->m_pAppProxy->m_strClosingFrameID = m_strKey;
			CString strKey = m_strKey + _T("-ondestroy");
			m_pDocFrame->m_pHubbleDoc->m_pAppProxy->m_hMainWnd = nullptr;
			if (m_pDocFrame->m_pHubbleDoc->m_pAppProxy != g_pHubble->m_pUniverseAppProxy)
			{
				g_pHubble->m_pUniverseAppProxy->OnDestroyMainFrame(strKey, g_pHubble->m_mapMDTFrame.size(), 1);
			}
			m_pDocFrame->m_pHubbleDoc->m_pAppProxy->OnDestroyMainFrame(strKey, g_pHubble->m_mapMDTFrame.size(), 1);
		}
	}

	if (m_pDocFrame)
	{
		if (m_pDocFrame->m_pHubbleDoc->m_pActiveWnd == this)
			m_pDocFrame->m_pHubbleDoc->m_pActiveWnd = nullptr;
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

void CHubbleDocWnd::OnFinalMessage(HWND hWnd)
{
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
	if (g_pHubble->m_bEclipse == false && g_pHubble->m_pMDIMainWnd == nullptr && g_pHubble->m_mapMDTFrame.size() == 0)
	{
		if (g_pHubble->m_mapBrowserWnd.size() == 1)
		{
			g_pHubble->m_bChromeNeedClosed = true;
			for (auto it : g_pHubble->m_mapBrowserWnd)
			{
				CBrowser* pWnd = (CBrowser*)it.second;
				pWnd->PostMessageW(WM_CLOSE, 0, 0);
			}
		}
	}
}

LRESULT CHubbleDocWnd::OnCtrlBarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HWND hWnd = (HWND)wParam;
	HWND hClient = ::GetDlgItem(hWnd, AFX_IDW_PANE_FIRST);
	if (::IsWindow(hClient))
	{
		::GetWindowText(hClient, g_pHubble->m_szBuffer, MAX_PATH);
		CString strText = CString(g_pHubble->m_szBuffer);
		CGalaxyCluster* pGalaxyCluster = nullptr;
		IGrid* pGrid = nullptr;
		HWND hPWnd = (HWND)lParam;
		auto it = g_pHubble->m_mapWindowPage.find(hPWnd);
		if (it == g_pHubble->m_mapWindowPage.end())
		{
			pGalaxyCluster = new CComObject<CGalaxyCluster>;
			pGalaxyCluster->m_hWnd = hPWnd;
			g_pHubble->m_mapWindowPage[hPWnd] = pGalaxyCluster;
			for (auto it : g_pHubble->m_mapHubbleAppProxy)
			{
				CGalaxyClusterProxy* pHubbleProxy = it.second->OnGalaxyClusterCreated(pGalaxyCluster);
				if (pHubbleProxy)
					pGalaxyCluster->m_mapGalaxyClusterProxy[it.second] = pHubbleProxy;
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
				((CGalaxy*)pGalaxy)->m_pDoc = m_pDocFrame->m_pHubbleDoc;
				CString strXml = _T("");
				auto it = m_pDocFrame->m_pHubbleDoc->m_mapWndScript.find(strText);
				if (it != m_pDocFrame->m_pHubbleDoc->m_mapWndScript.end())
				{
					strXml = it->second;
				}
				else
				{
					CString strPath = m_pDocFrame->m_pHubbleDoc->m_strPath;
					if (strPath == _T(""))
					{
						CString s = _T("");
						s.Format(_T("Do you want to create Control Bar \"%s\" from exists Common Template?"), strText);
						CString strCaption = s;
						s.Format(_T("Creating Control Bar: \"%s\""), strText);
						if (::MessageBox(::GetActiveWindow(), strCaption, s, MB_YESNO) == IDYES)
						{
							CString _strPath = g_pHubble->m_strAppCommonDocPath + _T("CommonCtrlBar\\");
							s.Format(_T("Please select Control Bar \"%s\" Template"), strText);
							strXml = g_pHubble->GetDocTemplateXml(s, _strPath, _T(".AppXml"));
						}
					}
				}
				pGalaxy->Observe(CComBSTR(strText), CComBSTR(strXml), &pGrid);
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CHubbleDocWnd::OnHubbleGetXml(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strGalaxyName = (LPCTSTR)wParam;
	strGalaxyName += _T("_");
	strGalaxyName += (LPCTSTR)lParam;
	auto it = this->m_pDocFrame->m_pHubbleDoc->m_mapWndScript.find(strGalaxyName);
	if (it != m_pDocFrame->m_pHubbleDoc->m_mapWndScript.end())
		return (LRESULT)LPCTSTR(it->second);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CHubbleDocWnd::OnHubbleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 19631222:
		return 0;
	case 1963://for Uer Control
	{
		CGridHelper* pWnd = (CGridHelper*)wParam;
		if (pWnd)
		{
			CGalaxyCluster* pGalaxyCluster = pWnd->m_pGrid->m_pGridShareData->m_pGalaxy->m_pGalaxyCluster;
			for (auto it : pWnd->m_mapDockCtrl)
			{
				TRACE(_T("DockCtrlName:%s\n"), it.first);
				TRACE(_T("DockCtrlHandle:%x\n"), it.second);
				CString strName = pWnd->m_pGrid->m_strName;
				strName += CComBSTR(L"_");
				strName += it.first;
				CString strKey = strName;
				strKey += CComBSTR(L"_default");
				CString strXml = _T("");
				auto it2 = m_pDocFrame->m_pHubbleDoc->m_mapWndScript.find(strKey);
				if (it2 != m_pDocFrame->m_pHubbleDoc->m_mapWndScript.end())
					strXml = it2->second;

				IGalaxy* pGalaxy = nullptr;
				pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)it.second), strName.AllocSysString(), &pGalaxy);
				CGalaxy* _pGalaxy = (CGalaxy*)pGalaxy;
				_pGalaxy->m_pParentGrid = pWnd->m_pGrid;
				IGrid* pGrid = nullptr;
				pGalaxy->Observe(CComBSTR(L"default"), CComBSTR(strXml), &pGrid);
				if (pGrid)
				{
					_pGalaxy->m_nGalaxyType = CtrlBarGalaxy;
					pWnd->m_pGrid->m_mapSubFrame[strName] = (CGalaxy*)pGalaxy;
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

	g_pHubble->m_pActiveDocWnd = this;
	LRESULT lRes = 0;
	HWND hPWnd = (HWND)lParam;
	if (::IsWindow(hPWnd))
		lRes = ::SendMessage(::GetParent(m_hWnd), WM_HUBBLE_DATA, 0, 1992);
	if (lRes)
	{
		m_pGalaxy = (CGalaxy*)lRes;
		IGrid* pGrid = nullptr;
		m_pGalaxy->Observe(m_pDocFrame->m_pHubbleDoc->m_strDocID.AllocSysString(), CComBSTR(_T("")), &pGrid);
		if (pGrid)
		{
			pGrid->put_SaveToConfigFile(true);
		}
	}
	if (m_pDocFrame)
	{
		IGalaxyCluster* pGalaxyCluster = nullptr;
		CComBSTR bstrTemplate(L"");
		auto it = m_pDocFrame->m_pHubbleDoc->m_mapWndScript.find(m_strWndID);
		if (it != m_pDocFrame->m_pHubbleDoc->m_mapWndScript.end())
			bstrTemplate = it->second.AllocSysString();
		//else
		//{
		//	if (::GetParent(m_hWnd) == NULL)
		//	{
		//		CString s = _T("");
		//		s.Format(_T("Do you want to create Tangram Document: \"%s\" from exists Common Document Template?"), m_pDocFrame->m_pHubbleDoc->m_strDocID);
		//		CString strCaption = s;
		//		s.Format(_T("Creating Tangram Document: \"%s\""), m_pDocFrame->m_pHubbleDoc->m_strDocID);
		//		if (::MessageBox(::GetActiveWindow(), strCaption, s, MB_YESNO) == IDYES)
		//		{
		//			CString _strPath = g_pHubble->m_strAppCommonDocPath + _T("CommonDocument\\");
		//			s.Format(_T("Please select Tangram Document \"%s\" Template"), m_pDocFrame->m_pHubbleDoc->m_strDocID);
		//			bstrTemplate = g_pHubble->GetDocTemplateXml(s, _strPath, _T(".AppXml")).AllocSysString();
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

		g_pHubble->CreateGalaxyCluster((LONGLONG)h, &pGalaxyCluster);
		if (pGalaxyCluster)
		{
			IGrid* pGrid = nullptr;
			CComBSTR bstrKey(m_pDocFrame->m_pHubbleDoc->m_strDocID);
			pGalaxyCluster->CreateGalaxyWithDefaultNode((LONGLONG)m_hView, bstrKey, m_strWndID.AllocSysString(), bstrTemplate, false, &pGrid);
			if (pGrid)
			{
				IGalaxy* pGalaxy = nullptr;
				pGrid->get_Galaxy(&pGalaxy);
				m_pDocFrame->m_pHostGalaxy = (CGalaxy*)pGalaxy;
				if (lRes != 0)
				{
					CGalaxyCluster* _pGalaxyCluster = (CGalaxyCluster*)pGalaxyCluster;
					_pGalaxyCluster->m_mapNeedSaveGalaxy[m_hView] = m_pDocFrame->m_pHostGalaxy;
				}
				m_pDocFrame->m_pHostGalaxy->m_pDoc = m_pDocFrame->m_pHubbleDoc;
				m_pDocFrame->m_pHubbleDoc->m_mapGalaxy[m_strWndID] = m_pDocFrame;
				m_pDocFrame->m_mapWnd[m_hWnd] = this;
				m_pDocFrame->m_pCurrentWnd = this;
				m_pDocFrame->m_pHubbleDoc->m_pActiveWnd = this;
				m_pDocFrame->m_pHostGalaxy->m_nGalaxyType = lParam ? MDIChildGalaxy : SDIGalaxy;
				if (lParam == 0)
				{
					CString strFix = g_pHubble->m_strAppName;
					if (strFix == _T(""))
						strFix = g_pHubble->m_strExeName;
					CString strAppTitle = m_pDocFrame->m_pHubbleDoc->m_pDocProxy->m_strAppName + _T(" - ") + strFix;
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
					pGrid = nullptr;
					it2.second->Observe(bstrKey, CComBSTR(L""), &pGrid);
					if (pGrid)
					{
						pGrid->put_SaveToConfigFile(true);
						CtrlBarDocActiveNotify(m_pDocFrame->m_pHubbleDoc, m_pDocFrame->m_pHostGalaxy->m_pWorkGrid, pGrid, it2.first);
					}
				}
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

void CHubbleDocWnd::CtrlBarDocActiveNotify(IHubbleDoc* pDoc, IGrid* pGridInDoc, IGrid* pGridInCtrlBar, HWND hCtrlBar)
{
	if (pDoc != nullptr)
	{
		CGrid* pGrid = (CGrid*)pGridInCtrlBar;
		switch (pGrid->m_nViewType)
		{
		case Grid:
		case TabGrid:
			for (auto it : pGrid->m_vChildNodes)
			{
				CtrlBarDocActiveNotify(pDoc, pGridInDoc, it, hCtrlBar);
			}
			break;
		default:
		{
			LRESULT lRes = ::SendMessage(m_pGalaxy->m_pGalaxyCluster->m_hWnd, WM_QUERYAPPPROXY, 0, 0);
			if (lRes > 0)
			{
				IUniverseAppProxy* pProxy = (IUniverseAppProxy*)lRes;
				pProxy->OnActiveDocument(pDoc, pGridInDoc, pGridInCtrlBar, hCtrlBar);
			}
			if (g_pHubble->m_pCosmosAppProxy)
			{
				g_pHubble->m_pCosmosAppProxy->OnActiveDocument(pDoc, pGridInDoc, pGridInCtrlBar, hCtrlBar);
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
	m_pWorkGrid = nullptr;
	m_pRootNodes = nullptr;
	m_pBindingGrid = nullptr;
	m_pContainerNode = nullptr;
	m_pDoc = nullptr;
	m_pGalaxyInfo = nullptr;
	m_pParentGrid = nullptr;
	m_pHubbleDocTemplate = nullptr;
	m_nGalaxyType = NOGalaxy;
#ifdef _DEBUG
	g_pHubble->m_nTangramFrame++;
#endif
}

CGalaxy::~CGalaxy()
{
#ifdef _DEBUG
	g_pHubble->m_nTangramFrame--;
#endif	
	//if (m_pGalaxyInfo)
	//	delete m_pGalaxyInfo;
	if (g_pHubble->m_pGalaxy == this)
		g_pHubble->m_pGalaxy = nullptr;
	for (auto it : g_pHubble->m_mapThreadInfo)
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
		CCommonFunction::ClearObject<CGridCollection>(m_pRootNodes);
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
	CGrid* pTopGrid = m_pWorkGrid;
	CGalaxy* _pGalaxy = this;
	if (!_pGalaxy->m_bDesignerState)
		while (pTopGrid && pTopGrid->m_pRootObj != pTopGrid)
		{
			_pGalaxy = pTopGrid->m_pRootObj->m_pGridShareData->m_pGalaxy;
			hwnd = _pGalaxy->m_hWnd;
			pTopGrid = _pGalaxy->m_pWorkGrid;
		}
	if (::IsWindow(hwnd) == false)
		return;
	HWND hPWnd = ::GetParent(m_hWnd);
	if (::IsWindow(_pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd))
	{
		RECT rt1;
		_pGalaxy->m_pWorkGrid->m_pHostWnd->GetWindowRect(&rt1);

		::ScreenToClient(hPWnd, (LPPOINT)&rt1);
		::ScreenToClient(hPWnd, ((LPPOINT)&rt1) + 1);

		HDWP dwh = BeginDeferWindowPos(1);
		UINT flag = SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_SHOWWINDOW;
		if (m_bObserve)
			flag |= SWP_NOREDRAW;
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
		//if (m_bTabbedMDIClient)
		::SendMessage(hPWnd, WM_QUERYAPPPROXY, 0, 19651965);
	}
}

CTangramXmlParse* CGalaxy::UpdateGrid()
{
	for (auto it : m_mapGrid) {
		CGrid* pWndGrid = (CGrid*)it.second;
		if (pWndGrid)
		{
			if (pWndGrid->m_pWindow) {
				if (pWndGrid->m_nActivePage > 0) {
					CString strVal = _T("");
					strVal.Format(_T("%d"), pWndGrid->m_nActivePage);
					pWndGrid->m_pHostParse->put_attr(_T("activepage"), strVal);
				}
				pWndGrid->m_pWindow->Save();
			}
			if (pWndGrid->m_nViewType == Grid)
			{
				((CGridWnd*)pWndGrid->m_pHostWnd)->Save();
			}

			for (auto it2 : pWndGrid->m_vChildNodes) {
				g_pHubble->UpdateGrid(it2);
			}

			if (pWndGrid == pWndGrid->m_pRootObj && pWndGrid->m_pGridShareData->m_pOfficeObj) {
				g_pHubble->UpdateOfficeObj(pWndGrid->m_pGridShareData->m_pOfficeObj, pWndGrid->m_pGridShareData->m_pHubbleParse->GetChild(TGM_CLUSTER)->xml(), pWndGrid->m_pGridShareData->m_pHubbleParse->name());
			}
		}
	}
	if (m_mapGrid.size())
		return m_mapGrid.begin()->second->m_pGridShareData->m_pHubbleParse;
	return nullptr;
}

void CGalaxy::UpdateDesignerTreeInfo()
{
	if (m_bDesignerState && g_pHubble->m_hChildHostWnd) {
		g_pHubble->m_pDesigningFrame = this;
		if (g_pHubble->m_pDocDOMTree && ::IsWindow(g_pHubble->m_pDocDOMTree->m_hWnd)) {
			g_pHubble->m_pDocDOMTree->DeleteItem(g_pHubble->m_pDocDOMTree->m_hFirstRoot);
			if (g_pHubble->m_pDocDOMTree->m_pHostXmlParse) {
				delete g_pHubble->m_pDocDOMTree->m_pHostXmlParse;
				g_pHubble->m_pDocDOMTree->m_pHostXmlParse = nullptr;
			}
			CGrid* pGrid = g_pHubble->m_pDesigningFrame->m_pWorkGrid;
			if (pGrid == nullptr) {
				return;
			}

			CString _strName = pGrid->m_strName;
			_strName += _T("-indesigning");
			_strName.MakeLower();
			CTangramXmlParse* pParse = nullptr;
			auto it = m_mapGrid.find(_strName);
			if (it != m_mapGrid.end())
				pParse = it->second->m_pGridShareData->m_pHubbleParse;
			else
				pParse = pGrid->m_pGridShareData->m_pHubbleParse;
			g_pHubble->InitDesignerTreeCtrl(pParse->xml());
		}
	}
}

HWND CGalaxy::GetWinForm(HWND hForm)
{
	if (g_pHubble->m_pCLRProxy)
	{
		int nForm = g_pHubble->m_pCLRProxy->IsWinForm(hForm);
		while (nForm >= 1)
		{
			m_pWorkGrid->m_pParentWinFormWnd = (CWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
			LRESULT lRes = ::SendMessage(hForm, WM_HUBBLE_GETNODE, 0, 0);
			if (lRes)
			{
				CGrid* pGrid = (CGrid*)lRes;
				hForm = pGrid->m_pRootObj->m_pGridShareData->m_pGalaxyCluster->m_hWnd;
				nForm = g_pHubble->m_pCLRProxy->IsWinForm(hForm);
			}
			else
			{
				break;
			}
		}
		if (nForm == 0)
		{
			hForm = ::GetParent(hForm);
			while (g_pHubble->m_pCLRProxy->IsWinForm(hForm) == 0)
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

CGrid* CGalaxy::ObserveXtmlDocument(CTangramXmlParse* _pParse, CString strKey, CString strFile)
{
	m_pWorkGrid = new CComObject<CGrid>;
	m_pWorkGrid->m_pRootObj = m_pWorkGrid;
	CGridShareData* pCommonData = new CGridShareData();
	m_pWorkGrid->m_pGridShareData = pCommonData;
	pCommonData->m_pGalaxy = this;
	pCommonData->m_pGalaxyCluster = m_pGalaxyCluster;
	pCommonData->m_pHubbleParse = _pParse;
	CTangramXmlParse* pParse = _pParse->GetChild(TGM_CLUSTER);
	m_pWorkGrid->m_pHostParse = pParse->GetChild(TGM_GRID);

	CreateGalaxyCluster();
	m_mapGrid[strKey] = m_pWorkGrid;

	if (m_pGalaxyCluster)
		m_pGalaxyCluster->Fire_OpenXmlComplete(strFile.AllocSysString(), (long)m_hHostWnd, m_pWorkGrid);
	m_pWorkGrid->m_strKey = strKey;
	m_pWorkGrid->Fire_ObserveComplete();
	if (g_pHubble->m_pCLRProxy)
	{
		CGalaxyCluster* pGalaxyCluster = m_pGalaxyCluster;
		HWND hForm = pGalaxyCluster->m_hWnd;
		hForm = GetWinForm(hForm);
		if(hForm)
			m_pWorkGrid->m_pParentWinFormWnd = (CWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
	}
	return m_pWorkGrid;
}

BOOL CGalaxy::CreateGalaxyCluster()
{
	if (::IsWindow(m_hWnd) == false)
		SubclassWindow(m_hHostWnd);
	HWND hPWnd = NULL;
	if (m_hPWnd && ::IsWindow(m_hPWnd))
		hPWnd = m_pGalaxyCluster->m_hWnd;
	else
		hPWnd = ::GetParent(m_hWnd);

	m_pWorkGrid->m_strName.Trim();
	m_pWorkGrid->m_strName.MakeLower();
	m_pWorkGrid->InitWndGrid();
	HWND hWnd = NULL;
	if (m_pWorkGrid->m_pObjClsInfo) {
		RECT rc;
		CWnd* pParentWnd = CWnd::FromHandle(hPWnd);
		m_pWorkGrid->m_pRootObj = m_pWorkGrid;
		CCreateContext	m_Context;
		m_Context.m_pNewViewClass = m_pWorkGrid->m_pObjClsInfo;
		CWnd* pWnd = (CWnd*)m_pWorkGrid->m_pObjClsInfo->CreateObject();
		GetWindowRect(&rc);
		if (pParentWnd)
			pParentWnd->ScreenToClient(&rc);

		pWnd->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rc, pParentWnd, 0, &m_Context);
		hWnd = pWnd->m_hWnd;
		pWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		::SetWindowPos(pWnd->m_hWnd, HWND_BOTTOM, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_NOACTIVATE);//|SWP_NOREDRAWSWP_NOZORDER);
	}

	CTangramXmlParse* pWndParse = m_pWorkGrid->m_pGridShareData->m_pHubbleParse->GetChild(_T("docplugin"));
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
						m_pWorkGrid->m_pGridShareData->m_PlugInDispDictionary[strPlugID] = pDisp.p;
						pDisp.p->AddRef();
					}

					m_pWorkGrid->Fire_GridAddInCreated(pDisp.p, bstrPlugIn, bstrXml);
				}
				else //for .NET Component
				{
					hr = g_pHubble->CreateCLRObj(bstrPlugIn, &pDisp);
					if (hr == S_OK)
					{
						m_pWorkGrid->m_pGridShareData->m_PlugInDispDictionary[strPlugID] = pDisp.p;

						bstrPlugIn = strPlugID.AllocSysString();
						m_pWorkGrid->Fire_GridAddInCreated(pDisp, bstrPlugIn, bstrXml);
					}
				}
				if (m_pGalaxyCluster && pDisp)
					m_pGalaxyCluster->Fire_AddInCreated(m_pWorkGrid, pDisp, bstrPlugIn, bstrXml);
				::SysFreeString(bstrPlugIn);
				bHavePlugin = true;
			}
			::SysFreeString(bstrXml);
		}

		if (bHavePlugin)
			m_pWorkGrid->Fire_GridAddInsCreated();
	}
	m_pWorkGrid->m_bCreated = true;
	return true;
}

STDMETHODIMP CGalaxy::get_RootGrids(IGridCollection** pGridColletion)
{
	if (m_pRootNodes == nullptr) {
		CComObject<CGridCollection>::CreateInstance(&m_pRootNodes);
		m_pRootNodes->AddRef();
	}

	m_pRootNodes->m_pGrids->clear();
	for (auto it : m_mapGrid) {
		m_pRootNodes->m_pGrids->push_back(it.second);
	}

	return m_pRootNodes->QueryInterface(IID_IGridCollection, (void**)pGridColletion);
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
		::ShowWindow(m_pWorkGrid->m_pHostWnd->m_hWnd, SW_HIDE);
		RECT rect;
		m_pWorkGrid->m_pHostWnd->GetWindowRect(&rect);
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
		::ShowWindow(m_pWorkGrid->m_pHostWnd->m_hWnd, SW_SHOW);
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
	if (m_pWorkGrid == nullptr)
		return S_FALSE;
	HWND hParent = (HWND)::GetParent(_hHostWnd);
	CWindow m_Parent;
	RECT rc;
	m_pWorkGrid->m_pHostWnd->GetWindowRect(&rc);
	if (::IsWindow(m_hWnd)) {
		HWND hTangramWnd = m_pGalaxyCluster->m_hWnd;
		auto it = g_pHubble->m_mapWindowPage.find(hTangramWnd);
		if (it != g_pHubble->m_mapWindowPage.end())
			g_pHubble->m_mapWindowPage.erase(it);
		m_pGalaxyCluster->m_hWnd = hTangramWnd;
		g_pHubble->m_mapWindowPage[hTangramWnd] = m_pGalaxyCluster;

		auto it2 = m_pGalaxyCluster->m_mapGalaxy.find(m_hWnd);
		if (it2 != m_pGalaxyCluster->m_mapGalaxy.end()) {
			m_pGalaxyCluster->m_mapGalaxy.erase(it2);
			m_pGalaxyCluster->m_mapGalaxy[_hHostWnd] = this;
			m_pGalaxyCluster->m_mapWnd[m_strGalaxyName] = _hHostWnd;
			DWORD dwID = ::GetWindowThreadProcessId(_hHostWnd, NULL);
			TRACE(_T("ObserveEx ThreadInfo:%x\n"), dwID);
			CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);
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
	for (auto it : m_mapGrid)
	{
		if (it.second != m_pWorkGrid)
		{
			::SetParent(it.second->m_pHostWnd->m_hWnd, m_pWorkGrid->m_pHostWnd->m_hWnd);
		}
	}
	::SetParent(m_pWorkGrid->m_pHostWnd->m_hWnd, ::GetParent(_hHostWnd));
	::SetWindowPos(m_pWorkGrid->m_pHostWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
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

STDMETHODIMP CGalaxy::get_VisibleGrid(IGrid** pVal)
{
	if (m_pWorkGrid)
		*pVal = m_pWorkGrid;
	return S_OK;
}

STDMETHODIMP CGalaxy::get_GalaxyCluster(IGalaxyCluster** pVal)
{
	if (m_pGalaxyCluster)
		*pVal = m_pGalaxyCluster;

	return S_OK;
}

STDMETHODIMP CGalaxy::Observe(BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid)
{
	if (::GetWindowLong(m_hWnd, GWL_STYLE) & MDIS_ALLCHILDSTYLES)
		m_nGalaxyType = GalaxyType::MDIClientGalaxy;
	CString _strXml = OLE2T(bstrXml);
	if (m_pGalaxyCluster->m_strPageFileName == _T(""))
	{
		m_pGalaxyCluster->m_strPageFileName = g_pHubble->m_strExeName;
		m_pGalaxyCluster->m_strPageFilePath = g_pHubble->m_strConfigDataFile;
	}
	DWORD dwID = ::GetWindowThreadProcessId(m_hHostWnd, NULL);
	TRACE(_T("ObserveEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);
	theApp.SetHook(dwID);

	CString strCurrentKey = OLE2T(bstrKey);
	if (strCurrentKey == _T(""))
		strCurrentKey = _T("default");
	if (m_strCurrentKey != strCurrentKey)
	{
		m_strLastKey = m_strCurrentKey;
		m_strCurrentKey = strCurrentKey;
	}
	g_pHubble->m_pGalaxyCluster = m_pGalaxyCluster;
	g_pHubble->m_pGalaxy = this;

	m_strCurrentKey = m_strCurrentKey.MakeLower();
	g_pHubble->m_strCurrentKey = m_strCurrentKey;
	CString strXml = _T("");
	auto it = m_mapGrid.find(m_strCurrentKey);

	CGrid* pOldNode = m_pWorkGrid;
	if (it != m_mapGrid.end())
		m_pWorkGrid = it->second;
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
				auto it = g_pHubble->m_mapValInfo.find(m_strGalaxyName + L"_" + m_strCurrentKey);
				if (it != g_pHubble->m_mapValInfo.end())
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
				if (pWnd == nullptr && m_pHubbleDocTemplate && m_strCurrentKey != _T("newdocument"))
				{
					CString strKey = m_strGalaxyName;
					strKey.Replace(_T("@"), _T("_"));
					auto it = m_pHubbleDocTemplate->m_mapXml.find(strKey);
					if (it != m_pHubbleDocTemplate->m_mapXml.end())
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
							CTangramXmlParse* m_pHubblePageParse = nullptr;
							CTangramXmlParse* m_pHubblePageParse2 = nullptr;
							if (m_pGalaxyCluster->m_bDoc == false && ::PathFileExists(m_pGalaxyCluster->m_strPageFilePath))
							{
								CTangramXmlParse m_Parse;
								if (m_Parse.LoadFile(m_pGalaxyCluster->m_strPageFilePath))
								{
									m_pHubblePageParse = m_Parse.GetChild(_T("hubblepage"));
									if (m_pHubblePageParse)
									{
										m_pHubblePageParse2 = m_pHubblePageParse->GetChild(m_pGalaxyCluster->m_strConfigFileNodeName);
										if (m_pHubblePageParse2)
										{
											int nCount = m_pHubblePageParse2->GetCount();
											for (int i = 0; i < nCount; i++)
											{
												CTangramXmlParse* _pParse = m_pHubblePageParse2->GetChild(i);
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
							strXml = _T("<default><layout><grid name=\"Start\" /></layout></default>");;
					}
					else
						strXml = _strXml;
				}
			}
		}

		Unlock();
		m_pGalaxyCluster->Fire_BeforeOpenXml(CComBSTR(strXml), (long)m_hHostWnd);

		m_bObserve = true;

		m_pWorkGrid = g_pHubble->ObserveEx((long)m_hHostWnd, _T(""), strXml);
		if (m_pWorkGrid == nullptr)
			return S_FALSE;
		if (::GetWindowLong(::GetParent(m_hWnd), GWL_EXSTYLE) & WS_EX_MDICHILD)
			m_bMDIChild = true;
	}
	m_pBindingGrid = m_pWorkGrid->m_pGridShareData->m_pHostClientView ? m_pWorkGrid->m_pGridShareData->m_pHostClientView->m_pGrid : nullptr;

	g_pHubble->m_strCurrentKey = _T("");
	*ppRetGrid = (IGrid*)m_pWorkGrid;
	for (auto it : g_pHubble->m_mapHubbleAppProxy)
	{
		it.second->OnObserverComplete(m_hHostWnd, strXml, m_pWorkGrid);
	}
	if (g_pHubble->m_pCosmosAppProxy)
		g_pHubble->m_pCosmosAppProxy->OnObserverComplete(m_hHostWnd, strXml, m_pWorkGrid);

	if (pOldNode && pOldNode != m_pWorkGrid)
	{
		RECT  rc;
		if (::IsWindow(pOldNode->m_pHostWnd->m_hWnd))
			::GetWindowRect(pOldNode->m_pHostWnd->m_hWnd, &rc);
		CWnd* pWnd = m_pWorkGrid->m_pHostWnd;
		HWND hParent = ::GetParent(m_hWnd);

		CWnd::FromHandle(hParent)->ScreenToClient(&rc);
		for (auto it : m_mapGrid)
		{
			HWND hwnd = it.second->m_pHostWnd->m_hWnd;
			BOOL bTop = (it.second == m_pWorkGrid);
			it.second->m_bTopObj = bTop;
			::SetWindowLongPtr(hwnd, GWLP_ID, bTop ? m_pWorkGrid->m_nID : 0);
			::SetParent(hwnd, bTop ? hParent : pWnd->m_hWnd);
			if (!bTop)
			{
				::ShowWindow(hwnd, SW_HIDE);
			}
		}
		::SetWindowPos(pWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);

		//if (m_pWorkGrid != nullptr) {
		//	if (m_pWorkGrid->m_pWebBrowser&& 
		//		m_pWorkGrid->m_pWebBrowser->m_pVisibleWebWnd->m_pChromeRenderFrameHost)
		//	{
		//		m_pWorkGrid->m_pWebBrowser->m_pVisibleWebWnd->m_pChromeRenderFrameHost->ShowWebPage();
		//		::PostMessage(m_pWorkGrid->m_pWebBrowser->m_pVisibleWebWnd->m_hWnd, WM_COSMOSMSG, 20200131, 0);
		//	}
		//}
		if (m_pWorkGrid != nullptr) {
			if (m_pWorkGrid->m_nViewType != Grid) {
				if (m_pWorkGrid->m_pHostWnd)
					m_pWorkGrid->m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
			HRESULT hr = S_OK;
			int cConnections = g_pHubble->m_vec.GetSize();
			if (cConnections) {
				CComVariant avarParams[3];
				avarParams[2] = (long)m_hHostWnd;
				avarParams[2].vt = VT_I4;
				avarParams[1] = strXml.AllocSysString();
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = (IGrid*)m_pWorkGrid;
				avarParams[0].vt = VT_DISPATCH;
				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				for (int iConnection = 0; iConnection < cConnections; iConnection++) {
					g_pHubble->Lock();
					CComPtr<IUnknown> punkConnection = g_pHubble->m_vec.GetAt(iConnection);
					g_pHubble->Unlock();
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
			it.second->OnExtend(m_pWorkGrid, m_strCurrentKey, strXml);
		}

		if (m_pBKWnd && m_pBKWnd->m_pGalaxy)
		{
			IGrid* pGrid = nullptr;
			m_pBKWnd->m_pGalaxy->Observe(CComBSTR(L"default"), CComBSTR(L""), &pGrid);
		}
	}

	m_bObserve = false;

	HostPosChanged();
	//Add 20200218
	if (m_pBindingGrid)
	{
		CGrid* _pHostNode = m_pBindingGrid;
		if (_pHostNode->m_pHostGalaxy)
		{
			CGalaxy* _pGalaxy = _pHostNode->m_pHostGalaxy;
			while (_pGalaxy)
			{
				_pHostNode = _pGalaxy->m_pBindingGrid;
				if (_pHostNode && _pHostNode->m_pHostGalaxy)
					_pGalaxy = _pHostNode->m_pHostGalaxy;
				else
					break;
			}
		}
		if (_pHostNode&&m_pWebPageWnd)
		{
			CGrid* pGrid = _pHostNode->m_pRootObj;
			if (pGrid->m_strLastIPCMsgID != _T(""))
			{
				IPCMsg pIPCInfo;
				pIPCInfo.m_strId = pGrid->m_strLastIPCMsgID;
				pIPCInfo.m_strParam1 = pGrid->m_strLastIPCParam1;
				pIPCInfo.m_strParam2 = pGrid->m_strLastIPCParam2;
				pIPCInfo.m_strParam3 = pGrid->m_strLastIPCParam5;
				pIPCInfo.m_strParam4 = pGrid->m_strLastIPCParam4;
				pIPCInfo.m_strParam5 = pGrid->m_strLastIPCParam3;
				m_pWebPageWnd->m_pChromeRenderFrameHost->SendHubbleMessage(&pIPCInfo);
			}
			g_pHubble->m_pCurrentIPCMsg = nullptr;
		}
	}
	//end Add 20200218

	if (m_pWorkGrid->m_pHostGalaxy)
	{
		IGrid* pGrid = nullptr;
		m_pWorkGrid->m_pHostGalaxy->Observe(CComBSTR(m_pWorkGrid->m_pHostGalaxy->m_strCurrentKey), CComBSTR(""), &pGrid);
	}
	for (auto it : m_pWorkGrid->m_mapExtendNode)
	{
		IGrid* pGrid = nullptr;
		it.first->Observe(CComBSTR(it.second), CComBSTR(""), &pGrid);
	}
	if (m_pWebPageWnd)
	{
		ATLTRACE(L"\n");
	}
	if (m_pHostWebBrowserWnd)
	{
		IGrid* pGrid = nullptr;
		CComPtr<IGridCollection> pCol;
		long nCount = 0;
		m_pWorkGrid->GetGrids(CComBSTR(m_strHostWebBrowserNodeName), &pGrid, &pCol, &nCount);
		if (pGrid)
		{
			CGrid* _pGrid = (CGrid*)pGrid;
			if (_pGrid->m_nViewType == BlankView)
			{
				CGalaxy* _pGalaxy = nullptr;
				if (_pGrid->m_pHostGalaxy)
				{
					_pGalaxy = _pGrid->m_pHostGalaxy;
					while (_pGalaxy)
					{
						if(_pGalaxy->m_pHostWebBrowserNode)
							_pGrid = _pGalaxy->m_pHostWebBrowserNode;
						if (_pGrid && _pGrid->m_pHostGalaxy)
						{
							_pGalaxy = _pGrid->m_pHostGalaxy;
							_pGrid = _pGalaxy->m_pHostWebBrowserNode;
						}
						else
							break;
					}
				}
				if (m_pHostWebBrowserWnd->m_pParentGrid == nullptr)
				{
					m_pHostWebBrowserWnd->m_pParentGrid = _pGrid;
					m_pHostWebBrowserWnd->m_pParentGrid->m_pWebBrowser = m_pHostWebBrowserWnd;
				}
				else if (m_pHostWebBrowserWnd->m_pParentGrid != _pGrid)//&&_pGrid->m_pHostWnd->IsWindowVisible())
				{
					m_pHostWebBrowserWnd->m_pParentGrid->m_pWebBrowser = nullptr;
					m_pHostWebBrowserWnd->m_pParentGrid = _pGrid;
					_pGrid->m_pWebBrowser = m_pHostWebBrowserWnd;
					::SetParent(m_pHostWebBrowserWnd->m_hWnd, _pGrid->m_pHostWnd->m_hWnd);
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
				it.second->m_pHubbleWPFObj->ShowVisual(false);
				it.second->m_pHubbleWPFObj->ShowVisual(true);
				it.second->m_pHubbleWPFObj->InvalidateVisual();
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
					it.second->m_pHubbleWPFObj->ShowVisual(false);
					m_mapVisibleWPFView.erase(it2);
				}
			}
		}
	}
}

void CGalaxy::Destroy()
{
	CGrid* pWndGrid = nullptr;
	CString strPlugID = _T("");
	for (auto it : m_mapGrid)
	{
		pWndGrid = it.second;
		if (pWndGrid->m_pGridShareData->m_pHubbleParse)
		{
			CTangramXmlParse* pParse = pWndGrid->m_pGridShareData->m_pHubbleParse->GetChild(_T("docplugin"));
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
							IDispatch* pDisp = (IDispatch*)pWndGrid->m_pGridShareData->m_PlugInDispDictionary[strPlugID];
							if (pDisp)
							{
								pWndGrid->m_pGridShareData->m_PlugInDispDictionary.RemoveKey(LPCTSTR(strPlugID));
								pDisp->Release();
							}
						}
					}
				}
			}

			pWndGrid->m_pGridShareData->m_PlugInDispDictionary.RemoveAll();
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
	if (g_pHubble->m_pActiveHtmlWnd)
	{
		g_pHubble->m_pActiveHtmlWnd = nullptr;
	}

	if ((long)(g_pHubble->m_pActiveAppProxy) > 1)
	{
		HWND hMenuWnd = g_pHubble->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	if (m_pBKWnd == nullptr)
	{
		g_pHubble->m_hActiveWnd = 0;
		g_pHubble->m_bWinFormActived = false;
		g_pHubble->m_pActiveGrid = nullptr;
		g_pHubble->m_pGalaxy = nullptr;
	}
	//bug fix for winform:
	//++++++++++++++++begin+++++++++++
	HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
	if (hTop != nullptr && g_pHubble->m_pCLRProxy)
	{
		g_pHubble->m_bWinFormActived = g_pHubble->m_pCLRProxy->IsWinForm(hTop);
		if (g_pHubble->m_bWinFormActived)
		{
			g_pHubble->m_hActiveWnd = hTop;
			g_pHubble->m_pGalaxy = this;
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
			::GetClassName(hFirst, g_pHubble->m_szBuffer, MAX_PATH);
			CString strName = CString(g_pHubble->m_szBuffer);
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
						CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(::GetWindowThreadProcessId(m_hWnd, NULL));
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
		if (wParam && g_pHubble->m_pMDIMainWnd && g_pHubble->m_pMDIMainWnd->m_hMDIClient == m_hWnd)
		{
			TRACE(_T("\n"));
		}
	}
	return lRes;
}

LRESULT CGalaxy::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pHubble->m_pCLRProxy) {
		g_pHubble->m_pCLRProxy->ReleaseHubbleObj((IGalaxy*)this);
	}
	if (m_mapGridScript.size())
	{
		this->UpdateGrid();
		for (auto it : m_mapGridScript)
		{
			CString strPath = it.first;
			CString s = it.second->m_pHostParse->xml();
			CString str = _T("");
			str.Format(_T("<nodexml><layout>%s</layout></nodexml>"), s);
			CTangramXmlParse parse;
			parse.LoadXml(str);
			parse.SaveFile(it.first);
		}
	}
	if (m_pBKWnd)
		m_pBKWnd->DestroyWindow();
	if (g_pHubble->m_pDesigningFrame && g_pHubble->m_pDesigningFrame == this)
		g_pHubble->m_pDesigningFrame = nullptr;
	m_pGalaxyCluster->BeforeDestory();
	m_pGalaxyCluster->m_strConfigFileNodeName.MakeLower();//20190116
	//auto it = g_pHubble->m_mapWindowPage.find(m_pGalaxyCluster->m_hWnd);
	//if (it != g_pHubble->m_mapWindowPage.end())
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
	CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);
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

LRESULT CGalaxy::OnHubbleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 2048:
	{
		if (m_hWnd != g_pHubble->m_hChildHostWnd)
		{
			CtrlInfo* pCtrlInfo = (CtrlInfo*)wParam;
			if (pCtrlInfo && pCtrlInfo->m_pGalaxyCluster)
			{
				CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)pCtrlInfo->m_pGalaxyCluster;
				pGalaxyCluster->Fire_ClrControlCreated(pCtrlInfo->m_pGrid, pCtrlInfo->m_pCtrlDisp, pCtrlInfo->m_strName.AllocSysString(), (long)pCtrlInfo->m_hWnd);
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
		if (m_pWorkGrid)
		{
			::PostMessage(m_pWorkGrid->m_pHostWnd->m_hWnd, WM_COSMOSMSG, 0, 20200601);
		}
	}
	break;
	case 20200531:
	{
		//for webruntimeVS Dockabe ToolWindow
		if (m_pWorkGrid)
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
		TangramFrameInfo* pHubbleFrameInfo = (TangramFrameInfo*)wParam;
		CString _strKey = _T(",");
		_strKey += pHubbleFrameInfo->m_strKey;
		_strKey += _T(",");
		if (m_strAsynKeys.Find(_strKey) != -1)
		{
			m_strAsynKeys.Replace(_strKey, _T(""));
		}

		IGrid* pGrid = nullptr;
		int nPos = pHubbleFrameInfo->m_strXml.ReverseFind('>');
		CString strXml = pHubbleFrameInfo->m_strXml;
		this->Observe(pHubbleFrameInfo->m_strKey.AllocSysString(), CComBSTR(strXml), &pGrid);
		delete pHubbleFrameInfo;
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
		::GetClassName(hWnd2, g_pHubble->m_szBuffer, MAX_PATH);
		CString strName = CString(g_pHubble->m_szBuffer);
		if (strName == _T("SWT_Window0"))
		{
			::GetClientRect(hWnd2, &rc);
			if (rc.right * rc.bottom == 0)
				::MoveWindow(hWnd2, 0, 0, rc0.right, rc0.bottom, 1);
			hWnd2 = ::GetWindow(hWnd2, GW_CHILD);
			if (hWnd2 == nullptr)
				return 0;
			::GetClassName(hWnd2, g_pHubble->m_szBuffer, MAX_PATH);
			strName = CString(g_pHubble->m_szBuffer);
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
	if (g_pHubble->m_pMDIMainWnd && lParam == 19651965)
	{
		m_bTabbedMDIClient = true;
		LPRECT lpRECT = (LPRECT)wParam;
		if (lpRECT && m_pWorkGrid && ::IsWindowVisible(m_pWorkGrid->m_pHostWnd->m_hWnd))
		{
			::SetWindowPos(m_pWorkGrid->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpRECT->left, lpRECT->top, lpRECT->right - lpRECT->left, lpRECT->bottom - lpRECT->top, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_FRAMECHANGED);/*SWP_FRAMECHANGED| SWP_HIDEWINDOW| SWP_NOZORDER | SWP_NOREDRAW */
			if (m_pBindingGrid && ::IsWindowVisible(m_pBindingGrid->m_pHostWnd->m_hWnd))
			{
				m_pBindingGrid->m_pHostWnd->GetWindowRect(lpRECT);
				g_pHubble->m_pMDIMainWnd->ScreenToClient(lpRECT);
			}
		}
		return m_pWorkGrid ? ((LRESULT)m_pWorkGrid->m_pHostWnd->m_hWnd) : 0;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT hr = DefWindowProc(uMsg, wParam, lParam);

	if (g_pHubble->m_pBrowserFactory && g_pHubble->m_bChromeNeedClosed)
		return hr;

	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
	if (m_pWorkGrid)
	{
		if (m_pBindingGrid)
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
			::SetWindowPos(m_pWorkGrid->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOACTIVATE| SWP_FRAMECHANGED );// |SWP_NOREDRAW); 
			CGrid* _pHostNode = m_pBindingGrid;
			if (_pHostNode->m_pHostGalaxy)
			{
				CGalaxy* _pGalaxy = _pHostNode->m_pHostGalaxy;
				while (_pGalaxy)
				{
					if (_pGalaxy->m_pBindingGrid)
						_pHostNode = _pGalaxy->m_pBindingGrid;
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
				CGrid* _pParentNode = m_pBindingGrid->m_pParentObj;
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
						CGridHelper* pWnd = (CGridHelper*)_pParentNode->m_pHostWnd;
						::InvalidateRect(pWnd->m_hWnd, nullptr, true);
					}
					break;
					}
				}
				if (m_pContainerNode && m_pContainerNode->m_pParentObj && m_pContainerNode->m_pParentObj->m_nViewType == Grid)
				{
					if (m_pContainerNode->m_nViewType == CLRCtrl)
					{
						CGridHelper* pGridWnd = (CGridHelper*)m_pContainerNode->m_pHostWnd;
						pGridWnd->m_bNoMove = true;
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
			::SetWindowPos(m_pWorkGrid->m_pHostWnd->m_hWnd, HWND_TOP, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOSENDCHANGING | /*SWP_NOZORDER |*/ SWP_NOACTIVATE | SWP_FRAMECHANGED);
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
	g_pHubble->m_pGalaxy = nullptr;
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
	CString strXmlData = _T("<Default><layout><grid name=\"Start\"/></layout></Default>");
	CString strName = _T("");
	CString strXml = _T("");

	map<CString, CString> m_mapTemp;
	map<CString, CString>::iterator it2;
	for (auto it : m_mapGrid)
	{
		g_pHubble->UpdateGrid(it.second);
		strName = it.first;
		int nPos = strName.Find(_T("-"));
		CString str = strName.Mid(nPos + 1);
		if (str.CompareNoCase(_T("inDesigning")) == 0)
		{
			strName = strName.Left(nPos);
			m_mapTemp[strName] = it.second->m_pGridShareData->m_pHubbleParse->xml();
		}
	}

	for (auto it : m_mapGrid)
	{
		strName = it.first;
		if (strName.Find(_T("-indesigning")) == -1)
		{
			it2 = m_mapTemp.find(strName);
			if (it2 != m_mapTemp.end())
				strXml = it2->second;
			else
				strXml = it.second->m_pGridShareData->m_pHubbleParse->xml();
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
	*pCount = (long)m_mapGrid.size();
	return S_OK;
}

STDMETHODIMP CGalaxy::get_Grid(VARIANT vIndex, IGrid** ppGrid)
{
	if (vIndex.vt == VT_I4)
	{
		long lCount = m_mapGrid.size();
		int iIndex = vIndex.lVal;
		if (iIndex < 0 || iIndex >= lCount) return E_INVALIDARG;

		auto it = m_mapGrid.begin();
		int iPos = 0;
		while (it != m_mapGrid.end())
		{
			if (iPos == iIndex) break;
			iPos++;
			it++;
		}

		CGrid* pGrid = it->second;
		*ppGrid = pGrid;
		return S_OK;
	}

	if (vIndex.vt == VT_BSTR)
	{
		CString strKey = OLE2T(vIndex.bstrVal);
		auto it = m_mapGrid.find(strKey);
		if (it != m_mapGrid.end())
		{
			*ppGrid = it->second;
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
		long nLen = (long)m_mapGrid.size();
		VARIANT* rgvar = new VARIANT[nLen];
		ZeroMemory(rgvar, sizeof(VARIANT) * nLen);
		VARIANT* pItem = rgvar;
		for (auto it : m_mapGrid)
		{
			IUnknown* pDisp = nullptr;
			CGrid* pObj = it.second;
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
	for (auto it : m_mapGrid)
	{
		CGrid* pWndGrid = (CGrid*)it.second;
		if (pWndGrid)
		{
			if (pWndGrid->m_pWindow)
			{
				if (pWndGrid->m_nActivePage > 0)
				{
					CString strVal = _T("");
					strVal.Format(_T("%d"), pWndGrid->m_nActivePage);
					pWndGrid->m_pHostParse->put_attr(_T("activepage"), strVal);
				}
				pWndGrid->m_pWindow->Save();
			}
			if (pWndGrid->m_nViewType == Grid)
			{
				((CGridWnd*)pWndGrid->m_pHostWnd)->Save();
			}

			for (auto it2 : pWndGrid->m_vChildNodes)
			{
				g_pHubble->UpdateGrid(it2);
			}

			if (pWndGrid == pWndGrid->m_pRootObj && pWndGrid->m_pGridShareData->m_pOfficeObj)
			{
				g_pHubble->UpdateOfficeObj(pWndGrid->m_pGridShareData->m_pOfficeObj, pWndGrid->m_pGridShareData->m_pHubbleParse->GetChild(TGM_CLUSTER)->xml(), pWndGrid->m_pGridShareData->m_pHubbleParse->name());
			}
		}
		CString strXml = pWndGrid->m_pGridShareData->m_pHubbleParse->GetChild(TGM_CLUSTER)->xml();
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

STDMETHODIMP CGalaxy::get_HubbleDoc(IHubbleDoc** pVal)
{
	if (m_pDoc == nullptr)
	{
		CHubbleDocWnd* pWnd = (CHubbleDocWnd*)::SendMessage(::GetParent(m_hWnd), WM_COSMOSMSG, 2016, 0);
		if (pWnd)
		{
			m_pDoc = pWnd->m_pDocFrame->m_pHubbleDoc;
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
			auto it = g_pHubble->m_mapBrowserWnd.find(hPWnd);
			if (it != g_pHubble->m_mapBrowserWnd.end())
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

