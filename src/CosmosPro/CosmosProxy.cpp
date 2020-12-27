/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202012280003
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
#pragma managed(push, off)
CCosmos theApp;
#pragma managed(pop)
CCosmosProxy theAppProxy;

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

void CMenuHelperWnd::OnFinalMessage(HWND hWnd)
{
	auto it = theAppProxy.m_mapMenuHelperWnd.find(hWnd);
	if (it != theAppProxy.m_mapMenuHelperWnd.end())
		theAppProxy.m_mapMenuHelperWnd.erase(it);

	it = theAppProxy.m_mapVisibleMenuHelperWnd.find(hWnd);
	if (it != theAppProxy.m_mapVisibleMenuHelperWnd.end())
		theAppProxy.m_mapVisibleMenuHelperWnd.erase(it);
	CWindowImpl::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CMenuHelperWnd::OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CMenuHelperWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	if (wParam == 0)
	{
		//InputLanguage::CurrentInputLanguage = theAppProxy.m_pCurInputLanguage;
		if (theAppProxy.m_pWorkingMenuHelperWnd == this)
			theAppProxy.m_pWorkingMenuHelperWnd = nullptr;
		auto it = theAppProxy.m_mapVisibleMenuHelperWnd.find(m_hWnd);
		if (it != theAppProxy.m_mapVisibleMenuHelperWnd.end())
			theAppProxy.m_mapVisibleMenuHelperWnd.erase(it);
	}
	else
	{
		//theAppProxy.m_pCurInputLanguage = InputLanguage::CurrentInputLanguage;
		//String^ name = L"";
		//for each(InputLanguage^ item in InputLanguage::InstalledInputLanguages)
		//{
		//	name = item->Culture->Name;
		//	if (name == L"en-US")
		//	{
		//		InputLanguage::CurrentInputLanguage = item;
		//		break;
		//	}
		//}		
		theAppProxy.m_mapVisibleMenuHelperWnd[m_hWnd] = this;
		theAppProxy.m_pWorkingMenuHelperWnd = this;
	}
	return DefWindowProc(uMsg, wParam, lParam);
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
	m_pPropertyGrid = nullptr;
	m_pSystemAssembly = nullptr;
	m_pOnLoad = nullptr;
	m_pOnMdiChildActivate = nullptr;
	m_pOnCtrlVisible = nullptr;
	m_htObjects = gcnew Hashtable();
	Forms::Application::ApplicationExit += gcnew EventHandler(&OnApplicationExit);
	if (::GetModuleHandle(_T("universe.dll")) == nullptr)
	{
		theApp.m_bHostApp = true;
		GetCosmos();
		if (theApp.m_pCosmos)
		{
			theApp.m_pCosmosImpl->m_pCLRProxy = this;
			theApp.m_pCosmosImpl->m_pCosmosAppProxy = &theApp;
			//ICosmosExtender* pExtender = nullptr;
			//theApp.m_pCosmos->get_Extender(&pExtender);
			//if (pExtender)
			//{
			//	CComQIPtr<IVSExtender> pVSExtender(pExtender);
			//	if (pVSExtender)
			//		theApp.m_pVSExtender = pVSExtender.Detach();
			//}
		}
	}

	Universe::Cosmos::GetCosmos();
}

CCosmosProxy::~CCosmosProxy()
{
	for (auto it : m_mapGalaxyInfo)
	{
		delete it.second;
	}

	if (theApp.m_bHostApp == false)
		theApp.m_pCosmosImpl->m_pCLRProxy = nullptr;

	//if (m_bHostApp)
	{
		//if (theApp.m_pCosmosImpl->m_pObjectFactory)
		//{
		//	delete theApp.m_pCosmosImpl->m_pObjectFactory;
		//	theApp.m_pCosmosImpl->m_pObjectFactory = nullptr;
		//}
		//BOOL bUnload = ::FreeLibrary(::GetModuleHandle(_T("universe.dll")));
		//while (bUnload)
		//	bUnload = ::FreeLibrary(::GetModuleHandle(_T("universe.dll")));
	}

	ATLTRACE(_T("Release CCosmosProxy :%p\n"), this);
}

void CCosmosProxy::_GetMenuInfo(FormInfo* pInfo, ToolStripMenuItem^ item)
{
	ToolStripMenuItem^ menuitem = (ToolStripMenuItem^)item;
	System::Windows::Forms::Keys keys = menuitem->ShortcutKeys;
	if (keys != System::Windows::Forms::Keys::None)
	{
		pInfo->m_mapShortcutItem[(int)keys] = menuitem;
	}
	if (menuitem->HasDropDownItems)
	{
		ToolStripDropDownItem^ pItem = (ToolStripDropDownItem^)menuitem;
		int Count = pItem->DropDownItems->Count;
		for each (ToolStripItem ^ _item in pItem->DropDownItems)
		{
			if (_item->Text->IndexOf("&") != -1)
				_GetMenuInfo(pInfo, (ToolStripMenuItem^)_item);
		}
	}
}

bool CCosmos::DoIdleWork() {
	if (theAppProxy.m_mapFormMenuStrip2.size())
	{
		auto it = theAppProxy.m_mapFormMenuStrip2.begin();// find(::GetActiveWindow());
		MenuStrip^ pMenuStrip = it->second;
		if (pMenuStrip != nullptr)
		{
			FormInfo* pInfo = new FormInfo();
			theAppProxy.m_mapFormInfo[it->first] = pInfo;
			for each (ToolStripMenuItem ^ item in pMenuStrip->Items)
			{
				theAppProxy._GetMenuInfo(pInfo, item);
			}
		}
		theAppProxy.m_mapFormMenuStrip2.erase(it);
	}
	Universe::Cosmos::GetCosmos()->Fire_OnCloudAppIdle();
	return false;
}

HWND CCosmos::InitCosmosApp()
{
	return 0;
};

void CCosmos::IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2)
{
	Universe::Cosmos::Fire_OnCosmosMsg((IntPtr)hWnd, BSTR2STRING(strType), BSTR2STRING(strParam1), BSTR2STRING(strParam2));
}

