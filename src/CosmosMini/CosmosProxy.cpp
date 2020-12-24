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

// CosmosProxy.cpp : Implementation of CCosmosNavigator

#include "stdafx.h"
#include "dllmain.h" 
#include "CosmosProxy.h"
#include "GridCLREvent.h"
#include <io.h>
#include <stdio.h>

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

CCosmosProxy::CCosmosProxy() : ICosmosCLRImpl()
{
	m_strExtendableTypes = L"|Button|TextBox|WebBrowser|Panel|TreeView|ListView|MonthCalendar|GroupBox|FlowLayoutPanel|TableLayoutPanel|SplitContainer|";
	m_pCurrentPForm = nullptr;
	m_strCurrentWinFormTemplate = _T("");
	Forms::Application::EnableVisualStyles();
	m_pOnLoad = nullptr;
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
			ICosmosExtender* pExtender = nullptr;
			theApp.m_pCosmos->get_Extender(&pExtender);
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

	ATLTRACE(_T("Release CCosmosProxy :%p\n"), this);
}

void CCosmos::ProcessMsg(MSG* msg) {
	if (msg)
	{
		//Universe::Cosmos::Fire_OnAppMsgLoop((IntPtr)msg->hwnd, (IntPtr)(__int32)msg->message, (IntPtr)(__int32)msg->wParam, (IntPtr)msg->lParam);
		::TranslateMessage(msg);
		::DispatchMessage(msg);
	}
};

IDispatch* CCosmosProxy::CreateWinForm(HWND hParent, BSTR strXML)
{
	return nullptr;
}

bool CCosmosProxy::IsSupportDesigner()
{
	return true;
}

void CCosmosProxy::OnDestroyChromeBrowser(IBrowser* pBrowser)
{
};

CWPFObj* CCosmosProxy::CreateWPFControl(IXobj* pXobj, HWND hPWnd, UINT nID)
{
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
				IGalaxyCluster* pGalaxyCluster = nullptr;
				theApp.m_pCosmos->CreateGalaxyCluster(pForm->Handle.ToInt64(), &pGalaxyCluster);
				String^ strType2 = strType->Replace(L"System.Windows.Forms.", L"");
				if ((theAppProxy.m_strExtendableTypes->IndexOf(L"|" + strType2 + L"|") != -1 && pChild->Dock == DockStyle::None) || pChild->Dock == DockStyle::Fill)
				{
					bool bExtendable = (pChild->Tag == nullptr);
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
						theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster, pInfo);
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
		}
	}
}

void CCosmosProxy::InitControl(Form^ pForm, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse)
{
	IGalaxyCluster* pGalaxyCluster = nullptr;
	theApp.m_pCosmos->CreateGalaxyCluster(pForm->Handle.ToInt64(), &pGalaxyCluster);
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
			if (pChild != pActiveCtrl)
			{
				Type^ pType = pChild->GetType();
				if (pActiveCtrl == nullptr)
				{
					String^ strType = pType->FullName;
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
							String^ name = pChild->Name;
							if (strType == L"System.Windows.Forms.TreeView")
							{
								TreeView^ pTreeView = (TreeView^)pChild;
								CTangramXmlParse* _pChild = pParse->GetChild(pChild->Name);
								if (_pChild)
								{
									_pChild = _pChild->GetChild(_T("uidata"));
									if (_pChild)
									{
										theApp.m_pCosmosImpl->m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild->xml();
										CtrlInit(0, pChild, pGalaxyCluster);
									}
								}
							}
							else if (strType == L"System.Windows.Forms.ListView")
							{
								ListView^ pListView = (ListView^)pChild;
								CTangramXmlParse* _pChild = pParse->GetChild(pChild->Name);
								if (_pChild)
								{
									_pChild = _pChild->GetChild(_T("uidata"));
									if (_pChild)
									{
										pListView->ItemSelectionChanged += gcnew Forms::ListViewItemSelectionChangedEventHandler(&OnItemSelectionChanged);
										theApp.m_pCosmosImpl->m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild->xml();
										CtrlInit(1, pChild, pGalaxyCluster);
									}
								}
							}

							if (String::IsNullOrEmpty(name))
								name = strType;
							BSTR strName = STRING2BSTR(name->ToLower());
							if (name == L"mdiclient")
							{
							}
							else
							{
								if (pParse)
								{
									CTangramXmlParse* pChildParse = pParse->GetChild(name->ToLower());
									CTangramXmlParse* pChildParse2 = nullptr;
									if (pChildParse)
									{
										pChildParse2 = pChildParse->GetChild(_T("default"));
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
										IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster, pInfo);
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
									IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster, pInfo);
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
				if (pType->IsSubclassOf(UserControl::typeid) == false)
					InitControl(pForm, pChild, bSave, pParse);
			}
		}
		if (pActiveCtrl != nullptr)
		{
			Control^ pChild = pActiveCtrl;
			Type^ pType = pChild->GetType();
			String^ strType = pType->FullName;
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

					String^ name = pChild->Name;
					if (String::IsNullOrEmpty(name))
						name = strType;
					BSTR strName = STRING2BSTR(name->ToLower());//OK!
					GalaxyInfo* pInfo = new GalaxyInfo;
					pInfo->m_strXobjXml = _T("");
					pInfo->m_pDisp = nullptr;
					pInfo->m_pParentDisp = nullptr;
					pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
					m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
					pInfo->m_strCtrlName = pChild->Name->ToLower();
					pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
					IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster, pInfo);
					::SysFreeString(strName);
				}
			}
			if (pType->IsSubclassOf(UserControl::typeid) == false)
				InitControl(pForm, pChild, bSave, pParse);
		}
	}
}

