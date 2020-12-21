/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
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

// TangramListView.cpp : implementation file
//

#include "stdafx.h"
#include "TangramListView.h"
#include "UniverseApp.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "TangramTabCtrl.h"
#include <io.h>
#include <stdio.h>
//#include "VisualStudioPlus/VSAddin.h"

// CTangramListView

IMPLEMENT_DYNCREATE(CTangramListView, CListView)

CTangramListView::CTangramListView()
{
	m_bCreated = FALSE;
	m_bFilling = FALSE;
	m_nStyle = 0;
	m_nActiveIndex = 0;
	m_pGalaxy = nullptr;
	m_pXobj = nullptr;
	m_pCosmosTabCtrl = nullptr;
}

CTangramListView::~CTangramListView()
{
	if (m_pXobj)
		delete m_pXobj;
}

BEGIN_MESSAGE_MAP(CTangramListView, CListView)
	ON_WM_MOUSEACTIVATE()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CTangramListView::OnLvnItemchanged)
	ON_MESSAGE(WM_HUBBLE_ACTIVEPAGE, OnActiveTangramPage)
END_MESSAGE_MAP()


// CTangramListView diagnostics

#ifdef _DEBUG
void CTangramListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CTangramListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG

// CTangramListView message handlers

void CTangramListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
}

