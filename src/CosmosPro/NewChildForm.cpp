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
* OUTLINED IN THE GPL LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
*
********************************************************************************/

#include "stdafx.h"
#include "dllmain.h"
#include "CosmosProxy.h"
#include "NewChildForm.h"

namespace Cosmos {
	System::Void NewChildForm::NewTemplateListView_DoubleClick(System::Object^ sender, System::EventArgs^ e) {
		String^ strXml = NewTemplateListView->SelectedItems[0]->Tag->ToString();
		if (String::IsNullOrEmpty(strXml) == false)
		{
			IDispatch* pFormDisp = theApp.m_pHubbleImpl->m_pCLRProxy->CreateCLRObj(strXml);
		}
		Close();
	}
}
