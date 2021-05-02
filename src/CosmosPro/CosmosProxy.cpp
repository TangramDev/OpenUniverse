/********************************************************************************
 *           Web Runtime for Application - Version 1.0.1.202105020001
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

 // CosmosProxy.cpp : Implementation of CTangramNavigator

#include "stdafx.h"
#include "dllmain.h" 
#include "CosmosProxy.h"
#include "NewChildForm.h"
#include "GridCLREvent.h"

#include <io.h>
#include <stdio.h>
#include "Markup.h"
#include "AssemblyLocalor.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

using namespace Universe;
using namespace System::Windows;
using namespace System::Security::Permissions;
#pragma managed(push, off)
CCosmos theApp;
#pragma managed(pop)
CCosmosProxy theAppProxy;
// CCosmos

ICosmos* GetCosmos()
{
	if (::GetModuleHandle(_T("universe.dll")) == nullptr)
	{
		HMODULE hModule = ::LoadLibrary(L"universe.dll");
		if (hModule == nullptr) {
			TCHAR m_szBuffer[MAX_PATH];
			if (SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer) == S_OK) {
				ATL::CString m_strProgramFilePath = ATL::CString(m_szBuffer);
				m_strProgramFilePath += _T("\\Cosmos\\universe.dll");
				if (::PathFileExists(m_strProgramFilePath)) {
					hModule = ::LoadLibrary(m_strProgramFilePath);
				}
			}
			if (hModule == nullptr)
			{
				::GetModuleFileName(::GetModuleHandle(_T("Cosmos.dll")), m_szBuffer, MAX_PATH);
				CString strPath = m_szBuffer;
				int nPos = strPath.ReverseFind('\\');
				strPath = strPath.Left(nPos + 1) + _T("universe.dll");
				if (::PathFileExists(strPath)) {
					hModule = ::LoadLibrary(strPath);
				}
			}
		}
		if (hModule) {
			typedef CCosmosImpl* (__stdcall* GetCosmosImpl)(ICosmos**);
			GetCosmosImpl _pCosmosImplFunction;
			_pCosmosImplFunction = (GetCosmosImpl)GetProcAddress(hModule, "GetCosmosImpl");
			if (_pCosmosImplFunction != NULL) {
				theApp.m_pCosmosImpl = _pCosmosImplFunction(&theApp.m_pCosmos);
				theApp.m_pCosmosImpl->m_pCosmosDelegate = (ICosmosDelegate*)&theApp;
				theApp.m_pCosmosImpl->m_pUniverseAppProxy = (IUniverseAppProxy*)&theApp;
			}
		}
	}
	return theApp.m_pCosmos;
}

FormInfo::FormInfo()
{

}

FormInfo::~FormInfo()
{
	m_mapShortcutItem.clear();
}

CCosmosProxy::CCosmosProxy() : ICosmosCLRImpl()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//TabPage|TabControl|
	//_CrtSetBreakAlloc(826);
	//m_strExtendableTypes = L"|WebBrowser|Panel|TreeView|ListView|MonthCalendar|GroupBox|FlowLayoutPanel|TableLayoutPanel|SplitContainer|";
	m_strExtendableTypes = L"|Button|TextBox|WebBrowser|Panel|TreeView|ListView|MonthCalendar|GroupBox|FlowLayoutPanel|TableLayoutPanel|SplitContainer|";
	m_pCurrentPForm = nullptr;
	m_strCurrentWinFormTemplate = _T("");
	Forms::Application::EnableVisualStyles();
	m_pCosmosWpfApp = nullptr;
	m_pSystemAssembly = nullptr;
	m_pOnLoad = nullptr;
	m_pOnCtrlVisible = nullptr;
	m_htObjects = gcnew Hashtable();
	Forms::Application::ApplicationExit += gcnew EventHandler(&OnApplicationExit);
	AppDomain^ thisDomain = AppDomain::CurrentDomain;
	thisDomain->ProcessExit += gcnew System::EventHandler(&OnProcessExit);
	thisDomain->DomainUnload += gcnew System::EventHandler(&OnDomainUnload);
	if (::GetModuleHandle(_T("universe.dll")) == nullptr)
	{
		theApp.m_bHostApp = true;
		GetCosmos();
		if (theApp.m_pCosmos)
		{
			theApp.m_pCosmosImpl->m_pCLRProxy = this;
			theApp.m_pCosmosImpl->m_pCosmosAppProxy = &theApp;
		}
	}

	Universe::Cosmos::GetCosmos();
}

CCosmosProxy::~CCosmosProxy()
{
	for (auto& it : m_mapGalaxyInfo)
	{
		delete it.second;
	}
	m_mapGalaxyInfo.clear();
	m_mapAssembly.erase(m_mapAssembly.begin(), m_mapAssembly.end());
	m_mapAssembly.clear();
	m_mapChromeWebPage.erase(m_mapChromeWebPage.begin(), m_mapChromeWebPage.end());
	m_mapChromeWebPage.clear();
	m_mapWpfControlWrapper.erase(m_mapWpfControlWrapper.begin(), m_mapWpfControlWrapper.end());
	m_mapWpfControlWrapper.clear();
	m_mapWebBrowser.erase(m_mapWebBrowser.begin(), m_mapWebBrowser.end());
	m_mapWebBrowser.clear();
	m_mapSession2Wormhole.erase(m_mapSession2Wormhole.begin(), m_mapSession2Wormhole.end());
	m_mapSession2Wormhole.clear();
	m_mapForm.erase(m_mapForm.begin(), m_mapForm.end());
	m_mapForm.clear();
	if (theApp.m_bHostApp == false)
		theApp.m_pCosmosImpl->m_pCLRProxy = nullptr;
	OutputDebugString(_T("------------------Release CCosmosProxy ------------------------\n"));
	CString strInfo = _T("");
}

bool CCosmos::DoIdleWork() {
	Universe::Cosmos::GetCosmos()->Fire_OnCloudAppIdle();
	return false;
}

HWND CCosmos::InitCosmosApp()
{
	return 0;
};

void CCosmos::IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2)
{
	Universe::Cosmos::Fire_OnCosmosMsg(marshal_as<IntPtr>((HANDLE)hWnd), marshal_as<String^>(strType), marshal_as<String^>(strParam1), marshal_as<String^>(strParam2));
}

void CCosmos::CustomizedDOMElement(HWND hWnd, CString strRuleName, CString strHTML)
{
	Universe::Cosmos::Fire_OnCustomizedDOMElement(marshal_as<IntPtr>((HANDLE)hWnd), marshal_as<String^>(strRuleName), marshal_as<String^>(strHTML));
}

void CCosmos::ProcessMsg(MSG* msg) {
	if (msg)
	{
		Universe::Cosmos::Fire_OnAppMsgLoop((IntPtr)msg->hwnd, (IntPtr)(__int32)msg->message, (IntPtr)(__int32)msg->wParam, (IntPtr)msg->lParam);
		if (msg->message == WM_CHAR)
		{
			ATLTRACE(_T("\n"));
		}
		Control^ pControl = Control::FromHandle((IntPtr)msg->hwnd);
		if (pControl)
		{
			if (msg->message == WM_LBUTTONDOWN || msg->message == WM_LBUTTONDOWN)
			{
				//Windows::Forms::Message msg2 = Forms::Message::Create(IntPtr(msg->hwnd), msg->message, IntPtr((__int32)msg->wParam), IntPtr(msg->lParam));
				//pControl->PreProcessMessage(msg2);
				//return ;
			}
		}
		::TranslateMessage(msg);
		::DispatchMessage(msg);
	}
};

void CCosmosProxy::ExportCLRObjInfo(CString strPath)
{
}

void CCosmosProxy::ExportAllCLRObjInfo(CString _strPath)
{
	CString strPath = _strPath;
	strPath += _T("\\");
	strPath.Replace(_T("\\\\"), _T("\\"));

	if (::PathIsDirectory(strPath) == false)
		return;
	HANDLE hFind; // file handle
	WIN32_FIND_DATA FindFileData;

	hFind = FindFirstFile(strPath + _T("*.*"), &FindFileData); // find the first file
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	bool bSearch = true;
	while (bSearch) // until we finds an entry
	{
		if (FindNextFile(hFind, &FindFileData))
		{
			if ((_tcscmp(FindFileData.cFileName, _T(".")) == 0) ||
				(_tcscmp(FindFileData.cFileName, _T("..")) == 0))
				continue;

			// We have found a directory
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString strPath2 = strPath + FindFileData.cFileName + _T("\\");
				ExportAllCLRObjInfo(strPath2);
			}
			else
			{
				CString str = FindFileData.cFileName;
				int nPos = str.ReverseFind('.');
				if (nPos != -1)
				{
					CString s = str.Mid(nPos);
					if (s.CompareNoCase(_T(".dll")) == 0)
						ExportCLRObjInfo(strPath + FindFileData.cFileName);
				}
			}

		}//FindNextFile
		else
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) // no more files there
				bSearch = false;
			else {
				// some error occured, close the handle and return false
				FindClose(hFind);
				return;
			}
		}
	}//while

	FindClose(hFind); // closing file handle
	return;
}

IDispatch* CCosmosProxy::CreateWinForm(HWND hParent, BSTR strXML)
{
	auto it = m_mapChromeWebPage.find(hParent);
	if (it != m_mapChromeWebPage.end())
	{
		Form^ pForm = Universe::Cosmos::CreateForm(it->second, marshal_as<String^>(strXML));
		if (pForm)
		{
			return (IDispatch*)Marshal::GetIUnknownForObject(pForm).ToPointer();
		}
	}
	return nullptr;
}

bool CCosmosProxy::IsSupportDesigner()
{
	return true;
}

void CCosmosProxy::OnDestroyChromeBrowser(IBrowser* pBrowser)
{
	auto it = m_mapWebBrowser.find(pBrowser);
	if (it != theAppProxy.m_mapWebBrowser.end())
	{
		it->second->m_pWebBrowser = nullptr;
		theAppProxy.m_mapWebBrowser.erase(it);
	}
};

CWPFObj* CCosmosProxy::CreateWPFControl(IXobj* pXobj, HWND hPWnd, UINT nID)
{
	if (pXobj)
	{
		CComBSTR bstrObjTypeID(L"");
		pXobj->get_Attribute(CComBSTR(L"objid"), &bstrObjTypeID);

		Type^ pType = Universe::Cosmos::GetType(marshal_as<String^>(bstrObjTypeID));
		::SysFreeString(bstrObjTypeID);

		CTangramWPFObjWrapper* pWpfControlWrapper = nullptr;
		try
		{
			pWpfControlWrapper = new CTangramWPFObjWrapper();
			if (pWpfControlWrapper->CreateControl(pType, hPWnd, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0))
			{
				Universe::Xobj^ _pXobj = (Universe::Xobj^)theAppProxy._createObject<IXobj, Universe::Xobj>(pXobj);
				Universe::Cosmos::m_pFrameworkElementDic[pWpfControlWrapper->m_pUIElement] = _pXobj;
				::SetWindowLongPtr(pWpfControlWrapper->m_hwndWPF, GWLP_ID, nID);
				m_mapWpfControlWrapper[pWpfControlWrapper->m_hwndWPF] = pWpfControlWrapper;
				try {
					pWpfControlWrapper->m_pSource->RootVisual = pWpfControlWrapper->m_pUIElement;
				}
				catch (Markup::XamlParseException^ e)
				{
					Debug::WriteLine(L"Cosmos WPFControlWrapper Exception 1: " + e->Message);
					Debug::WriteLine(L"Cosmos WPFControlWrapper Exception 1: " + e->InnerException->Message);
				}
			}
		}
		catch (System::Exception^ ex)
		{
			Debug::WriteLine(L"Cosmos WPFControlWrapper Exception 1: " + ex->Message);
			Debug::WriteLine(L"Cosmos WPFControlWrapper Exception 1: " + ex->InnerException->Message);
		}

		if (pWpfControlWrapper != nullptr)
			return pWpfControlWrapper;
	}
	return nullptr;
}

HRESULT CCosmosProxy::NavigateURL(IXobj* pXobj, CString strURL, IDispatch* dispObjforScript)
{
	return S_FALSE;
}

void CCosmosProxy::WindowCreated(LPCTSTR strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd)
{
	CString _strName = strName;
	CString _strClassName = strClassName;
	if (_strName != _T("TimerNativeWindow") && hWnd != hPWnd)
	{
		auto it = m_mapForm.find(hPWnd);
		if (it == m_mapForm.end())
		{
			Control^ pPForm = Form::FromHandle((IntPtr)hPWnd);
			if (pPForm != nullptr)
			{
				if (IsWinForm(hPWnd))
				{
					Form^ _pForm = static_cast<Form^>(pPForm);
					auto it = m_mapForm.find(hPWnd);
					if (it == m_mapForm.end())
					{
						if (::GetWindowLong(hPWnd, GWL_EXSTYLE) & WS_EX_APPWINDOW)
						{
							//::SetWindowLongPtr(hPWnd, GWL_EXSTYLE, ::GetWindowLongPtr(hPWnd, GWL_EXSTYLE) & ~(WS_EX_APPWINDOW));
							//_pForm->ShowInTaskbar = false;
						}
						m_mapForm[hPWnd] = _pForm;
						if (m_pOnLoad)
						{
						}
						else
							m_pOnLoad = gcnew EventHandler(CCosmosProxy::OnLoad);
						_pForm->Load += m_pOnLoad;
						::SendMessage(theApp.m_pCosmosImpl->m_hCosmosWnd, WM_WINFORMCREATED, (WPARAM)hPWnd, (LPARAM)0);
					}
				}
			}
		}
	}
}

void CCosmosProxy::WindowDestroy(HWND hWnd)
{
	auto it2 = this->m_mapWpfControlWrapper.find(hWnd);
	if (it2 != m_mapWpfControlWrapper.end())
	{
		delete it2->second;
		m_mapWpfControlWrapper.erase(it2);
	}
	auto it3 = m_mapGalaxyInfo.find(hWnd);
	if (it3 != m_mapGalaxyInfo.end())
	{
		delete it3->second;
		m_mapGalaxyInfo.erase(it3);
	}
	auto it7 = theApp.m_pCosmosImpl->m_mapUIData.find(hWnd);
	if (it7 != theApp.m_pCosmosImpl->m_mapUIData.end())
	{
		theApp.m_pCosmosImpl->m_mapUIData.erase(it7);
	}
	auto it = m_mapForm.find(hWnd);
	if (it != m_mapForm.end())
	{
		m_mapForm.erase(it);
		if (m_mapForm.size() == 0)
		{
			if (::GetModuleHandle(L"devenv.exe") == NULL)
				::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, theApp.m_bHostApp ? 1 : 0, 20190511);
		}
	}
}

void CCosmosProxy::OnVisibleChanged(System::Object^ sender, System::EventArgs^ e)
{
	Control^ pChild = (Control^)sender;
	if (pChild->Tag != nullptr)
	{
		String^ strTag = pChild->Tag->ToString();
		if (strTag == L"htmlclient")
		{
			if (pChild->Visible)
			{
			}
			else
			{
			}
			return;
		}
	}
	if (pChild->Visible)
	{
		LONG_PTR lp = ::GetWindowLongPtr((HWND)pChild->Handle.ToInt64(), GWLP_USERDATA);
		if (lp)
		{
			Form^ pForm = (Form^)Form::FromHandle((IntPtr)lp);
			if (pForm)
			{
				Type^ pType = pChild->GetType();
				String^ strType = pType->FullName;
				GalaxyCluster^ pGalaxyCluster = Universe::Cosmos::CreateGalaxyCluster(pForm, nullptr);
				String^ strType2 = strType->Replace(L"System.Windows.Forms.", L"");
				if ((theAppProxy.m_strExtendableTypes->IndexOf(L"|" + strType2 + L"|") != -1 && pChild->Dock == DockStyle::None) || pChild->Dock == DockStyle::Fill)
				{
					bool bExtendable = (pChild->Tag == nullptr);// false;// ;
					if (pChild->Tag != nullptr)
					{
						String^ strTag = pChild->Tag->ToString();
						bExtendable = (strTag->IndexOf(L"|Extendable|") >= 0);
					}
					if (bExtendable)
					{
						pChild->VisibleChanged += theAppProxy.m_pOnCtrlVisible;

						String^ name = pChild->Name;
						if (String::IsNullOrEmpty(name))
							name = strType;
						CString strName = marshal_as<CString>(name->ToLower());
						GalaxyInfo* pInfo = new GalaxyInfo;
						pInfo->m_strXobjXml = _T("");
						pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
						pInfo->m_pDisp = nullptr;
						pInfo->m_pParentDisp = nullptr;
						theAppProxy.m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
						pInfo->m_strCtrlName = pChild->Name->ToLower();
						pInfo->m_strParentCtrlName = pChild->Name->ToLower();
						theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), strName, pGalaxyCluster->m_pGalaxyCluster, pInfo);
					}
				}
				::SetWindowLongPtr((HWND)pChild->Handle.ToInt64(), GWLP_USERDATA, 0);
				return;
			}
			else
			{
				IXobj* pXobj = (IXobj*)lp;
			}
		}

		IGalaxyCluster* pXobj = theApp.m_pCosmosImpl->Observe((HWND)pChild->Handle.ToInt64(), marshal_as<CString>(pChild->Name), _T("default"));
	}
}

void CCosmosProxy::OnItemSelectionChanged(System::Object^ sender, Forms::ListViewItemSelectionChangedEventArgs^ e)
{
	Control^ pCtrl = (Control^)sender;
	Form^ m_pCurrentForm = nullptr;
	IntPtr handle = (IntPtr)pCtrl->Handle;
	while (handle != IntPtr::Zero)
	{
		Control^ ctl = Control::FromHandle(handle);
		if (ctl != nullptr)
		{
			if (ctl->GetType()->IsSubclassOf(Form::typeid))
			{
				m_pCurrentForm = (Form^)ctl;
				break;
			}
		}

		handle = (IntPtr)::GetAncestor((HWND)handle.ToPointer(), GA_PARENT);
	}
	Universe::Xobj^ pXobj = Universe::Cosmos::GetXobjFromControl(m_pCurrentForm);
	if (pXobj)
	{
		Galaxy^ pGalaxy = pXobj->Galaxy;
		if (e->Item->Tag != nullptr)
		{
			String^ strTag = e->Item->Tag->ToString();
			if (String::IsNullOrEmpty(strTag) == false)
			{
				if (strTag->IndexOf(L"|TangramNode|") != -1)
				{
					String^ strIndex = strTag->Substring(strTag->IndexOf(L":") + 1);
					if (String::IsNullOrEmpty(strIndex) == false)
					{
						Control^ pCtrl = (Control^)sender;
						Control^ pTop = pCtrl->TopLevelControl;
						Type^ pType = pTop->GetType();
						if (pType->IsSubclassOf(Form::typeid))
						{
							String^ name = pType->Name + pCtrl->Name;
							theApp.m_pCosmosImpl->ObserveCtrl(pCtrl->Handle.ToInt64(), name, strIndex);
						}
						pCtrl->Select();
					}
				}
			}
			if (pGalaxy)
			{
				CTangramXmlParse m_Parse;
				if (m_Parse.LoadXml(strTag))
				{
					CString strBindName = m_Parse.attr(_T("target"), _T(""));
					if (strBindName != _T(""))
					{
						pGalaxy->SendMessage(pCtrl->Name, pCtrl->Handle.ToInt64().ToString("d"), L"ControlBindMessage:MouseDoubleClick", strTag, marshal_as<String^>(strBindName));
					}
				}
			}
		}
	}
}

void CCosmosProxy::OnNodeMouseDoubleClick(Object^ sender, TreeNodeMouseClickEventArgs^ e)
{
	Control^ pCtrl = (Control^)sender;
	Form^ m_pCurrentForm = nullptr;
	IntPtr handle = (IntPtr)pCtrl->Handle;
	while (handle != IntPtr::Zero)
	{
		Control^ ctl = Control::FromHandle(handle);
		if (ctl != nullptr)
		{
			if (ctl->GetType()->IsSubclassOf(Form::typeid))
			{
				m_pCurrentForm = (Form^)ctl;
				break;
			}
		}

		handle = (IntPtr)::GetAncestor((HWND)handle.ToPointer(), GA_PARENT);
	}
	Universe::Xobj^ pXobj = Universe::Cosmos::GetXobjFromControl(m_pCurrentForm);
	Galaxy^ pGalaxy = pXobj->Galaxy;
	if (e->Node->Tag != nullptr)
	{
		String^ strTag = e->Node->Tag->ToString();
		if (String::IsNullOrEmpty(strTag) == false)
		{
			if (strTag->IndexOf(L"|TangramNode|") != -1)
			{
				String^ strIndex = strTag->Substring(strTag->IndexOf(L":") + 1);
				if (String::IsNullOrEmpty(strIndex) == false)
				{
					Control^ pTop = pCtrl->TopLevelControl;
					Type^ pType = pTop->GetType();
					if (pType->IsSubclassOf(Form::typeid))
					{
						String^ name = pType->Name + pCtrl->Name;
					}
				}
			}
		}
		if (pGalaxy)
		{
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(strTag))
			{
				CString strBindName = m_Parse.attr(_T("target"), _T(""));
				if (strBindName != _T(""))
				{
					CString strActionName = m_Parse.attr(_T("onnodemousedoubleclick"), pCtrl->Name);
					pGalaxy->SendMessage(marshal_as<String^>(strActionName), pCtrl->Handle.ToInt64().ToString("d"), L"ControlBindMessage:MouseDoubleClick", strTag, marshal_as<String^>(strBindName));
				}
			}
		}
	}
	pCtrl->Select();
}

void CCosmosProxy::OnAfterSelect(System::Object^ sender, TreeViewEventArgs^ e)
{
	Control^ pCtrl = (Control^)sender;
	Form^ m_pCurrentForm = nullptr;
	IntPtr handle = (IntPtr)pCtrl->Handle;
	while (handle != IntPtr::Zero)
	{
		Control^ ctl = Control::FromHandle(handle);
		if (ctl != nullptr)
		{
			if (ctl->GetType()->IsSubclassOf(Form::typeid))
			{
				m_pCurrentForm = (Form^)ctl;
				break;
			}
		}

		handle = (IntPtr)::GetAncestor((HWND)handle.ToPointer(), GA_PARENT);
	}
	Universe::Xobj^ pXobj = Universe::Cosmos::GetXobjFromControl(m_pCurrentForm);
	if (pXobj == nullptr)
		return;
	Galaxy^ pGalaxy = pXobj->Galaxy;
	if (e->Node->Tag != nullptr)
	{
		String^ strTag = e->Node->Tag->ToString();
		if (String::IsNullOrEmpty(strTag) == false)
		{
			if (strTag->IndexOf(L"|TangramNode|") != -1)
			{
				String^ strIndex = strTag->Substring(strTag->IndexOf(L":") + 1);
				if (String::IsNullOrEmpty(strIndex) == false)
				{
					Control^ pTop = pCtrl->TopLevelControl;
					Type^ pType = pTop->GetType();
					if (pType->IsSubclassOf(Form::typeid))
					{
						String^ name = pType->Name + pCtrl->Name;
					}
				}
			}
		}
	}
	pCtrl->Select();
}

void CCosmosProxy::SetObjectProperty(IDispatch* pDisp, BSTR bstrPropertyName, BSTR bstrPropertyValue)
{
	try
	{
		Object^ pObj = (Object^)Marshal::GetObjectForIUnknown((IntPtr)pDisp);
		if (pObj != nullptr)
		{
			PropertyInfo^ ppi = pObj->GetType()->GetProperty(marshal_as<String^>(bstrPropertyName));
			if (ppi != nullptr)
			{
				ppi->SetValue(pObj, marshal_as<String^>(bstrPropertyValue), nullptr);
			}
		}
	}
	catch (System::Exception^)
	{

	}
}

Object^ CCosmosProxy::InitControl(Form^ pForm, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse)
{
	if (pForm->Tag != nullptr && pForm->Tag->Equals(L"__NEWVERSION__"))
	{
		return nullptr;
	}
	GalaxyCluster^ pGalaxyCluster = Universe::Cosmos::CreateGalaxyCluster(pForm, nullptr);
	if (pCtrl && pForm)
	{
		HWND hWnd = (HWND)pForm->Handle.ToPointer();
		if (m_strCurrentWinFormTemplate != _T(""))
		{
			::SendMessage(hWnd, WM_HUBBLE_DATA, (WPARAM)m_strCurrentWinFormTemplate.GetBuffer(), 3);
			m_strCurrentWinFormTemplate = _T("");
		}
		Control^ pActiveCtrl = nullptr;
		String^ strTypeName = pCtrl->GetType()->FullName;
		if (strTypeName == L"System.Windows.Forms.TabControl" || pCtrl->GetType()->IsSubclassOf(Forms::TabControl::typeid))
		{
			TabControl^ pTabCtrl = (TabControl^)pCtrl;
			pActiveCtrl = pTabCtrl->SelectedTab;
		}
		if (pCtrl->GetType() == Forms::ToolStrip::typeid)
		{
			Forms::ToolStrip^ pToolStrip = (Forms::ToolStrip^)pCtrl;
			Forms::ToolStripItem^ button = (Forms::ToolStripItem^)pToolStrip->Items[L"newToolStripButton"];
			if (button != nullptr)
			{
				button->Click += gcnew System::EventHandler(&OnClick);
			}
		}
		if (pCtrl->GetType() == MenuStrip::typeid)
		{
			Forms::MenuStrip^ pMenuStrip = (MenuStrip^)pCtrl;
			Forms::ToolStripMenuItem^ item = gcnew Forms::ToolStripMenuItem();
			item->Name = L"__helperbtn__";
			pMenuStrip->Items->Add(item);
			pMenuStrip->Tag = item;
			item->Visible = false;
			item->Click += gcnew System::EventHandler(&OnClick);
			Forms::ToolStripMenuItem^ fileMenu = (Forms::ToolStripMenuItem^)pMenuStrip->Items[L"fileMenu"];
			if (fileMenu != nullptr)
			{
				Forms::ToolStripMenuItem^ item = (Forms::ToolStripMenuItem^)fileMenu->DropDownItems[L"newToolStripMenuItem"];
				if (item != nullptr)
				{
					item->Click += gcnew System::EventHandler(&OnClick);
				}
				if (GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)
				{
					item = (Forms::ToolStripMenuItem^)fileMenu->DropDownItems[L"exitToolStripMenuItem"];
					if (item)
					{
						fileMenu->DropDownItems->RemoveByKey(L"exitToolStripMenuItem");
					}
				}
			}
		}

		for each (Control ^ pChild in pCtrl->Controls)
		{
			String^ name = pChild->Name;
			Type^ pType = pChild->GetType();
			String^ strType = pType->FullName;
			if (strType == L"System.Windows.Forms.MdiClient")
			{
				name = "MdiClient";
				::SendMessage((HWND)pForm->Handle.ToPointer(), WM_HUBBLE_DATA, 0, 4);
			}
			if (String::IsNullOrEmpty(name))
				name = strType;
			CTangramXmlParse* _pChild = pParse->GetChild(name->ToLower());
			if (_pChild && pChild != pActiveCtrl)
			{
				if (pActiveCtrl == nullptr)
				{
					if (strType->IndexOf(L"System.Drawing") == 0)// || strType->IndexOf(L"System.Windows.Forms.Button") == 0 || strType->IndexOf(L"System.Windows.Forms.Label") == 0 || strType->IndexOf(L"System.Windows.Forms.Text") == 0)
						continue;
					String^ strType2 = strType->Replace(L"System.Windows.Forms.", L"");
					if ((m_strExtendableTypes->IndexOf(L"|" + strType2 + L"|") != -1 && pChild->Dock == DockStyle::None) || pChild->Dock == DockStyle::Fill)
					{
						bool bExtendable = (pChild->Tag == nullptr);
						if (pChild->Tag != nullptr)
						{
							String^ strTag = pChild->Tag->ToString();
							bExtendable = (strTag->IndexOf(L"|Extendable|") >= 0);
						}
						if (bExtendable)
						{
							if (m_pOnCtrlVisible)
							{
							}
							else
							{
								m_pOnCtrlVisible = gcnew EventHandler(CCosmosProxy::OnVisibleChanged);
							}
							pChild->VisibleChanged += m_pOnCtrlVisible;
							if (strType == L"System.Windows.Forms.TreeView")
							{
								TreeView^ pTreeView = (TreeView^)pChild;
								if (_pChild)
								{
									CTangramXmlParse* _pChild2 = _pChild->GetChild(_T("eventmap"));
									if (_pChild2)
									{
										pTreeView->Tag = marshal_as<String^>(_pChild2->xml());
									}
									_pChild2 = _pChild->GetChild(_T("uidata"));
									if (_pChild2)
									{
										//pTreeView->NodeMouseDoubleClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(&OnNodeMouseDoubleClick);
										//pTreeView->AfterSelect += gcnew Forms::TreeViewEventHandler(&OnAfterSelect);
										theApp.m_pCosmosImpl->m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild2->xml();
										CtrlInit(0, pChild, pGalaxyCluster->m_pGalaxyCluster);
									}
								}
							}
							else if (strType == L"System.Windows.Forms.ListView")
							{
								ListView^ pListView = (ListView^)pChild;
								if (_pChild)
								{
									CTangramXmlParse* _pChild2 = _pChild->GetChild(_T("eventmap"));
									if (_pChild2)
									{
										pListView->Tag = marshal_as<String^>(_pChild2->xml());
									}
									_pChild2 = _pChild->GetChild(_T("uidata"));
									if (_pChild2)
									{
										pListView->ItemSelectionChanged += gcnew Forms::ListViewItemSelectionChangedEventHandler(&OnItemSelectionChanged);
										theApp.m_pCosmosImpl->m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild2->xml();
										CtrlInit(1, pChild, pGalaxyCluster->m_pGalaxyCluster);
									}
								}
							}
							else
							{
								Control^ pBtn = (Control^)pChild;
								if (_pChild)
								{
									CTangramXmlParse* _pChild2 = _pChild->GetChild(_T("eventmap"));
									if (_pChild2)
									{
										pBtn->Tag = marshal_as<String^>(_pChild2->xml());
									}
								}
							}

							//strFormName = marshal_as<CString>(thisForm->Name);
							CString strName = marshal_as<CString>(name->ToLower());
							if (name == L"htmlclient")
							{
								::PostMessage(hWnd, WM_COSMOSMSG, (WPARAM)pChild->Handle.ToInt64(), 20200130);
								pChild->Tag = name;
								pChild->VisibleChanged += gcnew System::EventHandler(&OnVisibleChanged);
							}
							else if (name == L"mdiclient")
							{
								CTangramXmlParse* pChildParse2 = nullptr;
								if (_pChild)
									pChildParse2 = _pChild->GetChild(_T("default"));
								GalaxyInfo* pInfo = new GalaxyInfo;
								pInfo->m_pDisp = nullptr;
								pInfo->m_strXobjXml = _T("");
								if (pChildParse2)
									pInfo->m_strXobjXml = pChildParse2->xml();;
								pInfo->m_pParentDisp = nullptr;
								pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
								m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
								pInfo->m_strCtrlName = name->ToLower();
								pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
								IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), strName, pGalaxyCluster->m_pGalaxyCluster, pInfo);

							}
							else
							{
								if (strType != L"System.Windows.Forms.Button")
								{
									if (pParse)
									{
										CTangramXmlParse* pChildParse2 = nullptr;
										if (_pChild)
										{
											pChildParse2 = _pChild->GetChild(_T("default"));
										}

										if (pChildParse2)
										{
											GalaxyInfo* pInfo = new GalaxyInfo;
											pInfo->m_pDisp = nullptr;
											pInfo->m_strXobjXml = pChildParse2->xml();
											pInfo->m_pParentDisp = nullptr;
											pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
											m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
											pInfo->m_strCtrlName = name->ToLower();
											pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
											IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), strName, pGalaxyCluster->m_pGalaxyCluster, pInfo);
										}
									}
									else
									{
										GalaxyInfo* pInfo = new GalaxyInfo;
										pInfo->m_pDisp = nullptr;
										pInfo->m_strXobjXml = _T("");
										pInfo->m_pParentDisp = nullptr;
										pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
										m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
										pInfo->m_strCtrlName = name->ToLower();
										pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
										IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), strName, pGalaxyCluster->m_pGalaxyCluster, pInfo);
									}
								}
							}
						}
					}
				}
				else
				{
					::SetWindowLongPtr((HWND)pChild->Handle.ToInt64(), GWLP_USERDATA, (LONG_PTR)pForm->Handle.ToInt64());
					if (m_pOnCtrlVisible)
					{
					}
					else
					{
						m_pOnCtrlVisible = gcnew EventHandler(CCosmosProxy::OnVisibleChanged);
					}
					pChild->VisibleChanged += m_pOnCtrlVisible;
				}
			}
			if (pType->IsSubclassOf(UserControl::typeid) == false)
				InitControl(pForm, pChild, bSave, pParse);
		}
	}
	long nCount = 0;
	pGalaxyCluster->m_pGalaxyCluster->get_Count(&nCount);
	if (nCount == 0)
	{
		delete pGalaxyCluster;
		return nullptr;
	}
	return pGalaxyCluster;
}

Object^ CCosmosProxy::InitXobj(IXobj* _pXobj, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse)
{
	GalaxyCluster^ pGalaxyCluster = nullptr;
	Universe::Xobj^ pXobj = (Universe::Xobj^)theAppProxy._createObject<IXobj, Universe::Xobj>(_pXobj);
	if (pXobj)
	{
		pGalaxyCluster = pXobj->GalaxyCluster;
		for each (Control ^ pChild in pCtrl->Controls)
		{
			Type^ pType = pChild->GetType();
			String^ strType = pType->FullName;
			if (strType->IndexOf(L"System.Drawing") == 0)//|| strType->IndexOf(L"System.Windows.Forms.Button") == 0 || strType->IndexOf(L"System.Windows.Forms.Label") == 0 || strType->IndexOf(L"System.Windows.Forms.Text") == 0)
				continue;
			String^ strType2 = strType->Replace(L"System.Windows.Forms.", L"");
			if ((m_strExtendableTypes->IndexOf(L"|" + strType2 + L"|") != -1 && pChild->Dock == DockStyle::None) || pChild->Dock == DockStyle::Fill)
			{
				bool bExtendable = (pChild->Tag == nullptr);
				if (pChild->Tag != nullptr)
				{
					String^ strTag = pChild->Tag->ToString();
					bExtendable = (strTag->IndexOf(L"|Extendable|") >= 0);
				}
				if (bExtendable)
				{
					IGalaxy* pGalaxy = nullptr;
					_pXobj->get_Galaxy(&pGalaxy);
					CComBSTR bstrName("");
					pGalaxy->get_Name(&bstrName);
					String^ name = pXobj->Name + L".";
					if (String::IsNullOrEmpty(pChild->Name))
						name += strType;
					else
						name += pChild->Name;
					name += L"." + marshal_as<String^>(bstrName);
					CString strName = marshal_as<CString>(name->ToLower());//OK!
					if (pParse)
					{
						CString _strName = pChild->Name->ToLower();
						CTangramXmlParse* pChildParse = pParse->GetChild(_strName);
						CTangramXmlParse* pChildParse2 = nullptr;
						if (pChildParse)
						{
							pChildParse2 = pChildParse->GetChild(_T("default"));
						}
						if (pChildParse2)
						{
							GalaxyInfo* pInfo = new GalaxyInfo;
							pInfo->m_pDisp = nullptr;
							pInfo->m_pParentDisp = _pXobj;
							pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
							m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
							pInfo->m_strXobjXml = pChildParse2->xml();
							pInfo->m_strCtrlName = _strName;
							pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
							IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), strName, pGalaxyCluster->m_pGalaxyCluster, pInfo);
							if (m_pOnCtrlVisible)
							{
							}
							else
							{
								m_pOnCtrlVisible = gcnew EventHandler(CCosmosProxy::OnVisibleChanged);
							}
							pChild->VisibleChanged += m_pOnCtrlVisible;
						}
						if (strType == L"System.Windows.Forms.TreeView")
						{
							TreeView^ pTreeView = (TreeView^)pChild;
							CTangramXmlParse* _pChild = pParse->GetChild(pChild->Name);
							if (_pChild)
							{
								CTangramXmlParse* _pChild2 = _pChild->GetChild(_T("eventmap"));
								if (_pChild2)
								{
									pTreeView->Tag = marshal_as<String^>(_pChild2->xml());
								}
								_pChild2 = _pChild->GetChild(_T("uidata"));
								if (_pChild2)
								{
									//pTreeView->NodeMouseDoubleClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(&OnNodeMouseDoubleClick);
									//pTreeView->AfterSelect += gcnew Forms::TreeViewEventHandler(&OnAfterSelect);
									theApp.m_pCosmosImpl->m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild2->xml();
									CtrlInit(0, pChild, pGalaxyCluster->m_pGalaxyCluster);
								}
							}
						}
						else if (strType == L"System.Windows.Forms.ListView")
						{
							ListView^ pListView = (ListView^)pChild;
							CTangramXmlParse* _pChild = pParse->GetChild(pChild->Name);
							if (_pChild)
							{
								CTangramXmlParse* _pChild2 = _pChild->GetChild(_T("eventmap"));
								if (_pChild2)
								{
									pListView->Tag = marshal_as<String^>(_pChild2->xml());
								}
								_pChild2 = _pChild->GetChild(_T("uidata"));
								if (_pChild2)
								{
									pListView->ItemSelectionChanged += gcnew Forms::ListViewItemSelectionChangedEventHandler(&OnItemSelectionChanged);
									theApp.m_pCosmosImpl->m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild2->xml();
									CtrlInit(1, pChild, pGalaxyCluster->m_pGalaxyCluster);
								}
							}
						}
						else
						{
							Control^ pBtn = (Control^)pChild;
							CTangramXmlParse* _pChild = pParse->GetChild(pChild->Name);
							if (_pChild)
							{
								_pChild = _pChild->GetChild(_T("eventmap"));
								if (_pChild)
								{
									pBtn->Tag = marshal_as<String^>(_pChild->xml());
								}
							}
						}
					}
					else
					{
						if (strType != L"System.Windows.Forms.Button")
						{
							GalaxyInfo* pInfo = new GalaxyInfo;
							pInfo->m_pDisp = nullptr;
							pInfo->m_pParentDisp = _pXobj;
							pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
							m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
							pInfo->m_strXobjXml = _T("");
							pInfo->m_strCtrlName = pChild->Name->ToLower();
							pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
							IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), strName, pGalaxyCluster->m_pGalaxyCluster, pInfo);
							if (m_pOnCtrlVisible)
							{
							}
							else
							{
								m_pOnCtrlVisible = gcnew EventHandler(CCosmosProxy::OnVisibleChanged);
							}
							pChild->VisibleChanged += m_pOnCtrlVisible;
							if (strType == L"System.Windows.Forms.TreeView")
							{
								TreeView^ pTreeView = (TreeView^)pChild;
								pTreeView->AfterSelect += gcnew TreeViewEventHandler(&OnAfterSelect);
							}
							else if (strType == L"System.Windows.Forms.ListView")
							{
								ListView^ pListView = (ListView^)pChild;
								pListView->ItemSelectionChanged += gcnew ListViewItemSelectionChangedEventHandler(&OnItemSelectionChanged);
							}
						}
					}
				}
			}
			InitXobj(_pXobj, pChild, bSave, pParse);
		}
	}

	return pGalaxyCluster;
}

void CCosmosProxy::CtrlInit(int nType, Control^ ctrl, IGalaxyCluster* pGalaxyCluster)
{
	HWND hWnd = (HWND)ctrl->Handle.ToPointer();
	auto it = theApp.m_pCosmosImpl->m_mapUIData.find(hWnd);
	if (it != theApp.m_pCosmosImpl->m_mapUIData.end())
	{
		switch (nType)
		{
		case 0:
		{
			TreeView^ pTreeView = (TreeView^)ctrl;
			CString strXml = it->second;
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(strXml))
			{
				if (m_Parse.LoadXml(strXml) || m_Parse.LoadFile(strXml))
				{
					if (pTreeView->Nodes->Count == 0)
					{
						TreeNode^ pXobj = pTreeView->Nodes->Add(marshal_as<String^>(m_Parse.attr(_T("text"), _T(""))));
						pXobj->ImageIndex = m_Parse.attrInt(_T("imageindex"), 0);
						pXobj->SelectedImageIndex = m_Parse.attrInt(_T("selectedimageindex"), 0);
						CString strTagName = ctrl->Name->ToLower() + _T("_tag");
						CTangramXmlParse* pChild = m_Parse.GetChild(strTagName);
						if (pChild)
						{
							pXobj->Tag = marshal_as<String^>(pChild->xml());
						}
						LoadNode(pTreeView, pXobj, pGalaxyCluster, &m_Parse);
					}
				}
				pTreeView->ExpandAll();
			}
		}
		break;
		case 1:
		{
			ListView^ pListView = (ListView^)ctrl;
			CString strXml = it->second;
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(strXml))
			{
				if (m_Parse.LoadXml(strXml) || m_Parse.LoadFile(strXml))
				{
					if (pListView->Items->Count == 0)
					{
						int nCount = m_Parse.GetCount();
						for (int i = 0; i < nCount; i++)
						{
							CTangramXmlParse* pChildParse = m_Parse.GetChild(i);
							ListViewItem^ pItem = pListView->Items->Add(marshal_as<String^>(pChildParse->attr(_T("text"), _T(""))), pChildParse->attrInt(_T("imageindex")));
							pItem->Tag = marshal_as<String^>(pChildParse->xml());
							pItem->ToolTipText = marshal_as<String^>(pChildParse->attr(_T("tooptips"), _T("")));
						}
					}
				}
			}
		}
		break;
		}
		theApp.m_pCosmosImpl->m_mapUIData.erase(it);
	}
}

System::Void CCosmosProxy::LoadNode(TreeView^ pTreeView, TreeNode^ pXobj, IGalaxyCluster* pGalaxyCluster, CTangramXmlParse* pParse)
{
	if (pParse)
	{
		int nCount = pParse->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CTangramXmlParse* _pParse = pParse->GetChild(i);
			if (_pParse)
			{
				CString name = _pParse->name();
				if (name == _T("tangramxml"))
				{
					HWND hWnd = (HWND)pTreeView->Handle.ToPointer();
					String^ name = pTreeView->Name;
				}
				else
				{
					bool isTreeNode = _pParse->attrBool(_T("treenode"), false);
					if (isTreeNode)
					{
						TreeNode^ pChildNode = pXobj->Nodes->Add(marshal_as<String^>(_pParse->attr(_T("text"), _T(""))));
						if (pChildNode)
						{
							pChildNode->ImageIndex = _pParse->attrInt(_T("imageindex"), 0);
							pChildNode->SelectedImageIndex = _pParse->attrInt(_T("selectedimageindex"), 0);
							CString strTagName = name + _T("_tag");
							CTangramXmlParse* pChild2 = pParse->GetChild(strTagName);
							if (pChild2)
							{
								pChildNode->Tag = marshal_as<String^>(pChild2->xml());
							}
							LoadNode(pTreeView, pChildNode, pGalaxyCluster, pParse->GetChild(i));
						}
					}
				}
			}
		}
	}
}

void CCosmosProxy::OnLoad(System::Object^ sender, System::EventArgs^ e)
{
	Form^ pForm = static_cast<Form^>(sender);
	CTangramXmlParse* pParse = nullptr;
	CTangramXmlParse m_Parse;
	if (theAppProxy.m_strCurrentWinFormTemplate != _T(""))
	{
		m_Parse.LoadXml(theAppProxy.m_strCurrentWinFormTemplate);
		if (m_Parse.GetCount() == 0)
			pParse = nullptr;
		else
			pParse = &m_Parse;
	}
	if (pParse == nullptr)
	{
		pForm->Load -= theAppProxy.m_pOnLoad;
		return;
	}
	GalaxyCluster^ pGalaxyCluster = static_cast<GalaxyCluster^>(theAppProxy.InitControl(pForm, pForm, true, pParse));
	if (pGalaxyCluster)
		pGalaxyCluster->Fire_OnGalaxyClusterLoad(pGalaxyCluster);
	Control^ ctrl = Universe::Cosmos::GetMDIClient(pForm);
	if (ctrl != nullptr)
	{
		Form^ pForm2 = pForm->ActiveMdiChild;
		if (pForm2 != nullptr)
		{
			String^ strKey = pForm2->GetType()->FullName;
			Object^ objTag = pForm2->Tag;
			if (objTag != nullptr)
			{
				String^ strTag = objTag->ToString();
				if (String::IsNullOrEmpty(strTag) == false)
				{
					int nIndex = strTag->IndexOf("|");
					if (nIndex != -1)
					{
						String^ strKey2 = strTag->Substring(0, nIndex);
						if (String::IsNullOrEmpty(strKey2) == false)
						{
							strKey += L"_";
							strKey += strKey2;
						}
					}
				}
			}
			theApp.m_pCosmos->ObserveGalaxys(pForm->Handle.ToInt64(), CComBSTR(L""), marshal_as<CComBSTR>(strKey), CComBSTR(L""), true);
		}
	}
	if (pForm->IsMdiContainer)
	{
		ToolStrip^ defaultToolStrip = nullptr;
		for each (Control ^ ctrl in pForm->Controls)
		{
			if (ctrl->Name == L"toolStrip")
			{
				defaultToolStrip = (ToolStrip^)ctrl;

				//ToolStripSeparator^ toolStripSeparator = gcnew ToolStripSeparator();
				//toolStripSeparator->Name = "defaulttoolStripSeparator";
				//toolStripSeparator->Size = System::Drawing::Size(6, 39);
				//defaultToolStrip->Items->Add(toolStripSeparator);

				//ToolStripButton^ pToolStripButton = gcnew ToolStripButton();
				//pToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				//pToolStripButton->Image = (System::Drawing::Image^)(pForm->Icon->ToBitmap());
				//pToolStripButton->ImageTransparentColor = System::Drawing::Color::Black;
				//pToolStripButton->Name = L"defaultbtn";
				//pToolStripButton->Size = System::Drawing::Size(36, 36);
				//pToolStripButton->Text = L"default";
				//pToolStripButton->Tag = L"default";
				//pToolStripButton->Checked = true;
				//pToolStripButton->CheckOnClick = true;
				////pToolStripButton->ToolTipText = marshal_as<String^>(strTips);
				//pToolStripButton->Click += gcnew System::EventHandler(&OnClick);
				//defaultToolStrip->Items->Add(pToolStripButton);

				break;
			}
		}
		HWND hForm = (HWND)pForm->Handle.ToPointer();
		CMDIChildFormInfo* pInfo = (CMDIChildFormInfo*)::SendMessage(hForm, WM_COSMOSMSG, (WPARAM)0, 20190602);
		if (pInfo && defaultToolStrip)
		{
			int nIndex = 0;
			for (auto it : pInfo->m_mapFormsInfo)
			{
				CString strXml = it.second;
				CTangramXmlParse m_Parse;
				if (m_Parse.LoadXml(strXml))
				{
					CString strID = m_Parse.attr(_T("objid"), _T(""));
					if (strID != _T(""))
					{
						Type^ pType = Universe::Cosmos::GetType(marshal_as<String^>(strID));
						System::Drawing::Icon^ pIcon = nullptr;
						if (pType && pType->IsSubclassOf(Form::typeid))
						{
							System::ComponentModel::ComponentResourceManager^ resources = gcnew System::ComponentModel::ComponentResourceManager(pType);
							if (resources)
							{
								pIcon = static_cast<System::Drawing::Icon^>(resources->GetObject("$this.Icon"));
								if (pIcon == nullptr)
								{
									if (Universe::Cosmos::m_pDefaultIcon == nullptr)
									{
										Form^ _pForm = gcnew Form();
										Universe::Cosmos::m_pDefaultIcon = _pForm->Icon;
									}
									pIcon = Universe::Cosmos::m_pDefaultIcon;
								}
								if (pIcon)
								{
									ToolStripButton^ pToolStripButton = gcnew ToolStripButton();
									pToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
									pToolStripButton->Image = (System::Drawing::Image^)(pIcon->ToBitmap());
									pToolStripButton->ImageTransparentColor = System::Drawing::Color::Black;
									pToolStripButton->Name = marshal_as<String^>(it.first);
									pToolStripButton->Size = System::Drawing::Size(36, 36);
									pToolStripButton->Text = marshal_as<String^>(it.first);
									pToolStripButton->Tag = marshal_as<String^>(it.second);
									CString strTips = m_Parse.attr(_T("tooltips"), _T(""));
									if (strTips != _T(""))
										pToolStripButton->ToolTipText = marshal_as<String^>(strTips);
									else
									{
										strTips = m_Parse.text();
										if (strTips != _T(""))
											pToolStripButton->ToolTipText = marshal_as<String^>(strTips);
									}
									pToolStripButton->Click += gcnew System::EventHandler(&OnClick);
									nIndex++;
									defaultToolStrip->Items->Insert(nIndex, pToolStripButton);
								}
							}
						}
						else {
							CString strInfo = _T("");
							strInfo.Format(_T("Error .NET Form Type : %s"), strID);
							::MessageBox(NULL, strInfo, _T("Cosmos"), MB_OK);
						}
					}
				}
			}
			//delete pInfo;
		}
		//_pInfo->m_mapFormsInfo.clear();
	}

	theAppProxy.m_strCurrentWinFormTemplate = _T("");

	pForm->Load -= theAppProxy.m_pOnLoad;
}

void CCosmosProxy::OnCLRHostExit()
{
	Forms::Application::Exit();
}

void* CCosmosProxy::Extend(CString strKey, CString strData, CString strFeatures)
{
	if (strFeatures.CompareNoCase(_T("tangram:creatingform")) == 0)
	{
		int nPos = strKey.Find(_T(","));
		if (nPos != -1)
		{
			Object^ pObj = Universe::Cosmos::CreateObject(marshal_as<String^>(strKey));
			if (pObj && pObj->GetType()->IsSubclassOf(Form::typeid))
			{
				Form^ pForm = (Form^)pObj;
				pForm->Show();
			}
		}
	}
	else if (strFeatures.CompareNoCase(_T("tangram:creatingmdichildform")) == 0)
	{
		int nPos = strKey.Find(_T(":"));
		Form^ pParentForm = static_cast<Form^>(Form::FromHandle((IntPtr)_wtol(strKey.Mid(nPos + 1))));
		if (pParentForm && pParentForm->IsMdiContainer)
		{
			strKey = strKey.Left(nPos);
			nPos = strKey.Find(_T(","));
			if (nPos != -1)
			{
				Object^ pObj = Universe::Cosmos::CreateObject(marshal_as<String^>(strKey));
				if (pObj && pObj->GetType()->IsSubclassOf(Form::typeid))
				{
					Form^ pForm = (Form^)pObj;
					pForm->MdiParent = pParentForm;
					m_strCurrentWinFormTemplate = strData;
					pForm->Show();
				}
			}
		}
	}

	return nullptr;
}

HRESULT CCosmosProxy::ActiveCLRMethod(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData)
{
	String^ strObjID = marshal_as<String^>(bstrObjID);
	String^ strMethod = marshal_as<String^>(bstrMethod);
	String^ strData = marshal_as<String^>(bstrData);
	cli::array<Object^, 1>^ pObjs = { marshal_as<String^>(bstrParam), marshal_as<String^>(bstrData) };
	Universe::Cosmos::ActiveMethod(strObjID, strMethod, pObjs);
	return S_OK;
}

HRESULT CCosmosProxy::ActiveCLRMethod(IDispatch* pCLRObj, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData)
{
	Object^ obj = Marshal::GetObjectForIUnknown((IntPtr)pCLRObj);
	String^ strMethod = marshal_as<String^>(bstrMethod);
	String^ strData = marshal_as<String^>(bstrData);
	cli::array<Object^, 1>^ pObjs = {};
	Universe::Cosmos::ActiveObjectMethod(obj, strMethod, pObjs);
	return S_OK;
}

IDispatch* CCosmosProxy::CreateCLRObj(CString bstrObjID)
{
	if (bstrObjID.CompareNoCase(_T("chromert")) == 0)
	{
		theApp.InitCosmosApp(false);
		return nullptr;
	}
	if (bstrObjID.Find(_T("<")) != -1)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(bstrObjID))
		{
			CString strTagName = m_Parse.name();
			CWebPageImpl* pProxyBase = nullptr;
			Universe::Wormhole^ pCloudSession = nullptr;
			CSession* pCosmosSession = nullptr;
			__int64 nHandle = m_Parse.attrInt64(_T("renderframehostproxy"), 0);
			if (nHandle)
			{
				pProxyBase = (CWebPageImpl*)nHandle;
			}
			CString strObjID = m_Parse.attr(_T("objid"), _T(""));
			if (strObjID != _T(""))
			{
				WebPage^ pPage = nullptr;
				IWebPage* pWebPage = nullptr;
				nHandle = m_Parse.attrInt64(_T("webpage"), 0);
				if (nHandle)
				{
					pWebPage = (IWebPage*)nHandle;
					CTangramXmlParse* pChild = m_Parse.GetChild(_T("webui"));
					if (pChild && pWebPage != nullptr)
					{
						pPage = gcnew WebPage(pWebPage);
						pPage->m_hWnd = (HWND)m_Parse.attrInt64(_T("webpagehandle"), 0);
						IXobj* pXobj = nullptr;
						pWebPage->Observe(CComBSTR(strTagName), CComBSTR(pChild->xml()), &pXobj);
					}
				}
				Object^ pObj = Universe::Cosmos::CreateObject(marshal_as<String^>(strObjID));

				if (pObj != nullptr)
				{
					if (pObj->GetType()->IsSubclassOf(Form::typeid))
					{
						m_strCurrentWinFormTemplate = bstrObjID;
						CString strCaption = m_Parse.attr(_T("caption"), _T(""));
						Form^ thisForm = (Form^)pObj;
						int nWidth = m_Parse.attrInt(_T("width"), 0);
						int nHeight = m_Parse.attrInt(_T("height"), 0);
						if (nWidth * nHeight)
						{
							thisForm->Width = nWidth;
							thisForm->Height = nHeight;
						}
						if (m_pCurrentPForm)
						{
							if (thisForm->IsMdiContainer == false)
								thisForm->MdiParent = m_pCurrentPForm;
							m_pCurrentPForm = nullptr;
						}
						if (nHandle && thisForm->MdiParent == nullptr)
						{
							pProxyBase->OnWinFormCreated((HWND)thisForm->Handle.ToPointer());
						}

						if (strCaption != _T(""))
							thisForm->Text = marshal_as<String^>(strCaption);
						if (thisForm->IsMdiContainer)
						{
							Control^ mdiclient = Universe::Cosmos::GetMDIClient(thisForm);
							mdiclient->ControlRemoved += gcnew System::Windows::Forms::ControlEventHandler(&OnControlRemoved);
							CString strBKPage = m_Parse.attr(_T("mdibkpageid"), _T(""));
							if (strBKPage != _T(""))
							{
								Universe::Cosmos::CreateBKPage(thisForm, marshal_as<String^>(strBKPage));
							}
						}
						if (strTagName.CompareNoCase(_T("mainwindow")) == 0)
						{
							theApp.m_pCosmosImpl->m_hMainWnd = (HWND)thisForm->Handle.ToPointer();
						}
						thisForm->Tag = marshal_as<String^>(m_Parse.name());
						__int64 nIpcSession = m_Parse.attrInt64(_T("ipcsession"), 0);
						if (nIpcSession)
						{
							pCosmosSession = (CSession*)nIpcSession;
							bool bExists = Universe::Cosmos::Wormholes->TryGetValue(pObj, pCloudSession);
							if (bExists == false)
							{
								pCloudSession = gcnew Wormhole(pCosmosSession);
								Universe::Cosmos::Wormholes[pObj] = pCloudSession;
								pCloudSession->m_pHostObj = pObj;
							}
							theAppProxy.m_mapSession2Wormhole[pCosmosSession] = pCloudSession;

							CString strFormName = m_Parse.attr(_T("formname"), _T(""));
							if (strFormName == _T(""))
							{
								strFormName = marshal_as<CString>(thisForm->Name);
							}
							pCosmosSession->InsertString(_T("formname"), strFormName);
							pCosmosSession->InsertString(_T("tagName"), strTagName);
							//pCosmosSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
							CString strFormID = m_Parse.attr(_T("id"), _T(""));
							if (strFormID != _T(""))
							{
								pCosmosSession->InsertString(_T("id"), strFormID);
							}
							strFormID = m_Parse.attr(_T("objid"), _T(""));
							pCosmosSession->InsertString(_T("objid"), strFormID);

							if (thisForm->IsMdiContainer)
							{
								pCosmosSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
								pCosmosSession->InsertLong(_T("WinFormType"), 1);
							}
							else if (thisForm->MdiParent)
							{
								pCosmosSession->InsertLong(_T("WinFormType"), 2);
								pCosmosSession->Insertint64(_T("mdiformhandle"), thisForm->MdiParent->Handle.ToInt64());
								theApp.m_pCosmos->ObserveGalaxys(thisForm->MdiParent->Handle.ToInt64(), CComBSTR(L""), CComBSTR(strTagName.MakeLower()), CComBSTR(L""), false);
								thisForm->Show();
								pCosmosSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));
								pCosmosSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
								bool bNeedQueryOnCloseEvent = m_Parse.attrBool(_T("queryonclose"), false);
								if (bNeedQueryOnCloseEvent)
									::PostMessage((HWND)(thisForm->Handle.ToInt64()), WM_HUBBLE_DATA, 1, 20201029);
								pCosmosSession->InsertLong(_T("queryonclose"), bNeedQueryOnCloseEvent ? 1 : 0);
								::SetWindowLongPtr((HWND)(thisForm->Handle.ToInt64()), GWLP_USERDATA, (LONG_PTR)pCosmosSession);
								pCosmosSession->SendMessage();
								return (IDispatch*)Marshal::GetIUnknownForObject(pObj).ToPointer();
							}
							else
							{
								pCosmosSession->InsertLong(_T("WinFormType"), 0);
								pCosmosSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
							}

							bool bNeedQueryOnCloseEvent = m_Parse.attrBool(_T("queryonclose"), false);
							pCosmosSession->InsertLong(_T("queryonclose"), bNeedQueryOnCloseEvent ? 1 : 0);
							pCosmosSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));
							pCosmosSession->InsertString(_T("cosmosxml"), m_strCurrentWinFormTemplate);
							::SetWindowLongPtr((HWND)(thisForm->Handle.ToInt64()), GWLP_USERDATA, (LONG_PTR)pCosmosSession);
							if (bNeedQueryOnCloseEvent)
								::PostMessage((HWND)(thisForm->Handle.ToInt64()), WM_HUBBLE_DATA, 1, 20201029);
							pCosmosSession->SendMessage();
						}
						else
						{
							pCosmosSession = theApp.m_pCosmosImpl->CreateCloudSession(pProxyBase);
							pCloudSession = gcnew Wormhole(pCosmosSession);
							Universe::Cosmos::Wormholes[pObj] = pCloudSession;
							pCloudSession->m_pHostObj = pObj;
							CString strFormName = m_Parse.attr(_T("formname"), _T(""));
							if (strFormName == _T(""))
							{
								strFormName = marshal_as<CString>(thisForm->Name);
							}
							pCosmosSession->InsertString(_T("tagName"), strTagName);
							pCosmosSession->InsertString(_T("formname"), strFormName);
							pCosmosSession->InsertLong(_T("autodelete"), 0);
							pCosmosSession->Insertint64(_T("domhandle"), (__int64)pCosmosSession);
							CString strFormID = m_Parse.attr(_T("id"), _T(""));
							pCosmosSession->InsertString(_T("id"), strFormID);

							strFormID = m_Parse.attr(_T("objid"), _T(""));
							pCosmosSession->InsertString(_T("objid"), strFormID);
							pCosmosSession->InsertString(_T("cosmosxml"), m_strCurrentWinFormTemplate);
							theAppProxy.m_mapSession2Wormhole[pCosmosSession] = pCloudSession;
							if (thisForm->IsMdiContainer)
							{
								pCosmosSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
								pCosmosSession->InsertLong(_T("WinFormType"), 1);
							}
							else if (thisForm->MdiParent)
							{
								pCosmosSession->InsertLong(_T("WinFormType"), 2);
								pCosmosSession->Insertint64(_T("mdiformhandle"), thisForm->MdiParent->Handle.ToInt64());
								theApp.m_pCosmos->ObserveGalaxys(thisForm->MdiParent->Handle.ToInt64(), CComBSTR(L""), CComBSTR(strTagName.MakeLower()), CComBSTR(L""), false);
								thisForm->Show();
								pCosmosSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));
								pCosmosSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
								bool bNeedQueryOnCloseEvent = m_Parse.attrBool(_T("queryonclose"), false);
								if (bNeedQueryOnCloseEvent)
									::PostMessage((HWND)(thisForm->Handle.ToInt64()), WM_HUBBLE_DATA, 1, 20201029);
								pCosmosSession->InsertLong(_T("queryonclose"), bNeedQueryOnCloseEvent ? 1 : 0);
								::SetWindowLongPtr((HWND)(thisForm->Handle.ToInt64()), GWLP_USERDATA, (LONG_PTR)pCosmosSession);
								pCosmosSession->SendMessage();
								::PostMessage((HWND)(thisForm->Handle.ToInt64()), WM_COSMOSMSG, 0, 20201114);
								return (IDispatch*)Marshal::GetIUnknownForObject(pObj).ToPointer();
							}
							else
							{
								pCosmosSession->InsertLong(_T("WinFormType"), 0);
								pCosmosSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
							}
							pCosmosSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));
							bool bNeedQueryOnCloseEvent = m_Parse.attrBool(_T("queryonclose"), false);
							pCosmosSession->InsertLong(_T("queryonclose"), bNeedQueryOnCloseEvent ? 1 : 0);
							int nMainWnd = m_Parse.attrInt(_T("IsMainCosmosWnd"), 0);
							if (nMainWnd == 1)
							{
								::PostMessage((HWND)(thisForm->Handle.ToInt64()), WM_HUBBLE_DATA, 0, 20201029);
							}
							if (bNeedQueryOnCloseEvent)
								::PostMessage((HWND)(thisForm->Handle.ToInt64()), WM_HUBBLE_DATA, 1, 20201029);
							::SetWindowLongPtr((HWND)(thisForm->Handle.ToInt64()), GWLP_USERDATA, (LONG_PTR)pCosmosSession);
							pCosmosSession->SendMessage();
						}

						if (pPage)
						{
							int nModel = m_Parse.attrInt(_T("model"), 0);
							int nAddSubFormMap = m_Parse.attrInt(_T("addsubform"), 0);
							if (nAddSubFormMap)
								::PostMessage(pPage->m_hWnd, WM_COSMOSMSG, 20200213, (LPARAM)thisForm->Handle.ToPointer());
							switch (nModel)
							{
							case 1:
								thisForm->ShowInTaskbar = false;
								thisForm->ShowDialog();
								thisForm->StartPosition = FormStartPosition::CenterScreen;
								break;
							case 0:
							case 2:
								thisForm->StartPosition = FormStartPosition::CenterParent;
								thisForm->WindowState = FormWindowState::Minimized;


								if (nModel)
									thisForm->Show(pPage);
								else
								{
									thisForm->Show();
								}
								::PostMessage(::GetParent(pPage->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
								break;
							}
						}
						else
						{
							thisForm->Show();
						}
					}
					return (IDispatch*)Marshal::GetIUnknownForObject(pObj).ToPointer();
				}
			}
			else
			{
				if (Universe::Cosmos::m_pMainForm)
				{
					HWND hWnd = (HWND)Universe::Cosmos::m_pMainForm->Handle.ToPointer();
					theApp.m_pCosmosImpl->m_hMainWnd = hWnd;
					Form^ mainForm = Universe::Cosmos::MainForm;
					CString strCaption = m_Parse.attr(_T("caption"), _T(""));
					if (strCaption != _T(""))
						mainForm->Text = marshal_as<String^>(strCaption);
					int nWidth = m_Parse.attrInt(_T("width"), 0);
					int nHeight = m_Parse.attrInt(_T("height"), 0);
					if (nWidth * nHeight)
					{
						mainForm->Width = nWidth;
						mainForm->Height = nHeight;
					}
					Control^ client = nullptr;
					if (mainForm->IsMdiContainer)
					{
						Control^ mdiclient = Universe::Cosmos::GetMDIClient(mainForm);
						mdiclient->ControlRemoved += gcnew System::Windows::Forms::ControlEventHandler(&OnControlRemoved);
						CString strBKPage = m_Parse.attr(_T("mdibkpageid"), _T(""));
						if (strBKPage != _T(""))
						{
							Universe::Cosmos::CreateBKPage(mainForm, marshal_as<String^>(strBKPage));
						}
						client = Universe::Cosmos::GetMDIClient(mainForm);
					}
					else
					{
						if (mainForm && mainForm->Controls->Count == 0)
						{
							Panel^ panel = gcnew Panel();
							panel->Dock = DockStyle::Fill;
							panel->Visible = true;
							panel->Name = L"mainclient";
							mainForm->Controls->Add(panel);
							//mainForm->Width = 1500;
							//mainForm->Height = 1200;
							mainForm->ResumeLayout();
						}
						for each (Control ^ pChild in mainForm->Controls)
						{
							if (pChild->Dock == DockStyle::Fill)
							{
								if (pChild->Parent == mainForm)
								{
									client = pChild;
									break;
								}
							}
						}
					}
					pCosmosSession = theApp.m_pCosmosImpl->CreateCloudSession(pProxyBase);
					pCloudSession = gcnew Wormhole(pCosmosSession);
					Universe::Cosmos::Wormholes[mainForm] = pCloudSession;
					pCloudSession->m_pHostObj = mainForm;
					CString strFormName = mainForm->Name;
					pCosmosSession->InsertLong(_T("autodelete"), 0);
					pCosmosSession->Insertint64(_T("domhandle"), (__int64)pCosmosSession);
					pCosmosSession->InsertString(_T("objid"), _T("mainForm"));
					pCosmosSession->InsertString(_T("formname"), strFormName);
					theAppProxy.m_mapSession2Wormhole[pCosmosSession] = pCloudSession;

					CString strFormID = m_Parse.attr(_T("id"), _T(""));
					pCosmosSession->InsertString(_T("id"), strFormID);

					pCosmosSession->Insertint64(_T("formhandle"), mainForm->Handle.ToInt64());
					pCosmosSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));
					bool bNeedQueryOnCloseEvent = m_Parse.attrBool(_T("queryonclose"), false);
					pCosmosSession->InsertLong(_T("queryonclose"), bNeedQueryOnCloseEvent ? 1 : 0);
					::SetWindowLongPtr((HWND)(mainForm->Handle.ToInt64()), GWLP_USERDATA, (LONG_PTR)pCosmosSession);
					if (bNeedQueryOnCloseEvent)
						::PostMessage((HWND)(mainForm->Handle.ToInt64()), WM_HUBBLE_DATA, 1, 20201029);

					pCosmosSession->SendMessage();
					CTangramXmlParse* pChildForms = m_Parse.GetChild(_T("childforms"));
					if (pChildForms)
					{
						::SendMessage((HWND)mainForm->Handle.ToPointer(), WM_COSMOSMSG, (WPARAM)pChildForms, 20190601);
					}
					GalaxyCluster^ pGalaxyCluster = static_cast<GalaxyCluster^>(theAppProxy.InitControl(mainForm, mainForm, true, &m_Parse));
				}
			}
		}
		return nullptr;
	}

	Object^ pObj = Universe::Cosmos::CreateObject(marshal_as<String^>(bstrObjID));

	if (pObj != nullptr)
	{
		if (pObj->GetType()->IsSubclassOf(Form::typeid))
		{
			Form^ thisForm = (Form^)pObj;
		}
		return (IDispatch*)Marshal::GetIUnknownForObject(pObj).ToPointer();
	}
	return nullptr;
}

Control^ CCosmosProxy::GetCanSelect(Control^ ctrl, bool direct)
{
	int nCount = ctrl->Controls->Count;
	Control^ pCtrl = nullptr;
	if (nCount)
	{
		for (int i = direct ? (nCount - 1) : 0; direct ? i >= 0 : i <= nCount - 1; direct ? i-- : i++)
		{
			pCtrl = ctrl->Controls[i];
			if (direct && pCtrl->TabStop && pCtrl->Visible && pCtrl->Enabled)
				return pCtrl;
			pCtrl = GetCanSelect(pCtrl, direct);
			if (pCtrl != nullptr)
				return pCtrl;
		}
	}
	else if ((ctrl->CanSelect || ctrl->TabStop) && ctrl->Visible && ctrl->Enabled)
		return ctrl;
	return nullptr;
}

HRESULT CCosmosProxy::ProcessCtrlMsg(HWND hCtrl, bool bShiftKey)
{
	Control^ pCtrl = (Control^)Control::FromHandle((IntPtr)hCtrl);
	if (pCtrl == nullptr)
		return S_FALSE;
	Control^ pChildCtrl = GetCanSelect(pCtrl, !bShiftKey);

	if (pChildCtrl)
		pChildCtrl->Select();
	return S_OK;
}

static int g_nStart = 0;
BOOL CCosmosProxy::ProcessFormMsg(HWND hFormWnd, LPMSG lpMSG, int nMouseButton)
{
	Control^ Ctrl = Form::FromHandle((IntPtr)hFormWnd);
	if (Ctrl == nullptr)
		return false;
	System::Windows::Forms::Message Msg;
	Msg.HWnd = (IntPtr)lpMSG->hwnd;
	Msg.Msg = lpMSG->message;
	Msg.WParam = (IntPtr)((LONG)lpMSG->wParam);
	Msg.LParam = (IntPtr)lpMSG->lParam;
	Form^ pForm = static_cast<Form^>(Ctrl);
	if (pForm == nullptr)
		return Ctrl->PreProcessMessage(Msg);
	if (pForm->IsMdiContainer)
	{
		Ctrl = pForm->ActiveControl;
		if (Ctrl != nullptr)
		{
			Form^ pForm2 = static_cast<Form^>(Ctrl);
			if (pForm2 == nullptr)
			{
				return Ctrl->PreProcessMessage(Msg);
			}
			else
			{
				System::Windows::Forms::PreProcessControlState state = pForm2->PreProcessControlMessage(Msg);
				if (state == System::Windows::Forms::PreProcessControlState::MessageProcessed)
					return true;
				else
					return false;
			}
		}
	}
	else
	{
		System::Windows::Forms::PreProcessControlState state = pForm->PreProcessControlMessage(Msg);
		if (state == System::Windows::Forms::PreProcessControlState::MessageProcessed)
			return true;
		else
		{
			if (pForm && pForm->MdiParent)
			{
				Control^ pCtrl = Control::FromHandle(Msg.HWnd);
				if (pCtrl && pCtrl->GetType()->IsSubclassOf(System::Windows::Forms::TextBoxBase::typeid))
				{
					switch (lpMSG->message)
					{
					case WM_LBUTTONDOWN:
					{
						int xPos = GET_X_LPARAM(lpMSG->lParam);
						int yPos = GET_Y_LPARAM(lpMSG->lParam);
						TextBoxBase^ pBase = (TextBoxBase^)pCtrl;
						int nPos = pBase->GetCharIndexFromPosition(System::Drawing::Point(xPos, yPos));
						pBase->Select(nPos, 0);
						System::Drawing::Point nPos2 = pBase->GetPositionFromCharIndex(nPos);
						if (nPos2.X < xPos)
						{
							nPos += 1;
							pBase->Select(nPos, 0);
						}
						g_nStart = nPos;
					}
					break;
					case WM_LBUTTONUP:
					{
						int xPos = GET_X_LPARAM(lpMSG->lParam);
						int yPos = GET_Y_LPARAM(lpMSG->lParam);
						TextBoxBase^ pBase = (TextBoxBase^)pCtrl;
						int nPos = pBase->GetCharIndexFromPosition(System::Drawing::Point(xPos, yPos));
						//pBase->Select(nPos, 0);
						System::Drawing::Point nPos2 = pBase->GetPositionFromCharIndex(nPos);
						if (nPos2.X < xPos)
						{
							nPos++;
							//pBase->Select(nPos, 0);
						}
						int _nPos = g_nStart;
						g_nStart = 0;
						if (_nPos != nPos)
						{
							int nLength = _nPos - nPos;
							if (nLength > 0)
							{
								pBase->Select(nPos, nLength);
							}
							else
								pBase->Select(_nPos, -nLength);
						}
					}
					break;
					default:
						break;
					}
				}
			}
			return false;
		}
	}
	return pForm->PreProcessMessage(Msg);
}

HWND CCosmosProxy::GetHwnd(HWND parent, int x, int y, int width, int height)
{
	System::Windows::Interop::HwndSourceParameters hwsPars;
	hwsPars.ParentWindow = System::IntPtr(parent);
	hwsPars.WindowStyle = WS_CHILD | WS_VISIBLE;
	hwsPars.PositionX = x;
	hwsPars.PositionY = y;
	hwsPars.Width = width;
	hwsPars.Height = height;
	System::Windows::Interop::HwndSource^ hws = gcnew System::Windows::Interop::HwndSource(hwsPars);
	return nullptr;
}

IDispatch* CCosmosProxy::CreateObject(BSTR bstrObjID, HWND hParent, IXobj* pHostNode)
{
	String^ strID = marshal_as<String^>(bstrObjID);
	Object^ _pObj = Universe::Cosmos::CreateObject(strID);
	Universe::Xobj^ _pXobj = (Universe::Xobj^)_createObject<IXobj, Universe::Xobj>(pHostNode);
	if (_pObj == nullptr)
	{
		System::Windows::Forms::Label^ label = (gcnew System::Windows::Forms::Label());
		label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
			System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		label->ForeColor = System::Drawing::Color::Sienna;
		label->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
			static_cast<System::Int32>(static_cast<System::Byte>(255)));
		label->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		CString strInfo = _T("");
		strInfo.Format(_T("Error Information: the Component \"%s\" don't exists,Please install it correctly."), OLE2T(bstrObjID));
		label->Text = marshal_as<String^>(strInfo);
		return (IDispatch*)(Marshal::GetIUnknownForObject(label).ToInt64());
	}
	if (_pObj->GetType()->IsSubclassOf(Control::typeid))
	{
		Control^ pObj = static_cast<Control^>(_pObj);
		if (pObj != nullptr && pHostNode)
		{
			IXobj* pRootXobj = NULL;
			pHostNode->get_RootXobj(&pRootXobj);
			CComBSTR bstrName(L"");
			pHostNode->get_Name(&bstrName);
			CString strName = OLE2T(bstrName);
			bool bProperty = false;

			HWND hWnd = (HWND)pObj->Handle.ToInt64();
			theApp.m_pCosmosImpl->m_mapXobj[hWnd] = pHostNode;

			IDispatch* pDisp = (IDispatch*)(Marshal::GetIUnknownForObject(pObj).ToInt64());
			_pXobj->m_pHostObj = pObj;

			if (pObj->GetType()->IsSubclassOf(Form::typeid))
			{
				Form^ form = static_cast<Form^>(pObj);
				auto it = m_mapForm.find(hWnd);
				if (it != m_mapForm.end())
					m_mapForm.erase(it);
				theApp.m_pCosmosImpl->m_hFormNodeWnd = hWnd;
				::SetWindowLongPtr(hWnd, GWL_STYLE, ::GetWindowLongPtr(hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_BORDER | WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION) | WS_CHILD);// | WS_VISIBLE);
				::SetWindowLongPtr(hWnd, GWL_EXSTYLE, ::GetWindowLongPtr(hWnd, GWL_EXSTYLE) & ~(WS_EX_APPWINDOW/*|WS_EX_CLIENTEDGE*/));
				::SetParent(hWnd, (HWND)hParent);
				Universe::Cosmos::Fire_OnFormNodeCreated(marshal_as<String^>(bstrObjID), (Form^)pObj, _pXobj);
				form->Show(_pXobj);
				return pDisp;
			}
			//if (theApp.m_pCosmosImpl->IsMDIClientGalaxyNode(pHostNode) == false && bProperty == false)
			{
				BSTR bstrXml = ::SysAllocString(L"");
				pHostNode->GetUIScript(L"", &bstrXml);
				CString strXml = OLE2T(bstrXml);
				CTangramXmlParse m_Parse;
				if (strXml != _T("") && m_Parse.LoadXml(strXml))
					InitXobj(pHostNode, pObj, true, &m_Parse);

				::SysFreeString(bstrXml);
			}
			return pDisp;
		}
	}
	else
	{
		if (_pObj->GetType()->IsSubclassOf(System::Windows::FrameworkElement::typeid))
		{
			Universe::Cosmos::m_pFrameworkElementDic[_pObj] = _pXobj;
			try
			{
				System::Windows::Forms::Integration::ElementHost^ pElementHost = gcnew System::Windows::Forms::Integration::ElementHost();
				pElementHost->Child = (FrameworkElement^)_pObj;
				pElementHost->AutoSize = true;
				IDispatch* pDisp = (IDispatch*)(Marshal::GetIUnknownForObject(pElementHost).ToInt64());
				_pXobj->m_pHostObj = pElementHost;
				return pDisp;
			}
			catch (System::Exception^ ex)
			{
				Debug::WriteLine(L"Cosmos WPFControlWrapper Exception 1: " + ex->Message);
				if (ex->InnerException)
				{
					Debug::WriteLine(L"Cosmos WPFControlWrapper Exception 1: " + ex->InnerException->Message);
				}
			}
		}
	}
	return nullptr;
}

