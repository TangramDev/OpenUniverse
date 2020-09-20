/********************************************************************************
*					Open Universe - version 0.9.5								*
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

// Quasar.cpp : implementation file of CQuasar
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "Hubble.h"
#include "GridWnd.h"
#include "GridHelperWnd.h"
#include "grid.h"
#include "Quasar.h"

CWinForm::CWinForm(void)
{
	m_nState = -1;
	m_bMdiForm = false;
	m_pOwnerHtmlWnd = nullptr;
	m_pParentHtmlWnd = nullptr;
	m_strChildFormPath = m_strXml = m_strKey = m_strBKID = _T("");
	m_pChildFormsInfo = nullptr;
}

CWinForm::~CWinForm(void)
{
	if (m_pChildFormsInfo)
		delete m_pChildFormsInfo;
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
	switch (m_nState)
	{
	case 0:
	case 1:
	case 3:
	{
		//if (!::PathFileExists(m_strPath))
		//{
		//	int nPos = m_strPath.ReverseFind('\\');
		//	CString strPath = m_strPath.Left(nPos);
		//	::SHCreateDirectory(nullptr, strPath);
		//}
		//CGalaxyCluster* pGalaxyCluster = nullptr;
		//auto it = g_pHubble->m_mapWindowPage.find(m_hWnd);
		//if (it != g_pHubble->m_mapWindowPage.end())
		//{
		//	pGalaxyCluster = (CGalaxyCluster*)it->second;
		//	CString strData = _T("<winform>");
		//	CString strIndex = _T("@");
		//	for (auto it2 : pGalaxyCluster->m_mapQuasar)
		//	{
		//		CComBSTR bstrXml(L"");
		//		strIndex += it2.second->m_strQuasarName;
		//		strIndex += _T("@");
		//		it2.second->get_QuasarXML(&bstrXml);
		//		strData += OLE2T(bstrXml);
		//	}
		//	DWORD dw = ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
		//	if (dw & WS_EX_MDICHILD)
		//	{
		//		HWND h = ::GetParent(::GetParent(m_hWnd));
		//		if (h)
		//		{
		//			CWinForm* pParent = (CWinForm*)::SendMessage(h, WM_TANGRAMDATA, 0, 20190214);
		//			if (pParent)
		//			{
		//				auto it = pParent->m_mapKey.find(m_strKey);
		//				if (it != pParent->m_mapKey.end())
		//				{
		//					strData += it->second;
		//				}
		//			}
		//		}
		//	}
		//	strData += _T("</winform>");
		//	CTangramXmlParse xml;
		//	if (xml.LoadXml(strData))
		//		xml.SaveFile(m_strPath);
		//	// TODO Refresh ListCtrl
		//}
	}
	break;
	}
	if (g_pHubble->m_pActiveWinFormWnd == this)
		g_pHubble->m_pActiveWinFormWnd = nullptr;
	if (m_pParentHtmlWnd)
	{
		auto it = m_pParentHtmlWnd->m_mapSubWinForm.find(m_hWnd);
		if (it != m_pParentHtmlWnd->m_mapSubWinForm.end())
		{
			m_pParentHtmlWnd->m_mapSubWinForm.erase(it);
		}
	}
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
				//HWND h = ::GetParent(::GetParent(m_hWnd));
				//if (h)
				//{
				//	CWinForm* pParent = (CWinForm*)::SendMessage(h, WM_TANGRAMDATA, 0, 20190214);
				//	if (pParent)
				//	{
				//		auto it = pParent->m_mapKey.find(m_strKey);
				//		if (it == pParent->m_mapKey.end())
				//		{
				//			CTangramXmlParse m_Parse;
				//			if (m_Parse.LoadFile(m_strPath))
				//			{
				//				CTangramXmlParse* pChild = m_Parse.GetChild(m_strKey);
				//				if (pChild)
				//					pParent->m_mapKey[m_strKey] = pChild->xml();
				//			}
				//		}
				//	}
				//}
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
				HWND hWnd = pWndGrid->m_pGridCommonData->m_pQuasar->m_hWnd;
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
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	//auto it = g_pHubble->m_mapMainForm.find(m_hWnd);
	//if (it != g_pHubble->m_mapMainForm.end())
	//{
	//	g_pHubble->m_mapMainForm.erase(it);
	//	if (m_hWnd == g_pHubble->m_hMainWnd)
	//	{
	//		if (g_pHubble->m_mapMainForm.size())
	//		{
	//			it = g_pHubble->m_mapMainForm.begin();
	//			if (it != g_pHubble->m_mapMainForm.end())
	//				g_pHubble->m_hMainWnd = it->first;
	//		}
	//	}
	//}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnHubbleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20200216:
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

			CQuasar* pQuasar = nullptr;
			auto iter = pThreadInfo->m_mapQuasar.find(hWnd);
			if (iter != pThreadInfo->m_mapQuasar.end())
			{
				pQuasar = (CQuasar*)iter->second;
			}
			if (pQuasar->m_pHostWebBrowserWnd)
			{
				HWND hWnd = pQuasar->m_pHostWebBrowserWnd->m_pBrowser->GetActiveWebContentWnd();
				auto it = g_pHubble->m_mapHtmlWnd.find(hWnd);
				if (it != g_pHubble->m_mapHtmlWnd.end())
				{
					CWebPage* pHtmlWnd = (CWebPage*)it->second;
					CString strHandle = _T("");
					strHandle.Format(_T("%d"), m_hWnd);
					pHtmlWnd->SendChromeIPCMessage(_T("MdiWinForm_ActiveMdiChild"), m_strKey, strHandle, _T(""), m_strKey, _T(""));
				}
			}
		}
		return 0;
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
		//CTangramXmlParse* pChild = (CTangramXmlParse*)wParam;
		//if (pChild)
		//{
		//	int nCount = pChild->GetCount();
		//}
	}
	break;
	case 20190602:
	{
		return (LRESULT)m_pChildFormsInfo;
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnHubbleGetXml(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strFrameName = (LPCTSTR)wParam;
	CString currentKey = (LPCTSTR)lParam;
	CString strIndex = strFrameName + L"_" + currentKey;
	if (m_bMdiForm)
	{
		auto it = m_mapKey.find(currentKey);
		if (it != m_mapKey.end())
		{
			CString strXml = it->second;
			CTangramXmlParse parse;
			if (parse.LoadXml(strXml))
			{
				CTangramXmlParse* pParse = parse.GetChild(strFrameName);
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
		CTangramXmlParse* pParse = parse.GetChild(strFrameName);
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
	//if (m_pParentHtmlWnd)
	//{
	//	m_pParentHtmlWnd->m_pChromeRenderFrameHost->ShowWebPage(true);
	//}
	return  DefWindowProc(uMsg, wParam, lParam);
}
//
//LRESULT CWinForm::OnMdiClientCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
//{
//	CString strID = (LPCTSTR)lParam;
//	if (m_strBKID == _T(""))
//		m_strBKID = strID;
//	if (lParam)
//	{
//		::PostMessage(m_hWnd, WM_MDICLIENTCREATED, wParam, 0);
//	}
//
//	return  DefWindowProc(uMsg, wParam, lParam);
//}

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

// CQuasar
CQuasar::CQuasar()
{
	m_pCurrentIPCMsg = nullptr;
	m_strAsynKeys = _T("");
	m_strLastKey = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentXml = _T("");
	m_strQuasarName = _T("");
	m_bMDIChild = false;
	m_bDetached = false;
	m_pWebPageWnd = nullptr;
	m_pSubQuasar = nullptr;
	m_bDesignerState = true;
	m_hPWnd = nullptr;
	m_pGalaxyCluster = nullptr;
	m_pWorkNode = nullptr;
	m_pRootNodes = nullptr;
	m_pBindingStar = nullptr;
	m_pContainerNode = nullptr;
	m_pQuasarInfo = nullptr;
	m_pParentStar = nullptr;
	m_nQuasarType = NOQuasar;
#ifdef _DEBUG
	g_pHubble->m_nTangramFrame++;
#endif
}

CQuasar::~CQuasar()
{
#ifdef _DEBUG
	g_pHubble->m_nTangramFrame--;
#endif	
	//if (m_pQuasarInfo)
	//	delete m_pQuasarInfo;
	if (g_pHubble->m_pQuasar == this)
		g_pHubble->m_pQuasar = nullptr;
	for (auto it : g_pHubble->m_mapThreadInfo)
	{
		if (it.second)
		{
			auto it2 = it.second->m_mapQuasar.find(m_hHostWnd);
			if (it2 != it.second->m_mapQuasar.end())
			{
				it.second->m_mapQuasar.erase(it2);
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
		auto it = m_pGalaxyCluster->m_mapQuasar.find(m_hHostWnd);
		if (it != m_pGalaxyCluster->m_mapQuasar.end()) {
			m_pGalaxyCluster->m_mapQuasar.erase(it);
			if (m_pGalaxyCluster->m_mapQuasar.size() == 0)
				delete m_pGalaxyCluster;
		}
	}
	for (auto it : m_mapQuasarProxy)
	{
		if (it.second->m_bAutoDelete)
			delete it.second;
	}
	m_mapQuasarProxy.clear();
	m_hWnd = NULL;
}

void CQuasar::HostPosChanged()
{
	if (::IsWindow(m_hWnd) == false)
		return;
	HWND hwnd = m_hWnd;
	CGrid* pTopGrid = m_pWorkNode;
	CQuasar* _pQuasar = this;
	if (!_pQuasar->m_bDesignerState)
		while (pTopGrid && pTopGrid->m_pRootObj != pTopGrid)
		{
			_pQuasar = pTopGrid->m_pRootObj->m_pGridCommonData->m_pQuasar;
			hwnd = _pQuasar->m_hWnd;
			pTopGrid = _pQuasar->m_pWorkNode;
		}
	if (::IsWindow(hwnd) == false)
		return;
	HWND hPWnd = ::GetParent(m_hWnd);
	if (::IsWindow(_pQuasar->m_pWorkNode->m_pHostWnd->m_hWnd))
	{
		RECT rt1;
		_pQuasar->m_pWorkNode->m_pHostWnd->GetWindowRect(&rt1);

		::ScreenToClient(hPWnd, (LPPOINT)&rt1);
		::ScreenToClient(hPWnd, ((LPPOINT)&rt1) + 1);

		HDWP dwh = BeginDeferWindowPos(1);
		dwh = ::DeferWindowPos(dwh, hwnd, HWND_TOP,
			rt1.left,
			rt1.top,
			rt1.right - rt1.left,
			rt1.bottom - rt1.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_SHOWWINDOW
		);
		EndDeferWindowPos(dwh);
		::SendMessage(hPWnd, WM_QUERYAPPPROXY, 0, 19651965);
	}
}

CTangramXmlParse* CQuasar::UpdateGrid()
{
	for (auto it : m_mapGrid) {
		CGrid* pWindowNode = (CGrid*)it.second;
		if (pWindowNode)
		{
			if (pWindowNode->m_pWindow) {
				if (pWindowNode->m_nActivePage > 0) {
					CString strVal = _T("");
					strVal.Format(_T("%d"), pWindowNode->m_nActivePage);
					pWindowNode->m_pHostParse->put_attr(_T("activepage"), strVal);
				}
				pWindowNode->m_pWindow->Save();
			}
			if (pWindowNode->m_nViewType == Grid)
			{
				((CGridWnd*)pWindowNode->m_pHostWnd)->Save();
			}

			for (auto it2 : pWindowNode->m_vChildNodes) {
				g_pHubble->UpdateGrid(it2);
			}
		}
	}
	if (m_mapGrid.size())
		return m_mapGrid.begin()->second->m_pGridCommonData->m_pHubbleParse;
	return nullptr;
}

void CQuasar::UpdateDesignerTreeInfo()
{
}

CGrid* CQuasar::OpenXtmlDocument(CTangramXmlParse* _pParse, CString strKey, CString strFile)
{
	m_pWorkNode = new CComObject<CGrid>;
	m_pWorkNode->m_pRootObj = m_pWorkNode;
	CGridCommonData* pCommonData = new CGridCommonData();
	m_pWorkNode->m_pGridCommonData = pCommonData;
	pCommonData->m_pQuasar = this;
	pCommonData->m_pGalaxyCluster = m_pGalaxyCluster;
	pCommonData->m_pHubbleParse = _pParse;
	CTangramXmlParse* pParse = _pParse->GetChild(TGM_CLUSTER);
	m_pWorkNode->m_pHostParse = pParse->GetChild(TGM_NODE);

	CreateGalaxyCluster();
	m_mapGrid[strKey] = m_pWorkNode;

	if (m_pGalaxyCluster)
		m_pGalaxyCluster->Fire_OpenXmlComplete(strFile.AllocSysString(), (long)m_hHostWnd, m_pWorkNode);
	m_pWorkNode->m_strKey = strKey;
	m_pWorkNode->Fire_OpenComplete();

	return m_pWorkNode;
}

BOOL CQuasar::CreateGalaxyCluster()
{
	if (::IsWindow(m_hWnd) == false)
		SubclassWindow(m_hHostWnd);
	HWND hPWnd = NULL;
	if (m_hPWnd && ::IsWindow(m_hPWnd))
		hPWnd = m_pGalaxyCluster->m_hWnd;
	else
		hPWnd = ::GetParent(m_hWnd);

	m_pWorkNode->m_strName.Trim();
	m_pWorkNode->m_strName.MakeLower();
	m_pWorkNode->InitWndGrid();
	HWND hWnd = NULL;
	if (m_pWorkNode->m_pObjClsInfo) {
		RECT rc;
		CWnd* pParentWnd = CWnd::FromHandle(hPWnd);
		m_pWorkNode->m_pRootObj = m_pWorkNode;
		CCreateContext	m_Context;
		m_Context.m_pNewViewClass = m_pWorkNode->m_pObjClsInfo;
		CWnd* pWnd = (CWnd*)m_pWorkNode->m_pObjClsInfo->CreateObject();
		GetWindowRect(&rc);
		if (pParentWnd)
			pParentWnd->ScreenToClient(&rc);

		pWnd->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rc, pParentWnd, 0, &m_Context);
		hWnd = pWnd->m_hWnd;
		pWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		::SetWindowPos(pWnd->m_hWnd, HWND_BOTTOM, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_NOACTIVATE);//|SWP_NOREDRAWSWP_NOZORDER);
	}

	CTangramXmlParse* pWndParse = m_pWorkNode->m_pGridCommonData->m_pHubbleParse->GetChild(_T("docplugin"));
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
						m_pWorkNode->m_pGridCommonData->m_PlugInDispDictionary[strPlugID] = pDisp.p;
						pDisp.p->AddRef();
					}

					m_pWorkNode->Fire_NodeAddInCreated(pDisp.p, bstrPlugIn, bstrXml);
				}
				else //for .NET Component
				{
					hr = g_pHubble->CreateCLRObj(bstrPlugIn, &pDisp);
					if (hr == S_OK)
					{
						m_pWorkNode->m_pGridCommonData->m_PlugInDispDictionary[strPlugID] = pDisp.p;

						bstrPlugIn = strPlugID.AllocSysString();
						m_pWorkNode->Fire_NodeAddInCreated(pDisp, bstrPlugIn, bstrXml);
					}
				}
				if (m_pGalaxyCluster && pDisp)
					m_pGalaxyCluster->Fire_AddInCreated(m_pWorkNode, pDisp, bstrPlugIn, bstrXml);
				::SysFreeString(bstrPlugIn);
				bHavePlugin = true;
			}
			::SysFreeString(bstrXml);
		}

		if (bHavePlugin)
			m_pWorkNode->Fire_NodeAddInsCreated();
	}
	m_pWorkNode->m_bCreated = true;
	return true;
}

STDMETHODIMP CQuasar::get_RootGrids(IGridCollection** pGridColletion)
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

STDMETHODIMP CQuasar::get_QuasarData(BSTR bstrKey, VARIANT* pVal)
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

STDMETHODIMP CQuasar::put_QuasarData(BSTR bstrKey, VARIANT newVal)
{
	CString strKey = OLE2T(bstrKey);

	if (strKey == _T(""))
		return S_OK;
	strKey.Trim();
	strKey.MakeLower();
	m_mapVal[strKey] = newVal;
	return S_OK;
}

STDMETHODIMP CQuasar::Detach(void)
{
	if (::IsWindow(m_hWnd))
	{
		m_bDetached = true;
		::ShowWindow(m_pWorkNode->m_pHostWnd->m_hWnd, SW_HIDE);
		RECT rect;
		m_pWorkNode->m_pHostWnd->GetWindowRect(&rect);
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

STDMETHODIMP CQuasar::Attach(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!::IsWindow(m_hWnd)) {
		m_bDetached = false;
		::ShowWindow(m_pWorkNode->m_pHostWnd->m_hWnd, SW_SHOW);
		SubclassWindow(m_hHostWnd);
	}
	HostPosChanged();
	//::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20180115);
	return S_OK;
}

STDMETHODIMP CQuasar::ModifyHost(LONGLONG hHostWnd)
{
	HWND _hHostWnd = (HWND)hHostWnd;
	if (!::IsWindow(_hHostWnd) || m_hWnd == _hHostWnd)
	{
		return S_OK;
	}
	if (m_pWorkNode == nullptr)
		return S_FALSE;
	HWND hParent = (HWND)::GetParent(_hHostWnd);
	CWindow m_Parent;
	RECT rc;
	m_pWorkNode->m_pHostWnd->GetWindowRect(&rc);
	if (::IsWindow(m_hWnd)) {
		HWND hTangramWnd = m_pGalaxyCluster->m_hWnd;
		auto it = g_pHubble->m_mapWindowPage.find(hTangramWnd);
		if (it != g_pHubble->m_mapWindowPage.end())
			g_pHubble->m_mapWindowPage.erase(it);
		m_pGalaxyCluster->m_hWnd = hTangramWnd;
		g_pHubble->m_mapWindowPage[hTangramWnd] = m_pGalaxyCluster;

		auto it2 = m_pGalaxyCluster->m_mapQuasar.find(m_hWnd);
		if (it2 != m_pGalaxyCluster->m_mapQuasar.end()) {
			m_pGalaxyCluster->m_mapQuasar.erase(it2);
			m_pGalaxyCluster->m_mapQuasar[_hHostWnd] = this;
			m_pGalaxyCluster->m_mapWnd[m_strQuasarName] = _hHostWnd;
			DWORD dwID = ::GetWindowThreadProcessId(_hHostWnd, NULL);
			TRACE(_T("OpenEx ThreadInfo:%x\n"), dwID);
			CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);
			theUniverse.SetHook(dwID);
			auto iter = pThreadInfo->m_mapQuasar.find(m_hWnd);
			if (iter != pThreadInfo->m_mapQuasar.end()) {
				pThreadInfo->m_mapQuasar.erase(iter);
				pThreadInfo->m_mapQuasar[_hHostWnd] = this;
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
		if (it.second != m_pWorkNode)
		{
			::SetParent(it.second->m_pHostWnd->m_hWnd, m_pWorkNode->m_pHostWnd->m_hWnd);
		}
	}
	::SetParent(m_pWorkNode->m_pHostWnd->m_hWnd, ::GetParent(_hHostWnd));
	::SetWindowPos(m_pWorkNode->m_pHostWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	HostPosChanged();
	return S_OK;
}

STDMETHODIMP CQuasar::get_HWND(LONGLONG* pVal)
{
	if (m_hWnd)
		*pVal = (LONGLONG)m_hWnd;
	else
		*pVal = (LONGLONG)m_hHostWnd;
	return S_OK;
}

STDMETHODIMP CQuasar::get_VisibleGrid(IGrid** pVal)
{
	if (m_pWorkNode)
		*pVal = m_pWorkNode;
	return S_OK;
}

STDMETHODIMP CQuasar::get_GalaxyCluster(IGalaxyCluster** pVal)
{
	if (m_pGalaxyCluster)
		*pVal = m_pGalaxyCluster;

	return S_OK;
}

STDMETHODIMP CQuasar::Observe(BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid)
{
	if (::GetWindowLong(m_hWnd, GWL_STYLE) & MDIS_ALLCHILDSTYLES)
		m_nQuasarType = QuasarType::MDIClientQuasar;

	CString _strXml = OLE2T(bstrXml);
	if (m_pGalaxyCluster->m_strPageFileName == _T(""))
	{
		m_pGalaxyCluster->m_strPageFileName = g_pHubble->m_strExeName;
		m_pGalaxyCluster->m_strPageFilePath = g_pHubble->m_strConfigDataFile;
	}
	DWORD dwID = ::GetWindowThreadProcessId(m_hHostWnd, NULL);
	TRACE(_T("OpenEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);
	theUniverse.SetHook(dwID);

	CString strCurrentKey = OLE2T(bstrKey);
	if (strCurrentKey == _T(""))
		strCurrentKey = _T("default");
	if (m_strCurrentKey != strCurrentKey)
	{
		m_strLastKey = m_strCurrentKey;
		m_strCurrentKey = strCurrentKey;
	}
	g_pHubble->m_pGalaxyCluster = m_pGalaxyCluster;
	g_pHubble->m_pQuasar = this;

	m_strCurrentKey = m_strCurrentKey.MakeLower();
	g_pHubble->m_strCurrentKey = m_strCurrentKey;
	CString strXml = _T("");
	auto it = m_mapGrid.find(m_strCurrentKey);

	CGrid* pOldNode = m_pWorkNode;
	if (it != m_mapGrid.end())
		m_pWorkNode = it->second;
	else
	{
		bool bAtTemplate = false;

		LRESULT l = ::SendMessage(m_pGalaxyCluster->m_hWnd, WM_TANGRAMGETXML, (WPARAM)LPCTSTR(m_strQuasarName), (WPARAM)LPCTSTR(m_strCurrentKey));
		if (l)
		{
			if (m_strCurrentXml != _T(""))
			{
				strXml = m_strCurrentXml;
				m_strCurrentXml = _T("");
			}
			else
			{
				auto it = g_pHubble->m_mapValInfo.find(m_strQuasarName + L"_" + m_strCurrentKey);
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
				if (bAtTemplate == false)
				{
					if (m_strCurrentKey != _T("newdocument"))
					{
						CString _str = _T("@") + m_strQuasarName + _T("@") + m_pGalaxyCluster->m_strConfigFileNodeName;
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
							strXml = _T("<default><cluster><grid name=\"Start\" /></cluster></default>");;
					}
					else
						strXml = _strXml;
				}
			}
		}

		Unlock();
		m_pGalaxyCluster->Fire_BeforeOpenXml(CComBSTR(strXml), (long)m_hHostWnd);
		m_pWorkNode = g_pHubble->ObserveEx((long)m_hHostWnd, _T(""), strXml);
		if (m_pWorkNode == nullptr)
			return S_FALSE;
		if (::GetWindowLong(::GetParent(m_hWnd), GWL_EXSTYLE) & WS_EX_MDICHILD)
			m_bMDIChild = true;
	}
	m_pBindingStar = m_pWorkNode->m_pGridCommonData->m_pHostClientView ? m_pWorkNode->m_pGridCommonData->m_pHostClientView->m_pGrid : nullptr;

	g_pHubble->m_strCurrentKey = _T("");
	*ppRetGrid = (IGrid*)m_pWorkNode;
	for (auto it : g_pHubble->m_mapHubbleAppProxy)
	{
		it.second->OnOpenComplete(m_hHostWnd, strXml, m_pWorkNode);
	}
	if (g_pHubble->m_pCosmosAppProxy)
		g_pHubble->m_pCosmosAppProxy->OnOpenComplete(m_hHostWnd, strXml, m_pWorkNode);

	if (pOldNode && pOldNode != m_pWorkNode)
	{
		RECT  rc;
		if (::IsWindow(pOldNode->m_pHostWnd->m_hWnd))
			::GetWindowRect(pOldNode->m_pHostWnd->m_hWnd, &rc);
		CWnd* pWnd = m_pWorkNode->m_pHostWnd;
		HWND hParent = ::GetParent(m_hWnd);

		CWnd::FromHandle(hParent)->ScreenToClient(&rc);
		for (auto it : m_mapGrid)
		{
			HWND hwnd = it.second->m_pHostWnd->m_hWnd;
			BOOL bTop = (it.second == m_pWorkNode);
			it.second->m_bTopObj = bTop;
			::SetWindowLongPtr(hwnd, GWLP_ID, bTop ? m_pWorkNode->m_nID : 0);
			::SetParent(hwnd, bTop ? hParent : pWnd->m_hWnd);
			if (!bTop)
			{
				::ShowWindow(hwnd, SW_HIDE);
			}
		}
		::SetWindowPos(pWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);

		if (m_pWorkNode != nullptr) {
			if (m_pWorkNode->m_nViewType != Grid) {
				if (m_pWorkNode->m_pHostWnd)
					m_pWorkNode->m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}
			HRESULT hr = S_OK;
			int cConnections = g_pHubble->m_vec.GetSize();
			if (cConnections) {
				CComVariant avarParams[3];
				avarParams[2] = (long)m_hHostWnd;
				avarParams[2].vt = VT_I4;
				avarParams[1] = strXml.AllocSysString();
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = (IGrid*)m_pWorkNode;
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

		for (auto it : m_mapQuasarProxy)
		{
			it.second->OnExtend(m_pWorkNode, m_strCurrentKey, strXml);
		}
	}

	HostPosChanged();
	//Add 20200218
	if (m_pBindingStar)
	{
		CGrid* _pHostNode = m_pBindingStar;
		if (_pHostNode->m_pHostQuasar)
		{
			CQuasar* _pQuasar = _pHostNode->m_pHostQuasar;
			while (_pQuasar)
			{
				_pHostNode = _pQuasar->m_pBindingStar;
				if (_pHostNode && _pHostNode->m_pHostQuasar)
					_pQuasar = _pHostNode->m_pHostQuasar;
				else
					break;
			}
		}
		if (_pHostNode && m_pWebPageWnd)
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

	if (m_pWorkNode->m_pHostQuasar)
	{
		IGrid* pGrid = nullptr;
		m_pWorkNode->m_pHostQuasar->Observe(CComBSTR(m_pWorkNode->m_pHostQuasar->m_strCurrentKey), CComBSTR(""), &pGrid);
	}
	for (auto it : m_pWorkNode->m_mapExtendNode)
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
		m_pWorkNode->GetGrids(CComBSTR(m_strHostWebBrowserNodeName), &pGrid, &pCol, &nCount);
		if (pGrid)
		{
			CGrid* _pGrid = (CGrid*)pGrid;
			if (_pGrid->m_nViewType == BlankView)
			{
				CQuasar* _pQuasar = nullptr;
				if (_pGrid->m_pHostQuasar)
				{
					_pQuasar = _pGrid->m_pHostQuasar;
					while (_pQuasar)
					{
						if (_pQuasar->m_pHostWebBrowserNode)
							_pGrid = _pQuasar->m_pHostWebBrowserNode;
						if (_pGrid && _pGrid->m_pHostQuasar)
						{
							_pQuasar = _pGrid->m_pHostQuasar;
							_pGrid = _pQuasar->m_pHostWebBrowserNode;
						}
						else
							break;
					}
				}
				if (m_pHostWebBrowserWnd->m_pParentStar == nullptr)
				{
					m_pHostWebBrowserWnd->m_pParentStar = _pGrid;
					m_pHostWebBrowserWnd->m_pParentStar->m_pWebBrowser = m_pHostWebBrowserWnd;
				}
				else if (m_pHostWebBrowserWnd->m_pParentStar != _pGrid)//&&_pGrid->m_pHostWnd->IsWindowVisible())
				{
					m_pHostWebBrowserWnd->m_pParentStar->m_pWebBrowser = nullptr;
					m_pHostWebBrowserWnd->m_pParentStar = _pGrid;
					_pGrid->m_pWebBrowser = m_pHostWebBrowserWnd;
					::SetParent(m_pHostWebBrowserWnd->m_hWnd, _pGrid->m_pHostWnd->m_hWnd);
					::SetWindowPos(m_pHostWebBrowserWnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
		}
		//m_pHostWebBrowserWnd->m_pBrowser->LayoutBrowser();
	}
	::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20180115);
	return S_OK;
}

STDMETHODIMP CQuasar::get_CurrentNavigateKey(BSTR* pVal)
{
	*pVal = m_strCurrentKey.AllocSysString();
	return S_OK;
}

void CQuasar::Destroy()
{
	CGrid* pWndGrid = nullptr;
	CString strPlugID = _T("");
	for (auto it : m_mapGrid)
	{
		pWndGrid = it.second;
		if (pWndGrid->m_pGridCommonData->m_pHubbleParse)
		{
			CTangramXmlParse* pParse = pWndGrid->m_pGridCommonData->m_pHubbleParse->GetChild(_T("docplugin"));
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
							IDispatch* pDisp = (IDispatch*)pWndGrid->m_pGridCommonData->m_PlugInDispDictionary[strPlugID];
							if (pDisp)
							{
								pWndGrid->m_pGridCommonData->m_PlugInDispDictionary.RemoveKey(LPCTSTR(strPlugID));
								pDisp->Release();
							}
						}
					}
				}
			}

			pWndGrid->m_pGridCommonData->m_PlugInDispDictionary.RemoveAll();
		}
	}
}

void CQuasar::OnFinalMessage(HWND hWnd)
{
	HWND hParent = ::GetParent(hWnd);
	if (::IsWindow(hParent))
		::PostMessage(hParent, WM_COSMOSMSG, 4096, 0);
	__super::OnFinalMessage(hWnd);
}

LRESULT CQuasar::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
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

	{
		g_pHubble->m_hActiveWnd = 0;
		g_pHubble->m_bWinFormActived = false;
		g_pHubble->m_pActiveStar = nullptr;
		g_pHubble->m_pQuasar = nullptr;
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
			g_pHubble->m_pQuasar = this;
		}
	}
	//++++++++++++++++end+++++++++++

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CQuasar::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pHubble->m_pCLRProxy) {
		g_pHubble->m_pCLRProxy->ReleaseHubbleObj((IQuasar*)this);
	}
	if (m_mapGridScript.size())
	{
		this->UpdateGrid();
		for (auto it : m_mapGridScript)
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

	m_pGalaxyCluster->BeforeDestory();
	m_pGalaxyCluster->m_strConfigFileNodeName.MakeLower();//20190116
	auto it = g_pHubble->m_mapWindowPage.find(m_pGalaxyCluster->m_hWnd);
	if (it != g_pHubble->m_mapWindowPage.end())
	{
		auto it2 = m_pGalaxyCluster->m_mapNeedSaveQuasar.find(m_hWnd);
		if (it2 != m_pGalaxyCluster->m_mapNeedSaveQuasar.end())
		{
			if (!m_pGalaxyCluster->m_bNewVersion)
			{
				m_pGalaxyCluster->put_ConfigName(m_pGalaxyCluster->m_strConfigFileNodeName.AllocSysString());
			}
		}
	}

	DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
	TRACE(_T("OpenEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pHubble->GetThreadInfo(dwID);
	if (pThreadInfo)
	{
		//theApp.SetHook(dwID);
		auto iter = pThreadInfo->m_mapQuasar.find(m_hWnd);
		if (iter != pThreadInfo->m_mapQuasar.end())
		{
			pThreadInfo->m_mapQuasar.erase(iter);
			TRACE(_T("Cosmos Frame Removed from ThreadInfo Manager:%x\n"), m_hWnd);
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CQuasar::OnHubbleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
	case 20180115:
	{
		HostPosChanged();
	}
	break;
	case WM_BROWSERLAYOUT:
	{
		CWebPage* pWebWnd = (CWebPage*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		::PostMessage(::GetParent(pWebWnd->m_hWnd), WM_BROWSERLAYOUT, 0, 1);
	}
	break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CQuasar::OnTabChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD);
	if (hWnd == nullptr)
		return 0;
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CQuasar::OnDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
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

LRESULT CQuasar::OnGetDPIScaledSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
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

LRESULT CQuasar::OnBeforeParentDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CQuasar::OnAfterParentDpiChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	//HostPosChanged();
	//::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20180115);
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CQuasar::OnGetMe(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (lParam == 1992)
	{
		return (LRESULT)this;
	}

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CQuasar::OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LONG_PTR pfnWndProc = ::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
	LRESULT lr = DefWindowProc(uMsg, wParam, lParam);
	if (m_pfnSuperWindowProc != ::DefWindowProc && ::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC) == pfnWndProc)
		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_pfnSuperWindowProc);

	// mark window as destryed
	m_dwState |= WINSTATE_DESTROYED;
	return lr;
}

LRESULT CQuasar::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT hr = DefWindowProc(uMsg, wParam, lParam);

	if (g_pHubble->m_pBrowserFactory && g_pHubble->m_bChromeNeedClosed)
		return hr;

	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;

	if (m_pBindingStar)
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
		::SetWindowPos(m_pWorkNode->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_NOREDRAW);// ); 
		CGrid* _pHostNode = m_pBindingStar;
		if (_pHostNode->m_pHostQuasar)
		{
			CQuasar* _pQuasar = _pHostNode->m_pHostQuasar;
			while (_pQuasar)
			{
				if (_pQuasar->m_pBindingStar)
					_pHostNode = _pQuasar->m_pBindingStar;
				else
					break;
				if (_pHostNode && _pHostNode->m_pHostQuasar)
				{
					_pQuasar = _pHostNode->m_pHostQuasar;
					if (_pQuasar == nullptr)
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
			CGrid* _pParentNode = m_pBindingStar->m_pParentObj;
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
					CGridHelperWnd* pWnd = (CGridHelperWnd*)_pParentNode->m_pHostWnd;
					::InvalidateRect(pWnd->m_hWnd, nullptr, true);
				}
				break;
				}
			}
			if (m_pContainerNode && m_pContainerNode->m_pParentObj && m_pContainerNode->m_pParentObj->m_nViewType == Grid)
			{
				if (m_pContainerNode->m_nViewType == CLRCtrl)
				{
					CGridHelperWnd* pGridWnd = (CGridHelperWnd*)m_pContainerNode->m_pHostWnd;
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
		::SetWindowPos(m_pWorkNode->m_pHostWnd->m_hWnd, HWND_TOP, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOSENDCHANGING | /*SWP_NOZORDER |*/ SWP_NOACTIVATE | SWP_FRAMECHANGED);
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
		lpwndpos->flags |= SWP_HIDEWINDOW;
	}

	if (m_bMDIChild)
		lpwndpos->flags |= SWP_NOZORDER;
	::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return hr;
}

