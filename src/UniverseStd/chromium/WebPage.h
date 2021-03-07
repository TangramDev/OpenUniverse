/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103070045           *
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
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

#pragma once

#include "../stdafx.h"

class CWinForm;
namespace Browser
{
	class CExtendWnd : public CWindowImpl<CExtendWnd, CWindow>
	{
	public:
		CExtendWnd(void);
		BEGIN_MSG_MAP(CExtendWnd)
			MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		END_MSG_MAP()
		CWebPage* m_pHostPage = nullptr;
	private:
		void OnFinalMessage(HWND hWnd);
	public:
		LRESULT OnEraseBkgnd(UINT, WPARAM, LPARAM, BOOL&);
	};

	class ATL_NO_VTABLE CWebPage :
		public CWebPageImpl,
		public CWindowImpl<CWebPage, CWindow>,
		public CComObjectRootEx<CComSingleThreadModel>,
		public IDispatchImpl<IWebPage, &_uuidof(IWebPage), &LIBID_Universe, 1, 0>
	{
	public:
		CWebPage();
		~CWebPage() override;

		BOOL								m_bDevToolWnd;

		HWND								m_hChildWnd;
		HWND								m_hExtendWnd;
		HWND								m_hWebHostWnd;

		CString								m_strURL;
		CString								m_strCurKey;
		CString								m_strCurXml;
		CString								m_strAppProxyID;
		CString								m_strPageName = _T("");

		CGalaxy*							m_pGalaxy;
		CXobj*								m_pParentXobj;
		CXobj*								m_pBindXobj;
		CWinForm*							m_pBindWinForm;
		IUniverseAppProxy*					m_pAppProxy;
		CGalaxyCluster*						m_pGalaxyCluster;
		CosmosFrameWndInfo*					m_pCosmosFrameWndInfo = nullptr;

		CWebPage*							m_pWebWnd;
		CWebPage*							m_pDevToolWnd;
		map<CString, CString>				m_mapFormsInfo;
		map<CString, CString>				m_mapeclipsesInfo;
		map<CString, CString>				m_mapWorkBenchInfo;
		map<HWND, CWinForm*>				m_mapWinForm;
		map<HWND, CWinForm*>				m_mapSubWinForm;
		map<CString, CMDIChildFormInfo*>	m_mapChildFormsInfo;

		BEGIN_MSG_MAP(CWebPage)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
			MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
			MESSAGE_HANDLER(WM_COSMOSMSG, OnCosmosMsg)
			MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
			MESSAGE_HANDLER(WM_CHROMEIPCMSG, OnChromeIPCMsgReceived)
			MESSAGE_HANDLER(WM_CHROMEWNDPARENTCHANGED, OnParentChanged)
		END_MSG_MAP()

		BEGIN_COM_MAP(CWebPage)
			COM_INTERFACE_ENTRY(IDispatch)
			COM_INTERFACE_ENTRY(IWebPage)
		END_COM_MAP()

		STDMETHOD(get_HostWnd)(LONGLONG* Val);
		STDMETHOD(put_HostWnd)(LONGLONG newVal);
		STDMETHOD(CreateForm)(BSTR bstrKey, LONGLONG hParent, IDispatch** pRetForm);
		STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IXobj** pRetXobj);
		STDMETHOD(SendXmlMessage)(IXobj* sender, BSTR bstrXml);

		void SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5);
		CChromeBrowserBase* GetChromeBrowserBase(HWND) override;

		void LoadDocument2Viewport(CString strName, CString strXML);

		CString FindToken(CString pszContent, CString pszDelimiter, int& nStart);

		void HandleChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5);
		void HandleAggregatedMessage(CString strParam1, CString strParam2);
		void HandleSingleMessage(CString strParam);
		void CustomizedDOMElement(CString strRuleName, CString strHTML);
		void CustomizedMainWindowElement(CString strHTML);
		void CustomizedIndWindowElement(CString strHTML);
		void CustomizedWebBrowserElement(CString strHTML);
		void CustomizedNodeDetailsElement(CString strHTML);
		void CustomizedObjectElement(CString strHTML);
		void CustomizedExtraElement(CString strHTML);
		void CustomizedDataElement(CString strHTML);
		void CustomizedDocElement(CString strHTML);
		void OnNTPLoaded();
		void OnWinFormCreated(HWND);
		void OnCloudMsgReceived(CSession*);

	protected:
		ULONG InternalAddRef() { return 1; }
		ULONG InternalRelease() { return 1; }

	private:
		CString m_strDocXml;
		IXobj* GetParentXobj() { return (IXobj*)m_pParentXobj; }
		IGalaxy* GetGalaxy() { return (IGalaxy*)m_pGalaxy; }
		void Show(CString strID);
		void OnFinalMessage(HWND hWnd) override;
		LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnCosmosMsg(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnParentChanged(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnChromeIPCMsgReceived(UINT, WPARAM, LPARAM, BOOL&);
	};
};