/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103060044           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 *
 * Web Runtime is a new, subversive and Internet Oriented Development Technology
 * for Windows Desktop Software. Its core idea is that, relying on the open-source
 * project of Chromium, developers are allowed to use their familiar software
 * development technology to realize the Main Process of their application system
 * as the Browser Process of Modern Web Browser. Because the Main Process is realized
 * by the developers themselves, the application system developed in this way is
 * completely different from modern web browser. This kind of application will
 * integrate the infrastructure of modern web browser as a part of the system model,
 * the application system contains not only the browser window inherent in the browser,
 * but also other desktop elements oriented to the application, such as WinForms, Java
 * and other mature software elements.
 *
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
 *
 * Built-in Full-Featured Modern Web Browser Subsystem
 *		Since the main process of the application system is essentially a Browser
 * Process, the application system can naturally create a new browser window. The
 * difference with standard browsers is that we allow the browser window to be a child
 * window of other windows. in sub-window model, we see a brand-new scene: a multi-tab
 * browser sub-window, we think this structure is more suitable for organizing specific
 * applications. Of course, the application system fully supports the creation of an
 * independent browser window as a part of the complete application system, and the browser
 * window serves as a natural extension of the ecological chain to ensure the richness
 * and diversity of the application system content.
 * DOM Plus:
 *		Because a large number of mature binary components that conform to industry
 * standards have formed a wealth of software asset accumulation, web runtime allows
 * developers to regard these existing accumulation as new DOM elements and then become
 * a part of web pages, this starting point eventually forms "DOM Plus". DOM plus is a
 * natural extension of standard DOM for desktop applications, similar to the extension
 * from "real number system" to "complex number system" in mathematical theory. Undoubtedly,
 * for the development of mathematics, from "real number system" to "complex number system"
 * is a huge leap, which can be said to be a decisive change in the development of modern
 * mathematics. We hope that the evolution from "DOM" to "DOM plus" will have a similar
 * effect, which is bound to be a huge change from desktop software to Internet desktop software.
 *		Different from earlier IE, based on the sandbox mechanism of Modern Web Browser,
 * we let these binary elements run in the Browser Process, completely isolated from the
 * Browser's Renderer Process. This makes full use of the security mechanisms of modern
 * browsers and also takes into account the accumulation of mature software component
 * technologies. Based on such considerations, existing development technologies such as
 * .NET, COM, Java, etc. can be fully applied to Internet software development.
 *
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

 // Galaxy.cpp : implementation file of CGalaxy
 //

#include "stdafx.h"
#include "UniverseApp.h"
#include "Cosmos.h"
#include "GridWnd.h"
#include "XobjWnd.h"
#include "Xobj.h"
#include "Galaxy.h"
#include "Wormhole.h"

CWinForm::CWinForm(void)
{
	m_nState = -1;
	m_bMdiForm = false;
	m_pWormhole = nullptr;
	m_pOwnerHtmlWnd = nullptr;
	m_strXml = m_strKey = _T("");
}

CWinForm::~CWinForm(void)
{
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
	if (LOWORD(wParam) != WA_INACTIVE) {
		g_pCosmos->m_pActiveWinFormWnd = this;
		if (g_pCosmos->m_pCLRProxy)
		{
			g_pCosmos->m_pCLRProxy->OnWinFormActivate(m_hWnd, LOWORD(wParam));
		}
	}
	else
	{
	}
	return lRes;
}

