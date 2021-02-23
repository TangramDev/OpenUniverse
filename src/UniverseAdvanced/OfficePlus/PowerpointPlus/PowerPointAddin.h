/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102310036           *
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
#include "msppt.h"
#include "../OfficeAddin.h"
#include "PowerpointPlusEvents.h"

using namespace PowerPoint;
using namespace OfficePlus::PowerPointPlus;
using namespace OfficePlus::PowerPointPlus::PowerPointPlusEvent;

namespace OfficePlus
{
	namespace PowerPointPlus
	{
		class CCloudAddinPresentation;
		class CPowerPntObject : public COfficeObject
		{
		public:
			CPowerPntObject(void);
			~CPowerPntObject(void);

			CCloudAddinPresentation*	m_pPresentation;
			void OnObjDestory();
		};

		class CCloudAddinPresentation : 
			public COfficeDocument,
			public map<HWND, CPowerPntObject*>
		{
		public:
			CCloudAddinPresentation();
			~CCloudAddinPresentation();
			bool						m_bDesignState;
			CString						m_strKey;
		};

		class CPowerPntAddin : 
			public COfficeAddin,
			public CPowerpointPlusEApplication
		{
		public:
			CPowerPntAddin();
			CPowerPntObject*						m_pActivePowerPntObject;
			CCloudAddinPresentation*				m_pCurrentSavingPresentation;
			map<CString, CCloudAddinPresentation*>	m_mapCosmosPres;

		private:
			CComPtr<PowerPoint::_Application>		m_pApplication;

			STDMETHOD(put_AppKeyValue)(BSTR bstrKey, VARIANT newVal);
			STDMETHOD(CosmosCommand)(IDispatch* RibbonControl);
			STDMETHOD(GetCustomUI)(BSTR RibbonID, BSTR * RibbonXml);
			STDMETHOD(StartApplication)(BSTR bstrAppID, BSTR bstrXml);

			HRESULT OnConnection(IDispatch* pHostApp, int ConnectMode);
			HRESULT OnDisconnection(int DisConnectMode);

			void __stdcall OnPresentationSave(/*[in]*/ _Presentation * Pres);
			void __stdcall OnPresentationBeforeSave(/*[in]*/ _Presentation * Pres,	/*[in,out]*/ VARIANT_BOOL * Cancel);

			void AddDocXml(IDispatch* pDocdisp, BSTR bstrXml, BSTR bstrKey);
			CString GetDocXmlByKey(IDispatch* pDocdisp, BSTR bstrKey);

			//void UpdateOfficeObj(IDispatch* pObj, CString strXml, CString strName);
			void WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd);
			void WindowDestroy(HWND hWnd);
			void ConnectOfficeObj(HWND hWnd);
			bool OnActiveOfficeObj(HWND hWnd);
			HRESULT CreateCosmosCtrl(void* pv, REFIID riid, LPVOID* ppv);
		};

		// CPowerPntAppCtrl
		class ATL_NO_VTABLE CPowerPntAppCtrl :
			public CCosmosAppCtrl,
			public IOleObjectImpl<CPowerPntAppCtrl>,
			public IPersistStorageImpl<CPowerPntAppCtrl>,
			public IPersistStreamInitImpl<CPowerPntAppCtrl>,
			public CComCoClass<CPowerPntAppCtrl, &CLSID_CosmosCtrl>,
			public IDispatchImpl<ICosmosAppCtrl, &IID_ICosmosAppCtrl, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
		{
		public:
			CPowerPntAppCtrl();

			BEGIN_COM_MAP(CPowerPntAppCtrl)
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


