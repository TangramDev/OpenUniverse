/********************************************************************************
*					Open Universe - version 1.1.2.23							*
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

// TangramTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "Grid.h"
#include "Galaxy.h"
#include "TangramTabCtrl.h"
#include "TangramListView.h"
//#include "VisualStudioPlus/VSAddin.h"

#include <io.h>
#include <stdio.h>

// CTangramListCtrl

IMPLEMENT_DYNAMIC(CTangramListCtrl, CListCtrl)

CTangramListCtrl::CTangramListCtrl()
{
	m_nListViewIndex = 0;
	m_nListViewSelectedIndex = 0;
	m_strDir = _T("");
	m_strSubDir = _T("");
}

CTangramListCtrl::~CTangramListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTangramListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTangramListCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CTangramListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()


// CTangramListCtrl message handlers

void CTangramListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pHubbleTabCtrl->m_pGalaxy == nullptr)
	{
		IGalaxy* pGalaxy = nullptr;
		IGalaxyCluster* _pGalaxyCluster = nullptr;
		g_pHubble->CreateGalaxyCluster((__int64)m_hWnd, &_pGalaxyCluster);
		CGalaxyCluster* pGalaxyCluster = (CGalaxyCluster*)_pGalaxyCluster;
		if (pGalaxyCluster)
		{
			auto it = pGalaxyCluster->m_mapGalaxy.find(m_pHubbleTabCtrl->m_ListCtrl.m_hWnd);
			if (it == pGalaxyCluster->m_mapGalaxy.end())
			{
				pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((__int64)m_pHubbleTabCtrl->m_ListCtrl.m_hWnd), CComBSTR("HostListView"), &pGalaxy);
				if (pGalaxy)
					m_pHubbleTabCtrl->m_pGalaxy = (CGalaxy*)pGalaxy;
			}
		}
	}
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CString strText = GetItemText(pNMLV->iItem, 0);
	CString strFile = m_strDir + m_strSubDir + _T("\\") + strText + _T(".html");
	CString strPath = g_pHubble->m_strAppCommonDocPath;
	IGrid* pGrid = nullptr;
	if (::PathFileExists(strFile))
	{
		strPath += _T("templatedetail.xml");
		m_pHubbleTabCtrl->m_pGalaxy->Observe(CComBSTR("HostListView"), CComBSTR(strPath), &pGrid);
	}
	else if (strText.CompareNoCase(_T("new Template"))==0)
	{
		strPath += _T("newtemplate.xml");
		m_pHubbleTabCtrl->m_pGalaxy->Observe(CComBSTR("newtemplate"), CComBSTR(strPath), &pGrid);
	}
	else
	{
		CString strXml = _T("<tangram><layout><grid name=\"start\" objid=\"nucleus\" /></layout></tangram>"); 
		m_pHubbleTabCtrl->m_pGalaxy->Observe(CComBSTR("defaultListView"), CComBSTR(strXml), &pGrid);
	}

	*pResult = 0;
}

void CTangramListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (g_pHubble->m_pMDIMainWnd == nullptr&&g_pHubble->m_pActiveMDIChildWnd)
	{
		if (::IsChild(g_pHubble->m_pActiveMDIChildWnd->m_hWnd, g_pHubble->m_hTemplateWnd))
		{
			RECT rc;
			::GetWindowRect(g_pHubble->m_pDocTemplateFrame->m_pWorkGrid->m_pHostWnd->m_hWnd, &rc);
			g_pHubble->m_pActiveMDIChildWnd->ScreenToClient(&rc);

			::ShowWindow(g_pHubble->m_hTemplateWnd, SW_HIDE);
			::SetParent(g_pHubble->m_hTemplateWnd, g_pHubble->m_hHostWnd);
			::ShowWindow(g_pHubble->m_pActiveMDIChildWnd->m_pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd, SW_SHOW);
			::SetWindowLongPtr(g_pHubble->m_pActiveMDIChildWnd->m_hChild,GWLP_ID, AFX_IDW_PANE_FIRST);

			::SetWindowPos(g_pHubble->m_pActiveMDIChildWnd->m_hChild, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED);
		}
		int nItem = pNMItemActivate->iItem;
		if (nItem == -1)
		{
			return;
		}

		switch (m_nListViewIndex)
		{
		case 0:
			{
				auto it = g_pHubble->m_mapHubbleDocTemplateInfo.find(m_nListViewSelectedIndex);
				if (it != g_pHubble->m_mapHubbleDocTemplateInfo.end())
				{
					CString strTemplateFile = m_strDir + m_strSubDir + _T("\\") + GetItemText(nItem, 0);// +it->second->m_strFilter.Mid(1);
					if(it->second->m_strFilter!=_T("*.*"))
						strTemplateFile += it->second->m_strFilter.Mid(1);;
					if (it->second->m_bCOMObj)
					{
						g_pHubble->StartApplication(it->second->m_strProxyID.AllocSysString(), strTemplateFile.AllocSysString());
					}
					else
					{
						IUniverseAppProxy* pProxy = nullptr;
						CString strLib = it->second->m_strProxyID;
						strLib.Trim();
						strLib.MakeLower();
						BOOL bOK = FALSE;
						if (strLib == _T("") || strLib.CompareNoCase(_T("default")) == 0)
						{
							pProxy = g_pHubble->m_pUniverseAppProxy;
						}
						auto it2 = g_pHubble->m_mapHubbleAppProxy.find(it->second->m_strProxyID);
						if (it2 == g_pHubble->m_mapHubbleAppProxy.end())
						{
							if (::LoadLibrary(it->second->m_strLib))
							{
								if (g_pHubble->m_hForegroundIdleHook == NULL)
									g_pHubble->m_hForegroundIdleHook = SetWindowsHookEx(WH_FOREGROUNDIDLE, CUniverse::ForegroundIdleProc, NULL, ::GetCurrentThreadId());
								it2 = g_pHubble->m_mapHubbleAppProxy.find(it->second->m_strProxyID);
								if (it2 != g_pHubble->m_mapHubbleAppProxy.end())
								{
									pProxy = (IUniverseAppProxy*)it2->second;
								}
							}
						}
						else
						{
							pProxy = it2->second;
						}
						if (it->second->m_strLib == _T(""))
							pProxy = g_pHubble->m_pUniverseAppProxy;

						if (pProxy&&it->second->m_strLib != _T(""))
						{
							auto it2 = g_pHubble->m_mapTemplateInfo.find(it->second->m_strExt);
							if (it2 != g_pHubble->m_mapTemplateInfo.end())
							{
								CTangramXmlParse m_Parse;
								if (m_Parse.LoadFile(strTemplateFile))
								{
									g_pHubble->m_pActiveMDIChildWnd = nullptr;
									g_pHubble->m_strCurrentFrameID = m_Parse.attr(_T("mainframeid"), _T("default"));
									CString strAppTitle = m_Parse.attr(_T("apptitle"), g_pHubble->m_strCurrentFrameID);
									g_pHubble->m_strCurrentFrameID.MakeLower();
									g_pHubble->m_strCurrentDocTemplateXml = m_Parse.xml();
									pProxy->m_strCreatingFrameTitle = strAppTitle;
									pProxy->CreateNewDocument(g_pHubble->m_strCurrentFrameID, strAppTitle, it2->second, false);
								}
							}
						}
						else
						{
							IGrid* pGrid = nullptr;
							CGalaxy* pGalaxy = m_pHubbleTabCtrl->m_pGrid->m_pGridShareData->m_pGalaxy;
							pGalaxy->Observe(pGalaxy->m_strLastKey.AllocSysString(), CComBSTR(_T("")), &pGrid);
							HubbleDocTemplateInfo* pHubbleDocTemplateInfo = it->second;
							g_pHubble->m_pHubbleDocTemplateInfo = pHubbleDocTemplateInfo;
							g_pHubble->m_strTemplatePath = strTemplateFile;
							CTangramXmlParse m_Parse;
							if (m_Parse.LoadFile(g_pHubble->m_strTemplatePath))
							{
								g_pHubble->m_strStartupCLRObj = strTemplateFile;
								g_pHubble->m_strTemplatePath = m_Parse.xml();
								//fix at 20170715
								::SendMessage(g_pHubble->m_pActiveMDIChildWnd->m_hWnd, WM_COSMOSMSG, (WPARAM)pHubbleDocTemplateInfo->m_pDocTemplate, TANGRAM_CONST_NEWDOC);
								if (g_pHubble->m_nAppType != 1965)
								{
									g_pHubble->m_pActiveMDIChildWnd->OnCreateDoc(g_pHubble->m_strTemplatePath);
									g_pHubble->m_strTemplatePath = _T("");
								}
							}
						}
						g_pHubble->m_bNewFile = FALSE;
					}
				}
			}
			break;
		}
		return;
	}
	IGrid* pGrid = nullptr;
	CGalaxy* pGalaxy = m_pHubbleTabCtrl->m_pGrid->m_pGridShareData->m_pGalaxy;
	int nItem = pNMItemActivate->iItem;
	if (nItem == -1)
	{
		::GetClassName(pGalaxy->m_hWnd, g_pHubble->m_szBuffer, MAX_PATH);
		CString strName = g_pHubble->m_szBuffer;
		CString strKey = pGalaxy->m_strLastKey;
		if (pGalaxy->m_pWebPageWnd)
		{
			if (strKey == _T(""))
			{
				strKey = _T("__default__key__for__chrome__");
			}
			pGalaxy->Observe(strKey.AllocSysString(), CComBSTR(g_pHubble->m_strDefaultXml), &pGrid);
		}
		else
		{
			pGalaxy->Observe(strKey.AllocSysString(), CComBSTR(_T("")), &pGrid);
		}
		return;
	}

	switch (m_nListViewIndex)
	{
	case 0:
	{
		HWND hWnd = pGalaxy->GetParent().m_hWnd;
		if (hWnd != g_pHubble->m_hHostWnd)
		{
			::GetClassName(pGalaxy->m_hWnd, g_pHubble->m_szBuffer, MAX_PATH);
			CString strName = g_pHubble->m_szBuffer;
			CString strKey = pGalaxy->m_strLastKey;
			if (pGalaxy->m_pWebPageWnd)
			{
				if (strKey == _T(""))
				{
					strKey = _T("__default__key__for__chrome__");
				}
				pGalaxy->Observe(strKey.AllocSysString(), CComBSTR(g_pHubble->m_strDefaultXml), &pGrid);
			}
			else
			{
				if (g_pHubble->m_strExeName.CompareNoCase(_T("excel")))
					pGalaxy->Observe(strKey.AllocSysString(), CComBSTR(_T("")), &pGrid);
			}
		}

		auto it = g_pHubble->m_mapHubbleDocTemplateInfo.find(m_nListViewSelectedIndex);
		if (it != g_pHubble->m_mapHubbleDocTemplateInfo.end())
		{
			CString strTemplateFile = m_strDir + m_strSubDir + _T("\\") + GetItemText(nItem, 0);
			if (it->second->m_strFilter != _T("*.*"))
				strTemplateFile += it->second->m_strFilter.Mid(1);
			if (it->second->m_bCOMObj)
			{
				if (!g_pHubble->m_bEclipse)
					g_pHubble->StartApplication(it->second->m_strProxyID.AllocSysString(), strTemplateFile.AllocSysString());
			}
			else
			{
				IUniverseAppProxy* pProxy = nullptr;
				CString strLib = it->second->m_strProxyID;
				strLib.Trim();
				strLib.MakeLower();
				BOOL bOK = FALSE;
				if (strLib == _T("") || strLib.CompareNoCase(_T("default")) == 0)
				{
					pProxy = g_pHubble->m_pUniverseAppProxy;
				}
				auto it2 = g_pHubble->m_mapHubbleAppProxy.find(it->second->m_strProxyID);
				if (it2 == g_pHubble->m_mapHubbleAppProxy.end())
				{
					if (::LoadLibrary(it->second->m_strLib))
					{
						if (g_pHubble->m_hForegroundIdleHook == NULL)
							g_pHubble->m_hForegroundIdleHook = SetWindowsHookEx(WH_FOREGROUNDIDLE, CUniverse::ForegroundIdleProc, NULL, ::GetCurrentThreadId());
						it2 = g_pHubble->m_mapHubbleAppProxy.find(it->second->m_strProxyID);
						if (it2 != g_pHubble->m_mapHubbleAppProxy.end())
						{
							pProxy = (IUniverseAppProxy*)it2->second;
						}
					}
				}
				else
				{
					pProxy = it2->second;
				}
				if (it->second->m_strLib == _T(""))
					pProxy = g_pHubble->m_pUniverseAppProxy;

				if (pProxy&&it->second->m_pDocTemplate==nullptr/*it->second->m_strLib != _T("")*/)
				{
					bool bCreateDoc = false;
					auto it2 = g_pHubble->m_mapTemplateInfo.find(it->second->m_strExt);
					if (it2 != g_pHubble->m_mapTemplateInfo.end())
					{
						CTangramXmlParse m_Parse;
						if (m_Parse.LoadFile(strTemplateFile))
						{
							g_pHubble->m_pActiveMDIChildWnd = nullptr;
							g_pHubble->m_strCurrentFrameID = m_Parse.attr(_T("mainframeid"), _T("default"));
							CString strAppTitle = m_Parse.attr(_T("apptitle"), g_pHubble->m_strCurrentFrameID);
							g_pHubble->m_strCurrentFrameID.MakeLower();
							g_pHubble->m_strCurrentDocTemplateXml = m_Parse.xml();
							pProxy->m_strCreatingFrameTitle = strAppTitle;
							bCreateDoc = true;
							pProxy->CreateNewDocument(g_pHubble->m_strCurrentFrameID, strAppTitle, it2->second, false);
						}
					}
					if (bCreateDoc == false)
					{
						if (it->second->m_pDocTemplate == nullptr)
						{
							CString strID = it->second->m_strProxyID;
							strID.Replace(_T("tangram"), _T(""));
							it->second->m_pDocTemplate = (void*)_wtoi64(strID);
						}
						if (it->second->m_pDocTemplate)
						{
							pProxy->OpenDocument(it->second->m_pDocTemplate, _T(""), true);
						}
					}
					//else
					//{
					//	HubbleDocTemplateInfo* pHubbleDocTemplateInfo = it->second;
					//	g_pHubble->m_pHubbleDocTemplateInfo = pHubbleDocTemplateInfo;
					//	g_pHubble->m_strTemplatePath = strTemplateFile;
					//	CTangramXmlParse m_Parse;
					//	if (m_Parse.LoadFile(g_pHubble->m_strTemplatePath))
					//	{
					//		if (g_pHubble->m_pMDIMainWnd)
					//		{
					//			::SendMessage(g_pHubble->m_pMDIMainWnd->m_hWnd, WM_QUERYAPPPROXY, (WPARAM)pHubbleDocTemplateInfo->m_pDocTemplate, TANGRAM_CONST_NEWDOC);
					//			if (g_pHubble->m_strDefaultTemplate2 == _T(""))
					//				g_pHubble->m_pMDIMainWnd->OnCreateDoc(_T(""));
					//		}
					//	}
					//}
				}
				else
				{
					HubbleDocTemplateInfo* pHubbleDocTemplateInfo = it->second;
					g_pHubble->m_pHubbleDocTemplateInfo = pHubbleDocTemplateInfo;
					g_pHubble->m_strTemplatePath = strTemplateFile;
					CTangramXmlParse m_Parse;
					if (m_Parse.LoadFile(g_pHubble->m_strTemplatePath))
					{
						if (g_pHubble->m_pMDIMainWnd)
						{
							::SendMessage(g_pHubble->m_pMDIMainWnd->m_hWnd, WM_QUERYAPPPROXY, (WPARAM)pHubbleDocTemplateInfo->m_pDocTemplate, TANGRAM_CONST_NEWDOC);
							if(g_pHubble->m_strDefaultTemplate2==_T(""))
								g_pHubble->m_pMDIMainWnd->OnCreateDoc(_T(""));
						}
					}
				}
			}
			g_pHubble->m_bNewFile = FALSE;
		}
		else
		{
			CWinForm* pWnd = nullptr;
			map<CString, HubbleDocTemplateInfo*>*	m_pMapHubbleFormsTemplateInfo = &g_pHubble->m_mapHubbleFormsTemplateInfo;
			map<int, HubbleDocTemplateInfo*>*		m_pMapHubbleFormsTemplateInfo2 = &g_pHubble->m_mapHubbleFormsTemplateInfo2;
			CString strFormsInfoPath = _T("");
			HWND hParent = NULL;
			if (m_pHubbleTabCtrl)
			{
				hParent = m_pHubbleTabCtrl->m_pGrid->m_pGridShareData->m_pGalaxy->m_pGalaxyCluster->m_hWnd;
				if (hParent)
				{
					pWnd = (CWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
					if (pWnd)
					{
						strFormsInfoPath = pWnd->m_strChildFormPath;
						//m_pMapHubbleFormsTemplateInfo = &pWnd->m_mapHubbleFormsTemplateInfo;
						//m_pMapHubbleFormsTemplateInfo2 = &pWnd->m_mapHubbleFormsTemplateInfo2;
					}
				}
			}

			CString strItem = m_pHubbleTabCtrl->m_pHubbleListView->GetListCtrl().GetItemText(m_nListViewSelectedIndex, 0);
			CString strTemplateFile = _T("");
			if (strFormsInfoPath == _T(""))
				strTemplateFile = m_strDir + m_strSubDir + _T("\\") + GetItemText(nItem, 0);
			else
				strTemplateFile = strFormsInfoPath + strItem + _T("\\") + m_strSubDir + _T("\\") + GetItemText(nItem, 0);
			auto it = m_pMapHubbleFormsTemplateInfo->find(strItem);
			if (it != m_pMapHubbleFormsTemplateInfo->end())
			{
				if (g_pHubble->m_pCLRProxy == nullptr)
					g_pHubble->LoadCLR();
				if (g_pHubble->m_pCLRProxy)
				{
					g_pHubble->m_strAppCurrentFormTemplatePath = strTemplateFile + it->second->m_strExt;
					if(strFormsInfoPath==_T(""))
						g_pHubble->m_pCLRProxy->Extend(strItem, strTemplateFile, _T("tangram:creatingform"));
					else
						g_pHubble->m_pCLRProxy->Extend(strItem, strTemplateFile, _T("tangram:creatingmdichildform"));
				}
			}
			else
			{
				auto it = m_pMapHubbleFormsTemplateInfo2->find(m_nListViewSelectedIndex);
				if (it != m_pMapHubbleFormsTemplateInfo2->end())
				{
					if (g_pHubble->m_pCLRProxy == nullptr)
						g_pHubble->LoadCLR();
					if (g_pHubble->m_pCLRProxy)
					{
						g_pHubble->m_strAppCurrentFormTemplatePath = strTemplateFile + it->second->m_strExt;
						if (::IsChild(g_pHubble->m_hHostWnd, m_hWnd))
						{
							if (pNMItemActivate->iItem == 0)
							{
								CWinForm* pWnd = (CWinForm*)g_pHubble->m_pCLRProxy->Extend(it->second->m_strProxyID, m_strDir, L"tangram:creatingform:design:new");
								if (pWnd)
								{
									CString strFile = pWnd->m_strPath;
									int nPos = strFile.ReverseFind('\\');
									CString strDir = strFile.Left(nPos + 1);
									CString strText = strFile.Mid(nPos + 1);
									nPos = strText.ReverseFind('\\');
									strText = strText.Mid(nPos + 1);
									::SHCreateDirectory(nullptr, strDir);
									CString strXml = _T("<winform></winform>");
									CTangramXmlParse m_Parse;
									m_Parse.LoadXml(strXml);
									m_Parse.SaveFile(strFile);
									int nItem = pNMItemActivate->iItem;
									LVITEM item;
									item.mask = LVFI_PARAM | LVIF_IMAGE | LVIF_TEXT;
									item.iSubItem = 0;
									item.iImage = 0;
									item.iItem = 0;
									this->GetItem(&item);
									int nImage = item.iImage;
									//InsertItem(&item);
									item.iItem = this->GetItemCount();
									item.pszText = strText.GetBuffer();
									item.iImage = nItem;
									InsertItem(&item);
								}
							}
						}
						else
						{
							if(strFormsInfoPath==_T(""))
								g_pHubble->m_pCLRProxy->Extend(it->second->m_strProxyID, strTemplateFile + it->second->m_strExt, L"tangram:creatingform");
							else
							{
								CString str = _T(""); 
								str.Format(_T("%s:%d"),it->second->m_strProxyID, hParent);
								CString strFeatures = _T("tangram:creatingmdichildform");
								if (pWnd&&pWnd->m_nState == 1)
								{
									strFeatures = _T("tangram:creatingmdichildform:design");
								}

								g_pHubble->m_pCLRProxy->Extend(str, strTemplateFile + it->second->m_strExt, strFeatures);
							}
						}
					}
				}
			}
		}
	}
	break;
	case 1:
	{
	}
	break;
	}
	*pResult = 0;
}

