/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101010001
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser;
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

#include <map>
#include <atlstr.h>
#include <string>
#include "jniforchrome.h"
#include "universe.h"
#include <ShlObj.h>

#define TANGRAM_CONST_OPENFILE			19920612
#define TANGRAM_CONST_NEWDOC			19631222
#define TANGRAM_CONST_PANE_FIRST		20022017
#define TANGRAM_CHROME_APP_INIT			20190501

#define IPC_CLR_CONTROL_CREARED							20200220
#define IPC_CLR_CONTROL_CREARED_ID						_T("Tangram_CLR_Control_Created")
#define IPC_NODE_CREARED								20200221
#define IPC_NODE_CREARED_ID								_T("Cosmos_WndXobj_Created")
#define IPC_NODE_ONMOUSEACTIVATE						20200222
#define IPC_NODE_ONMOUSEACTIVATE_ID						_T("Tangram_WndXobj_OnMouseActivate")
#define IPC_MDIWINFORM_ACTIVEMDICHILD					20200224
#define IPC_MDIWINFORM_ACTIVEMDICHILD_ID				_T("MdiWinForm_ActiveMdiChild")

#define IPC_HUBBLE_MSG_BASE								20201202
#define IPC_HUBBLE_MSG_BindCLRObject					(IPC_HUBBLE_MSG_BASE + 1)
#define IPC_HUBBLE_MSG_FIRE_EVENT						(IPC_HUBBLE_MSG_BASE + 2)
#define IPC_HUBBLE_MSG_WINFORM_CREATED					(IPC_HUBBLE_MSG_BASE + 3)
#define IPC_HUBBLE_MSG_WINFORM_ONCLOSE					(IPC_HUBBLE_MSG_BASE + 4)
#define IPC_HUBBLE_MSG_Tangram_WndXobj_Created			(IPC_HUBBLE_MSG_BASE + 5)
#define IPC_HUBBLE_MSG_BIND_NATIVEOBJ_IPC_MSG			(IPC_HUBBLE_MSG_BASE + 6)
#define IPC_HUBBLE_MSG_MdiWinForm_ActiveMdiChild		(IPC_HUBBLE_MSG_BASE + 7)
#define IPC_HUBBLE_MSG_MdiWinForm_Ready					(IPC_HUBBLE_MSG_BASE + 8)
#define IPC_HUBBLE_MSG_MDIFORM_ALLMDICHILDREMOVED		(IPC_HUBBLE_MSG_BASE + 9)
#define IPC_HUBBLE_MSG_OPEN_XML_SPLITTER				(IPC_HUBBLE_MSG_BASE + 10)

#define APP_WIN32				500
#define APP_BROWSER				1000
#define APP_ECLIPSE				2000
#define APP_BROWSER_ECLIPSE		3000
#define APP_BROWSERAPP			4000

#define APP_WILLCLOSE			2019
#define APP_OTHER				0
#define APP_SDI					1965
#define APP_MDI					1992
#define APP_MDT					1963

#define WM_OPENDOCUMENT					(WM_USER + 0x00004002)
#define WM_SPLITTERREPOSITION			(WM_USER + 0x00004003)
#define WM_ECLIPSEWORKBENCHCREATED		(WM_USER + 0x00004004)
#define WM_TABCHANGE					(WM_USER + 0x00004005)
#define WM_COSMOSMSG					(WM_USER + 0x00004006)
#define WM_OFFICEOBJECTCREATED			(WM_USER + 0x00004008)
#define WM_MDICHILDMIN					(WM_USER + 0x00004009)
#define WM_HUBBLE_APPINIT				(WM_USER + 0x0000400a)
#define WM_HUBBLE_APPQUIT				(WM_USER + 0x0000400b)
#define WM_HUBBLE_DATA					(WM_USER + 0x0000400c)
#define WM_DOWNLOAD_MSG					(WM_USER + 0x0000400d)
#define WM_HUBBLE_NEWOUTLOOKOBJ			(WM_USER + 0x0000400e)
#define WM_HUBBLE_ACTIVEINSPECTORPAGE	(WM_USER + 0x0000400f)
#define WM_USER_TANGRAMTASK				(WM_USER + 0x00004010)
#define WM_SETWNDFOCUSE					(WM_USER + 0x00004011)
#define WM_UPLOADFILE					(WM_USER + 0x00004012)
#define WM_HUBBLE_DESIGNMSG				(WM_USER + 0x00004013)
#define WM_INSERTTREENODE				(WM_USER + 0x00004014)
#define WM_REFRESHDATA					(WM_USER + 0x00004015)
#define WM_GETSELECTEDNODEINFO			(WM_USER + 0x00004016)
#define WM_HUBBLE_DESIGNERCMD			(WM_USER + 0x00004017)
#define WM_HUBBLE_GETTREEINFO			(WM_USER + 0x00004018)
#define WM_HUBBLE_GETNODE				(WM_USER + 0x00004019)
#define WM_HUBBLE_UPDATENODE			(WM_USER + 0x0000401a)
#define WM_HUBBLE_SAVE					(WM_USER + 0x0000401b)
#define WM_MDICLIENTCREATED				(WM_USER + 0x0000401c)
#define WM_TGM_SETACTIVEPAGE			(WM_USER + 0x0000401d)
#define WM_TGM_SET_CAPTION				(WM_USER + 0x0000401e)	
#define WM_GETNODEINFO					(WM_USER + 0x0000401f)
#define WM_CREATETABPAGE				(WM_USER + 0x00004020)
#define WM_ACTIVETABPAGE				(WM_USER + 0x00004021)
#define WM_MODIFYTABPAGE				(WM_USER + 0x00004022)
#define WM_ADDTABPAGE					(WM_USER + 0x00004023)
#define WM_HUBBLE_ITEMLOAD				(WM_USER + 0x00003024)
#define WM_INITOUTLOOK					(WM_USER + 0x00004026)
#define WM_CONTROLBARCREATED			(WM_USER + 0x00004027)
#define WM_QUERYAPPPROXY				(WM_USER + 0x00004028)
#define WM_HUBBLE_ACTIVEPAGE			(WM_USER + 0x00004029)
#define WM_HUBBLE_SETAPPTITLE			(WM_USER + 0x0000402a)
#define WM_LYNCIMWNDCREATED				(WM_USER + 0x0000402b)
#define WM_STOPTRACKING					(WM_USER + 0x0000402c)
#define WM_HUBBLE_INIT					(WM_USER + 0x0000402d)
#define WM_VSSHOWPROPERTYGRID			(WM_USER + 0x0000402e)
#define WM_REMOVERESTKEY				(WM_USER + 0x0000402f)
#define WM_HUBBLE_GETXML				(WM_USER + 0x00004030)
#define WM_CHROMEWEBCLIENTCREATED		(WM_USER + 0x00004031)
#define WM_CHROMERENDERERFRAMEHOSTINIT	(WM_USER + 0x00004032)
#define WM_CHROMEOPENWINDOWMSG			(WM_USER + 0x00004033)
#define WM_BROWSERLAYOUT                (WM_USER + 0x00004034)
#define WM_CHROMEMSG	                (WM_USER + 0x00004035)
#define WM_CHROMEWNDPARENTCHANGED       (WM_USER + 0x00004035)
#define WM_DEVICESCALEFACTORCHANGED     (WM_USER + 0x00004036)
#define WM_CHROMEDEVTOOLMSG	            (WM_USER + 0x00004037)
#define WM_BACKGROUNDWEBPROXY_MSG       (WM_USER + 0x00004039)
#define WM_CHROMEWNDNODEMSG             (WM_USER + 0x00004040)
#define WM_DOCUMENTONLOADCOMPLETED      (WM_USER + 0x00004043)
#define WM_DOCUMENTFAILLOADWITHERROR    (WM_USER + 0x00004044)
#define WM_CHROMEHELPWND                (WM_USER + 0x00004045)
#define WM_CHROMEOMNIBOXPOPUPVISIBLE    (WM_USER + 0x00004046)
#define WM_HOSTNODEFORSPLITTERCREATED   (WM_USER + 0x00004047)
#define WM_CHROMEOPENURLMSG				(WM_USER + 0x00004048)
#define WM_LBUTTONDOWNEX			    (WM_USER + 0x00004049)
#define WM_WINFORMCREATED				(WM_USER + 0x0000404a)
#define WM_CHROMEIPCMSG					(WM_USER + 0x00004052)
#define WM_CHROMEAPPINIT				(WM_USER + 0x00004053)

