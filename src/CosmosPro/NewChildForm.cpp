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

#include "stdafx.h"
#include "dllmain.h"
#include "CosmosProxy.h"
#include "NewChildForm.h"

namespace Universe {
	System::Void NewChildForm::NewTemplateListView_DoubleClick(System::Object^ sender, System::EventArgs^ e) {
		String^ strXml = NewTemplateListView->SelectedItems[0]->Tag->ToString();
		if (String::IsNullOrEmpty(strXml) == false)
		{
			IDispatch* pFormDisp = theApp.m_pCosmosImpl->m_pCLRProxy->CreateCLRObj(strXml);
		}
		Close();
	}
}
