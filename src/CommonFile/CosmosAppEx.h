/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101070006           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 *
 * Web Runtime is a new, subversive and Internet Oriented Development Technology
 * for Windows Desktop Software. Its core idea is that, relying on the open-source
 * project of Chromium, developers are allowed to use their familiar software
 * development technology to realize the Main Process of their application system
 * as the Browser Process of Modern Web Browser. Because the Main Process is realized
 * by the developers themselves, the application system developed in this way is
 * completely different from modern web browser. This kind of application will
 * integrate the infrastructure of modern web browser as a part of the system model,
 * the application system contains not only the browser window inherent in the browser,
 * but also other desktop elements oriented to the application, such as WinForms, Java
 * and other mature software elements.
 *
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
 *
 * Built-in Full-Featured Modern Web Browser Subsystem
 *		Since the main process of the application system is essentially a Browser
 * Process, the application system can naturally create a new browser window. The
 * difference with standard browsers is that we allow the browser window to be a child
 * window of other windows. in sub-window model, we see a brand-new scene: a multi-tab
 * browser sub-window, we think this structure is more suitable for organizing specific
 * applications. Of course, the application system fully supports the creation of an
 * independent browser window as a part of the complete application system, and the browser
 * window serves as a natural extension of the ecological chain to ensure the richness
 * and diversity of the application system content.
 * DOM Plus:
 *		Because a large number of mature binary components that conform to industry
 * standards have formed a wealth of software asset accumulation, web runtime allows
 * developers to regard these existing accumulation as new DOM elements and then become
 * a part of web pages, this starting point eventually forms "DOM Plus". DOM plus is a
 * natural extension of standard DOM for desktop applications, similar to the extension
 * from "real number system" to "complex number system" in mathematical theory. Undoubtedly,
 * for the development of mathematics, from "real number system" to "complex number system"
 * is a huge leap, which can be said to be a decisive change in the development of modern
 * mathematics. We hope that the evolution from "DOM" to "DOM plus" will have a similar
 * effect, which is bound to be a huge change from desktop software to Internet desktop software.
 *		Different from earlier IE, based on the sandbox mechanism of Modern Web Browser,
 * we let these binary elements run in the Browser Process, completely isolated from the
 * Browser's Renderer Process. This makes full use of the security mechanisms of modern
 * browsers and also takes into account the accumulation of mature software component
 * technologies. Based on such considerations, existing development technologies such as
 * .NET, COM, Java, etc. can be fully applied to Internet software development.
 *
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 * In the Internet era, because the web software model has broken through the
 * limitations of local computers, this makes the software system based on
 * Internet services popular and makes desktop software appear to be dwarfed.
 * With the development of Internet technology, software content service is a
 * key topic that many software infrastructures must face, this topic further
 * highlights the dilemma of the current desktop software structure: on the one
 * hand, a large number of software component assets have been formed, on the
 * other hand, Internet services clearly represent the trend of software development.
 * Therefore, how to properly adapt the desktop software to the development of
 * the contemporary Internet is a very urgent problem and a basic problem that
 * today's desktop software must face.
 * We know that the presentation of web software needs a modern web browser, an
 * indisputable fact is that a desktop Internet web browser is also a desktop
 * software, but because its content relies on Web pages, it is almost recognized
 * as being out of the ranks of desktop software. In the age of the Internet, the
 * difference between the Desktop Software System supported by binary components and
 * the Browser Application Architecture supported by the content of Web pages has
 * become more and more prominent, and it can be said that the two are gradually
 * moving away. How Desktop Software finds its most suitable position has always been
 * an interesting topic, but there has been no proper solution. Although CEF and Electron
 * are very popular, they cannot fundamentally solve the key problem of desktop software
 * connecting to the Internet. Web Runtime tries to consider this issue from a new
 * perspective, due to the needs of Internet content security and rendering, Internet
 * giants have formed a unique Sandbox-Based Multi-Process Architecture for the basic
 * framework of the Web Browser, which makes the browser structure completely different
 * from other desktop software systems in most cases. The Sandbox-Based Multi-Process
 * Architecture indeed solves many problems caused by the Internet, and it also makes
 * the Browser series and traditional desktop software form an increasingly clear difference.
 * We believe that handling this difference is the key to solving the problem of Desktop
 * Software adapting to the Internet.
 * If we take Web Browser as the smallest Desktop Software, for example, let's imagine
 * that we use the basic structure of the Web Browser as the initial structure of other
 * desktop software, what will happen? the problem will become clearer and clearer. We
 * know that in modern browser architecture, the tasks of rendering and displaying web
 * content are completed by the Render Process and GPU Process. The responsibility of
 * the Browser Process seems to be to provide a content "viewport" for Web Content and
 * "auxiliary facilities" around the web content, such as tab strip, toolbar, etc. The
 * key to the difference between Desktop Software and Internet Web Browsers seems to be
 * here. Since Chromium, Mozilla and other such types of projects only focus on the Browser
 * Architecture itself, the restrictions that must be formed around the Browser Architecture
 * also appear in Chromium, for example, because Chromium is a Browser Project, the
 * implementation of the Browser Process is strictly limited to the C++ language level
 * due to the project itself, from this we can see that the technical constraints on the
 * browser process are one of the key reason to the huge difference between desktop software
 * and browser software, if this constraint can be broken and developers are allowed to
 * reconstruct the Browser Process while retaining the responsibility of the Browser Process,
 * the situation will be completely different. According to this thinking principle,
 * the problem will be how to use the existing Desktop Software Development Technology
 * to reconstruct the Browser Process of Web Browser Multi-Process Architecture. This
 * consideration seems to be a blind spot in the software industry today, since the vast
 * majority of software teams believe that only the Browser Architecture requires the Process
 * Separation of Browser, Renderer, GPU and others, this has led to a clear difference between
 * the software structure of the browser team and the desktop software team. In order to
 * fundamentally solve the basic problem of desktop software adapting to the Modern Internet,
 * we should unify the core structure of the Browser Infrastructure and the Desktop Software
 * Infrastructure. Recognizing this, the problem will become concise. We just need to consider,
 * what conditions do developers need to build their own browser process?
 * Thanks to Google’s open-source project Chromium, we can compile the Browser Process
 * Infrastructure of modern browsers into a set of dynamic link libraries, based on this,
 * today’s developers are provided with a concise use of their familiar technical framework
 * (such as .Net, Java) to construct their own Browser Process according to their own wishes,
 * therefore a new desktop software construction mode appears.
 *
 *******************************************************************************/

