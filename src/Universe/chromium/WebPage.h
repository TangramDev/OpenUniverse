/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
********************************************************************************/

#pragma once

#include "../stdafx.h"

class CWinForm;
namespace Browser
{
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
		bool								m_bWebContentVisible;

		HWND								m_hChildWnd;
		HWND								m_hExtendWnd;
		HWND								m_hWebHostWnd;

		CString								m_strURL;
		CString								m_strCurKey;
		CString								m_strCurXml;
		CString								m_strAppProxyID;

		CGalaxy*							m_pGalaxy;
		CGrid*								m_pParentGrid;
		IUniverseAppProxy*					m_pAppProxy;
		CGalaxyCluster*						m_pGalaxyCluster;

		CWebPage*							m_pWebWnd;
		CWebPage*							m_pDevToolWnd;
		map<CString, CString>				m_mapFormsInfo;
		map<HWND, CWinForm*>				m_mapWinForm;
		map<HWND, CWinForm*>				m_mapSubWinForm;

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

		void SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5);
	protected:
		ULONG InternalAddRef() { return 1; }
		ULONG InternalRelease() { return 1; }

	private:
		CString m_strDocXml;
		IGrid* GetParentGrid() { return (IGrid*)m_pParentGrid; }
		IGalaxy* GetGalaxy() { return (IGalaxy*)m_pGalaxy; }
		CString FindToken(CString pszContent, CString pszDelimiter, int& nStart);
		void LoadDocument2Viewport(CString strName, CString strXML);
		void OnFinalMessage(HWND hWnd) override;
		LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnCosmosMsg(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnParentChanged(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnChromeIPCMsgReceived(UINT, WPARAM, LPARAM, BOOL&);
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
		STDMETHOD(get_HostWnd)(LONGLONG* Val);
		STDMETHOD(put_HostWnd)(LONGLONG newVal);
		STDMETHOD(CreateForm)(BSTR bstrKey, LONGLONG hParent, IDispatch** pRetForm);
		STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IGrid** pRetGrid);
		STDMETHOD(SendXmlMessage)(IGrid* sender, BSTR bstrXml);
		CChromeBrowserBase* GetChromeBrowserBase(HWND) override;
	};
};