void CTangramListView::InitTabCtrl(CTangramTabCtrl* pTabCtrl)
{
	m_pCosmosTabCtrl = pTabCtrl;
	CComBSTR bstrStyle(L"");
	m_pXobj->get_Attribute(CComBSTR(L"style"), &bstrStyle);
	m_nStyle = _wtoi(OLE2T(bstrStyle));
	CImageList* pImageList = GetListCtrl().GetImageList(LVSIL_NORMAL);
	if (pImageList)
	{
	}
	else
	{
		LVITEM item;
		item.mask = LVFI_PARAM | LVIF_IMAGE | LVIF_TEXT;
		switch (m_nStyle)
		{
		case 0:
		{
			map<CString, CosmosDocTemplateInfo*>*	m_pMapCosmosFormsTemplateInfo = &g_pCosmos->m_mapCosmosFormsTemplateInfo;
			map<int, CosmosDocTemplateInfo*>*		m_pMapCosmosFormsTemplateInfo2 = &g_pCosmos->m_mapCosmosFormsTemplateInfo2;
			CWinForm* pWnd = nullptr;
			CString strFormsInfoPath = g_pCosmos->m_strAppFormsInfoPath;
			if (m_pCosmosTabCtrl)
			{
				HWND hWnd = m_pCosmosTabCtrl->m_pXobj->m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster->m_hWnd;
				if (hWnd)
				{
					//pWnd = (CWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
					//if (pWnd)
					//{
					//	strFormsInfoPath = pWnd->m_strChildFormPath;
					//	m_pMapCosmosFormsTemplateInfo = &pWnd->m_mapCosmosFormsTemplateInfo;
					//	m_pMapCosmosFormsTemplateInfo2 = &pWnd->m_mapCosmosFormsTemplateInfo2;
					//}
				}
			}
			if (g_pCosmos->m_mapCosmosDocTemplateInfo.size() == 0)
				g_pCosmos->InitCosmosDocManager();

			GetListCtrl().SetImageList(&g_pCosmos->m_DocImageList, LVSIL_NORMAL);
			int nCount = g_pCosmos->m_mapCosmosDocTemplateInfo.size();
			for (auto it = g_pCosmos->m_mapCosmosDocTemplateInfo.begin(); it != g_pCosmos->m_mapCosmosDocTemplateInfo.end(); it++)
			{
				item.iImage = it->second->m_nImageIndex;
				item.iItem = it->second->m_nImageIndex;
				item.iSubItem = 0;
				CString s = it->second->m_strDocTemplateKey;
				if (g_pCosmos->m_bUsingDefaultAppDocTemplate&&it->second->m_strTemplatePath.Find(g_pCosmos->m_strAppCommonDocPath + _T("CommonMFCAppTemplate")) == 0)
				{
					s = _T("Common DocTemplate");
				}
				s.Replace(_T("."), _T(" "));
				item.pszText = (LPTSTR)LPCTSTR(s);
				int nItem = GetListCtrl().InsertItem(&item);
			}
			_wfinddata64_t fd;
			intptr_t fp = _wfindfirst64(strFormsInfoPath + L"*.ico", &fd);
			if (g_pCosmos->m_bCLRObjTemplateInit == false||pWnd!=nullptr)
			{
				while (fp != -1)
				{
					if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0)
					{
						HICON hIcon = (HICON)::LoadImage(NULL, strFormsInfoPath + fd.name, IMAGE_ICON, 48, 48, LR_LOADFROMFILE | LR_DEFAULTCOLOR);
						item.iImage = g_pCosmos->m_DocImageList.Add(hIcon);;
						g_pCosmos->m_DocTemplateImageList.Add(hIcon);
						item.iItem = item.iImage;
						CString s = fd.name;
						int nPos = s.Replace(_T(".ico"), _T(""));
						CString strType = s;
						nPos = s.Find(_T(","));
						CString strText = s.Left(nPos);
						s = s.Mid(nPos + 1);
						nPos = s.Find(_T(","));
						strText += s.Left(nPos);
						item.pszText = (LPTSTR)LPCTSTR(strText);
						CosmosDocTemplateInfo* pInfo = nullptr;
						auto it = m_pMapCosmosFormsTemplateInfo->find(strType);
						if (it == m_pMapCosmosFormsTemplateInfo->end())
						{
							pInfo = new CosmosDocTemplateInfo();
							pInfo->m_nImageIndex = item.iImage;// item.iItem;
							pInfo->m_strExt = _T(".formxml");
							pInfo->m_strFilter = _T("*.formxml");
							pInfo->m_strProxyID = strType;
							(*m_pMapCosmosFormsTemplateInfo)[strType] = pInfo;
						}
						(*m_pMapCosmosFormsTemplateInfo2)[GetListCtrl().InsertItem(&item)] = pInfo;
					}
					if (_wfindnext64(fp, &fd) == -1)
					{
						break;
					}
				}
				if(pWnd==nullptr)
					g_pCosmos->m_bCLRObjTemplateInit = true;
			}
			else
			{
				while (fp != -1)
				{
					if ((fd.attrib & FILE_ATTRIBUTE_DIRECTORY) == 0)
					{
						CString s = fd.name;
						int nPos = s.Replace(_T(".ico"), _T(""));
						CString strType = s;
						nPos = s.Find(_T(","));
						CString strText = s.Left(nPos);
						s = s.Mid(nPos + 1);
						nPos = s.Find(_T(","));
						strText += s.Left(nPos);
						item.pszText = (LPTSTR)LPCTSTR(strText);
						auto it = m_pMapCosmosFormsTemplateInfo->find(strType);
						if (it != m_pMapCosmosFormsTemplateInfo->end())
						{
							item.iItem = item.iImage = it->second->m_nImageIndex;
							GetListCtrl().InsertItem(&item);
						}
						else
						{
							HICON hIcon = (HICON)::LoadImage(NULL, strFormsInfoPath + fd.name, IMAGE_ICON, 48, 48, LR_LOADFROMFILE | LR_DEFAULTCOLOR);
							item.iImage = g_pCosmos->m_DocImageList.Add(hIcon);;
							g_pCosmos->m_DocTemplateImageList.Add(hIcon);
							CosmosDocTemplateInfo* pInfo = nullptr;
							pInfo = new CosmosDocTemplateInfo();
							pInfo->m_nImageIndex = item.iImage;// item.iItem;
							pInfo->m_strExt = _T(".formxml");
							pInfo->m_strFilter = _T("*.formxml");
							pInfo->m_strProxyID = strType;
							(*m_pMapCosmosFormsTemplateInfo)[strType] = pInfo;
							(*m_pMapCosmosFormsTemplateInfo2)[GetListCtrl().InsertItem(&item)] = pInfo;
						}
					}
					if (_wfindnext64(fp, &fd) == -1)
					{
						break;
					}
				}
			}
		}
		break;
		case 1:
		{
			if (g_pCosmos->m_nAppID != 9)
			{
				break;
			}
		}
		break;
		default:
			break;
		}
		m_bCreated = TRUE;
	}
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	int nIndex = 0;
	switch (m_nStyle)
	{
	case 0:
		{
			GetListCtrl().SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			TCHAR	szBuffer[MAX_PATH];
			::GetModuleFileName(nullptr, szBuffer, MAX_PATH);
			CString strPath = CString(szBuffer);

			auto it = g_pCosmos->m_mapCosmosDocTemplateInfo.find(0);
			if (it != g_pCosmos->m_mapCosmosDocTemplateInfo.end())
			{
				m_pCosmosTabCtrl->m_nImageIndex = it->second->m_nImageIndex;
				m_pCosmosTabCtrl->m_ListCtrl.m_strDir = it->second->m_strTemplatePath;
				CString m_strDocTemplatePath = m_pCosmosTabCtrl->m_ListCtrl.m_strDir;
				CString strKey = it->second->m_strDocTemplateKey;
				if (::PathIsDirectory(m_strDocTemplatePath) == false)
				{
					if (::SHCreateDirectoryEx(NULL, m_strDocTemplatePath, NULL))
					{
						ATLTRACE(L"CreateDirectory failed (%d)\n", GetLastError());
						break;
					}
					int nPos = strKey.ReverseFind('.');
					CString strTitle = strKey.Left(nPos);
					CString strPath = m_strDocTemplatePath + strTitle;
					if (::SHCreateDirectoryEx(NULL, strPath, NULL))
					{
						ATLTRACE(L"CreateDirectory failed (%d)\n", GetLastError());
						break;
					}
					else
					{
						CString strXml = _T("");
						strXml.Format(_T("<%s mainframeid='defaultmainframe' apptitle='%s' />"), strKey, strTitle);
						CTangramXmlParse m_Parse;
						m_Parse.LoadXml(strXml);
						m_Parse.SaveFile(strPath + _T("\\default.xml"));
					}
				}
				m_strDocTemplatePath += _T("*.*");

				CString strCaption = _T("");
				CXobj* pXobj = m_pCosmosTabCtrl->m_pXobj;
				if (pXobj)
				{
					strCaption = _T("Current Selected Document Template: ");
					strCaption += it->second->m_strDocTemplateKey;
					pXobj->put_Caption(strCaption.AllocSysString());
				}
				strCaption = _T("Create New Tangram Document");
				m_pXobj->put_Caption(strCaption.AllocSysString());
				_wfinddata_t fd;
				fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
				intptr_t pf = _wfindfirst(m_strDocTemplatePath, &fd);
				while (!_wfindnext(pf, &fd))
				{
					if (fd.attrib&FILE_ATTRIBUTE_DIRECTORY)
					{
						CString str = fd.name;
						if (str != _T(".."))
						{
							tcItem.pszText = LPWSTR(LPCWSTR(str));
							CString strDocTemplatePath = m_pCosmosTabCtrl->m_ListCtrl.m_strDir + str + _T("\\") + it->second->m_strFilter;

							_wfinddata_t fd;
							fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
							int nItem = 0;
							intptr_t pf = _wfindfirst(strDocTemplatePath, &fd);
							while (pf != -1)
							{
								if ((fd.attrib&FILE_ATTRIBUTE_DIRECTORY) == 0)
								{
									CString str = fd.name;
									if (str != _T(".."))
									{
										nItem++;
										break;
									}
								}
								int nVal = _wfindnext(pf, &fd);
								ATLTRACE(_T("_wfindnext:%d\n"), nVal);
								if (nVal == -1)
									break;
							}

							if (nItem)
								m_pCosmosTabCtrl->InsertItem(nIndex, &tcItem);

							nIndex++;
						}
					}
				}
				_findclose(pf);
				m_pCosmosTabCtrl->m_strFilter = it->second->m_strFilter;
				m_pCosmosTabCtrl->FillListCtrl();
			}
		}
		break;
	case 1:
		{
			if (g_pCosmos->m_nAppID != 9)
			{
				break;
			}
		}
		break;
	}
}

