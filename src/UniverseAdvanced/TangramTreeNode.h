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

// TangramTreeNode.h : Declaration of the CTangramTreeNode

#pragma once
#include "resource.h"       // main symbols
#include "universe.h"

struct CTangramXHtmlTreeNode;
// CTangramTreeNode

class CTangramTreeNode :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICosmosTreeNode, &IID_ICosmosTreeNode, &LIBID_Universe, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTangramTreeNode();

DECLARE_NO_REGISTRY()


BEGIN_COM_MAP(CTangramTreeNode)
	COM_INTERFACE_ENTRY(ICosmosTreeNode)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	CTangramXHtmlTreeNode* m_pXHTMLTREEDATA;
};
