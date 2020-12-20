/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE MIT LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
********************************************************************************/


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
			public IDispatchImpl<ICosmosAppCtrl, &IID_ICosmosAppCtrl, &LIBID_World, /*wMajor =*/ 1, /*wMinor =*/ 0>
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


