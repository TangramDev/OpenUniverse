/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
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


#include "../../stdafx.h"
#include "../../UniverseApp.h"
#include "../../Grid.h"
#include "../../Galaxy.h"
#include "WordPlusDoc.h"
#include "WordAddin.h"
 
namespace OfficePlus
{
	namespace WordPlus
	{
		CWordDocument::CWordDocument(void)
		{
#ifdef _DEBUG
			g_pHubble->m_nOfficeDocs++;
#endif			
			m_pDoc					= nullptr;
			m_pGalaxy				= nullptr;
			m_pDocGalaxyCluster				= nullptr;
			m_pTaskPaneGalaxy		= nullptr;
			m_pTaskPaneGalaxyCluster			= nullptr;
			m_strTaskPaneXml		= _T("");
		}

		CWordDocument::~CWordDocument(void)
		{
#ifdef _DEBUG
			g_pHubble->m_nOfficeDocs--;
#endif			
		}

		void CWordDocument::OnClose()
		{
			//this->DispEventUnadvise(m_pDoc);
		}

		void CWordDocument::InitDocument()
		{
			CWordAddin* pAddin = (CWordAddin*)g_pHubble;
			IStream* pStream = 0;
			IStream* pStream2 = 0;
			HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(IID_IHubble, (IHubble*)g_pHubble, &pStream);
			hr = ::CoMarshalInterThreadInterfaceInStream(IID__Document, m_pDoc, &pStream2);
			auto t = create_task([pAddin,pStream, pStream2, this]()
			{
				CoInitializeEx(NULL, COINIT_MULTITHREADED);
				IHubble* _pAddin = nullptr;
				HRESULT hr = ::CoGetInterfaceAndReleaseStream(pStream, IID_IHubble, (LPVOID *)&_pAddin);
				_Document* _pDoc = nullptr;
				hr = ::CoGetInterfaceAndReleaseStream(pStream2, IID__Document, (LPVOID *)&_pDoc);
				if (m_strDocXml == _T(""))
				{
					m_strDocXml = pAddin->GetDocXmlByKey(_pDoc, CComBSTR(L"tangram"));
				}
				else
				{
					pAddin->AddDocXml(_pDoc, CComBSTR(m_strDocXml), CComBSTR(L"tangram"));
				}
				CoUninitialize();
			}).then([pAddin,this]()
			{
				CString _strXml = pAddin->GetXmlData(_T("documentui"), m_strDocXml);
				CString strKey = _T("<layout>");
				CString strData = _T("");
				CString strVal = _T("");
				int nPos = _strXml.Find(strKey);
				while (nPos != -1)
				{
					strData = _strXml.Left(nPos);
					nPos = strData.ReverseFind('<');
					strData = strData.Mid(nPos + 1);
					nPos = strData.ReverseFind('>');
					strData = strData.Left(nPos);
					strData.Trim();
					strVal = g_pHubble->GetXmlData(strData, _strXml);
					strKey = _T("</layout>");
					nPos = _strXml.Find(strKey);
					_strXml = _strXml.Mid(nPos + 9);
					nPos = _strXml.Find(_T("<layout>"));
					m_mapDocUIInfo[strData] = strVal;
				}
				_strXml = pAddin->GetXmlData(_T("taskpaneui"), m_strDocXml);
				strKey = _T("<layout>");
				strData = _T("");
				strVal = _T("");
				nPos = _strXml.Find(strKey);
				while (nPos != -1)
				{
					strData = _strXml.Left(nPos);
					nPos = strData.ReverseFind('<');
					strData = strData.Mid(nPos + 1);
					nPos = strData.ReverseFind('>');
					strData = strData.Left(nPos);
					strData.Trim();
					strVal = g_pHubble->GetXmlData(strData, _strXml);
					strKey = _T("</layout>");
					nPos = _strXml.Find(strKey);
					_strXml = _strXml.Mid(nPos + 9);
					nPos = _strXml.Find(_T("<layout>"));
					m_mapDocUIInfo[strData] = strVal;
				}

				_strXml = g_pHubble->GetXmlData(_T("userforms"), m_strDocXml);
				strKey = _T("<layout>");
				nPos = _strXml.Find(strKey);
				while (nPos != -1)
				{
					strData = _strXml.Left(nPos);
					nPos = strData.ReverseFind('<');
					strData = strData.Mid(nPos + 1);
					nPos = strData.ReverseFind('>');
					strData = strData.Left(nPos);
					strData.Trim();
					strVal = g_pHubble->GetXmlData(strData, _strXml);
					strKey = _T("</layout>");
					nPos = _strXml.Find(strKey);
					_strXml = _strXml.Mid(nPos + 9);
					nPos = _strXml.Find(_T("<layout>"));
					m_mapUserFormScript[strData] = strVal;
				}
				::PostMessage(pAddin->m_hHostWnd, WM_OPENDOCUMENT, (WPARAM)this, 0);
			});
		}

