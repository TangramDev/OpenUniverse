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


// Grid.h : Declaration of the CXobj

#include "GalaxyCluster.h"
#include "chromium/Browser.h"

#pragma once

class CWormhole;
class CXobjShareData
{
public:
	CXobjShareData();
	~CXobjShareData();

	CGalaxy*				m_pGalaxy = nullptr;
	CGalaxy*				m_pOldGalaxy = nullptr;
	IDispatch*				m_pOfficeObj = nullptr;
	CXobjHelper*			m_pHostClientView = nullptr;
	CTangramXmlParse*		m_pCosmosParse = nullptr;
	CGalaxyCluster*			m_pGalaxyCluster = nullptr;
	map<CString, CXobj*>	m_mapLayoutNodes;
	map<CString, CXobj*>	m_mapAxNodes;
	map<CString, CXobj*>	m_mapCLRNodes;
	map<CString, CXobj*>	m_mapCppGrids;
	CMapStringToPtr			m_PlugInDispDictionary;
};

// CXobj 
class ATL_NO_VTABLE CXobj : 
	public CComObjectRootBase,
	public IConnectionPointContainerImpl<CXobj>,
	public IConnectionPointImpl<CXobj, &__uuidof(_IXobjEvents)>,
	public IDispatchImpl<IXobj, &IID_IXobj, &LIBID_World, 1, 0>
{
public:
	CXobj();
	virtual ~CXobj();

	BOOL							m_bTopObj;
	BOOL							m_bCreated;
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
	CString 						m_strCosmosXml = _T("");
	CString 						m_strXmlFileFromVS;
	CString 						m_strXmlRefGridInfo=_T("");

	CString							m_strLastIPCMsgID = _T("");
	CString							m_strLastIPCParam1 = _T("");
	CString							m_strLastIPCParam2 = _T("");
	CString							m_strLastIPCParam3 = _T("");
	CString							m_strLastIPCParam4 = _T("");
	CString							m_strLastIPCParam5 = _T("");

	CString							m_strNodeName;
	CString 						m_strExtenderID;

	IDispatch*						m_pDisp;
	CXobj* 							m_pRootObj;
	CXobj* 							m_pParentObj;
	CWinForm*						m_pParentWinFormWnd;
	CTangramXmlParse*				m_pHostParse;
	CTangramXmlParse* 				m_pDocXmlParseNode;
	ICosmosWindow*					m_pWindow;
	CMDIChildFormInfo*				m_pChildFormsInfo;
	CXobjShareData*					m_pXobjShareData;
	CBrowser*						m_pWebBrowser;
	CWnd*							m_pHostWnd;
	CGalaxy*						m_pHostGalaxy;
	CRuntimeClass*					m_pObjClsInfo;

	CXobjEvents*					m_pCLREventConnector;

	VARIANT							m_varTag;
	IDispatch*						m_pExtender;
	CGridVector						m_vChildNodes;
	CXobj*							m_pCurrentExNode;
	CWebPage*						m_pWebPage = nullptr;
	CWormhole*						m_pCosmosCloudSession;
	map<CString, CGalaxy*>			m_mapSubFrame;
	map<CXobj*, CString>			m_mapExtendNode;
	map<CString, CXobj*>			m_mapChildGrid;
	CComObject<CXobjCollection>*	m_pChildNodeCollection;

	map<IUniverseAppProxy*, CGridProxy*> m_mapWndGridProxy;

	void	InitWndGrid();
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
	BOOL	PreTranslateMessage(MSG* pMsg);
	BOOL	AddChildNode(CXobj* pXobj);
	BOOL	RemoveChildNode(CXobj* pXobj);
	CString GetNames();
	CWebPage* GetHtmlWnd();
	void NodeCreated();

	HRESULT Fire_ObserveComplete();
	HRESULT Fire_Destroy();
	HRESULT Fire_GridAddInCreated(IDispatch * pAddIndisp, BSTR bstrAddInID, BSTR bstrAddInXml);
	HRESULT Fire_GridAddInsCreated();
	HRESULT Fire_GridDocumentComplete(IDispatch * ExtenderDisp, BSTR bstrURL);
	HRESULT Fire_ControlNotify(IXobj * sender, LONG NotifyCode, LONG CtrlID, LONGLONG CtrlHandle, BSTR CtrlClassName);
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
	STDMETHOD(get_ChildNodes)(IXobjCollection** );
	STDMETHOD(get_Row)(long* nRow);
	STDMETHOD(get_Col)(long* nCol);
	STDMETHOD(get_Objects)(long nType, IXobjCollection** );
	STDMETHOD(get_GridType)(GridType* nType);
	STDMETHOD(get_ParentGrid)(IXobj** ppGrid);
	STDMETHOD(get_RootGrid)(IXobj** ppGrid);
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
	STDMETHOD(get_HostGrid)(IXobj** pVal);
	STDMETHOD(put_HostGrid)(IXobj* newVal);
	STDMETHOD(get_ActivePage)(int* pVal);
	STDMETHOD(put_ActivePage)(int newVal);
	STDMETHOD(get_OfficeObj)(IDispatch** pVal);
	STDMETHOD(get_WebPage)(IWebPage** pVal);

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
	STDMETHOD(get_URL)(BSTR* pVal);
	STDMETHOD(put_URL)(BSTR newVal);

	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IXobj** ppRetGrid);
	STDMETHOD(ObserveEx)(int nRow, int nCol, BSTR bstrKey, BSTR bstrXml, IXobj** ppRetGrid);
	STDMETHOD(ActiveTabPage)(IXobj* pXobj);
	STDMETHOD(GetGrid)(long nRow, long nCol,IXobj** ppGrid);
	STDMETHOD(GetGrids)(BSTR bstrName, IXobj** ppGrid, IXobjCollection** ppGrids, long* pCount);
	STDMETHOD(GetCtrlByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp);
	STDMETHOD(GetCtrlValueByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal);
	STDMETHOD(SetCtrlValueByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal);
	STDMETHOD(LoadXML)(int nType, BSTR bstrXML);
	STDMETHOD(Show)();
	STDMETHOD(GetGridByName)(BSTR bstrName, IXobjCollection** pVal);
	STDMETHOD(GetChildGridByName)(BSTR bstrName, IXobj** pVal);
	STDMETHOD(get_DockObj)(BSTR bstrName, LONGLONG* pVal);
	STDMETHOD(put_DockObj)(BSTR bstrName, LONGLONG newVal);
	STDMETHOD(NavigateURL)(BSTR bstrURL, IDispatch* dispObjforScript);

	STDMETHOD(SendIPCMessage)(BSTR bstrTo, BSTR bstrPayload, BSTR bstrExtra, BSTR bstrMsgId, BSTR* bstrRet);
	STDMETHOD(GetUIScript)(BSTR bstrCtrlName, BSTR* bstrVal);

	BEGIN_COM_MAP(CXobj)
		COM_INTERFACE_ENTRY(IXobj)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CXobj)
		CONNECTION_POINT_ENTRY(__uuidof(_IXobjEvents))
	END_CONNECTION_POINT_MAP()

	HWND CreateView(HWND hParentWnd, CString strTag);

private:
	CString _GetNames(CXobj* pXobj);
	void _get_Objects(CXobj* pXobj, UINT32& nType, CXobjCollection* pGridColletion);
	int _getNodes(CXobj* pXobj, CString& strName, CXobj**ppRetGrid, CXobjCollection* pGrids);
};

// CXobjCollection

class ATL_NO_VTABLE CXobjCollection :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IXobjCollection, &IID_IXobjCollection, &LIBID_World, 1, 0>
{
public:
	CXobjCollection();
	~CXobjCollection();

	BEGIN_COM_MAP(CXobjCollection)
		COM_INTERFACE_ENTRY(IXobjCollection)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	STDMETHOD(get_GridCount)(long* pCount);
	STDMETHOD(get_Item)(long iIndex, IXobj ** ppGrid);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	CGridVector*	m_pXobjs;

private:
	CGridVector	m_vGrids;
};
