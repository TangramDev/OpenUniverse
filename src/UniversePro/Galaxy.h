/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103060044
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
#include "chromium/WebPage.h"

using namespace Browser;
class CBKWnd;
class CWormhole;

class CCosmosHelperWnd : public CWnd
{
public:
	CCosmosHelperWnd() {}
	virtual ~CCosmosHelperWnd() {}
	HWND m_hClient = nullptr;
protected:
	void PostNcDestroy()
	{
		CWnd::PostNcDestroy();
		delete this;
	}
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

// CCosmosTreeCtrl window
class CCosmosTreeCtrl : public CTreeCtrl
{
	// Construction
public:
	CCosmosTreeCtrl() noexcept;

	// Overrides
protected:

	// Implementation
public:
	virtual ~CCosmosTreeCtrl();
	HTREEITEM FillTreeView(CTangramXmlParse* pParse, CTangramXmlParse* pParseTag, HTREEITEM hParentItem);
	CXobj* m_pHostObj = nullptr;
	CGalaxy* m_pGalaxy = nullptr;
	map<HTREEITEM, CosmosUIItemData*> m_mapTreeItemData;
protected:
	afx_msg LRESULT OnCloudMsgReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMTVStateImageChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTvnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTvnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};

// CCosmosListCtrl window
class CCosmosListCtrl : public CListCtrl
{
	// Construction
public:
	CCosmosListCtrl() noexcept;

	// Overrides
protected:

