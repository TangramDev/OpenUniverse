/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102250037
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

#include "excel.h"
#include "../OfficeAddin.h"
#include "ExcelPlusEvents.h"

using namespace Excel;
using namespace OfficePlus::ExcelPlus::ExcelPlusEvent;

namespace OfficePlus
{
	namespace ExcelPlus
	{
		class CExcelObject;
		class CExcelAppCtrl;
		class CExcelWorkSheet;
		class CExcelWorkBook;

		class CExcelAppObjEvents :
			public CExcelAppEvents
		{
		public:
			CExcelAppObjEvents(void) {};
			void __stdcall OnNewWorkbook(_Workbook* Wb);
			void __stdcall OnSheetSelectionChange(IDispatch* Sh, Excel::Range* Target);
			void __stdcall OnSheetBeforeDoubleClick(IDispatch* Sh, Excel::Range* Target, VARIANT_BOOL* Cancel);
			void __stdcall OnSheetBeforeRightClick(IDispatch* Sh, Excel::Range* Target, VARIANT_BOOL* Cancel);
			void __stdcall OnSheetActivate(IDispatch* Sh);
			void __stdcall OnSheetDeactivate(IDispatch* Sh);
			void __stdcall OnSheetCalculate(IDispatch* Sh);
			void __stdcall OnSheetChange(IDispatch* Sh, Excel::Range* Target);
			void __stdcall OnWorkbookOpen(_Workbook* Wb);
			void __stdcall OnWorkbookActivate(_Workbook* Wb);
			void __stdcall OnWorkbookDeactivate(_Workbook* Wb);
			void __stdcall OnWorkbookBeforeClose(_Workbook* Wb, VARIANT_BOOL* Cancel);
			void __stdcall OnWorkbookBeforeSave(_Workbook* Wb, VARIANT_BOOL SaveAsUI, VARIANT_BOOL* Cancel);
			void __stdcall OnWorkbookBeforePrint(_Workbook* Wb, VARIANT_BOOL* Cancel);
			void __stdcall OnWorkbookNewSheet(_Workbook* Wb, IDispatch* Sh);
			void __stdcall OnWorkbookAddinInstall(_Workbook* Wb);
			void __stdcall OnWorkbookAddinUninstall(_Workbook* Wb);
			void __stdcall OnWindowResize(_Workbook* Wb, Excel::Window* Wn);
			void __stdcall OnWindowActivate(_Workbook* Wb, Excel::Window* Wn);
			void __stdcall OnWindowDeactivate(_Workbook* Wb, Excel::Window* Wn);
			void __stdcall OnSheetFollowHyperlink(IDispatch* Sh, Excel::Hyperlink* Target);
			void __stdcall OnSheetPivotTableUpdate(IDispatch* Sh, PivotTable* Target);
			void __stdcall OnWorkbookPivotTableCloseConnection(_Workbook* Wb, PivotTable* Target);
			void __stdcall OnWorkbookPivotTableOpenConnection(_Workbook* Wb, PivotTable* Target);
			void __stdcall OnWorkbookSync(_Workbook* Wb, MsoSyncEventType SyncEvent);
			void __stdcall OnWorkbookBeforeXmlImport(_Workbook* Wb, XmlMap* Map, BSTR Url, VARIANT_BOOL IsRefresh, VARIANT_BOOL* Cancel);
			void __stdcall OnWorkbookAfterXmlImport(_Workbook* Wb, XmlMap* Map, VARIANT_BOOL IsRefresh, XlXmlImportResult Result);
			void __stdcall OnWorkbookBeforeXmlExport(_Workbook* Wb, XmlMap* Map, BSTR Url, VARIANT_BOOL* Cancel);
			void __stdcall OnWorkbookAfterXmlExport(_Workbook* Wb, XmlMap* Map, BSTR Url, XlXmlExportResult Result);
			void __stdcall OnWorkbookRowsetComplete(_Workbook* Wb, BSTR Description, BSTR Sheet, VARIANT_BOOL Success);
			void __stdcall OnAfterCalculate();
			void __stdcall OnSheetPivotTableAfterValueChange(IDispatch* Sh, PivotTable* TargetPivotTable, Excel::Range* TargetRange);
			void __stdcall OnSheetPivotTableBeforeAllocateChanges(IDispatch* Sh, PivotTable* TargetPivotTable, long ValueChangeStart, long ValueChangeEnd, VARIANT_BOOL* Cancel);
			void __stdcall OnSheetPivotTableBeforeCommitChanges(IDispatch* Sh, PivotTable* TargetPivotTable, long ValueChangeStart, long ValueChangeEnd, VARIANT_BOOL* Cancel);
			void __stdcall OnSheetPivotTableBeforeDiscardChanges(IDispatch* Sh, PivotTable* TargetPivotTable, long ValueChangeStart, long ValueChangeEnd);
			void __stdcall OnProtectedViewWindowOpen(Excel::ProtectedViewWindow* Pvw);
			void __stdcall OnProtectedViewWindowBeforeEdit(Excel::ProtectedViewWindow* Pvw, VARIANT_BOOL* Cancel);
			void __stdcall OnProtectedViewWindowBeforeClose(Excel::ProtectedViewWindow* Pvw, XlProtectedViewCloseReason Reason, VARIANT_BOOL* Cancel);
			void __stdcall OnProtectedViewWindowResize(Excel::ProtectedViewWindow* Pvw);
			void __stdcall OnProtectedViewWindowActivate(Excel::ProtectedViewWindow* Pvw);
			void __stdcall OnProtectedViewWindowDeactivate(Excel::ProtectedViewWindow* Pvw);
			void __stdcall OnWorkbookAfterSave(_Workbook* Wb, VARIANT_BOOL Success);
			void __stdcall OnWorkbookNewChart(_Workbook* Wb, Excel::Chart* Ch);
			void __stdcall OnSheetLensGalleryRenderComplete(IDispatch* Sh);
			void __stdcall OnSheetTableUpdate(IDispatch* Sh, TableObject* Target);
			void __stdcall OnWorkbookModelChange(_Workbook* Wb, ModelChanges* Changes);
			void __stdcall OnSheetBeforeDelete(IDispatch* Sh);
		};