HWND CCosmosProxy::GetWinForm(HWND hWnd, __int64& nMDIClient)
{
	if (::IsWindow(hWnd) == false)
		return nullptr;
	HWND hTopParent = ::GetAncestor(hWnd, GA_ROOT);
	if (hTopParent == NULL)
		hTopParent = hWnd;
	Control^ ctrl = Form::FromHandle((IntPtr)hTopParent);
	if (ctrl != nullptr)
	{
		if (ctrl->GetType()->IsSubclassOf(Form::typeid))
		{
			Form^ thisForm = static_cast<Form^>(ctrl);
			if (thisForm->IsMdiContainer)
			{
				Control^ ctrl = Universe::Cosmos::GetMDIClient(thisForm);
				HWND hClient = (HWND)ctrl->Handle.ToPointer();
				if (hWnd == hClient)
				{
					nMDIClient = 1;
					return hTopParent;
				}
				else
				{
					if (::IsChild(hClient, hWnd))
					{
						nMDIClient = (__int64)10;
						for each (Control ^ child in ctrl->Controls)
						{
							HWND hForm = (HWND)child->Handle.ToPointer();
							if (::IsChild(hForm, hWnd))
							{
								return hForm;
							}
						}
					}
					else
					{
						nMDIClient = (__int64)ctrl->Handle.ToPointer();
						return hTopParent;
					}
				}
			}
			else
			{
				nMDIClient = 0;
				return hTopParent;
			}
		}
	}
	return nullptr;
}