// CTangramTabCtrl

IMPLEMENT_DYNCREATE(CTangramTabCtrl, CTabCtrl)

CTangramTabCtrl::CTangramTabCtrl()
{
	m_nImageIndex = 0;
	m_strFilter = _T("*.xml");
	m_pGalaxy = nullptr;
	m_pGrid = nullptr;
	m_pHubbleListView = nullptr;
}

CTangramTabCtrl::~CTangramTabCtrl()
{
}

BEGIN_MESSAGE_MAP(CTangramTabCtrl, CTabCtrl)
	ON_WM_MOUSEACTIVATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchange)
	ON_MESSAGE(WM_INITIALUPDATE, OnInitialUpdate)
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

// CTangramTabCtrl message handlers

BOOL CTangramTabCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_pGrid = g_pHubble->m_pActiveGrid;
	m_pGrid->m_pHostWnd = this;
	m_pGrid->m_nViewType = TabCtrl;
	m_pGrid->m_nID = nID;
	BOOL bRet = CTabCtrl::Create(dwStyle| TCS_MULTILINE|WS_CLIPCHILDREN, rect, pParentWnd, nID);
	m_ListCtrl.Create(WS_CHILD | WS_VISIBLE| LVS_AUTOARRANGE | LVS_SINGLESEL | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, 0);
	m_ListCtrl.m_pHubbleTabCtrl = this;
	::GetModuleFileName(theApp.m_hInstance, g_pHubble->m_szBuffer, MAX_PATH);

	int width=320, heigh=90, clines=3;
	CComBSTR bstrVal("");
	m_pGrid->get_Attribute(CComBSTR("sizeandclines"), &bstrVal);
	if (!CString(bstrVal).IsEmpty())
	{
		_stscanf_s(CString(bstrVal), _T("SizeandcLines(%d,%d,%d)"), &width, &heigh, &clines);
	}

	SIZE size = { width, heigh};
	LVTILEVIEWINFO tileViewInfo = { 0 };

	tileViewInfo.cbSize = sizeof(tileViewInfo);
	tileViewInfo.dwFlags = LVTVIF_FIXEDSIZE;
	tileViewInfo.dwMask = LVTVIM_COLUMNS | LVTVIM_TILESIZE;
	tileViewInfo.cLines = clines;
	tileViewInfo.sizeTile = size;

	ListView_SetTileViewInfo(m_ListCtrl.m_hWnd, &tileViewInfo);
	ListView_SetView(m_ListCtrl.m_hWnd, LV_VIEW_TILE);
	CString strPath = g_pHubble->m_szBuffer;
	int nPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(nPos + 1) + _T("HubbleInit.dll");
	if (::PathFileExists(strPath))
	{
		CString strURL = _T("res://");
		strURL += strPath;
		strURL += _T("/images/tangrambk.png");
		m_ListCtrl.SetBkImage(LPTSTR(LPCTSTR(strURL)), TRUE);
	}

	PostMessage(WM_INITIALUPDATE);
	return bRet;
}