#define TANGRAM_OBJECT_ENTRY_AUTO(clsid, class) \
    __declspec(selectany) ATL::_ATL_OBJMAP_CACHE __objCache__##class = { NULL, 0 }; \
	const ATL::_ATL_OBJMAP_ENTRY_EX __objMap_##class = {&clsid, class::UpdateRegistry, class::_ClassFactoryCreatorClass::CreateInstance, class::CreateInstance, &__objCache__##class, class::GetObjectDescription, class::GetCategoryMap, class::ObjectMain }; \
	extern "C" __declspec(allocate("ATL$__m")) __declspec(selectany) const ATL::_ATL_OBJMAP_ENTRY_EX* const __pobjMap_##class = &__objMap_##class; \
	OBJECT_ENTRY_PRAGMA(class)

#define OBJECT_ENTRY_AUTO_EX(clsid, class) \
    __declspec(selectany) ATL::_ATL_OBJMAP_CACHE __objCache__##class = { NULL, 0 }; \
	const ATL::_ATL_OBJMAP_ENTRY_EX __objMap_##class = {&clsid, class::UpdateRegistry, class::_ClassFactoryCreatorClass::CreateInstance, class::CreateInstance, &__objCache__##class, class::GetObjectDescription, class::GetCategoryMap, class::ObjectMain }; \
	extern "C" __declspec(allocate("ATL$__m")) __declspec(selectany) const ATL::_ATL_OBJMAP_ENTRY_EX* const __pobjMap_##class = &__objMap_##class; \
	OBJECT_ENTRY_PRAGMA(class)

#define TGM_NAME				_T("name")
#define TGM_CAPTION				_T("caption")
#define TGM_GRID_TYPE			_T("gridtype")
#define TGM_OBJ_ID				_T("objid")
#define TGM_HEIGHT				_T("height")
#define TGM_WIDTH				_T("width")
#define TGM_STYLE				_T("style")
#define TGM_ACTIVE_PAGE			_T("activepage")
#define TGM_TAG					_T("tag")
#define TGM_CLUSTER				_T("cluster")
#define TGM_XOBJ				_T("xobj")
#define TGM_NUCLEUS				_T("nucleus")

#define TGM_ROWS				_T("rows")
#define TGM_COLS				_T("cols")

#define TGM_SETTING_HEAD		_T("#$^&TANGRAM")
#define TGM_SETTING_FOMRAT		_T("#$^&TANGRAM[%ld][%ld]")

#define TGM_S_EXCEL_INPUT		1

namespace CommonUniverse {
	using namespace std;
	using namespace ATL;

	class CChromeTab;
	class CCosmosImpl;
	class ICosmosWindow;
	class CChromeTabProxy;
	class CSession;
	class ICosmosDelegate;
	class CChromeBrowserBase;
	class CBrowserImpl;
	class CChromeRenderProcess;
	class CChromeWebContentBase;
	class CCosmosMainDllLoader;
	class OmniboxViewViewsProxy;
	class CCosmosBrowserFactory;
	class CChromeRenderProcessProxy;
	class CChromeWebContentProxyBase;
	class CChromeRenderFrameHost;
	class CChromeChildProcessHostImpl;
	class CWebPageImpl;
	class ICosmosWindowProvider;
	class CChromeChildProcessHostImplProxy;

	class ICosmosCLRImpl;
	class IUniverseAppProxy;

	extern CCosmosImpl* g_pCosmosImpl;

	typedef struct IPCMsg {
		CString m_strId = _T("");
		CString m_strParam1 = _T("");
		CString m_strParam2 = _T("");
		CString m_strParam3 = _T("");
		CString m_strParam4 = _T("");
		CString m_strParam5 = _T("");
		long m_nHandleFrom = 0;
		long m_nHandleTo = 0;
	} IPCMsg;

