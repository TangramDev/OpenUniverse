/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202012280003
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

// Cosmos.h : Declaration of the CCosmos

#include "universe.h"
#include "ObjSafe.h"
#include "wpfview.h"

#include "chromium\Browser.h"

#pragma once
//https://github.com/eclipse/rt.equinox.framework/tree/master/features/org.eclipse.equinox.executable.feature/library/win32

using namespace Browser;
using namespace CommonUniverse;
class CWinForm;
struct CommonThreadInfo
{
	HHOOK						 m_hGetMessageHook;
	map<HWND, CGalaxy*> m_mapGalaxy;
};

struct TangramFrameInfo
{
	CString m_strKey;
	CString m_strXml;
};

struct RemoteDebugInfo
{
	LONGLONG nHandle;
	CString m_strKey;
	void* m_pVoid;
};

class ATL_NO_VTABLE CCosmosEvent :
	public CComObjectRootBase,
	public IDispatchImpl<ICosmosEventObj, &IID_ICosmosEventObj, &LIBID_Universe, 1, 0>
{
public:
	CCosmosEvent();
	virtual ~CCosmosEvent();

	BEGIN_COM_MAP(CCosmosEvent)
		COM_INTERFACE_ENTRY(ICosmosEventObj)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	void Lock() {}
	void Unlock() {}

	void Init(CString strEventName, int nEventIndex, IDispatch* pSourceDisp)
	{
		m_nEventIndex = nEventIndex;
		m_strEventName = strEventName;
		if (pSourceDisp)
		{
			m_pSourceObj = pSourceDisp;
			m_pSourceObj->AddRef();
		}
	}
	BOOL m_bNotFired;
	int m_nEventIndex;
	CString m_strEventName;
	IDispatch*	m_pSourceObj;
	map<int, IDispatch*> m_mapDisp;
	map<int, VARIANT> m_mapVar;
protected:
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease() { return 1; }
public:
	STDMETHOD(get_EventName)(BSTR* pVal);
	STDMETHOD(put_EventName)(BSTR newVal);
	STDMETHOD(get_Object)(int nIndex, IDispatch** pVal);
	STDMETHOD(put_Object)(int nIndex, IDispatch* newVal);
	STDMETHOD(get_Value)(int nIndex, VARIANT* pVal);
	STDMETHOD(put_Value)(int nIndex, VARIANT newVal);
	STDMETHOD(get_eventSource)(IDispatch** pVal);
	STDMETHOD(put_eventSource)(IDispatch* pSource);
	STDMETHOD(get_Index)(int* nVal);
	STDMETHOD(put_Index)(int newVal);
};

class ATL_NO_VTABLE CCosmosDoc :
	public CComObjectRootBase,
	public IDispatchImpl<ICosmosDoc, &IID_ICosmosDoc, &LIBID_Universe, 1, 0>
{
public:
	CCosmosDoc();
	virtual ~CCosmosDoc();
	BEGIN_COM_MAP(CCosmosDoc)
		COM_INTERFACE_ENTRY(ICosmosDoc)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	int								m_nState;

	LONGLONG						m_llDocID;
	CString							m_strDocID;
	CString							m_strCurrentWndID;
	CString							m_strTemplateXml;
	CString							m_strMainFrameID;
	CString							m_strPath;

	IUniverseAppProxy*				m_pAppProxy;
	CCosmosDocWnd*					m_pActiveWnd;
	CCosmosDocProxy*				m_pDocProxy;
	map<CString, CString>			m_mapWndScript;
	map<CString, CCosmosDocFrame*> m_mapGalaxy;

	void Lock() {}
	void Unlock() {}

protected:
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease() { return 1; }

private:
	STDMETHOD(get_TemplateXml)(BSTR* bstrDocData);
	STDMETHOD(put_TemplateXml)(BSTR newVal);
	STDMETHOD(put_DocType)(BSTR newVal);
	STDMETHOD(get_DocID)(LONGLONG* pVal);
	STDMETHOD(put_DocID)(LONGLONG newVal);
	STDMETHOD(GetGalaxyWndXml)(BSTR bstrWndID, BSTR* bstrWndScriptVal);
};