int CCosmosProxy::IsWinForm(HWND hWnd)
{
	if (hWnd == 0)
		return false;
	IntPtr handle = (IntPtr)hWnd;
	Control^ pControl = Control::FromHandle(handle);
	if (pControl != nullptr)
	{
		if (pControl->GetType()->IsSubclassOf(Form::typeid))
		{
			Form^ pForm = (Form^)pControl;
			if (pForm->IsMdiContainer)
				return 2;
			else
				return 1;
		}
		else if (::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_APPWINDOW)
		{
			int nCount = pControl->Controls->Count;
			String^ strName = L"";
			for (int i = nCount - 1; i >= 0; i--)
			{
				Control^ pCtrl = pControl->Controls[i];
				strName = pCtrl->GetType()->Name->ToLower();
				if (strName == L"mdiclient")
				{
					return 2;
				}
			}
			return 1;
		}
	}
	return 0;
}

IDispatch* CCosmosProxy::GetCLRControl(IDispatch* CtrlDisp, BSTR bstrNames)
{
	CString strNames = OLE2T(bstrNames);
	if (strNames != _T(""))
	{
		int nPos = strNames.Find(_T(","));
		if (nPos == -1)
		{
			Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)CtrlDisp);
			if (pCtrl != nullptr)
			{
				Control::ControlCollection^ Ctrls = pCtrl->Controls;
				pCtrl = Ctrls[marshal_as<String^>(bstrNames)];
				if (pCtrl == nullptr)
				{
					int nIndex = _wtoi(OLE2T(bstrNames));
					pCtrl = Ctrls[nIndex];
				}
				if (pCtrl != nullptr)
					return (IDispatch*)Marshal::GetIUnknownForObject(pCtrl).ToPointer();
			}
			return S_OK;
		}

		Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)CtrlDisp);
		while (nPos != -1)
		{
			CString strName = strNames.Left(nPos);
			if (strName != _T(""))
			{
				if (pCtrl != nullptr)
				{
					Control^ pCtrl2 = pCtrl->Controls[marshal_as<String^>(strName)];
					if (pCtrl2 == nullptr)
					{
						if (pCtrl->Controls->Count > 0)
							pCtrl2 = pCtrl->Controls[_wtoi(strName)];
					}
					if (pCtrl2 != nullptr)
						pCtrl = pCtrl2;
					else
						return S_OK;
				}
				else
					return S_OK;
			}
			strNames = strNames.Mid(nPos + 1);
			nPos = strNames.Find(_T(","));
			if (nPos == -1)
			{
				if (pCtrl != nullptr)
				{
					Control^ pCtrl2 = pCtrl->Controls[marshal_as<String^>(strNames)];
					if (pCtrl2 == nullptr)
					{
						if (pCtrl->Controls->Count > 0)
							pCtrl2 = pCtrl->Controls[_wtoi(strName)];
					}
					if (pCtrl2 == nullptr)
						return S_OK;
					if (pCtrl2 != nullptr)
						return (IDispatch*)Marshal::GetIUnknownForObject(pCtrl2).ToPointer();
				}
			}
		}
	}
	return NULL;
}

