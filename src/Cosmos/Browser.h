/********************************************************************************
 *					DOM Plus for Application - Version 1.1.7.40
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#pragma once

#include "BrowserWndOpenDisposition.h"

using namespace System;
using namespace System::Windows::Forms;

namespace Universe
{
	ref class Xobj;
	ref class Galaxy;

	public ref class Wormhole
	{
	public:
		Wormhole(CSession* pSession);

		CSession* m_pSession;
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
		//property String^ EventBindInfo
		//{
		//	String^ get()
		//	{
		//		return m_strEvents->ToLower();
		//	};
		//	void set(String^ strData)
		//	{
		//		m_strEvents = strData->ToLower();
		//	};
		//}
		bool isBindCLRObjToWebPage(Object^ obj, String^ eventName);
		void AddEventInfo(Object^ obj, String^ eventName);

		String^ m_strEvents = L"";
	};

	public ref class Browser
	{
	public:
		Browser(IBrowser* pBrowser);
		void AddURLs(String^ urls);
		void OpenURL(String^ url, Disposition disposition, String^ key, String^ xml);
	private:
		IBrowser*			m_pWebBrowser;
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

		property Xobj^ ParentXobj
		{
			Xobj^ get();
		}

		property Galaxy^ HostGalaxy
		{
			Galaxy^ get();
		}

		Form^ CreateForm(String^ strFormKey);
	};
}