class CUniverseMDIChild;
class ATL_NO_VTABLE CCosmosDocTemplate :
	public CComObjectRootBase,
	public IDispatchImpl<ICosmosDocTemplate, &IID_ICosmosDocTemplate, &LIBID_Universe, 1, 0>
{
public:
	CCosmosDocTemplate();
	virtual ~CCosmosDocTemplate();
	BEGIN_COM_MAP(CCosmosDocTemplate)
		COM_INTERFACE_ENTRY(ICosmosDocTemplate)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	CString							m_strKey;
	CString							m_strClientKey;
	CString							m_strDocTemplatePath;
	map<CString, CString>			m_mapXml;
	map<HWND, CXobj*>				m_mapMainPageNode;
	map<HWND, CGalaxy*>				m_mapConnectedFrame;
	map<HWND, CUniverseMDIChild*> m_mapCosmosMDIChildWnd;

	void InitXmlData();
	bool SaveXmlData();

	void Lock() {}
	void Unlock() {}

protected:
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease() { return 1; }

private:
	STDMETHOD(get_TemplateXml)(BSTR* bstrDocData);
	STDMETHOD(put_TemplateXml)(BSTR newVal);
	STDMETHOD(put_DocType)(BSTR newVal);
	STDMETHOD(get_DocID)(LONGLONG* pVal);
	STDMETHOD(put_DocID)(LONGLONG newVal);
	STDMETHOD(GetGalaxyWndXml)(BSTR bstrWndID, BSTR* bstrWndScriptVal);
};

class CCosmosDocFrame
{
public:
	CCosmosDocFrame();
	~CCosmosDocFrame();

	CString							m_strWndID;
	CCosmosDoc*						m_pCosmosDoc;
	CCosmosDocWnd*					m_pCurrentWnd;
	CGalaxy*						m_pHostGalaxy;

	map<HWND, CCosmosDocWnd*>		m_mapWnd;
};