LRESULT CQuasar::OnParentNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pHubble->m_pQuasar = nullptr;
	return DefWindowProc(uMsg, wParam, lParam);
}

STDMETHODIMP CQuasar::get_DesignerState(VARIANT_BOOL* pVal)
{
	if (m_bDesignerState)
		*pVal = true;
	else
		*pVal = false;

	return S_OK;
}

STDMETHODIMP CQuasar::put_DesignerState(VARIANT_BOOL newVal)
{
	m_bDesignerState = newVal;
	return S_OK;
}

STDMETHODIMP CQuasar::GetXml(BSTR bstrRootName, BSTR* bstrRet)
{
	CString strRootName = OLE2T(bstrRootName);
	if (strRootName == _T(""))
		strRootName = _T("DocumentUI");
	CString strXmlData = _T("<Default><cluster><grid name=\"Start\"/></cluster></Default>");
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
			m_mapTemp[strName] = it.second->m_pGridCommonData->m_pHubbleParse->xml();
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
				strXml = it.second->m_pGridCommonData->m_pHubbleParse->xml();
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

STDMETHODIMP CQuasar::get_Count(long* pCount)
{
	*pCount = (long)m_mapGrid.size();
	return S_OK;
}

STDMETHODIMP CQuasar::get_Grid(VARIANT vIndex, IGrid** ppGrid)
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

STDMETHODIMP CQuasar::get__NewEnum(IUnknown** ppVal)
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

STDMETHODIMP CQuasar::get_QuasarXML(BSTR* pVal)
{
	CString strData = _T("<");
	CString strName = m_strQuasarName;
	strName.Replace(_T("@"), _T("_"));
	strData += strName;
	strData += _T(">");
	for (auto it : m_mapGrid)
	{
		CGrid* pWindowNode = (CGrid*)it.second;
		if (pWindowNode)
		{
			if (pWindowNode->m_pWindow)
			{
				if (pWindowNode->m_nActivePage > 0)
				{
					CString strVal = _T("");
					strVal.Format(_T("%d"), pWindowNode->m_nActivePage);
					pWindowNode->m_pHostParse->put_attr(_T("activepage"), strVal);
				}
				pWindowNode->m_pWindow->Save();
			}
			if (pWindowNode->m_nViewType == Grid)
			{
				((CGridWnd*)pWindowNode->m_pHostWnd)->Save();
			}

			for (auto it2 : pWindowNode->m_vChildNodes)
			{
				g_pHubble->UpdateGrid(it2);
			}
		}
		CString strXml = pWindowNode->m_pGridCommonData->m_pHubbleParse->GetChild(TGM_CLUSTER)->xml();
		CString s = _T("");
		s.Format(_T("<%s>%s</%s>"), it.first, strXml, it.first);
		CString strKey = it.second->m_strKey + _T("@") + this->m_strQuasarName + _T("@") + _T("tangramdefaultpage");
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
		if (strKey.Find(_T("@") + m_strQuasarName + _T("@")) != -1)
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

STDMETHODIMP CQuasar::get_TangramDoc(IHubbleDoc** pVal)
{
	return S_OK;
}

STDMETHODIMP CQuasar::get_QuasarType(QuasarType* pVal)
{
	*pVal = m_nQuasarType;
	return S_OK;
}

STDMETHODIMP CQuasar::get_Name(BSTR* pVal)
{
	*pVal = m_strQuasarName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CQuasar::get_HostBrowser(IBrowser** ppChromeWebBrowser)
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

STDMETHODIMP CQuasar::get_HostWebPage(IWebPage** ppChromeWebPage)
{
	if (m_pWebPageWnd != nullptr)
	{
		*ppChromeWebPage = (IWebPage*)m_pWebPageWnd;
	}
	return S_FALSE;
}
