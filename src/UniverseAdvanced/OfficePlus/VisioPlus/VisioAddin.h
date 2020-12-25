/********************************************************************************
 *             DOM Plus for Application - Version 1.1.8.202012250001
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

#pragma once
#include "../OfficeAddin.h"
#include "..\..\CosmosCtrl.h"

namespace OfficePlus
{
	namespace VisioPlus
	{
		class CVisioAddin :
			public COfficeAddin
		{
		public:
			CVisioAddin();
			virtual ~CVisioAddin();
			//CUniverse:
			STDMETHOD(CosmosCommand)(IDispatch* RibbonControl);
			STDMETHOD(GetCustomUI)(BSTR RibbonID, BSTR * RibbonXml);
			HRESULT OnConnection(IDispatch* pHostApp, int ConnectMode);
			HRESULT OnDisconnection(int DisConnectMode);
			void WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd);
			HRESULT CreateCosmosCtrl(void* pv, REFIID riid, LPVOID* ppv);
		};

		// CVisioAppCtrl
		class ATL_NO_VTABLE CVisioAppCtrl :
			public CCosmosAppCtrl,
			public IOleObjectImpl<CVisioAppCtrl>,
			public IPersistStorageImpl<CVisioAppCtrl>,
			public IPersistStreamInitImpl<CVisioAppCtrl>,
			public CComCoClass<CVisioAppCtrl, &CLSID_CosmosCtrl>,
			public IDispatchImpl<ICosmosAppCtrl, &IID_ICosmosAppCtrl, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
		{
		public:
			CVisioAppCtrl();

			BEGIN_COM_MAP(CVisioAppCtrl)
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


