/********************************************************************************
*					Open Universe - version 0.0.1								*
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
	ref class Star;
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

	/*
	* 赫歇尔：现代天文学之父
	*/
	public ref class Herschel
	{
	public:
		IBrowser*			m_pWebBrowser;
		Herschel(IBrowser* pChromeWebBrowser);
		void AddURLs(String^ urls);
		void OpenURL(String^ url, Disposition disposition, String^ key, String^ xml);
	};

	public ref class Galileo : public IWin32Window
	{
	public:
		HWND							m_hWnd;
		IWebPage	*					m_pChromeWebPage;
		CGalileoImpl*	m_pProxy;
		Galileo(IWebPage* m_pChromeWebPage);

		property IntPtr Handle
		{
			virtual IntPtr get();
		}

		property Star^ ParentStar
		{
			Star^ get();
		}

		property Quasar^ HostQuasar
		{
			Quasar^ get();
		}

		Form^ CreateForm(String^ strFormKey);
	};
}