	typedef struct CosmosInfo
	{
		HWND			m_hCtrlHandle = nullptr;
		HWND			m_pParentForm = nullptr;
		HWND			m_pMDIParent = nullptr;
		CString			m_strName = _T("");
		CString			m_strNodeName = _T("");
		CString			m_strXobjXml = _T("");
		CString			m_strSessionID = _T("");
		CString			m_strTag = _T("");
		IXobj*			m_pXobj	= nullptr;
		IGalaxy*		m_pGalaxy = nullptr;
		IGalaxyCluster*	m_pGalaxyCluster = nullptr;
	}CosmosInfo;

	typedef struct GalaxyInfo
	{
		HWND			m_hCtrlHandle;
		IDispatch*		m_pDisp;
		IDispatch*		m_pParentDisp;
		CString			m_strCtrlName;
		CString			m_strGalaxyName;
		CString			m_strXobjXml;
		CString			m_strParentCtrlName;
	}GalaxyInfo;

	typedef struct DocTemplateInfo
	{
		bool bDll;
		int nResID;
		CString strProxyName;
		CString strProxyID;
		CString strFileTypeID;
		CString strExt;
		CString strfilterName;
	} DocTemplateInfo;

	typedef struct CosmosDocTemplateInfo
	{
		BOOL			m_bCOMObj;
		int				m_nImageIndex;
		HWND			m_hWnd;
		CString			m_strLib;
		CString			m_strExt;
		CString			m_strFilter;
		CString			m_strProxyID;
		CString			m_strDocTemplateKey;
		CString			m_strTemplatePath;
		void* m_pDocTemplate;
	}CosmosDocTemplateInfo;

	typedef struct TangramProjectInfo
	{
		BOOL			m_bTangramSupport;
		int				m_nPrjType;
		int				m_nImageIndex;
		int				m_nIndex;
		CString			m_strPrjFullPath;
		CString			m_strExt;
		CString			m_strFilter;
		IDispatch* m_pPrjDisp;
	}TangramProjectInfo;

	typedef struct CosmosDocInfo
	{
		CString		m_strTangramID;
		CString		m_strAppProxyID;
		CString		m_strAppName;
		CString		m_strMainFrameID;
		CString		m_strDocID;
		CString		m_strCosmosData;
	}CosmosDocInfo;

	typedef struct CtrlInfo
	{
		HWND					m_hWnd;
		CString					m_strName;
		IGalaxyCluster*			m_pGalaxyCluster;
		IXobj*					m_pXobj;
		IDispatch*				m_pCtrlDisp;
	}CtrlInfo;

	typedef struct IPCSession
	{
	public:
		std::map<std::wstring, std::wstring> m_mapString;
		std::map<std::wstring, long> m_mapLong;
		std::map<std::wstring, __int64> m_mapint64;
		std::map<std::wstring, float> m_mapFloat;
	}IPCSession;

	class CSession
	{
	public:
		CSession() {}
		virtual ~CSession() {}

		CWebPageImpl* m_pOwner;

		virtual void InsertString(CString key, CString value) {}
		virtual void InsertLong(CString key, long value) {}
		virtual void Insertint64(CString key, __int64 value) {}
		virtual void InsertFloat(CString key, float value) {}
		virtual CString GetString(CString key) { return _T(""); }
		virtual long GetLong(CString key) { return 0; }
		virtual __int64 Getint64(CString key) { return 0; }
		virtual float GetFloat(CString key) { return 0; }
		virtual void SendMessage() {}
		virtual void addEventListener(CString ListenerName) {}
		virtual void removeEventListener(CString ListenerName) {}
	};

	class CMDIChildFormInfo
	{
	public:
		CMDIChildFormInfo() {}
		~CMDIChildFormInfo() {}
		map<CString, CString>	m_mapFormsInfo;
	};

	typedef CCosmosImpl* (__stdcall* GetCosmosImplFunction)(ICosmos** ppCosmos);

	class CWPFObj
	{
	public:
		CWPFObj()
		{
			m_pDisp = nullptr;
			m_hwndWPF = nullptr;
		}
		~CWPFObj() {}
		HWND m_hwndWPF;
		IDispatch* m_pDisp;
		map<CString, IDispatch*> m_mapWPFObj;
		virtual BOOL IsVisible() { return false; }
		virtual void InvalidateVisual() {}
		virtual void ShowVisual(BOOL bShow) {}
		virtual void Focusable(BOOL bFocus) {}
	};

	class CXobjProxy
	{
	public:
		CXobjProxy() {}
		virtual ~CXobjProxy() {}

		bool	m_bAutoDelete;

		virtual void OnObserverComplete() {}
		virtual void OnDestroy() {}
		virtual void OnXobjAddInCreated(IDispatch* pAddIndisp, CString bstrAddInID, CString bstrAddInXml) {}
		virtual void OnXobjAddInsCreated() {}
		virtual void OnXobjDocumentComplete(IDispatch* ExtenderDisp, CString bstrURL) {}
		virtual void OnControlNotify(IXobj* sender, LONG NotifyCode, LONG CtrlID, HWND CtrlHandle, CString CtrlClassName) {}
		virtual void OnTabChange(LONG ActivePage, LONG OldPage) {}
		virtual void OnCosmosDocEvent(ICosmosEventObj* pEventObj) {}
	};

	class CGalaxyClusterProxy
	{
	public:
		CGalaxyClusterProxy() { }
		virtual ~CGalaxyClusterProxy() {}

		bool	m_bAutoDelete;

