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
* http://www.tangramteam.com/
*
********************************************************************************/
#include "stdafx.h"
#include "dllmain.h"
#include "WizCtrl.h"
#include "WizForm.h"
#include "Cosmos.h"

namespace Universe
{
	System::Void WizCtrl::WizCtrl_Load(System::Object^ sender, System::EventArgs^ e) {
#ifndef _WIN64
		WizForm^ pWizForm = (WizForm^)Cosmos::m_pWizForm;
		if (pWizForm!=nullptr&&pWizForm->m_pWizListView!=nullptr)
		{
			if(pWizForm->m_pWizListView->SelectedItems->Count)
				pWizForm->WizCtrlDic[pWizForm->m_pWizListView->SelectedItems[0]->Text] = this;
			else
				pWizForm->WizCtrlDic[pWizForm->m_pWizListView->Items[0]->Text] = this;
		}
#endif
	}
}
