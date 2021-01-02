/********************************************************************************
*					Tangram Library - version 10.0.0							*
*********************************************************************************
* Copyright (C) 2002-2019 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE GPL LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangramteam.com
*
********************************************************************************/

#pragma once

#include <map>
#include "Universe.h"
#include "metahost.h"
#include "TangramCommon.h"
#include "TangramXmlParse.h"
#include <ppl.h>
#include <ppltasks.h>
#include <agents.h>
#include <shlobj.h>
#include <atlctl.h>

using namespace concurrency;
using namespace std;
using namespace ATL;

#pragma comment(lib, "imagehlp.lib")

namespace TangramCommon
{
	class CTangramAppProxy;
	class CCosmosImpl;
	class CWPFObj;

	class CWPFObj
	{
	public:
		CWPFObj()
		{
			m_pDisp = nullptr;
			m_hwndWPF = nullptr;
		};
		~CWPFObj() {};
		HWND m_hwndWPF;
		IDispatch* m_pDisp;
		map<CString, IDispatch*> m_mapWPFObj;
		virtual BOOL IsVisible() { return false; };
		virtual void InvalidateVisual() {};
		virtual void ShowVisual(BOOL bShow) {};
		virtual void Focusable(BOOL bFocus) {};
	};

	class CTangramWindow : public CWindowImpl<CTangramWindow, CWindow>
	{
	public:
		CTangramWindow();
		virtual ~CTangramWindow();

		BEGIN_MSG_MAP(CTangramWindow)
			MESSAGE_HANDLER(WM_CREATE, OnCreate)
			MESSAGE_HANDLER(WM_CLOSE, OnClose)
			MESSAGE_HANDLER(WM_COMMAND, OnCommand)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
			MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		END_MSG_MAP()
		CTangramWindow* m_pChild;
		void OnFinalMessage(HWND hWnd);
		LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
		LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
		LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
		LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
		LRESULT OnWindowPosChanging(UINT, WPARAM, LPARAM, BOOL&);
	};

	class CTangramApplicationImpl
	{
	public:
		CTangramApplicationImpl();
		virtual ~CTangramApplicationImpl();

		virtual void OnHubbleCtrlCreated(ICosmosCtrl* pICosmosCtrl) {};

		bool				m_bUsingDefaultUI;
		CTangramAppProxy*	m_pUniverseAppProxy;

		HRESULT InitHubble(BOOL bUsingDefaultUI, CTangramAppProxy* pProxy);
	};

	class CTangramCLRProxyImpl
	{
	public:
		CTangramCLRProxyImpl()
		{
			m_pProxy = NULL;
			m_strObjTypeName = _T("");
		};
		CString				m_strObjTypeName;
		CString				m_strCurrentWinFormTemplate;

