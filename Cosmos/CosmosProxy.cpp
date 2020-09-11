/********************************************************************************
*					Open Universe - version 0.1.0								*
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

// CosmosProxy.cpp : Implementation of CCosmosNavigator

#include "stdafx.h"
#include "dllmain.h" 
#include "CosmosProxy.h"
#include "StarCLREvent.h"

#include <io.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

using namespace Cosmos;
using namespace System::Windows;
#pragma managed(push, off)
CCosmos theApp;
#pragma managed(pop)
CCosmosProxy theAppProxy;

IHubble* GetHubble()
{
	if (::GetModuleHandle(_T("universe.dll")) == nullptr)
	{
		HMODULE hModule = ::LoadLibrary(L"universe.dll");
		if (hModule == nullptr) {
			TCHAR m_szBuffer[MAX_PATH];
			if (SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer) == S_OK) {
				ATL::CString m_strProgramFilePath = ATL::CString(m_szBuffer);
				m_strProgramFilePath += _T("\\Hubble\\universe.dll");
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
			typedef CHubbleImpl* (__stdcall* GetHubbleImpl)(IHubble**);
			GetHubbleImpl _pTangramImplFunction;
			_pTangramImplFunction = (GetHubbleImpl)GetProcAddress(hModule, "GetHubbleImpl");
			if (_pTangramImplFunction != NULL) {
				theApp.m_pHubbleImpl = _pTangramImplFunction(&theApp.m_pHubble);
				theApp.m_pHubbleImpl->m_pHubbleDelegate = (IHubbleDelegate*)&theApp;
				theApp.m_pHubbleImpl->m_pUniverseAppProxy = (IHubbleAppProxy*)&theApp;
			}
		}
	}
	return theApp.m_pHubble;
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
		if (theAppProxy.m_pWorkingMenuHelperWnd == this)
			theAppProxy.m_pWorkingMenuHelperWnd = nullptr;
		auto it = theAppProxy.m_mapVisibleMenuHelperWnd.find(m_hWnd);
		if (it != theAppProxy.m_mapVisibleMenuHelperWnd.end())
			theAppProxy.m_mapVisibleMenuHelperWnd.erase(it);
	}
	else
	{
		theAppProxy.m_mapVisibleMenuHelperWnd[m_hWnd] = this;
		theAppProxy.m_pWorkingMenuHelperWnd = this;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

CCosmosProxy::CCosmosProxy() : IHubbleCLRImpl()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//TabPage|TabControl|
	//_CrtSetBreakAlloc(826);
	//m_strExtendableTypes = L"|WebBrowser|Panel|TreeView|ListView|MonthCalendar|GroupBox|FlowLayoutPanel|TableLayoutPanel|SplitContainer|";
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
		GetHubble();
		if (theApp.m_pHubble)
		{
			theApp.m_pHubbleImpl->m_pCLRProxy = this;
			theApp.m_pHubbleImpl->m_pCosmosAppProxy = &theApp;
			IHubbleExtender* pExtender = nullptr;
			theApp.m_pHubble->get_Extender(&pExtender);
		}
	}

	Cosmos::Hubble::GetHubble();
}

CCosmosProxy::~CCosmosProxy()
{
	for (auto it : m_mapQuasarInfo)
	{
		delete it.second;
	}

	if (theApp.m_bHostApp == false)
		theApp.m_pHubbleImpl->m_pCLRProxy = nullptr;

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
		auto it = theAppProxy.m_mapFormMenuStrip2.begin();
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
	Hubble::GetHubble()->Fire_OnCloudAppIdle();
	return false;
}

HWND CCosmos::InitTangramApp()
{
	return 0;
};

void CCosmos::IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2)
{
	Hubble::Fire_OnHubbleMsg((IntPtr)hWnd, BSTR2STRING(strType), BSTR2STRING(strParam1), BSTR2STRING(strParam2));
}

void CCosmos::RenderHTMLElement(HWND hWnd, CString strRuleName, CString strHTML)
{
	Hubble::Fire_OnRenderHTMLElement((IntPtr)hWnd, BSTR2STRING(strRuleName), BSTR2STRING(strHTML));
}

void CCosmos::ProcessMsg(MSG* msg) {
	if (msg)
	{
		Cosmos::Hubble::Fire_OnAppMsgLoop((IntPtr)msg->hwnd, (IntPtr)(__int32)msg->message, (IntPtr)(__int32)msg->wParam, (IntPtr)msg->lParam);
		::TranslateMessage(msg);
		::DispatchMessage(msg);
	}
};

IDispatch* CCosmosProxy::CreateWinForm(HWND hParent, BSTR strXML)
{
	auto it = m_mapChromeWebPage.find(hParent);
	if (it != m_mapChromeWebPage.end())
	{
		Form^ pForm = Cosmos::Hubble::CreateForm(it->second, BSTR2STRING(strXML));
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

void CCosmosProxy::OnDestroyChromeBrowser(IBrowser* pChromeWebBrowser)
{
	auto it = m_mapChromeWebBrowser.find(pChromeWebBrowser);
	if (it != theAppProxy.m_mapChromeWebBrowser.end())
	{
		it->second->m_pWebBrowser = nullptr;
		theAppProxy.m_mapChromeWebBrowser.erase(it);
	}
};

CTangramWPFObj* CCosmosProxy::CreateWPFControl(IStar* pNode, HWND hPWnd, UINT nID)
{
	return nullptr;
}

HRESULT CCosmosProxy::NavigateURL(IStar* pNode, CString strURL, IDispatch* dispObjforScript)
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
						::SendMessage(theApp.m_pHubbleImpl->m_hTangramWnd, WM_WINFORMCREATED, (WPARAM)hPWnd, (LPARAM)0);
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
			::PostMessage(theApp.m_pHubbleImpl->m_hHostWnd, WM_COSMOSMSG, (WPARAM)hWnd, 20200120);
		}
	}
}

void CCosmosProxy::WindowDestroy(HWND hWnd)
{
	auto it3 = m_mapQuasarInfo.find(hWnd);
	if (it3 != m_mapQuasarInfo.end())
	{
		delete it3->second;
		m_mapQuasarInfo.erase(it3);
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
	auto it7 = theAppProxy.m_mapUIData.find(hWnd);
	if (it7 != theAppProxy.m_mapUIData.end())
	{
		theAppProxy.m_mapUIData.erase(it7);
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
				GalaxyCluster^ pGalaxyCluster = Cosmos::Hubble::CreateGalaxyCluster(pForm, nullptr);
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
						QuasarInfo* pInfo = new QuasarInfo;
						pInfo->m_strNodeXml = _T("");
						pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
						pInfo->m_pDisp = nullptr;
						pInfo->m_pParentDisp = nullptr;
						theAppProxy.m_mapQuasarInfo[pInfo->m_hCtrlHandle] = pInfo;
						pInfo->m_strCtrlName = pChild->Name->ToLower();
						pInfo->m_strParentCtrlName = pChild->Name->ToLower();
						theApp.m_pHubbleImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
						::SysFreeString(strName);
					}
				}
				::SetWindowLongPtr((HWND)pChild->Handle.ToInt64(), GWLP_USERDATA, 0);
				return;
			}
			else
			{
				IStar* pNode = (IStar*)lp;
			}
		}
		BSTR bstrName = STRING2BSTR(pChild->Name); //OK!
		IGalaxyCluster* pNode = theApp.m_pHubbleImpl->Observe((HWND)pChild->Handle.ToInt64(), OLE2T(bstrName), _T("default"));
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
	Star^ pNode = Cosmos::Hubble::GetNodeFromControl(m_pCurrentForm);
	if (pNode)
	{
		Quasar^ pQuasar = pNode->Quasar;
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
							theApp.m_pHubbleImpl->ObserveCtrl(pCtrl->Handle.ToInt64(), name, strIndex);
						}
						pCtrl->Select();
					}
				}
			}
			if (pQuasar)
			{
				CTangramXmlParse m_Parse;
				if (m_Parse.LoadXml(strTag))
				{
					CString strBindName = m_Parse.attr(_T("target"), _T(""));
					if (strBindName != _T(""))
					{
						pQuasar->SendMessage(pCtrl->Name, pCtrl->Handle.ToInt64().ToString("d"), L"ControlBindMessage:MouseDoubleClick", strTag, BSTR2STRING(strBindName));
					}
				}
			}
		}
	}
}
//
//void CCosmosProxy::OnNodeMouseDoubleClick(Object^ sender, TreeNodeMouseClickEventArgs^ e)
//{
//	Control^ pCtrl = (Control^)sender;
//	Form^ m_pCurrentForm = nullptr;
//	IntPtr handle = (IntPtr)pCtrl->Handle;
//	while (handle != IntPtr::Zero)
//	{
//		Control^ ctl = Control::FromHandle(handle);
//		if (ctl != nullptr)
//		{
//			if (ctl->GetType()->IsSubclassOf(Form::typeid))
//			{
//				m_pCurrentForm = (Form^)ctl;
//				break;
//			}
//		}
//
//		handle = (IntPtr)::GetAncestor((HWND)handle.ToPointer(), GA_PARENT);
//	}
//	Star^ pNode = Cosmos::Hubble::GetNodeFromControl(m_pCurrentForm);
//	Quasar^ pQuasar = pNode->Quasar;
//	if (e->Node->Tag != nullptr)
//	{
//		String^ strTag = e->Node->Tag->ToString();
//		if (String::IsNullOrEmpty(strTag) == false)
//		{
//			if (strTag->IndexOf(L"|TangramNode|") != -1)
//			{
//				String^ strIndex = strTag->Substring(strTag->IndexOf(L":") + 1);
//				if (String::IsNullOrEmpty(strIndex) == false)
//				{
//					Control^ pTop = pCtrl->TopLevelControl;
//					Type^ pType = pTop->GetType();
//					if (pType->IsSubclassOf(Form::typeid))
//					{
//						String^ name = pType->Name + pCtrl->Name;
//						//theApp.m_pHubbleImpl->OpenCtrl(pCtrl->Handle.ToInt64(), name, strIndex);
//					}
//				}
//			}
//		}
//		if (pQuasar)
//		{
//			CTangramXmlParse m_Parse;
//			if (m_Parse.LoadXml(strTag))
//			{
//				CString strBindName = m_Parse.attr(_T("target"), _T(""));
//				if (strBindName != _T(""))
//				{
//					CString strActionName = m_Parse.attr(_T("onnodemousedoubleclick"), pCtrl->Name);
//					pQuasar->SendMessage(BSTR2STRING(strActionName), pCtrl->Handle.ToInt64().ToString("d"), L"ControlBindMessage:MouseDoubleClick", strTag, BSTR2STRING(strBindName));
//				}
//			}
//		}
//	}
//	pCtrl->Select();
//}

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
	Star^ pNode = Cosmos::Hubble::GetNodeFromControl(m_pCurrentForm);
	if (pNode == nullptr)
		return;
	Quasar^ pQuasar = pNode->Quasar;
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
						//theApp.m_pHubbleImpl->OpenCtrl(pCtrl->Handle.ToInt64(), name, strIndex);
					}
				}
			}
		}
		//if (pQuasar)
		//{
		//	CTangramXmlParse m_Parse;
		//	if (m_Parse.LoadXml(strTag))
		//	{
		//		CString strBindName = m_Parse.attr(_T("target"), _T(""));
		//		CString strActionName = m_Parse.attr(_T("onafterselect"), pCtrl->Name);

		//		if (pQuasar->m_pQuasar)
		//		{
		//			//__int64 nHandle;
		//			//pQuasar->m_pQuasar->get_HWND(&nHandle);
		//			//HWND hWnd = (HWND)nHandle;
		//			//IPCMsg msg;
		//			//msg.m_strId = _T("WinForm_TreeView_Node_OnAfterSelect");
		//			//msg.m_strParam1 = strActionName;
		//			//msg.m_strParam2 = pCtrl->Handle.ToInt64().ToString("d");
		//			//msg.m_strParam3 = _T("");
		//			//msg.m_strParam4 = strTag;
		//			//msg.m_strParam5 = strBindName;
		//			//theApp.m_pHubbleImpl->m_pCurrentIPCMsg = &msg;
		//			//::SendMessage(hWnd, WM_TANGRAMDATA, (WPARAM)&msg, 20200203);
		//		}
		//		theApp.m_pHubbleImpl->m_pCurrentIPCMsg = nullptr;
		//	}
		//}
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

Object^ CCosmosProxy::InitTangramCtrl(Form^ pForm, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse)
{
	GalaxyCluster^ pGalaxyCluster = Cosmos::Hubble::CreateGalaxyCluster(pForm, nullptr);
	if (pCtrl && pForm)
	{
		HWND hWnd = (HWND)pForm->Handle.ToPointer();
		if (m_strCurrentWinFormTemplate != _T(""))
		{
			::SendMessage(hWnd, WM_TANGRAMDATA, (WPARAM)m_strCurrentWinFormTemplate.GetBuffer(), 3);
			m_strCurrentWinFormTemplate = _T("");
		}
		else if (theApp.m_pHubbleImpl->m_strAppCurrentFormTemplatePath != _T(""))
		{
			::SendMessage(hWnd, WM_TANGRAMDATA, (WPARAM)theApp.m_pHubbleImpl->m_strAppCurrentFormTemplatePath.GetBuffer(), 3);
		}
		theApp.m_pHubbleImpl->m_strAppCurrentFormTemplatePath = _T("");
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
										theAppProxy.m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild->xml();
										CtrlInit(0, pChild, pGalaxyCluster->m_pGalaxyCluster);
									}
								}
							}
							else if (strType == L"System.Windows.Forms.ListView")
							{
								ListView^ pListView = (ListView^)pChild;
								CTangramXmlParse* _pChild = pParse->GetChild(pChild->Name);// ->GetChild(_T("uidata"));
								if (_pChild)
								{
									_pChild = _pChild->GetChild(_T("uidata"));
									if (_pChild)
									{
										pListView->ItemSelectionChanged += gcnew Forms::ListViewItemSelectionChangedEventHandler(&OnItemSelectionChanged);
										theAppProxy.m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild->xml();
										CtrlInit(1, pChild, pGalaxyCluster->m_pGalaxyCluster);
									}
								}
							}

							if (String::IsNullOrEmpty(name))
								name = strType;
							BSTR strName = STRING2BSTR(name->ToLower());
							if (name == L"mdiclient")
							{
								CTangramXmlParse* pChildParse = pParse->GetChild(L"mdiclient");
								CTangramXmlParse* pChildParse2 = nullptr;
								if (pChildParse)
									pChildParse2 = pChildParse->GetChild(_T("default"));
								QuasarInfo* pInfo = new QuasarInfo;
								pInfo->m_pDisp = nullptr;
								pInfo->m_strNodeXml = _T("");
								if (pChildParse2)
									pInfo->m_strNodeXml = pChildParse2->xml();;
								pInfo->m_pParentDisp = nullptr;
								pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
								m_mapQuasarInfo[pInfo->m_hCtrlHandle] = pInfo;
								pInfo->m_strCtrlName = name->ToLower();
								pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
								IQuasar* _pQuasar = theApp.m_pHubbleImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
								::SysFreeString(strName);

							}
							else
							{
								if (pParse)
								{
									CTangramXmlParse* pChildParse = pParse->GetChild(name->ToLower());
									CTangramXmlParse* pChildParse2 = nullptr;
									if (pChildParse)
									{
										CString strWebName = pChildParse->attr(_T("id"), _T(""));
										if (strWebName == _T(""))strWebName = pChild->Name;
										if (strWebName != _T(""))
										{
											BindWebObj* pObj = new BindWebObj;
											pObj->nType = 0;
											pObj->m_pObjDisp = (IDispatch*)Marshal::GetIUnknownForObject(pChild).ToPointer();
											pObj->m_hWnd = (HWND)pChild->Handle.ToPointer();
											pObj->m_strObjName = name;
											pObj->m_strObjType = strType;
											pObj->m_strBindObjName = strWebName;
											pObj->m_strBindData = pChildParse->attr(_T("bindevent"), _T(""));
											HWND hForm = (HWND)pForm->Handle.ToPointer();
											::PostMessage(hForm, WM_TANGRAMDATA, (WPARAM)pObj, 5);
										}
										pChildParse2 = pChildParse->GetChild(_T("default"));
									}

									if (pChildParse2)
									{
										QuasarInfo* pInfo = new QuasarInfo;
										pInfo->m_pDisp = nullptr;
										pInfo->m_strNodeXml = pChildParse2->xml();
										pInfo->m_pParentDisp = nullptr;
										pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
										m_mapQuasarInfo[pInfo->m_hCtrlHandle] = pInfo;
										pInfo->m_strCtrlName = name->ToLower();
										pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
										IQuasar* _pQuasar = theApp.m_pHubbleImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
									}
								}
								else
								{
									QuasarInfo* pInfo = new QuasarInfo;
									pInfo->m_pDisp = nullptr;
									pInfo->m_strNodeXml = _T("");
									pInfo->m_pParentDisp = nullptr;
									pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
									m_mapQuasarInfo[pInfo->m_hCtrlHandle] = pInfo;
									pInfo->m_strCtrlName = name->ToLower();
									pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
									IQuasar* _pQuasar = theApp.m_pHubbleImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
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
					InitTangramCtrl(pForm, pChild, bSave, pParse);
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
					QuasarInfo* pInfo = new QuasarInfo;
					pInfo->m_strNodeXml = _T("");
					pInfo->m_pDisp = nullptr;
					pInfo->m_pParentDisp = nullptr;
					pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
					m_mapQuasarInfo[pInfo->m_hCtrlHandle] = pInfo;
					pInfo->m_strCtrlName = pChild->Name->ToLower();
					pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
					IQuasar* _pQuasar = theApp.m_pHubbleImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
					::SysFreeString(strName);
				}
			}
			if (pType->IsSubclassOf(UserControl::typeid) == false)
				InitTangramCtrl(pForm, pChild, bSave, pParse);
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

Object^ CCosmosProxy::InitTangramNode(IStar* _pNode, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse)
{
	if (::IsChild(theApp.m_pHubbleImpl->m_hHostWnd, (HWND)pCtrl->Handle.ToInt64()))
		return nullptr;
	GalaxyCluster^ pGalaxyCluster = nullptr;
	Star^ pNode = (Star^)theAppProxy._createObject<IStar, Cosmos::Star>(_pNode);
	if (pNode)
	{
		pGalaxyCluster = pNode->GalaxyCluster;
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
					IQuasar* pQuasar = nullptr;
					_pNode->get_Quasar(&pQuasar);
					CComBSTR bstrName("");
					pQuasar->get_Name(&bstrName);
					String^ name = pNode->Name + L".";
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
							CString strWebName = pChildParse->attr(_T("id"), _T(""));
							if (strWebName == _T(""))strWebName = pChild->Name;
							if (strWebName != _T(""))
							{
								HWND hCtrl = (HWND)pChild->Handle.ToPointer();
								CString strEvents = pChildParse->attr(_T("bindevent"), _T(""));
								BindWebObj* pObj = new BindWebObj;
								pObj->nType = 0;
								pObj->m_pNode = _pNode;
								pObj->m_strBindData = strEvents;
								pObj->m_hWnd = hCtrl;
								pObj->m_strObjName = pChild->Name;
								pObj->m_strObjType = strType;
								pObj->m_strBindObjName = strWebName;
								__int64 nHandle = 0;
								_pNode->get_Handle(&nHandle);
								HWND hWnd = (HWND)nHandle;
								::PostMessage(theApp.m_pHubbleImpl->m_hTangramWnd, WM_TANGRAMDATA, (WPARAM)pObj, (LPARAM)20200204);
							}

							pChildParse2 = pChildParse->GetChild(_T("default"));
						}
						if (pChildParse2)
						{
							QuasarInfo* pInfo = new QuasarInfo;
							pInfo->m_pDisp = nullptr;
							pInfo->m_pParentDisp = _pNode;
							pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
							m_mapQuasarInfo[pInfo->m_hCtrlHandle] = pInfo;
							pInfo->m_strNodeXml = pChildParse2->xml();
							pInfo->m_strCtrlName = _strName;
							pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
							IQuasar* _pQuasar = theApp.m_pHubbleImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
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
									theAppProxy.m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild->xml();
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
								_pChild = _pChild->GetChild(_T("uidata"));
								if (_pChild)
								{
									pListView->ItemSelectionChanged += gcnew Forms::ListViewItemSelectionChangedEventHandler(&OnItemSelectionChanged);
									theAppProxy.m_mapUIData[(HWND)pChild->Handle.ToPointer()] = _pChild->xml();
									CtrlInit(1, pChild, pGalaxyCluster->m_pGalaxyCluster);
								}
							}
						}
					}
					else
					{
						QuasarInfo* pInfo = new QuasarInfo;
						pInfo->m_pDisp = nullptr;
						pInfo->m_pParentDisp = _pNode;
						pInfo->m_hCtrlHandle = (HWND)pChild->Handle.ToInt64();
						m_mapQuasarInfo[pInfo->m_hCtrlHandle] = pInfo;
						pInfo->m_strNodeXml = _T("");
						pInfo->m_strCtrlName = pChild->Name->ToLower();
						pInfo->m_strParentCtrlName = pCtrl->Name->ToLower();
						IQuasar* _pQuasar = theApp.m_pHubbleImpl->ConnectGalaxyCluster((HWND)pChild->Handle.ToInt64(), OLE2T(strName), pGalaxyCluster->m_pGalaxyCluster, pInfo);
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

					::SysFreeString(strName);
				}
			}
			InitTangramNode(_pNode, pChild, bSave, pParse);
		}
		auto it = theApp.m_pHubbleImpl->m_mapControlScript.find(_pNode);
		if (it != theApp.m_pHubbleImpl->m_mapControlScript.end())
			theApp.m_pHubbleImpl->m_mapControlScript.erase(it);
	}

	return pGalaxyCluster;
}

void CCosmosProxy::CtrlInit(int nType, Control^ ctrl, IGalaxyCluster* pGalaxyCluster)
{
	HWND hWnd = (HWND)ctrl->Handle.ToPointer();
	auto it = m_mapUIData.find(hWnd);
	if (it != m_mapUIData.end())
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
						TreeNode^ pNode = pTreeView->Nodes->Add(BSTR2STRING(m_Parse.attr(_T("text"), _T(""))));
						pNode->ImageIndex = m_Parse.attrInt(_T("imageindex"), 0);
						pNode->SelectedImageIndex = m_Parse.attrInt(_T("selectedimageindex"), 0);
						CString strTagName = ctrl->Name->ToLower() + _T("_tag");
						CTangramXmlParse* pChild = m_Parse.GetChild(strTagName);
						if (pChild)
						{
							pNode->Tag = BSTR2STRING(pChild->xml());
						}
						LoadNode(pTreeView, pNode, pGalaxyCluster, &m_Parse);
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

System::Void CCosmosProxy::LoadNode(TreeView^ pTreeView, TreeNode^ pNode, IGalaxyCluster* pGalaxyCluster, CTangramXmlParse* pParse)
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
						TreeNode^ pChildNode = pNode->Nodes->Add(BSTR2STRING(_pParse->attr(_T("text"), _T(""))));
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
	GalaxyCluster^ pGalaxyCluster = static_cast<GalaxyCluster^>(theAppProxy.InitTangramCtrl(pForm, pForm, true, pParse));
	if (pGalaxyCluster)
		pGalaxyCluster->Fire_OnGalaxyClusterLoad(pGalaxyCluster);
	Control^ ctrl = Cosmos::Hubble::GetMDIClient(pForm);
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
			theApp.m_pHubble->ObserveQuasars(pForm->Handle.ToInt64(), CComBSTR(L""), STRING2BSTR(strKey), CComBSTR(L""), true);
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
						Type^ pType = Cosmos::Hubble::GetType(BSTR2STRING(strID));
						System::Drawing::Icon^ pIcon = nullptr;
						if (pType && pType->IsSubclassOf(Form::typeid))
						{
							System::ComponentModel::ComponentResourceManager^ resources = gcnew System::ComponentModel::ComponentResourceManager(pType);
							if (resources)
							{
								pIcon = static_cast<System::Drawing::Icon^>(resources->GetObject("$this.Icon"));
								if (pIcon == nullptr)
								{
									if (Cosmos::Hubble::m_pDefaultIcon == nullptr)
									{
										Form^ _pForm = gcnew Form();
										Cosmos::Hubble::m_pDefaultIcon = _pForm->Icon;
									}
									pIcon = Cosmos::Hubble::m_pDefaultIcon;
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
							::MessageBox(NULL, strInfo, _T("Hubble"), MB_OK);
						}
					}
				}
			}
			//delete pInfo;
		}
		//_pInfo->m_mapFormsInfo.clear();
	}


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
			Object^ pObj = Cosmos::Hubble::CreateObject(BSTR2STRING(strKey));
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
				Object^ pObj = Cosmos::Hubble::CreateObject(BSTR2STRING(strKey));
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
	Cosmos::Hubble::ActiveMethod(strObjID, strMethod, pObjs);
	return S_OK;
}

HRESULT CCosmosProxy::ActiveCLRMethod(IDispatch* pCLRObj, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData)
{
	Object^ obj = Marshal::GetObjectForIUnknown((IntPtr)pCLRObj);
	String^ strMethod = BSTR2STRING(bstrMethod);
	String^ strData = BSTR2STRING(bstrData);
	cli::array<Object^, 1>^ pObjs = {};
	Cosmos::Hubble::ActiveObjectMethod(obj, strMethod, pObjs);
	return S_OK;
}

IDispatch* CCosmosProxy::CreateCLRObj(CString bstrObjID)
{
	if (bstrObjID.CompareNoCase(_T("chromert")) == 0)
	{
		theApp.InitTangramApp(false);
		return nullptr;
	}
	if (bstrObjID.Find(_T("<")) != -1)
	{
		CTangramXmlParse m_Parse;
		if (m_Parse.LoadXml(bstrObjID))
		{
			m_strCurrentWinFormTemplate = bstrObjID;
			CString strTagName = m_Parse.name();
			CWebPageImpl* pProxyBase = nullptr;
			Cosmos::Wormhole^ pCloudSession = nullptr;
			CSession* pTangramSession = nullptr;
			__int64 nHandle = m_Parse.attrInt64(_T("renderframehostproxy"), 0);
			if (nHandle)
			{
				pProxyBase = (CWebPageImpl*)nHandle;
			}
			CString strObjID = m_Parse.attr(_T("objid"), _T(""));
			if (strObjID != _T(""))
			{
				Object^ pObj = Cosmos::Hubble::CreateObject(BSTR2STRING(strObjID));

				if (pObj != nullptr)
				{
					if (pObj->GetType()->IsSubclassOf(Form::typeid))
					{
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
						WebPage^ pPage = nullptr;
						nHandle = m_Parse.attrInt64(_T("webpage"), 0);
						if (nHandle)
						{
							IWebPage* pWebPage = (IWebPage*)nHandle;
							if (pWebPage != nullptr)
							{
								pPage = gcnew WebPage(pWebPage);
								pPage->m_hWnd = (HWND)m_Parse.attrInt64(_T("webpagehandle"), 0);
							}
						}
						if (m_pCurrentPForm)
						{
							thisForm->MdiParent = m_pCurrentPForm;
							m_pCurrentPForm = nullptr;
						}

						if (strCaption != _T(""))
							thisForm->Text = BSTR2STRING(strCaption);
						if (strTagName.CompareNoCase(_T("mainwindow")) == 0)
						{
							theApp.m_pHubbleImpl->m_hMainWnd = (HWND)thisForm->Handle.ToPointer();
							::PostMessage(theApp.m_pHubbleImpl->m_hMainWnd, WM_COSMOSMSG, 0, 20200419);
						}
						thisForm->Tag = BSTR2STRING(m_Parse.name());
						__int64 nIpcSession = m_Parse.attrInt64(_T("ipcsession"), 0);
						if (nIpcSession)
						{
							pTangramSession = (CSession*)nIpcSession;
							bool bExists = Cosmos::Hubble::WebBindEventDic->TryGetValue(pObj, pCloudSession);
							if (bExists == false)
							{
								pCloudSession = gcnew Wormhole(pTangramSession);
								Cosmos::Hubble::WebBindEventDic[pObj] = pCloudSession;
								pCloudSession->m_pHostObj = pObj;
							}
							theAppProxy.m_mapSession2Wormhole[pTangramSession] = pCloudSession;
							CString strFormName = m_Parse.attr(_T("formname"), _T(""));
							if (strFormName == _T(""))
							{
								BSTR bstrName = STRING2BSTR(thisForm->Name);
								strFormName = OLE2T(bstrName);
								::SysFreeString(bstrName);
							}
							pTangramSession->InsertString(_T("formname"), strFormName);
							pTangramSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
							pTangramSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));
							pTangramSession->SendMessage();
						}
						else
						{
							pTangramSession = theApp.m_pHubbleImpl->CreateCloudSession(pProxyBase);
							pCloudSession = gcnew Wormhole(pTangramSession);
							Cosmos::Hubble::WebBindEventDic[pObj] = pCloudSession;
							pCloudSession->m_pHostObj = pObj;
							CString strFormName = m_Parse.attr(_T("formname"), _T(""));
							if (strFormName == _T(""))
							{
								BSTR bstrName = STRING2BSTR(thisForm->Name);
								strFormName = OLE2T(bstrName);
								::SysFreeString(bstrName);
							}
							pTangramSession->InsertString(_T("formname"), strFormName);
							pTangramSession->InsertLong(_T("autodelete"), 0);
							pTangramSession->Insertint64(_T("domhandle"), (__int64)pTangramSession);
							CString strFormID = m_Parse.attr(_T("id"), _T(""));
							pTangramSession->InsertString(_T("id"), strFormID);

							strFormID = m_Parse.attr(_T("objid"), _T(""));
							pTangramSession->InsertString(_T("objid"), strFormID);

							theAppProxy.m_mapSession2Wormhole[pTangramSession] = pCloudSession;
							if (thisForm->IsMdiContainer)
								pTangramSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
							else if (thisForm->MdiParent)
							{
								pTangramSession->Insertint64(_T("mdiformhandle"), thisForm->MdiParent->Handle.ToInt64());
								thisForm->Show();
								pTangramSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));
								pTangramSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
								pTangramSession->SendMessage();
								return (IDispatch*)Marshal::GetIUnknownForObject(pObj).ToPointer();
							}
							else
								pTangramSession->Insertint64(_T("formhandle"), thisForm->Handle.ToInt64());
							pTangramSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));
							pTangramSession->SendMessage();
						}

						if (pPage)
						{
							thisForm->ShowInTaskbar = false;
							int nModel = m_Parse.attrInt(_T("model"), 0);
							::PostMessage(pPage->m_hWnd, WM_COSMOSMSG, 20200213, (LPARAM)thisForm->Handle.ToPointer());
							if (nModel == 1)
							{
								thisForm->ShowDialog();
								thisForm->StartPosition = FormStartPosition::CenterScreen;
							}
							else
							{
								thisForm->Show(pPage);
							}
						}
						else
							thisForm->Show();

						CTangramXmlParse* pChildForms = m_Parse.GetChild(_T("childforms"));
						if (pChildForms)
						{
							::SendMessage((HWND)thisForm->Handle.ToPointer(), WM_COSMOSMSG, (WPARAM)pChildForms, 20190601);
						}
					}
					return (IDispatch*)Marshal::GetIUnknownForObject(pObj).ToPointer();
				}
			}
			else
			{
				if (Cosmos::Hubble::m_pMainForm)
				{
					HWND hWnd = (HWND)Cosmos::Hubble::m_pMainForm->Handle.ToPointer();
					theApp.m_pHubbleImpl->m_hMainWnd = hWnd;
					Form^ mainForm = Cosmos::Hubble::MainForm;
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
						client = Cosmos::Hubble::GetMDIClient(mainForm);
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
					pTangramSession = theApp.m_pHubbleImpl->CreateCloudSession(pProxyBase);
					pCloudSession = gcnew Wormhole(pTangramSession);
					Cosmos::Hubble::WebBindEventDic[mainForm] = pCloudSession;
					pCloudSession->m_pHostObj = mainForm;
					CString strFormName = mainForm->Name;
					pTangramSession->InsertLong(_T("autodelete"), 0);
					pTangramSession->Insertint64(_T("domhandle"), (__int64)pTangramSession);
					pTangramSession->InsertString(_T("objid"), _T("mainForm"));
					pTangramSession->InsertString(_T("formname"), strFormName);
					theAppProxy.m_mapSession2Wormhole[pTangramSession] = pCloudSession;

					CString strFormID = m_Parse.attr(_T("id"), _T(""));
					pTangramSession->InsertString(_T("id"), strFormID);

					pTangramSession->Insertint64(_T("formhandle"), mainForm->Handle.ToInt64());
					pTangramSession->InsertString(_T("msgID"), _T("WINFORM_CREATED"));

					pTangramSession->SendMessage();

					//if (client != nullptr)
					//{
					//	HWND hWnd = (HWND)client->Handle.ToPointer();
					//	if (::IsWindow(hWnd))
					//	{
					//		CTangramXmlParse* pParse = m_Parse.GetChild(_T("mainclient"));
					//		if (pParse)
					//		{
					//			CString strWebName = pParse->attr(_T("id"), _T(""));
					//			if (strWebName == _T(""))strWebName = m_Parse.name();
					//			if (strWebName != _T(""))
					//			{
					//				BindWebObj* pObj = new BindWebObj;
					//				pObj->nType = 0;
					//				pObj->m_pObjDisp = (IDispatch*)Marshal::GetIUnknownForObject(mainForm).ToPointer();
					//				pObj->m_hWnd = hWnd;
					//				pObj->m_strObjName = strWebName;
					//				pObj->m_strObjType = "clrctrl";
					//				pObj->m_strBindObjName = strWebName;
					//				//pObj->m_strBindData = pChildParse->attr(_T("bindevent"), _T(""));
					//				HWND hForm = (HWND)::GetParent(hWnd);
					//				::PostMessage(hForm, WM_TANGRAMDATA, (WPARAM)pObj, 5);
					//			}
					//			pParse = pParse->GetChild(_T("default"));
					//			if (pParse)
					//			{
					//				CString strMainForm = pParse->xml();
					//				IGalaxyCluster* pManager = nullptr;
					//				theApp.m_pHubble->CreateGalaxyCluster((__int64)::GetParent(hWnd), &pManager);
					//				if (pManager)
					//				{
					//					IQuasar* pQuasar = nullptr;
					//					pManager->CreateQuasar(CComVariant(0), CComVariant((__int64)hWnd), L"default", &pQuasar);
					//					if (pQuasar) {
					//						IStar* pNode = nullptr;
					//						pQuasar->Observe(L"default", CComBSTR(strMainForm), &pNode);
					//					}
					//				}
					//			}
					//		}
					//	}
					//}
					CTangramXmlParse* pChildForms = m_Parse.GetChild(_T("childforms"));
					if (pChildForms)
					{
						::SendMessage((HWND)mainForm->Handle.ToPointer(), WM_COSMOSMSG, (WPARAM)pChildForms, 20190601);
					}
					GalaxyCluster^ pGalaxyCluster = static_cast<GalaxyCluster^>(theAppProxy.InitTangramCtrl(mainForm, mainForm, true, &m_Parse));
				}
			}
		}
		return nullptr;
	}

	Object^ pObj = Cosmos::Hubble::CreateObject(BSTR2STRING(bstrObjID));

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
//
//void CCosmosProxy::SelectNode(IStar* pNode)
//{
//}

IDispatch* CCosmosProxy::CreateObject(BSTR bstrObjID, HWND hParent, IStar* pHostNode)
{
	String^ strID = BSTR2STRING(bstrObjID);
	Object^ _pObj = Cosmos::Hubble::CreateObject(strID);
	Star^ _pNode = (Star^)_createObject<IStar, Cosmos::Star>(pHostNode);
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
			IStar* pRootNode = NULL;
			pHostNode->get_RootStar(&pRootNode);
			CComBSTR bstrName(L"");
			pHostNode->get_Name(&bstrName);
			CString strName = OLE2T(bstrName);
			HWND hWnd = (HWND)pObj->Handle.ToInt64();
			IDispatch* pDisp = (IDispatch*)(Marshal::GetIUnknownForObject(pObj).ToInt64());
			_pNode->m_pHostObj = pObj;

			if (pObj->GetType()->IsSubclassOf(Form::typeid))
			{
				::SetParent(hWnd, (HWND)hParent);
				auto it = m_mapForm.find(hWnd);
				if (it != m_mapForm.end())
					m_mapForm.erase(it);
				theApp.m_pHubbleImpl->m_hFormNodeWnd = hWnd;
				::SetWindowLongPtr(hWnd, GWL_STYLE, ::GetWindowLongPtr(hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_BORDER | WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_CAPTION) | WS_CHILD | WS_VISIBLE);
				::SetWindowLongPtr(hWnd, GWL_EXSTYLE, ::GetWindowLongPtr(hWnd, GWL_EXSTYLE) & ~(WS_EX_APPWINDOW/*|WS_EX_CLIENTEDGE*/));
				Cosmos::Hubble::Fire_OnFormNodeCreated(BSTR2STRING(bstrObjID), (Form^)pObj, _pNode);

				((Form^)pObj)->Show();
				return pDisp;
			}
			if (theApp.m_pHubbleImpl->IsMDIClientQuasarNode(pHostNode) == false)
			{
				if (theApp.m_pHubbleImpl->m_mapControlScript.size())
				{
					auto it = theApp.m_pHubbleImpl->m_mapControlScript.find(pHostNode);
					if (it != theApp.m_pHubbleImpl->m_mapControlScript.end())
					{
						CString m_strXml = _T("");
						m_strXml = it->second;
						theApp.m_pHubbleImpl->m_mapControlScript.erase(it);
						CTangramXmlParse m_Parse;
						if (m_strXml != _T("") && m_Parse.LoadXml(m_strXml))
							InitTangramNode(pHostNode, pObj, true, &m_Parse);
					}
				}
				//m_strXml = (LPCTSTR)::SendMessage((HWND)hParent, WM_COSMOSMSG, 0, 20191223);
				//else
				//	InitTangramNode(pHostNode, pObj, true, nullptr);
			}
			return pDisp;
		}
	}
	else
	{
		if (_pObj->GetType()->IsSubclassOf(System::Windows::FrameworkElement::typeid))
		{
			Cosmos::Hubble::m_pFrameworkElementDic[_pObj] = _pNode;
			try
			{
				System::Windows::Forms::Integration::ElementHost^ pElementHost = gcnew System::Windows::Forms::Integration::ElementHost();
				pElementHost->Child = (FrameworkElement^)_pObj;
				IDispatch* pDisp = (IDispatch*)(Marshal::GetIUnknownForObject(pElementHost).ToInt64());
				_pNode->m_pHostObj = pElementHost;
				return pDisp;
			}
			catch (System::Exception ^ ex)
			{
				Debug::WriteLine(L"Hubble WPFControlWrapper Exception 1: " + ex->Message);
				if (ex->InnerException)
				{
					Debug::WriteLine(L"Hubble WPFControlWrapper Exception 1: " + ex->InnerException->Message);
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

void CCosmosProxy::ReleaseTangramObj(IDispatch* pDisp)
{
	LONGLONG nValue = (LONGLONG)pDisp;
	Object^ pObj = (Object^)Marshal::GetObjectForIUnknown((IntPtr)pDisp);
	Cosmos::Wormhole^ pCloudSession = nullptr;
	bool bExists = Cosmos::Hubble::WebBindEventDic->TryGetValue(pObj, pCloudSession);
	if (bExists == true)
	{
		if (pCloudSession != nullptr)
		{
			Cosmos::Hubble::WebBindEventDic->Remove(pObj);
		}
	}
	_removeObject(nValue);
}

HWND CCosmosProxy::GetMDIClientHandle(IDispatch* pMDICtrl)
{
	Form^ pCtrl = (Form^)Marshal::GetObjectForIUnknown((IntPtr)pMDICtrl);
	if (pCtrl != nullptr)
	{
		Control^ ctrl = Cosmos::Hubble::GetMDIClient(pCtrl);
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

int CCosmosProxy::IsSpecifiedType(IUnknown* pUnknown, BSTR bstrName)
{
	Object^ pObj = Marshal::GetObjectForIUnknown((IntPtr)pUnknown);
	if (pObj && pObj->GetType()->FullName->Equals(BSTR2STRING(bstrName)))
	{
		return 1;
	}
	return 0;
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

void CCosmosProxy::ConnectNodeToWebPage(IStar* pNode, bool bAdd)
{
	CSession* pSession = theApp.m_pHubbleImpl->GetCloudSession(pNode);
	if (pSession == nullptr)
		return;
	IDispatch* pDisp = nullptr;
	__int64 nDisp = pSession->Getint64(_T("objectdisp"));
	if (nDisp)
	{
		Object^ pObj = Marshal::GetObjectForIUnknown((IntPtr)nDisp);
		if (pObj != nullptr)
		{
			Cosmos::Wormhole^ pCloudSession = nullptr;
			bool bExists = Cosmos::Hubble::WebBindEventDic->TryGetValue(pObj, pCloudSession);
			if (bAdd)
			{
				if (bExists == false)
				{
					pCloudSession = gcnew Wormhole(pSession);
					Cosmos::Hubble::WebBindEventDic[pObj] = pCloudSession;
					pCloudSession->m_pHostObj = pObj;
					theAppProxy.m_mapSession2Wormhole[pSession] = pCloudSession;
				}
			}
			else
			{
				if (pCloudSession != nullptr)
				{
					Cosmos::Hubble::WebBindEventDic->Remove(pObj);
				}
			}
		}
	}
}

void CCosmosProxy::OnCloudMsgReceived(CSession* pSession)
{
	CString strMsgID = pSession->GetString(L"msgID");
	auto it = m_mapSession2Wormhole.find(pSession);
	if (it != m_mapSession2Wormhole.end())
	{
		Object^ pObj = nullptr;
		pObj = it->second->m_pHostObj;
		if (strMsgID == _T("MODIFY_CTRL_VALUE"))
		{
			CString strSubObj = pSession->GetString(L"currentsubobjformodify");
			if (pObj->GetType()->IsSubclassOf(Control::typeid))
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
		CString strType = pSession->GetString(L"eventtype");
		CString strCallback = pSession->GetString(L"callbackid");
		if (strCallback != _T(""))
		{
			Cosmos::Wormhole^ pCloudSession = nullptr;
			if (!Cosmos::Hubble::WebBindEventDic->TryGetValue(pObj, pCloudSession))
			{
				pCloudSession = gcnew Cosmos::Wormhole(pSession);
				Cosmos::Hubble::WebBindEventDic[pObj] = pCloudSession;
			}
			CString strEventName = pSession->GetString(LPCTSTR(strCallback));
			if (strType == _T("SyncCtrlTextChange"))
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
								Cosmos::Wormhole^ pCloudSession2 = nullptr;
								if (!Cosmos::Hubble::WebBindEventDic->TryGetValue(pSubCtrl, pCloudSession2))
								{
									Cosmos::Hubble::WebBindEventDic[pSubCtrl] = pCloudSession;
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
							Cosmos::Wormhole^ pCloudSession2 = nullptr;
							if (!Cosmos::Hubble::WebBindEventDic->TryGetValue(pSubCtrl, pCloudSession2))
							{
								Cosmos::Hubble::WebBindEventDic[pSubCtrl] = pCloudSession;
							}

							Cosmos::Hubble::Fire_OnBindCLRObjToWebPage(pSubCtrl, pCloudSession, _strEventName);
							return;
						}
					}
				}
				else
				{
					Object^ subObj = Cosmos::Hubble::Fire_OnGetSubObjForWebPage(pObj, _strSubObjName);
					if (subObj != nullptr)
					{
						Cosmos::Hubble::Fire_OnBindCLRObjToWebPage(subObj, pCloudSession, _strEventName);
						return;
					}
				}
			}
			else
			{
				if (_strEventName == _T(""))
				{
					HWND hWnd = (HWND)pSession->Getint64(L"nodehandle");
					IStar* pNode = (IStar*)pSession->Getint64(L"nodeobj");
					Star^ thisNode = theAppProxy._createObject<IStar, Star>(pNode);
					if (thisNode != nullptr)
					{
						thisNode->Fire_OnCloudMessageReceived(pCloudSession);
					}
					else
					{
						Cosmos::Hubble::Fire_OnHubbleMsgReceived(pCloudSession);
					}
				}
				else
				{
					Cosmos::Hubble::Fire_OnBindCLRObjToWebPage(pObj, pCloudSession, _strEventName);
				}
			}
		}
	}
	else
	{
		HWND hWnd = (HWND)pSession->Getint64(L"nodehandle");
		IStar* pNode = (IStar*)pSession->Getint64(L"nodeobj");
		Star^ thisNode = theAppProxy._createObject<IStar, Star>(pNode);
		Cosmos::Wormhole^ pCloudSession = nullptr;// gcnew Universe::Wormhole(pSession);
		if (thisNode!=nullptr)
		{
			if (thisNode->m_pWormhole == nullptr)
			{
				pCloudSession = gcnew Cosmos::Wormhole(pSession);
				thisNode->m_pWormhole = pCloudSession;
			}
			else
			{
				pCloudSession = thisNode->m_pWormhole;
			}
		}
		m_mapSession2Wormhole[pSession] = pCloudSession;
		Cosmos::Hubble::Fire_OnHubbleMsgReceived(pCloudSession);
	}
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
	Control^ pTextCtrl = (Control^)sender;
	Cosmos::Wormhole^ pCloudSession = nullptr;
	if (Cosmos::Hubble::WebBindEventDic->TryGetValue(sender, pCloudSession))
	{
		pCloudSession->InsertString(pTextCtrl->Name, pTextCtrl->Text);
		pCloudSession->InsertString("msgID", "FIRE_EVENT");
		pCloudSession->InsertString("currentsubobj", pTextCtrl->Name);
		String^ strEventtype = pCloudSession->GetString(L"eventtype");
		pCloudSession->InsertString("currentevent", "OnTextChanged@" + strEventtype);
		//pCloudSession->InsertString(L"callbackid", _T(""));
		//pCloudSession->InsertString(L"eventtype", _T(""));

		pCloudSession->SendMessage();
	}
}

HWND CCosmosProxy::GetCtrlHandle(IDispatch* _pCtrl)
{
	Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)_pCtrl);
	if (pCtrl != nullptr)
		return (HWND)pCtrl->Handle.ToInt64();
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

HWND CCosmosProxy::IsCtrlCanNavigate(IDispatch* ctrl)
{
	Control^ pCtrl = (Control^)Marshal::GetObjectForIUnknown((IntPtr)ctrl);
	if (pCtrl != nullptr)
	{
		if (pCtrl->Dock == DockStyle::Fill)
			return (HWND)pCtrl->Handle.ToInt64();
	}
	return 0;
}

void CCosmosProxy::HubbleAction(BSTR bstrXml, void* pvoid)
{
	CString strXml = OLE2T(bstrXml);
	if (strXml != _T(""))
	{
		if (strXml.CompareNoCase(_T("startclrapp")) == 0)
		{
			if (m_bInitApp == false)
			{
				m_bInitApp = true;
				if (Cosmos::Hubble::Fire_OnAppInit() == false)
				{
					PostQuitMessage(0);
					return;
				}
				switch (Cosmos::Hubble::AppType)
				{
				case CosmosAppType::APPWIN32:
				{
					ATLTRACE(_T("CosmosAppType::APP_WIN32\n"));
				}
				break;
				case CosmosAppType::APPBROWSER:
				case CosmosAppType::APPBROWSERAPP:
				case CosmosAppType::APPBROWSER_ECLIPSE:
				{
					theApp.m_pHubbleImpl->m_hMainWnd = NULL;
					Form^ mainForm = Cosmos::Hubble::MainForm::get();
					if (mainForm)
						theApp.m_pHubbleImpl->m_hMainWnd = (HWND)mainForm->Handle.ToPointer();
					theApp.InitTangramApp(theApp.m_pHubbleImpl->m_bIsSupportCrashReporting);
				}
				break;
				}
			}
			return;
		}
		if (strXml.CompareNoCase(_T("setmainform")) == 0)
		{
			if (Cosmos::Hubble::m_pMainForm != nullptr)
			{
				theApp.m_pHubbleImpl->m_hMainWnd = (HWND)Hubble::m_pMainForm->Handle.ToPointer();
			}
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
				Star^ pWindowNode = (Star^)theAppProxy._createObject<IStar, Star>((IStar*)pvoid);
				if (pWindowNode)
				{
					int nType = m_Parse.attrInt(_T("Type"), 0);
					switch (nType)
					{
					case 5:
						if (pWindowNode != nullptr)
						{
						}
						break;
					default:
					{
						CString strID = m_Parse.attr(_T("ObjID"), _T(""));
						CString strMethod = m_Parse.attr(_T("Method"), _T(""));
						if (strID != _T("") && strMethod != _T(""))
						{
							cli::array<Object^, 1>^ pObjs = { BSTR2STRING(strXml), pWindowNode };
							Cosmos::Hubble::ActiveMethod(BSTR2STRING(strID), BSTR2STRING(strMethod), pObjs);
						}
					}
					break;
					}
				}
			}
		}
		//else if(pvoid!=nullptr)
		//{
		//	Star^ pWindowNode = (Star^)theAppProxy._createObject<IStar, Star>((IStar*)pvoid);
		//	if (pWindowNode != nullptr)
		//	{
		//		CString strToken = _T("@IPCMessage@");
		//		int nPos = strXml.Find(strToken);
		//		if (nPos != -1)
		//		{
		//			Universe::Hubble::Fire_OnHubbleIPCMessage(pWindowNode, BSTR2STRING(strXml));
		//		}
		//		else
		//			Universe::Hubble::Fire_OnHubbleLoadDocument2Viewport(pWindowNode, BSTR2STRING(strXml));
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

void CCosmosNodeEvent::OnOpenComplete()
{
	if (m_pStarCLREvent)
		m_pStarCLREvent->OnOpenComplete(NULL);
}

void CCosmosNodeEvent::OnTabChange(int nActivePage, int nOldPage)
{
	if (m_pStar != nullptr)
		m_pStarCLREvent->OnTabChange(nActivePage, nOldPage);
}

void CCosmosNodeEvent::OnIPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra)
{
	if (m_pStar != nullptr)
		m_pStarCLREvent->OnIPCMessageReceived(bstrFrom, bstrTo, bstrMsgId, bstrPayload, bstrExtra);
}

void CCosmosNodeEvent::OnDestroy()
{
	LONGLONG nValue = (LONGLONG)m_pStar;
	theAppProxy._removeObject(nValue);
	if (m_pStarCLREvent)
	{
		m_pStarCLREvent->OnDestroy();
		delete m_pStarCLREvent;
		m_pStarCLREvent = nullptr;
	}
	this->DispEventUnadvise(m_pStar);
}

void CCosmosNodeEvent::OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl)
{
	if (m_pStar != nullptr)
		m_pStarCLREvent->OnDocumentComplete(pDocdisp, bstrUrl);
}

void CCosmosNodeEvent::OnNodeAddInCreated(IDispatch* pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml)
{
	if (m_pStar != nullptr)
		m_pStarCLREvent->OnNodeAddInCreated(pAddIndisp, bstrAddInID, bstrAddInXml);
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
			theApp.m_pHubbleImpl->m_pCLRProxy->HideMenuStripPopup();
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

void CCosmos::OnHubbleClose()
{
	AtlTrace(_T("*************Begin CCosmos::OnClose:  ****************\n"));
	Cosmos::Hubble::GetHubble()->Fire_OnClose();
	FormCollection^ pCollection = Forms::Application::OpenForms;
	int nCount = pCollection->Count;
	while (pCollection->Count > 0) {
		Form^ pForm = pCollection[0];
		pForm->Close();
	}
	if (theApp.m_pHubble && theApp.m_pHubbleImpl->m_pCLRProxy) {
		theApp.m_pHubble->put_AppKeyValue(CComBSTR(L"CLRProxy"), CComVariant((LONGLONG)0));
		theApp.m_pHubble = nullptr;
	}
	AtlTrace(_T("*************End CCosmos::OnClose:  ****************\n"));
}

void CCosmos::OnOpenComplete(HWND hWnd, CString strUrl, IStar* pRootNode)
{
	Cosmos::Hubble^ pManager = Cosmos::Hubble::GetHubble();
	Star^ _pRootNode = theAppProxy._createObject<IStar, Star>(pRootNode);
	IntPtr nHandle = (IntPtr)hWnd;
	pManager->Fire_OnOpenComplete(nHandle, BSTR2STRING(strUrl), _pRootNode);
	// Notify all descendant nodes under the root node.
	_pRootNode->Fire_RootNodeCreated(nHandle, BSTR2STRING(strUrl), _pRootNode);
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

void __stdcall  CGalaxyClusterEvent::OnTabChange(IStar* sender, int nActivePage, int nOldPage)
{
	Object^ pObj = m_pGalaxyCluster;
	Cosmos::GalaxyCluster^ pGalaxyCluster = static_cast<Cosmos::GalaxyCluster^>(pObj);
	Star^ pWindowNode = (Star^)theAppProxy._createObject<IStar, Star>(sender);
	pGalaxyCluster->Fire_OnTabChange(pWindowNode, nActivePage, nOldPage);
}

void CGalaxyClusterEvent::OnInitialize(IDispatch* pHtmlWnd, BSTR bstrUrl)
{
	Object^ pObj = m_pGalaxyCluster;
	Cosmos::GalaxyCluster^ pGalaxyCluster = static_cast<Cosmos::GalaxyCluster^>(pObj);
	pGalaxyCluster->Fire_OnDocumentComplete(pGalaxyCluster, Marshal::GetObjectForIUnknown((IntPtr)pHtmlWnd), BSTR2STRING(bstrUrl));
}

void CGalaxyClusterEvent::OnIPCMsg(IQuasar* sender, BSTR bstrType, BSTR bstrContent, BSTR bstrFeature)
{
	Object^ pObj = m_pGalaxyCluster;
	Cosmos::GalaxyCluster^ pGalaxyCluster = static_cast<Cosmos::GalaxyCluster^>(pObj);
	Quasar^ pQuasar = (Quasar^)theAppProxy._createObject<IQuasar, Quasar>(sender);
	pGalaxyCluster->Fire_OnIPCMsg(pQuasar, BSTR2STRING(bstrType), BSTR2STRING(bstrContent), BSTR2STRING(bstrFeature));
}

void CCosmosProxy::OnApplicationExit(System::Object^ sender, System::EventArgs^ e)
{
	theAppProxy.m_mapChromeWebPage.clear();
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
	System::Drawing::Icon^ icon = Cosmos::Hubble::Fire_OnGetAppIcon(nIndex);
	if (icon != nullptr)
		return (HICON)icon->Handle.ToPointer();
	else
	{
		if (Cosmos::Hubble::m_pDefaultIcon == nullptr)
		{
			Form^ _pForm = gcnew Form();
			Cosmos::Hubble::m_pDefaultIcon = _pForm->Icon;
		}
		return (HICON)Cosmos::Hubble::m_pDefaultIcon->Handle.ToPointer();
	}
}