void CCosmos::CustomizedDOMElement(HWND hWnd, CString strRuleName, CString strHTML)
{
	Universe::Cosmos::Fire_OnCustomizedDOMElement((IntPtr)hWnd, BSTR2STRING(strRuleName), BSTR2STRING(strHTML));
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
	if (strPath == _T(""))
		strPath = Forms::Application::ExecutablePath->ToLower();
	Assembly^ m_pDotNetAssembly = nullptr;
	try
	{
		m_pDotNetAssembly = Assembly::LoadFile(BSTR2STRING(strPath));
		if (m_pDotNetAssembly == nullptr)
			return;
	}
	catch (ArgumentNullException ^ e)
	{
		Debug::WriteLine(L"Cosmos InitAppFormTypeDic: " + e->Message);
	}
	catch (ArgumentException ^ e)
	{
		Debug::WriteLine(L"Cosmos InitAppFormTypeDic: " + e->Message);
	}
	catch (FileNotFoundException ^ e)
	{
		Debug::WriteLine(L"Cosmos InitAppFormTypeDic: " + e->Message);
	}
	catch (FileLoadException ^ e)
	{
		Debug::WriteLine(L"Cosmos InitAppFormTypeDic: " + e->Message);
	}
	catch (BadImageFormatException ^ e)
	{
		Debug::WriteLine(L"Cosmos InitAppFormTypeDic: " + e->Message);
	}
	finally
	{
		if (m_pDotNetAssembly != nullptr)
		{
			BSTR bstrAssemblyFullName = STRING2BSTR(m_pDotNetAssembly->FullName);
			CString strAssemblyFullName = OLE2T(bstrAssemblyFullName);
			//if (theApp.m_pCosmosImpl->m_mapExcludedObjects.find(strAssemblyFullName) != theApp.m_pCosmosImpl->m_mapExcludedObjects.end())
			//{
			//	// Skip it.
			//}
			//else
			{
				cli::array<Type^>^ pArray = m_pDotNetAssembly->GetExportedTypes();
				for each (Type ^ type in pArray)
				{
					BSTR bstrTypeFullName = STRING2BSTR(type->FullName);
					CString strTypeFullName = OLE2T(bstrTypeFullName);
					//if (theApp.m_pCosmosImpl->m_mapExcludedObjects.find(strTypeFullName) != theApp.m_pCosmosImpl->m_mapExcludedObjects.end())
					//{
					//	// Skip it.
					//	continue;
					//}
					if (type->IsSubclassOf(Form::typeid))
					{
						Form^ m_pObj = nullptr;
						try
						{
							m_pObj = (Form^)Activator::CreateInstance(type);
							if (m_pObj && (m_pObj->FormBorderStyle != FormBorderStyle::FixedToolWindow || m_pObj->FormBorderStyle != FormBorderStyle::None || m_pObj->FormBorderStyle != FormBorderStyle::SizableToolWindow))
							{
								if (m_pObj->Controls->Count)
								{
									HICON hIcon = (HICON)m_pObj->Icon->Handle.ToPointer();
									if (hIcon)
									{
										CString strPath2 = theApp.m_pCosmosImpl->m_strAppFormsInfoPath;
										strPath2 += STRING2BSTR(type->AssemblyQualifiedName);
										strPath2 += _T(".ico");
										//Write Icon to File Stream
										System::IO::FileStream^ fs = gcnew System::IO::FileStream(BSTR2STRING(strPath2), System::IO::FileMode::OpenOrCreate);
										m_pObj->Icon->Save(fs);
										fs->Close();
										delete fs;
									}
								}
							}
						}
						catch (Exception^)
						{
						}
						finally
						{
							if (m_pObj)
								m_pObj->Close();
						}
					}
					else if (type->IsSubclassOf(Control::typeid))
					{
						try
						{
							CString strPath2 = theApp.m_pCosmosImpl->m_strAppControlsInfoPath;
							strPath2 += STRING2BSTR(type->AssemblyQualifiedName);
							strPath2 += _T(".xml");
							CString strXml = _T("<control></control>");
							CTangramXmlParse m_Parse;
							m_Parse.LoadXml(strXml);
							m_Parse.SaveFile(strPath2);
						}
						catch (Exception^)
						{
						}
						finally
						{
						}
					}
					else if (type->IsSubclassOf(FrameworkElement::typeid))
					{
						try
						{
							CString strPath2 = theApp.m_pCosmosImpl->m_strAppWPFObjsInfoPath;
							strPath2 += STRING2BSTR(type->AssemblyQualifiedName);
							strPath2 += _T(".xml");
							CString strXml = _T("<control></control>");
							CTangramXmlParse m_Parse;
							m_Parse.LoadXml(strXml);
							m_Parse.SaveFile(strPath2);
						}
						catch (Exception^)
						{
						}
						finally
						{
						}
					}
					::SysFreeString(bstrTypeFullName);
				}
			}
			::SysFreeString(bstrAssemblyFullName);
		}
	}
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
		Form^ pForm = Universe::Cosmos::CreateForm(it->second, BSTR2STRING(strXML));
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

		Type^ pType = Universe::Cosmos::GetType(BSTR2STRING(bstrObjTypeID));
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
				catch (Markup::XamlParseException ^ e)
				{
					Debug::WriteLine(L"Cosmos WPFControlWrapper Exception 1: " + e->Message);
					Debug::WriteLine(L"Cosmos WPFControlWrapper Exception 1: " + e->InnerException->Message);
				}
			}
		}
		catch (System::Exception ^ ex)
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

		if (hPWnd == NULL && _strClassName.Find(_T("WindowsForms")) != -1)
		{
			bool bMenu = false;
			if (m_hCreatingCLRWnd)
			{
				Control^ pControl = Control::FromHandle((IntPtr)m_hCreatingCLRWnd);
				if (pControl)
				{
					bMenu = pControl->GetType()->Name == L"ToolStripDropDownMenu";
				}
			}
			if (bMenu == false)
				m_hCreatingCLRWnd = hWnd;
			::PostMessage(theApp.m_pCosmosImpl->m_hHostWnd, WM_COSMOSMSG, (WPARAM)hWnd, 20200120);
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
	auto it4 = m_mapFormMenuStrip.find(hWnd);
	if (it4 != m_mapFormMenuStrip.end())
	{
		m_mapFormMenuStrip.erase(it4);
	}
	auto it6 = m_mapFormInfo.find(hWnd);
	if (it6 != m_mapFormInfo.end())
	{
		FormInfo* pInfo = it6->second;
		delete it6->second;
		m_mapFormInfo.erase(it6);
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
						BSTR strName = STRING2BSTR(name->ToLower());
						GalaxyInfo* pInfo = new GalaxyInfo;
						pInfo->m_strXobjXml = _T("");
						pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
						pInfo->m_pDisp = nullptr;
						pInfo->m_pParentDisp = nullptr;
						theAppProxy.m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
						pInfo->m_strCtrlName = pChild->Name->ToLower();
						pInfo->m_strParentCtrlName = pChild->Name->ToLower();
						theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
						::SysFreeString(strName);
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
		BSTR bstrName = STRING2BSTR(pChild->Name); //OK!
		IGalaxyCluster* pXobj = theApp.m_pCosmosImpl->Observe((HWND)pChild->Handle.ToInt64(), OLE2T(bstrName), _T("default"));
		::SysFreeString(bstrName);
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
						pGalaxy->SendMessage(pCtrl->Name, pCtrl->Handle.ToInt64().ToString("d"), L"ControlBindMessage:MouseDoubleClick", strTag, BSTR2STRING(strBindName));
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
					pGalaxy->SendMessage(BSTR2STRING(strActionName), pCtrl->Handle.ToInt64().ToString("d"), L"ControlBindMessage:MouseDoubleClick", strTag, BSTR2STRING(strBindName));
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
			PropertyInfo^ ppi = pObj->GetType()->GetProperty(BSTR2STRING(bstrPropertyName));
			if (ppi != nullptr)
			{
				ppi->SetValue(pObj, BSTR2STRING(bstrPropertyValue), nullptr);
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
		else if (theApp.m_pCosmosImpl->m_strAppCurrentFormTemplatePath != _T(""))
		{
			::SendMessage(hWnd, WM_HUBBLE_DATA, (WPARAM)theApp.m_pCosmosImpl->m_strAppCurrentFormTemplatePath.GetBuffer(), 3);
		}
		theApp.m_pCosmosImpl->m_strAppCurrentFormTemplatePath = _T("");
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
			theAppProxy.m_mapFormMenuStrip[hWnd] = pMenuStrip;
			theAppProxy.m_mapFormMenuStrip2[hWnd] = pMenuStrip;
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
				if (m_pOnMdiChildActivate)
				{
				}
				else
				{
					m_pOnMdiChildActivate = gcnew EventHandler(CCosmosProxy::OnMdiChildActivate);
				}
				pForm->MdiChildActivate += m_pOnMdiChildActivate;
			}
			if (String::IsNullOrEmpty(name))
				name = strType;
			CTangramXmlParse* _pChild = pParse->GetChild(name->ToLower());
			if (_pChild&&pChild != pActiveCtrl)
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
										pTreeView->Tag = BSTR2STRING(_pChild2->xml());
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
										pListView->Tag = BSTR2STRING(_pChild2->xml());
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
										pBtn->Tag = BSTR2STRING(_pChild2->xml());
									}
								}
							}

							BSTR strName = STRING2BSTR(name->ToLower());
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
								IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
								::SysFreeString(strName);

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
											IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
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
										IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
									}
								}
							}
							::SysFreeString(strName);
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
	if (::IsChild(theApp.m_pCosmosImpl->m_hHostWnd, (HWND)pCtrl->Handle.ToInt64()))
		return nullptr;
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
					name += L"." + BSTR2STRING(bstrName);
					BSTR strName = STRING2BSTR(name->ToLower());//OK!
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
							IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
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
									pTreeView->Tag = BSTR2STRING(_pChild2->xml());
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
									pListView->Tag = BSTR2STRING(_pChild2->xml());
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
									pBtn->Tag = BSTR2STRING(_pChild->xml());
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
							IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
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

					::SysFreeString(strName);
				}
			}
			InitXobj(_pXobj, pChild, bSave, pParse);
		}
	}

	return pGalaxyCluster;
}

