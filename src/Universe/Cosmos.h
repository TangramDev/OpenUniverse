/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102160031           *
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

// Cosmos.h : Declaration of the CCosmos

#include "Universe.h"
#include "ObjSafe.h"

#include "chromium\Browser.h"

#pragma once

using namespace Browser;
using namespace CommonUniverse;
class CWinForm;
struct CommonThreadInfo
{
	HHOOK					m_hGetMessageHook;
	map<HWND, CGalaxy*>		m_mapGalaxy;
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
	friend class CUniverse;
public:
	CCosmos();
	virtual ~CCosmos();

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
	HWND								m_hTempBrowserWnd = NULL;
	HHOOK								m_hCBTHook;
	HHOOK								m_hForegroundIdleHook;
	TCHAR								m_szBuffer[MAX_PATH];

	LPCTSTR								m_lpszSplitterClass;

	//.NET Version 4: 
	ICLRRuntimeHost*					m_pClrHost;

	CWebPage*							m_pHostHtmlWnd = nullptr;
	CWebPage*							m_pHtmlWndCreated;
	CWebPage*							m_pActiveHtmlWnd;

	CWinForm*							m_pActiveWinFormWnd;

	CXobj*								m_pActiveXobj;
	CXobj*								m_pDesignXobj;
	CGalaxy*							m_pGalaxy;

	map<CString, long>					m_mapIPCMsgIndexDic;

	map<CString, CRuntimeClass*>		m_TabWndClassInfoDictionary;
	map<__int64, CXobjCollection*>		m_mapWndXobjCollection;
	map<CXobj*, CString>				m_mapXobjForHtml;
	map<CString, HWND>					m_mapSingleWndApp;
	map<HWND, CWinForm*>				m_mapNeedQueryOnClose;

	BEGIN_COM_MAP(CCosmos)
		COM_INTERFACE_ENTRY(ICosmos)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CCosmos)
		CONNECTION_POINT_ENTRY(__uuidof(_ICosmos))
	END_CONNECTION_POINT_MAP()

	STDMETHOD(get_RootNodes)(IXobjCollection** pXobjColletion);
	STDMETHOD(get_CurrentActiveXobj)(IXobj** pVal);
	STDMETHOD(get_CreatingXobj)(IXobj** pVal);
	STDMETHOD(get_DesignNode)(IXobj** pVal);
	STDMETHOD(get_AppKeyValue)(BSTR bstrKey, VARIANT* pVal);
	STDMETHOD(put_AppKeyValue)(BSTR bstrKey, VARIANT newVal);
	STDMETHOD(get_RemoteHelperHWND)(LONGLONG* pVal);
	STDMETHOD(put_RemoteHelperHWND)(LONGLONG newVal) { m_hHostWnd = (HWND)newVal; return S_OK; };
	STDMETHOD(get_HostWnd)(LONGLONG* pVal);
	STDMETHOD(get_RemoteCosmos)(BSTR bstrID, ICosmos** pVal);
	STDMETHOD(get_Extender)(ICosmosExtender** pVal);
	STDMETHOD(put_Extender)(ICosmosExtender* newVal);
	STDMETHOD(get_DocTemplate)(BSTR bstrKey, LONGLONG* pVal);
	STDMETHOD(get_ActiveChromeBrowserWnd)(IBrowser** ppChromeWebBrowser);
	STDMETHOD(get_HostChromeBrowserWnd)(IBrowser** ppChromeWebBrowser);

	STDMETHOD(NavigateNode)(IXobj* pXobj, BSTR bstrObjID, BSTR bstrXml, IXobj** pVal);
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
	STDMETHOD(ObserveGalaxys)(LONGLONG hWnd, BSTR bstrDeleteGalaxys, BSTR bstrKey, BSTR bstrXml, VARIANT_BOOL bSave);
	STDMETHOD(DeletePage)(LONGLONG PageHandle);
	STDMETHOD(LoadDocComponent)(BSTR bstrLib, LONGLONG* llAppProxy);
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
	void CosmosInitFromeWeb();
	void ExitInstance();
	int	 LoadCLR();
	LRESULT Close(void);
	CXobj* ObserveEx(long hHostMainWnd, CString strExXml, CString strXTMLFile);
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
	CWindow								m_HelperWnd;
	CComObject<CXobjCollection>*		m_pRootNodes;
	map<DWORD, CommonThreadInfo*>		m_mapThreadInfo;

	void CosmosLoad();
	void AttachXobj(void* pXobjEvents);

	IGalaxyCluster* Observe(HWND, CString strName, CString strKey);
	IXobj* ObserveCtrl(__int64 handle, CString name, CString NodeTag);
	void BrowserAppStart();
	bool IsMDIClientGalaxyNode(IXobj*);

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
	void CosmosNotify(CString strXml1, CString strXml2, LONGLONG wParam, LONGLONG lParam);
};