void CTangramListView::ChangeTemplate(int nItem)
{
	if (m_bCreated == FALSE)
		return;
	if (m_pCosmosTabCtrl == nullptr)
		return;
	if (m_pCosmosTabCtrl->m_ListCtrl.m_strDir == _T(""))
		return;
	switch (m_nStyle)
	{
	case 0:
		{
			auto it = g_pCosmos->m_mapCosmosDocTemplateInfo.find(nItem);
			if (it != g_pCosmos->m_mapCosmosDocTemplateInfo.end())
			{
				CString strDir = g_pCosmos->m_strAppCommonDocPath + it->second->m_strProxyID + _T("\\");
				if (m_nActiveIndex != nItem|| m_pCosmosTabCtrl->m_ListCtrl.m_strDir != strDir)
				{
					m_pCosmosTabCtrl->m_ListCtrl.m_nListViewSelectedIndex = nItem;
					m_pCosmosTabCtrl->DeleteAllItems();
					m_pCosmosTabCtrl->m_strFilter = it->second->m_strFilter;
					m_pCosmosTabCtrl->m_nImageIndex = it->second->m_nImageIndex;
					CString strKey = it->second->m_strDocTemplateKey;
					CString strCaption = _T("");
					CXobj* pXobj = m_pCosmosTabCtrl->m_pXobj;
					if (pXobj)
					{
						strCaption = _T("Current Selected Document Template: ");
						strCaption += it->second->m_strDocTemplateKey;
						pXobj->put_Caption(strCaption.AllocSysString());
					}
					strCaption = _T("Create New Tangram Document");
					TCITEM tcItem;
					tcItem.mask = TCIF_TEXT;
					int nIndex = 0;
					m_pCosmosTabCtrl->m_ListCtrl.m_strDir = it->second->m_strTemplatePath;
					CString m_strDocTemplatePath = m_pCosmosTabCtrl->m_ListCtrl.m_strDir;
					if (::PathIsDirectory(m_strDocTemplatePath) == false)
					{
						if (::SHCreateDirectoryEx(NULL, m_strDocTemplatePath, NULL))
						{
							ATLTRACE(L"CreateDirectory failed (%d)\n", GetLastError());
							break;
						}
						int nPos = strKey.ReverseFind('.');
						CString strTitle = strKey.Left(nPos);
						CString strPath = m_strDocTemplatePath + strTitle;
						if (::SHCreateDirectoryEx(NULL, strPath, NULL))
						{
							ATLTRACE(L"CreateDirectory failed (%d)\n", GetLastError());
							break;
						}
						else
						{
							CString strXml = _T("");
							strXml.Format(_T("<%s mainframeid='defaultmainframe' apptitle='%s' />"), strKey, strTitle);
							CTangramXmlParse m_Parse;
							m_Parse.LoadXml(strXml);
							m_Parse.SaveFile(strPath+_T("\\default.xml"));
						}
					}
					m_strDocTemplatePath += _T("*.*");
					_wfinddata_t fd;
					fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
					intptr_t pf = _wfindfirst(m_strDocTemplatePath, &fd);
					if (pf != -1)
					{
						while (!_wfindnext(pf, &fd))
						{
							if (fd.attrib&FILE_ATTRIBUTE_DIRECTORY)
							{
								CString str = fd.name;
								if (str != _T(".."))
								{
									tcItem.pszText = LPWSTR(LPCWSTR(str));
									CString strDocTemplatePath = m_pCosmosTabCtrl->m_ListCtrl.m_strDir + str + _T("\\") + it->second->m_strFilter;

									_wfinddata_t fd;
									fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
									int nItem = 0;
									intptr_t pf = _wfindfirst(strDocTemplatePath, &fd);
									while (pf != -1)
									{
										if ((fd.attrib&FILE_ATTRIBUTE_DIRECTORY) == 0)
										{
											CString str = fd.name;
											if (str != _T(".."))
											{
												nItem++;
												break;
											}
										}
										int nVal = _wfindnext(pf, &fd);
										ATLTRACE(_T("_wfindnext:%d\n"), nVal);
										if (nVal == -1)
											break;
									}

									if(nItem)
										m_pCosmosTabCtrl->InsertItem(nIndex, &tcItem);
									nIndex++;
								}
							}
						}
						_findclose(pf);
						m_nActiveIndex = nItem;
						m_pCosmosTabCtrl->FillListCtrl();
					}
				}
			}
			else
			{
				map<CString, CosmosDocTemplateInfo*>*	m_pMapCosmosFormsTemplateInfo = &g_pCosmos->m_mapCosmosFormsTemplateInfo;
				map<int, CosmosDocTemplateInfo*>*		m_pMapCosmosFormsTemplateInfo2 = &g_pCosmos->m_mapCosmosFormsTemplateInfo2;
				CWinForm* pWnd = nullptr;
				if (m_pCosmosTabCtrl)
				{
					HWND hWnd = m_pCosmosTabCtrl->m_pXobj->m_pXobjShareData->m_pGalaxy->m_pGalaxyCluster->m_hWnd;
					if (hWnd)
					{
						//pWnd = (CWinForm*)::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 20190214);
						//if (pWnd)
						//{
						//	m_pMapCosmosFormsTemplateInfo = &pWnd->m_mapCosmosFormsTemplateInfo;
						//	m_pMapCosmosFormsTemplateInfo2 = &pWnd->m_mapCosmosFormsTemplateInfo2;
						//}
					}
				}
				CString strText = GetListCtrl().GetItemText(nItem, 0);
				auto it2 = m_pMapCosmosFormsTemplateInfo2->find(nItem);
				if (it2 != m_pMapCosmosFormsTemplateInfo2->end())
				{
					CString strDir = g_pCosmos->m_strAppFormsTemplatePath;
					if (pWnd)
					{
						strDir = pWnd->m_strChildFormPath;
					}

					if (m_nActiveIndex != nItem || m_pCosmosTabCtrl->m_ListCtrl.m_strDir != strDir)
					{
						m_pCosmosTabCtrl->m_ListCtrl.m_nListViewSelectedIndex = nItem;
						m_pCosmosTabCtrl->DeleteAllItems();
						m_pCosmosTabCtrl->m_strFilter = it2->second->m_strFilter;
						m_pCosmosTabCtrl->m_nImageIndex = it2->second->m_nImageIndex;
						TCITEM tcItem;
						tcItem.mask = TCIF_TEXT;
						int nIndex = 0;
						m_pCosmosTabCtrl->m_ListCtrl.m_strDir = strDir + strText + _T("\\");
						CString m_strDocTemplatePath = m_pCosmosTabCtrl->m_ListCtrl.m_strDir;
						CString strKey = it2->second->m_strDocTemplateKey;
						CString strCaption = _T("");
						CXobj* pXobj = m_pCosmosTabCtrl->m_pXobj;
						if (pXobj)
						{
							strCaption = _T("Current Selected Document Template: ");
							strCaption += it2->second->m_strDocTemplateKey;
							pXobj->put_Caption(strCaption.AllocSysString());
						}
						strCaption = _T("Create New Tangram Document");
						tcItem.mask = TCIF_TEXT;
						//m_pCosmosTabCtrl->m_ListCtrl.m_strDir = it2->second->m_strTemplatePath;
						if (::PathIsDirectory(m_strDocTemplatePath) == false)
						{
							if (::SHCreateDirectoryEx(NULL, m_strDocTemplatePath, NULL))
							{
								ATLTRACE(L"CreateDirectory failed (%d)\n", GetLastError());
								break;
							}
							int nPos = strKey.ReverseFind('.');
							CString strTitle = strKey.Left(nPos);
							CString strPath = m_strDocTemplatePath + strTitle;
							if (::SHCreateDirectoryEx(NULL, strPath, NULL))
							{
								ATLTRACE(L"CreateDirectory failed (%d)\n", GetLastError());
								break;
							}
							else
							{
								CString strXml = _T("");
								strXml.Format(_T("<%s mainframeid='defaultmainframe' apptitle='%s' />"), strKey, strTitle);
								CTangramXmlParse m_Parse;
								m_Parse.LoadXml(strXml);
								m_Parse.SaveFile(strPath + _T("\\default.xml"));
							}
						}
						m_strDocTemplatePath += _T("*.*");
						_wfinddata_t fd;
						fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
						intptr_t pf = _wfindfirst(m_strDocTemplatePath, &fd);
						if (pf != -1)
						{
							while (!_wfindnext(pf, &fd))
							{
								if (fd.attrib&FILE_ATTRIBUTE_DIRECTORY)
								{
									CString str = fd.name;
									if (str != _T(".."))
									{
										tcItem.pszText = LPWSTR(LPCWSTR(str));
										CString strDocTemplatePath = m_pCosmosTabCtrl->m_ListCtrl.m_strDir + str + _T("\\") + it2->second->m_strFilter;

										_wfinddata_t fd;
										fd.attrib = FILE_ATTRIBUTE_DIRECTORY;
										int nItem = 0;
										intptr_t pf = _wfindfirst(strDocTemplatePath, &fd);
										while (pf != -1)
										{
											if ((fd.attrib&FILE_ATTRIBUTE_DIRECTORY) == 0)
											{
												CString str = fd.name;
												if (str != _T(".."))
												{
													nItem++;
													break;
												}
											}
											int nVal = _wfindnext(pf, &fd);
											ATLTRACE(_T("_wfindnext:%d\n"), nVal);
											if (nVal == -1)
												break;
										}

										if (nItem)
											m_pCosmosTabCtrl->InsertItem(nIndex, &tcItem);
										nIndex++;
									}
								}
							}
							_findclose(pf);
							m_nActiveIndex = nItem;
							m_pCosmosTabCtrl->m_pCosmosListView = this;
							m_pCosmosTabCtrl->FillListCtrl();
						}
					}
				}
			}
			m_pCosmosTabCtrl->RePosition();
		}
		break;
	case 1:
		{
			if (g_pCosmos->m_nAppID != 9)
			{
				break;
			}
		}
		break;
	}
}