LRESULT CTangramTabCtrl::OnInitialUpdate(WPARAM wParam, LPARAM lParam)
{
	CComBSTR bstrKey(L"relationnode");
	m_pGrid->get_Attribute(bstrKey, &bstrKey);
	CString strKey = OLE2T(bstrKey);
	if (strKey == _T("TangramList"))
	{
		if (g_pHubble->m_DocTemplateImageList.m_hImageList == 0)
			g_pHubble->m_DocTemplateImageList.Create(32, 32, ILC_COLOR32, 0, 4);
		m_ListCtrl.SetImageList(&g_pHubble->m_DocTemplateImageList, LVSIL_NORMAL);
	}
	if (strKey == _T("TangramListCLR"))
	{
	}
	CComPtr<IGridCollection> pColl;
	CGrid* pGrid = m_pGrid->m_pRootObj;
	IGrid* _pGrid = nullptr;
	long nCount = 0;
	pGrid->GetGrids(bstrKey, &_pGrid, &pColl, &nCount);
	if (_pGrid)
	{
		CGridCollection* pWndGridCollection = (CGridCollection*)pColl.p;

		for (auto it : *pWndGridCollection->m_pGrids)
		{
			CTangramListView* pWnd = (CTangramListView*)(it)->m_pHostWnd;
			pWnd->m_pHubbleTabCtrl = this;
			pWnd->InitTabCtrl(this);
		}
	}

	return CWnd::DefWindowProc(WM_INITIALUPDATE, wParam, lParam);
}

