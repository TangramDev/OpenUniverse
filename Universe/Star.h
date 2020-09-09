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

// Star.h : Declaration of the CStar

#include "browser.h"
#include "GalaxyCluster.h"
#include "chromium/BrowserWnd.h"

#pragma once

class CWinForm;
class CWormhole;
class CStarCommonData
{
public:
	CStarCommonData();
	~CStarCommonData();

	CQuasar*					m_pQuasar;
	CQuasar*					m_pOldQuasar;
	CStarWnd*					m_pHostClientView;
	CTangramXmlParse*			m_pTangramParse;
	CGalaxyCluster*				m_pGalaxyCluster;
	map<CString, CStar*>		m_mapLayoutNodes;
	map<CString, CStar*>		m_mapAxNodes;
	map<CString, CStar*>		m_mapCLRNodes;
	map<CString, CStar*>		m_mapCppNodes;
	CMapStringToPtr				m_PlugInDispDictionary;
};

// CStar 
class ATL_NO_VTABLE CStar : 
	public CComObjectRootBase,
	public IConnectionPointContainerImpl<CStar>,
	public IConnectionPointImpl<CStar, &__uuidof(_IStarEvents)>,
	public IDispatchImpl<IStar, &IID_IStar, &LIBID_Universe, 1, 0>
{
public:
	CStar();
	virtual ~CStar();

	BOOL							m_bTopObj;
	BOOL							m_bCreated;
	BOOL							m_bWebInit;
	BOOL							m_bNodeDocComplete;

	StarType						m_nViewType;
	int								m_nID;
	int								m_nActivePage;
	int								m_nWidth;
	int								m_nHeigh;
	int								m_nRow;
	int								m_nCol;
	int								m_nRows;
	int								m_nCols;
	HWND							m_hHostWnd;
	HWND							m_hChildHostWnd;

	CString 						m_strID;
	CString 						m_strURL;
	CString							m_strKey;
	CString							m_strName;
	CString 						m_strCnnID;
	CString 						m_strCaption;

	CString							m_strLastIPCMsgID = _T("");
	CString							m_strLastIPCParam1 = _T("");
	CString							m_strLastIPCParam2 = _T("");
	CString							m_strLastIPCParam3 = _T("");
	CString							m_strLastIPCParam4 = _T("");
	CString							m_strLastIPCParam5 = _T("");

	CString							m_strNodeName;
	CString 						m_strExtenderID;

	IDispatch*						m_pDisp;
	CStar* 							m_pRootObj;
	CStar* 							m_pParentObj;
	CStar*							m_pVisibleXMLObj;
	CWinForm*						m_pParentWinFormWnd;
	CTangramXmlParse*				m_pHostParse;
	CTangramXmlParse* 				m_pDocXmlParseNode;
	CSession*						m_pCloudSession = nullptr;
	IHubbleWindow*					m_pWindow;
	CMDIChildFormInfo*				m_pChildFormsInfo;
	CStarCommonData*				m_pStarCommonData;
	ChromePlus::CHerschel*			m_pWebBrowser;
	CWnd*							m_pHostWnd;
	CQuasar*						m_pHostQuasar;
	CRuntimeClass*					m_pObjClsInfo;

	CStarEvents*					m_pCLREventConnector;

	VARIANT							m_varTag;
	IDispatch*						m_pExtender;
	CStarVector						m_vChildNodes;
	CStar*							m_pCurrentExNode;
	CWormhole*						m_pTangramCloudSession;
	map<CString, CQuasar*>			m_mapSubFrame;
	map<CStar*, CString>			m_mapExtendNode;
	CComObject<CStarCollection>*	m_pChildNodeCollection;

	map<IHubbleAppProxy*, CTangramWndNodeProxy*> m_mapWndNodeProxy;

	void	InitWndNode();
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
	BOOL	PreTranslateMessage(MSG* pMsg);
	BOOL	AddChildNode(CStar* pNode);
	BOOL	RemoveChildNode(CStar* pNode);
	CString GetNames();
	ChromePlus::CGalileo* GetHtmlWnd();
	void NodeCreated();

	HRESULT Fire_OpenComplete();
	HRESULT Fire_Destroy();
	HRESULT Fire_NodeAddInCreated(IDispatch * pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml);
	HRESULT Fire_NodeAddInsCreated();
	HRESULT Fire_NodeDocumentComplete(IDispatch * ExtenderDisp, BSTR bstrURL);
	HRESULT Fire_ControlNotify(IStar * sender, LONG NotifyCode, LONG CtrlID, LONGLONG CtrlHandle, BSTR CtrlClassName);
	HRESULT Fire_TabChange(LONG ActivePage, LONG OldPage);
	HRESULT Fire_IPCMessageReceived(BSTR bstrFrom, BSTR bstrTo, BSTR bstrMsgId, BSTR bstrPayload, BSTR bstrExtra);

	void Lock(){}
	void Unlock(){}
protected:
	ULONG InternalAddRef(){ return 1; }
	ULONG InternalRelease(){ return 1; }

public:
	STDMETHOD(get_XObject)(VARIANT* pVar);
	STDMETHOD(get_Tag)(VARIANT* pVar);
	STDMETHOD(put_Tag)(VARIANT var);
	STDMETHOD(get_AxPlugIn)(BSTR bstrName, IDispatch** pVal);
	STDMETHOD(get_Name)(BSTR* pVal);
	STDMETHOD(put_Name)(BSTR bstrName);
	STDMETHOD(get_Caption)(BSTR* pVal);
	STDMETHOD(put_Caption)(BSTR bstrCaption);
	STDMETHOD(get_Attribute)(BSTR bstrKey, BSTR* pVal);
	STDMETHOD(put_Attribute)(BSTR bstrKey, BSTR bstrVal);
	STDMETHOD(get_Handle)(LONGLONG* hWnd);
	STDMETHOD(get_XML)(BSTR* pVal);
	STDMETHOD(get_ChildNodes)(IStarCollection** );
	STDMETHOD(get_Row)(long* nRow);
	STDMETHOD(get_Col)(long* nCol);
	STDMETHOD(get_Objects)(long nType, IStarCollection** );
	STDMETHOD(get_StarType)(StarType* nType);
	STDMETHOD(get_ParentStar)(IStar** ppNode);
	STDMETHOD(get_RootStar)(IStar** ppNode);
	STDMETHOD(get_OuterXml)(BSTR* pVal);
	STDMETHOD(get_Key)(BSTR* pVal);
	STDMETHOD(get_Quasar)(IQuasar** pVal);
	STDMETHOD(get_HostQuasar)(IQuasar** pVal);
	STDMETHOD(get_Height)(LONG* pVal);
	STDMETHOD(get_Width)(LONG* pVal);
	STDMETHOD(get_Extender)(IDispatch** pVal);
	STDMETHOD(put_Extender)(IDispatch* newVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_NameAtWindowPage)(BSTR* pVal);
	STDMETHOD(get_DocXml)(BSTR* pVal);
	STDMETHOD(get_HostStar)(IStar** pVal);
	STDMETHOD(put_HostStar)(IStar* newVal);
	STDMETHOD(get_ActivePage)(int* pVal);
	STDMETHOD(put_ActivePage)(int newVal);
	STDMETHOD(get_OfficeObj)(IDispatch** pVal);

	STDMETHOD(get_Rows)(long* nRows);
	STDMETHOD(get_Cols)(long* nCols);
	STDMETHOD(get_Hmin)(int* pVal);
	STDMETHOD(put_Hmin)(int newVal);
	STDMETHOD(get_Hmax)(int* pVal);
	STDMETHOD(put_Hmax)(int newVal);
	STDMETHOD(get_Vmin)(int* pVal);
	STDMETHOD(put_Vmin)(int newVal);
	STDMETHOD(get_Vmax)(int* pVal);
	STDMETHOD(put_Vmax)(int newVal);
	STDMETHOD(get_rgbMiddle)(OLE_COLOR* pVal);
	STDMETHOD(put_rgbMiddle)(OLE_COLOR newVal);
	STDMETHOD(get_rgbRightBottom)(OLE_COLOR* pVal);
	STDMETHOD(put_rgbRightBottom)(OLE_COLOR newVal);
	STDMETHOD(get_rgbLeftTop)(OLE_COLOR* pVal);
	STDMETHOD(put_rgbLeftTop)(OLE_COLOR newVal);
	STDMETHOD(get_MasterRow)(int* pVal);
	STDMETHOD(put_MasterRow)(int newVal);
	STDMETHOD(get_MasterCol)(int* pVal);
	STDMETHOD(put_MasterCol)(int newVal);
	STDMETHOD(put_SaveToConfigFile)(VARIANT_BOOL newVal);

	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IStar** ppRetNode);
	STDMETHOD(ObserveEx)(int nRow, int nCol, BSTR bstrKey, BSTR bstrXml, IStar** ppRetNode);
	STDMETHOD(ActiveTabPage)(IStar* pNode);
	STDMETHOD(GetStar)(long nRow, long nCol,IStar** ppTangramNode);
	STDMETHOD(GetStars)(BSTR bstrName, IStar** ppNode, IStarCollection** ppNodes, long* pCount);
	STDMETHOD(GetCtrlByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp);
	STDMETHOD(GetCtrlValueByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal);
	STDMETHOD(SetCtrlValueByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal);
	STDMETHOD(Refresh)(void);
	STDMETHOD(LoadXML)(int nType, BSTR bstrXML);
	STDMETHOD(Show)();
	STDMETHOD(GetStarByName)(BSTR bstrName, IStarCollection** pVal);
	STDMETHOD(get_DockObj)(BSTR bstrName, LONGLONG* pVal);
	STDMETHOD(put_DockObj)(BSTR bstrName, LONGLONG newVal);
	STDMETHOD(NavigateURL)(BSTR bstrURL, IDispatch* dispObjforScript);
	STDMETHOD(get_URL)(BSTR* pVal);
	STDMETHOD(put_URL)(BSTR newVal);

	STDMETHOD(SendIPCMessage)(BSTR bstrTo, BSTR bstrPayload, BSTR bstrExtra, BSTR bstrMsgId, BSTR* bstrRet);

	BEGIN_COM_MAP(CStar)
		COM_INTERFACE_ENTRY(IStar)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CStar)
		CONNECTION_POINT_ENTRY(__uuidof(_IStarEvents))
	END_CONNECTION_POINT_MAP()

	HWND CreateView(HWND hParentWnd, CString strTag);

private:
	CString _GetNames(CStar* pNode);
	void _get_Objects(CStar* pNode, UINT32& nType, CStarCollection* pNodeColletion);
	int _getNodes(CStar* pNode, CString& strName, CStar**ppRetNode, CStarCollection* pNodes);
};

// CStarCollection

class ATL_NO_VTABLE CStarCollection :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IStarCollection, &IID_IStarCollection, &LIBID_Universe, 1, 0>
{
public:
	CStarCollection();
	~CStarCollection();

	BEGIN_COM_MAP(CStarCollection)
		COM_INTERFACE_ENTRY(IStarCollection)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	STDMETHOD(get_StarCount)(long* pCount);
	STDMETHOD(get_Item)(long iIndex, IStar **ppTangramNode);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	CStarVector*	m_pNodes;

private:
	CStarVector	m_vNodes;
};