// CCosmos
class ATL_NO_VTABLE CCosmos :
	public CCosmosImpl,
	public CComObjectRootBase,
	public IConnectionPointContainerImpl<CCosmos>,
	public IConnectionPointImpl<CCosmos, &__uuidof(_ICosmos)>,
	public IDispatchImpl<ICosmos, &IID_ICosmos, &LIBID_Universe, 1, 0>
{
	friend class CGalaxy;
	friend class CEclipseWnd;
	friend class CUniverse;
public:
	CCosmos();
	virtual ~CCosmos();

	int										m_nRef;
	int										m_nAppID;
	int 									launchMode;
	int 									m_nJVMVersion;

	int										m_nTangramObj;
	bool									m_bOfficeAddinUnLoad;

#ifdef _DEBUG
	int										m_nJsObj;
	int										m_nTangram;
	int										m_nTangramCtrl;
	int										m_nTangramFrame;
	int										m_nOfficeDocs;
	int										m_nOfficeDocsSheet;
	int										m_nTangramNodeCommonData;
#endif
	HWND									m_hTempBrowserWnd = NULL;
	HHOOK									m_hCBTHook;
	HHOOK									m_hForegroundIdleHook;
	TCHAR									m_szBuffer[MAX_PATH];

	LPCTSTR									m_lpszSplitterClass;
	CString									m_strAppXml;
	CString									m_strMainWndXml;
	CString									m_strDefaultWorkBenchXml;
	CImageList								m_DocImageList;
	CImageList								m_DocTemplateImageList;

	//.NET Version 4: 
	ICLRRuntimeHost*						m_pClrHost;

	CWebPage*								m_pHostHtmlWnd = nullptr;
	CWebPage*								m_pHtmlWndCreated;
	CWebPage*								m_pActiveHtmlWnd;

	CGalaxy*								m_pDocTemplateFrame;
	CCosmosDocWnd*							m_pActiveDocWnd;
	CUniverseMDIMain*						m_pMDIMainWnd;
	CWinForm*								m_pActiveWinFormWnd;
	CCosmosDocTemplate*						m_pActiveTemplate;
	CUniverseMDIChild*						m_pActiveMDIChildWnd;
	CosmosDocTemplateInfo*					m_pCosmosDocTemplateInfo;

	CXobj*									m_pActiveXobj;
	CXobj*									m_pDesignRootNode;
	CXobj*									m_pDesignXobj;
	CXobj*									m_pHostDesignUINode;
	CGalaxy*								m_pGalaxy;
	CGalaxy*								m_pDesignerFrame;
	CGalaxy*								m_pDesigningFrame;
	CGalaxyCluster*							m_pGalaxyCluster;
	CGalaxyCluster*							m_pDesignerGalaxyCluster;

	CCosmosAppCtrl*							m_pCosmosAppCtrl;
	ICosmos*								m_pCosmosVS = nullptr;

	CTangramHtmlTreeWnd*					m_pDocDOMTree;
	CEclipseWnd*							m_pActiveEclipseWnd;

	vector<HWND>							m_vHtmlWnd;
	//map<CString, int>						m_mapEventDic;
	map<CString, long>						m_mapIPCMsgIndexDic;
	map<CString, CCosmosDoc*>				m_mapOpenDoc;
	map<HWND, CGalaxy*>						m_mapBKFrame;
	map<HWND, CCosmosDocWnd*>				m_mapMDTFrame;

	map<LONGLONG, CCosmosEvent*>			m_mapEvent;
	map<HWND, CCommonCtrl*>					m_mapCosmosCommonCtrl;
	vector<HWND>							m_vecEclipseHideTopWnd;
	map<HWND, CWinForm*>					m_mapMainForm;
	map<HWND, CUniverseMDIChild*>			m_mapCosmosMDIChildWnd;
	map<HWND, CAFXHelperWnd*>				m_mapCosmosAFXHelperWnd;
	map<CString, CRuntimeClass*>			m_TabWndClassInfoDictionary;
	map<CString, CHelperWnd*>				m_mapRemoteTangramHelperWnd;
	map<__int64, CXobjCollection*>			m_mapWndXobjCollection;
	map<int, ICosmos*>						m_mapRemoteTangramApp;
	map<CXobj*, CString>					m_mapXobjForHtml;
	map<CString, HWND>						m_mapSingleWndApp;
	map<HWND, CWinForm*>					m_mapNeedQueryOnClose;

	BEGIN_COM_MAP(CCosmos)
		COM_INTERFACE_ENTRY(ICosmos)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CCosmos)
		CONNECTION_POINT_ENTRY(__uuidof(_ICosmos))
	END_CONNECTION_POINT_MAP()

	BEGIN_CATEGORY_MAP(CCosmos)
		IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
		IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	END_CATEGORY_MAP()

	STDMETHOD(get_RootNodes)(IXobjCollection** pXobjColletion);
	STDMETHOD(get_CurrentActiveXobj)(IXobj** pVal);
	STDMETHOD(get_CreatingXobj)(IXobj** pVal);
	STDMETHOD(get_DesignNode)(IXobj** pVal);
	STDMETHOD(get_AppExtender)(BSTR bstrKey, IDispatch** pVal);
	STDMETHOD(put_AppExtender)(BSTR bstrKey, IDispatch* newVal);
	STDMETHOD(get_AppKeyValue)(BSTR bstrKey, VARIANT* pVal);
	STDMETHOD(put_AppKeyValue)(BSTR bstrKey, VARIANT newVal);
	STDMETHOD(get_RemoteHelperHWND)(LONGLONG* pVal);
	STDMETHOD(put_RemoteHelperHWND)(LONGLONG newVal) { m_hHostWnd = (HWND)newVal; return S_OK; };
	STDMETHOD(get_CosmosDoc)(LONGLONG AppProxy, LONGLONG nDocID, ICosmosDoc** pVal);
	STDMETHOD(get_HostWnd)(LONGLONG* pVal);
	STDMETHOD(get_RemoteCosmos)(BSTR bstrID, ICosmos** pVal);
	STDMETHOD(get_Extender)(ICosmosExtender** pVal);
	STDMETHOD(put_Extender)(ICosmosExtender* newVal);
	STDMETHOD(get_DocTemplate)(BSTR bstrKey, LONGLONG* pVal);
	STDMETHOD(get_ActiveChromeBrowserWnd)(IBrowser** ppChromeWebBrowser);
	STDMETHOD(get_HostChromeBrowserWnd)(IBrowser** ppChromeWebBrowser);

	STDMETHOD(NavigateNode)(IXobj* pXobj, BSTR bstrObjID, BSTR bstrXml, IXobj** pVal);
	STDMETHOD(ActiveCLRMethod)(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData);
	STDMETHOD(AttachObjEvent)(IDispatch* pDisp, int nEventIndex);
	STDMETHOD(CreateOfficeDocument)(BSTR bstrXml);// { return S_OK; };
	STDMETHOD(CreateCLRObj)(BSTR bstrObjID, IDispatch** ppDisp);
	STDMETHOD(CreateGalaxyCluster)(LONGLONG hWnd, IGalaxyCluster** ppCosmos);
	STDMETHOD(CreateCosmosCtrl)(BSTR bstrAppID, ICosmosCtrl** ppRetCtrl);
	STDMETHOD(CreateCosmosEventObj)(ICosmosEventObj** ppCosmosEventObj);
	STDMETHOD(DownLoadFile)(BSTR strFileURL, BSTR bstrTargetFile, BSTR bstrActionXml);
	STDMETHOD(Encode)(BSTR bstrSRC, VARIANT_BOOL bEncode, BSTR* bstrRet);
	STDMETHOD(ExportOfficeObjXml)(IDispatch* OfficeObject, BSTR* bstrXml) { return S_OK; };
	STDMETHOD(FireCosmosEventObj)(ICosmosEventObj* pCosmosEventObj);
	STDMETHOD(GetGalaxy)(LONGLONG hHostWnd, IGalaxy** ppGalaxy);
	STDMETHOD(GetXobjFromHandle)(LONGLONG hWnd, IXobj** ppRetXobj);
	STDMETHOD(GetCtrlByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp);
	STDMETHOD(GetCtrlValueByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal);
	STDMETHOD(GetDocTemplateXml)(BSTR bstrCaption, BSTR bstrPath, BSTR bstrFilter, BSTR* bstrTemplatePath);
	STDMETHOD(GetWindowClientDefaultNode)(IDispatch* pAddDisp, LONGLONG hParent, BSTR bstrWndClsName, BSTR bstrGalaxyClusterName, IXobj** ppXobj);
	STDMETHOD(GetItemText)(IXobj* pXobj, long nCtrlID, LONG nMaxLengeh, BSTR* bstrRet);
	STDMETHOD(GetCLRControl)(IDispatch* CtrlDisp, BSTR bstrNames, IDispatch** ppRetDisp);
	STDMETHOD(MessageBox)(LONGLONG hWnd, BSTR bstrContext, BSTR bstrCaption, long nStyle, int* nRet);
	STDMETHOD(NewGUID)(BSTR* retVal);
	STDMETHOD(StartApplication)(BSTR bstrAppID, BSTR bstrXml);
	STDMETHOD(SetCtrlValueByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal);
	STDMETHOD(SetItemText)(IXobj* pXobj, long nCtrlID, BSTR bstrRet);
	STDMETHOD(SetHostFocus)(void);
	STDMETHOD(UpdateXobj)(IXobj* pXobj);
	STDMETHOD(CosmosCommand)(IDispatch* RibbonControl) { return S_OK; };
	STDMETHOD(CosmosGetImage)(BSTR strValue, IPictureDisp ** ppDispImage) { return S_OK; };
	STDMETHOD(CosmosGetVisible)(IDispatch* RibbonControl, VARIANT* varVisible) { return S_OK; };
	STDMETHOD(CosmosOnLoad)(IDispatch* RibbonControl) { return S_OK; };
	STDMETHOD(CosmosGetItemCount)(IDispatch* RibbonControl, long* nCount) { return S_OK; };
	STDMETHOD(CosmosGetItemLabel)(IDispatch* RibbonControl, long nIndex, BSTR* bstrLabel) { return S_OK; };
	STDMETHOD(CosmosGetItemID)(IDispatch* RibbonControl, long nIndex, BSTR* bstrID) { return S_OK; };
	STDMETHOD(ObserveGalaxys)(LONGLONG hWnd, BSTR bstrGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSave);
	STDMETHOD(DeletePage)(LONGLONG PageHandle);
	STDMETHOD(LoadDocComponent)(BSTR bstrLib, LONGLONG* llAppProxy);
	STDMETHOD(OpenTangramFile)(ICosmosDoc** ppDoc);
	STDMETHOD(OpenCosmosDocFile)(BSTR bstrFilePath, ICosmosDoc** ppDoc);
	STDMETHOD(NewWorkBench)(BSTR bstrCosmosDoc, IWorkBenchWindow** ppWorkBenchWindow);
	STDMETHOD(CreateOutLookObj)(BSTR bstrObjType, int nType, BSTR bstrURL, IDispatch** ppRetDisp);
	STDMETHOD(InitEclipseApp)();
	STDMETHOD(DeleteGalaxy)(IGalaxy* pGalaxy);
	STDMETHOD(InitCLRApp)(BSTR strInitXml, LONGLONG* llHandle);
	STDMETHOD(CreateBrowser)(ULONGLONG hParentWnd, BSTR strUrls, IBrowser** ppRet);
	STDMETHOD(CosmosNotify)(BSTR strXml1, BSTR strXml2, LONGLONG wParam, LONGLONG lParam);
	STDMETHOD(SelectVSObj)(BSTR strData, IDispatch* pVSObj, LONGLONG nHandle);

	void Init();
	void Lock() {}
	void Unlock() {}
	void CosmosInit();
	void TangramInitFromeWeb();
	void EclipseInit();
	void ExitInstance();
	void InitCosmosDocManager();
	void InitDesignerTreeCtrl(CString strXml);
	void FireAppEvent(CCosmosEvent*);
	void ExportComponentInfo();
	void CreateEclipseApp(CString strKey, CString strXml);
	int	 LoadCLR();
	bool ImportCosmosDocTemplate(CString strFile);
	BOOL CopyFolder(CString strSrcPath, CString strDesPath);
	BOOL IsUserAdministrator();
	BOOL LoadImageFromResource(ATL::CImage *pImage, HMODULE hMod, CString strResID, LPCTSTR lpTyp);
	BOOL LoadImageFromResource(ATL::CImage *pImage, HMODULE hMod, UINT nResID, LPCTSTR lpTyp);
	BOOL InitJNIForTangram();
	CString ConfigJavaVMInfo(CString strOption);
	CString EncodeFileToBase64(CString strSRC);
	CString InitEclipse(_TCHAR* jarFile);
	CString ComputeHash(CString source);
	CString GetDesignerInfo(CString);
	CString GetXmlData(CString strName, CString strXml);
	CString GetDesignerData(CXobj* pXobj);
	CString GetDocTemplateXml(CString strCaption, CString strPath, CString strFilter);
	CString GetPropertyFromObject(IDispatch* pObj, CString strPropertyName);
	CString	BuildSipURICodeStr(CString strURI, CString strPrev, CString strFix, CString strData, int n1);
	CString	GetDataFromStr(CString strCoded, CString& strTime, CString strPrev, CString strFix, int n1);
	CString tangram_for_eclipse(CString strKey, CString strData, CString strFeatures);
	CString GetOfficePath();
	LRESULT Close(void);
	CXobj* ObserveEx(long hHostMainWnd, CString strExXml, CString strXTMLFile);
	CommonThreadInfo* GetThreadInfo(DWORD dwInfo = 0);