BSTR CCosmosProxy::GetCtrlName(IDispatch* _pCtrl)
{
	Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)_pCtrl);
	if (pCtrl != nullptr)
		return marshal_as<CComBSTR>(pCtrl->Name);
	return L"";
}

void CCosmosProxy::ReleaseCosmosObj(IDispatch* pDisp)
{
	LONGLONG nValue = (LONGLONG)pDisp;
	Object^ pObj = Marshal::GetObjectForIUnknown((IntPtr)pDisp);
	Universe::Wormhole^ pCloudSession = nullptr;
	bool bExists = Universe::Cosmos::Wormholes->TryGetValue(pObj, pCloudSession);
	if (bExists == true)
	{
		if (pCloudSession != nullptr)
		{
			Universe::Cosmos::Wormholes->Remove(pObj);
		}
	}
	if (pObj->GetType()->IsSubclassOf(Control::typeid))
	{
		Control^ pCtrl = (Control^)pObj;
		HWND hWnd = (HWND)pCtrl->Handle.ToPointer();
		auto it = theApp.m_pCosmosImpl->m_mapXobj.find(hWnd);
		if (it != theApp.m_pCosmosImpl->m_mapXobj.end())
			theApp.m_pCosmosImpl->m_mapXobj.erase(it);

		int dw = Marshal::Release((IntPtr)nValue);
		while (dw > 0)
			dw = Marshal::Release((IntPtr)nValue);
		//delete pCtrl;
	}
	_removeObject(nValue);
}

