#include "stdafx.h"
#include "dllmain.h"
#include "WizForm.h"
#include "Cosmos.h"

namespace Cosmos {
	WizForm::WizForm(void)
	{
		InitializeComponent();
		//
		//TODO: Add the constructor code here
		//
#ifndef _WIN64
		Cosmos::Hubble::m_pWizForm = this;
		m_pWizCtrlDic->Clear();
#endif
	}

	System::Void WizForm::WizForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}

	void WizForm::NavigateData::set(String^ strData)
	{
		if (String::IsNullOrEmpty(strData)==false)
		{
			Cosmos::Hubble::m_strWizData = strData;
		}
	}

	void WizForm::InitCustomizeDic(String^ strDicData)
	{
		if (String::IsNullOrEmpty(strDicData)==false)
		{
			CString strData = strDicData;
			int nTokenPos = 0;
			CString strToken = strData.Tokenize(_T("|"), nTokenPos);
			while (!strToken.IsEmpty())
			{
				Cosmos::Hubble::CustomizeDic[BSTR2STRING(strToken)] = L"";
				strToken = strData.Tokenize(_T("|"), nTokenPos);
			}
		}
	}

	void WizForm::FillListView(String^ _strXmlData, System::Windows::Forms::ListView^ ctrl)
	{
		CString strXmlData = _strXmlData;
		CTangramXmlParse m_Parse;
		int nPos = strXmlData.ReverseFind('\\');
		CString strPath = strXmlData.Left(nPos + 1);
		if (m_Parse.LoadFile(strXmlData) || m_Parse.LoadXml(strXmlData))
		{
			ImageList^ pImageList = gcnew ImageList();
			pImageList->ImageSize = System::Drawing::Size(64, 64);
			ctrl->LargeImageList = pImageList;
			m_pWizListView = ctrl;
			int nCount = m_Parse.GetCount();
			for (int i = 0; i < nCount; i++)
			{
				CTangramXmlParse* pChild = m_Parse.GetChild(i);
				CString _strPath = strPath + pChild->attr(_T("iconName"), pChild->name()) + _T(".png");
				int nIndex = -1;
				if (::PathFileExists(_strPath))
				{
					//System::Drawing::Icon^ pIcon = gcnew System::Drawing::Icon(BSTR2STRING(_strPath));
					//pImageList->Images->Add(pIcon);
					pImageList->Images->Add(Image::FromFile(BSTR2STRING(_strPath)));
				}
				CString strName = pChild->attr(_T("name"), pChild->name());
				strName.Replace(_T("_"), _T(" "));
				ctrl->Items->Add(BSTR2STRING(strName), i)->Tag = BSTR2STRING(pChild->xml());
			}
			if (ctrl->Items->Count > 0)
			{
				ctrl->Items[0]->Selected = true;
			}
		}
	}
}
