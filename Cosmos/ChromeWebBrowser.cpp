/********************************************************************************
*					Open Universe - version 0.9.0								*
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

#include "stdafx.h"
#include "dllmain.h"
#include "CosmosProxy.h"
#include "ChromeWebBrowser.h"
#include "Cosmos.h"

using namespace std;
using namespace System;
using namespace System::IO;
using namespace System::Windows;
using namespace System::Threading;
using namespace System::Reflection;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::ComponentModel;
using namespace System::Threading::Tasks;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

using System::Runtime::InteropServices::Marshal;

namespace Cosmos
{
	Wormhole::Wormhole(CSession* pSession)
	{
		m_pWormhole = pSession;
	}

	void Wormhole::InsertString(String^ key, String^ value)
	{
		if(m_pWormhole)
			m_pWormhole->InsertString(key, value);
	}

	void Wormhole::InsertLong(String^ key, long value)
	{
		if(m_pWormhole)
			m_pWormhole->InsertLong(key, value);
	}

	void Wormhole::InsertInt64(String^ key, __int64 value)
	{
		if(m_pWormhole)
			m_pWormhole->Insertint64(key, value);
	}

	void Wormhole::InsertFloat(String^ key, float value)
	{
		if(m_pWormhole)
			m_pWormhole->InsertFloat(key, value);
	}

	String^ Wormhole::GetString(String^ key)
	{
		if(m_pWormhole)
			return BSTR2STRING(m_pWormhole->GetString(key));
		return L"";
	}

	long Wormhole::GetLong(String^ key)
	{
		if(m_pWormhole)
			return m_pWormhole->GetLong(key);
		return 0;
	}

	__int64 Wormhole::GetInt64(String^ key)
	{
		if(m_pWormhole)
			return m_pWormhole->Getint64(key);
		return 0;
	}

	float Wormhole::GetFloat(String^ key)
	{
		if(m_pWormhole)
			return m_pWormhole->GetFloat(key);
		return 0;
	}

	void Wormhole::SendMessage()
	{
		if (m_pWormhole)
			return m_pWormhole->SendMessage();
	}

	Browser::Browser(IBrowser* pChromeWebBrowser)
	{
		m_pWebBrowser = pChromeWebBrowser;
	}

	void Browser::AddURLs(String^ urls)
	{
		BSTR bstrUrl = STRING2BSTR(urls);
		if (m_pWebBrowser)
		{
			m_pWebBrowser->OpenURL(bstrUrl, BrowserWndOpenDisposition::NEW_FOREGROUND_TAB, L"", L"");
		}
	}

	void Browser::OpenURL(String^ url, Disposition disposition, String^ key, String^ xml)
	{
		BSTR bstrUrl = STRING2BSTR(url);
		BSTR bstrKey = STRING2BSTR(key);
		BSTR bstrXml = STRING2BSTR(xml);
		if (m_pWebBrowser)
		{
			m_pWebBrowser->OpenURL(bstrUrl, (BrowserWndOpenDisposition)disposition, bstrKey, bstrXml);
		}
	}

	WebPage::WebPage(IWebPage* pWebPage)
	{
		m_pWebPage = pWebPage;
	}
	
	IntPtr WebPage::Handle::get()
	{
		return (IntPtr)m_hWnd;
	}
	
	Star^ WebPage::ParentStar::get()
	{
		return theAppProxy._createObject<IStar, Star>(m_pProxy->GetParentNode());
	}
	
	Quasar^ WebPage::HostQuasar::get()
	{
		return theAppProxy._createObject<IQuasar, Quasar>(m_pProxy->GetQuasar());
	}

	Form^ WebPage::CreateForm(String^ strFormKey)
	{
		if (m_pWebPage)
		{
			IDispatch* pFormDisp = nullptr;
			pFormDisp = theApp.m_pHubbleImpl->m_pCLRProxy->CreateWinForm(m_hWnd, STRING2BSTR(strFormKey));
			return (Form^)Marshal::GetObjectForIUnknown((IntPtr)pFormDisp);
		}
		return nullptr;
	}
}