/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103250054
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

#pragma once
#include "msprj.h"
#include "../OfficeAddin.h"
#include "ProjectPlusEvents.h"
#include "..\..\CosmosCtrl.h"
using namespace MSProject;
using namespace OfficePlus::ProjectPlus::ProjectPlusEvent;

namespace OfficePlus
{
	namespace ProjectPlus
	{
		class CProjectAddin :
			public COfficeAddin,
			public CTangramEProjectAppEvents,
			public CTangramEProjectAppEvents2
		{
		public:
			CProjectAddin();
			virtual ~CProjectAddin();

			CComPtr<_MSProject> m_pApplication;
			//CUniverse:
			STDMETHOD(GetCustomUI)(BSTR RibbonID, BSTR * RibbonXml);
			STDMETHOD(CosmosCommand)(IDispatch* RibbonControl);
			HRESULT OnConnection(IDispatch* pHostApp, int ConnectMode);
			HRESULT OnDisconnection(int DisConnectMode);
			void WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd);
			HRESULT CreateCosmosCtrl(void* pv, REFIID riid, LPVOID* ppv);
		};

		// COutLookAppCtrl
		class ATL_NO_VTABLE CProjectAppCtrl :
			public CCosmosAppCtrl,
			public IOleObjectImpl<CProjectAppCtrl>,
			public IPersistStorageImpl<CProjectAppCtrl>,
			public IPersistStreamInitImpl<CProjectAppCtrl>,
			public CComCoClass<CProjectAppCtrl, &CLSID_CosmosCtrl>,
			public IDispatchImpl<ICosmosAppCtrl, &IID_ICosmosAppCtrl, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
		{
		public:
			CProjectAppCtrl();

			BEGIN_COM_MAP(CProjectAppCtrl)
				COM_INTERFACE_ENTRY(ICosmosAppCtrl)
				COM_INTERFACE_ENTRY(IDispatch)
				COM_INTERFACE_ENTRY(IOleObject)
				COM_INTERFACE_ENTRY(IViewObject)
				COM_INTERFACE_ENTRY(IViewObject2)
				COM_INTERFACE_ENTRY(IOleInPlaceObject)
				//COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
				COM_INTERFACE_ENTRY(IPersistStorage)
				COM_INTERFACE_ENTRY(IPersistStreamInit)
				COM_INTERFACE_ENTRY(IConnectionPointContainer)
			END_COM_MAP()

			//HRESULT FinalConstruct();
			void OnFinalMessage(HWND hWnd);

		public:
			// ICosmosAppCtrl
			STDMETHOD(get_tag)(VARIANT* pVal);
			STDMETHOD(put_tag)(VARIANT newVal);
			STDMETHOD(get_HWND)(LONGLONG* pVal);
			STDMETHOD(get_Cosmos)(ICosmos** pVal);
			STDMETHOD(put_AppCtrl)(VARIANT_BOOL newVal);
		};
	}
}// namespace OfficePlus