		class CExcelAddin :
			public COfficeAddin,
			public map<_Workbook*, CExcelWorkBook*>
		{
		public:
			CExcelAddin();
			virtual ~CExcelAddin();

			CExcelAppCtrl*					m_pExcelAppCtrl;
			CExcelObject*					m_pActiveExcelObject;
			CComPtr<Excel::_Application>	m_pExcelApplication;

			CExcelAppObjEvents*				m_pExcelAppObjEvents;

			void AddDocXml(IDispatch* pDocdisp, BSTR bstrXml, BSTR bstrKey);
			CString GetDocXmlByKey(IDispatch* pDocdisp, BSTR bstrKey);
			STDMETHOD(put_AppKeyValue)(BSTR bstrKey, VARIANT newVal);
			STDMETHOD(AttachObjEvent)(IDispatch* pDisp, int nEventIndex);
			STDMETHOD(ExportOfficeObjXml)(IDispatch* OfficeObject, BSTR* bstrXml);
			STDMETHOD(StartApplication)(BSTR bstrAppID, BSTR bstrXml);

		private:
			bool							m_bOldVer;

			void OnOpenDoc(WPARAM);
			void SetFocus(HWND);
			void ProcessMsg(LPMSG lpMsg);
			//void OnDestroyVbaForm(HWND);
			void OnVbaFormInit(HWND, CGalaxy*);
			void OnWorkBookActivate(CExcelObject* pExcelObject);
			void UpdateOfficeObj(IDispatch* pObj, CString strXml, CString strName);
			CString GetFormXml(CString strFormName);
			//CUniverse:
			void WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd);
			void WindowDestroy(HWND hWnd);
			
			void CreateCommonDesignerToolBar();

			STDMETHOD(CosmosCommand)(IDispatch* RibbonControl);
			HRESULT OnConnection(IDispatch* pHostApp, int ConnectMode);
			HRESULT OnDisconnection(int DisConnectMode)
			{
				if (m_pExcelAppObjEvents != nullptr)
				{
					m_pExcelAppObjEvents->DispEventUnadvise(m_pExcelApplication);
				}
				return S_OK;
			};

			STDMETHOD(GetCustomUI)(BSTR RibbonID, BSTR * RibbonXml);
			//void InsertComponent(CExcelWorkBookWnd* pWnd, CString strXml);
			void ConnectOfficeObj(HWND hWnd);
			bool OnActiveOfficeObj(HWND hWnd);
			
			HRESULT CreateCosmosCtrl(void* pv, REFIID riid, LPVOID* ppv);
		};

		// CExcelAppCtrl
		class ATL_NO_VTABLE CExcelAppCtrl :
			public CCosmosAppCtrl,
			public IOleObjectImpl<CExcelAppCtrl>,
			public IPersistStorageImpl<CExcelAppCtrl>,
			public IPersistStreamInitImpl<CExcelAppCtrl>,
			public CComCoClass<CExcelAppCtrl, &CLSID_CosmosCtrl>,
			public IDispatchImpl<ICosmosAppCtrl, &IID_ICosmosAppCtrl, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
		{
		public:
			CExcelAppCtrl();

			BEGIN_COM_MAP(CExcelAppCtrl)
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
			BEGIN_MSG_MAP(OfficePlus::ExcelPlus::CExcelAppCtrl)
				MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
			END_MSG_MAP()
			LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL& );
		};

		class CExcelObject :
			public COfficeObject
		{
		public:
			CExcelObject(void);
			~CExcelObject(void);

			HWND				m_hExcelEdit;
			HWND				m_hExcelEdit2;

			CString				m_strActiveSheetName;

			CExcelWorkBook*	    m_pWorkBook;
			void ProcessMouseDownMsg(HWND);
			void OnObjDestory();
			void SheetNameChanged();
			void OnOpenDoc();
		};
	}
}