void CCosmosProxy::InitXobj(IXobj* _pXobj, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse)
{
	if (::IsChild(theApp.m_pCosmosImpl->m_hHostWnd, (HWND)pCtrl->Handle.ToInt64()))
		return ;
	IGalaxyCluster* pGalaxyCluster = nullptr;
	_pXobj->get_GalaxyCluster(&pGalaxyCluster);

	Universe::Xobj^ pXobj = (Universe::Xobj^)theAppProxy._createObject<IXobj, Universe::Xobj>(_pXobj);
	if (pXobj)
	{
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
							IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster, pInfo);
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
								_pChild = _pChild->GetChild(_T("uidata"));
								if (_pChild)
								{
									theApp.m_pCosmosImpl->m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild->xml();
									CtrlInit(0, pChild, pGalaxyCluster);
								}
							}
						}
						else if (strType == L"System.Windows.Forms.ListView")
						{
							ListView^ pListView = (ListView^)pChild;
							CTangramXmlParse* _pChild = pParse->GetChild(pChild->Name);
							if (_pChild)
							{
								_pChild = _pChild->GetChild(_T("uidata"));
								if (_pChild)
								{
									pListView->ItemSelectionChanged += gcnew Forms::ListViewItemSelectionChangedEventHandler(&OnItemSelectionChanged);
									theApp.m_pCosmosImpl->m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild->xml();
									CtrlInit(1, pChild, pGalaxyCluster);
								}
							}
						}
					}
					else
					{
						GalaxyInfo* pInfo = new GalaxyInfo;
						pInfo->m_pDisp = nullptr;
						pInfo->m_pParentDisp = _pXobj;
						pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
						m_mapGalaxyInfo[pInfo->m_hCtrlHandle] = pInfo;
						pInfo->m_strXobjXml = _T("");
						pInfo->m_strCtrlName = pChild->Name->ToLower();
						pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
						IGalaxy* _pGalaxy = theApp.m_pCosmosImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster, pInfo);
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
						}
						else if (strType == L"System.Windows.Forms.ListView")
						{
							ListView^ pListView = (ListView^)pChild;
							pListView->ItemSelectionChanged += gcnew ListViewItemSelectionChangedEventHandler(&OnItemSelectionChanged);
						}
					}

					::SysFreeString(strName);
				}
			}
			InitXobj(_pXobj, pChild, bSave, pParse);
		}
	}
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
	
	theAppProxy.InitControl(pForm, pForm, true, pParse);
	theAppProxy.m_strCurrentWinFormTemplate = _T("");
	pForm->Load -= theAppProxy.m_pOnLoad;
}

void CCosmosProxy::OnCLRHostExit()
{
	Forms::Application::Exit();
}