HWND CCosmosProxy::GetMDIClientHandle(IDispatch* pMDICtrl)
{
	Form^ pCtrl = (Form^)Marshal::GetObjectForIUnknown((IntPtr)pMDICtrl);
	if (pCtrl != nullptr)
	{
		Control^ ctrl = Universe::Cosmos::GetMDIClient(pCtrl);
		if (ctrl != nullptr)
			return (HWND)ctrl->Handle.ToInt64();
	}
	return NULL;
}

IDispatch* CCosmosProxy::GetCtrlByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild)
{
	try
	{
		Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)CtrlDisp);
		if (pCtrl != nullptr)
		{
			cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(marshal_as<String^>(bstrName), bFindInChild);
			if (pArray != nullptr && pArray->Length)
				return (IDispatch*)Marshal::GetIUnknownForObject(pArray[0]).ToPointer();
		}

	}
	catch (System::Exception^)
	{

	}
	return NULL;
}

BSTR CCosmosProxy::GetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild)
{
	try
	{
		Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)CtrlDisp);
		if (pCtrl != nullptr)
		{
			if (bstrName != L"")
			{
				cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(marshal_as<String^>(bstrName), bFindInChild);
				if (pArray != nullptr && pArray->Length)
				{
					return marshal_as<CComBSTR>(pArray[0]->Text);
				}
			}
			else
				return marshal_as<CComBSTR>(pCtrl->Text);
		}
	}
	catch (System::Exception^)
	{

	}
	return NULL;
}