		virtual void OnGalaxyClusterLoaded(IDispatch* sender, CString url) {}
		virtual void OnXobjCreated(IXobj* pXobjCreated) {}
		virtual void OnAddInCreated(IXobj* pRootXobj, IDispatch* pAddIn, CString bstrID, CString bstrAddInXml) {}
		virtual void OnBeforeOpenXml(CString bstrXml, HWND hWnd) {}
		virtual void OnOpenXmlComplete(CString bstrXml, HWND hWnd, IXobj* pRetRootNode) {}
		virtual void OnDestroy() {}
		virtual void OnNodeMouseActivate(IXobj* pActiveNode) {}
		virtual void OnClrControlCreated(IXobj* Node, IDispatch* Ctrl, CString CtrlName, HWND CtrlHandle) {}
		virtual void OnTabChange(IXobj* sender, LONG ActivePage, LONG OldPage) {}
		virtual void OnEvent(IDispatch* sender, IDispatch* EventArg) {}
		virtual void OnControlNotify(IXobj* sender, LONG NotifyCode, LONG CtrlID, HWND CtrlHandle, CString CtrlClassName) {}
		virtual void OnCosmosEvent(ICosmosEventObj* NotifyObj) {}
	};

	class CGalaxyProxy
	{
	public:
		CGalaxyProxy() { }
		virtual ~CGalaxyProxy() {}

		bool	m_bAutoDelete;
		virtual void OnExtend(IXobj* pRetNode, CString bstrKey, CString bstrXml) {}
	};

	class CCosmosDocProxy
	{
	public:
		CCosmosDocProxy() {}
		virtual ~CCosmosDocProxy()
		{
			m_bDocLoaded = false;
			m_bCanDestroyFrame = true;
			m_strCosmosData = _T("");
			m_pDoc = nullptr;
		}

		BOOL		m_bDocLoaded;
		BOOL		m_bCanDestroyFrame;
		CString		m_strTangramID;
		CString		m_strAppProxyID;
		CString		m_strAppName;
		CString		m_strMainFrameID;
		CString		m_strDocID;
		CString		m_strCosmosData;

		ICosmosDoc* m_pDoc;
		virtual void SaveDoc() {}
		virtual void CosmosDocEvent(ICosmosEventObj* pEventObj) {}
	};

	class IUniverseAppProxy
	{
	public:
		IUniverseAppProxy() {
			m_bAutoDelete = TRUE;
			m_hMainWnd = nullptr;
			m_hClosingFrame = m_hCreatingView = nullptr;
			m_pvoid = nullptr;
			m_pCurDocProxy = nullptr;
			m_strProxyName = _T("");
			m_strProxyID = _T("");
			m_strClosingFrameID = _T("");
			m_strCreatingFrameTitle = _T("");

			m_bCreatingNewFrame = FALSE;
			m_mapCosmosDoc.clear();

			m_nFrameIndex = 0;
			m_strAppKey = _T("");
		}

		virtual ~IUniverseAppProxy() {}

		BOOL								m_bAutoDelete;
		HWND								m_hMainWnd;
		HWND								m_hCreatingView;
		LPCTSTR								m_strProxyName;
		LPCTSTR								m_strProxyID;
		LPCTSTR								m_strCreatingFrameTitle;
		LPCTSTR								m_strClosingFrameID;
		void* m_pvoid;
		CCosmosDocProxy* m_pCurDocProxy;
		CCosmosImpl* m_pCosmosImpl;

		BOOL								m_bCreatingNewFrame;
		int									m_nFrameIndex;
		HWND								m_hClosingFrame;
		CString								m_strAppKey;
		map<CString, void*>					m_mapMainFrame;
		map<LONGLONG, ICosmosDoc*>			m_mapCosmosDoc;
		map<void*, LONG>					m_mapCosmosDocTemplateID;

		virtual BOOL InitCosmos(void* pVoid) {
			return TRUE;
		}
		virtual BOOL CosmosSaveAllModified() {
			return TRUE;
		}

		virtual void OnActiveMainFrame(HWND) {}
		virtual int OnDestroyMainFrame(CString strID, int nMainFrameCount, int nWndType) {
			return -1;
		}
		virtual LRESULT OnForegroundIdleProc() { return 0; }
		virtual BOOL UniversePreTranslateMessage(MSG* pMsg) { return false; }
		virtual void OnCosmosClose() {}
		virtual void OnObserverComplete(HWND hWnd, CString bstrUrl, IXobj* pRootXobj) {}
		virtual void OnCosmosEvent(ICosmosEventObj* NotifyObj) {}
		virtual void RegistWndClassToCosmos() {}
		virtual void OnActiveDocument(ICosmosDoc* ActiveDoc, IXobj* pXobjInDoc, IXobj* pXobjInCtrlBar, HWND hCtrlBar) {}
		virtual HWND CreateNewFrame(CString strFrameKey) { return NULL; }
		virtual HWND GetActivePopupMenu(HWND) { return NULL; }
		virtual HRESULT CreateCosmosCtrl(void* pv, REFIID riid, LPVOID* ppv) { return S_OK; }
		virtual ICosmosDoc* CreateNewDocument(LPCTSTR lpszFrameID, LPCTSTR lpszAppTitle, void* pDocTemplate, BOOL bNewFrame) { return NULL; }
		virtual ICosmosDoc* OpenDocument(void* pDocTemplate, CString strFile, BOOL bNewFrame) { return NULL; }
		virtual CXobjProxy* OnXobjInit(IXobj* pNewNode) { return nullptr; }
		virtual CGalaxyProxy* OnGalaxyCreated(IGalaxy* pNewGalaxy) { return nullptr; }
		virtual CGalaxyClusterProxy* OnGalaxyClusterCreated(IGalaxyCluster* pNewGalaxy) { return nullptr; }
		virtual void MouseMoveProxy(HWND hWnd) {}
		void RemoveDoc(LONGLONG llDocID)
		{
			auto it = m_mapCosmosDoc.find(llDocID);
			if (it != m_mapCosmosDoc.end())
				m_mapCosmosDoc.erase(it);
		}

		void AddDoc(LONGLONG llDocID, ICosmosDoc* pDoc)
		{
			m_mapCosmosDoc[llDocID] = pDoc;
		}

		ICosmosDoc* GetDoc(LONGLONG llDocID)
		{
			auto it = m_mapCosmosDoc.find(llDocID);
			if (it != m_mapCosmosDoc.end())
				return it->second;
			return nullptr;
		}
		virtual ICosmosDoc* NewDoc() { return nullptr; }
		virtual HWND InitCosmosApp() { return NULL; }
	};

	class ICosmosCLRImpl
	{
	public:
		ICosmosCLRImpl()
		{
		}