#pragma once

#include <map>
#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include "metahost.h"
#include "CommonUniverse.h"
#include "TangramXmlParse.h"
#include <ppl.h>
#include <ppltasks.h>
#include <agents.h>
#include <shlobj.h>
#include <atlctl.h>
#include <afxcview.h>

using namespace std;
using namespace ATL;
using namespace concurrency;

#pragma comment(lib, "imagehlp.lib")

namespace CommonUniverse
{
	class CCosmosImpl;
	class CTangramXDoc;
	class IUniverseAppProxy;
	class CTangramTabbedPane;

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

	// CTangramTabCtrlWnd

	class CTangramTabCtrlWnd :
		public CMFCTabCtrl,
		public ICosmosWindow
	{
		DECLARE_DYNAMIC(CTangramTabCtrlWnd)

	public:
		CTangramTabCtrlWnd();
		virtual ~CTangramTabCtrlWnd();
		virtual BOOL SetActiveTab(int iTab);

		int m_nCurSelTab;
	public:
		IXobj* m_pWndNode;

	protected:
		DECLARE_MESSAGE_MAP()
		afx_msg LRESULT OnCreatePage(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnActivePage(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnModifyPage(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnTgmSetCaption(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnActiveTangramObj(WPARAM wParam, LPARAM lParam);
		virtual void Save();
		virtual void PostNcDestroy();
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

	class CCosmosDelegate :
		public ICosmosDelegate,
		public IUniverseAppProxy,
		public ICosmosWindowProvider
	{
	public:
		CCosmosDelegate();
		virtual ~CCosmosDelegate();
		bool m_bBuiltInBrowser = false;
		bool m_bCrashReporting = false;
		BOOL IsBrowserModel(bool bCrashReporting);
		bool ProcessAppType(bool bCrashReporting);

		virtual void InitTangramInstance() {};

		//ICosmosDelegate:
		virtual bool OnAppIdle(BOOL& bIdle, LONG lCount);
		virtual bool DoIdleWork();
		virtual bool IsAppIdleMessage();
		virtual void ProcessMsg(MSG* msg);
		virtual void ForegroundIdleProc();
		virtual CString GetNTPXml();
		virtual bool OnUniversePreTranslateMessage(MSG* pMsg);
		virtual HWND GetMainWnd();
		virtual HWND QueryCanClose(HWND hWnd);
		virtual bool GetClientAreaBounds(HWND hWnd, RECT& rc) { return false; };
		virtual bool HookAppDocTemplateInfo();
		virtual bool EclipseAppInit();
		virtual void IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2);
		virtual void CustomizedDOMElement(HWND hWnd, CString strRuleName, CString strHTML);

		//IUniverseAppProxy:
		virtual void OnCosmosEvent(ICosmosEventObj* NotifyObj);
		virtual void OnObserverComplete(HWND hContentLoaderWnd, CString strUrl, IXobj* pRootNode);
		virtual CXobjProxy* OnXobjInit(IXobj* pNewNode);
		virtual CGalaxyProxy* OnGalaxyCreated(IGalaxy* pNewFrame);
		virtual CGalaxyClusterProxy* OnGalaxyClusterCreated(IGalaxyCluster* pNewContentLoaderManager);
		virtual ICosmosDoc* OpenDocument(void* pDocTemplate, CString strFile, BOOL bNewFrame);

		//ICosmosWindowProvider:
		virtual bool CosmosInit(CString strID);
		virtual CString GetNames();
		virtual CString GetTags(CString strName);
	};

	// MDI support (zero or more documents)
	class CTangramMultiDocTemplate : public CMultiDocTemplate
	{
		DECLARE_DYNAMIC(CTangramMultiDocTemplate)

		// Constructors
	public:
		CTangramMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
			CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	};

	// MDT support (zero or more documents)
	class CTangramMDTDocTemplate : public CMultiDocTemplate
	{
		DECLARE_DYNAMIC(CTangramMDTDocTemplate)

		// Constructors
	public:
		CTangramMDTDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
			CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	};

	// SDI support 
	class CCosmosDocTemplate : public CSingleDocTemplate
	{
		DECLARE_DYNAMIC(CCosmosDocTemplate)

		// Constructors
	public:
		CCosmosDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
			CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	};

	class CTangramComponentApp :
		public CWinApp,
		public ICosmosWindowProvider
	{
	public:
		CTangramComponentApp();
		virtual ~CTangramComponentApp();
		bool HubbleInit(CString strID);
		ICosmos* m_pHubble = nullptr;
		virtual int ExitInstance();
	private:
		virtual CString GetNames();
		virtual CString GetTags(CString strName);
		virtual HWND Create(HWND hParentWnd, IXobj* pGrid);
	};

	class CCosmosAppEx :
		public CWinAppEx,
		public CCosmosDelegate
	{
	public:
		CCosmosAppEx();
		virtual ~CCosmosAppEx();

		afx_msg void OnFileNew();

		virtual int Run();
		virtual bool InitApp();
		virtual BOOL InitInstance();
	private:
		virtual BOOL InitApplication();
		virtual HWND Create(HWND hParentWnd, IXobj* pGrid);
		virtual bool GetClientAreaBounds(HWND hWnd, RECT& rc);
		virtual HWND GetActivePopupMenu(HWND hWnd) ;
	};

	class CTangramFrameWndEx :
		public CFrameWndEx,
		public CGalaxyClusterProxy
	{
	public:
		CTangramFrameWndEx();
		virtual ~CTangramFrameWndEx();

		static CTangramFrameWndEx* m_pActiveTangramFrameWnd;

		DECLARE_DYNCREATE(CTangramFrameWndEx)

		virtual BOOL OnShowPopupMenu(CMFCPopupMenu* /*pMenuPopup*/);
		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	protected:
		afx_msg void OnClose();
		DECLARE_MESSAGE_MAP()

	private:
		void OnTabChange(IXobj* sender, LONG ActivePage, LONG OldPage);
		void OnClrControlCreated(IXobj* Node, IDispatch* Ctrl, CString CtrlName, HWND CtrlHandle);
		void OnEvent(IDispatch* sender, IDispatch* EventArg);
		void OnControlNotify(IXobj* sender, LONG NotifyCode, LONG CtrlID, HWND CtrlHandle, CString CtrlClassName);
		void OnHubbleEvent(ICosmosEventObj* NotifyObj);
	};

	class CTangramMDIFrameWndEx :
		public CMDIFrameWndEx,
		public CGalaxyClusterProxy
	{
	public:
		CTangramMDIFrameWndEx();
		virtual ~CTangramMDIFrameWndEx();

		CRect GetClientAreaBounds();
		DECLARE_DYNCREATE(CTangramMDIFrameWndEx)

		virtual void AdjustClientArea();

		virtual BOOL OnShowPopupMenu(CMFCPopupMenu* /*pMenuPopup*/);
		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
		virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnQueryAppProxy(WPARAM wp, LPARAM lp);

	protected:
		DECLARE_MESSAGE_MAP()

	private:
		void OnTabChange(IXobj* sender, LONG ActivePage, LONG OldPage);
		void OnClrControlCreated(IXobj* Node, IDispatch* Ctrl, CString CtrlName, HWND CtrlHandle);
		void OnEvent(IDispatch* sender, IDispatch* EventArg);
		void OnControlNotify(IXobj* sender, LONG NotifyCode, LONG CtrlID, HWND CtrlHandle, CString CtrlClassName);
		void OnHubbleEvent(ICosmosEventObj* NotifyObj);
	};

	class CTangramMDIChildWndEx :
		public CMDIChildWndEx,
		public CGalaxyClusterProxy
	{
		DECLARE_DYNCREATE(CTangramMDIChildWndEx)
	public:
		CTangramMDIChildWndEx();

		// Attributes
		CTangramXDoc* m_pDoc;
		// Implementation
	public:
		virtual ~CTangramMDIChildWndEx();

		// Generated message map functions
	protected:
		afx_msg LRESULT OnCosmosMsg(WPARAM wp, LPARAM lp);
		DECLARE_MESSAGE_MAP()

	private:
		void OnTabChange(IXobj* sender, LONG ActivePage, LONG OldPage);
		void OnClrControlCreated(IXobj* Node, IDispatch* Ctrl, CString CtrlName, HWND CtrlHandle);
		void OnEvent(IDispatch* sender, IDispatch* EventArg);
		void OnControlNotify(IXobj* sender, LONG NotifyCode, LONG CtrlID, HWND CtrlHandle, CString CtrlClassName);
		void OnHubbleEvent(ICosmosEventObj* NotifyObj);

		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	};

	class CTangramXDoc : public CDocument
	{
	protected: // create from serialization only
		CTangramXDoc();
		DECLARE_DYNCREATE(CTangramXDoc)

		// Attributes
	public:
		bool	m_bDocLoaded;
		CString m_strXmlData;
		CMapStringToString m_mapDataMap;
		ICosmosDocTemplate* m_pCosmosTemplate;

		// Overrides
	public:
		virtual BOOL OnNewDocument();
		virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
		virtual void InitializeSearchContent();
		virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

		// Implementation
	public:
		virtual ~CTangramXDoc();

		// Generated message map functions
	protected:
		DECLARE_MESSAGE_MAP()
	};
};

using namespace CommonUniverse;
extern ICosmos* g_pCosmos;
extern CCosmosBrowserFactory* g_pBrowserFactory;
