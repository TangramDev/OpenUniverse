/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101020002
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

// Galaxy.cpp : implementation file of CGalaxy
//

#include "stdafx.h"
#include "UniverseApp.h"
#include "Cosmos.h"
#include "GridWnd.h"
#include "XobjHelper.h"
#include "Xobj.h"
#include "Galaxy.h"

CWinForm::CWinForm(void)
{
	m_nState = -1;
	m_bMdiForm = false;
	m_strXml = m_strKey =  _T("");
}

CWinForm::~CWinForm(void)
{
}

LRESULT CWinForm::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	g_pCosmos->m_pActiveWinFormWnd = this;
	if (g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_pCLRProxy->OnWinFormActivate(m_hWnd, LOWORD(wParam));
	}
	return lRes;
}

LRESULT CWinForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pCosmos->m_pActiveWinFormWnd == this)
		g_pCosmos->m_pActiveWinFormWnd = nullptr;
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
	case 20190214:
		return (LRESULT)this;
		break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnCosmosGetXml(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	CString strGalaxyName = (LPCTSTR)wParam;
	CString currentKey = (LPCTSTR)lParam;
	CString strIndex = strGalaxyName + L"_" + currentKey;
	CTangramXmlParse parse;
	if (parse.LoadXml(m_strXml) || parse.LoadFile(m_strXml))
	{
		CTangramXmlParse* pParse = parse.GetChild(strGalaxyName);
		if (pParse)
		{
			CTangramXmlParse* pParse2 = pParse->GetChild(currentKey);
			if (pParse2)
			{
				auto it = g_pCosmos->m_mapValInfo.find(strIndex);
				if (it != g_pCosmos->m_mapValInfo.end())
				{
					g_pCosmos->m_mapValInfo.erase(it);
				}
				g_pCosmos->m_mapValInfo[strIndex] = CComVariant(pParse2->xml());
				return 1;
			}
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_pActiveWinFormWnd = this;
	::BringWindowToTop(m_hWnd);
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
	if (g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_pCLRProxy->PreWindowPosChanging(m_hWnd, (WINDOWPOS*)lParam, 1);
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CWinForm::OnFormCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_hFormNodeWnd = nullptr;
	return DefWindowProc(uMsg, wParam, lParam);
}

void CWinForm::OnFinalMessage(HWND hWnd)
{
	auto it = g_pCosmos->m_mapFormWebPage.find(hWnd);
	if (it != g_pCosmos->m_mapFormWebPage.end())
		g_pCosmos->m_mapFormWebPage.erase(it);
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

// CGalaxy
CGalaxy::CGalaxy()
{
	m_strLastKey = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentXml = _T("");
	m_strGalaxyName = _T("");
	m_bMDIChild = false;
	m_bDetached = false;
	m_bDesignerState = true;
	m_hPWnd = nullptr;
	m_pGalaxyCluster = nullptr;
	m_pWorkXobj = nullptr;
	m_pRootNodes = nullptr;
	m_pBindingXobj = nullptr;
	m_pContainerNode = nullptr;
	m_pGalaxyInfo = nullptr;
	m_pParentXobj = nullptr;
	m_nGalaxyType = NOGalaxy;
#ifdef _DEBUG
	g_pCosmos->m_nTangramFrame++;
#endif
}

CGalaxy::~CGalaxy()
{
#ifdef _DEBUG
	g_pCosmos->m_nTangramFrame--;
#endif	
	//if (m_pGalaxyInfo)
	//	delete m_pGalaxyInfo;
	if (g_pCosmos->m_pGalaxy == this)
		g_pCosmos->m_pGalaxy = nullptr;
	for (auto it : g_pCosmos->m_mapThreadInfo)
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
		CCommonFunction::ClearObject<CXobjCollection>(m_pRootNodes);

	if (m_pGalaxyCluster) {
		auto it = m_pGalaxyCluster->m_mapGalaxy.find(m_hHostWnd);
		if (it != m_pGalaxyCluster->m_mapGalaxy.end()) {
			m_pGalaxyCluster->m_mapGalaxy.erase(it);
			if (m_pGalaxyCluster->m_mapGalaxy.size() == 0)
				delete m_pGalaxyCluster;
		}
	}
	m_hWnd = NULL;
}

void CGalaxy::HostPosChanged()
{
	if (::IsWindow(m_hWnd) == false)
		return;
	HWND hwnd = m_hWnd;
	CXobj* pTopXobj = m_pWorkXobj;
	CGalaxy* _pGalaxy = this;
	if (!_pGalaxy->m_bDesignerState)
		while (pTopXobj && pTopXobj->m_pRootObj != pTopXobj)
		{
			_pGalaxy = pTopXobj->m_pRootObj->m_pXobjShareData->m_pGalaxy;
			hwnd = _pGalaxy->m_hWnd;
			pTopXobj = _pGalaxy->m_pWorkXobj;
		}
	if (::IsWindow(hwnd) == false)
		return;
	HWND hPWnd = ::GetParent(m_hWnd);
	if (::IsWindow(_pGalaxy->m_pWorkXobj->m_pHostWnd->m_hWnd))
	{
		RECT rt1;
		_pGalaxy->m_pWorkXobj->m_pHostWnd->GetWindowRect(&rt1);

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

HWND CGalaxy::GetWinForm(HWND hForm)
{
	if (g_pCosmos->m_pCLRProxy)
	{
		int nForm = g_pCosmos->m_pCLRProxy->IsWinForm(hForm);
		while (nForm >= 1)
		{
			m_pWorkXobj->m_pParentWinFormWnd = (CWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
			LRESULT lRes = ::SendMessage(hForm, WM_HUBBLE_GETNODE, 0, 0);
			if (lRes)
			{
				CXobj* pXobj = (CXobj*)lRes;
				hForm = pXobj->m_pRootObj->m_pXobjShareData->m_pGalaxyCluster->m_hWnd;
				nForm = g_pCosmos->m_pCLRProxy->IsWinForm(hForm);
			}
			else
			{
				break;
			}
		}
		if (nForm == 0)
		{
			hForm = ::GetParent(hForm);
			while (g_pCosmos->m_pCLRProxy->IsWinForm(hForm) == 0)
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

CXobj* CGalaxy::OpenXtmlDocument(CTangramXmlParse* _pParse, CString strKey, CString strFile)
{
	m_pWorkXobj = new CComObject<CXobj>;
	m_pWorkXobj->m_pRootObj = m_pWorkXobj;
	CXobjShareData* pCommonData = new CXobjShareData();
	m_pWorkXobj->m_pXobjShareData = pCommonData;
	pCommonData->m_pGalaxy = this;
	pCommonData->m_pGalaxyCluster = m_pGalaxyCluster;
	pCommonData->m_pCosmosParse = _pParse;
	CTangramXmlParse* pParse = _pParse->GetChild(TGM_CLUSTER);
	m_pWorkXobj->m_pHostParse = pParse->GetChild(TGM_XOBJ);

	CreateGalaxyCluster();
	m_mapXobj[strKey] = m_pWorkXobj;

	m_pWorkXobj->m_strKey = strKey;
	m_pWorkXobj->Fire_ObserveComplete();
	if (g_pCosmos->m_pCLRProxy)
	{
		CGalaxyCluster* pGalaxyCluster = m_pGalaxyCluster;
		HWND hForm = pGalaxyCluster->m_hWnd;
		hForm = GetWinForm(hForm);
		if (hForm)
			m_pWorkXobj->m_pParentWinFormWnd = (CWinForm*)::SendMessage(hForm, WM_HUBBLE_DATA, 0, 20190214);
	}

	return m_pWorkXobj;
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

	//m_pWorkXobj->m_strName.Trim();
	//m_pWorkXobj->m_strName.MakeLower();
	m_pWorkXobj->InitWndXobj();
	HWND hWnd = NULL;
	if (m_pWorkXobj->m_pObjClsInfo) {
		RECT rc;
		CWnd* pParentWnd = CWnd::FromHandle(hPWnd);
		m_pWorkXobj->m_pRootObj = m_pWorkXobj;
		CCreateContext	m_Context;
		m_Context.m_pNewViewClass = m_pWorkXobj->m_pObjClsInfo;
		CWnd* pWnd = (CWnd*)m_pWorkXobj->m_pObjClsInfo->CreateObject();
		GetWindowRect(&rc);
		if (pParentWnd)
			pParentWnd->ScreenToClient(&rc);

		pWnd->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rc, pParentWnd, 0, &m_Context);
		hWnd = pWnd->m_hWnd;
		pWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		::SetWindowPos(pWnd->m_hWnd, HWND_BOTTOM, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_NOACTIVATE);//|SWP_NOREDRAWSWP_NOZORDER);
	}

	m_pWorkXobj->m_bCreated = true;
	return true;
}

STDMETHODIMP CGalaxy::get_RootXobjs(IXobjCollection** pXobjColletion)
{
	if (m_pRootNodes == nullptr) {
		CComObject<CXobjCollection>::CreateInstance(&m_pRootNodes);
		m_pRootNodes->AddRef();
	}

	m_pRootNodes->m_pXobjs->clear();
	for (auto it : m_mapXobj) {
		m_pRootNodes->m_pXobjs->push_back(it.second);
	}

	return m_pRootNodes->QueryInterface(IID_IXobjCollection, (void**)pXobjColletion);
}

STDMETHODIMP CGalaxy::get_GalaxyData(BSTR bstrKey, VARIANT* pVal)
{
	return S_OK;
}

STDMETHODIMP CGalaxy::put_GalaxyData(BSTR bstrKey, VARIANT newVal)
{
	return S_OK;
}

STDMETHODIMP CGalaxy::Detach(void)
{
	if (::IsWindow(m_hWnd))
	{
		m_bDetached = true;
		::ShowWindow(m_pWorkXobj->m_pHostWnd->m_hWnd, SW_HIDE);
		RECT rect;
		m_pWorkXobj->m_pHostWnd->GetWindowRect(&rect);
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
		::ShowWindow(m_pWorkXobj->m_pHostWnd->m_hWnd, SW_SHOW);
		SubclassWindow(m_hHostWnd);
	}
	HostPosChanged();
	return S_OK;
}

STDMETHODIMP CGalaxy::ModifyHost(LONGLONG hHostWnd)
{
	HWND _hHostWnd = (HWND)hHostWnd;
	if (!::IsWindow(_hHostWnd) || m_hWnd == _hHostWnd)
	{
		return S_OK;
	}
	if (m_pWorkXobj == nullptr)
		return S_FALSE;
	HWND hParent = (HWND)::GetParent(_hHostWnd);
	CWindow m_Parent;
	RECT rc;
	m_pWorkXobj->m_pHostWnd->GetWindowRect(&rc);
	if (::IsWindow(m_hWnd)) {
		HWND hTangramWnd = m_pGalaxyCluster->m_hWnd;
		auto it = g_pCosmos->m_mapWindowPage.find(hTangramWnd);
		if (it != g_pCosmos->m_mapWindowPage.end())
			g_pCosmos->m_mapWindowPage.erase(it);
		m_pGalaxyCluster->m_hWnd = hTangramWnd;
		g_pCosmos->m_mapWindowPage[hTangramWnd] = m_pGalaxyCluster;

		auto it2 = m_pGalaxyCluster->m_mapGalaxy.find(m_hWnd);
		if (it2 != m_pGalaxyCluster->m_mapGalaxy.end()) {
			m_pGalaxyCluster->m_mapGalaxy.erase(it2);
			m_pGalaxyCluster->m_mapGalaxy[_hHostWnd] = this;
			m_pGalaxyCluster->m_mapWnd[m_strGalaxyName] = _hHostWnd;
			DWORD dwID = ::GetWindowThreadProcessId(_hHostWnd, NULL);
			TRACE(_T("OpenEx ThreadInfo:%x\n"), dwID);
			CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
			theUniverse.SetHook(dwID);
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
	for (auto it : m_mapXobj)
	{
		if (it.second != m_pWorkXobj)
		{
			::SetParent(it.second->m_pHostWnd->m_hWnd, m_pWorkXobj->m_pHostWnd->m_hWnd);
		}
	}
	::SetParent(m_pWorkXobj->m_pHostWnd->m_hWnd, ::GetParent(_hHostWnd));
	::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
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

STDMETHODIMP CGalaxy::get_VisibleXobj(IXobj** pVal)
{
	if (m_pWorkXobj)
		*pVal = m_pWorkXobj;
	return S_OK;
}

STDMETHODIMP CGalaxy::get_GalaxyCluster(IGalaxyCluster** pVal)
{
	if (m_pGalaxyCluster)
		*pVal = m_pGalaxyCluster;

	return S_OK;
}

STDMETHODIMP CGalaxy::Observe(BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj)
{
	if (::GetWindowLong(m_hWnd, GWL_STYLE) & MDIS_ALLCHILDSTYLES)
		m_nGalaxyType = GalaxyType::MDIClientGalaxy;
	CString _strXml = OLE2T(bstrXml);
	DWORD dwID = ::GetWindowThreadProcessId(m_hHostWnd, NULL);
	TRACE(_T("OpenEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
	theUniverse.SetHook(dwID);

	CString strCurrentKey = OLE2T(bstrKey);
	if (strCurrentKey == _T(""))
		strCurrentKey = _T("default");
	if (m_strCurrentKey != strCurrentKey)
	{
		m_strLastKey = m_strCurrentKey;
		m_strCurrentKey = strCurrentKey;
	}
	g_pCosmos->m_pGalaxy = this;

	m_strCurrentKey = m_strCurrentKey.MakeLower();
	g_pCosmos->m_strCurrentKey = m_strCurrentKey;
	CString strXml = _T("");
	auto it = m_mapXobj.find(m_strCurrentKey);

	CXobj* pOldNode = m_pWorkXobj;
	if (it != m_mapXobj.end())
		m_pWorkXobj = it->second;
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
				auto it = g_pCosmos->m_mapValInfo.find(m_strGalaxyName + L"_" + m_strCurrentKey);
				if (it != g_pCosmos->m_mapValInfo.end())
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
		m_pWorkXobj = g_pCosmos->ObserveEx((long)m_hHostWnd, _T(""), strXml);
		if (m_pWorkXobj == nullptr)
			return S_FALSE;
		if (::GetWindowLong(::GetParent(m_hWnd), GWL_EXSTYLE) & WS_EX_MDICHILD)
			m_bMDIChild = true;
	}
	m_pBindingXobj = m_pWorkXobj->m_pXobjShareData->m_pHostClientView ? m_pWorkXobj->m_pXobjShareData->m_pHostClientView->m_pXobj : nullptr;

	g_pCosmos->m_strCurrentKey = _T("");
	*ppRetXobj = (IXobj*)m_pWorkXobj;
	for (auto it : g_pCosmos->m_mapCosmosAppProxy)
	{
		it.second->OnObserverComplete(m_hHostWnd, strXml, m_pWorkXobj);
	}
	if (g_pCosmos->m_pCosmosAppProxy)
		g_pCosmos->m_pCosmosAppProxy->OnObserverComplete(m_hHostWnd, strXml, m_pWorkXobj);

	if (pOldNode && pOldNode != m_pWorkXobj)
	{
		RECT  rc;
		if (::IsWindow(pOldNode->m_pHostWnd->m_hWnd))
			::GetWindowRect(pOldNode->m_pHostWnd->m_hWnd, &rc);
		CWnd* pWnd = m_pWorkXobj->m_pHostWnd;
		HWND hParent = ::GetParent(m_hWnd);

		CWnd::FromHandle(hParent)->ScreenToClient(&rc);
		for (auto it : m_mapXobj)
		{
			HWND hwnd = it.second->m_pHostWnd->m_hWnd;
			BOOL bTop = (it.second == m_pWorkXobj);
			it.second->m_bTopObj = bTop;
			::SetWindowLongPtr(hwnd, GWLP_ID, bTop ? m_pWorkXobj->m_nID : 0);
			::SetParent(hwnd, bTop ? hParent : pWnd->m_hWnd);
			if (!bTop)
			{
				::ShowWindow(hwnd, SW_HIDE);
			}
		}

		::SetWindowPos(pWnd->m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);

		if (m_pWorkXobj != nullptr) {
			if (m_pWorkXobj->m_nViewType != Grid) {
				if (m_pWorkXobj->m_pHostWnd)
					m_pWorkXobj->m_pHostWnd->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, 0);
			}

			HRESULT hr = S_OK;
			int cConnections = g_pCosmos->m_vec.GetSize();
			if (cConnections) {
				CComVariant avarParams[3];
				avarParams[2] = (long)m_hHostWnd;
				avarParams[2].vt = VT_I4;
				avarParams[1] = strXml.AllocSysString();
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = (IXobj*)m_pWorkXobj;
				avarParams[0].vt = VT_DISPATCH;
				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				for (int iConnection = 0; iConnection < cConnections; iConnection++) {
					g_pCosmos->Lock();
					CComPtr<IUnknown> punkConnection = g_pCosmos->m_vec.GetAt(iConnection);
					g_pCosmos->Unlock();
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
	if (m_pBindingXobj)
	{
		CXobj* _pHostNode = m_pBindingXobj;
		if (_pHostNode->m_pHostGalaxy)
		{
			CGalaxy* _pGalaxy = _pHostNode->m_pHostGalaxy;
			while (_pGalaxy)
			{
				_pHostNode = _pGalaxy->m_pBindingXobj;
				if (_pHostNode && _pHostNode->m_pHostGalaxy)
					_pGalaxy = _pHostNode->m_pHostGalaxy;
				else
					break;
			}
		}
	}
	//end Add 20200218
	if (m_pWorkXobj->m_pHostGalaxy)
	{
		IXobj* pXobj = nullptr;
		m_pWorkXobj->m_pHostGalaxy->Observe(CComBSTR(m_pWorkXobj->m_pHostGalaxy->m_strCurrentKey), CComBSTR(""), &pXobj);
	}
	for (auto it : m_pWorkXobj->m_mapExtendNode)
	{
		IXobj* pXobj = nullptr;
		it.first->Observe(CComBSTR(it.second), CComBSTR(""), &pXobj);
	}

	::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20180115);
	return S_OK;
}

STDMETHODIMP CGalaxy::get_CurrentNavigateKey(BSTR* pVal)
{
	*pVal = m_strCurrentKey.AllocSysString();
	return S_OK;
}

void CGalaxy::Destroy()
{
}

void CGalaxy::OnFinalMessage(HWND hWnd)
{
	HWND hParent = ::GetParent(hWnd);
	if (::IsWindow(hParent))
		::PostMessage(hParent, WM_COSMOSMSG, 4096, 0);
	__super::OnFinalMessage(hWnd);
}

LRESULT CGalaxy::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if ((long)(g_pCosmos->m_pActiveAppProxy) > 1)
	{
		HWND hMenuWnd = g_pCosmos->m_pActiveAppProxy->GetActivePopupMenu(nullptr);
		if (::IsWindow(hMenuWnd))
			::PostMessage(hMenuWnd, WM_CLOSE, 0, 0);
	}

	{
		g_pCosmos->m_hActiveWnd = 0;
		g_pCosmos->m_bWinFormActived = false;
		g_pCosmos->m_pActiveXobj = nullptr;
		g_pCosmos->m_pGalaxy = nullptr;
	}
	//bug fix for winform:
	//++++++++++++++++begin+++++++++++
	HWND hTop = ::GetAncestor(m_hWnd, GA_ROOT);
	if (hTop != nullptr && g_pCosmos->m_pCLRProxy)
	{
		g_pCosmos->m_bWinFormActived = g_pCosmos->m_pCLRProxy->IsWinForm(hTop);
		if (g_pCosmos->m_bWinFormActived)
		{
			g_pCosmos->m_hActiveWnd = hTop;
			g_pCosmos->m_pGalaxy = this;
		}
	}
	//++++++++++++++++end+++++++++++

	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pCosmos->m_pCLRProxy) {
		g_pCosmos->m_pCLRProxy->ReleaseCosmosObj((IGalaxy*)this);
	}
	m_pGalaxyCluster->BeforeDestory();

	DWORD dwID = ::GetWindowThreadProcessId(m_hWnd, NULL);
	TRACE(_T("OpenEx ThreadInfo:%x\n"), dwID);
	CommonThreadInfo* pThreadInfo = g_pCosmos->GetThreadInfo(dwID);
	if (pThreadInfo)
	{
		//theApp.SetHook(dwID);
		auto iter = pThreadInfo->m_mapGalaxy.find(m_hWnd);
		if (iter != pThreadInfo->m_mapGalaxy.end())
		{
			pThreadInfo->m_mapGalaxy.erase(iter);
			TRACE(_T("Cosmos Frame Removed from ThreadInfo Manager:%x\n"), m_hWnd);
		}
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CGalaxy::OnCosmosMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	switch (lParam)
	{
		case 20180115:
		{
			HostPosChanged();
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

LRESULT CGalaxy::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	LRESULT hr = DefWindowProc(uMsg, wParam, lParam);

	if (g_pCosmos->m_pBrowserFactory && g_pCosmos->m_bChromeNeedClosed)
		return hr;

	WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;

	if (m_pBindingXobj)
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
		::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_BOTTOM, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOACTIVATE | SWP_FRAMECHANGED); //| SWP_NOREDRAW);// ); 
		CXobj* _pHostNode = m_pBindingXobj;
		if (_pHostNode->m_pHostGalaxy)
		{
			CGalaxy* _pGalaxy = _pHostNode->m_pHostGalaxy;
			while (_pGalaxy)
			{
				if (_pGalaxy->m_pBindingXobj)
					_pHostNode = _pGalaxy->m_pBindingXobj;
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
			CXobj* _pParentNode = m_pBindingXobj->m_pParentObj;
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
					CXobjHelper* pWnd = (CXobjHelper*)_pParentNode->m_pHostWnd;
				}
				break;
				}
			}
			if (m_pContainerNode && m_pContainerNode->m_pParentObj && m_pContainerNode->m_pParentObj->m_nViewType == Grid)
			{
				if (m_pContainerNode->m_nViewType == CLRCtrl)
				{
					CXobjHelper* pXobjWnd = (CXobjHelper*)m_pContainerNode->m_pHostWnd;
					pXobjWnd->m_bNoMove = true;
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
		::SetWindowPos(m_pWorkXobj->m_pHostWnd->m_hWnd, HWND_TOP, lpwndpos->x, lpwndpos->y, lpwndpos->cx, lpwndpos->cy, lpwndpos->flags | SWP_NOSENDCHANGING | /*SWP_NOZORDER |*/ SWP_NOACTIVATE | SWP_FRAMECHANGED);
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
		lpwndpos->flags |= SWP_HIDEWINDOW;
	}

	if (m_bMDIChild)
		lpwndpos->flags |= SWP_NOZORDER;
	::InvalidateRect(::GetParent(m_hWnd), nullptr, true);
	return hr;
}

LRESULT CGalaxy::OnParentNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_pGalaxy = nullptr;
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
	return S_OK;
}

STDMETHODIMP CGalaxy::get_Count(long* pCount)
{
	*pCount = (long)m_mapXobj.size();
	return S_OK;
}

STDMETHODIMP CGalaxy::get_Xobj(VARIANT vIndex, IXobj** ppXobj)
{
	if (vIndex.vt == VT_I4)
	{
		long lCount = m_mapXobj.size();
		int iIndex = vIndex.lVal;
		if (iIndex < 0 || iIndex >= lCount) return E_INVALIDARG;

		auto it = m_mapXobj.begin();
		int iPos = 0;
		while (it != m_mapXobj.end())
		{
			if (iPos == iIndex) break;
			iPos++;
			it++;
		}

		CXobj* pXobj = it->second;
		*ppXobj = pXobj;
		return S_OK;
	}

	if (vIndex.vt == VT_BSTR)
	{
		CString strKey = OLE2T(vIndex.bstrVal);
		auto it = m_mapXobj.find(strKey);
		if (it != m_mapXobj.end())
		{
			*ppXobj = it->second;
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
		long nLen = (long)m_mapXobj.size();
		VARIANT* rgvar = new VARIANT[nLen];
		ZeroMemory(rgvar, sizeof(VARIANT) * nLen);
		VARIANT* pItem = rgvar;
		for (auto it : m_mapXobj)
		{
			IUnknown* pDisp = nullptr;
			CXobj* pObj = it.second;
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
	return S_OK;
}

STDMETHODIMP CGalaxy::get_CosmosDoc(ICosmosDoc** pVal)
{
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
	return S_FALSE;
}

STDMETHODIMP CGalaxy::get_HostWebPage(IWebPage** ppChromeWebPage)
{
	return S_FALSE;
}