void CCosmosProxy::OnMdiChildActivate(System::Object^ sender, System::EventArgs^ e)
{
	Form^ pForm = static_cast<Form^>(sender);
	String^ strKey = L"";
	if (pForm->ActiveMdiChild != nullptr)
	{
		strKey = pForm->ActiveMdiChild->GetType()->FullName->ToLower();
		LRESULT l = ::SendMessage((HWND)pForm->ActiveMdiChild->Handle.ToPointer(), WM_HUBBLE_DATA, 0, 2);
		if (l)
		{
			CString strPath = (LPCTSTR)l;
			theApp.m_pCosmos->ObserveGalaxys(pForm->Handle.ToInt64(), CComBSTR(L""), CComBSTR(strPath), CComBSTR(L""), false);
			HWND hWnd = (HWND)pForm->ActiveMdiChild->Handle.ToPointer();
			::PostMessage(hWnd, WM_COSMOSMSG, 0, 20200216);
			return;
		}
		Object^ objTag = pForm->Tag;
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
	}
	BSTR bstrKey = STRING2BSTR(strKey);
	theApp.m_pCosmos->ObserveGalaxys(pForm->Handle.ToInt64(), CComBSTR(L""), bstrKey, CComBSTR(L""), true);
	::SysFreeString(bstrKey);
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
						TreeNode^ pXobj = pTreeView->Nodes->Add(BSTR2STRING(m_Parse.attr(_T("text"), _T(""))));
						pXobj->ImageIndex = m_Parse.attrInt(_T("imageindex"), 0);
						pXobj->SelectedImageIndex = m_Parse.attrInt(_T("selectedimageindex"), 0);
						CString strTagName = ctrl->Name->ToLower() + _T("_tag");
						CTangramXmlParse* pChild = m_Parse.GetChild(strTagName);
						if (pChild)
						{
							pXobj->Tag = BSTR2STRING(pChild->xml());
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
							ListViewItem^ pItem = pListView->Items->Add(BSTR2STRING(pChildParse->attr(_T("text"), _T(""))), pChildParse->attrInt(_T("imageindex")));
							pItem->Tag = BSTR2STRING(pChildParse->xml());
							pItem->ToolTipText = BSTR2STRING(pChildParse->attr(_T("tooptips"), _T("")));
						}
					}
				}
			}
		}
		break;
		}
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
						TreeNode^ pChildNode = pXobj->Nodes->Add(BSTR2STRING(_pParse->attr(_T("text"), _T(""))));
						if (pChildNode)
						{
							pChildNode->ImageIndex = _pParse->attrInt(_T("imageindex"), 0);
							pChildNode->SelectedImageIndex = _pParse->attrInt(_T("selectedimageindex"), 0);
							CString strTagName = name + _T("_tag");
							CTangramXmlParse* pChild2 = pParse->GetChild(strTagName);
							if (pChild2)
							{
								pChildNode->Tag = BSTR2STRING(pChild2->xml());
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
			theApp.m_pCosmos->ObserveGalaxys(pForm->Handle.ToInt64(), CComBSTR(L""), STRING2BSTR(strKey), CComBSTR(L""), true);
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

				ToolStripSeparator^ toolStripSeparator = gcnew ToolStripSeparator();
				toolStripSeparator->Name = "defaulttoolStripSeparator";
				toolStripSeparator->Size = System::Drawing::Size(6, 39);
				defaultToolStrip->Items->Add(toolStripSeparator);

				ToolStripButton^ pToolStripButton = gcnew ToolStripButton();
				pToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				pToolStripButton->Image = (System::Drawing::Image^)(pForm->Icon->ToBitmap());
				pToolStripButton->ImageTransparentColor = System::Drawing::Color::Black;
				pToolStripButton->Name = L"defaultbtn";
				pToolStripButton->Size = System::Drawing::Size(36, 36);
				pToolStripButton->Text = L"default";
				pToolStripButton->Tag = L"default";
				pToolStripButton->Checked = true;
				pToolStripButton->CheckOnClick = true;
				//pToolStripButton->ToolTipText = BSTR2STRING(strTips);
				pToolStripButton->Click += gcnew System::EventHandler(&OnClick);
				defaultToolStrip->Items->Add(pToolStripButton);

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
						Type^ pType = Universe::Cosmos::GetType(BSTR2STRING(strID));
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
									pToolStripButton->Name = BSTR2STRING(it.first);
									pToolStripButton->Size = System::Drawing::Size(36, 36);
									pToolStripButton->Text = BSTR2STRING(it.first);
									pToolStripButton->Tag = BSTR2STRING(it.second);
									CString strTips = m_Parse.attr(_T("tooltips"), _T(""));
									if (strTips != _T(""))
										pToolStripButton->ToolTipText = BSTR2STRING(strTips);
									else
									{
										strTips = m_Parse.text();
										if (strTips != _T(""))
											pToolStripButton->ToolTipText = BSTR2STRING(strTips);
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
			Object^ pObj = Universe::Cosmos::CreateObject(BSTR2STRING(strKey));
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
				Object^ pObj = Universe::Cosmos::CreateObject(BSTR2STRING(strKey));
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
	String^ strObjID = BSTR2STRING(bstrObjID);
	String^ strMethod = BSTR2STRING(bstrMethod);
	String^ strData = BSTR2STRING(bstrData);
	cli::array<Object^, 1>^ pObjs = { BSTR2STRING(bstrParam), BSTR2STRING(bstrData) };
	Universe::Cosmos::ActiveMethod(strObjID, strMethod, pObjs);
	return S_OK;
}

HRESULT CCosmosProxy::ActiveCLRMethod(IDispatch* pCLRObj, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData)
{
	Object^ obj = Marshal::GetObjectForIUnknown((IntPtr)pCLRObj);
	String^ strMethod = BSTR2STRING(bstrMethod);
	String^ strData = BSTR2STRING(bstrData);
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
					if (pChild&&pWebPage != nullptr)
					{
						pPage = gcnew WebPage(pWebPage);
						pPage->m_hWnd = (HWND)m_Parse.attrInt64(_T("webpagehandle"), 0);
						IXobj* pXobj = nullptr;
						pWebPage->Observe(CComBSTR(strTagName), CComBSTR(pChild->xml()), &pXobj);
					}
				}
				Object^ pObj = Universe::Cosmos::CreateObject(BSTR2STRING(strObjID));

				if (pObj != nullptr)
				{
					if (pObj->GetType()->IsSubclassOf(Form::typeid))
					{
						m_strCurrentWinFormTemplate = bstrObjID;
						CString strCaption = m_Parse.attr(_T("caption"), _T(""));
						Form^ thisForm = (Form^)pObj;
						if (nHandle)
						{
							pProxyBase->OnWinFormCreated((HWND)thisForm->Handle.ToPointer());
						}
						int nWidth = m_Parse.attrInt(_T("width"), 0);
						int nHeight= m_Parse.attrInt(_T("height"), 0);
						if (nWidth * nHeight)
						{
							thisForm->Width = nWidth;
							thisForm->Height = nHeight;
						}
						if (m_pCurrentPForm)
						{
							if(thisForm->IsMdiContainer==false)
								thisForm->MdiParent = m_pCurrentPForm;
							m_pCurrentPForm = nullptr;
						}

						if (strCaption != _T(""))
							thisForm->Text = BSTR2STRING(strCaption);
						if (thisForm->IsMdiContainer)
						{
							Control^ mdiclient = Universe::Cosmos::GetMDIClient(thisForm);
							mdiclient->ControlRemoved += gcnew System::Windows::Forms::ControlEventHandler(&OnControlRemoved);
							CString strBKPage = m_Parse.attr(_T("mdibkpageid"), _T(""));
							if (strBKPage != _T(""))
							{
								Universe::Cosmos::CreateBKPage(thisForm, BSTR2STRING(strBKPage));
							}
						}
						if (strTagName.CompareNoCase(_T("mainwindow")) == 0)
						{
							theApp.m_pCosmosImpl->m_hMainWnd = (HWND)thisForm->Handle.ToPointer();
							::PostMessage(theApp.m_pCosmosImpl->m_hMainWnd, WM_COSMOSMSG, 0, 20200419);
						}
						thisForm->Tag = BSTR2STRING(m_Parse.name());
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
								BSTR bstrName = STRING2BSTR(thisForm->Name);
								strFormName = OLE2T(bstrName);
								::SysFreeString(bstrName);
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
								BSTR bstrName = STRING2BSTR(thisForm->Name);
								strFormName = OLE2T(bstrName);
								::SysFreeString(bstrName);
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
							if(nAddSubFormMap)
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
									thisForm->Focus();
								}

								thisForm->WindowState = FormWindowState::Normal;
								::PostMessage(::GetParent(pPage->m_hWnd), WM_BROWSERLAYOUT, 0, 4);
								thisForm->Focus();
								::PostMessage((HWND)thisForm->Handle.ToPointer(), WM_HUBBLE_DATA, 0, 20201225);
								break;
							}
						}
						else
							thisForm->Show();

						CTangramXmlParse* pChildForms = m_Parse.GetChild(_T("childforms"));
						if (pChildForms)
						{
							::SendMessage((HWND)thisForm->Handle.ToPointer(), WM_COSMOSMSG, (WPARAM)pChildForms, 20190601);
						}
						thisForm->Focus();
						::PostMessage((HWND)thisForm->Handle.ToPointer(), WM_HUBBLE_DATA, 0, 20201225);
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
						mainForm->Text = BSTR2STRING(strCaption);
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
							Universe::Cosmos::CreateBKPage(mainForm, BSTR2STRING(strBKPage));
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

	Object^ pObj = Universe::Cosmos::CreateObject(BSTR2STRING(bstrObjID));

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

void CCosmosProxy::SelectXobj(IXobj* pXobj)
{
	if (pXobj == nullptr)
	{
		return;
	}
	Object^ pObj = nullptr;
	try
	{
		if (pXobj)
			pObj = theAppProxy._createObject<IXobj, Universe::Xobj>(pXobj);
	}
	catch (...)
	{

	}
	finally
	{
		if (pObj != nullptr)
		{
			try
			{
				m_pPropertyGrid->SelectedObject = pObj;
			}
			catch (...)
			{

			}
		}
		else
		{
			m_pPropertyGrid->SelectedObject = nullptr;
		}
	}
}

IDispatch* CCosmosProxy::CreateObject(BSTR bstrObjID, HWND hParent, IXobj* pHostNode)
{
	String^ strID = BSTR2STRING(bstrObjID);
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
		label->Text = BSTR2STRING(strInfo);
		return (IDispatch*)(Marshal::GetIUnknownForObject(label).ToInt64());
	}
	if (_pObj->GetType()->IsSubclassOf(Control::typeid))
	{
		Control^ pObj = static_cast<Control^>(_pObj);
		if (pObj != nullptr && pHostNode)
		{
			__int64 h = 0;
			pHostNode->get_Handle(&h);
			if (h)
				::SendMessage((HWND)h, WM_COSMOSMSG, 0, 19920612);
			IXobj* pRootXobj = NULL;
			pHostNode->get_RootXobj(&pRootXobj);
			CComBSTR bstrName(L"");
			pHostNode->get_Name(&bstrName);
			CString strName = OLE2T(bstrName);
			bool bProperty = false;
			if (strName.CompareNoCase(_T("TangramPropertyGrid")) == 0)
			{
				bProperty = true;
				m_pPropertyGrid = (PropertyGrid^)pObj;
				m_pPropertyGrid->ToolbarVisible = false;
				m_pPropertyGrid->PropertySort = PropertySort::Alphabetical;
			}

			HWND hWnd = (HWND)pObj->Handle.ToInt64();
			theApp.m_pCosmosImpl->m_mapXobj[hWnd] = pHostNode;

			IDispatch* pDisp = (IDispatch*)(Marshal::GetIUnknownForObject(pObj).ToInt64());
			_pXobj->m_pHostObj = pObj;

			if (pObj->GetType()->IsSubclassOf(Form::typeid))
			{
				::SetParent(hWnd, (HWND)hParent);
				auto it = m_mapForm.find(hWnd);
				if (it != m_mapForm.end())
					m_mapForm.erase(it);
				theApp.m_pCosmosImpl->m_hFormNodeWnd = hWnd;
				::SetWindowLongPtr(hWnd, GWL_STYLE, ::GetWindowLongPtr(hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_BORDER | WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION) | WS_CHILD );// | WS_VISIBLE);
				::SetWindowLongPtr(hWnd, GWL_EXSTYLE, ::GetWindowLongPtr(hWnd, GWL_EXSTYLE) & ~(WS_EX_APPWINDOW/*|WS_EX_CLIENTEDGE*/));
				Universe::Cosmos::Fire_OnFormNodeCreated(BSTR2STRING(bstrObjID), (Form^)pObj, _pXobj);
				((Form^)pObj)->Show();
				return pDisp;
			}
			if (theApp.m_pCosmosImpl->IsMDIClientGalaxyNode(pHostNode) == false && bProperty == false)
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
				IDispatch* pDisp = (IDispatch*)(Marshal::GetIUnknownForObject(pElementHost).ToInt64());
				_pXobj->m_pHostObj = pElementHost;
				return pDisp;
			}
			catch (System::Exception ^ ex)
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
				pCtrl = Ctrls[BSTR2STRING(bstrNames)];
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
					Control^ pCtrl2 = pCtrl->Controls[BSTR2STRING(strName)];
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
					Control^ pCtrl2 = pCtrl->Controls[BSTR2STRING(strNames)];
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
		return STRING2BSTR(pCtrl->Name);
	return L"";
}

void CCosmosProxy::ReleaseCosmosObj(IDispatch* pDisp)
{
	LONGLONG nValue = (LONGLONG)pDisp;
	Object^ pObj = (Object^)Marshal::GetObjectForIUnknown((IntPtr)pDisp);
	Universe::Wormhole^ pCloudSession = nullptr;
	bool bExists = Universe::Cosmos::Wormholes->TryGetValue(pObj, pCloudSession);
	if (bExists == true)
	{
		if (pCloudSession != nullptr)
		{
			Universe::Cosmos::Wormholes->Remove(pObj);
		}
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
			cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(BSTR2STRING(bstrName), bFindInChild);
			if (pArray != nullptr && pArray->Length)
				return (IDispatch*)Marshal::GetIUnknownForObject(pArray[0]).ToPointer();
		}

	}
	catch (System::Exception^)
	{

	}
	return NULL;
}