void CCosmosProxy::SetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild, BSTR strVal)
{
	try
	{
		Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)CtrlDisp);
		if (pCtrl == nullptr)
			return;
		CString strName = OLE2T(bstrName);
		if (strName != _T(""))
		{
			if (pCtrl != nullptr)
			{
				cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(marshal_as<String^>(bstrName), bFindInChild);
				if (pArray != nullptr && pArray->Length)
				{
					pArray[0]->Text = marshal_as<String^>(strVal);
					return;
				}
			}
		}
		else
			pCtrl->Text = marshal_as<String^>(strVal);
	}
	catch (System::Exception^)
	{

	}
}

void CCosmosProxy::ConnectXobjToWebPage(IXobj* pXobj, bool bAdd)
{
	CSession* pSession = theApp.m_pCosmosImpl->GetCloudSession(pXobj);
	if (pSession == nullptr)
		return;
	IDispatch* pDisp = nullptr;
	__int64 nDisp = pSession->Getint64(_T("objectdisp"));
	if (nDisp)
	{
		Object^ pObj = Marshal::GetObjectForIUnknown((IntPtr)nDisp);
		if (pObj != nullptr)
		{
			Universe::Wormhole^ pCloudSession = nullptr;
			bool bExists = Universe::Cosmos::Wormholes->TryGetValue(pObj, pCloudSession);
			if (bAdd)
			{
				if (bExists == false)
				{
					pCloudSession = gcnew Wormhole(pSession);
					Universe::Cosmos::Wormholes[pObj] = pCloudSession;
					pCloudSession->m_pHostObj = pObj;
					theAppProxy.m_mapSession2Wormhole[pSession] = pCloudSession;
				}
			}
			else
			{
				if (pCloudSession != nullptr)
				{
					Universe::Cosmos::Wormholes->Remove(pObj);
				}
			}
		}
	}
}

