/********************************************************************************
 *             DOM Plus for Application - Version 1.1.8.202012250001
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

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