void CCosmosProxy::SelectObj(IDispatch* CtrlDisp)
{
	try
	{
		Object^ pCtrl = (Object^)Marshal::GetObjectForIUnknown((IntPtr)CtrlDisp);
		if (pCtrl != nullptr)
		{
			m_pPropertyGrid->SelectedObject = pCtrl;
		}

	}
	catch (System::Exception ^ e)
	{
		String^ strInfo = e->Message;
	}
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
				cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(BSTR2STRING(bstrName), bFindInChild);
				if (pArray != nullptr && pArray->Length)
				{
					return STRING2BSTR(pArray[0]->Text);
				}
			}
			else
				return STRING2BSTR(pCtrl->Text);
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
				cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(BSTR2STRING(bstrName), bFindInChild);
				if (pArray != nullptr && pArray->Length)
				{
					pArray[0]->Text = BSTR2STRING(strVal);
					return;
				}
			}
		}
		else
			pCtrl->Text = BSTR2STRING(strVal);
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
			String^ ctrlName = BSTR2STRING(strName);
			Control^ ctrl = (Control^)thisNode->XObject;
			cli::array<Control^, 1>^ pArray = ctrl->Controls->Find(ctrlName, true);
			if (pArray != nullptr && pArray->Length)
			{
				Control^ _ctrl = pArray[0];
				Universe::Cosmos::Observe(_ctrl, BSTR2STRING(strKey), BSTR2STRING(strXml));
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
						cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(BSTR2STRING(strObjName), true);
						if (pArray != nullptr && pArray->Length)
						{
							pSubCtrl = pArray[0];
							Universe::Wormhole^ pCloudSession2 = nullptr;
							if (!Universe::Cosmos::Wormholes->TryGetValue(pSubCtrl, pCloudSession2))
							{
								Universe::Cosmos::Wormholes[pSubCtrl] = pCloudSession;
							}
							Universe::Cosmos::Fire_OnBindCLRObjToWebPage(pSubCtrl, pCloudSession, BSTR2STRING(strEventName));

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
					CString strName = pChild->attr(_T("refname"),_T(""));
					CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)hRefWnd, _T("CosmosInfo"));
					if (pInfo)
						pRefXobj = pInfo->m_pXobj;
					if (pRefXobj)
					{
						thisNode[BSTR2STRING(strName)] = theAppProxy._createObject<IXobj, Universe::Xobj>(pRefXobj);
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
				thisNode->Caption = BSTR2STRING(pSession->GetString(L"caption"));
			}
			else if (pObj->GetType()->IsSubclassOf(Control::typeid))
			{
				Control^ pCtrl = (Control^)pObj;
				Control^ pSubCtrl = nullptr;
				String^ _strSubObjName = BSTR2STRING(strSubObj);
				if (String::IsNullOrEmpty(_strSubObjName) == false)
				{
					cli::array<Control^, 1>^ pArray = pCtrl->Controls->Find(_strSubObjName, true);
					if (pArray != nullptr && pArray->Length)
					{
						pSubCtrl = pArray[0];
						pSubCtrl->Text = BSTR2STRING(pSession->GetString(strSubObj));
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
						String^ _strCtrls = BSTR2STRING(strCtrls);
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
					_strEventName = BSTR2STRING(strEventName.Left(nPos));
					_strSubObjName = BSTR2STRING(strEventName.Mid(nPos + 1));
				}
				else
				{
					_strEventName = BSTR2STRING(strEventName);
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
					for (auto it : pInfo->m_mapFormsInfo)
					{
						CString strXml = it.second;
						CTangramXmlParse m_Parse;
						if (m_Parse.LoadXml(strXml))
						{
							CString strID = m_Parse.attr(_T("objid"), _T(""));
							if (strID != _T(""))
							{
								Type^ pType = Universe::Cosmos::GetType(BSTR2STRING(strID));
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
									ListViewItem^ pItem = pList->Items->Add(BSTR2STRING(it.first));
									pItem->ImageIndex = nIndex;
									pItem->Tag = BSTR2STRING(strXml);
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
		return STRING2BSTR(pCtrl->GetType()->FullName);
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
						CTangramXmlParse m_Parse;
						if (m_Parse.LoadFile(theApp.m_pCosmosImpl->m_strStartupURL))
						{

						}
						else
						{
						}
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
			ExportAllCLRObjInfo(theApp.m_pCosmosImpl->m_strAppFormsPath);
			ExportAllCLRObjInfo(theApp.m_pCosmosImpl->m_strAppCommonFormsPath);
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
				pWndXobj->ActionData = BSTR2STRING(bstrXml);
				Universe::Cosmos::Fire_OnCosmosActionDelegate(pWndXobj, L"");
			}
			return;
		}
		if (strXml.CompareNoCase(_T("Debug:")) >= 0)
		{
			strXml.Replace(_T("Debug:"), _T(""));
			Universe::Cosmos::Fire_OnCosmosDebugDelegate(BSTR2STRING(strXml));
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
							cli::array<Object^, 1>^ pObjs = { BSTR2STRING(strXml), pWndXobj };
							Universe::Cosmos::ActiveMethod(BSTR2STRING(strID), BSTR2STRING(strMethod), pObjs);
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
		//			Universe::Cosmos::Fire_OnCosmosIPCMessage(pWndXobj, BSTR2STRING(strXml));
		//		}
		//		else
		//			Universe::Cosmos::Fire_OnCosmosLoadDocument2Viewport(pWndXobj, BSTR2STRING(strXml));
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
	switch (pMsg->message)
	{
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_POINTERDOWN:
	case WM_SETWNDFOCUSE:
	{
		Form^ m_pCurrentForm = nullptr;
		Form^ m_pCurrentParentForm = nullptr;
		IntPtr handle = (IntPtr)pMsg->hwnd;
		while (handle != IntPtr::Zero)
		{
			Control^ ctl = Control::FromHandle(handle);
			if (ctl != nullptr)
			{
				if (ctl->GetType()->IsSubclassOf(Form::typeid))
				{
					m_pCurrentForm = (Form^)ctl;
					if (m_pCurrentForm->MdiParent)
					{
						m_pCurrentParentForm = m_pCurrentForm;
						m_pCurrentForm = nullptr;
					}
					else
						break;
				}
			}

			handle = (IntPtr)::GetAncestor((HWND)handle.ToPointer(), GA_PARENT);
		}
		if (m_pCurrentForm)
		{
			auto it = theAppProxy.m_mapFormMenuStrip.find((HWND)m_pCurrentForm->Handle.ToPointer());
			if (it != theAppProxy.m_mapFormMenuStrip.end())
			{
				for each (ToolStripItem ^ item in it->second->Items)
				{
					ToolStripMenuItem^ menuitem = (ToolStripMenuItem^)item;
					menuitem->Checked = false;
					menuitem->HideDropDown();
				}
			}
		}
		HWND h = pMsg->hwnd;
		auto it = theAppProxy.m_mapVisibleMenuHelperWnd.find(h);
		if (it == theAppProxy.m_mapVisibleMenuHelperWnd.end())
		{
			theApp.m_pCosmosImpl->m_pCLRProxy->HideMenuStripPopup();
		}
		if (m_pCurrentParentForm/*&& WM_LBUTTONDOWN== pMsg->message*/)
		{
			System::Windows::Forms::Message Msg = System::Windows::Forms::Message::Create((IntPtr)pMsg->hwnd, pMsg->message, (IntPtr)(__int64)(pMsg->wParam), (IntPtr)pMsg->lParam);
			//m_pCurrentParentForm->PreProcessMessage(Msg);
			Control^ pCtrl = Control::FromHandle((IntPtr)pMsg->hwnd);
			//pCtrl->PreProcessMessage(Msg);
			//TranslateMessage(pMsg);
			//::DispatchMessage(pMsg);
			System::Windows::Forms::PreProcessControlState state = pCtrl->PreProcessControlMessage(Msg);
			if (state == System::Windows::Forms::PreProcessControlState::MessageProcessed)
				return true;
			else
				return false;
		}
	}
	break;
	case WM_KEYDOWN:
	{
		switch (pMsg->wParam)
		{
		case VK_PRIOR:
		case VK_NEXT:
		case VK_END:
		case VK_LEFT:
		case VK_RIGHT:
		{
			Form^ m_pCurrentForm = nullptr;
			IntPtr handle = (IntPtr)pMsg->hwnd;
			while (handle != IntPtr::Zero)
			{
				Control^ ctl = Control::FromHandle(handle);
				if (ctl != nullptr)
				{
					if (ctl->GetType()->IsSubclassOf(Form::typeid))
					{
						m_pCurrentForm = (Form^)ctl;
						if (m_pCurrentForm->MdiParent)
							m_pCurrentForm = nullptr;
						else
							break;
					}
				}

				handle = (IntPtr)::GetAncestor((HWND)handle.ToPointer(), GA_PARENT);
			}
			if (m_pCurrentForm == nullptr)
			{
				HWND hWnd = (HWND)::SendMessage(::GetActiveWindow(), WM_COSMOSMSG, 20200128, 0);
				if (hWnd)
				{
					Control^ ctl = Control::FromHandle((IntPtr)hWnd);
					if (ctl && ctl->GetType()->IsSubclassOf(Form::typeid))
					{
						m_pCurrentForm = (Form^)ctl;
					}
				}
			}
			if (m_pCurrentForm)
			{
				auto it = theAppProxy.m_mapFormMenuStrip.find((HWND)m_pCurrentForm->Handle.ToPointer());
				if (it != theAppProxy.m_mapFormMenuStrip.end())
				{
					if (pMsg->wParam != VK_UP && pMsg->wParam != VK_DOWN)
					{
						for each (ToolStripItem ^ item in it->second->Items)
						{
							if (item->Selected == true)
							{
								ArrowDirection direct = ArrowDirection::Right;
								if (pMsg->wParam == VK_LEFT)
									direct = ArrowDirection::Left;
								ToolStripItem^ next = it->second->GetNextItem(item, direct);
								next->Select();
								ToolStripMenuItem^ menuitem = (ToolStripMenuItem^)next;
								menuitem->ShowDropDown();
								if (menuitem->DropDown->Items->Count)
									menuitem->DropDown->Items[0]->Select();
								return false;
							}
						}
					}
				}
			}
		}
		break;
		case VK_UP:
		case VK_DOWN:
		{
			if (theAppProxy.m_pWorkingMenuHelperWnd)
			{
				ToolStripItem^ pSelectedItem = nullptr;
				ToolStripDropDownMenu^ pToolStripDropDownMenu = theAppProxy.m_pWorkingMenuHelperWnd->m_pToolStripDropDownMenu;
				for each (ToolStripItem ^ item in pToolStripDropDownMenu->Items)
				{
					if (item->Selected)
					{
						pSelectedItem = item;
						break;
					}
				}
				if (pSelectedItem == nullptr)
				{
					pSelectedItem = pToolStripDropDownMenu->Items[0];
					pSelectedItem->Select();
				}
				else
				{
					ArrowDirection direct = ArrowDirection::Up;
					if (pMsg->wParam == VK_DOWN)
						direct = ArrowDirection::Down;
					ToolStripItem^ next = pToolStripDropDownMenu->GetNextItem(pSelectedItem, direct);
					next->Select();
				}
			}
		}
		break;
		case VK_RETURN:
		{
			if (theAppProxy.m_pWorkingMenuHelperWnd)
			{
				ToolStripDropDownMenu^ pToolStripDropDownMenu = theAppProxy.m_pWorkingMenuHelperWnd->m_pToolStripDropDownMenu;
				theAppProxy.m_pWorkingMenuHelperWnd = nullptr;
				ToolStripItem^ pSelectedItem = nullptr;
				for each (ToolStripItem ^ item in pToolStripDropDownMenu->Items)
				{
					if (item->Selected)
					{
						pSelectedItem = item;
						break;
					}
				}
				if (pSelectedItem)
					pSelectedItem->PerformClick();
				return true;
			}
		}
		break;
		}
		break;
	}
	case WM_SYSKEYDOWN:
	{
		if (::GetAsyncKeyState(VK_MENU) < 0)
		{
			int key = (int)pMsg->wParam;
			ToolStripMenuItem^ pSelectedItem = nullptr;
			HWND hWnd = ::GetActiveWindow();
			if (theAppProxy.m_pWorkingMenuHelperWnd)
			{
				ToolStripDropDownMenu^ pToolStripDropDownMenu = theAppProxy.m_pWorkingMenuHelperWnd->m_pToolStripDropDownMenu;
				for each (ToolStripItem ^ item in pToolStripDropDownMenu->Items)
				{
					String^ strText = item->Text;
					int nIndex = strText->IndexOf(L"&");
					if (nIndex != -1)
					{
						wchar_t s = strText[nIndex + 1];
						if (s == key)
						{
							pSelectedItem = (ToolStripMenuItem^)item;
							break;
						}
						strText = strText->ToUpper();
						s = strText[nIndex + 1];
						if (s == key)
						{
							pSelectedItem = (ToolStripMenuItem^)item;
							break;
						}
						strText = strText->ToLower();
						s = strText[nIndex + 1];
						if (s == key)
						{
							pSelectedItem = (ToolStripMenuItem^)item;
							break;
						}
					}
				}
				if (pSelectedItem)
				{
					pSelectedItem->PerformClick();
					return true;
				}
			}
			if (pSelectedItem == nullptr)
			{
				MenuStrip^ pMenuStrip = nullptr;
				auto it = theAppProxy.m_mapFormMenuStrip.find(hWnd);
				if (it != theAppProxy.m_mapFormMenuStrip.end())
				{
					pMenuStrip = it->second;
				}
				else
				{
					hWnd = (HWND)::SendMessage(hWnd, WM_COSMOSMSG, 20200128, 0);
					if (hWnd)
					{
						auto it = theAppProxy.m_mapFormMenuStrip.find(hWnd);
						if (it != theAppProxy.m_mapFormMenuStrip.end())
						{
							pMenuStrip = it->second;
						}
					}
				}
				if (pMenuStrip)
				{
					for each (ToolStripItem ^ item in pMenuStrip->Items)
					{
						String^ strText = item->Text;
						int nIndex = strText->IndexOf(L"&");
						if (nIndex != -1)
						{
							ToolStripMenuItem^ menuitem = (ToolStripMenuItem^)item;
							wchar_t s = strText[nIndex + 1];
							if (s == key)
							{
								pSelectedItem = menuitem;
								break;
							}
							strText = strText->ToUpper();
							s = strText[nIndex + 1];
							if (s == key)
							{
								pSelectedItem = menuitem;
								break;
							}
							strText = strText->ToLower();
							s = strText[nIndex + 1];
							if (s == key)
							{
								pSelectedItem = menuitem;
								break;
							}
						}
					}
				}
			}
			if (pSelectedItem)
			{
				pSelectedItem->Select();
				pSelectedItem->ShowDropDown();
				if (pSelectedItem->DropDown->Items->Count)
					pSelectedItem->DropDown->Items[0]->Select();
				return true;
			}
		}
	}
	break;
	case WM_CHAR:
	{
		if (::GetKeyState(VK_CONTROL) < 0)
		{
			int key = (int)pMsg->wParam + 64 + (int)System::Windows::Forms::Keys::Control;//VK_CONTROL;
			Form^ m_pCurrentForm = nullptr;
			FormInfo* pInfo = nullptr;
			auto it = theAppProxy.m_mapFormInfo.find(::GetActiveWindow());
			if (it != theAppProxy.m_mapFormInfo.end())
			{
				pInfo = it->second;
				auto it2 = pInfo->m_mapShortcutItem.find(key);
				if (it2 != pInfo->m_mapShortcutItem.end())
				{
					it2->second->PerformClick();
					break;
				}
			}
			else
			{
				IntPtr handle = (IntPtr)pMsg->hwnd;
				while (handle != IntPtr::Zero)
				{
					Control^ ctl = Control::FromHandle(handle);
					if (ctl != nullptr)
					{
						if (ctl->GetType()->IsSubclassOf(Form::typeid))
						{
							m_pCurrentForm = (Form^)ctl;
							if (m_pCurrentForm->MdiParent)
								m_pCurrentForm = nullptr;
							else
								break;
						}
					}

					handle = (IntPtr)::GetAncestor((HWND)handle.ToPointer(), GA_PARENT);
				}
				if (m_pCurrentForm)
				{
					HWND hWnd = (HWND)m_pCurrentForm->Handle.ToPointer();
					auto it = theAppProxy.m_mapFormInfo.find(hWnd);
					if (it != theAppProxy.m_mapFormInfo.end())
					{
						auto it2 = it->second->m_mapShortcutItem.find(key);
						if (it2 != it->second->m_mapShortcutItem.end())
						{
							it2->second->PerformClick();
							break;
						}
					}
				}
			}
		}
		if (theAppProxy.m_pWorkingMenuHelperWnd)
		{
			int key = (int)pMsg->wParam;
			ToolStripItem^ pSelectedItem = nullptr;
			ToolStripDropDownMenu^ pToolStripDropDownMenu = theAppProxy.m_pWorkingMenuHelperWnd->m_pToolStripDropDownMenu;
			for each (ToolStripItem ^ item in pToolStripDropDownMenu->Items)
			{
				String^ strText = item->Text;
				int nIndex = strText->IndexOf(L"&");
				if (nIndex != -1)
				{
					wchar_t s = strText[nIndex + 1];
					if (s == key)
					{
						pSelectedItem = item;
						break;
					}
					strText = strText->ToUpper();
					s = strText[nIndex + 1];
					if (s == key)
					{
						pSelectedItem = item;
						break;
					}
					strText = strText->ToLower();
					s = strText[nIndex + 1];
					if (s == key)
					{
						pSelectedItem = item;
						break;
					}
				}
			}
			if (pSelectedItem)
			{
				pSelectedItem->PerformClick();
				return true;
			}
		}
	}
	break;
	}
	return false;
};

void CCosmos::OnCosmosClose()
{
	AtlTrace(_T("*************Begin CCosmos::OnClose:  ****************\n"));
	Universe::Cosmos::GetCosmos()->Fire_OnClose();
	FormCollection^ pCollection = Forms::Application::OpenForms;
	int nCount = pCollection->Count;
	while (pCollection->Count > 0) {
		Form^ pForm = pCollection[0];
		pForm->Close();
	}
	if (theApp.m_pCosmos && theApp.m_pCosmosImpl->m_pCLRProxy) {
		theApp.m_pCosmos->put_AppKeyValue(CComBSTR(L"CLRProxy"), CComVariant((LONGLONG)0));
		theApp.m_pCosmos = nullptr;
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
	Object^ pPro = (Object^)theAppProxy.m_pPropertyGrid;
	if (pPro && theAppProxy.m_pPropertyGrid->SelectedObject)
		theAppProxy.m_pPropertyGrid->SelectedObject = nullptr;
	AtlTrace(_T("*************End CCosmos::OnClose:  ****************\n"));
}

void CCosmos::OnObserverComplete(HWND hWnd, CString strUrl, IXobj* pRootXobj)
{
	Universe::Cosmos^ pManager = Universe::Cosmos::GetCosmos();
	Universe::Xobj^ _pRootNode = theAppProxy._createObject<IXobj, Universe::Xobj>(pRootXobj);
	IntPtr nHandle = (IntPtr)hWnd;
	pManager->Fire_OnObserverComplete(nHandle, BSTR2STRING(strUrl), _pRootNode);
	// Notify all descendant nodes under the root node.
	_pRootNode->Fire_RootXobjCreated(nHandle, BSTR2STRING(strUrl), _pRootNode);
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
	pGalaxyCluster->Fire_OnDocumentComplete(pGalaxyCluster, Marshal::GetObjectForIUnknown((IntPtr)pHtmlWnd), BSTR2STRING(bstrUrl));
}

void CGalaxyClusterEvent::OnIPCMsg(IGalaxy* sender, BSTR bstrType, BSTR bstrContent, BSTR bstrFeature)
{
	Object^ pObj = m_pGalaxyCluster;
	Universe::GalaxyCluster^ pGalaxyCluster = static_cast<Universe::GalaxyCluster^>(pObj);
	Galaxy^ pGalaxy = (Galaxy^)theAppProxy._createObject<IGalaxy, Galaxy>(sender);
	pGalaxyCluster->Fire_OnIPCMsg(pGalaxy, BSTR2STRING(bstrType), BSTR2STRING(bstrContent), BSTR2STRING(bstrFeature));
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
	case 0:
	{
		for (auto it = m_mapVisibleMenuHelperWnd.begin(); it != m_mapVisibleMenuHelperWnd.end(); it++)
		{
			CMenuHelperWnd* pWnd = it->second;
			if (::IsChild(hWnd, pWnd->m_hOwner) == TRUE)
			{
				bFind = true;
				break;
			}
		}
	}
	break;
	case 1:
	{
		for (auto it = m_mapVisibleMenuHelperWnd.begin(); it != m_mapVisibleMenuHelperWnd.end(); it++)
		{
			CMenuHelperWnd* pWnd = it->second;
			if (pWnd->m_hOwner == hWnd)
			{
				bFind = true;
				break;
			}
		}
	}
	break;
	}
	if (bFind == false)
	{
		if (m_mapVisibleMenuHelperWnd.size())
			lpwndpos->flags |= SWP_NOZORDER | SWP_NOREPOSITION;
		else if (m_hCreatingCLRWnd)
		{
			Control^ pCtrl = Control::FromHandle((IntPtr)m_hCreatingCLRWnd);
			if (pCtrl)
			{
				String^ name = pCtrl->GetType()->Name;
				if (name == L"ToolStripDropDownMenu")
					lpwndpos->flags |= SWP_NOZORDER | SWP_NOREPOSITION;
			}
		}
	}
	return false;
}

void CCosmosProxy::HideMenuStripPopup()
{
	while (m_mapVisibleMenuHelperWnd.size())
	{
		ToolStripDropDownMenu^ pToolStripDropDownMenu = m_mapVisibleMenuHelperWnd.begin()->second->m_pToolStripDropDownMenu;
		ToolStripMenuItem^ menuitem = (ToolStripMenuItem^)pToolStripDropDownMenu->OwnerItem;
		menuitem->HideDropDown();
	}
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

void CCosmosProxy::OnWinFormActivate(HWND hForm, int nState)
{
	switch (nState)
	{
	case 0:
	{
		Control^ pCtrl = Control::FromHandle((IntPtr)hForm);
		if (pCtrl && pCtrl->GetType()->IsSubclassOf(Form::typeid))
		{
			MenuStrip^ pMenuStrip = nullptr;
			auto it = theAppProxy.m_mapFormMenuStrip.find(hForm);
			if (it != theAppProxy.m_mapFormMenuStrip.end())
			{
				pMenuStrip = it->second;
				for each (ToolStripItem ^ item in pMenuStrip->Items)
				{
					ToolStripMenuItem^ menuitem = (ToolStripMenuItem^)item;
					menuitem->HideDropDown();
				}
			}
			//else
			//{
			//	Form^ pForm = (Form^)pCtrl;
			//	for each (Control ^ ctrl in pForm->Controls)
			//	{
			//		if (ctrl->GetType() == (MenuStrip::typeid))
			//		{
			//			pMenuStrip = (MenuStrip^)ctrl;
			//			theAppProxy.m_mapFormMenuStrip[(HWND)pForm->Handle.ToPointer()] = pMenuStrip;
			//			for each (ToolStripItem ^ item in pMenuStrip->Items)
			//			{
			//				ToolStripMenuItem^ menuitem = (ToolStripMenuItem^)item;
			//				String^ strText = menuitem->Text;
			//				int nIndex = strText->IndexOf(L"&");
			//				if (nIndex != -1)
			//				{
			//					char s = strText[nIndex + 1];
			//				}
			//				System::Windows::Forms::Keys keys = menuitem->ShortcutKeys;
			//				if(keys!= System::Windows::Forms::Keys::None)
			//				{ 
			//				}
			//			}
			//			break;
			//		}
			//	}
			//}

			//if (pMenuStrip)
			//{
			//}
		}
	}
	break;
	case 4:
	{
		Control^ pCtrl = Control::FromHandle((IntPtr)hForm);
		if (pCtrl)
		{
			String^ name = pCtrl->GetType()->Name;
			CString s = name;
			if (s == _T("ToolStripDropDownMenu"))
			{
				//theAppProxy.m_pCurInputLanguage = InputLanguage::CurrentInputLanguage;
				//String^ name = L"";
				//for each (InputLanguage ^ item in InputLanguage::InstalledInputLanguages)
				//{
				//	name = item->Culture->Name;
				//	if (name == L"en-US")
				//	{
				//		InputLanguage::CurrentInputLanguage = item;
				//		break;
				//	}
				//}
				m_hCreatingCLRWnd = nullptr;
				CMenuHelperWnd* pWnd = new CMenuHelperWnd();
				theAppProxy.m_mapMenuHelperWnd[hForm] = pWnd;
				theAppProxy.m_mapVisibleMenuHelperWnd[hForm] = pWnd;
				pWnd->SubclassWindow(hForm);
				pWnd->m_pToolStripDropDownMenu = (ToolStripDropDownMenu^)pCtrl;
				ToolStripItem^ pItem = pWnd->m_pToolStripDropDownMenu->OwnerItem;
				ToolStrip^ pToolStrip = pItem->Owner;
				while (pToolStrip == nullptr)
				{
					pItem = pItem->OwnerItem;
					if (pItem == nullptr)
						break;
					pToolStrip = pItem->Owner;
					if (pToolStrip != nullptr)
						break;
				}
				Form^ pForm = nullptr;
				if (pToolStrip)
				{
					Control^ parent = pToolStrip->Parent;
					if (parent->GetType()->IsSubclassOf(Form::typeid))
						pForm = (Form^)parent;
				}
				theAppProxy.m_pWorkingMenuHelperWnd = pWnd;
			}
		}
	}
	break;
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
		__int64 nHandle = e->Control->Handle.ToInt64();
		HWND hWnd = (HWND)((Form^)sender)->Handle.ToInt64();
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)nHandle);
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
			//BSTR bstrKey = CComBSTR("default");
			//theApp.m_pCosmos->ObserveGalaxys(thisForm->Handle.ToInt64(), CComBSTR(L""), bstrKey, CComBSTR(L""), true);
			//::SysFreeString(bstrKey);
		}
	}
}

void CCosmosProxy::OnHandleDestroyed(Object^ sender, EventArgs^ e)
{
	Form^ pForm = (Form^)sender;
	HWND hWnd = (HWND)pForm->Handle.ToInt64();
	auto it = theAppProxy.m_mapDesigningForm.find(hWnd);
	if (it != theAppProxy.m_mapDesigningForm.end())
	{
		theAppProxy.m_mapDesigningForm.erase(it);
	}
}
