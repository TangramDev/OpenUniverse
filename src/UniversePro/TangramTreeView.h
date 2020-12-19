// TangramTreeView.h : Declaration of the CTangramTreeView

#pragma once
#include "resource.h"       // main symbols
#include "universe.h"

class CTangramHtmlTreeWnd;
// CTangramTreeView

class CTangramTreeView :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICosmosTreeView, &IID_ICosmosTreeView, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTangramTreeView();

DECLARE_NO_REGISTRY()

	CTangramHtmlTreeWnd* m_pCosmosHtmlTreeWnd;
	map<CString,ICosmosTreeViewCallBack*>	m_mapCosmosTreeViewCallBack;
BEGIN_COM_MAP(CTangramTreeView)
	COM_INTERFACE_ENTRY(ICosmosTreeView)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease();

public:
	STDMETHOD(put_TangramTreeViewCallBack)(BSTR bstrKey, ICosmosTreeViewCallBack* newVal);
	STDMETHOD(AddTreeNode)(long hItem, BSTR bstrXml);
	STDMETHOD(InsertNode)(BSTR bstrXml, long* hItem);
	STDMETHOD(get_FirstRoot)(long* pVal);
};
