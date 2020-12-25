/********************************************************************************
 *             DOM Plus for Application - Version 1.1.8.202012250001
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#pragma once
#include "chromium/WebPage.h"

using namespace Browser;
class CBKWnd;
class CWormhole;
#define WM_HUBBLE_NOTIFY WM_NOTIFY+WM_REFLECT_BASE
class CCommonCtrl :
	public CWindowImpl<CCommonCtrl, CWindow>
{
public:
	CCommonCtrl(void);
	virtual ~CCommonCtrl(void);
	BEGIN_MSG_MAP(CCommonCtrl)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	END_MSG_MAP()
	virtual void OnFinalMessage(HWND hWnd);
	LRESULT OnNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CAFXHelperWnd :
	public CWindowImpl<CAFXHelperWnd, CWindow>
{
public:
	CAFXHelperWnd(void);
	virtual ~CAFXHelperWnd(void);
	HWND m_hFrame;
	HWND m_hParent;
	BEGIN_MSG_MAP(CAFXHelperWnd)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMg)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanging)
	END_MSG_MAP()

private:
	void OnFinalMessage(HWND hWnd);
	LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCosmosMg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CCosmosDocTemplate;
class CUniverseMDIChild;
class CUniverseMDIMain :
	public CWindowImpl<CUniverseMDIMain, CWindow>
{
public:
	CUniverseMDIMain(void);
	virtual ~CUniverseMDIMain(void);

	HWND									m_hMDIClient;
	CGalaxyCluster*								m_pGalaxyCluster;
	CCosmosDocTemplate*					m_pDocTemplate;
	map<HWND, CString>						m_mapDesignableWnd;
	map<CString, CCosmosDocTemplate*>		m_mapCosmosDocTemplate;
	BEGIN_MSG_MAP(CUniverseMDIMain)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_CONTROLBARCREATED, OnControlBarCreated)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
	END_MSG_MAP()
	
	void OnFinalMessage(HWND hWnd);

	void OnCreateDoc(CString strDocData);
	void NavigateKey(CString strKey);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnControlBarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CUniverseMDIChild :
	public CWindowImpl<CUniverseMDIChild, CWindow>
{
public:
	CUniverseMDIChild(void);
	virtual ~CUniverseMDIChild(void);
	BOOL				m_bNoDocView;
	HWND				m_hChild;

	CString				m_strCaption;
	CString				m_strTemplatePath;
	CString				m_strXml;
	CGalaxy*			m_pGalaxy;
	map<HWND, CString>	m_mapDesignableWnd;

	CCosmosDocTemplate* m_pDocTemplate;
	BEGIN_MSG_MAP(CUniverseMDIChild)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_DPICHANGED, OnDpiChanged)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
		MESSAGE_HANDLER(WM_MDIACTIVATE, OnMDIActivate)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_CONTROLBARCREATED, OnControlBarCreated)
	END_MSG_MAP()

	void OnCreateDoc(CString strDocData);
private:
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMDIActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnDpiChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnCosmosMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnWindowPosChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnControlBarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	void OnFinalMessage(HWND hWnd);
};

class CWinForm :
	public CWindowImpl<CWinForm, CWindow>
{
public:
	CWinForm(void);
	virtual ~CWinForm(void);
	bool									m_bReady = false;
	int										m_nState;
	BOOL									m_bMdiForm;
	BOOL									m_bMainForm = false;
	CString									m_strKey;
	CString									m_strXml;
	CString									m_strPath;
	CString									m_strBKID;
	CString									m_strChildFormPath;
	
	CBKWnd*									m_pBKWnd;
	CXobj*									m_pBindMDIXobj = nullptr;
	CXobj*									m_pWebBindMDIXobj = nullptr;
	CWebPage*								m_pOwnerHtmlWnd;
	CWormhole*								m_pWormhole;
	CMDIChildFormInfo*						m_pChildFormsInfo;

	void SendMessage();

	BEGIN_MSG_MAP(CWinForm)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
		MESSAGE_HANDLER(WM_HUBBLE_DATA, OnGetMe)
		MESSAGE_HANDLER(WM_DPICHANGED, OnDpiChanged)
		MESSAGE_HANDLER(WM_GETDPISCALEDSIZE, OnGetDPIScaledSize)
		MESSAGE_HANDLER(WM_WINFORMCREATED, OnFormCreated)
		MESSAGE_HANDLER(WM_HUBBLE_GETXML, OnCosmosGetXml)
		MESSAGE_HANDLER(WM_MDICLIENTCREATED, OnMdiClientCreated)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	void OnFinalMessage(HWND hWnd);

private:
	LRESULT OnDpiChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnGetMe(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnFormCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnCosmosMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnCosmosGetXml(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnGetDPIScaledSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnMdiClientCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CCosmosDocWnd :
	public CWindowImpl<CCosmosDocWnd, CWindow>
{
public:
	CCosmosDocWnd(void);
	virtual ~CCosmosDocWnd(void);

	HWND m_hView;
	CString	m_strKey;
	CString m_strWndID;
	CGalaxy* m_pGalaxy;
	CGalaxyCluster* m_pParentPage;
	CCosmosDocFrame* m_pDocFrame;

	map<CString, HWND> m_mapCtrlBar;
	BEGIN_MSG_MAP(CCosmosDocWnd)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
		MESSAGE_HANDLER(WM_HUBBLE_GETXML, OnCosmosGetXml)
		MESSAGE_HANDLER(WM_CONTROLBARCREATED, OnCtrlBarCreated)
	END_MSG_MAP()

	void OnFinalMessage(HWND hWnd);

private:
	void CtrlBarDocActiveNotify(ICosmosDoc* pDoc, IXobj* pXobjInDoc, IXobj* pXobjInCtrlBar, HWND hCtrlBar);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnCosmosMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnCosmosGetXml(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnCtrlBarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
};

class ATL_NO_VTABLE CGalaxy :
	public CComObjectRootBase,	
	public CWindowImpl<CGalaxy, CWindow>,
	public IDispatchImpl<IGalaxy, &IID_IGalaxy, &LIBID_Universe, 1, 0>
{
public:
	CGalaxy();           
	virtual ~CGalaxy();  

	bool											m_bObserve = false;

	BOOL											m_bTabbedMDIClient;
	BOOL											m_bDetached;
	BOOL											m_bDesignerState;
	BOOL											m_bMDIChild;
	GalaxyType										m_nGalaxyType;

	HWND											m_hPWnd;
	HWND											m_hHostWnd;
	RECT											m_OldRect;
	CString											m_strLastKey;
	CString											m_strAsynKeys;
	CString											m_strGalaxyName;
	CString											m_strCurrentKey;
	CString											m_strCurrentXml;
	CString											m_strHostWebBrowserNodeName = _T("");
	CEclipseWnd*									m_pWorkBenchFrame;
	CCosmosDocTemplate*								m_pCosmosDocTemplate;
	map<IUniverseAppProxy*, CGalaxyProxy*>			m_mapGalaxyProxy;

	IPCMsg*											m_pCurrentIPCMsg;
	CBKWnd*											m_pBKWnd;
	CWebPage*										m_pWebPageWnd;
	CXobj*											m_pHostWebBrowserNode = nullptr;
	CBrowser*										m_pHostWebBrowserWnd = nullptr;
	CGalaxyCluster*									m_pGalaxyCluster;
	CXobj*											m_pParentXobj;
	CXobj*											m_pWorkXobj;
	CXobj*											m_pContainerNode;
	CXobj*											m_pBindingXobj;
	CGalaxy*										m_pSubGalaxy;
	GalaxyInfo*										m_pGalaxyInfo;
	CCosmosDoc*										m_pDoc;
	map<CString, CXobj*>							m_mapXobj;
	map<CString, CXobj*>							m_mapNeedSaveToConfigNode;
	map<CString, VARIANT>							m_mapVal;
	map<HWND, CWPFView*>							m_mapWPFView;
	map<HWND, CWPFView*>							m_mapVisibleWPFView;
	map<CString, CXobj*>							m_mapXobjScript;
	CComObject<CXobjCollection>*					m_pRootNodes;

	void Lock(){}
	void Unlock(){}
	void Destroy();
	void HostPosChanged();
	HWND GetWinForm(HWND hForm);
	void UpdateDesignerTreeInfo();
	void UpdateVisualWPFMap(HWND, BOOL);
	CTangramXmlParse* UpdateXobj();
	BOOL CreateGalaxyCluster();
	CXobj* ObserveXtmlDocument(CTangramXmlParse* pParse, CString strKey, CString	strFile);

	STDMETHOD(get_GalaxyXML)(BSTR* pVal);
	STDMETHOD(ModifyHost)(LONGLONG hHostWnd);
	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj);
	STDMETHOD(GetXml)(BSTR bstrRootName, BSTR* bstrRet);

	BEGIN_COM_MAP(CGalaxy)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IGalaxy)
	END_COM_MAP()

	BEGIN_MSG_MAP(CGalaxy)
		MESSAGE_HANDLER(WM_DPICHANGED, OnDpiChanged)
		MESSAGE_HANDLER(WM_GETDPISCALEDSIZE, OnGetDPIScaledSize)
		MESSAGE_HANDLER(WM_DPICHANGED_BEFOREPARENT, OnBeforeParentDpiChanged)
		MESSAGE_HANDLER(WM_DPICHANGED_AFTERPARENT, OnAfterParentDpiChanged)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_VSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_HUBBLE_DATA, OnGetMe)
		MESSAGE_HANDLER(WM_NCDESTROY, OnNcDestroy)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_PARENTNOTIFY, OnParentNotify)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_QUERYAPPPROXY, OnQueryAppProxy)
		MESSAGE_HANDLER(WM_HUBBLE_ACTIVEPAGE, OnTabChanged)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
	END_MSG_MAP()

protected:
	ULONG InternalAddRef(){ return 1; }
	ULONG InternalRelease(){ return 1; }

private:
	LRESULT OnDpiChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnGetDPIScaledSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnBeforeParentDpiChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnAfterParentDpiChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnGetMe(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnHScroll(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnNcDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnTabChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCosmosMsg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnParentNotify(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnQueryAppProxy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnWindowPosChanging(UINT, WPARAM, LPARAM, BOOL&);

	STDMETHOD(get_Count)(long* pCount);
	STDMETHOD(get_Xobj)(VARIANT vIndex, IXobj **ppXobj);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	STDMETHOD(get_HWND)(LONGLONG* pVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_CurrentNavigateKey)(BSTR* pVal);
	STDMETHOD(get_VisibleXobj)(IXobj** pVal);
	STDMETHOD(get_RootXobjs)(IXobjCollection** pXobjColletion);
	STDMETHOD(get_GalaxyData)(BSTR bstrKey, VARIANT* pVal);
	STDMETHOD(put_GalaxyData)(BSTR bstrKey, VARIANT newVal);
	STDMETHOD(get_DesignerState)(VARIANT_BOOL* pVal);
	STDMETHOD(put_DesignerState)(VARIANT_BOOL newVal);
	STDMETHOD(get_CosmosDoc)(ICosmosDoc** pVal);
	STDMETHOD(get_GalaxyType)(GalaxyType* pVal);
	STDMETHOD(get_Name)(BSTR* pVal);
	STDMETHOD(get_HostBrowser)(IBrowser** ppChromeWebBrowser);
	STDMETHOD(get_HostWebPage)(IWebPage** ppChromeWebPage);

	STDMETHOD(Attach)(void);
	STDMETHOD(Detach)(void);

	void OnFinalMessage(HWND hWnd);
};
