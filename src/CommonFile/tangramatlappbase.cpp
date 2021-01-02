/********************************************************************************
*					Tangram Library - version 10.0.0							*
*********************************************************************************
* Copyright (C) 2002-2019 by Tangram Team.   All Rights Reserved.				*
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
* mailto:tangramteam@outlook.com
* https://www.tangramteam.com
*
********************************************************************************/

//#include "stdafx.h"
#include "Tangram.c"
#include "TangramatlAppBase.h"
#include "TangramXmlParse.cpp"

typedef CCosmosImpl* (__stdcall* GetCosmosImpl)();
typedef CTangramBase* (__stdcall* GetCosmosBase)();
typedef ICosmos* (__stdcall* GetCosmos)();

typedef int(__stdcall* _InitApp)(bool bSupportCrashReporting, void*, Universe::CTangramApplicationImpl*);
typedef bool(__stdcall* _IsBrowserModel)(bool bSupportCrashReporting, void*, Universe::CTangramApplicationImpl*);
_InitApp FuncInitApp;
_IsBrowserModel FuncIsBrowserModel;

namespace TangramCommon
{
	ICosmos* g_pHubble = nullptr;
	Universe::CChromeBrowserCreatorImpl* g_pCreatorImpl = nullptr;
	CTangramApplicationImpl theAppImpl;

	CTangramWindow::CTangramWindow()
	{
		m_pChild = NULL;
	}

	CTangramWindow::~CTangramWindow()
	{
	}

	void CTangramWindow::OnFinalMessage(HWND hWnd)
	{
		CWindowImpl::OnFinalMessage(hWnd);
		delete this;
	}


	LRESULT CTangramWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		ATLTRACE(_T("OnCreate Tangram Document :%x\n"), this);
		HWND hPWnd = ::GetParent(m_hWnd);
		if (hPWnd == NULL)
		{
			RECT rc;
			::GetClientRect(m_hWnd, &rc);
			m_pChild = new CTangramWindow();
			m_pChild->Create(m_hWnd, ATL::_U_RECT(&rc), _T(""), WS_CHILD | WS_VISIBLE, 0, 0xe900U, 0);
			IContentLoaderManager* pContentLoaderManager = nullptr;
			g_pHubble->CreateContentLoaderManager((__int64)m_hWnd, &pContentLoaderManager);
			if (pContentLoaderManager)
			{
				IXobj* pGrid = nullptr;
				pContentLoaderManager->CreateContentLoaderWithDefaultNode((__int64)m_pChild->m_hWnd, CComBSTR(""), CComBSTR(""), CComBSTR(""), true, &pGrid);
			}
		}
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CTangramWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		ATLTRACE(_T("Close Tangram Window :%x\n"), this);
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CTangramWindow::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CTangramWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		if (m_pChild)
		{
			HWND hDesignerWnd = theAppImpl.m_pUniverseAppProxy->m_pHubbleImpl->m_hHostWnd;
			if(::IsWindow(hDesignerWnd))
				::DestroyWindow(hDesignerWnd);
			PostQuitMessage(0);
		}
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT CTangramWindow::OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		LRESULT hr = DefWindowProc(uMsg, wParam, lParam);

