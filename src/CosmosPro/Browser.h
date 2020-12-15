#pragma once

#include "BrowserWndOpenDisposition.h"

using namespace System;
using namespace System::Windows::Forms;

namespace Cosmos
{
	ref class Grid;
	ref class Galaxy;

	public ref class Wormhole
	{
	public:
		CSession*	m_pHubbleSession;
		Wormhole(CSession* m_pHubbleSession);
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

		property Grid^ ParentGrid
		{
			Grid^ get();
		}

		property Galaxy^ HostGalaxy
		{
			Galaxy^ get();
		}

		Form^ CreateForm(String^ strFormKey);
	};

	public ref class CloudForm
	{
	public:
		HWND					m_hWnd;
		IBrowser*				m_pChromeWebBrowserHost;
		CloudForm(IBrowser* pBrowser);
	};
}