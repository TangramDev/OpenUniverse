/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101230016
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

// TangramAddin.h : Declaration of the COfficeAddin
#include "..\Cosmos.h"
#include "..\CosmosCtrl.h"
#pragma once

typedef map<LONG, Office::_CustomTaskPane*> CTaskPaneMap;
typedef CTaskPaneMap::iterator TaskPaneMapIT;

namespace OfficePlus
{
	class COfficeObject;

	class COfficeDocument
	{
	public:
		COfficeDocument()
		{
			m_strDocXml = _T("");
			m_strTaskPaneXml = _T("");
			m_strTaskPaneTitle = _T("");

			m_pDocGalaxyCluster = nullptr;
			m_pGalaxy = nullptr;
			m_pTaskPaneGalaxyCluster = nullptr;
			m_pTaskPaneGalaxy = nullptr;
			m_nMsoCTPDockPosition = msoCTPDockPositionRight;
			m_nMsoCTPDockPositionRestrict = msoCTPDockPositionRestrictNone;
		}

		int								m_nWidth;
		int								m_nHeight;
		MsoCTPDockPosition				m_nMsoCTPDockPosition;
		MsoCTPDockPositionRestrict		m_nMsoCTPDockPositionRestrict;
		CString							m_strDocXml;
		CString							m_strTaskPaneXml;
		CString							m_strTaskPaneTitle;

		CGalaxy*				m_pGalaxy;
		CGalaxy*				m_pTaskPaneGalaxy;
		CGalaxyCluster*					m_pDocGalaxyCluster;
		CGalaxyCluster*					m_pTaskPaneGalaxyCluster;
		map<CString, CString>			m_mapUserFormScript;
	};

	// COfficeAddin
	class ATL_NO_VTABLE COfficeAddin :
		public CCosmos
		,public IDispatchImpl<IRibbonExtensibility, &__uuidof(IRibbonExtensibility), &LIBID_Office, /* wMajor = */ 2, /* wMinor = */ 4>,
		public IDispatchImpl<ICustomTaskPaneConsumer, &__uuidof(ICustomTaskPaneConsumer), &LIBID_Office, /* wMajor = */ 2, /* wMinor = */ 4>,
		public IDispatchImpl<AddInDesignerObjects::_IDTExtensibility2, &AddInDesignerObjects::IID__IDTExtensibility2, &AddInDesignerObjects::LIBID_AddInDesignerObjects, 1, 0>
	{
	public:
		COfficeAddin();
		virtual ~COfficeAddin();

		CString							m_strUser;
		CString							m_strRibbonXml;
		CString							m_strRibbonXmlFile;

		CComPtr<ICTPFactory>			m_pCTPFactory;
		CTaskPaneMap					m_mapTaskPaneMap;
		map<HWND, CGalaxy*>	m_mapVBAForm;
		map<HWND, COfficeObject*>		m_mapOfficeObjects;
		map<HWND, COfficeObject*>		m_mapOfficeObjects2;

		void OnCloseOfficeObj(CString strName, HWND hWnd);
		void _AddDocXml(_CustomXMLParts* pCustomXMLParts, BSTR bstrXml, BSTR bstrKey);
		BEGIN_COM_MAP(COfficeAddin)
			COM_INTERFACE_ENTRY(ICosmos)
			COM_INTERFACE_ENTRY2(IDispatch, ICosmos)
			COM_INTERFACE_ENTRY(IConnectionPointContainer)
			COM_INTERFACE_ENTRY(IRibbonExtensibility)
			COM_INTERFACE_ENTRY(ICustomTaskPaneConsumer)
			COM_INTERFACE_ENTRY(AddInDesignerObjects::IDTExtensibility2)
		END_COM_MAP()

		HRESULT HrGetResource(CString strLib, int nId, LPCTSTR lpType, LPVOID* ppvResourceData, DWORD* pdwSizeInBytes);

		virtual void SetFocus(HWND) {};
		virtual void ConnectOfficeObj(HWND hWnd) {};
		virtual void OnVbaFormInit(HWND, CGalaxy*) {};
		virtual bool OnActiveOfficeObj(HWND hWnd) { return false; };
		virtual HRESULT Tangram_OnLoad(IDispatch* RibbonControl) { return S_OK; };
		virtual HRESULT Tangram_GetItemCount(IDispatch* RibbonControl, long* nCount) { return S_OK; };
		virtual HRESULT Tangram_GetItemLabel(IDispatch* RibbonControl, long nIndex, BSTR* bstrLabel) { return S_OK; };
		virtual HRESULT Tangram_GetItemID(IDispatch* RibbonControl, long nIndex, BSTR* bstrID) { return S_OK; };
		virtual HRESULT OnConnection(IDispatch* pHostApp, int ConnectMode);
		virtual HRESULT OnDisconnection(int DisConnectMode) 
		{
			return S_OK;
		};
		virtual HRESULT OnUpdate(void) { return S_OK; };
		virtual HRESULT StartupComplete(void) { return S_OK; };
		//virtual HRESULT BeginShutdown(void) { return S_OK; };
		virtual CString GetFormXml(CString strFormName) { return _T(""); };

		virtual HRESULT CreateCosmosCtrl(void* pv, REFIID riid, LPVOID* ppv) { return S_FALSE; };
		virtual void CreateCommonDesignerToolBar();

	protected:
		CString					m_strLib;
		CComQIPtr<IRibbonUI>	m_spRibbonUI;
		CString _GetDocXmlByKey(_CustomXMLParts* pCustomXMLParts, BSTR bstrKey);
	private:
		STDMETHOD(GetCustomUI)(BSTR RibbonID, BSTR * RibbonXml);
		//IDTExtensibility2 implementation:
		STDMETHOD(OnConnection)(IDispatch * Application, AddInDesignerObjects::ext_ConnectMode ConnectMode, IDispatch *AddInInst, SAFEARRAY **custom);
		STDMETHOD(OnDisconnection)(AddInDesignerObjects::ext_DisconnectMode RemoveMode, SAFEARRAY **custom);
		STDMETHOD(OnAddInsUpdate)(SAFEARRAY **custom);
		STDMETHOD(OnStartupComplete)(SAFEARRAY **custom);
		STDMETHOD(OnBeginShutdown)(SAFEARRAY **custom);
		//IRibbonExtensibility implementation

		STDMETHOD(CTPFactoryAvailable)(ICTPFactory * CTPFactoryInst);

		CString GetDocXmlByKey(IDispatch* pDocdisp, BSTR bstrKey) { return _T(""); };
		STDMETHOD(CosmosCommand)(IDispatch* RibbonControl);
		STDMETHOD(CosmosGetImage)(BSTR strValue, IPictureDisp ** ppDispImage);
		STDMETHOD(CosmosGetVisible)(IDispatch* RibbonControl, VARIANT* varVisible);
		STDMETHOD(CosmosOnLoad)(IDispatch* RibbonControl);
		STDMETHOD(CosmosGetItemCount)(IDispatch* RibbonControl, long* nCount);
		STDMETHOD(CosmosGetItemLabel)(IDispatch* RibbonControl, long nIndex, BSTR* bstrLabel);
		STDMETHOD(CosmosGetItemID)(IDispatch* RibbonControl, long nIndex, BSTR* bstrID);
		STDMETHOD(get_RemoteHelperHWND)(LONGLONG* pVal);

		void AddDocXml(IDispatch* pDocdisp, BSTR bstrXml, BSTR bstrKey) {};
	};