BOOL CTangramListView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_pXobj = g_pCosmos->m_pActiveXobj;
	m_pXobj->m_pHostWnd = this;
	m_pXobj->m_nViewType = TangramListView;
	m_pXobj->m_nID = nID;
	CString strURL = _T("");
	BOOL bRet = CListView::Create(lpszClassName, lpszWindowName, dwStyle| LVS_ICON| LVS_AUTOARRANGE| LVS_SINGLESEL| LVS_SHAREIMAGELISTS| LVS_SHOWSELALWAYS, rect, pParentWnd, nID, pContext);
	if (::FindResource(NULL, _T("TANGRAMBK.PNG"), _T("IMAGES")) == nullptr)
	{
		::GetModuleFileName(theApp.m_hInstance, g_pCosmos->m_szBuffer, MAX_PATH);
		CString strPath = g_pCosmos->m_szBuffer;
		int nPos = strPath.ReverseFind('\\');
		strPath = strPath.Left(nPos + 1) + _T("CosmosInit.dll");
		//CString strPath = g_pCosmos->m_strProgramFilePath;
		//strPath += _T("\\tangram\\CosmosInit.dll");
		if (::PathFileExists(strPath))
		{
			strURL = _T("res://");
			strURL += strPath;
			strURL += _T("/images/tangrambk.png");
		}
	}
	else
	{
		::GetModuleFileName(NULL, g_pCosmos->m_szBuffer, MAX_PATH);
		strURL = _T("res://") + CString(g_pCosmos->m_szBuffer) + _T("/images/tangrambk.png");
	}
	SendMessage(WM_INITIALUPDATE);
	if(strURL!=_T(""))
		GetListCtrl().SetBkImage(LPTSTR(LPCTSTR(strURL)), TRUE);

	int width = 320, heigh = 90, clines = 3;
	CComBSTR bstrVal("");
	m_pXobj->get_Attribute(CComBSTR("sizeandclines"), &bstrVal);
	if (!CString(bstrVal).IsEmpty())
	{
		_stscanf_s(CString(bstrVal), _T("SizeandcLines(%d,%d,%d)"), &width, &heigh, &clines);
	}
	SIZE size = { width, heigh };
	LVTILEVIEWINFO tileViewInfo = { 0 };

	tileViewInfo.cbSize = sizeof(tileViewInfo);
	tileViewInfo.dwFlags = LVTVIF_FIXEDSIZE;
	tileViewInfo.dwMask = LVTVIM_COLUMNS | LVTVIM_TILESIZE;
	tileViewInfo.cLines = clines;
	tileViewInfo.sizeTile = size;

	ListView_SetTileViewInfo(GetListCtrl().m_hWnd, &tileViewInfo);
	ListView_SetView(GetListCtrl().m_hWnd, LV_VIEW_TILE);
	return bRet;
}