#ifndef _WIN64
	void ConnectWebAgent();
#endif

	virtual void CreateCommonDesignerToolBar();
	virtual void ProcessMsg(LPMSG lpMsg);
	virtual void OnOpenDoc(WPARAM) {};
	virtual void UpdateOfficeObj(IDispatch* pObj, CString strXml, CString strName) {};
	virtual void WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd) {};
	virtual void WindowDestroy(HWND hWnd) {};
	virtual HRESULT COMObjCreated(REFCLSID rclsid, LPVOID pv) { return 0; };
	virtual HRESULT RemoteObjCreated(CString strID, void** ppvObject) { return 0; };
	virtual void ConnectDocTemplate(LPCTSTR strType, LPCTSTR strExt, void* pTemplate);
	virtual ICosmosDoc* ConnectCosmosDoc(IUniverseAppProxy* AppProxy, LONGLONG docID, HWND hView, HWND hGalaxy, LPCTSTR strDocType);
	IGalaxy* ConnectGalaxyCluster(HWND, CString, IGalaxyCluster* pGalaxyCluster, GalaxyInfo*);
	IWebPage* GetWebPageFromForm(HWND);

	void _addObject(void* pThis, IUnknown* pUnknown)
	{
		m_mapObjects.insert(make_pair(pThis, pUnknown));
	}

	void _removeObject(void* pThis)
	{
		auto it = m_mapObjects.find(pThis);
		if (it != m_mapObjects.end())
		{
			m_mapObjects.erase(it);
		}
	}

	void _clearObjects()
	{
		map<void*, IUnknown*>::iterator it;
		while ((it = m_mapObjects.begin()) != m_mapObjects.end())
		{
			IUnknown* pUnknown = it->second;
			CCommonFunction::ClearObject<IUnknown>(pUnknown);
			m_mapObjects.erase(it);
		}
	}

	CString GetNewGUID();