	// Implementation
public:
	virtual ~CCosmosListCtrl();
	CXobj* m_pHostObj = nullptr;
	CGalaxy* m_pGalaxy = nullptr;
	map<LVITEM, CosmosUIItemData*> m_mapListItemData;
protected:
	afx_msg LRESULT OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloudMsgReceived(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnLvnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnLvnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnLvnInsertitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnLvnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnLvnLinkClicked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMHover(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMRDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMReturn(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};

// CCosmosTabCtrl window
class CCosmosTabCtrl : public CTabCtrl
{
	// Construction
public:
	CCosmosTabCtrl() noexcept;

	// Overrides
protected:

	// Implementation
public:
	virtual ~CCosmosTabCtrl();
	CGalaxy* m_pGalaxy = nullptr;
	CXobj* m_pHostObj = nullptr;
	map<TCITEM, CosmosUIItemData*> m_mapTabItemData;
protected:
	afx_msg LRESULT OnXobjCreatedMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloudMsgReceived(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTcnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};

class CMDIChild :
	public CWindowImpl<CMDIChild, CWindow>
{
public:
	CMDIChild(void);
	virtual ~CMDIChild(void);
	HWND		m_hClient;

	CString		m_strKey;
	CString		m_strDocXml = _T("");
	CString		m_strDocTemplateKey = _T("");

	CGalaxy*	m_pGalaxy = nullptr;
	CXobj*		m_pClientBindingObj = nullptr;
	CMDIParent* m_pParent = nullptr;
	BEGIN_MSG_MAP(CMDIChild)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMg)
		MESSAGE_HANDLER(WM_MDIACTIVATE, OnMDIActivate)
		MESSAGE_HANDLER(WM_COSMOSOBSERVED, OnCosmosDocObserved)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanging)
	END_MSG_MAP()

private:
	void OnFinalMessage(HWND hWnd);
	LRESULT OnMDIActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCosmosMg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCosmosDocObserved(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnWindowPosChanging(UINT, WPARAM, LPARAM, BOOL&);
};

class CMDTWnd :
	public CWindowImpl<CMDTWnd, CWindow>
{
public:
	CMDTWnd(void);
	virtual ~CMDTWnd(void);
	CString m_strDocTemplateKey = _T("");
	BEGIN_MSG_MAP(CMDTWnd)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSZ)
		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSZ)
	END_MSG_MAP()
private:
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnExitSZ(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEnterSZ(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCosmosMsg(UINT, WPARAM, LPARAM, BOOL&);

	void OnFinalMessage(HWND hWnd);
};

class CMDIParent :
	public CWindowImpl<CMDIParent, CWindow>
{
public:
	CMDIParent(void);
	virtual ~CMDIParent(void);

	BOOL					m_bDestroy = false;
	HWND					m_hMDIClient = nullptr;

	CXobj*					m_pClientXobj = nullptr;
	CGalaxy*				m_pGalaxy = nullptr;
	CGalaxyCluster*			m_pGalaxyCluster = nullptr;

	CMDIChild*				m_pActiveMDIChild = nullptr;
	CosmosFrameWndInfo*		m_pCosmosFrameWndInfo = nullptr;

	CXobjVector				m_vMdiClientXobjs;
	map<HWND, CMDIChild*>	m_mapMDIChild;

	void ShowMdiClientXobj();
	BEGIN_MSG_MAP(CMDIParent)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSZ)
		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSZ)
	END_MSG_MAP()

	void OnFinalMessage(HWND hWnd);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCosmosMsg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnExitSZ(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEnterSZ(UINT, WPARAM, LPARAM, BOOL&);
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

	CBKWnd* m_pBKWnd;
	CXobj* m_pBindMDIXobj = nullptr;
	CXobj* m_pWebBindMDIXobj = nullptr;
	CWebPage* m_pOwnerHtmlWnd;
	CWormhole* m_pWormhole;
	CMDIChildFormInfo* m_pChildFormsInfo;

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
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSZ)
		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSZ)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	void OnFinalMessage(HWND hWnd);

private:
	LRESULT OnDpiChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnGetMe(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnFormCreated(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCosmosMsg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnCosmosGetXml(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnGetDPIScaledSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMdiClientCreated(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnWindowPosChanging(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnExitSZ(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnEnterSZ(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
};

class ATL_NO_VTABLE CGalaxy :
	public CComObjectRootBase,
	public CWindowImpl<CGalaxy, CWindow>,
	public IDispatchImpl<IGalaxy, &IID_IGalaxy, &LIBID_Universe, 1, 0>
{
public:
	CGalaxy();
	virtual ~CGalaxy();

	BOOL											m_bNoRedrawState = false;
	BOOL											m_bTabbedMDIClient;
	BOOL											m_bDetached;
	BOOL											m_bMDIChild;
	GalaxyType										m_nGalaxyType;

	HWND											m_hHostWnd;
	CString											m_strLastKey;
	CString											m_strGalaxyName;
	CString											m_strCurrentKey;
	CString											m_strCurrentXml;
	CString											m_strDocTemplateID = _T("");
	CString											m_strHostWebBrowserNodeName = _T("");
	CEclipseWnd*									m_pWorkBenchFrame;
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
	GalaxyInfo*										m_pGalaxyInfo;
	CosmosFrameWndInfo*								m_pCosmosFrameWndInfo = nullptr;
	CWormhole*										m_pWormhole = nullptr;
	map<CString, CXobj*>							m_mapXobj;
	map<HWND, CWPFView*>							m_mapWPFView;
	map<HWND, CWPFView*>							m_mapVisibleWPFView;
	CComObject<CXobjCollection>*					m_pRootNodes;

	void Lock() {}
	void Unlock() {}
	void Destroy();
	void HostPosChanged();
	HWND GetWinForm(HWND hForm);
	void UpdateVisualWPFMap(HWND, BOOL);
	CTangramXmlParse* UpdateXobj();
	BOOL Create();
	CXobj* ObserveXtmlDocument(CTangramXmlParse* pParse, CString strKey);

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
	ULONG InternalAddRef() { return 1; }
	ULONG InternalRelease() { return 1; }

private:
	LRESULT OnDpiChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnGetDPIScaledSize(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnBeforeParentDpiChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnAfterParentDpiChanged(UINT, WPARAM, LPARAM, BOOL&);
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
	STDMETHOD(get_Xobj)(VARIANT vIndex, IXobj** ppXobj);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	STDMETHOD(get_HWND)(LONGLONG* pVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_CurrentNavigateKey)(BSTR* pVal);
	STDMETHOD(get_VisibleXobj)(IXobj** pVal);
	STDMETHOD(get_RootXobjs)(IXobjCollection** pXobjColletion);
	STDMETHOD(get_GalaxyType)(GalaxyType* pVal);
	STDMETHOD(get_Name)(BSTR* pVal);
	STDMETHOD(get_HostBrowser)(IBrowser** ppChromeWebBrowser);
	STDMETHOD(get_HostWebPage)(IWebPage** ppChromeWebPage);

	STDMETHOD(Attach)(void);
	STDMETHOD(Detach)(void);

	void OnFinalMessage(HWND hWnd);
};
