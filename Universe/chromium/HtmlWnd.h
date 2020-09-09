/********************************************************************************
*					Open Universe - version 0.0.1								*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
********************************************************************************/

#pragma once

#include "../stdafx.h"

class CWinForm;
namespace ChromePlus
{
	/*About CGalileo:
	* Galileo Galileo (1564-1642) was a great astronomer, mechanist, philosopher, 
	* physicist and mathematician in the late Renaissance of Italy. He is also the
	* pioneer of modern experimental physics, known as "the father of modern science". 
	* At the age of 17, he entered the University of Pisa to study medicine. Later, 
	* he turned to mathematics and became a university professor after graduation. 
	* He is an indomitable fighter for the sake of truth. Engels called him "one of 
	* the giants who can break down the old theory and create the new one regardless 
	* of any obstacles.". He was born in Pisa on February 15, 1564 and died in Pisa 
	* on January 8, 1642. The family name of Galileo is Galilei, and his full name is
	* Galileo Galilei
	*/
	class ATL_NO_VTABLE CGalileo :
		public CGalileoImpl,
		public CWindowImpl<CGalileo, CWindow>,
		public CComObjectRootEx<CComSingleThreadModel>,
		public IDispatchImpl<IWebPage, &_uuidof(IWebPage), &LIBID_Universe, 1, 0>
	{
	public:
		CGalileo();
		~CGalileo() override;

		BOOL								m_bDevToolWnd;
		bool								m_bWebContentVisible;

		HWND								m_hChildWnd;
		HWND								m_hExtendWnd;
		HWND								m_hWebHostWnd;

		CString								m_strURL;
		CString								m_strCurKey;
		CString								m_strCurXml;
		CString								m_strAppProxyID;

		CQuasar*							m_pQuasar;
		CStar*								m_pParentStar;
		CStar*								m_pBindStar;
		CWinForm*							m_pBindWinForm;
		IHubbleAppProxy*					m_pAppProxy;
		CGalaxyCluster*						m_pGalaxyCluster;

		CGalileo*							m_pWebWnd;
		CGalileo*							m_pDevToolWnd;
		map<CString, CString>				m_mapFormsInfo;
		map<CString, CString>				m_mapUserControlsInfo;
		map<HWND, CWinForm*>				m_mapWinForm;
		map<HWND, CWinForm*>				m_mapSubWinForm;
		map<CString, CMDIChildFormInfo*>	m_mapChildFormsInfo;

		BEGIN_MSG_MAP(CGalileo)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
			MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
			MESSAGE_HANDLER(WM_COSMOSMSG, OnHubbleMsg)
			MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
			MESSAGE_HANDLER(WM_CHROMEIPCMSG, OnChromeIPCMsgReceived)
			MESSAGE_HANDLER(WM_CHROMEWNDPARENTCHANGED, OnParentChanged)
		END_MSG_MAP()

		BEGIN_COM_MAP(CGalileo)
			COM_INTERFACE_ENTRY(IDispatch)
			COM_INTERFACE_ENTRY(IWebPage)
		END_COM_MAP()

		void SendChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5);
	protected:
		ULONG InternalAddRef() { return 1; }
		ULONG InternalRelease() { return 1; }

	private:
		CString m_strDocXml;
		IStar* GetParentNode() { return (IStar*)m_pParentStar; }
		IQuasar* GetQuasar() { return (IQuasar*)m_pQuasar; }
		CString FindToken(CString pszContent, CString pszDelimiter, int& nStart);
		void LoadDocument2Viewport(CString strName, CString strXML);
		void OnFinalMessage(HWND hWnd) override;
		LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnShowWindow(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnHubbleMsg(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnMouseActivate(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnParentChanged(UINT, WPARAM, LPARAM, BOOL&);
		LRESULT OnChromeIPCMsgReceived(UINT, WPARAM, LPARAM, BOOL&);
		void HandleChromeIPCMessage(CString strId, CString strParam1, CString strParam2, CString strParam3, CString strParam4, CString strParam5);
		void HandleAggregatedMessage(CString strParam1, CString strParam2);
		void HandleSingleMessage(CString strParam);
		void RenderHTMLElement(CString strRuleName, CString strHTML);
		void RenderHTMLMainWindowElement(CString strHTML);
		void RenderHTMLIndWindowElement(CString strHTML);
		void RenderHTMLWebBrowserElement(CString strHTML);
		void RenderHTMLNodeDetailsElement(CString strHTML);
		void RenderHTMLObjectElement(CString strHTML);
		void RenderHTMLExtraElement(CString strHTML);
		void RenderHTMLDataElement(CString strHTML);
		void RenderHTMLDocElement(CString strHTML);
		void OnNTPLoaded();
		void OnWinFormCreated(HWND);
		void OnCloudMsgReceived(CSession*);
		STDMETHOD(get_HostWnd)(LONGLONG* Val);
		STDMETHOD(put_HostWnd)(LONGLONG newVal);
		STDMETHOD(CreateForm)(BSTR bstrKey, LONGLONG hParent, IDispatch** pRetForm);
		CChromeBrowserBase* GetChromeBrowserBase(HWND) override;
	};
};