		CCosmosImpl*	m_pProxy;
		virtual BSTR AttachObjEvent(IDispatch* EventObj, IDispatch* SourceObj, WindowEventType EventType, IDispatch* HTMLWindow) = 0;
		virtual HRESULT ActiveCLRMethod(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData) = 0;
		virtual HRESULT ActiveCLRMethod(IDispatch* obj, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData) = 0;
		virtual IDispatch* CreateCLRObj(CString bstrObjID) = 0;
		virtual HRESULT ProcessCtrlMsg(HWND hCtrl, bool bShiftKey) = 0;
		virtual BOOL ProcessFormMsg(HWND hFormWnd, LPMSG lpMsg, int nMouseButton) = 0;
		virtual IDispatch* CreateObject(BSTR bstrObjID, HWND hParent, IXobj* pHostNode) = 0;
		virtual int IsWinForm(HWND hWnd) = 0;
		virtual IDispatch* GetCLRControl(IDispatch* CtrlDisp, BSTR bstrNames) = 0;
		virtual BSTR GetCtrlName(IDispatch* pCtrl) = 0;
		virtual IDispatch* GetCtrlFromHandle(HWND hWnd) = 0;
		virtual HWND GetMDIClientHandle(IDispatch* pMDICtrl) = 0;
		virtual IDispatch* GetCtrlByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild) = 0;
		virtual HWND GetCtrlHandle(IDispatch* pCtrl) = 0;
		virtual BSTR GetCtrlType(IDispatch* pCtrl) = 0;
		virtual HWND IsGalaxy(IDispatch* ctrl) = 0;
		virtual void HubbleAction(BSTR bstrXml, IXobj* pGrid) = 0;
		virtual BSTR GetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild) = 0;
		virtual void SetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild, BSTR strVal) = 0;
		virtual void SelectGrid(IXobj*) { };
		virtual void SelectObj(IDispatch*) { };
		virtual void ReleaseHubbleObj(IDispatch*) { };
		virtual void AttachVSPropertyWnd(HWND) { };
		virtual void AttachCLRObjEvent(IDispatch* Sender, WindowEventType nType, HWND hNotifyWnd, VARIANT_BOOL bAttachEvent) { };
		virtual void WindowCreated(LPCTSTR strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd) {};
		virtual void WindowDestroy(HWND hWnd) {};
		virtual CWPFObj* CreateWPFControl(IXobj* pGrid, HWND hPWnd, UINT nID) { return nullptr; };
		virtual HRESULT NavigateURL(IXobj* pGrid, CString strURL, IDispatch* dispObjforScript) { return S_FALSE; };
		virtual void OnCLRHostExit() {};
		virtual void OnDestroyChromeBrowser(IBrowser*) {};
		virtual void ExportAllFormsInfo() {};
		virtual void SetObjectProperty(IDispatch* pObj, BSTR bstrPropertyName, BSTR bstrPropertyValue) {};
		virtual void* Extend(CString strKey, CString strData, CString strFeatures) = 0;
		virtual bool IsSupportDesigner() { return false; };
		virtual HICON GetAppIcon(int nIndex) = 0;
		virtual bool BindObjectToWindow(IDispatch* pDisp, HWND hWnd, CString strXml) = 0;
		virtual bool BindObjectToWindow(CString objID, CString AssemblyQualifiedName, HWND hWnd, CString strXml) = 0;
	};

	class CCosmosImpl : public Universe::CTangramBase
	{
	public:
		CCosmosImpl()
		{
			m_hHostWnd = nullptr;
			m_hFormNodeWnd = nullptr;
			m_hMainWnd = nullptr;
			m_hHubbleWnd = nullptr;
			m_pCLRProxy = nullptr;
			m_hChildHostWnd = nullptr;
			m_pJVM = nullptr;
			m_pJVMenv = nullptr;
			systemClass = nullptr;
			exitMethod = nullptr;
			loadMethod = nullptr;
			m_strAppCurrentFormTemplatePath = _T("");
		};

		HWND						m_hVSToolBoxWnd;

		CMDIChildFormInfo*			m_pCurMDIChildFormInfo;
		CTangramAppProxy*			m_pActiveAppProxy;
		CTangramAppProxy*			m_pUniverseAppProxy;
		CTangramAppProxy*			m_pCosmosAppProxy;
		CTangramCLRProxyImpl*		m_pCLRProxy;
		ITaskbarList3*				m_pTaskbarList3;

		map<CString, CTangramAppProxy*> m_mapHubbleAppProxy;

		virtual ICosmosDoc* ConnectHubbleDoc(CTangramAppProxy* AppProxy, LONGLONG docID, HWND hView, HWND hGalaxy, LPCTSTR strDocType) { return nullptr; };
	};

	class CTangramAppProxy
	{
	public:
		CTangramAppProxy();
		virtual ~CTangramAppProxy() {};


		BOOL								m_bAutoDelete;
		HWND								m_hMainWnd;
		HWND								m_hCreatingView;
		HINSTANCE							m_hInstance;
		LPCTSTR								m_strProxyName;
		LPCTSTR								m_strProxyID;
		LPCTSTR								m_strCreatingFrameTitle;
		LPCTSTR								m_strClosingFrameID;
		void*								m_pvoid;
		CCosmosDocProxy*					m_pCurDocProxy;
		CCosmosImpl*						m_pHubbleImpl;

		BOOL								m_bCreatingNewFrame;
		int									m_nFrameIndex;
		HWND								m_hClosingFrame;
		CString								m_strAppKey;
		map<CString, void*>					m_mapMainFrame;
		map<LONGLONG, ICosmosDoc*>			m_mapHubbleDoc;
		map<void*, LONG>					m_mapHubbleDocTemplateID;

		BOOL InitHubble(void* pVoid);
		BOOL HubbleSaveAllModified();
		void RegistWndRunTimeClass(void* clsInfo);

		virtual void OnActiveMainFrame(HWND);
		virtual int OnDestroyMainFrame(CString strID, int nMainFrameCount, int nWndType);
		virtual LRESULT OnForegroundIdleProc();
		virtual BOOL UniversePreTranslateMessage(MSG* pMsg);
		virtual void OnHubbleClose() {};
		virtual void OnExtendComplete(HWND hWnd, CString bstrUrl, IXobj* pRootNode) {};
		virtual void OnHubbleEvent(ICosmosEventObj* NotifyObj) {};
		virtual void RegistWndClassToHubble() {};
		virtual void OnActiveDocument(ICosmosDoc* ActiveDoc, IXobj* pGridInDoc, IXobj* pGridInCtrlBar, HWND hCtrlBar);
		virtual HWND CreateWindowObj(LPCTSTR strClsName, IXobj* pGrid, HWND hParent);
		virtual HWND CreateNewFrame(CString strFrameKey) /*{ return nullptr; }*/;
		virtual HWND GetActivePopupMenu(HWND);
		virtual HRESULT CreateTangramCtrl(void* pv, REFIID riid, LPVOID* ppv) { return S_FALSE; };
		virtual ICosmosDoc* CreateNewDocument(LPCTSTR lpszFrameID, LPCTSTR lpszAppTitle, void* pDocTemplate, BOOL bNewFrame);
		virtual ICosmosDoc* OpenDocument(void* pDocTemplate, CString strFile, BOOL bNewFrame);
		virtual CStarProxy* OnGridInit(IXobj* pNewNode) { return nullptr; };
		virtual CGalaxyProxy* OnContentLoaderCreated(IContentLoader* pNewContentLoader) { return nullptr; };
		virtual CGalaxyClusterProxy* OnContentLoaderManagerCreated(IContentLoaderManager* pNewWndPage) { return nullptr; };
		virtual void MouseMoveProxy(HWND hWnd);
		virtual void AddDoc(LONGLONG llDocID, ICosmosDoc* pDoc);
		virtual void RemoveDoc(LONGLONG llDocID);
		virtual ICosmosDoc* GetDoc(LONGLONG llDocID);
		virtual ICosmosDoc* NewDoc() { return nullptr; };
		virtual HWND InitHubbleApp() { return NULL; };

	private:
		map<CString, void*>					m_mapRuntimeClass;
	};

	#define TGM_NAME				_T("name")
	#define TGM_CAPTION				_T("caption")
	#define TGM_NODE_TYPE			_T("id")
	#define TGM_CNN_ID				_T("cnnid")
	#define TGM_HEIGHT				_T("height")
	#define TGM_WIDTH				_T("width")
	#define TGM_STYLE				_T("style")
	#define TGM_ACTIVE_PAGE			_T("activepage")
	#define TGM_TAG					_T("tag")
	#define TGM_NODE				_T("node")

	#define TGM_ROWS				_T("rows")
	#define TGM_COLS				_T("cols")

	#define TGM_SETTING_HEAD		_T("#$^&TANGRAM")
	#define TGM_SETTING_FOMRAT		_T("#$^&TANGRAM[%ld][%ld]")

	#define TGM_S_EXCEL_INPUT		1

	extern ICosmos* g_pHubble;
	extern 	CTangramApplicationImpl theAppImpl;
	extern 	Universe::CChromeBrowserCreatorImpl* g_pCreatorImpl;
};

using namespace TangramCommon;