void CCosmosProxy::OnCloudMsgReceived(CSession* pSession)
{
	CString strMsgID = pSession->GetString(L"msgID");
	Universe::Wormhole^ pCloudSession = nullptr;
	HWND hWnd = (HWND)pSession->Getint64(L"xobjhandle");
	IXobj* pXobj = (IXobj*)pSession->Getint64(L"xobj");
	Universe::Xobj^ thisNode = nullptr;
	if (pXobj)
	{
		thisNode = theAppProxy._createObject<IXobj, Universe::Xobj>(pXobj);
	}
	if (strMsgID == _T("OPEN_XML_CTRL"))
	{
		if (pXobj)
		{
			CString strName = pSession->GetString(_T("ctrlName"));
			CString strKey = pSession->GetString(_T("openkey"));
			CString strXml = pSession->GetString(_T("openxml"));
			String^ ctrlName = marshal_as<String^>(strName);
			Control^ ctrl = (Control^)thisNode->XObject;
			cli::array<Control^, 1>^ pArray = ctrl->Controls->Find(ctrlName, true);
			if (pArray != nullptr && pArray->Length)
			{
				Control^ _ctrl = pArray[0];
				Universe::Cosmos::Observe(_ctrl, marshal_as<String^>(strKey), marshal_as<String^>(strXml));
			}
		}
	}
	else if (strMsgID == _T("BIND_NATIVEOBJ_IPC_MSG"))
	{
		auto it = m_mapSession2Wormhole.find(pSession);
		if (it != m_mapSession2Wormhole.end())
		{
			Object^ pObj = nullptr;
			pObj = it->second->m_pHostObj;
			if (pObj)
			{
				if (!Universe::Cosmos::Wormholes->TryGetValue(pObj, pCloudSession))
				{
					pCloudSession = gcnew Universe::Wormhole(pSession);
					Universe::Cosmos::Wormholes[pObj] = pCloudSession;
				}
				CString strEventName = pSession->GetString(L"Bindevent");
				CString strObjName = pSession->GetString(L"BindObj");
				if (pObj->GetType()->IsSubclassOf(Control::typeid))
				{
					Control^ pCtrl = (Control^)pObj;
					Control^ pSubCtrl = nullptr;
					if (pCtrl != nullptr)
					{
						cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(marshal_as<String^>(strObjName), true);
						if (pArray != nullptr && pArray->Length)
						{
							pSubCtrl = pArray[0];
							Universe::Wormhole^ pCloudSession2 = nullptr;
							if (!Universe::Cosmos::Wormholes->TryGetValue(pSubCtrl, pCloudSession2))
							{
								Universe::Cosmos::Wormholes[pSubCtrl] = pCloudSession;
							}
							Universe::Cosmos::Fire_OnBindCLRObjToWebPage(pSubCtrl, pCloudSession, marshal_as<String^>(strEventName));

							return;
						}
					}
				}
			}
		}
	}
	else if (strMsgID == _T("SET_REFGRIDS_IPC_MSG"))
	{
		if (thisNode != nullptr) {
			CString strXml = pSession->GetString(L"RefInfo");
			CTangramXmlParse m_Parse;
			if (m_Parse.LoadXml(strXml))
			{
				int nCount = m_Parse.GetCount();
				for (int i = 0; i < nCount; i++)
				{
					CTangramXmlParse* pChild = m_Parse.GetChild(i);
					IXobj* pRefXobj = nullptr;
					__int64 hRefWnd = pChild->attrInt64(_T("handle"));
					CString strName = pChild->attr(_T("refname"), _T(""));
					CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)hRefWnd, _T("CosmosInfo"));
					if (pInfo)
						pRefXobj = pInfo->m_pXobj;
					if (pRefXobj)
					{
						thisNode[marshal_as<String^>(strName)] = theAppProxy._createObject<IXobj, Universe::Xobj>(pRefXobj);
					}
				}
			}
		}
	}

	auto it = m_mapSession2Wormhole.find(pSession);
	Object^ pObj = nullptr;
	if (it != m_mapSession2Wormhole.end())
	{
		pCloudSession = it->second;
		pObj = it->second->m_pHostObj;
		if (pObj == nullptr)
		{
			IDispatch* pDisp = nullptr;
			__int64 nDisp = pSession->Getint64(_T("objectdisp"));
			if (nDisp)
			{
				pObj = Marshal::GetObjectForIUnknown((IntPtr)nDisp);
				pCloudSession->m_pHostObj = pObj;
			}
		}
		if (strMsgID == _T("MODIFY_CTRL_VALUE"))
		{
			CString strSubObj = pSession->GetString(L"currentsubobjformodify");
			if (strSubObj == _T("caption"))
			{
				thisNode->Caption = marshal_as<String^>(pSession->GetString(L"caption"));
			}
			else if (pObj->GetType()->IsSubclassOf(Control::typeid))
			{
				Control^ pCtrl = (Control^)pObj;
				Control^ pSubCtrl = nullptr;
				String^ _strSubObjName = marshal_as<String^>(strSubObj);
				if (String::IsNullOrEmpty(_strSubObjName) == false)
				{
					cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(_strSubObjName, true);
					if (pArray != nullptr && pArray->Length)
					{
						pSubCtrl = pArray[0];
						pSubCtrl->Text = marshal_as<String^>(pSession->GetString(strSubObj));
					}
				}
			}
		}
		else if (strMsgID == _T("WINFORM_CREATED"))
		{
			CString strType = pSession->GetString(L"eventtype");
			CString strCallback = pSession->GetString(L"callbackid");
			if (pObj != nullptr)
			{
				CString strEventName = pSession->GetString(LPCTSTR(strCallback));
				if (strType == _T("BindCtrlValue"))
				{
					if (pObj->GetType()->IsSubclassOf(Control::typeid))
					{
						Control^ pCtrl = (Control^)pObj;
						Control^ pSubCtrl = nullptr;
						CString strCtrls = pSession->GetString(L"ctrls");
						String^ _strCtrls = marshal_as<String^>(strCtrls);
						cli::array<String^, 1>^ s = _strCtrls->Split(';');
						for each (String ^ _strSubObjName in s)
						{
							if (String::IsNullOrEmpty(_strSubObjName) == false)
							{
								cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(_strSubObjName, true);
								if (pArray != nullptr && pArray->Length)
								{
									pSubCtrl = pArray[0];
									Universe::Wormhole^ pCloudSession2 = nullptr;
									if (!Universe::Cosmos::Wormholes->TryGetValue(pSubCtrl, pCloudSession2))
									{
										Universe::Cosmos::Wormholes[pSubCtrl] = pCloudSession;
									}
									pSession->Insertint64(pSubCtrl->Name, pSubCtrl->Handle.ToInt64());
									pSubCtrl->TextChanged += gcnew System::EventHandler(&OnTextChanged);
								}
							}
						}
					}

					return;
				}
				String^ _strEventName = L"";
				String^ _strSubObjName = L"";
				int nPos = strEventName.Find(_T("@"));
				if (nPos != -1)
				{
					_strEventName = marshal_as<String^>(strEventName.Left(nPos));
					_strSubObjName = marshal_as<String^>(strEventName.Mid(nPos + 1));
				}
				else
				{
					_strEventName = marshal_as<String^>(strEventName);
				}
				if (!String::IsNullOrEmpty(_strSubObjName))
				{
					if (pObj->GetType()->IsSubclassOf(Control::typeid))
					{
						Control^ pCtrl = (Control^)pObj;
						Control^ pSubCtrl = nullptr;
						if (pCtrl != nullptr)
						{
							cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(_strSubObjName, true);
							if (pArray != nullptr && pArray->Length)
							{
								pSubCtrl = pArray[0];
								Universe::Wormhole^ pCloudSession2 = nullptr;
								if (!Universe::Cosmos::Wormholes->TryGetValue(pSubCtrl, pCloudSession2))
								{
									Universe::Cosmos::Wormholes[pSubCtrl] = pCloudSession;
								}
								Universe::Cosmos::Fire_OnBindCLRObjToWebPage(pSubCtrl, pCloudSession, _strEventName);

								return;
							}
						}
					}
					else
					{
						Object^ subObj = Universe::Cosmos::Fire_OnGetSubObjForWebPage(pObj, _strSubObjName);
						if (subObj != nullptr)
						{
							Universe::Cosmos::Fire_OnBindCLRObjToWebPage(subObj, pCloudSession, _strEventName);
							return;
						}
					}
				}
				else
				{
					if (_strEventName != _T(""))
					{
						Universe::Cosmos::Fire_OnBindCLRObjToWebPage(pObj, pCloudSession, _strEventName);
						return;
					}
				}
			}
			return;
		}
	}
	else
	{
		if (pXobj)
		{
			if (thisNode->m_pSession == nullptr)
			{
				pCloudSession = gcnew Universe::Wormhole(pSession);
				thisNode->m_pSession = pCloudSession;
			}
			else
			{
				pCloudSession = thisNode->m_pSession;
			}
			m_mapSession2Wormhole[pSession] = pCloudSession;
		}
	}
	if (thisNode != nullptr)
	{
		thisNode->Fire_OnCloudMessageReceived(pCloudSession);
	}
	Universe::Cosmos::Fire_OnCosmosMsgReceived(pCloudSession);
}

void CCosmosProxy::OnClick(Object^ sender, EventArgs^ e)
{
	Type^ type = sender->GetType();
	if (type->IsSubclassOf(ToolStripItem::typeid))
	{
		ToolStripItem^ button = (ToolStripItem^)sender;
		if (button)
		{
			if (button->Name == L"__helperbtn__")
			{
				//button->Visible = false;
				return;
			}
			Form^ form = (Form^)button->Owner->Parent;
			if (form == nullptr)
			{
				form = (Form^)button->OwnerItem->Owner->Parent;
			}
			if (form && form->IsMdiContainer)
			{
				theAppProxy.m_pCurrentPForm = form;
				if (button->Tag != nullptr)
				{
					String^ strXml = button->Tag->ToString();
					if (strXml == L"default")
					{
						ToolStripButton^ pBtn = (ToolStripButton^)button;
						if (pBtn->Name == L"defaultbtn")
						{
							switch (pBtn->CheckState)
							{
							case CheckState::Checked:
							{
								//Wormhole^ pCloudSession = nullptr;
								//bool bExists = Universe::Cosmos::Wormholes->TryGetValue(form, pCloudSession);
								//if (bExists)
								//{
								//	pCloudSession->InsertLong(L"MDIChildCount", 0);
								//	pCloudSession->InsertString(L"msgID", L"MDIFORM_ALLMDICHILDREMOVED");
								//	pCloudSession->SendMessage();
								//	return;
								//}

								String^ strKey = L"";
								if (form->ActiveMdiChild != nullptr)
								{
									HWND hWnd = (HWND)form->ActiveMdiChild->Handle.ToPointer();
									LRESULT l = ::SendMessage(hWnd, WM_HUBBLE_DATA, 0, 2);
									if (l)
									{
										CString strPath = (LPCTSTR)l;
										theApp.m_pCosmos->ObserveGalaxys(form->Handle.ToInt64(), CComBSTR(L""), CComBSTR(strPath), CComBSTR(L""), false);
										::SendMessage(hWnd, WM_COSMOSMSG, 0, 20200216);
										return;
									}
								}
							}
							break;
							case CheckState::Unchecked:
								Universe::Cosmos::ExtendMDIClient(form, L"default", L"");
								::SendMessage((HWND)form->Handle.ToInt64(), WM_COSMOSMSG, 0, 20200216);
								break;
							}
						}
						return;
					}
					if (String::IsNullOrEmpty(strXml) == false)
					{
						::SendMessage((HWND)form->Handle.ToPointer(), WM_COSMOSMSG, 0, 20210415);
						IDispatch* pFormDisp = theApp.m_pCosmosImpl->m_pCLRProxy->CreateCLRObj(strXml);
						if (pFormDisp)
							return;
					}
				}
				HWND hForm = (HWND)form->Handle.ToPointer();
				CMDIChildFormInfo* pInfo = (CMDIChildFormInfo*)::SendMessage(hForm, WM_COSMOSMSG, 0, 20190602);
				if (pInfo)
				{
					NewChildForm^ pCosmosNewChildForm = gcnew NewChildForm();
					pCosmosNewChildForm->Tag = L"__NEWVERSION__";
					pCosmosNewChildForm->m_pParent = form;
					ListView^ pList = pCosmosNewChildForm->GetListView();
					pList->LargeImageList = gcnew ImageList();
					pList->LargeImageList->ImageSize = System::Drawing::Size(48, 48);
					int nIndex = -1;
					for (auto& it : pInfo->m_mapFormsInfo)
					{
						CString strXml = it.second;
						CTangramXmlParse m_Parse;
						if (m_Parse.LoadXml(strXml))
						{
							CString strID = m_Parse.attr(_T("objid"), _T(""));
							if (strID != _T(""))
							{
								Type^ pType = Universe::Cosmos::GetType(marshal_as<String^>(strID));
								System::Drawing::Icon^ pIcon = nullptr;
								if (pType && pType->IsSubclassOf(Form::typeid))
								{
									System::ComponentModel::ComponentResourceManager^ resources = gcnew System::ComponentModel::ComponentResourceManager(pType);
									if (resources)
									{
										pIcon = (System::Drawing::Icon^)(resources->GetObject("$this.Icon"));
										if (pIcon == nullptr)
										{
											if (Universe::Cosmos::m_pDefaultIcon == nullptr)
											{
												Form^ _pForm = gcnew Form();
												Universe::Cosmos::m_pDefaultIcon = _pForm->Icon;
											}
											pIcon = Universe::Cosmos::m_pDefaultIcon;
										}
										if (pIcon)
										{
											nIndex++;
											pList->LargeImageList->Images->Add(pIcon);
										}
									}
									ListViewItem^ pItem = pList->Items->Add(marshal_as<String^>(it.first));
									pItem->ImageIndex = nIndex;
									pItem->Tag = marshal_as<String^>(strXml);
								}
								else {
									CString strInfo = _T("");
									strInfo.Format(_T("Error .NET Form Type : %s"), strID);
									::MessageBox(NULL, strInfo, _T("Cosmos"), MB_OK);
									return;
								}
							}
						}
					}
					pCosmosNewChildForm->Text = L"Please Select a New Form Template:";
					pCosmosNewChildForm->StartPosition = FormStartPosition::CenterScreen;
					pCosmosNewChildForm->ShowDialog(form);
				}
			}
			return;
		}

	}
	Form^ form = (Form^)Form::FromHandle((IntPtr)::GetActiveWindow());
	if (form && form->IsMdiContainer)
		Universe::Cosmos::ExtendMDIClient(form, L"newdoc", L"");
}

void CCosmosProxy::OnTextChanged(System::Object^ sender, System::EventArgs^ e)
{
	Control^ pTextCtrl = (Control^)sender;
	Universe::Wormhole^ pCloudSession = nullptr;
	if (Universe::Cosmos::Wormholes->TryGetValue(sender, pCloudSession))
	{
		pCloudSession->InsertString(pTextCtrl->Name, pTextCtrl->Text);
		pCloudSession->InsertString("msgID", "FIRE_EVENT");
		pCloudSession->InsertString("currentsubobj", pTextCtrl->Name);
		String^ strEventtype = pCloudSession->GetString(L"eventtype");
		pCloudSession->InsertString("currentevent", "OnTextChanged@" + strEventtype);

		pCloudSession->SendMessage();
	}
}

HWND CCosmosProxy::GetCtrlHandle(IDispatch* _pCtrl)
{
	Object^ pObj = (Object^)Marshal::GetObjectForIUnknown((IntPtr)_pCtrl);
	if (pObj->GetType()->IsSubclassOf(System::Windows::FrameworkElement::typeid))
	{
		System::Windows::Forms::Integration::ElementHost^ pElementHost = (System::Windows::Forms::Integration::ElementHost^)pObj;
		return (HWND)pElementHost->Handle.ToInt64();
	}
	else
	{
		Control^ pCtrl = (Control^)pObj;
		if (pCtrl != nullptr)
			return (HWND)pCtrl->Handle.ToInt64();
	}
	return 0;
}

BSTR CCosmosProxy::GetCtrlType(IDispatch* _pCtrl)
{
	Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)_pCtrl);
	if (pCtrl != nullptr)
		return marshal_as<CComBSTR>(pCtrl->GetType()->FullName);
	return L"";
}

IDispatch* CCosmosProxy::GetCtrlFromHandle(HWND hWnd)
{
	Control^ pCtrl = Control::FromHandle((IntPtr)hWnd);
	if (pCtrl != nullptr) {
		return (IDispatch*)Marshal::GetIUnknownForObject(pCtrl).ToPointer();
	}
	return nullptr;
}

HWND CCosmosProxy::IsGalaxy(IDispatch* ctrl)
{
	Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)ctrl);
	if (pCtrl != nullptr)
	{
		if (pCtrl->Dock == DockStyle::Fill)
			return (HWND)pCtrl->Handle.ToInt64();
	}
	return 0;
}