void* CCosmosProxy::Extend(CString strKey, CString strData, CString strFeatures)
{
	return nullptr;
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
			m_strCurrentWinFormTemplate = bstrObjID;
			CString strTagName = m_Parse.name();
			CString strObjID = m_Parse.attr(_T("objid"), _T(""));
			if (strObjID != _T(""))
			{
				Object^ pObj = Universe::Cosmos::CreateObject(BSTR2STRING(strObjID));

				if (pObj != nullptr)
				{
					if (pObj->GetType()->IsSubclassOf(Form::typeid))
					{
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

						if (strCaption != _T(""))
							thisForm->Text = BSTR2STRING(strCaption);
						if (theApp.m_pCosmosImpl->m_hMainWnd == NULL && strTagName.CompareNoCase(_T("mainwindow")) == 0)
						{
							theApp.m_pCosmosImpl->m_hMainWnd = (HWND)thisForm->Handle.ToPointer();
						}
						thisForm->Tag = BSTR2STRING(m_Parse.name());
						thisForm->Show();
					}
					return (IDispatch*)Marshal::GetIUnknownForObject(pObj).ToPointer();
				}
			}
		}
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
	if (!pForm->IsMdiContainer)
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
//
//void CCosmosProxy::SelectXobj(IXobj* pXobj)
//{
//}

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
				::SetWindowLongPtr(hWnd, GWL_STYLE, ::GetWindowLongPtr(hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_BORDER | WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION) | WS_CHILD | WS_VISIBLE);
				::SetWindowLongPtr(hWnd, GWL_EXSTYLE, ::GetWindowLongPtr(hWnd, GWL_EXSTYLE) & ~(WS_EX_APPWINDOW/*|WS_EX_CLIENTEDGE*/));
				Universe::Cosmos::Fire_OnFormNodeCreated(BSTR2STRING(bstrObjID), (Form^)pObj, _pXobj);

				((Form^)pObj)->Show();
				return pDisp;
			}
			if (theApp.m_pCosmosImpl->IsMDIClientGalaxyNode(pHostNode) == false)
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
			try
			{
				System::Windows::Forms::Integration::ElementHost^ pElementHost = gcnew System::Windows::Forms::Integration::ElementHost();
				pElementHost->Child = (FrameworkElement^)_pObj;
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
	_removeObject(nValue);
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
}

void CCosmosProxy::OnCloudMsgReceived(CSession* pSession)
{
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
			return;
		}
	}
}

void CCosmosProxy::OnTextChanged(System::Object^ sender, System::EventArgs^ e)
{
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
		if (pCtrl->Dock == DockStyle::Fill|| pCtrl->Dock == DockStyle::None)
			return (HWND)pCtrl->Handle.ToInt64();
	}
	return 0;
}

void CCosmosProxy::CosmosAction(BSTR bstrXml, void* pvoid)
{
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
		}
		HWND h = pMsg->hwnd;
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
			}
		}
		break;
		case VK_UP:
		case VK_DOWN:
		{
		}
		break;
		case VK_RETURN:
		{
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
		}
	}
	break;
	case WM_CHAR:
	{
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
	AtlTrace(_T("*************End CCosmos::OnClose:  ****************\n"));
}

void CCosmos::OnObserverComplete(HWND hWnd, CString strUrl, IXobj* pRootXobj)
{
	Universe::Xobj^ _pRootNode = theAppProxy._createObject<IXobj, Universe::Xobj>(pRootXobj);
	IntPtr nHandle = (IntPtr)hWnd;
	Universe::Cosmos::GetCosmos()->Fire_OnObserverComplete(nHandle, BSTR2STRING(strUrl), _pRootNode);
	// Notify all descendant nodes under the root node.
	_pRootNode->Fire_RootXobjCreated(nHandle, BSTR2STRING(strUrl), _pRootNode);
}

void CCosmosProxy::OnApplicationExit(System::Object^ sender, System::EventArgs^ e)
{
}

bool CCosmosProxy::PreWindowPosChanging(HWND hWnd, WINDOWPOS* lpwndpos, int nType)
{
	bool bFind = false;
	switch (nType)
	{
	case 0:
	{
	}
	break;
	case 1:
	{
	}
	break;
	}
	if (bFind == false)
	{
		if (m_hCreatingCLRWnd)
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
}

void CCosmosProxy::OnWebPageCreated(HWND hWnd, CWebPageImpl* pProxy, IWebPage* pChromeWebContent, int nState)
{
}

void CCosmosProxy::OnWinFormActivate(HWND hForm, int nState)
{
	switch (nState)
	{
	case 0:
	{
	}
	break;
	case 4:
	{
		Control^ pCtrl = Control::FromHandle((IntPtr)hForm);
		if (pCtrl)
		{
		}
	}
	break;
	}
}

HICON CCosmosProxy::GetAppIcon(int nIndex)
{
	return nullptr;
}