protected:
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease();

private:
	HWND								m_hCreatingWnd;
	CString								m_strOfficeAppIDs;
	CString								m_strExcludeAppExtenderIDs;
	CWindow								m_HelperWnd;
	CComObject<CXobjCollection>*		m_pRootNodes;
	map<DWORD, CommonThreadInfo*>		m_mapThreadInfo;

	void CosmosLoad();
	bool CheckUrl(CString&   url);
	void AttachXobj(void* pXobjEvents);
	CString Encode(CString strSRC, BOOL bEnCode);
	CString GetNewLayoutNodeName(BSTR strObjTypeID, IXobj* pDesignNode);
	IGalaxyCluster* Observe(HWND, CString strName, CString strKey);
	IXobj* ObserveCtrl(__int64 handle, CString name, CString NodeTag);
	void BrowserAppStart();
	bool IsMDIClientGalaxyNode(IXobj*);
	int CalculateByteMD5(BYTE* pBuffer, int BufferSize, CString &MD5);
	void FireNodeEvent(int nIndex, CXobj* pXobj, CCosmosEvent* pObj);
	void GetCosmosInfo(CString strFile, CosmosDocInfo*);

	CString RemoveUTF8BOM(CString strUTF8);

	void ChromeTabCreated(CChromeTab* pTab);
	void OnRenderProcessCreated(CChromeRenderProcess* pProcess);
	void OnDocumentOnLoadCompleted(CChromeRenderFrameHost*, HWND hHtmlWnd, void*);
	void ChromeChildProcessCreated(CChromeChildProcessHostImpl*	pChromeChildProcessHostImpl);
	void OnSubBrowserWndCreated(HWND hParent, HWND hBrowser);
	CString GetProcessPath(const char* _ver, CString process_type);
	CString GetSchemeBaseName();
	HICON GetAppIcon(int nIndex);
	CChromeBrowserBase* GetChromeBrowserBase(HWND);
	IBrowser* GetHostBrowser(HWND hNodeWnd);
	void InserttoDataMap(int nType, CString strKey, void* pData);
	char* GetSchemeString(int nType, CString strKey);
	long GetIPCMsgIndex(CString strMsgID);
	CSession* CreateCloudSession(CWebPageImpl*);
	CSession* GetCloudSession(IXobj*);
	void SetMainWnd(HWND hMain);
	DWORD ExecCmd(const CString cmd, const BOOL setCurrentDirectory);
	void CosmosNotify(CString strXml1, CString strXml2, LONGLONG wParam, LONGLONG lParam);
};
