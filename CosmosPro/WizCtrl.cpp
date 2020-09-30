#include "stdafx.h"
#include "dllmain.h"
#include "WizCtrl.h"
#include "WizForm.h"
#include "Cosmos.h"

namespace Cosmos
{
	System::Void WizCtrl::WizCtrl_Load(System::Object^ sender, System::EventArgs^ e) {
#ifndef _WIN64
		WizForm^ pWizForm = (WizForm^)Hubble::m_pWizForm;
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
