/********************************************************************************
*					Open Universe - version 1.1.1.21							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
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
#include "msprj.h"
#include "../OfficeAddin.h"
#include "ProjectPlusEvents.h"
#include "..\..\HubbleCtrl.h"
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
			STDMETHOD(HubbleCommand)(IDispatch* RibbonControl);
			HRESULT OnConnection(IDispatch* pHostApp, int ConnectMode);
			HRESULT OnDisconnection(int DisConnectMode);
			void WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd);
			HRESULT CreateHubbleCtrl(void* pv, REFIID riid, LPVOID* ppv);
		};

		// COutLookAppCtrl
		class ATL_NO_VTABLE CProjectAppCtrl :
			public CHubbleAppCtrl,
			public IOleObjectImpl<CProjectAppCtrl>,
			public IPersistStorageImpl<CProjectAppCtrl>,
			public IPersistStreamInitImpl<CProjectAppCtrl>,
			public CComCoClass<CProjectAppCtrl, &CLSID_HubbleCtrl>,
			public IDispatchImpl<IHubbleAppCtrl, &IID_IHubbleAppCtrl, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
		{
		public:
			CProjectAppCtrl();

			BEGIN_COM_MAP(CProjectAppCtrl)
				COM_INTERFACE_ENTRY(IHubbleAppCtrl)
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
			// IHubbleAppCtrl
			STDMETHOD(get_tag)(VARIANT* pVal);
			STDMETHOD(put_tag)(VARIANT newVal);
			STDMETHOD(get_HWND)(LONGLONG* pVal);
			STDMETHOD(get_Hubble)(IHubble** pVal);
			STDMETHOD(put_AppCtrl)(VARIANT_BOOL newVal);
		};
	}
}// namespace OfficePlus

