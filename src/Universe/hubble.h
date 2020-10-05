/********************************************************************************
*					Open Universe - version 1.0.0.7								*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
********************************************************************************/

// Hubble.h : Declaration of the CHubble

#include "Universe.h"
#include "ObjSafe.h"

#include "chromium\Browser.h"

#pragma once

using namespace Web;
using namespace CommonUniverse;
class CWinForm;
struct CommonThreadInfo
{
	HHOOK					m_hGetMessageHook;
	map<HWND, CGalaxy*>		m_mapGalaxy;
};

// CHubble
/*Chubble is the key core object of the universe framework. From the perspective
of cosmology, chubble is equivalent to the Hubble telescope in the software universe*/
class ATL_NO_VTABLE CHubble :
	public CHubbleImpl,
	public CComObjectRootBase,
	public IConnectionPointContainerImpl<CHubble>,
	public IConnectionPointImpl<CHubble, &__uuidof(_IHubble)>,
	public IDispatchImpl<IHubble, &IID_IHubble, &LIBID_Universe, 1, 0>
{
	friend class CGalaxy;
	friend class CUniverse;
public:
	CHubble();
	virtual ~CHubble();

	int									m_nRef;
	int									m_nAppID;
	int									m_nTangramObj;

#ifdef _DEBUG
	int									m_nJsObj;
	int									m_nTangram;
	int									m_nTangramCtrl;
	int									m_nTangramFrame;
	int									m_nOfficeDocs;
	int									m_nOfficeDocsSheet;
	int									m_nTangramNodeCommonData;
#endif
	HHOOK								m_hCBTHook;
	HHOOK								m_hForegroundIdleHook;
	TCHAR								m_szBuffer[MAX_PATH];

	LPCTSTR								m_lpszSplitterClass;
	CString								m_strAppXml;
	CString								m_strMainWndXml;

	//.NET Version 4: 
	ICLRRuntimeHost*					m_pClrHost;

	CWebPage*							m_pHostHtmlWnd = nullptr;
	CWebPage*							m_pHtmlWndCreated;
	CWebPage*							m_pActiveHtmlWnd;

	CWinForm*							m_pActiveWinFormWnd;

	CGrid*								m_pActiveGrid;
	CGrid*								m_pDesignGrid;
	CGalaxy*							m_pGalaxy;

	map<CString, long>					m_mapIPCMsgIndexDic;

	map<CString, CRuntimeClass*>		m_TabWndClassInfoDictionary;
	map<__int64, CGridCollection*>		m_mapWndGridCollection;
	map<CGrid*, CString>				m_mapGridForHtml;

	BEGIN_COM_MAP(CHubble)
		COM_INTERFACE_ENTRY(IHubble)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CHubble)
		CONNECTION_POINT_ENTRY(__uuidof(_IHubble))
	END_CONNECTION_POINT_MAP()

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

	STDMETHOD(NavigateNode)(IGrid* pGrid, BSTR bstrObjID, BSTR bstrXnl);
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
	STDMETHOD(ObserveGalaxys)(LONGLONG hWnd, BSTR bstrDeleteGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSave);
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
	void HubbleInitFromeWeb();
	void ExitInstance();
	int	 LoadCLR();
	LRESULT Close(void);
	CGrid* ObserveEx(long hHostMainWnd, CString strExXml, CString strXTMLFile);
	CommonThreadInfo* GetThreadInfo(DWORD dwInfo = 0);

	virtual void ProcessMsg(LPMSG lpMsg);
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
	CString								m_strExcludeAppExtenderIDs;
	CWindow								m_HelperWnd;
	CComObject<CGridCollection>*		m_pRootNodes;
	map<DWORD, CommonThreadInfo*>		m_mapThreadInfo;

	void HubbleLoad();
	void AttachGrid(void* pGridEvents);

	IGalaxyCluster* Observe(HWND, CString strName, CString strKey);
	IGrid* ObserveCtrl(__int64 handle, CString name, CString NodeTag);
	void BrowserAppStart();
	bool IsMDIClientGalaxyNode(IGrid*);

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
	void HubbleNotify(CString strXml1, CString strXml2, LONGLONG wParam, LONGLONG lParam);
};
