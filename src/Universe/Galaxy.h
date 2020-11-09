/********************************************************************************
*					Open Universe - version 1.0.1.13							*
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

#pragma once
#include "chromium/WebPage.h"

using namespace Browser;
#define WM_HUBBLE_NOTIFY WM_NOTIFY+WM_REFLECT_BASE

class CWinForm :
	public CWindowImpl<CWinForm, CWindow>
{
public:
	CWinForm(void);
	virtual ~CWinForm(void);
	int										m_nState;
	BOOL									m_bMdiForm;
	BOOL									m_bMainForm = false;
	CString									m_strKey;
	CString									m_strXml;
	CString									m_strPath;
	
	CWebPage*								m_pOwnerHtmlWnd;
	CWebPage*								m_pParentHtmlWnd;

	map<CString, CString>					m_mapKey;
	map<CString, BindWebObj*>				m_mapBindWebObj;
	BEGIN_MSG_MAP(CWinForm)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_HUBBLE_DATA, OnGetMe)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_WINFORMCREATED, OnFormCreated)
		MESSAGE_HANDLER(WM_HUBBLE_GETXML, OnHubbleGetXml)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
	END_MSG_MAP()

	void OnFinalMessage(HWND hWnd);

private:
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnGetMe(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnFormCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnHubbleGetXml(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class ATL_NO_VTABLE CGalaxy : 
	public CComObjectRootBase,	
	public CWindowImpl<CGalaxy, CWindow>,
	public IDispatchImpl<IGalaxy, &IID_IGalaxy>
{
public:
	CGalaxy();           
	virtual ~CGalaxy();           
	bool											m_bObserve = false;
	BOOL											m_bDetached;
	BOOL											m_bMDIChild;
	BOOL											m_bDesignerState;
	GalaxyType										m_nGalaxyType;

	HWND											m_hPWnd;
	HWND											m_hHostWnd;
	CString											m_strLastKey;
	CString											m_strGalaxyName;
	CString											m_strCurrentKey;
	CString											m_strCurrentXml;
	CString											m_strHostWebBrowserNodeName = _T("");

	IPCMsg*											m_pCurrentIPCMsg;
	CWebPage*										m_pWebPageWnd;
	CGrid*											m_pHostWebBrowserNode = nullptr;
	CBrowser*										m_pHostWebBrowserWnd = nullptr;
	CGalaxyCluster*									m_pGalaxyCluster;
	CGrid*											m_pParentGrid;
	CGrid*											m_pWorkGrid;
	CGrid*											m_pContainerNode;
	CGrid*											m_pBindingGrid;
	GalaxyInfo*										m_pGalaxyInfo;
	map<CString, CGrid*>							m_mapGrid;
	CComObject<CGridCollection>*					m_pRootNodes;

	void Lock(){}
	void Unlock(){}
	void Destroy();
	void HostPosChanged();

	BOOL CreateGalaxyCluster();
	CGrid* OpenXtmlDocument(CTangramXmlParse* pParse, CString strKey, CString	strFile);

	STDMETHOD(get_GalaxyXML)(BSTR* pVal);
	STDMETHOD(ModifyHost)(LONGLONG hHostWnd);
	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);
	STDMETHOD(GetXml)(BSTR bstrRootName, BSTR* bstrRet);

	BEGIN_COM_MAP(CGalaxy)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IGalaxy)
	END_COM_MAP()

	BEGIN_MSG_MAP(CGalaxy)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_HUBBLE_DATA, OnGetMe)
		MESSAGE_HANDLER(WM_NCDESTROY, OnNcDestroy)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
		MESSAGE_HANDLER(WM_PARENTNOTIFY, OnParentNotify)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_HUBBLE_ACTIVEPAGE, OnTabChanged)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
	END_MSG_MAP()

protected:
	ULONG InternalAddRef(){ return 1; }
	ULONG InternalRelease(){ return 1; }

private:
	LRESULT OnGetMe(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnNcDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnTabChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnHubbleMsg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnParentNotify(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnWindowPosChanging(UINT, WPARAM, LPARAM, BOOL&);

	STDMETHOD(get_Count)(long* pCount);
	STDMETHOD(get_Grid)(VARIANT vIndex, IGrid **ppGrid);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	STDMETHOD(get_HWND)(LONGLONG* pVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_CurrentNavigateKey)(BSTR* pVal);
	STDMETHOD(get_VisibleGrid)(IGrid** pVal);
	STDMETHOD(get_RootGrids)(IGridCollection** pGridColletion);
	STDMETHOD(get_GalaxyData)(BSTR bstrKey, VARIANT* pVal);
	STDMETHOD(put_GalaxyData)(BSTR bstrKey, VARIANT newVal);
	STDMETHOD(get_DesignerState)(VARIANT_BOOL* pVal);
	STDMETHOD(put_DesignerState)(VARIANT_BOOL newVal);
	STDMETHOD(get_HubbleDoc)(IHubbleDoc** pVal);
	STDMETHOD(get_GalaxyType)(GalaxyType* pVal);
	STDMETHOD(get_Name)(BSTR* pVal);
	STDMETHOD(get_HostBrowser)(IBrowser** ppChromeWebBrowser);
	STDMETHOD(get_HostWebPage)(IWebPage** ppChromeWebPage);

	STDMETHOD(Attach)(void);
	STDMETHOD(Detach)(void);

	void OnFinalMessage(HWND hWnd);
};
