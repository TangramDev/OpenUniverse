/********************************************************************************
*					Tangram Wizard - version 1.0.0							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE GPL LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
*
*
********************************************************************************/

// CCosmosFormView.cpp : implementation file
//

#include "pch.h"
#include "resource.h"
#include "dllmain.h"
#include "CosmosForm.h"

using namespace CommonUniverse;

// CCosmosFormView

IMPLEMENT_DYNCREATE(CCosmosFormView, CFormView)

CCosmosFormView::CCosmosFormView()
	: CFormView(IDD_ITANGRAMVSCOMPONENTFORMVIEW)
{

}

CCosmosFormView::~CCosmosFormView()
{
}

void CCosmosFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCosmosFormView, CFormView)
	ON_BN_CLICKED(IDC_BTN_HOWTO, &CCosmosFormView::OnBnClickedBtnHowto)
END_MESSAGE_MAP()


// CCosmosFormView diagnostics

#ifdef _DEBUG
void CCosmosFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCosmosFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CCosmosFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

// CCosmosFormView message handlers


void CCosmosFormView::OnBnClickedBtnHowto()
{
	//if (g_pCosmosImpl&& g_pHubbleImpl->m_pCLRProxy)
	//{
	//	IGrid* pNode = nullptr;
	//	theApp.m_pHubble->GetGridFromHandle((__int64)m_hWnd, &pNode);
	//	g_pHubbleImpl->m_pCLRProxy->HubbleAction(_T("ExternAction:test"), pNode);
	//}
}