void CCosmosProxy::CosmosAction(BSTR bstrXml, void* pvoid)
{
	CString strXml = OLE2T(bstrXml);
	if (strXml != _T(""))
	{
		if (strXml.CompareNoCase(_T("startclrapp")) == 0)
		{
			if (m_bInitApp == false)
			{
				m_bInitApp = true;
				if (Universe::Cosmos::Fire_OnAppInit() == false)
				{
					PostQuitMessage(0);
					return;
				}
				switch (Universe::Cosmos::AppType)
				{
				case CosmosAppType::APPWIN32:
				{
					ATLTRACE(_T("CosmosAppType::APP_WIN32\n"));
					if (Universe::Cosmos::MainForm == nullptr || theApp.m_pCosmosImpl->m_hMainWnd == NULL)
					{
					}
				}
				break;
				case CosmosAppType::APPBROWSER:
				case CosmosAppType::APPBROWSERAPP:
				case CosmosAppType::APPBROWSER_ECLIPSE:
				{
					theApp.m_pCosmosImpl->m_hMainWnd = NULL;
					Form^ mainForm = Universe::Cosmos::MainForm::get();
					if (mainForm)
						theApp.m_pCosmosImpl->m_hMainWnd = (HWND)mainForm->Handle.ToPointer();
					theApp.InitCosmosApp(theApp.m_pCosmosImpl->m_bIsSupportCrashReporting);
				}
				break;
				case CosmosAppType::APPECLIPSE:
				{
					if (theApp.m_pCosmos && !theApp.m_pCosmosImpl->m_bIsEclipseInit)
					{
						theApp.m_pCosmos->InitEclipseApp();
						if (theApp.m_bHostApp && theAppProxy.m_pCosmosWpfApp)
						{
							theApp.ExitJVM();
						}
					}
				}
				break;
				}
			}
			return;
		}
		if (strXml.CompareNoCase(_T("setmainform")) == 0)
		{
			if (Universe::Cosmos::m_pMainForm != nullptr)
			{
				theApp.m_pCosmosImpl->m_hMainWnd = (HWND)Universe::Cosmos::m_pMainForm->Handle.ToPointer();
			}
			return;
		}
		if (strXml.CompareNoCase(_T("ExportCLRObjInfo")) == 0)
		{
			ExportCLRObjInfo(_T(""));
			return;
		}
		if (strXml.CompareNoCase(_T("EndInitEclipseApp")) == 0)
		{
			if (theApp.m_bHostApp && m_pCosmosWpfApp)
			{
				theApp.ExitJVM();
			}
			return;
		}
		if (strXml.CompareNoCase(_T("ExternAction:")) >= 0)
		{
			Universe::Xobj^ pWndXobj = (Universe::Xobj^)theAppProxy._createObject<IXobj, Universe::Xobj>((IXobj*)pvoid);
			if (pWndXobj != nullptr)
			{
				pWndXobj->ActionData = marshal_as<String^>(bstrXml);
				Universe::Cosmos::Fire_OnCosmosActionDelegate(pWndXobj, L"");
			}
			return;
		}
		if (strXml.CompareNoCase(_T("Debug:")) >= 0)
		{
			strXml.Replace(_T("Debug:"), _T(""));
			Universe::Cosmos::Fire_OnCosmosDebugDelegate(marshal_as<String^>(strXml));
			return;
		}

		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(strXml))
		{
			if (pvoid == nullptr)
			{
			}
			else
			{
				Universe::Xobj^ pWndXobj = (Universe::Xobj^)theAppProxy._createObject<IXobj, Universe::Xobj>((IXobj*)pvoid);
				if (pWndXobj)
				{
					int nType = m_Parse.attrInt(_T("Type"), 0);
					switch (nType)
					{
					case 5:
						if (pWndXobj != nullptr)
						{
						}
						break;
					default:
					{
						CString strID = m_Parse.attr(_T("ObjID"), _T(""));
						CString strMethod = m_Parse.attr(_T("Method"), _T(""));
						if (strID != _T("") && strMethod != _T(""))
						{
							cli::array<Object^, 1>^ pObjs = { marshal_as<String^>(strXml), pWndXobj };
							Universe::Cosmos::ActiveMethod(marshal_as<String^>(strID), marshal_as<String^>(strMethod), pObjs);
						}
					}
					break;
					}
				}
			}
		}
		//else if(pvoid!=nullptr)
		//{
		//	Xobj^ pWndXobj = (Xobj^)theAppProxy._createObject<IXobj, Xobj>((IXobj*)pvoid);
		//	if (pWndXobj != nullptr)
		//	{
		//		CString strToken = _T("@IPCMessage@");
		//		int nPos = strXml.Find(strToken);
		//		if (nPos != -1)
		//		{
		//			Universe::Cosmos::Fire_OnCosmosIPCMessage(pWndXobj, marshal_as<String^>(strXml));
		//		}
		//		else
		//			Universe::Cosmos::Fire_OnCosmosLoadDocument2Viewport(pWndXobj, marshal_as<String^>(strXml));
		//	}
		//}
	}
}

bool CCosmosProxy::_insertObject(Object^ key, Object^ val)
{
	Hashtable^ htObjects = (Hashtable^)m_htObjects;
	htObjects[key] = val;
	return true;
}

Object^ CCosmosProxy::_getObject(Object^ key)
{
	Hashtable^ htObjects = (Hashtable^)m_htObjects;
	return htObjects[key];
}

bool CCosmosProxy::_removeObject(Object^ key)
{
	Hashtable^ htObjects = (Hashtable^)m_htObjects;

	if (htObjects->ContainsKey(key))
	{
		htObjects->Remove(key);
		return true;
	}
	return false;
}

void CCosmosXobjEvent::OnObserverComplete()
{
	if (m_pXobjCLREvent)
		m_pXobjCLREvent->OnObserverComplete(NULL);
}

void CCosmosXobjEvent::OnTabChange(int nActivePage, int nOldPage)
{
	if (m_pXobj != nullptr)
		m_pXobjCLREvent->OnTabChange(nActivePage, nOldPage);
}

void CCosmosXobjEvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	if (m_pXobj != nullptr)
		m_pXobjCLREvent->OnIPCMessageReceived(bstrFrom, bstrTo, bstrMsgId, bstrPayload, bstrExtra);
}

void CCosmosXobjEvent::OnDestroy()
{
	LONGLONG nValue = (LONGLONG)m_pXobj;
	theAppProxy._removeObject(nValue);
	if (m_pXobjCLREvent)
	{
		m_pXobjCLREvent->OnDestroy();
		delete m_pXobjCLREvent;
		m_pXobjCLREvent = nullptr;
	}
	this->DispEventUnadvise(m_pXobj);
}

void CCosmosXobjEvent::OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl)
{
	if (m_pXobj != nullptr)
		m_pXobjCLREvent->OnDocumentComplete(pDocdisp, bstrUrl);
}

void CCosmosXobjEvent::OnXobjAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
	if (m_pXobj != nullptr)
		m_pXobjCLREvent->OnXobjAddInCreated(pAddIndisp, bstrAddInID, bstrAddInXml);
}

bool CCosmos::EclipseAppInit()
{
	Universe::Cosmos::GetCosmos()->Fire_OnEclipseAppInit();
	return false;
}

bool CCosmos::OnUniversePreTranslateMessage(MSG* pMsg)
{
	return false;
};

void CCosmos::OnCosmosClose(CosmosCloseState state)
{
	switch (state)
	{
	case CosmosCloseState::StartClose:
	{
		AtlTrace(_T("*************Begin CCosmos::OnClose:  ****************\n"));
		Universe::Cosmos::GetCosmos()->Fire_OnClose();
		FormCollection^ pCollection = Forms::Application::OpenForms;
		int nCount = pCollection->Count;
		while (pCollection->Count > 0) {
			Form^ pForm = pCollection[0];
			pForm->Close();
		}
		if (theAppProxy.m_pCosmosWpfApp)
		{
			WindowCollection^ pWnds = theAppProxy.m_pCosmosWpfApp->Windows;
			int nCount = pWnds->Count;
			for (int i = 0; i < pWnds->Count; i++) {
				Window^ pWnd = pWnds[i];
				pWnd->Close();
			}
		}
		AtlTrace(_T("*************End CCosmos::OnClose:  ****************\n"));
	}
	case CosmosCloseState::FinalBrowserClose:
		break;
	case CosmosCloseState::ProcessQuit:
		break;
	}
	//::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 20210420);
}

void CCosmos::OnObserverComplete(HWND hWnd, CString strUrl, IXobj* pRootXobj)
{
	Universe::Cosmos^ pManager = Universe::Cosmos::GetCosmos();
	Universe::Xobj^ _pRootNode = theAppProxy._createObject<IXobj, Universe::Xobj>(pRootXobj);
	IntPtr nHandle = (IntPtr)hWnd;
	pManager->Fire_OnObserverComplete(nHandle, marshal_as<String^>(strUrl), _pRootNode);
	// Notify all descendant nodes under the root node.
	_pRootNode->Fire_RootXobjCreated(nHandle, marshal_as<String^>(strUrl), _pRootNode);
}

CGalaxyClusterEvent::CGalaxyClusterEvent()
{

}

CGalaxyClusterEvent::~CGalaxyClusterEvent()
{
}

void __stdcall  CGalaxyClusterEvent::OnDestroy()
{
	if (m_pGalaxyCluster)
		delete m_pGalaxyCluster;
}

void __stdcall  CGalaxyClusterEvent::OnTabChange(IXobj* sender, int nActivePage, int nOldPage)
{
	Object^ pObj = m_pGalaxyCluster;
	Universe::GalaxyCluster^ pGalaxyCluster = static_cast<Universe::GalaxyCluster^>(pObj);
	Universe::Xobj^ pWndXobj = (Universe::Xobj^)theAppProxy._createObject<IXobj, Universe::Xobj>(sender);
	pGalaxyCluster->Fire_OnTabChange(pWndXobj, nActivePage, nOldPage);
}

void CGalaxyClusterEvent::OnInitialize(IDispatch* pHtmlWnd, BSTR bstrUrl)
{
	Object^ pObj = m_pGalaxyCluster;
	Universe::GalaxyCluster^ pGalaxyCluster = static_cast<Universe::GalaxyCluster^>(pObj);
	pGalaxyCluster->Fire_OnDocumentComplete(pGalaxyCluster, Marshal::GetObjectForIUnknown((IntPtr)pHtmlWnd), marshal_as<String^>(bstrUrl));
}

void CGalaxyClusterEvent::OnIPCMsg(IGalaxy* sender, BSTR bstrType, BSTR bstrContent, BSTR bstrFeature)
{
	Object^ pObj = m_pGalaxyCluster;
	Universe::GalaxyCluster^ pGalaxyCluster = static_cast<Universe::GalaxyCluster^>(pObj);
	Galaxy^ pGalaxy = (Galaxy^)theAppProxy._createObject<IGalaxy, Galaxy>(sender);
	pGalaxyCluster->Fire_OnIPCMsg(pGalaxy, marshal_as<String^>(bstrType), marshal_as<String^>(bstrContent), marshal_as<String^>(bstrFeature));
}

void CCosmosProxy::OnApplicationExit(System::Object^ sender, System::EventArgs^ e)
{
	theAppProxy.m_mapChromeWebPage.clear();
	if (theApp.m_bHostApp)
	{
		theApp.ExitJVM();
	}
}

CTangramWPFObjWrapper::~CTangramWPFObjWrapper(void)
{
	Universe::Xobj^ pXobj = nullptr;
	if (Universe::Cosmos::m_pFrameworkElementDic->TryGetValue(m_pUIElement, pXobj))
	{
		Universe::Cosmos::m_pFrameworkElementDic->Remove(m_pUIElement);
	}
}

void CTangramWPFObjWrapper::ShowVisual(BOOL bShow)
{
	if (bShow)
	{
		m_pUIElement->Visibility = System::Windows::Visibility::Visible;
	}
	else
	{
		m_pUIElement->Visibility = System::Windows::Visibility::Hidden;
	}
}

void CTangramWPFObjWrapper::Focusable(BOOL bFocus)
{
	m_pUIElement->Focusable = bFocus;
}

void CTangramWPFObjWrapper::InvalidateVisual()
{
	if (m_pUIElement)
	{
		m_pUIElement->InvalidateVisual();
	}
}

BOOL CTangramWPFObjWrapper::IsVisible()
{
	return m_pUIElement->IsVisible;
}

CWPFObj* CTangramWPFObjWrapper::CreateControl(Type^ type, HWND parent, DWORD style, int x, int y, int width, int height)
{
	m_pUIElement = (FrameworkElement^)Activator::CreateInstance(type);
	m_pDisp = (IDispatch*)(System::Runtime::InteropServices::Marshal::GetIUnknownForObject(m_pUIElement).ToInt64());
	if (m_pDisp)
	{
		Interop::HwndSourceParameters^ sourceParams = gcnew Interop::HwndSourceParameters("Cosmos WpfControlWrapper");
		sourceParams->PositionX = x;
		sourceParams->PositionY = y;
		sourceParams->Height = height;
		sourceParams->Width = width;
		sourceParams->WindowStyle = style;
		sourceParams->ParentWindow = IntPtr(parent);

		m_pSource = gcnew Interop::HwndSource(*sourceParams);
		m_pSource->AddHook(gcnew Interop::HwndSourceHook(&ChildHwndSourceHook));
		m_hwndWPF = (HWND)m_pSource->Handle.ToPointer();
	}

	return m_hwndWPF == NULL ? nullptr : this;
}

bool CCosmosProxy::PreWindowPosChanging(HWND hWnd, WINDOWPOS* lpwndpos, int nType)
{
	bool bFind = false;
	switch (nType)
	{
	case 1:
	{
		Control^ ctrl = Control::FromHandle(marshal_as<IntPtr>((HANDLE)hWnd)); 
		if (ctrl->GetType()->IsSubclassOf(Form::typeid))
		{
			Form^ thisForm = (Form^)ctrl;
			if (thisForm->IsMdiContainer)
			{
				thisForm->LayoutMdi(MdiLayout::ArrangeIcons);
			}
		}
	}
	break;
	}
	return false;
}

void CCosmosProxy::OnWebPageCreated(HWND hWnd, CWebPageImpl* pProxy, IWebPage* pChromeWebContent, int nState)
{
	if (nState == 0)
	{
		WebPage^ pPage = gcnew WebPage(pChromeWebContent);
		pPage->m_hWnd = hWnd;
		pPage->m_pProxy = pProxy;
		m_mapChromeWebPage[hWnd] = pPage;
	}
	else
	{
		auto it = m_mapChromeWebPage.find(hWnd);
		if (it != m_mapChromeWebPage.end())
		{
			m_mapChromeWebPage.erase(it);
		}
	}
}

HICON CCosmosProxy::GetAppIcon(int nIndex)
{
	System::Drawing::Icon^ icon = Universe::Cosmos::Fire_OnGetAppIcon(nIndex);
	if (icon != nullptr)
		return (HICON)icon->Handle.ToPointer();
	else
	{
		if (Universe::Cosmos::m_pDefaultIcon == nullptr)
		{
			Form^ _pForm = gcnew Form();
			Universe::Cosmos::m_pDefaultIcon = _pForm->Icon;
		}
		return (HICON)Universe::Cosmos::m_pDefaultIcon->Handle.ToPointer();
	}
}

void CCosmosProxy::OnControlAdded(Object^ sender, ControlEventArgs^ e)
{
	String^ strType = e->Control->GetType()->ToString();
	if (strType == L"System.Windows.Forms.MdiClient")
	{
		HWND hWnd = (HWND)marshal_as<HANDLE>(((Form^)sender)->Handle); 
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)e->Control->Handle.ToInt64());
	}
}

void CCosmosProxy::OnControlRemoved(Object^ sender, ControlEventArgs^ e)
{
	String^ strType = sender->GetType()->ToString();
	if (strType == L"System.Windows.Forms.MdiClient")
	{
		Control^ ctrl = (Control^)sender;
		int nCount = ctrl->Controls->Count;
		if (nCount == 0)
		{
			Form^ thisForm = (Form^)ctrl->Parent;
			Universe::Wormhole^ pCloudSession = nullptr;
			bool bExists = Universe::Cosmos::Wormholes->TryGetValue(thisForm, pCloudSession);
			if (bExists)
			{
				pCloudSession->InsertLong(L"MDIChildCount", 0);
				pCloudSession->InsertString(L"msgID", L"MDIFORM_ALLMDICHILDREMOVED");
				pCloudSession->SendMessage();
			}
		}
	}
}

void CCosmosProxy::OnDomainUnload(System::Object^ sender, System::EventArgs^ e)
{
}

void CCosmosProxy::OnProcessExit(System::Object^ sender, System::EventArgs^ e)
{
}