void CTangramTabCtrl::PostNcDestroy()
{
	if (m_pGrid)
		delete m_pGrid;

	CTabCtrl::PostNcDestroy();
	delete this;
}

int CTangramTabCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pHubble->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = g_pHubble->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	g_pHubble->m_pActiveGrid = m_pGrid;
	g_pHubble->m_bWinFormActived = false;
	return MA_ACTIVATE;
}

void CTangramTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	FillListCtrl();
	*pResult = 0;
}

void CTangramTabCtrl::RePosition()
{
	if (::IsWindow(m_ListCtrl.m_hWnd))
	{
		RECT  rc;
		GetWindowRect(&rc);
		AdjustRect(false, &rc);
		ScreenToClient(&rc);
		rc.top += 2;
		::SetWindowPos(m_ListCtrl.m_hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE| SWP_NOREDRAW);
	}
}

void CTangramTabCtrl::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CTabCtrl::OnWindowPosChanged(lpwndpos);
	RePosition();
}

void CTangramTabCtrl::FillListCtrl()
{
	m_ListCtrl.DeleteAllItems();
	TCITEM TabCtrlItem;
	TabCtrlItem.pszText = g_pHubble->m_szBuffer;
	TabCtrlItem.mask |= TCIF_TEXT;
	TabCtrlItem.cchTextMax = 256;
	int index = GetCurSel();
	GetItem(index, &TabCtrlItem);
	CString strItem = TabCtrlItem.pszText;
	m_ListCtrl.m_strSubDir = strItem;
	CString m_strDocTemplatePath = m_ListCtrl.m_strDir + strItem + _T("\\") + m_strFilter;

	LVITEM item;
	item.mask = LVFI_PARAM | LVIF_IMAGE | LVIF_TEXT;
	item.iSubItem = 0;
	item.iImage = m_nImageIndex;
	int nItem = 0;

	// Create `New Template` icon when tab control belong to designer window.
	if (::IsChild(g_pHubble->m_hHostWnd, m_hWnd))
	{
		item.iItem = nItem;
		item.pszText = L"New Template";
		item.lParam = nItem;
		m_ListCtrl.InsertItem(&item);
		nItem++;
	}

	_wfinddata_t fd;
	fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
	intptr_t pf = _wfindfirst(m_strDocTemplatePath, &fd);
	while (pf!=-1)
	{
		if ((fd.attrib&FILE_ATTRIBUTE_DIRECTORY)==0)
		{
			CString str = fd.name;
			if (str != _T(".."))
			{
				item.iItem = nItem;
				if(m_strFilter!=_T("*.*"))
					str = str.Left(str.ReverseFind('.'));
				item.pszText = str.GetBuffer();
				item.lParam = nItem;
				m_ListCtrl.InsertItem(&item);
				nItem++;
			}
		}
		if (_wfindnext(pf, &fd) == -1)
			break;
	}
	if (nItem)
	{
		m_ListCtrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	_findclose(pf);
	RePosition();
	::InvalidateRect(m_hWnd, nullptr, true);
	CString strPath = g_pHubble->m_strAppCommonDocPath + _T("templatedetail.xml");
	if (::PathFileExists(strPath))
	{
		IGrid* pGrid = nullptr;
		CString strTemplateFile = m_ListCtrl.m_strDir + m_ListCtrl.m_strSubDir + _T("\\") + m_ListCtrl.GetItemText(0, 0)+ _T(".html");
		if (::PathFileExists(strTemplateFile))
		{
			m_pGalaxy->Observe(CComBSTR("HostListView"), CComBSTR(strPath), &pGrid);
		}
		else
		{
			strPath = g_pHubble->m_strAppCommonDocPath + _T("default.xml");
			if(m_pGalaxy)
				m_pGalaxy->Observe(CComBSTR("default"), CComBSTR(strPath), &pGrid);
		}
	}
}

