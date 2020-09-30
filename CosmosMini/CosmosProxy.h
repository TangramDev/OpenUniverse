/********************************************************************************
*					Open Universe - version 1.0.0.5								*
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

// CosmosProxy.h : Declaration of the CCosmosProxy

#pragma once

#include "resource.h"       // main symbols
#include "TangramBase.h"       
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::Collections;

class CCosmosProxy : public IHubbleCLRImpl
{
public: 
	CCosmosProxy();
	virtual~CCosmosProxy();

	bool												m_bInitApp = false;
	HWND												m_hCreatingCLRWnd = nullptr;
	gcroot<String^>										m_strExtendableTypes;
	map<CString, gcroot<Assembly^>>						m_mapAssembly;
	map<HWND, GalaxyInfo*>								m_mapGalaxyInfo;
	map<HWND, CString>									m_mapEventBindInfo;

	gcroot<Form^>										m_pCurrentPForm;
	Object^ _getObject(Object^ key);
	bool _insertObject(Object^ key, Object^ val);
	bool _removeObject(Object^ key);

	template<typename T1, typename T2>
	T2^ _createObject(T1* punk)
	{
		T2^ pRetObj = nullptr;

		if (punk != NULL)
		{
			LONGLONG l = (LONGLONG)punk;
			pRetObj = (T2^)_getObject(l);

			if (pRetObj == nullptr)
			{
				pRetObj = gcnew T2(punk);
				_insertObject(l, pRetObj);
			}
		}
		return pRetObj;
	}

	IDispatch* CreateCLRObj(CString bstrObjID);
	void HubbleAction(BSTR bstrXml, void*);
private:
	map<HWND, gcroot<Form^>>				m_mapForm;
	map<HWND, CString>						m_mapUIData;
	gcroot<Hashtable^>						m_htObjects;
	gcroot<EventHandler^>					m_pOnLoad;
	gcroot<EventHandler^>					m_pOnCtrlVisible;

	virtual void OnCLRHostExit();
	CWPFObj* CreateWPFControl(IGrid* pGrid, HWND hPWnd, UINT nID);
	//HRESULT ActiveCLRMethod(BSTR bstrObjID, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData);
	//HRESULT ActiveCLRMethod(IDispatch* pCLRObj, BSTR bstrMethod, BSTR bstrParam, BSTR bstrData);
	HRESULT ProcessCtrlMsg(HWND hCtrl, bool bShiftKey);
	BOOL ProcessFormMsg(HWND hFormWnd, LPMSG lpMsg, int nMouseButton);
	IDispatch* CreateObject(BSTR bstrObjID, HWND hParent, IGrid* pHostNode);
	int IsWinForm(HWND hWnd);
	IDispatch* GetCLRControl(IDispatch* CtrlDisp, BSTR bstrNames);
	BSTR GetCtrlName(IDispatch* pCtrl);
	HWND GetMDIClientHandle(IDispatch* pMDICtrl) { return NULL; };
	IDispatch* GetCtrlByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild);
	HWND GetCtrlHandle(IDispatch* pCtrl);
	BSTR GetCtrlType(IDispatch* pCtrl);
	IDispatch* GetCtrlFromHandle(HWND hWnd);
	HWND IsGalaxy(IDispatch* ctrl);
	void ReleaseHubbleObj(IDispatch*);
	BSTR GetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild);
	void SetCtrlValueByName(IDispatch* CtrlDisp, BSTR bstrName, bool bFindInChild, BSTR strVal);
	HRESULT NavigateURL(IGrid* pGrid, CString strURL, IDispatch* dispObjforScript);
	Control^ GetCanSelect(Control^ ctrl, bool direct);

	HWND GetHwnd(HWND parent, int x, int y, int width, int height);
	void OnDestroyChromeBrowser(IBrowser*);

	void* Extend(CString strKey, CString strData, CString strFeatures);
	bool IsSupportDesigner();
	HICON GetAppIcon(int nIndex);
	void OnWinFormActivate(HWND, int nState);
	void OnWebPageCreated(HWND, CWebPageImpl*, IWebPage*, int nState);
	void HideMenuStripPopup();
	bool PreWindowPosChanging(HWND hWnd, WINDOWPOS* lpwndpos, int nType);
	void ConnectGridToWebPage(IGrid*, bool);
	void OnCloudMsgReceived(CSession*);

	void WindowCreated(LPCTSTR strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd);
	void WindowDestroy(HWND hWnd);
	//void SelectGrid(IGrid* );
	void InitControl(Form^ pForm, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse);
	void InitGrid(IGrid* pGrid, Control^ pCtrl, bool bSave, CTangramXmlParse* pParse);
	void SetObjectProperty(IDispatch* pObj, BSTR bstrPropertyName, BSTR bstrPropertyValue) {};
	IDispatch* CreateWinForm(HWND hParent, BSTR strXML);

	void CtrlInit(int nType, Control^ treeview, IGalaxyCluster* pGalaxyCluster);
	System::Void LoadNode(TreeView^ pTreeView, TreeNode^ pGrid, IGalaxyCluster* pGalaxyCluster, CTangramXmlParse* pParse);
	static void OnLoad(Object ^sender, EventArgs ^e);
	static void OnApplicationExit(Object ^sender, EventArgs ^e);
	static void OnVisibleChanged(Object ^sender, EventArgs ^e);
	static void OnItemSelectionChanged(Object ^sender, ListViewItemSelectionChangedEventArgs ^e);
	static void OnClick(Object ^sender, EventArgs ^e);
	static void OnTextChanged(System::Object^ sender, System::EventArgs^ e);
};

