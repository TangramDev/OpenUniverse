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
* OUTLINED IN THE GPL LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* http://www.tangramteam.com/
*
********************************************************************************/

#pragma once

#include "BrowserWndOpenDisposition.h"

using namespace System;
using namespace System::Windows::Forms;

namespace DOMPlus
{
	ref class Xobj;
	ref class Galaxy;

	public ref class Wormhole
	{
	public:
		CSession*	m_pCosmosSession;
		Wormhole(CSession* m_pCosmosSession);
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

		String^ m_strEvents = L"";
		property String^ EventBindInfo
		{
			String^ get()
			{
				return m_strEvents->ToLower();
			};
			void set(String^ strData)
			{
				m_strEvents = strData->ToLower();
			};
		}

		bool isBindCLRObjToWebPage(Object^ obj);
	};

	public ref class Browser
	{
	public:
		IBrowser* m_pWebBrowser;
		Browser(IBrowser* pWebBrowser);
		void AddURLs(String^ urls);
		void OpenURL(String^ url, Disposition disposition, String^ key, String^ xml);
	};

	public ref class WebPage : public IWin32Window
	{
	public:
		HWND			m_hWnd;
		IWebPage*		m_pWebPage;
		CWebPageImpl*	m_pProxy;
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