/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30								*
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

// grid.h : Declaration of the CXobj

#include "GalaxyCluster.h"

#pragma once

class CWinForm;
class CXobjShareData
{
public:
	CXobjShareData();
	~CXobjShareData();

	CGalaxy*					m_pGalaxy;
	CGalaxy*					m_pOldGalaxy;
	CXobjHelper*				m_pHostClientView;
	CTangramXmlParse*			m_pCosmosParse;
	CGalaxyCluster*				m_pGalaxyCluster;
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

	XobjType						m_nViewType;
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

	CString							m_strNodeName;

	IDispatch*						m_pDisp;
	CXobj* 							m_pRootObj;
	CXobj* 							m_pParentObj;
	CWinForm*						m_pParentWinFormWnd;
	CTangramXmlParse*				m_pHostParse;
	CXobjShareData*					m_pXobjShareData;
	CWnd*							m_pHostWnd;
	CGalaxy*						m_pHostGalaxy;
	CRuntimeClass*					m_pObjClsInfo;

	CXobjEvents*					m_pCLREventConnector;

	VARIANT							m_varTag;
	IDispatch*						m_pExtender;
	CXobjVector						m_vChildNodes;
	CXobj*							m_pCurrentExNode;
	map<CXobj*, CString>			m_mapExtendNode;
	map<CString, CXobj*>			m_mapChildXobj;
	CComObject<CXobjCollection>*	m_pChildNodeCollection;

	map<IUniverseAppProxy*, CXobjProxy*> m_mapWndXobjProxy;

	void	InitWndXobj();
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
	BOOL	PreTranslateMessage(MSG* pMsg);
	BOOL	AddChildNode(CXobj* pXobj);
	BOOL	RemoveChildNode(CXobj* pXobj);
	CString GetNames();

	void NodeCreated();

	HRESULT Fire_ObserveComplete();
	HRESULT Fire_Destroy();
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
	STDMETHOD(get_XobjType)(XobjType* nType);
	STDMETHOD(get_ParentXobj)(IXobj** ppXobj);
	STDMETHOD(get_RootXobj)(IXobj** ppXobj);
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
	STDMETHOD(get_HostXobj)(IXobj** pVal);
	STDMETHOD(put_HostXobj)(IXobj* newVal);
	STDMETHOD(get_ActivePage)(int* pVal);
	STDMETHOD(put_ActivePage)(int newVal);
	STDMETHOD(get_OfficeObj)(IDispatch** pVal);
	STDMETHOD(get_WebPage)(IWebPage** pVal) { return S_FALSE; };

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

	STDMETHOD(Observe)(BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj);
	STDMETHOD(ObserveEx)(int nRow, int nCol, BSTR bstrKey, BSTR bstrXml, IXobj** ppRetXobj);
	STDMETHOD(ActiveTabPage)(IXobj* pXobj);
	STDMETHOD(GetXobj)(long nRow, long nCol,IXobj** ppXobj);
	STDMETHOD(GetXobjs)(BSTR bstrName, IXobj** ppXobj, IXobjCollection** ppXobjs, long* pCount);
	STDMETHOD(GetCtrlByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, IDispatch** ppRetDisp);
	STDMETHOD(GetChildXobjByName)(BSTR bstrName, IXobj** pVal);
	STDMETHOD(GetCtrlValueByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR* bstrVal);
	STDMETHOD(SetCtrlValueByName)(BSTR bstrName, VARIANT_BOOL bFindInChild, BSTR bstrVal);
	STDMETHOD(LoadXML)(int nType, BSTR bstrXML);
	STDMETHOD(Show)();
	STDMETHOD(GetXobjByName)(BSTR bstrName, IXobjCollection** pVal);
	STDMETHOD(get_DockObj)(BSTR bstrName, LONGLONG* pVal);
	STDMETHOD(put_DockObj)(BSTR bstrName, LONGLONG newVal);
	STDMETHOD(NavigateURL)(BSTR bstrURL, IDispatch* dispObjforScript);
	STDMETHOD(GetUIScript)(BSTR bstrCtrlName, BSTR* bstrVal);
	STDMETHOD(SendIPCMessage)(BSTR bstrTo, BSTR bstrPayload, BSTR bstrExtra, BSTR bstrMsgId, BSTR* bstrRet);

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
	int _getNodes(CXobj* pXobj, CString& strName, CXobj**ppRetXobj, CXobjCollection* pXobjs);
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

	STDMETHOD(get_XobjCount)(long* pCount);
	STDMETHOD(get_Item)(long iIndex, IXobj **ppXobj);
	STDMETHOD(get__NewEnum)(IUnknown** ppVal);
	CXobjVector*	m_pXobjs;

private:
	CXobjVector	m_vXobjs;
};
