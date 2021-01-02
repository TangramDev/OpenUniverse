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
#include "CommonUniverse.h"
#include "TangramXmlParse.h"
#include <atlenc.h>
#ifdef _WIN64
	#ifdef _DEBUG
		#ifdef _AFXDLL
			#pragma comment (lib, "libcpprest142_2_10_64_md_d.lib")
		#else
			#pragma comment (lib, "libcpprest142_2_10_64_d.lib")
		#endif
	#else
		#ifdef _AFXDLL
			#pragma comment (lib, "libcpprest142_2_10_64_md.lib")
		#else
			#pragma comment (lib, "libcpprest142_2_10_64.lib")
		#endif
	#endif
#else
	#ifdef _DEBUG
		#ifdef _AFXDLL
			#pragma comment (lib, "libcpprest142_2_10_md_d.lib")
		#else
			#pragma comment (lib, "libcpprest142_2_10_d.lib")
		#endif
	#else
		#ifdef _AFXDLL
			#pragma comment (lib, "libcpprest142_2_10_md.lib")
		#else
			#pragma comment (lib, "libcpprest142_2_10.lib")
		#endif
	#endif
#endif

//#ifdef _WIN64
//	#ifdef _DEBUG
//		#ifdef _AFXDLL
//			#pragma comment (lib, "libcpprest142D_2_10_64_md.lib")
//		#else
//			#pragma comment (lib, "libcpprest142_2_10_64_d.lib")
//		#endif
//	#else
//		#ifdef _AFXDLL
//			#pragma comment (lib, "libcpprest142_2_10_64_md.lib")
//		#else
//			#pragma comment (lib, "libcpprest142_2_10_64.lib")
//		#endif
//	#endif
//#else
//	#ifdef _DEBUG
//		#ifdef _AFXDLL
//			#pragma comment (lib, "libcpprest142D_2_10_md.lib")
//		#else
//			#pragma comment (lib, "libcpprest142D_2_10.lib")
//		#endif
//	#else
//		#ifdef _AFXDLL
//			#pragma comment (lib, "libcpprest142_2_10_md.lib")
//		#else
//			#pragma comment (lib, "libcpprest142_2_10.lib")
//		#endif
//	#endif
//#endif

using namespace std;

#include <ppl.h>
#include <ppltasks.h>
#include <codecvt>
#include <sstream>
#include <iostream>
#include <agents.h>
#include "cpprest/filestream.h"
#include <cpprest/http_client.h>
#include <cpprest/ws_client.h>
#include <cpprest/rawptrstream.h>
#include <cpprest/http_listener.h>
#include <cpprest/containerstream.h>
#include <cpprest/producerconsumerstream.h>

using namespace pplx;
using namespace utility;
using namespace web::http;
//using namespace web::experimental::web_sockets;
using namespace web::http::client;
using namespace concurrency;
using namespace concurrency::streams;
using namespace web::http::experimental::listener;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

using namespace std;
using namespace ATL;

#pragma comment (lib, "wininet.lib")
#pragma comment (lib, "urlmon.lib")
#pragma comment (lib, "Winhttp.lib")
#pragma comment (lib, "httpapi.lib")
#pragma comment (lib, "bcrypt.lib")
#pragma comment (lib, "crypt32.lib")

#pragma comment(lib, "imagehlp.lib")

namespace TangramCommon
{
	class CCosmosImpl;
	class CTangramAppProxy;
	class CTangramTabbedPane;
	class CTangramCLRProxyImpl;

	typedef struct tagMainFrameInfo
	{
		BOOL				m_bNewFrame;
		BOOL				m_bTabIcons;
		BOOL				m_bTabCloseButton;
		BOOL				m_bTabCustomTooltips;
		BOOL				m_bAutoColor;
		BOOL				m_bDocumentMenu;
		BOOL				m_bEnableTabSwap;
		BOOL				m_bFlatFrame;
		BOOL				m_bActiveTabCloseButton;
		BOOL				m_bReuseRemovedTabGroups;
		BOOL				m_bTabbedMDI;
		CString				m_strID;
		CString				m_strTitle;
		int					m_nResID;
		int					m_nTabMDIStyle;
		int					m_tabLocation;
		int					m_nTabBorderSize;
		void*				m_pHubbleFrame;
		CTangramAppProxy*	m_pUniverseAppProxy;
	}MainFrameInfo;

	class CTangramOleMessageFilter : public COleMessageFilter
	{
	public:
		CTangramOleMessageFilter()
		{
		}

		virtual ~CTangramOleMessageFilter()
		{
		}

		// Overridables
		virtual BOOL OnMessagePending(const MSG* pMsg);
	};

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

	class CCosmosImpl : public CTangramBase
	{
	public:
		CCosmosImpl()
		{
			m_hMainWnd = nullptr;
			m_hSmallIcon = nullptr;
			m_hLargeIcon = nullptr;
			m_pCurMDIChildFormInfo = nullptr;
			m_hHostWnd = nullptr;
			m_hFormNodeWnd = nullptr;
			m_hHubbleWnd = nullptr;
			m_pCLRProxy = nullptr;
			m_hChildHostWnd = nullptr;
			m_strNtpXml = _T("");
			m_strNtpDataXml = _T("");
			m_strAppCurrentFormTemplatePath = _T("");
		};