LRESULT CWinForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (g_pCosmos->m_mapBrowserWnd.size())
	{
		CSession* pSession = (CSession*)::GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
		if (pSession)
		{
			int nCount = 0;
			if (m_bMainForm)
			{
				nCount = g_pCosmos->m_mapNeedQueryOnClose.size();
				if (nCount > 1)
				{
					pSession->InsertLong(_T("FormAppNeedClosed"), nCount);
				}
				for (auto it : g_pCosmos->m_mapNeedQueryOnClose)
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
	if (g_pCosmos->m_pActiveWinFormWnd == this)
		g_pCosmos->m_pActiveWinFormWnd = nullptr;
	auto it = g_pCosmos->m_mapNeedQueryOnClose.find(m_hWnd);
	if (it != g_pCosmos->m_mapNeedQueryOnClose.end())
		g_pCosmos->m_mapNeedQueryOnClose.erase(it);
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
	case 20201029:
	{
		if (wParam == 0)
			m_bMainForm = true;
		else
		{
			g_pCosmos->m_mapNeedQueryOnClose[m_hWnd] = this;
		}
	}
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
	g_pCosmos->m_pActiveHtmlWnd = nullptr;
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
	m_pCurrentIPCMsg = nullptr;
	m_strLastKey = _T("");
	m_strCurrentKey = _T("");
	m_strCurrentXml = _T("");
	m_strGalaxyName = _T("");
	m_bMDIChild = false;
	m_bDetached = false;
	m_pWebPageWnd = nullptr;
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
		UINT flag = SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOREDRAW;
		dwh = ::DeferWindowPos(dwh, hwnd, HWND_TOP,
			rt1.left,
			rt1.top,
			rt1.right - rt1.left,
			rt1.bottom - rt1.top,
			flag
		);
		EndDeferWindowPos(dwh);
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

CXobj* CGalaxy::OpenXtmlDocument(CTangramXmlParse* _pParse, CString strKey)
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
	Create();
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

	m_pWorkXobj->m_strCosmosXml = _pParse->xml();
	return m_pWorkXobj;
}

BOOL CGalaxy::Create()
{
	if (::IsWindow(m_hWnd) == false)
		SubclassWindow(m_hHostWnd);

	m_pWorkXobj->InitWndXobj();
	HWND hWnd = NULL;
	if (m_pWorkXobj->m_pObjClsInfo) {
		RECT rc;
		CWnd* pParentWnd = CWnd::FromHandle(::GetParent(m_hWnd));
		//CWnd* pParentWnd = CWnd::FromHandle(hPWnd);
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
		m_pWorkXobj = g_pCosmos->ObserveEx((long)m_hHostWnd, _T(""), strXml);
		if (m_pWorkXobj == nullptr)
			return S_FALSE;
		if (::GetWindowLong(::GetParent(m_hWnd), GWL_EXSTYLE) & WS_EX_MDICHILD)
			m_bMDIChild = true;
	}
	m_pBindingXobj = m_pWorkXobj->m_pXobjShareData->m_pHostClientView ? m_pWorkXobj->m_pXobjShareData->m_pHostClientView->m_pXobj : nullptr;
	if (m_strGalaxyName == _T("default"))
	{
		CString strName = m_pWorkXobj->m_pHostParse->attr(_T("galaxy"), _T(""));
		if (strName != _T(""))
			m_strGalaxyName = strName;
	}

	g_pCosmos->m_strCurrentKey = _T("");
	*ppRetXobj = (IXobj*)m_pWorkXobj;
	for (auto& it : g_pCosmos->m_mapCosmosAppProxy)
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
		if (_pHostNode && m_pWebPageWnd)
		{
			CXobj* pXobj = _pHostNode->m_pRootObj;
			if (pXobj->m_strLastIPCMsgID != _T(""))
			{
				IPCMsg pIPCInfo;
				pIPCInfo.m_strId = pXobj->m_strLastIPCMsgID;
				pIPCInfo.m_strParam1 = pXobj->m_strLastIPCParam1;
				pIPCInfo.m_strParam2 = pXobj->m_strLastIPCParam2;
				pIPCInfo.m_strParam3 = pXobj->m_strLastIPCParam5;
				pIPCInfo.m_strParam4 = pXobj->m_strLastIPCParam4;
				pIPCInfo.m_strParam5 = pXobj->m_strLastIPCParam3;
				m_pWebPageWnd->m_pChromeRenderFrameHost->SendCosmosMessage(&pIPCInfo);
			}
			g_pCosmos->m_pCurrentIPCMsg = nullptr;
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
	if (m_pHostWebBrowserWnd)
	{
		IXobj* pXobj = nullptr;
		CComPtr<IXobjCollection> pCol;
		long nCount = 0;
		m_pWorkXobj->GetXobjs(CComBSTR(m_strHostWebBrowserNodeName), &pXobj, &pCol, &nCount);
		if (pXobj)
		{
			CXobj* _pXobj = (CXobj*)pXobj;
			if (_pXobj->m_nViewType == BlankView)
			{
				CGalaxy* _pGalaxy = nullptr;
				if (_pXobj->m_pHostGalaxy)
				{
					_pGalaxy = _pXobj->m_pHostGalaxy;
					while (_pGalaxy)
					{
						if (_pGalaxy->m_pHostWebBrowserNode)
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						if (_pXobj && _pXobj->m_pHostGalaxy)
						{
							_pGalaxy = _pXobj->m_pHostGalaxy;
							_pXobj = _pGalaxy->m_pHostWebBrowserNode;
						}
						else
							break;
					}
				}
				if (m_pHostWebBrowserWnd->m_pParentXobj == nullptr)
				{
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
				}
				else if (m_pHostWebBrowserWnd->m_pParentXobj != _pXobj)//&&_pXobj->m_pHostWnd->IsWindowVisible())
				{
					m_pHostWebBrowserWnd->m_pParentXobj->m_pWebBrowser = nullptr;
					m_pHostWebBrowserWnd->m_pParentXobj = _pXobj;
					_pXobj->m_pWebBrowser = m_pHostWebBrowserWnd;
					::SetParent(m_pHostWebBrowserWnd->m_hWnd, _pXobj->m_pHostWnd->m_hWnd);
					::SetWindowPos(m_pHostWebBrowserWnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOREDRAW);
				}
			}
		}
		//m_pHostWebBrowserWnd->m_pBrowser->LayoutBrowser();
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
	if (g_pCosmos->m_pActiveHtmlWnd)
	{
		g_pCosmos->m_pActiveHtmlWnd = nullptr;
	}

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
		::PostMessage(m_hWnd, WM_COSMOSMSG, 0, 20210216);
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
					CXobjWnd* pWnd = (CXobjWnd*)_pParentNode->m_pHostWnd;
				}
				break;
				}
			}
			if (m_pContainerNode && m_pContainerNode->m_pParentObj && m_pContainerNode->m_pParentObj->m_nViewType == Grid)
			{
				if (m_pContainerNode->m_nViewType == CLRCtrl)
				{
					CXobjWnd* pXobjWnd = (CXobjWnd*)m_pContainerNode->m_pHostWnd;
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
	if (::IsWindowVisible(m_hWnd))
		::InvalidateRect(m_hWnd, nullptr, true);
	return hr;
}

LRESULT CGalaxy::OnParentNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_pCosmos->m_pGalaxy = nullptr;
	return DefWindowProc(uMsg, wParam, lParam);
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
			auto it = g_pCosmos->m_mapBrowserWnd.find(hPWnd);
			if (it != g_pCosmos->m_mapBrowserWnd.end())
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
