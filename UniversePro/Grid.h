/********************************************************************************
*					Tangram Library - version 10.0.0							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE TANGRAM LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
*
********************************************************************************/


// Grid.h : Declaration of the CGrid

#include "GalaxyCluster.h"
#include "chromium/BrowserWnd.h"

#pragma once

class CWormhole;
class CGridShareData
{
public:
	CGridShareData();
	~CGridShareData();

	CGalaxy*		m_pGalaxy = nullptr;
	CGalaxy*		m_pOldGalaxy = nullptr;
	IDispatch*				m_pOfficeObj = nullptr;
	CGridHelper*				m_pHostClientView = nullptr;
	CTangramXmlParse*		m_pHubbleParse = nullptr;
	CGalaxyCluster*			m_pGalaxyCluster = nullptr;
	map<CString, CGrid*>	m_mapLayoutNodes;
	map<CString, CGrid*>	m_mapAxNodes;
	map<CString, CGrid*>	m_mapCLRNodes;
	map<CString, CGrid*>	m_mapCppGrids;
	CMapStringToPtr			m_PlugInDispDictionary;
};

// CGrid 
class ATL_NO_VTABLE CGrid : 
	public CComObjectRootBase,
	public IConnectionPointContainerImpl<CGrid>,
	public IConnectionPointImpl<CGrid, &__uuidof(_IGridEvents)>,
	public IDispatchImpl<IGrid, &IID_IGrid, &LIBID_Universe, 1, 0>
{
public:
	CGrid();
	virtual ~CGrid();

	BOOL							m_bTopObj;
	BOOL							m_bCreated;
	BOOL							m_bWebInit;
	BOOL							m_bNodeDocComplete;

	GridType						m_nViewType;
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
	CString 						m_strObjTypeID;
	CString 						m_strCaption;
	CString 						m_strXmlFileFromVS;

	CString							m_strLastIPCMsgID = _T("");
	CString							m_strLastIPCParam1 = _T("");
	CString							m_strLastIPCParam2 = _T("");
	CString							m_strLastIPCParam3 = _T("");
	CString							m_strLastIPCParam4 = _T("");
	CString							m_strLastIPCParam5 = _T("");

	CString							m_strNodeName;
	CString 						m_strExtenderID;

	IDispatch*						m_pDisp;
	CGrid* 							m_pRootObj;
	CGrid* 							m_pParentObj;
	CWinForm*						m_pParentWinFormWnd;
	CTangramXmlParse*				m_pHostParse;
	CTangramXmlParse* 				m_pDocXmlParseNode;
	IHubbleWindow*					m_pWindow;
	CMDIChildFormInfo*				m_pChildFormsInfo;
	CGridShareData*					m_pGridShareData;
	CBrowser*						m_pWebBrowser;
	CWnd*							m_pHostWnd;
	CGalaxy*						m_pHostGalaxy;
	CRuntimeClass*					m_pObjClsInfo;

	CGridEvents*					m_pCLREventConnector;

	VARIANT							m_varTag;
	IDispatch*						m_pExtender;
	CGridVector						m_vChildNodes;
	CGrid*							m_pCurrentExNode;
	CWormhole*						m_pHubbleCloudSession;
	map<CString, CGalaxy*>			m_mapSubFrame;
	map<CGrid*, CString>			m_mapExtendNode;
	CComObject<CGridCollection>*	m_pChildNodeCollection;

	map<IUniverseAppProxy*, CGridProxy*> m_mapWndGridProxy;

	void	InitWndGrid();
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
	BOOL	PreTranslateMessage(MSG* pMsg);
	BOOL	AddChildNode(CGrid* pGrid);
	BOOL	RemoveChildNode(CGrid* pGrid);
	CString GetNames();
	CWebPage* GetHtmlWnd();
	void NodeCreated();

	HRESULT Fire_ObserveComplete();
	HRESULT Fire_Destroy();
	HRESULT Fire_GridAddInCreated(IDispatch * pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml);
	HRESULT Fire_GridAddInsCreated();
	HRESULT Fire_GridDocumentComplete(IDispatch * ExtenderDisp, BSTR bstrURL);
	HRESULT Fire_ControlNotify(IGrid * sender, LONG NotifyCode, LONG CtrlID, LONGLONG CtrlHandle, BSTR CtrlClassName);
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
	STDMETHOD(get_ChildNodes)(IGridCollection** );
	STDMETHOD(get_Row)(long* nRow);
	STDMETHOD(get_Col)(long* nCol);
	STDMETHOD(get_Objects)(long nType, IGridCollection** );
	STDMETHOD(get_GridType)(GridType* nType);
	STDMETHOD(get_ParentGrid)(IGrid** ppGrid);
	STDMETHOD(get_RootGrid)(IGrid** ppGrid);
	STDMETHOD(get_OuterXml)(BSTR* pVal);
	STDMETHOD(get_Key)(BSTR* pVal);
	STDMETHOD(get_Galaxy)(IGalaxy** pVal);
	STDMETHOD(get_HostGalaxy)(IGalaxy** pVal);
	STDMETHOD(get_Height)(LONG* pVal);
	STDMETHOD(get_Width)(LONG* pVal);
	STDMETHOD(get_Extender)(IDispatch** pVal);
	STDMETHOD(put_Extender)(IDispatch* newVal);
	STDMETHOD(get_GalaxyCluster)(IGalaxyCluster** pVal);
	STDMETHOD(get_NameAtWindowPage)(BSTR* pVal);
	STDMETHOD(get_DocXml)(BSTR* pVal);
	STDMETHOD(get_HostGrid)(IGrid** pVal);
	STDMETHOD(put_HostGrid)(IGrid* newVal);
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

	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);
	STDMETHOD(ObserveEx)(int nRow, int nCol, BSTR bstrKey, BSTR bstrXml, IGrid** ppRetGrid);
	STDMETHOD(ActiveTabPage)(IGrid* pGrid);
	STDMETHOD(GetGrid)(long nRow, long nCol,IGrid** ppGrid);
	STDMETHOD(GetGrids)(BSTR bstrName, IGrid** ppGrid, IGridCollection** ppGrids, long* pCount);
	STDMETHOD(GetCtrlByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp);
	STDMETHOD(GetCtrlValueByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal);
	STDMETHOD(SetCtrlValueByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal);
	STDMETHOD(LoadXML)(int nType, BSTR bstrXML);
	STDMETHOD(Show)();
	STDMETHOD(GetGridByName)(BSTR bstrName, IGridCollection** pVal);
	STDMETHOD(get_DockObj)(BSTR bstrName, LONGLONG* pVal);
	STDMETHOD(put_DockObj)(BSTR bstrName, LONGLONG newVal);
	STDMETHOD(NavigateURL)(BSTR bstrURL, IDispatch* dispObjforScript);
	STDMETHOD(get_URL)(BSTR* pVal);
	STDMETHOD(put_URL)(BSTR newVal);

	STDMETHOD(SendIPCMessage)(BSTR bstrTo, BSTR bstrPayload, BSTR bstrExtra, BSTR bstrMsgId, BSTR* bstrRet);

	BEGIN_COM_MAP(CGrid)
		COM_INTERFACE_ENTRY(IGrid)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CGrid)
		CONNECTION_POINT_ENTRY(__uuidof(_IGridEvents))
	END_CONNECTION_POINT_MAP()

	HWND CreateView(HWND hParentWnd, CString strTag);

private:
	CString _GetNames(CGrid* pGrid);
	void _get_Objects(CGrid* pGrid, UINT32& nType, CGridCollection* pGridColletion);
	int _getNodes(CGrid* pGrid, CString& strName, CGrid**ppRetGrid, CGridCollection* pGrids);
};

// CGridCollection

class ATL_NO_VTABLE CGridCollection :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IGridCollection, &IID_IGridCollection, &LIBID_Universe, 1, 0>
{
public:
	CGridCollection();
	~CGridCollection();

	BEGIN_COM_MAP(CGridCollection)
		COM_INTERFACE_ENTRY(IGridCollection)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	STDMETHOD(get_GridCount)(long* pCount);
	STDMETHOD(get_Item)(long iIndex, IGrid ** ppGrid);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	CGridVector*	m_pGrids;

private:
	CGridVector	m_vGrids;
};
