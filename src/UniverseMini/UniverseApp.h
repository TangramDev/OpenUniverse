/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101180012
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

// UniverseApp.h : DLL

#pragma once

typedef HRESULT(__stdcall* CosmosCLRCreateInstance)(REFCLSID clsid, REFIID riid, LPVOID* ppInterface);

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
	static LRESULT CALLBACK CosmosWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	static LRESULT CALLBACK CosmosMsgWndProc(_In_ HWND hWnd, UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	static LRESULT CALLBACK ForegroundIdleProc(int nCode, WPARAM wParam, LPARAM lParam);
	DECLARE_LIBID(LIBID_Universe)

private:
	int ExitInstance();
	BOOL InitInstance();

	static LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
};

extern CUniverse theUniverse;
extern CCosmos* g_pCosmos;
