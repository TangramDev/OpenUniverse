/********************************************************************************
 *           Web Runtime for Application - Version Version 1.0.0.202101130008           *
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
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

#pragma once

#include "tangrambase.h"
#include "msacc.h"
#include "AccessEvents.h"
#include "../OfficeAddin.h"

using namespace MSAccess;
using namespace OfficePlus::AccessPlus::AccessPlusEvent;

namespace OfficePlus
{
	namespace AccessPlus
	{
		class CAccessAddin :
			public COfficeAddin
		{
		public:
			CAccessAddin();
			virtual ~CAccessAddin();

			CComPtr<MSAccess::_Application>			m_pApplication;
		private:
			//CUniverse:
			STDMETHOD(GetObjectFromWnd)(LONG hWnd, IDispatch** ppObjFromWnd);
			STDMETHOD(GetCustomUI)(BSTR RibbonID, BSTR * RibbonXml);
			STDMETHOD(CosmosCommand)(IDispatch* RibbonControl);
			HRESULT OnConnection(IDispatch* pHostApp, int ConnectMode);
			HRESULT OnDisconnection(int DisConnectMode);
			void WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd);
			void ConnectOfficeObj(HWND hWnd);
			void WindowDestroy(HWND hWnd);
		};

		class CAccessForm :
			public COfficeObject,
			public CMSAccessFormEvents
		{
		public:
			CAccessForm(void);
			~CAccessForm(void);
			
			void OnObjDestory();
			
			//void __stdcall Load();
			//void __stdcall Current();
			//void __stdcall BeforeInsert(short* Cancel);
			//void __stdcall AfterInsert();
			//void __stdcall BeforeUpdate(short* Cancel);
			//void __stdcall AfterUpdate();
			//void __stdcall Delete(short* Cancel);
			//void __stdcall BeforeDelConfirm(short* Cancel, short* Response);
			//void __stdcall AfterDelConfirm(short* Status);
			//void __stdcall Open(short* Cancel);
			//void __stdcall Resize();
			//void __stdcall Unload(short* Cancel);
			//void __stdcall Close();
			//void __stdcall Activate();
			//void __stdcall Deactivate();
			//void __stdcall GotFocus();
			//void __stdcall LostFocus();
		};
	}
}// namespace OfficePlus


