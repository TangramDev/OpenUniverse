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
* OUTLINED IN THE MIT LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
********************************************************************************/

// Universe.h : DLL

#pragma once
#include <jni.h>

namespace Browser
{
	class CBrowser;
	class CWebPage;
}

using namespace Browser;

typedef HRESULT(__stdcall* CosmosCLRCreateInstance)(REFCLSID clsid, REFIID riid, LPVOID* ppInterface);

class CHelperWnd :
	public CWindowImpl<CHelperWnd, CWindow>
{
public:
	CHelperWnd(void)
	{
		m_strID = _T("");
	};

	~CHelperWnd(void) {};

	CString m_strID;
	DECLARE_WND_CLASS(_T("Tangram Helper Window Class"))
	BEGIN_MSG_MAP(CHelperWnd)
	END_MSG_MAP()
	void OnFinalMessage(HWND hWnd);
};

class CWebHelperWnd :
	public CWindowImpl<CWebHelperWnd, CWindow>
{
public:
	CWebHelperWnd(void)
	{
		m_hWebHost = nullptr;
	};

	~CWebHelperWnd(void) {};

	HWND m_hWebHost;
	BEGIN_MSG_MAP(CHelperWnd)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
	END_MSG_MAP()
	void OnFinalMessage(HWND hWnd);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CTBToolboxPaneWnd :
	public CWindowImpl<CTBToolboxPaneWnd, CWindow>
{
public:
	CTBToolboxPaneWnd(void)
	{
	};

	~CTBToolboxPaneWnd(void) {};

	BEGIN_MSG_MAP(CTBToolboxPaneWnd)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()
	void OnFinalMessage(HWND hWnd);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CGenericPaneWnd :
	public CWindowImpl<CGenericPaneWnd, CWindow>
{
public:
	CGenericPaneWnd(void)
	{
		m_hChild = NULL;
	};

	~CGenericPaneWnd(void) {};
	HWND m_hChild;
	CGalaxy* m_pGalaxy = nullptr;
	CString m_strToolType = _T("");
	BEGIN_MSG_MAP(CGenericPaneWnd)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
		MESSAGE_HANDLER(WM_HUBBLE_DATA, OnCosmosData)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanging)
	END_MSG_MAP()
	void OnFinalMessage(HWND hWnd);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCosmosMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCosmosData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CUniverse :
	public CWinApp,
	public CAtlDllModuleT< CUniverse >
{
public:
	CUniverse();
	~CUniverse();

	bool				m_bHostCLR;

	CString GetFileVer();
	void SetHook(DWORD ThreadID);

	static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CosmosWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	static LRESULT CALLBACK CosmosMsgWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	static LRESULT CALLBACK CosmosExtendedWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	static LRESULT CALLBACK ForegroundIdleProc(int nCode, WPARAM wParam, LPARAM lParam);

	DECLARE_LIBID(LIBID_DOMPlus)

private:
	int ExitInstance();
	BOOL InitInstance();

	static LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
};

//TANGRAM_OBJECT_ENTRY_AUTO(CLSID_DOMPlus, CUniverse)

extern CUniverse theApp;
extern CCosmos* g_pCosmos;
