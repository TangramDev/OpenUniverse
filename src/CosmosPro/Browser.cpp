/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103310057
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
 
 /********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103310057
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser;
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

#include "stdafx.h"
#include "dllmain.h"
#include "Cosmos.h"
#include "CosmosProxy.h"
#include "Browser.h"

using namespace std;
using namespace System;
using namespace System::Windows;
using namespace System::Reflection;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Threading;
using namespace System::Threading::Tasks;

using System::Runtime::InteropServices::Marshal;

namespace Universe
{
	Wormhole::Wormhole(CSession* pCosmosSession)
	{
		m_pCosmosSession = pCosmosSession;
	}

	void Wormhole::InsertString(String^ key, String^ value)
	{
		if(m_pCosmosSession)
			m_pCosmosSession->InsertString(key, value);
	}

	void Wormhole::InsertLong(String^ key, long value)
	{
		if(m_pCosmosSession)
			m_pCosmosSession->InsertLong(key, value);
	}

	void Wormhole::InsertInt64(String^ key, __int64 value)
	{
		if(m_pCosmosSession)
			m_pCosmosSession->Insertint64(key, value);
	}

	void Wormhole::InsertFloat(String^ key, float value)
	{
		if(m_pCosmosSession)
			m_pCosmosSession->InsertFloat(key, value);
	}

	String^ Wormhole::GetString(String^ key)
	{
		if(m_pCosmosSession)
			return BSTR2STRING(m_pCosmosSession->GetString(key));
		return L"";
	}

	long Wormhole::GetLong(String^ key)
	{
		if(m_pCosmosSession)
			return m_pCosmosSession->GetLong(key);
		return 0;
	}

	__int64 Wormhole::GetInt64(String^ key)
	{
		if(m_pCosmosSession)
			return m_pCosmosSession->Getint64(key);
		return 0;
	}

	float Wormhole::GetFloat(String^ key)
	{
		if(m_pCosmosSession)
			return m_pCosmosSession->GetFloat(key);
		return 0;
	}

	void Wormhole::SendMessage()
	{
		if (m_pCosmosSession)
			return m_pCosmosSession->SendMessage();
	}

	bool Wormhole::isBindCLRObjToWebPage(Object^ obj, String^ eventName)
	{
		if (obj->GetType()->IsSubclassOf(Control::typeid))
		{
			Control^ ctrl = (Control^)obj;
			String^ strInfo = L"@" + ctrl->Handle.ToString() + eventName->ToLower() + L"@";
			//String^ strInfo = L"@" + ctrl->Name->ToLower() + L"@";
			if (m_strEvents->IndexOf(strInfo) != -1)
			{
				return true;
			}
		}
		return false;
	}

	void Wormhole::AddEventInfo(Object^ obj, String^ eventName)
	{
		if (obj->GetType()->IsSubclassOf(Control::typeid))
		{
			Control^ ctrl = (Control^)obj;
			String^ strInfo = L"@" + ctrl->Handle.ToString() + eventName->ToLower() + L"@";
			//String^ strInfo = L"@" + ctrl->Name->ToLower() + L"@";
			if (m_strEvents->IndexOf(strInfo) == -1)
			{
				m_strEvents += strInfo;
			}
		}
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
	
	Xobj^ WebPage::ParentXobj::get()
	{
		return theAppProxy._createObject<IXobj, Xobj>(m_pProxy->GetParentXobj());
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
			pFormDisp = theApp.m_pCosmosImpl->m_pCLRProxy->CreateWinForm(m_hWnd, STRING2BSTR(strFormKey));
			return (Form^)Marshal::GetObjectForIUnknown((IntPtr)pFormDisp);
		}
		return nullptr;
	}
}