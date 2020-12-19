/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
********************************************************************************/

#include "stdafx.h"
#include "dllmain.h"
#include "Cosmos.h"
#include "CosmosProxy.h"
#include "Browser.h"

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
		m_pSession = pSession;
	}

	void Wormhole::InsertString(String^ key, String^ value)
	{
		if(m_pSession)
			m_pSession->InsertString(key, value);
	}

	void Wormhole::InsertLong(String^ key, long value)
	{
		if(m_pSession)
			m_pSession->InsertLong(key, value);
	}

	void Wormhole::InsertInt64(String^ key, __int64 value)
	{
		if(m_pSession)
			m_pSession->Insertint64(key, value);
	}

	void Wormhole::InsertFloat(String^ key, float value)
	{
		if(m_pSession)
			m_pSession->InsertFloat(key, value);
	}

	String^ Wormhole::GetString(String^ key)
	{
		if(m_pSession)
			return BSTR2STRING(m_pSession->GetString(key));
		return L"";
	}

	long Wormhole::GetLong(String^ key)
	{
		if(m_pSession)
			return m_pSession->GetLong(key);
		return 0;
	}

	__int64 Wormhole::GetInt64(String^ key)
	{
		if(m_pSession)
			return m_pSession->Getint64(key);
		return 0;
	}

	float Wormhole::GetFloat(String^ key)
	{
		if(m_pSession)
			return m_pSession->GetFloat(key);
		return 0;
	}

	void Wormhole::SendMessage()
	{
		if (m_pSession)
			return m_pSession->SendMessage();
	}

	bool Wormhole::isBindCLRObjToWebPage(Object^ obj)
	{
		if (obj->GetType()->IsSubclassOf(Control::typeid))
		{
			Control^ ctrl = (Control^)obj;
			String^ strInfo = L"@" + ctrl->Name->ToLower() + L"@";
			if (this->m_strEvents->IndexOf(strInfo) != -1)
			{
				return true;
			}
		}
		return false;
	}

	Browser::Browser(IBrowser* pBrowser)
	{
		m_pWebBrowser = pBrowser;
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
		::SysFreeString(bstrUrl);
		::SysFreeString(bstrKey);
		::SysFreeString(bstrXml);
	}

	WebPage::WebPage(IWebPage* pWebPage)
	{
		m_pWebPage = pWebPage;
	}
	
	IntPtr WebPage::Handle::get()
	{
		return (IntPtr)m_hWnd;
	}
	
	Grid^ WebPage::ParentGrid::get()
	{
		return theAppProxy._createObject<IGrid, Grid>(m_pProxy->GetParentGrid());
	}
	
	Galaxy^ WebPage::HostGalaxy::get()
	{
		return theAppProxy._createObject<IGalaxy, Galaxy>(m_pProxy->GetGalaxy());
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