	class ATL_NO_VTABLE COfficeExtender :
		public CComObjectRootEx<CComSingleThreadModel>,
		public IDispatchImpl<IOfficeExtender, &IID_IOfficeExtender, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
	{
	public:
		COfficeExtender(void);
		~COfficeExtender(void);
		COfficeAddin* m_pAddin;
		BEGIN_COM_MAP(COfficeExtender)
			COM_INTERFACE_ENTRY(IOfficeExtender)
			COM_INTERFACE_ENTRY(IDispatch)
		END_COM_MAP()
	protected:
	public:
		STDMETHOD(Close)();
		STDMETHOD(get_ActiveWorkBenchWindow)(BSTR bstrID, IWorkBenchWindow** pVal);
		STDMETHOD(InitVBAForm)(IDispatch*, long, BSTR bstrXml, IXobj** ppRetXobj);
		STDMETHOD(AddVBAFormsScript)(IDispatch* OfficeObject, BSTR bstrKey, BSTR bstrXml);
		STDMETHOD(GetGalaxyFromVBAForm)(IDispatch* pForm, IGalaxy** ppGalaxy);
		STDMETHOD(GetActiveTopXobj)(IDispatch* pForm, IXobj** WndXobj);
		STDMETHOD(GetObjectFromWnd)(LONG hWnd, IDispatch** ppObjFromWnd);
	};

	class COfficeObject :
		public CComObjectRootBase,
		public IDispatchImpl<IOfficeObject, &IID_IOfficeObject, &LIBID_Universe, 1, 0>
	{
	public:
		COfficeObject(void);
		virtual ~COfficeObject(void);

		BOOL				 m_bMDIClient;
		HWND				 m_hForm;
		HWND				 m_hClient;
		HWND				 m_hChildClient;
		HWND				 m_hTaskPane;
		HWND				 m_hTaskPaneWnd;
		HWND				 m_hTaskPaneChildWnd;
		IGalaxy*	 m_pGalaxy;
		IDispatch*			 m_pOfficeObj;
		CGalaxyCluster* m_pGalaxyCluster;

		BEGIN_COM_MAP(COfficeObject)
			COM_INTERFACE_ENTRY(IDispatch)
			COM_INTERFACE_ENTRY(IOfficeObject)
		END_COM_MAP()

		STDMETHOD(Show)(VARIANT_BOOL bShow);
		STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IXobj** ppXobj);
		STDMETHOD(UnloadCosmos)();

		void Lock() {}
		void Unlock() {}
		virtual void OnObjDestory() {};
	protected:
		ULONG InternalAddRef() { return 1; }
		ULONG InternalRelease() { return 1; }
	};
}

