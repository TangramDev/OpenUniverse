/********************************************************************************
*					Open Universe - version 1.0.1.13							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
********************************************************************************/

// UniverseApp.h : DLL

#pragma once

namespace Web
{
	class CBrowser;
	class CWebPage;
}

using namespace Web;

typedef HRESULT(__stdcall* HubbleCLRCreateInstance)(REFCLSID clsid, REFIID riid, LPVOID* ppInterface);

class CUniverse :
	public CWinApp,
	public CAtlDllModuleT< CUniverse >
{
public:
	CUniverse();
	~CUniverse();

	bool				m_bHostCLR;

	void SetHook(DWORD ThreadID);

	static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HubbleWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	static LRESULT CALLBACK HubbleMsgWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	static LRESULT CALLBACK HubbleExtendedWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	static LRESULT CALLBACK ForegroundIdleProc(int nCode, WPARAM wParam, LPARAM lParam);
	DECLARE_LIBID(LIBID_Universe)

private:
	int ExitInstance();
	BOOL InitInstance();

	static LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
};

extern CUniverse theUniverse;
extern CHubble* g_pHubble;