		CWordObject::CWordObject(void)
		{
			m_hForm				= NULL;
			m_hClient			= NULL;
			m_hTaskPane			= NULL;
			m_pWordPlusDoc		= nullptr;
			m_bDesignState		= false;
			m_bDesignTaskPane	= false;
		}

		void CWordObject::OnObjDestory()
		{
			if (m_pOfficeObj != nullptr)
			{
				CWordAddin* pAddin = ((CWordAddin*)g_pHubble);

				if (pAddin->m_pActiveWordObject == this)
				{
					pAddin->m_pActiveWordObject = nullptr;
					pAddin->m_pActiveGrid = nullptr;
				}

				auto it2 = m_pWordPlusDoc->find(m_hChildClient);
				if (it2 != m_pWordPlusDoc->end())
				{
					m_pWordPlusDoc->erase(it2);
				}

				size_t nCount = m_pWordPlusDoc->size();
				if (nCount > 0 && m_pWordPlusDoc->m_pGalaxy)
				{
					it2 = m_pWordPlusDoc->begin();
					m_pWordPlusDoc->m_pGalaxy->ModifyHost((LONGLONG)::GetParent(it2->second->m_hChildClient));
					if (m_hTaskPane)
					{
						m_hTaskPane = NULL;
						m_pWordPlusDoc->m_pTaskPaneGalaxy->ModifyHost((LONGLONG)it2->second->m_hTaskPaneChildWnd);
						::DestroyWindow(m_hTaskPaneWnd);
					}
				}

				auto it = pAddin->m_mapTaskPaneMap.find((LONGLONG)m_hForm);
				if (it != pAddin->m_mapTaskPaneMap.end())
				{
					it->second->Delete();
					pAddin->m_mapTaskPaneMap.erase(it);
				}
				if (nCount == 0)
				{
					if (m_pWordPlusDoc->m_pGalaxy)
					{
						::PostMessage(g_pHubble->m_hHostWnd, WM_COSMOSMSG, (WPARAM)m_pWordPlusDoc->m_pGalaxy->m_pWorkGrid->m_pHostWnd->m_hWnd, 1992);
					}
					HRESULT hr = S_OK;
					hr = m_pWordPlusDoc->DispEventUnadvise(m_pWordPlusDoc->m_pDoc);
					m_pWordPlusDoc->erase(m_pWordPlusDoc->begin(), m_pWordPlusDoc->end());
					auto it2 = pAddin->find(m_pWordPlusDoc->m_pDoc);
					if (it2 != pAddin->end())
						pAddin->erase(it2);

					delete m_pWordPlusDoc;
				}
				auto it3 = pAddin->m_mapOfficeObjects2.find(m_hForm);
				if (it3 != pAddin->m_mapOfficeObjects2.end())
					pAddin->m_mapOfficeObjects2.erase(it3);
			}
		}
	}
}
