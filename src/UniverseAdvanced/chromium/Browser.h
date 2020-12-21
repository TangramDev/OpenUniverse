/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
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

namespace Browser {
	using namespace ATL;

	class CWebPage;

	class ATL_NO_VTABLE CBrowser :
		public CBrowserImpl, 
		public CWindowImpl<CBrowser, CWindow>,
		public CComObjectRootEx<CComSingleThreadModel>,
		public IDispatchImpl<IBrowser, &_uuidof(IBrowser), &LIBID_World, 1, 0>
	{
		friend class CXobj;
	public:
		CBrowser();
		~CBrowser() override;

		bool		m_bTabChange;
		int			m_heightfix;
		float		m_fdevice_scale_factor;
		HWND		m_hDrawWnd;
		HWND		m_hOldTab;
		CString		m_strCurKey;
		CXobj*		m_pParentGrid;
		CWebPage*	m_pVisibleWebWnd;
		IXobj*		m_pRemoteGrid;
		LRESULT		BrowserLayout();

		map<HWND, CWebPage*> m_mapNorifyPage;

		STDMETHOD(get_RemoteGrid)(IXobj** pVal);
		STDMETHOD(put_RemoteGrid)(IXobj* newVal);
		STDMETHOD(AddURLs)(BSTR bstrURLs);
		STDMETHOD(OpenURL)(BSTR bstrURL, BrowserWndOpenDisposition nDisposition, BSTR bstrKey, BSTR bstrXml);
		BEGIN_MSG_MAP(CBrowser)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
			MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
			MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
			MESSAGE_HANDLER(WM_TABCHANGE, OnChromeTabChange)
			MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
			MESSAGE_HANDLER(WM_BROWSERLAYOUT, OnBrowserLayout)
			MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
			MESSAGE_HANDLER(WM_DEVICESCALEFACTORCHANGED, OnDeviceScaleFactorChanged)
		END_MSG_MAP()

		BEGIN_COM_MAP(CBrowser)
			COM_INTERFACE_ENTRY(IBrowser)
			COM_INTERFACE_ENTRY(IDispatch)
		END_COM_MAP()
	protected:
		ULONG InternalAddRef() { return 1; }
		ULONG InternalRelease() { return 1; }
	private:
		void UpdateContentRect(HWND hContentWnd, RECT& rc, int nTopFix) override;
		void ActiveChromeTab(HWND hActive, HWND hOldWnd) override;

		void OnFinalMessage(HWND hWnd) override;
		LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnCosmosMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnBrowserLayout(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnChromeTabChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnMouseActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnWindowPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnDeviceScaleFactorChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	};
};