		HWND						m_hVSToolBoxWnd;

		CMDIChildFormInfo* m_pCurMDIChildFormInfo;
		CTangramAppProxy* m_pActiveAppProxy;
		CTangramAppProxy* m_pUniverseAppProxy;
		CTangramAppProxy* m_pCosmosAppProxy;
		CTangramCLRProxyImpl* m_pCLRProxy;
		ITaskbarList3* m_pTaskbarList3;

		map<CString, CTangramAppProxy*>			m_mapHubbleAppProxy;

		virtual ICosmosDoc* ConnectHubbleDoc(CTangramAppProxy* AppProxy, LONGLONG docID, HWND hView, HWND hGalaxy, LPCTSTR strDocType) { return nullptr; };
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

		CCosmosImpl* m_pProxy;
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
		virtual BSTR GetCtrlType(IDispatch* pCtrl) { return L""; };
		virtual HWND IsGalaxy(IDispatch* ctrl) = 0;
		virtual void HubbleAction(BSTR bstrXml, IXobj* pGrid) = 0;
		virtual BSTR GetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild) = 0;
		virtual void SetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild, BSTR strVal) = 0;
		virtual void SelectGrid(IXobj*) { };
		virtual void SelectObj(IDispatch*) { };
		virtual void ReleaseHubbleObj(IDispatch*) { };
		virtual void AttachCLRObjEvent(IDispatch* Sender, WindowEventType nType, HWND hNotifyWnd, VARIANT_BOOL bAttachEvent) { };
		virtual void WindowCreated(LPCTSTR strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd) {};
		virtual void WindowDestroy(HWND hWnd) {};
		virtual CWPFObj* CreateWPFControl(IXobj* pGrid, HWND hPWnd, UINT nID) { return nullptr; };
		virtual HRESULT NavigateURL(IXobj* pGrid, CString strURL, IDispatch* dispObjforScript) { return S_FALSE; };
		virtual void OnCLRHostExit() {};
		virtual void OnDestroyChromeBrowser(IBrowser*) {};
		virtual void ExportAllFormsInfo() {};
		virtual void SetObjectProperty(IDispatch* pObj, BSTR bstrPropertyName, BSTR bstrPropertyValue) {};
		virtual void* Extend(CString strKey, CString strData, CString strFeatures) { return nullptr; };
		virtual bool IsSupportDesigner() { return false; };
		virtual HICON GetAppIcon(int nIndex) = 0;
		virtual bool BindObjectToWindow(IDispatch* pDisp, HWND hWnd, CString strXml) = 0;
		virtual bool BindObjectToWindow(CString objID, CString AssemblyQualifiedName, HWND hWnd, CString strXml) = 0;
	};

	class CTangramMFCTabCtrl : public CMFCTabCtrl
	{
		DECLARE_DYNCREATE(CTangramMFCTabCtrl)

		// Construction
	public:
		CTangramMFCTabCtrl();
		// Implementation
	public:
		virtual ~CTangramMFCTabCtrl();

	protected:
		void FireChangeActiveTab(int nNewTab);
		BOOL FireChangingActiveTab(int nNewTab);
		DECLARE_MESSAGE_MAP()
	};

	class CTangramMFCToolBar : public CMFCToolBar
	{
		DECLARE_SERIAL(CTangramMFCToolBar);
	public:
		CTangramMFCToolBar();
		virtual ~CTangramMFCToolBar();
	};

	class CCosmosDockablePane : public CDockablePane
	{
		DECLARE_SERIAL(CCosmosDockablePane);
	public:
		CCosmosDockablePane();
		virtual ~CCosmosDockablePane();
	protected:
		afx_msg LRESULT OnShowCtrlBar(WPARAM, LPARAM);
		DECLARE_MESSAGE_MAP()
		CTabbedPane* CreateTabbedPane();
		void ToggleAutoHide();
		BOOL CreateEx(DWORD dwStyleEx, LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, BOOL bHasGripper,
			UINT nID, DWORD dwStyle, DWORD dwTabbedStyle, DWORD dwControlBarStyle, CCreateContext* pContext);
	};

	class CTangramTabbedPane : public CTabbedPane
	{
		DECLARE_SERIAL(CTangramTabbedPane);
	public:
		CTangramTabbedPane();
		virtual ~CTangramTabbedPane();
	protected:
		DECLARE_MESSAGE_MAP()
		CTabbedPane* CreateTabbedPane();
		void ToggleAutoHide();
		BOOL CreateEx(DWORD dwStyleEx, LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, BOOL bHasGripper,
			UINT nID, DWORD dwStyle, DWORD dwTabbedStyle, DWORD dwControlBarStyle, CCreateContext* pContext);
	};

	class CTangramFrameWnd : public CFrameWndEx
	{
	public:
		CTangramFrameWnd();
		virtual ~CTangramFrameWnd();

		DECLARE_DYNCREATE(CTangramFrameWnd)

		int m_nFrameIndex;
		HWND m_hClient;
		HWND  m_hwndLastTopLevelFrame;
		CString	m_strRegKey;
		CDocument* m_pDoc;

		static CTangramFrameWnd* m_pActiveTangramFrameWnd;
		virtual BOOL OnShowPopupMenu(CMFCPopupMenu* /*pMenuPopup*/);
		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	protected:
		afx_msg LRESULT OnSetAppTitle(WPARAM wp, LPARAM lp);
		afx_msg LRESULT OnQueryAppProxy(WPARAM wp, LPARAM lp);
		afx_msg void OnClose();
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
		afx_msg void OnDestroy();
		afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
		afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
		DECLARE_MESSAGE_MAP()
	};

	class CTangramMainFrameWnd : public CFrameWndEx
	{
	public:
		CTangramFrameWnd* m_pHostFrame;
		CTangramMainFrameWnd();
		virtual ~CTangramMainFrameWnd();
	protected:
		virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	};

	class CTangramMainWnd : public CWnd
	{
	public:
		CTangramMainWnd();
		virtual ~CTangramMainWnd();
	protected:
		virtual void PostNcDestroy();
		DECLARE_MESSAGE_MAP()
	};

	class CTangramAppProxy
	{
	public:
		CTangramAppProxy();
		virtual ~CTangramAppProxy() 
		{
			m_pHubbleImpl = nullptr;
		};

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
		CTangramMainWnd*					m_pHubbleWnd;
		BOOL InitHubble(void* pVoid);
		void RegistWndRunTimeClass(void* clsInfo);

		virtual void OnActiveMainFrame(HWND);
		virtual int OnDestroyMainFrame(CString strID, int nMainFrameCount, int nWndType);
		virtual LRESULT OnForegroundIdleProc();
		virtual BOOL UniversePreTranslateMessage(MSG* pMsg);
		virtual void OnHubbleClose() {};
		virtual void OnObserverComplete(HWND hWnd, CString bstrUrl, IXobj* pRootNode) {};
		virtual void OnHubbleEvent(ICosmosEventObj* NotifyObj) {};
		virtual void RegistWndClassToHubble() {};
		virtual void OnActiveDocument(ICosmosDoc* ActiveDoc, IXobj* pGridInDoc, IXobj* pGridInCtrlBar, HWND hCtrlBar);
		virtual HWND CreateWindowObj(LPCTSTR strClsName, IXobj* pGrid, HWND hParent);
		virtual HWND CreateNewFrame(CString strFrameKey);
		virtual HWND GetActivePopupMenu(HWND hwnd);
		virtual HRESULT CreateTangramCtrl(void* pv, REFIID riid, LPVOID* ppv) { return S_FALSE; };
		virtual ICosmosDoc* CreateNewDocument(LPCTSTR lpszFrameID, LPCTSTR lpszAppTitle, void* pDocTemplate, BOOL bNewFrame);
		virtual ICosmosDoc* OpenDocument(void* pDocTemplate, CString strFile, BOOL bNewFrame);
		virtual CStarProxy* OnGridInit(IXobj* pNewNode) { return nullptr; };
		virtual CGalaxyProxy* OnContentLoaderCreated(IContentLoader* pNewFrame) { return nullptr; };
		virtual CGalaxyClusterProxy* OnContentLoaderManagerCreated(IContentLoaderManager* pNewContentLoaderManager) { return nullptr; };
		virtual void MouseMoveProxy(HWND hWnd);
		virtual void AddDoc(LONGLONG llDocID, ICosmosDoc* pDoc);
		virtual void RemoveDoc(LONGLONG llDocID);
		virtual ICosmosDoc* GetDoc(LONGLONG llDocID);
		virtual ICosmosDoc* NewDoc();
		virtual HWND InitHubbleApp() { return NULL; };

	private:
		map<CString, void*>					m_mapRuntimeClass;
	};

	class CCosmosDocHelper : public CCosmosDocProxy
	{
	public:
		CCosmosDocHelper();
		virtual ~CCosmosDocHelper();
		CDocument*	m_pDocument;

		CMapStringToString m_mapDataMap;

		virtual void SaveDoc()
		{
			if (m_pDocument)
			{
				m_pDocument->DoFileSave();
			}
		};
		virtual void HubbleDocEvent(ICosmosEventObj* pEventObj) {};
		virtual void TangramSerialize(CDocument* _pDoc, CArchive* pAr);
	};

	class CTangramViewHelper
	{
	public:
		CTangramViewHelper();
		virtual ~CTangramViewHelper();

		CString m_strDocID;
		ICosmosDoc* m_pDoc;

		BOOL CreateView(CCreateContext* pContext, HWND hView, HWND hParent);
	};

	extern ICosmos* g_pHubble;
	extern 	CChromeBrowserCreatorImpl* g_pCreatorImpl;
};

using namespace TangramCommon;