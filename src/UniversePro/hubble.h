/********************************************************************************
*					Open Universe - version 1.1.4.25							*
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

// Hubble.h : Declaration of the CHubble

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
	public IDispatchImpl<IHubbleEventObj, &IID_IHubbleEventObj, &LIBID_Universe, 1, 0>
{
public:
	CCosmosEvent();
	virtual ~CCosmosEvent();

	BEGIN_COM_MAP(CCosmosEvent)
		COM_INTERFACE_ENTRY(IHubbleEventObj)
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

class ATL_NO_VTABLE CHubbleDoc :
	public CComObjectRootBase,
	public IDispatchImpl<IHubbleDoc, &IID_IHubbleDoc, &LIBID_Universe, 1, 0>
{
public:
	CHubbleDoc();
	virtual ~CHubbleDoc();
	BEGIN_COM_MAP(CHubbleDoc)
		COM_INTERFACE_ENTRY(IHubbleDoc)
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
	CHubbleDocWnd*					m_pActiveWnd;
	CHubbleDocProxy*				m_pDocProxy;
	map<CString, CString>			m_mapWndScript;
	map<CString, CHubbleDocFrame*> m_mapGalaxy;

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
class ATL_NO_VTABLE CHubbleDocTemplate :
	public CComObjectRootBase,
	public IDispatchImpl<IHubbleDocTemplate, &IID_IHubbleDocTemplate, &LIBID_Universe, 1, 0>
{
public:
	CHubbleDocTemplate();
	virtual ~CHubbleDocTemplate();
	BEGIN_COM_MAP(CHubbleDocTemplate)
		COM_INTERFACE_ENTRY(IHubbleDocTemplate)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	CString							m_strKey;
	CString							m_strClientKey;
	CString							m_strDocTemplatePath;
	map<CString, CString>			m_mapXml;
	map<HWND, CGrid*>				m_mapMainPageNode;
	map<HWND, CGalaxy*>				m_mapConnectedFrame;
	map<HWND, CUniverseMDIChild*> m_mapHubbleMDIChildWnd;

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

class CHubbleDocFrame
{
public:
	CHubbleDocFrame();
	~CHubbleDocFrame();

	CString							m_strWndID;
	CHubbleDoc*						m_pHubbleDoc;
	CHubbleDocWnd*					m_pCurrentWnd;
	CGalaxy*						m_pHostGalaxy;

	map<HWND, CHubbleDocWnd*>		m_mapWnd;
};

// CHubble
class ATL_NO_VTABLE CHubble :
	public CHubbleImpl,
	public CComObjectRootBase,
	public IConnectionPointContainerImpl<CHubble>,
	public IConnectionPointImpl<CHubble, &__uuidof(_IHubble)>,
	public IDispatchImpl<IHubble, &IID_IHubble, &LIBID_Universe, 1, 0>
{
	friend class CGalaxy;
	friend class CEclipseWnd;
	friend class CUniverse;
public:
	CHubble();
	virtual ~CHubble();

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
	CHubbleDocWnd*							m_pActiveDocWnd;
	CUniverseMDIMain*						m_pMDIMainWnd;
	CWinForm*								m_pActiveWinFormWnd;
	CHubbleDocTemplate*						m_pActiveTemplate;
	CUniverseMDIChild*						m_pActiveMDIChildWnd;
	HubbleDocTemplateInfo*					m_pHubbleDocTemplateInfo;

	CGrid*									m_pActiveGrid;
	CGrid*									m_pDesignRootNode;
	CGrid*									m_pDesignGrid;
	CGrid*									m_pHostDesignUINode;
	CGalaxy*								m_pGalaxy;
	CGalaxy*								m_pDesignerFrame;
	CGalaxy*								m_pDesigningFrame;
	CGalaxyCluster*							m_pGalaxyCluster;
	CGalaxyCluster*							m_pDesignerGalaxyCluster;

	CHubbleAppCtrl*							m_pHubbleAppCtrl;
	IHubble*								m_pHubbleVS = nullptr;

	CTangramHtmlTreeWnd*					m_pDocDOMTree;
	CEclipseWnd*							m_pActiveEclipseWnd;

	vector<HWND>							m_vHtmlWnd;
	//map<CString, int>						m_mapEventDic;
	map<CString, long>						m_mapIPCMsgIndexDic;
	map<CString, CHubbleDoc*>				m_mapOpenDoc;
	map<HWND, CGalaxy*>						m_mapBKFrame;
	map<HWND, CHubbleDocWnd*>				m_mapMDTFrame;

	map<LONGLONG, CCosmosEvent*>			m_mapEvent;
	map<HWND, CCommonCtrl*>					m_mapHubbleCommonCtrl;
	vector<HWND>							m_vecEclipseHideTopWnd;
	map<HWND, CWinForm*>					m_mapMainForm;
	map<HWND, CUniverseMDIChild*>			m_mapHubbleMDIChildWnd;
	map<HWND, CAFXHelperWnd*>				m_mapHubbleAFXHelperWnd;
	map<CString, CRuntimeClass*>			m_TabWndClassInfoDictionary;
	map<CString, CHelperWnd*>				m_mapRemoteTangramHelperWnd;
	map<__int64, CGridCollection*>			m_mapWndGridCollection;
	map<int, IHubble*>						m_mapRemoteTangramApp;
	map<CGrid*, CString>					m_mapGridForHtml;
	map<CString, HWND>						m_mapSingleWndApp;
	map<HWND, CWinForm*>					m_mapNeedQueryOnClose;

	BEGIN_COM_MAP(CHubble)
		COM_INTERFACE_ENTRY(IHubble)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CHubble)
		CONNECTION_POINT_ENTRY(__uuidof(_IHubble))
	END_CONNECTION_POINT_MAP()

	BEGIN_CATEGORY_MAP(CHubble)
		IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
		IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	END_CATEGORY_MAP()

	STDMETHOD(get_RootNodes)(IGridCollection** pGridColletion);
	STDMETHOD(get_CurrentActiveGrid)(IGrid** pVal);
	STDMETHOD(get_CreatingGrid)(IGrid** pVal);
	STDMETHOD(get_DesignNode)(IGrid** pVal);
	STDMETHOD(get_AppExtender)(BSTR bstrKey, IDispatch** pVal);
	STDMETHOD(put_AppExtender)(BSTR bstrKey, IDispatch* newVal);
	STDMETHOD(get_AppKeyValue)(BSTR bstrKey, VARIANT* pVal);
	STDMETHOD(put_AppKeyValue)(BSTR bstrKey, VARIANT newVal);
	STDMETHOD(get_RemoteHelperHWND)(LONGLONG* pVal);
	STDMETHOD(put_RemoteHelperHWND)(LONGLONG newVal) { m_hHostWnd = (HWND)newVal; return S_OK; };
	STDMETHOD(get_HubbleDoc)(LONGLONG AppProxy, LONGLONG nDocID, IHubbleDoc** pVal);
	STDMETHOD(get_HostWnd)(LONGLONG* pVal);
	STDMETHOD(get_RemoteHubble)(BSTR bstrID, IHubble** pVal);
	STDMETHOD(get_Extender)(IHubbleExtender** pVal);
	STDMETHOD(put_Extender)(IHubbleExtender* newVal);
	STDMETHOD(get_DocTemplate)(BSTR bstrKey, LONGLONG* pVal);
	STDMETHOD(get_ActiveChromeBrowserWnd)(IBrowser** ppChromeWebBrowser);
	STDMETHOD(get_HostChromeBrowserWnd)(IBrowser** ppChromeWebBrowser);

	STDMETHOD(NavigateNode)(IGrid* pGrid, BSTR bstrObjID, BSTR bstrXml, IGrid** pVal);
	STDMETHOD(ActiveCLRMethod)(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData);
	STDMETHOD(AttachObjEvent)(IDispatch* pDisp, int nEventIndex);
	STDMETHOD(CreateOfficeDocument)(BSTR bstrXml);// { return S_OK; };
	STDMETHOD(CreateCLRObj)(BSTR bstrObjID, IDispatch** ppDisp);
	STDMETHOD(CreateGalaxyCluster)(LONGLONG hWnd, IGalaxyCluster** ppHubble);
	STDMETHOD(CreateHubbleCtrl)(BSTR bstrAppID, IHubbleCtrl** ppRetCtrl);
	STDMETHOD(CreateHubbleEventObj)(IHubbleEventObj** ppHubbleEventObj);
	STDMETHOD(DownLoadFile)(BSTR strFileURL, BSTR bstrTargetFile, BSTR bstrActionXml);
	STDMETHOD(Encode)(BSTR bstrSRC, VARIANT_BOOL bEncode, BSTR* bstrRet);
	STDMETHOD(ExportOfficeObjXml)(IDispatch* OfficeObject, BSTR* bstrXml) { return S_OK; };
	STDMETHOD(FireHubbleEventObj)(IHubbleEventObj* pHubbleEventObj);
	STDMETHOD(GetGalaxy)(LONGLONG hHostWnd, IGalaxy** ppGalaxy);
	STDMETHOD(GetGridFromHandle)(LONGLONG hWnd, IGrid** ppRetGrid);
	STDMETHOD(GetCtrlByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp);
	STDMETHOD(GetCtrlValueByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal);
	STDMETHOD(GetDocTemplateXml)(BSTR bstrCaption, BSTR bstrPath, BSTR bstrFilter, BSTR* bstrTemplatePath);
	STDMETHOD(GetWindowClientDefaultNode)(IDispatch* pAddDisp, LONGLONG hParent, BSTR bstrWndClsName, BSTR bstrGalaxyClusterName, IGrid** ppGrid);
	STDMETHOD(GetItemText)(IGrid* pGrid, long nCtrlID, LONG nMaxLengeh, BSTR* bstrRet);
	STDMETHOD(GetCLRControl)(IDispatch* CtrlDisp, BSTR bstrNames, IDispatch** ppRetDisp);
	STDMETHOD(MessageBox)(LONGLONG hWnd, BSTR bstrContext, BSTR bstrCaption, long nStyle, int* nRet);
	STDMETHOD(NewGUID)(BSTR* retVal);
	STDMETHOD(StartApplication)(BSTR bstrAppID, BSTR bstrXml);
	STDMETHOD(SetCtrlValueByName)(IDispatch* pCtrl, BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal);
	STDMETHOD(SetItemText)(IGrid* pGrid, long nCtrlID, BSTR bstrRet);
	STDMETHOD(SetHostFocus)(void);
	STDMETHOD(UpdateGrid)(IGrid* pGrid);
	STDMETHOD(HubbleCommand)(IDispatch* RibbonControl) { return S_OK; };
	STDMETHOD(HubbleGetImage)(BSTR strValue, IPictureDisp ** ppDispImage) { return S_OK; };
	STDMETHOD(HubbleGetVisible)(IDispatch* RibbonControl, VARIANT* varVisible) { return S_OK; };
	STDMETHOD(HubbleOnLoad)(IDispatch* RibbonControl) { return S_OK; };
	STDMETHOD(HubbleGetItemCount)(IDispatch* RibbonControl, long* nCount) { return S_OK; };
	STDMETHOD(HubbleGetItemLabel)(IDispatch* RibbonControl, long nIndex, BSTR* bstrLabel) { return S_OK; };
	STDMETHOD(HubbleGetItemID)(IDispatch* RibbonControl, long nIndex, BSTR* bstrID) { return S_OK; };
	STDMETHOD(ObserveGalaxys)(LONGLONG hWnd, BSTR bstrGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSave);
	STDMETHOD(DeletePage)(LONGLONG PageHandle);
	STDMETHOD(LoadDocComponent)(BSTR bstrLib, LONGLONG* llAppProxy);
	STDMETHOD(OpenTangramFile)(IHubbleDoc** ppDoc);
	STDMETHOD(OpenHubbleDocFile)(BSTR bstrFilePath, IHubbleDoc** ppDoc);
	STDMETHOD(NewWorkBench)(BSTR bstrHubbleDoc, IWorkBenchWindow** ppWorkBenchWindow);
	STDMETHOD(CreateOutLookObj)(BSTR bstrObjType, int nType, BSTR bstrURL, IDispatch** ppRetDisp);
	STDMETHOD(InitEclipseApp)();
	STDMETHOD(DeleteGalaxy)(IGalaxy* pGalaxy);
	STDMETHOD(InitCLRApp)(BSTR strInitXml, LONGLONG* llHandle);
	STDMETHOD(CreateBrowser)(ULONGLONG hParentWnd, BSTR strUrls, IBrowser** ppRet);
	STDMETHOD(HubbleNotify)(BSTR strXml1, BSTR strXml2, LONGLONG wParam, LONGLONG lParam);
	STDMETHOD(SelectVSObj)(BSTR strData, IDispatch* pVSObj, LONGLONG nHandle);

	void Init();
	void Lock() {}
	void Unlock() {}
	void HubbleInit();
	void TangramInitFromeWeb();
	void EclipseInit();
	void ExitInstance();
	void InitHubbleDocManager();
	void InitDesignerTreeCtrl(CString strXml);
	void FireAppEvent(CCosmosEvent*);
	void ExportComponentInfo();
	void CreateEclipseApp(CString strKey, CString strXml);
	int	 LoadCLR();
	bool ImportHubbleDocTemplate(CString strFile);
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
	CString GetDesignerData(CGrid* pGrid);
	CString GetDocTemplateXml(CString strCaption, CString strPath, CString strFilter);
	CString GetPropertyFromObject(IDispatch* pObj, CString strPropertyName);
	CString	BuildSipURICodeStr(CString strURI, CString strPrev, CString strFix, CString strData, int n1);
	CString	GetDataFromStr(CString strCoded, CString& strTime, CString strPrev, CString strFix, int n1);
	CString tangram_for_eclipse(CString strKey, CString strData, CString strFeatures);
	CString GetOfficePath();
	LRESULT Close(void);
	CGrid* ObserveEx(long hHostMainWnd, CString strExXml, CString strXTMLFile);
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
	virtual IHubbleDoc* ConnectHubbleDoc(IUniverseAppProxy* AppProxy, LONGLONG docID, HWND hView, HWND hGalaxy, LPCTSTR strDocType);
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
	CComObject<CGridCollection>*		m_pRootNodes;
	map<DWORD, CommonThreadInfo*>		m_mapThreadInfo;

	void HubbleLoad();
	bool CheckUrl(CString&   url);
	void AttachGrid(void* pGridEvents);
	CString Encode(CString strSRC, BOOL bEnCode);
	CString GetNewLayoutNodeName(BSTR strObjTypeID, IGrid* pDesignNode);
	IGalaxyCluster* Observe(HWND, CString strName, CString strKey);
	IGrid* ObserveCtrl(__int64 handle, CString name, CString NodeTag);
	void BrowserAppStart();
	bool IsMDIClientGalaxyNode(IGrid*);
	int CalculateByteMD5(BYTE* pBuffer, int BufferSize, CString &MD5);
	void FireNodeEvent(int nIndex, CGrid* pGrid, CCosmosEvent* pObj);
	void GetHubbleInfo(CString strFile, HubbleDocInfo*);

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
	CSession* GetCloudSession(IGrid*);
	void SetMainWnd(HWND hMain);
	DWORD ExecCmd(const CString cmd, const BOOL setCurrentDirectory);
	void HubbleNotify(CString strXml1, CString strXml2, LONGLONG wParam, LONGLONG lParam);
};