		CString							m_strCurrentWinFormTemplate;

		//virtual HRESULT ActiveCLRMethod(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData) = 0;
		//virtual HRESULT ActiveCLRMethod(IDispatch* obj, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData) = 0;
		virtual IDispatch* CreateCLRObj(CString bstrObjID) { return nullptr; }
		virtual HRESULT ProcessCtrlMsg(HWND hCtrl, bool bShiftKey) { return 0; }
		virtual BOOL ProcessFormMsg(HWND hFormWnd, LPMSG lpMsg, int nMouseButton) { return false; }
		virtual IDispatch* CreateObject(BSTR bstrObjID, HWND hParent, IXobj* pHostNode) { return nullptr; }
		virtual int IsWinForm(HWND hWnd) { return 0; }
		virtual IDispatch* GetCLRControl(IDispatch* CtrlDisp, BSTR bstrNames) { return nullptr; }
		virtual BSTR GetCtrlName(IDispatch* pCtrl) { return CComBSTR(""); }
		virtual IDispatch* GetCtrlFromHandle(HWND hWnd) { return nullptr; }
		virtual HWND GetMDIClientHandle(IDispatch* pMDICtrl) { return NULL; }
		virtual IDispatch* GetCtrlByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild) { return nullptr; }
		virtual HWND GetCtrlHandle(IDispatch* pCtrl) { return NULL; }
		virtual BSTR GetCtrlType(IDispatch* pCtrl) { return CComBSTR(""); }
		virtual HWND IsGalaxy(IDispatch* ctrl) { return NULL; }
		virtual void CosmosAction(BSTR bstrXml, void*) {}
		virtual BSTR GetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild) { return CComBSTR(""); }
		virtual void SetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild, BSTR strVal) {}
		virtual void SelectXobj(IXobj*) {}
		virtual void SelectObj(IDispatch*) {}
		virtual void ReleaseCosmosObj(IDispatch*) {}
		virtual void WindowCreated(LPCTSTR strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd) {}
		virtual void WindowDestroy(HWND hWnd) {}
		virtual CWPFObj* CreateWPFControl(IXobj* pXobj, HWND hPWnd, UINT nID) { return nullptr; }
		virtual HRESULT NavigateURL(IXobj* pXobj, CString strURL, IDispatch* dispObjforScript) { return S_FALSE; }
		virtual void OnCLRHostExit() {}
		virtual void OnDestroyChromeBrowser(IBrowser*) {}
		virtual void ExportAllFormsInfo() {}
		virtual void SetObjectProperty(IDispatch* pObj, BSTR bstrPropertyName, BSTR bstrPropertyValue) {}
		virtual void* Extend(CString strKey, CString strData, CString strFeatures) { return nullptr; }
		virtual bool IsSupportDesigner() { return false; }
		virtual HICON GetAppIcon(int nIndex) { return NULL; }
		virtual void OnWinFormActivate(HWND, int nState) {}
		virtual IDispatch* CreateWinForm(HWND hParent, BSTR strXML) { return nullptr; }
		virtual void OnWebPageCreated(HWND, CWebPageImpl*, IWebPage* pChromeWebContent, int nState) {}
		virtual void HideMenuStripPopup() {}
		virtual bool PreWindowPosChanging(HWND hWnd, WINDOWPOS* lpwndpos, int nType) { return false; }
		virtual void OnCloudMsgReceived(CSession*) {}
		virtual void ConnectXobjToWebPage(IXobj*, bool) {}
	};

	class CCosmosImpl {
	public:
		CCosmosImpl() {
			m_hParent = NULL;
			m_hSmallIcon = nullptr;
			m_hLargeIcon = nullptr;
			m_hHostWnd = nullptr;
			m_hFormNodeWnd = nullptr;
			m_hMainWnd = nullptr;
			m_hCosmosWnd = nullptr;
			m_pCLRProxy = nullptr;
			m_hChildHostWnd = nullptr;
			m_pActiveAppProxy = nullptr;
			m_pCosmosAppProxy = nullptr;
			m_pUniverseAppProxy = nullptr;
			m_pCurMDIChildFormInfo = nullptr;
			m_strNtpXml = _T("");
			m_strAppCurrentFormTemplatePath = _T("");
		}

		virtual ~CCosmosImpl() {}

		bool									m_bChromeNeedClosed;
		bool									m_bCreatingDevTool;
		bool									m_bOMNIBOXPOPUPVISIBLE;
		bool									m_b64bitSystem = false;
		bool									m_bOfficeApp = false;
		bool									m_bEclipse = false;
		bool									m_bCanClose;
		bool									m_bWinFormActived;
		bool									m_bEnableProcessFormTabKey;
		bool									m_bDeleteGalaxyCluster;
		bool									m_bNewFile;
		bool									m_bUsingDefaultAppDocTemplate;
		bool									m_bCLRObjTemplateInit;
		bool									m_bIsSupportCrashReporting = false;
		bool									m_bIsEclipseInit = false;

		DWORD									m_dwThreadID;
		DWORD									m_nAppType;
		HWND									m_hTemplateWnd;
		HWND									m_hTemplateChildWnd;
		HWND									m_hActiveWnd;
		HWND									m_hEclipseHideWnd;
		HWND									m_hMainWnd;
		HWND									m_hHostWnd;
		HWND									m_hChildHostWnd;
		HWND									m_hCosmosWnd;
		HWND									m_hFormNodeWnd;
		HWND									m_hParent;
		HWND									m_hHostBrowserWnd;
		HWND									m_hVSToolBoxWnd;
		HICON									m_hLargeIcon;
		HICON									m_hSmallIcon;

		CString									m_strAppKey;
		CString									m_strAppName;
		CString									m_strExeName;
		CString									m_strAppPath;
		CString 								m_strConfigDataFile;
		CString									m_strAppDataPath;
		CString									m_strCurrentAppID;
		CString									m_strProgramFilePath;
		CString									m_strAppCommonDocPath;
		CString									m_strAppCommonDocPath2;
		CString									m_strAppCommonFormsPath;
		CString									m_strAppFormsPath;
		CString									m_strAppFormsInfoPath;
		CString									m_strAppWPFObjsInfoPath;
		CString									m_strAppControlsInfoPath;
		CString									m_strAppFormsTemplatePath;
		CString									m_strAppCurrentFormTemplatePath;
		CString									m_strXobjSelectedText;
		CString									m_strDesignerTip1;
		CString									m_strDesignerTip2;
		CString									m_strDesignerXml;
		CString									m_strDesignerToolBarCaption;
		CString									m_strStartView;
		CString									m_strNewDocXml;
		CString									m_strStartXml;
		CString									m_strNtpXml;

		CString 								m_strConfigFile;
		CString									m_strDocFilters;

		CString									m_strTemplatePath;
		CString									m_strCurrentFrameID;
		CString									m_strDocTemplateStrs;
		CString									m_strDefaultTemplate;
		CString									m_strDefaultTemplate2;
		CString									m_strCurrentDocTemplateXml;
		CString									m_strCurrentKey;
		CString									m_strWorkBenchStrs;
		CString									m_strStartupURL = _T("");
		CString									m_strStartupCLRObj;
		CString									m_strCurrentEclipsePagePath;
		CString									m_strDefaultXml;
		CString									m_strLibs;

		CStringA								m_strBridgeJavaClass;
		CString									m_strStartJarPath;

		IPCMsg*									m_pCurrentIPCMsg;
		ICosmosCLRImpl*							m_pCLRProxy;
		IUniverseAppProxy*						m_pActiveAppProxy;
		IUniverseAppProxy*						m_pUniverseAppProxy;
		IUniverseAppProxy*						m_pCosmosAppProxy;
		CMDIChildFormInfo*						m_pCurMDIChildFormInfo;
		IXobj*									m_pHostViewDesignerNode = nullptr;
		ICosmosExtender*						m_pExtender = nullptr;
		ICosmosDelegate*						m_pCosmosDelegate = nullptr;
		CChromeBrowserBase*						m_pActiveBrowser = nullptr;
		CCosmosBrowserFactory*					m_pBrowserFactory = nullptr;
		ICosmosWindow*							m_pCreatingWindow = nullptr;
		OmniboxViewViewsProxy*					m_pCreatingOmniboxViewViews = nullptr;
		CChromeRenderFrameHost*					m_pCreatingChromeRenderFrameHostBase = nullptr;
		CWebPageImpl*							m_pMainWebPageImpl = nullptr;

		map<CString, IDispatch*>				m_mapObjDic;
		map<HWND, IGalaxyCluster*>				m_mapGalaxy2GalaxyCluster;
		map<HWND, IGalaxyCluster*>				m_mapWindowPage;
		map<CString, CComVariant>				m_mapValInfo;
		map<CString, void*>						m_mapTemplateInfo;
		map<CString, ICosmos*>					m_mapRemoteCosmos;
		map<CString, IUniverseAppProxy*>		m_mapCosmosAppProxy;
		map<CString, ICosmosWindowProvider*>	m_mapWindowProvider;
		map<int, CosmosDocTemplateInfo*>		m_mapCosmosDocTemplateInfo;
		map<CString, CosmosDocTemplateInfo*>	m_mapCosmosDocTemplateInfo2;
		map<CString, CosmosDocTemplateInfo*>	m_mapCosmosFormsTemplateInfo;
		map<int, CosmosDocTemplateInfo*>		m_mapCosmosFormsTemplateInfo2;
		map<HWND, CWebPageImpl*>				m_mapHtmlWnd;
		map<HWND, IXobj*>						m_mapXobj;
		map<HWND, IWebPage*>					m_mapFormWebPage;
		map<HWND, IBrowser*>					m_mapBrowserWnd;
		map<HWND, IWorkBenchWindow*>			m_mapWorkBenchWnd;
		map<void*, IUnknown*>					m_mapObjects;
		map<IDispatch*, CString>				m_mapObjEventDic;
		map<CString, CString>					m_mapJavaNativeInfo;
		map<CString, CString>					m_mapCreatingWorkBenchInfo;
		map<HWND, HWND>							m_mapVSWebPage;
		map<HWND, CString>						m_mapUIData;
		map<HWND, CString>						m_mapCtrlTag;

		virtual void BrowserAppStart() {}
		virtual IGalaxy* ConnectGalaxyCluster(HWND, CString, IGalaxyCluster* pGalaxyCluster, GalaxyInfo*) { return nullptr; }
		virtual void OnSubBrowserWndCreated(HWND hParent, HWND hBrowser) {}
		virtual void OnRenderProcessCreated(CChromeRenderProcess* pProcess) {}
		virtual void OnDocumentOnLoadCompleted(CChromeRenderFrameHost*, HWND hHtmlWnd, void*) {}
		virtual void ChromeTabCreated(CChromeTab* pTab) {}
		virtual CString GetProcessPath(const char* _ver, CString process_type) { return _T(""); }
		virtual CString GetSchemeBaseName() { return _T(""); }
		virtual HICON GetAppIcon(int nIndex) { return NULL; }
		virtual CChromeBrowserBase* GetChromeBrowserBase(HWND) { return nullptr; }
		virtual IBrowser* GetHostBrowser(HWND hNodeWnd) { return nullptr; }
		virtual void AttachXobj(void* pXobjEvents) {}
		virtual void CosmosInit() {}
		virtual ICosmosDoc* ConnectCosmosDoc(IUniverseAppProxy* AppProxy, LONGLONG docID, HWND hView, HWND hGalaxy, LPCTSTR strDocType) { return nullptr; }
		virtual CString GetNewLayoutNodeName(BSTR strObjTypeID, IXobj* pDesignNode) { return _T(""); }
		virtual IGalaxyCluster* Observe(HWND, CString strName, CString strKey) { return nullptr; }
		virtual IXobj* ObserveCtrl(__int64 handle, CString name, CString NodeTag) { return nullptr; }
		virtual bool IsMDIClientGalaxyNode(IXobj*) { return false; }
		virtual void ExportComponentInfo() {}
		virtual void ConnectDocTemplate(LPCTSTR strType, LPCTSTR strExt, void* pTemplate) {}
		virtual void InserttoDataMap(int nType, CString strKey, void* pData) {}
		virtual IWebPage* GetWebPageFromForm(HWND) { return nullptr; }
		virtual long GetIPCMsgIndex(CString strMsgID) { return 0; }
		virtual CSession* CreateCloudSession(CWebPageImpl*) { return nullptr; }
		virtual CSession* GetCloudSession(IXobj*) { return nullptr; }
		virtual void SetMainWnd(HWND hMain) {}
		virtual void CosmosNotify(CString strPara1, CString strPara2, WPARAM, LPARAM) {}
	};

	class ICosmosWindowProvider
	{
	public:
		ICosmosWindowProvider() {}
		virtual ~ICosmosWindowProvider()
		{
			if (::GetModuleHandle(L"universe") != NULL && g_pCosmosImpl)
			{
				g_pCosmosImpl->InserttoDataMap(0, m_strProviderID, nullptr);
			}
		}

		CString					m_strProviderID = _T("");
		CString					m_strContainer = _T("");
		ICosmos*				m_pCosmos = nullptr;
		map<CString, CString>	m_mapInnerObjStyle;
		map<CString, void*>		m_mapInnerObjInfo;

		virtual bool CosmosInit(CString strID)
		{
			strID.MakeLower().Trim();
			if (strID != _T("") && g_pCosmosImpl == nullptr)
			{
				m_strProviderID = strID;
				HMODULE hModule = ::GetModuleHandle(_T("universe.dll"));
				if (hModule) {
					typedef CCosmosImpl* (__stdcall* GetCosmosImpl)(ICosmos**);
					GetCosmosImpl _pCosmosFunction;
					_pCosmosFunction = (GetCosmosImpl)GetProcAddress(hModule, "GetCosmosImpl");
					ICosmos* pCosmos = nullptr;
					g_pCosmosImpl = _pCosmosFunction(&pCosmos);
					m_strProviderID.MakeLower();
					g_pCosmosImpl->InserttoDataMap(1, m_strProviderID, static_cast<ICosmosWindowProvider*>(this));
					return true;
				}
			}
			return false;
		}

		virtual CString GetNames() { return _T(""); }
		virtual CString GetTags(CString strName) { return _T(""); }
		virtual HWND Create(HWND hParentWnd, IXobj* pXobj) { return NULL; }
	};

	class ICosmosWindow
	{
	public:
		ICosmosWindow(){
			if (g_pCosmosImpl)
				g_pCosmosImpl->m_pCreatingWindow = this;
		}

		virtual ~ICosmosWindow() {}

		virtual void Save() {}
	};

	class ICosmosDelegate {
	public:
		ICosmosDelegate() {
			m_bBrowserWndCreated = false;
			m_pJVM = nullptr;
			m_pJVMenv = nullptr;
			systemClass = nullptr;
			exitMethod = nullptr;
			loadMethod = nullptr;
		}

		virtual ~ICosmosDelegate() {}

		BOOL				m_bBrowserWndCreated;
		JavaVM*				m_pJVM;
		JNIEnv*				m_pJVMenv;
		jclass				systemClass;
		jmethodID			exitMethod;
		jmethodID			loadMethod;

		virtual void ProcessMsg(MSG* msg) {}
		virtual void ForegroundIdleProc() {}
		virtual bool DoIdleWork() { return false; }
		virtual BOOL OnAppIdle(BOOL& bIdle, LONG lCount) { return false; }
		virtual BOOL IsAppIdleMessage() { return false; }
		virtual bool OnUniversePreTranslateMessage(MSG* pMsg) { return false; }
		virtual CString GetNTPXml() { return _T(""); }
		virtual HWND GetMainWnd() { return NULL; }
		virtual HWND QueryCanClose(HWND hWnd) { return NULL; }
		virtual bool GetClientAreaBounds(HWND hWnd, RECT& rc) { return false; }
		virtual bool HookAppDocTemplateInfo() { return false; }
		virtual bool EclipseAppInit() { return false; }
		virtual void IPCMsg(HWND hWnd, CString strType, CString strParam1, CString strParam2) {}
		virtual void CustomizedDOMElement(HWND hWnd, CString strRuleName, CString strHTML) {}
		virtual void CosmosNotify(CString strPara1, CString strPara2, WPARAM, LPARAM) {}
	};

	class CCosmosMainDllLoader {
	public:
		CCosmosMainDllLoader() {}
		virtual ~CCosmosMainDllLoader() {}
		virtual void LaunchEx(CCosmosImpl*) {}
	};

	class CCosmosBrowserFactory {
	public:
		CCosmosBrowserFactory() {}
		virtual ~CCosmosBrowserFactory() {}

		virtual HWND CreateBrowser(HWND hParent, CString strXml) { return NULL; }
	};

	class CChromeChildProcessHostImpl {
	public:
		CChromeChildProcessHostImpl() { m_pChildProcessHostProxy = nullptr; }
		virtual ~CChromeChildProcessHostImpl() {}

		CChromeChildProcessHostImplProxy* m_pChildProcessHostProxy;
	};

	class CChromeChildProcessHostImplProxy {
	public:
		CChromeChildProcessHostImplProxy() {}

		virtual ~CChromeChildProcessHostImplProxy() {}

		virtual void OnRenderProcessCreated() {}
	};

	class CChromeRenderProcess {
	public:
		CChromeRenderProcess() { m_pRenderProcessProxy = nullptr; }
		virtual ~CChromeRenderProcess() {}

		CChromeRenderProcessProxy* m_pRenderProcessProxy;
	};

	class CChromeRenderProcessProxy {
	public:
		CChromeRenderProcessProxy() {}

		virtual ~CChromeRenderProcessProxy() {}

		virtual CString ProcessPath() { return _T(""); }
		virtual void OnRenderProcessCreated() {}
	};

	class CChromeTab {
	public:
		CChromeTab() {
			m_pTabProxy = nullptr;
			HMODULE hModule = ::GetModuleHandle(L"universe.dll");
			if (hModule != nullptr) {
				GetCosmosImplFunction GetCosmosImplFunction =
					(CommonUniverse::GetCosmosImplFunction)GetProcAddress(
						hModule, "GetCosmosImpl");
				if (GetCosmosImplFunction != NULL) {
					ICosmos* pCosmos = nullptr;
					CCosmosImpl* _pImpl = GetCosmosImplFunction(&pCosmos);
					if (_pImpl) {
						_pImpl->ChromeTabCreated(this);
					}
				}
			}
		}
		virtual ~CChromeTab() {
		}

		CChromeTabProxy* m_pTabProxy;
	};

	class CChromeTabProxy {
	public:
		CChromeTabProxy() {
			m_pTab = nullptr;
		}
		virtual ~CChromeTabProxy() {
		}
		CChromeTab* m_pTab;
	};

	class OmniboxViewViewsProxy {
	public:
		OmniboxViewViewsProxy() {
			HMODULE hModule = ::GetModuleHandle(L"universe.dll");
			if (hModule != nullptr) {
				GetCosmosImplFunction GetCosmosImplFunction =
					(CommonUniverse::GetCosmosImplFunction)GetProcAddress(
						hModule, "GetCosmosImpl");
				if (GetCosmosImplFunction != NULL) {
					ICosmos* pCosmos = nullptr;
					CCosmosImpl* _pImpl = GetCosmosImplFunction(&pCosmos);
					if (_pImpl) {
						_pImpl->m_pCreatingOmniboxViewViews = this;
					}
				}
			}
		}
		virtual ~OmniboxViewViewsProxy() {}

		virtual void Blur() = 0;
		virtual bool IsFocused() = 0;
	};

	class CChromeBrowserBase {
	public:
		CChromeBrowserBase() {
			HMODULE hModule = ::GetModuleHandle(L"universe.dll");
			if (hModule != nullptr) {
				GetCosmosImplFunction GetCosmosImplFunction =
					(CommonUniverse::GetCosmosImplFunction)GetProcAddress(
						hModule, "GetCosmosImpl");
				if (GetCosmosImplFunction != NULL) {
					ICosmos* pCosmos = nullptr;
					CCosmosImpl* _pImpl = GetCosmosImplFunction(&pCosmos);
					if (_pImpl) {
						m_pProxy = nullptr;
						_pImpl->m_pActiveBrowser = this;
					}
				}
			}
		}
		virtual ~CChromeBrowserBase() {}

		CBrowserImpl* m_pProxy;

		virtual int GetType() { return -1; }
		virtual void* GetBrowser() { return nullptr; }
		virtual void LayoutBrowser() {}
		virtual void OpenURL(std::wstring strURL,
			BrowserWndOpenDisposition nPos,
			void* pVoid) {}
		virtual HWND GetActiveWebContentWnd() { return NULL; }
		virtual bool IsActiveWebContentWnd(HWND hWebWnd) { return false; }
	};

	class CBrowserImpl {
	public:
		CBrowserImpl() {}
		virtual ~CBrowserImpl() {}

		CChromeBrowserBase* m_pBrowser;
		OmniboxViewViewsProxy* m_pOmniboxViewViews;

		virtual void UpdateContentRect(HWND hWebContent, RECT& rc, int nTopFix) {}
		virtual void ActiveChromeTab(HWND hActive, HWND hOldWnd) {}
	};

	class CChromeWebContentBase {
	public:
		CChromeWebContentBase() { m_pProxy = nullptr; }

		virtual ~CChromeWebContentBase() {}

		CChromeWebContentProxyBase* m_pProxy;

		virtual CChromeRenderFrameHost* GetMainRenderFrameHost() { return nullptr; }
	};

	class CChromeWebContentProxyBase {
	public:
		CChromeWebContentProxyBase() { m_pWebContent = nullptr; }

		virtual ~CChromeWebContentProxyBase() {}

		CChromeWebContentBase* m_pWebContent;
	};

	class CChromeRenderFrameHost {
	public:
		CChromeRenderFrameHost() {
			m_pProxy = nullptr;
		}

		virtual ~CChromeRenderFrameHost() {
			for (auto it : m_mapCosmosSession)
			{
				delete it.second;
			}
			m_mapCosmosSession.erase(m_mapCosmosSession.begin(), m_mapCosmosSession.end());
		}

		CWebPageImpl* m_pProxy;
		map<CString, IPCSession*> m_mapCosmosSession;
		virtual void ShowWebPage(bool bShow) {}
		virtual void SendCosmosMessage(IPCMsg*) {}
		virtual void SendCosmosMessage(IPCSession* var) {}
		virtual HWND GetHostBrowserWnd() { return NULL; }
		virtual IPCSession* GetIPCSession() { return nullptr; }
		virtual void InsertString(IPCSession*, CString key, CString value) {}
		virtual void InsertLong(IPCSession*, CString key, long value) {}
		virtual void Insertint64(IPCSession*, CString key, __int64 value) {}
		virtual void InsertFloat(IPCSession*, CString key, float value) {}
		virtual CString GetString(IPCSession*, CString key) { return _T(""); }
		virtual long GetLong(IPCSession*, CString key) { return 0; }
		virtual __int64 Getint64(IPCSession*, CString key) { return 0; }
		virtual float GetFloat(IPCSession*, CString key) { return 0; }
	};

	class CWebPageImpl {
	public:
		CWebPageImpl() {
			m_pRemoteCosmos = nullptr;
			m_pChromeRenderFrameHost = nullptr;
		}

		virtual ~CWebPageImpl()
		{
		}

		ICosmos* m_pRemoteCosmos = nullptr;
		CChromeRenderFrameHost* m_pChromeRenderFrameHost;
		virtual void SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5) = 0;
		virtual CChromeBrowserBase* GetChromeBrowserBase(HWND) { return nullptr; }
		virtual void OnWinFormCreated(HWND) {}
		virtual IXobj* GetParentXobj() { return nullptr; }
		virtual IGalaxy* GetGalaxy() { return nullptr; }
		virtual void OnCloudMsgReceived(CSession*) {}
	};

	class CChromeRendererFrameBase {
	public:
		CChromeRendererFrameBase() {}

		virtual ~CChromeRendererFrameBase() {}
	};

}  // namespace CommonUniverse
