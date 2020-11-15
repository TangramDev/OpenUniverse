/********************************************************************************
*					Open Universe - version 1.0.1.15							*
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
		MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMg)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanging)
	END_MSG_MAP()

private:
	void OnFinalMessage(HWND hWnd);
	LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnHubbleMg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CHubbleDocTemplate;
class CUniverseMDIChild;
class CUniverseMDIMain :
	public CWindowImpl<CUniverseMDIMain, CWindow>
{
public:
	CUniverseMDIMain(void);
	virtual ~CUniverseMDIMain(void);

	HWND									m_hMDIClient;
	CGalaxyCluster*								m_pGalaxyCluster;
	CHubbleDocTemplate*					m_pDocTemplate;
	map<HWND, CString>						m_mapDesignableWnd;
	map<CString, CHubbleDocTemplate*>		m_mapHubbleDocTemplate;
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

	CHubbleDocTemplate* m_pDocTemplate;
	BEGIN_MSG_MAP(CUniverseMDIChild)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_DPICHANGED, OnDpiChanged)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
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
	LRESULT OnHubbleMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
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
	CGrid*									m_pBindMDIGrid = nullptr;
	CGrid*									m_pWebBindMDIGrid = nullptr;
	CWebPage*								m_pOwnerHtmlWnd;
	CWormhole*								m_pWormhole;
	CMDIChildFormInfo*						m_pChildFormsInfo;
	map<CString, BindWebObj*>				m_mapBindWebObj;

	map<CString, CString>					m_mapKey;
	void SendMessage();

	//map<CString, HubbleDocTemplateInfo*>	m_mapHubbleFormsTemplateInfo;
	//map<int, HubbleDocTemplateInfo*>		m_mapHubbleFormsTemplateInfo2;
	BEGIN_MSG_MAP(CWinForm)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
		MESSAGE_HANDLER(WM_HUBBLE_DATA, OnGetMe)
		MESSAGE_HANDLER(WM_DPICHANGED, OnDpiChanged)
		MESSAGE_HANDLER(WM_GETDPISCALEDSIZE, OnGetDPIScaledSize)
		MESSAGE_HANDLER(WM_WINFORMCREATED, OnFormCreated)
		MESSAGE_HANDLER(WM_HUBBLE_GETXML, OnHubbleGetXml)
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
	LRESULT OnHubbleMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnHubbleGetXml(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnGetDPIScaledSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnMdiClientCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL&);
	LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CHubbleDocWnd :
	public CWindowImpl<CHubbleDocWnd, CWindow>
{
public:
	CHubbleDocWnd(void);
	virtual ~CHubbleDocWnd(void);

	HWND m_hView;
	CString	m_strKey;
	CString m_strWndID;
	CGalaxy* m_pGalaxy;
	CGalaxyCluster* m_pParentPage;
	CHubbleDocFrame* m_pDocFrame;

	map<CString, HWND> m_mapCtrlBar;
	BEGIN_MSG_MAP(CHubbleDocWnd)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
		MESSAGE_HANDLER(WM_HUBBLE_GETXML, OnHubbleGetXml)
		MESSAGE_HANDLER(WM_CONTROLBARCREATED, OnCtrlBarCreated)
	END_MSG_MAP()

	void OnFinalMessage(HWND hWnd);

private:
	void CtrlBarDocActiveNotify(IHubbleDoc* pDoc, IGrid* pGridInDoc, IGrid* pGridInCtrlBar, HWND hCtrlBar);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnHubbleMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
	LRESULT OnHubbleGetXml(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& );
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
	CHubbleDocTemplate*								m_pHubbleDocTemplate;
	map<IUniverseAppProxy*, CGalaxyProxy*>			m_mapGalaxyProxy;

	IPCMsg*											m_pCurrentIPCMsg;
	CBKWnd*											m_pBKWnd;
	CWebPage*										m_pWebPageWnd;
	CGrid*											m_pHostWebBrowserNode = nullptr;
	CBrowser*										m_pHostWebBrowserWnd = nullptr;
	CGalaxyCluster*									m_pGalaxyCluster;
	CGrid*											m_pParentGrid;
	CGrid*											m_pWorkGrid;
	CGrid*											m_pContainerNode;
	CGrid*											m_pBindingGrid;
	CGalaxy*										m_pSubGalaxy;
	GalaxyInfo*										m_pGalaxyInfo;
	CHubbleDoc*										m_pDoc;
	map<CString, CGrid*>							m_mapGrid;
	map<CString, CGrid*>							m_mapNeedSaveToConfigNode;
	map<CString, VARIANT>							m_mapVal;
	map<HWND, CWPFView*>							m_mapWPFView;
	map<HWND, CWPFView*>							m_mapVisibleWPFView;
	map<CString, CGrid*>							m_mapGridScript;
	CComObject<CGridCollection>*					m_pRootNodes;

	void Lock(){}
	void Unlock(){}
	void Destroy();
	void HostPosChanged();
	HWND GetWinForm(HWND hWnd);
	void UpdateDesignerTreeInfo();
	void UpdateVisualWPFMap(HWND, BOOL);

	CTangramXmlParse* UpdateGrid();
	BOOL CreateGalaxyCluster();
	CGrid* ObserveXtmlDocument(CTangramXmlParse* pParse, CString strKey, CString	strFile);

	STDMETHOD(get_GalaxyXML)(BSTR* pVal);
	STDMETHOD(ModifyHost)(LONGLONG hHostWnd);
	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);
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
		MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
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
	LRESULT OnHubbleMsg(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnParentNotify(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnQueryAppProxy(UINT, WPARAM, LPARAM, BOOL&);
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
