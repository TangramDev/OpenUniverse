/********************************************************************************
*					Open Universe - version 0.9.99								*
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

#pragma once

#include "BrowserWndOpenDisposition.h"

using namespace System;
using namespace System::Windows::Forms;

namespace Cosmos
{
	ref class Grid;
	ref class Quasar;

	public ref class Wormhole
	{
	public:
		Wormhole(CSession* pSession);

		CSession*	m_pWormhole;
		Object^ m_pHostObj = nullptr;

		void InsertString(String^ key, String^ value);
		void InsertLong(String^ key, long value);
		void InsertInt64(String^ key, __int64 value);
		void InsertFloat(String^ key, float value);
		String^ GetString(String^ key);
		long GetLong(String^ key);
		float GetFloat(String^ key);
		__int64 GetInt64(String^ key);
		void SendMessage();
	};

	public ref class Browser
	{
	public:
		IBrowser*			m_pWebBrowser;
		Browser(IBrowser* pChromeWebBrowser);
		void AddURLs(String^ urls);
		void OpenURL(String^ url, Disposition disposition, String^ key, String^ xml);
	};

	public ref class WebPage : public IWin32Window
	{
	public:
		HWND				m_hWnd;
		IWebPage*			m_pWebPage;
		CWebPageImpl*		m_pProxy;

		WebPage(IWebPage* m_pWebPage);

		property IntPtr Handle
		{
			virtual IntPtr get();
		}

		property Grid^ ParentGrid
		{
			Grid^ get();
		}

		property Quasar^ HostQuasar
		{
			Quasar^ get();
		}

		Form^ CreateForm(String^ strFormKey);
	};
}