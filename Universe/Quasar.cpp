/********************************************************************************
*					Open Universe - version 1.0.0.4								*
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
#include "GridHelper.h"
#include "grid.h"
#include "Quasar.h"

CWinForm::CWinForm(void)
{
	m_nState = -1;
	m_bMdiForm = false;
	m_pOwnerHtmlWnd = nullptr;
	m_pParentHtmlWnd = nullptr;
	m_strXml = m_strKey =  _T("");
}

CWinForm::~CWinForm(void)
{
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
				//Don't Support MDI
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
		m_bMdiForm = true;
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
				HWND hWnd = pWndGrid->m_pGridShareData->m_pQuasar->m_hWnd;
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

LRESULT CWinForm::OnHubbleGetXml(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strQuasarName = (LPCTSTR)wParam;
	CString currentKey = (LPCTSTR)lParam;
	CString strIndex = strQuasarName + L"_" + currentKey;
	if (m_bMdiForm)
	{
		auto it = m_mapKey.find(currentKey);
		if (it != m_mapKey.end())
		{
			CString strXml = it->second;
			CTangramXmlParse parse;
			if (parse.LoadXml(strXml))
			{
				CTangramXmlParse* pParse = parse.GetChild(strQuasarName);
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
		CTangramXmlParse* pParse = parse.GetChild(strQuasarName);
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

LRESULT CWinForm::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pHubble->m_pActiveHtmlWnd = nullptr;
	g_pHubble->m_pActiveWinFormWnd = this;
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

// CQuasar
CQuasar::CQuasar()
{
	m_pCurrentIPCMsg = nullptr;
	m_strLastKey = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentXml = _T("");
	m_strQuasarName = _T("");
	m_bMDIChild = false;
	m_bDetached = false;
	m_pWebPageWnd = nullptr;
	m_bDesignerState = true;
	m_hPWnd = nullptr;
	m_pGalaxyCluster = nullptr;
	m_pWorkGrid = nullptr;
	m_pRootNodes = nullptr;
	m_pBindingGrid = nullptr;
	m_pContainerNode = nullptr;
	m_pQuasarInfo = nullptr;
	m_pParentGrid = nullptr;
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

	if (m_pGalaxyCluster) {
		auto it = m_pGalaxyCluster->m_mapQuasar.find(m_hHostWnd);
		if (it != m_pGalaxyCluster->m_mapQuasar.end()) {
			m_pGalaxyCluster->m_mapQuasar.erase(it);
			if (m_pGalaxyCluster->m_mapQuasar.size() == 0)
				delete m_pGalaxyCluster;
		}
	}
	m_hWnd = NULL;
}

void CQuasar::HostPosChanged()
{
	if (::IsWindow(m_hWnd) == false)
		return;
	HWND hwnd = m_hWnd;
	CGrid* pTopGrid = m_pWorkGrid;
	CQuasar* _pQuasar = this;
	if (!_pQuasar->m_bDesignerState)
		while (pTopGrid && pTopGrid->m_pRootObj != pTopGrid)
		{
			_pQuasar = pTopGrid->m_pRootObj->m_pGridShareData->m_pQuasar;
			hwnd = _pQuasar->m_hWnd;
			pTopGrid = _pQuasar->m_pWorkGrid;
		}
	if (::IsWindow(hwnd) == false)
		return;
	HWND hPWnd = ::GetParent(m_hWnd);
	if (::IsWindow(_pQuasar->m_pWorkGrid->m_pHostWnd->m_hWnd))
	{
		RECT rt1;
		_pQuasar->m_pWorkGrid->m_pHostWnd->GetWindowRect(&rt1);

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
		::SendMessage(hPWnd, WM_QUERYAPPPROXY, 0, 19651965);
	}
}

CGrid* CQuasar::OpenXtmlDocument(CTangramXmlParse* _pParse, CString strKey, CString strFile)
{
	m_pWorkGrid = new CComObject<CGrid>;
	m_pWorkGrid->m_pRootObj = m_pWorkGrid;
	CGridShareData* pCommonData = new CGridShareData();
	m_pWorkGrid->m_pGridShareData = pCommonData;
	pCommonData->m_pQuasar = this;
	pCommonData->m_pGalaxyCluster = m_pGalaxyCluster;
	pCommonData->m_pHubbleParse = _pParse;
	CTangramXmlParse* pParse = _pParse->GetChild(TGM_CLUSTER);
	m_pWorkGrid->m_pHostParse = pParse->GetChild(TGM_NODE);

	CreateGalaxyCluster();
	m_mapGrid[strKey] = m_pWorkGrid;

	m_pWorkGrid->m_strKey = strKey;
	m_pWorkGrid->Fire_ObserveComplete();

	return m_pWorkGrid;
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

	m_pWorkGrid->m_bCreated = true;
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
	return S_OK;
}

STDMETHODIMP CQuasar::put_QuasarData(BSTR bstrKey, VARIANT newVal)
{
	return S_OK;
}

STDMETHODIMP CQuasar::Detach(void)
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

STDMETHODIMP CQuasar::Attach(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!::IsWindow(m_hWnd)) {
		m_bDetached = false;
		::ShowWindow(m_pWorkGrid->m_pHostWnd->m_hWnd, SW_SHOW);
		SubclassWindow(m_hHostWnd);
	}
	HostPosChanged();
	return S_OK;
}

STDMETHODIMP CQuasar::ModifyHost(LONGLONG hHostWnd)
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
	if (m_pWorkGrid)
		*pVal = m_pWorkGrid;
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
	g_pHubble->m_pQuasar = this;

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

		LRESULT l = ::SendMessage(m_pGalaxyCluster->m_hWnd, WM_HUBBLE_GETXML, (WPARAM)LPCTSTR(m_strQuasarName), (WPARAM)LPCTSTR(m_strCurrentKey));
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
					strXml = _strXml;
				}
			}
		}

		Unlock();
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
	}

	m_bObserve = false;

	HostPosChanged();
	//Add 20200218
	if (m_pBindingGrid)
	{
		CGrid* _pHostNode = m_pBindingGrid;
		if (_pHostNode->m_pHostQuasar)
		{
			CQuasar* _pQuasar = _pHostNode->m_pHostQuasar;
			while (_pQuasar)
			{
				_pHostNode = _pQuasar->m_pBindingGrid;
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
	if (m_pWorkGrid->m_pHostQuasar)
	{
		IGrid* pGrid = nullptr;
		m_pWorkGrid->m_pHostQuasar->Observe(CComBSTR(m_pWorkGrid->m_pHostQuasar->m_strCurrentKey), CComBSTR(""), &pGrid);
	}
	for (auto it : m_pWorkGrid->m_mapExtendNode)
	{
		IGrid* pGrid = nullptr;
		it.first->Observe(CComBSTR(it.second), CComBSTR(""), &pGrid);
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
		g_pHubble->m_pActiveGrid = nullptr;
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
	m_pGalaxyCluster->BeforeDestory();

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
		::SetWindowPos(m_pWorkGrid->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOACTIVATE | SWP_FRAMECHANGED); //| SWP_NOREDRAW);// ); 
		CGrid* _pHostNode = m_pBindingGrid;
		if (_pHostNode->m_pHostQuasar)
		{
			CQuasar* _pQuasar = _pHostNode->m_pHostQuasar;
			while (_pQuasar)
			{
				if (_pQuasar->m_pBindingGrid)
					_pHostNode = _pQuasar->m_pBindingGrid;
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
	return S_OK;
}

STDMETHODIMP CQuasar::get_HubbleDoc(IHubbleDoc** pVal)
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