		if (m_pChild&&::IsWindow(m_pChild->m_hWnd))
		{
			WINDOWPOS* lpwndpos = (WINDOWPOS*)lParam;
			RECT rc;
			::GetClientRect(m_hWnd, &rc);
			::SetWindowPos(m_pChild->m_hWnd, HWND_BOTTOM, 0, 0, rc.right, rc.bottom, SWP_NOREDRAW | SWP_NOACTIVATE);
		}
		return hr;
	}

	CTangramApplicationImpl::CTangramApplicationImpl()
	{
		m_pUniverseAppProxy	= nullptr;
	}

	CTangramApplicationImpl::~CTangramApplicationImpl()
	{
		OutputDebugString(_T("delete CTangramApplicationImpl\n"));
		//HMODULE hModule = ::GetModuleHandle(L"universe.dll");
		//if (hModule) {
		//	typedef int(__stdcall* FreeCLR)();
		//	FreeCLR FuncFreeCLR;
		//	FuncFreeCLR = (FreeCLR)GetProcAddress(hModule, "FreeCLR");
		//	if (FuncFreeCLR != NULL) {
		//		FuncFreeCLR();
		//	}
		//	BOOL bUnload = ::FreeLibrary(hModule);
		//	while (bUnload)
		//		bUnload = ::FreeLibrary(hModule);
		//}
		
		//hModule = ::GetModuleHandle(L"chrome.dll");
		//if (hModule) {
		//	BOOL bUnload = ::FreeLibrary(hModule);
		//	while (bUnload)
		//		bUnload = ::FreeLibrary(hModule);
		//}
		
		//hModule = ::GetModuleHandle(L"tangram_chrome_rt.dll");
		//if (hModule) {
		//	BOOL bUnload = ::FreeLibrary(hModule);
		//	//while (bUnload)
		//	//	bUnload = ::FreeLibrary(hModule);
		//}
	}

	HRESULT CTangramApplicationImpl::InitHubble(BOOL bUsingDefaultUI, CTangramAppProxy* pProxy)
	{
		return S_OK;
		//m_bUsingDefaultUI = bUsingDefaultUI;
		//m_pUniverseAppProxy = pProxy;
		//m_pUniverseAppProxy->m_bAutoDelete = FALSE;
		//BSTR bstrKey = ::SysAllocString(L"TangramApplicationImpl");
		//HRESULT hr = S_FALSE;
		//if (g_pHubble == nullptr)
		//{
		//	hr = CoCreateInstance(CLSID_Tangram, NULL, CLSCTX_ALL, IID_ICosmos, (LPVOID*)&g_pHubble);
		//}
		//hr = g_pHubble->put_AppKeyValue(bstrKey, CComVariant((__int64)(static_cast<CTangramApplicationImpl*>(this))));
		//::SysFreeString(bstrKey);
		//return m_pJVMenv ? S_FALSE : hr;
	}

	CTangramAppProxy::CTangramAppProxy()
	{
		m_bAutoDelete = TRUE;
		m_hMainWnd = nullptr;
		m_hClosingFrame = m_hCreatingView = nullptr;
		m_pvoid = nullptr;
		m_pCurDocProxy = nullptr;
		m_strProxyName = _T("");
		m_strProxyID = _T("");
		m_strClosingFrameID = _T("");
		m_strCreatingFrameTitle = _T("");

		m_bCreatingNewFrame = FALSE;
		m_mapHubbleDoc.clear();

		m_nFrameIndex = 0;
		m_strAppKey = _T("");
	}

	BOOL CTangramAppProxy::InitHubble(void* pVoid)
	{
		return TRUE;
	}

	HWND CTangramAppProxy::CreateNewFrame(CString strFrameKey)
	{
		return nullptr;
	};

	void CTangramAppProxy::RegistWndRunTimeClass(void* clsInfo)
	{
	}

	void CTangramAppProxy::OnActiveMainFrame(HWND hWnd)
	{
	}

	BOOL CTangramAppProxy::HubbleSaveAllModified()
	{
		return TRUE;
	}

	int CTangramAppProxy::OnDestroyMainFrame(CString strID, int nMainFrameCount, int nWndType)
	{
		return -1;
	}

	HWND CTangramAppProxy::CreateWindowObj(LPCTSTR _strClsName, IXobj* pGrid, HWND hParent)
	{
		return nullptr; 
	}

	LRESULT CTangramAppProxy::OnForegroundIdleProc()
	{
		return 0;
	}

	BOOL CTangramAppProxy::UniversePreTranslateMessage(MSG* pMsg)
	{
		return TRUE;
	}

	void CTangramAppProxy::MouseMoveProxy(HWND hWnd)
	{
	}

	HWND CTangramAppProxy::GetActivePopupMenu(HWND hWnd)
	{
		return nullptr;
	}

	ICosmosDoc* CTangramAppProxy::OpenDocument(void* pDocTemplate, CString strFile, BOOL bNewFrame)
	{
		return nullptr;
	}

	ICosmosDoc* CTangramAppProxy::CreateNewDocument(LPCTSTR lpszFrameID, LPCTSTR lpszAppTitle, void* pDocTemplate, BOOL bNewFrame)
	{
		return nullptr;
	};

	void CTangramAppProxy::OnActiveDocument(ICosmosDoc* ActiveDoc, IXobj* pGridInDoc, IXobj* pGridInCtrlBar, HWND hCtrlBar)
	{
	}

	void CTangramAppProxy::RemoveDoc(LONGLONG llDocID)
	{
		auto it = m_mapHubbleDoc.find(llDocID);
		if (it != m_mapHubbleDoc.end())
			m_mapHubbleDoc.erase(it);
	}

	void CTangramAppProxy::AddDoc(LONGLONG llDocID, ICosmosDoc* pDoc)
	{
		m_mapHubbleDoc[llDocID] = pDoc;
	}

	ICosmosDoc* CTangramAppProxy::GetDoc(LONGLONG llDocID)
	{
		auto it = m_mapHubbleDoc.find(llDocID);
		if (it != m_mapHubbleDoc.end())
			return it->second;
		return nullptr; 
	}
}