int CTangramListView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (g_pCosmos->m_pUniverseAppProxy)
	{
		HWND hMenuWnd = g_pCosmos->m_pUniverseAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}
	g_pCosmos->m_pActiveXobj = m_pXobj;
	g_pCosmos->m_bWinFormActived = false;
	return MA_ACTIVATE;
}

void CTangramListView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bCreated == FALSE)
		return;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nItem = pNMLV->iItem;
	ChangeTemplate(nItem);
	*pResult = 0;
}

LRESULT CTangramListView::OnActiveTangramPage(WPARAM wParam, LPARAM lParam)
{
	if (m_pCosmosTabCtrl)
	{
		int nCount = GetListCtrl().GetSelectedCount();
		POSITION nPos = GetListCtrl().GetFirstSelectedItemPosition();
		int nIndex = GetListCtrl().GetNextSelectedItem(nPos);
		GetListCtrl().SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		m_pCosmosTabCtrl->m_ListCtrl.m_nListViewIndex = wParam;
		switch (wParam)
		{
		case 0:
			{
				int width = 320, heigh = 90, clines = 3;
				CComBSTR bstrVal("");
				m_pCosmosTabCtrl->m_pXobj->get_Attribute(CComBSTR("sizeandclines"), &bstrVal);
				if (!CString(bstrVal).IsEmpty())
				{
					_stscanf_s(CString(bstrVal), _T("SizeandcLines(%d,%d,%d)"), &width, &heigh, &clines);
				}
				m_pCosmosTabCtrl->m_ListCtrl.SetImageList(&g_pCosmos->m_DocTemplateImageList, LVSIL_NORMAL);
				SIZE size = { width, heigh };
				LVTILEVIEWINFO tileViewInfo = { 0 };

				tileViewInfo.cbSize = sizeof(tileViewInfo);
				tileViewInfo.dwFlags = LVTVIF_FIXEDSIZE;
				tileViewInfo.dwMask = LVTVIM_COLUMNS | LVTVIM_TILESIZE;
				tileViewInfo.cLines = clines;
				tileViewInfo.sizeTile = size;

				ListView_SetTileViewInfo(m_pCosmosTabCtrl->m_ListCtrl.m_hWnd, &tileViewInfo);
				ListView_SetView(m_pCosmosTabCtrl->m_ListCtrl.m_hWnd, LV_VIEW_TILE);
				auto it = g_pCosmos->m_mapCosmosDocTemplateInfo.find(nIndex);
				if (it != g_pCosmos->m_mapCosmosDocTemplateInfo.end())
				{
					ChangeTemplate(nIndex);
				}
			}
			break;
		case 1:
			{
				if (g_pCosmos->m_nAppID != 9)
				{
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	return CWnd::DefWindowProc(WM_TABCHANGE, wParam, lParam);
}
