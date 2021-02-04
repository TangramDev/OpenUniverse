/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102050024
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser: Independent Browser Window and Browser Window
 *    as sub windows of other windows are supported in the application process;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#include "../../stdafx.h"
#include "../../Xobj.h"
#include "../../Galaxy.h"
#include "../../UniverseApp.h"
#include "../../GalaxyCluster.h"
#include "../../TangramHtmlTreeWnd.h"
#include "ExcelPlusWnd.h"
#include "ExcelAddin.h"

namespace OfficePlus
{
	namespace ExcelPlus
	{
		CExcelWorkBook::CExcelWorkBook(void)
		{
#ifdef _DEBUG
			g_pCosmos->m_nOfficeDocs++;
#endif			
			m_bCreating						= FALSE;
			m_pDocGalaxyCluster				= nullptr;
			m_pGalaxy						= nullptr;
			m_pWorkBook						= nullptr;
			m_pSheetNode					= nullptr;
			m_pTaskPaneGalaxyCluster		= nullptr;
			m_pTaskPaneGalaxy				= nullptr;
			m_strDocXml						= _T("");
			m_strTaskPaneXml				= _T("");
			m_strDefaultSheetXml			= _T("");

		}

		CExcelWorkBook::~CExcelWorkBook(void)
		{
#ifdef _DEBUG
			g_pCosmos->m_nOfficeDocs--;
#endif			
		}

		void CExcelWorkBook::InitWorkBook()
		{
			CExcelAddin* pAddin = (CExcelAddin*)g_pCosmos;
			 
			auto t = create_task([pAddin, this]()
			{
				CoInitializeEx(NULL, COINIT_MULTITHREADED);
				if (m_strDocXml==_T(""))
				{
					m_strDocXml = pAddin->GetDocXmlByKey(m_pWorkBook, CComBSTR(L"tangram"));
					CTangramXmlParse m_Parse;
					m_Parse.LoadXml(m_strDocXml);
					m_strTaskPaneTitle = m_Parse.attr(_T("title"), _T("TaskPane"));
					m_nWidth = m_Parse.attrInt(_T("width"), 200);
					m_nHeight = m_Parse.attrInt(_T("height"), 300);
					m_nMsoCTPDockPosition = (MsoCTPDockPosition)m_Parse.attrInt(_T("dockposition"), 4);
					m_nMsoCTPDockPositionRestrict = (MsoCTPDockPositionRestrict)m_Parse.attrInt(_T("dockpositionrestrict"), 3);

					CTangramXmlParse* pChild = m_Parse.GetChild(_T("default"));
					if (pChild == nullptr)
					{
						CoUninitialize();
						return;
					}
					m_strDefaultSheetXml = pChild->xml();
				}
				else
				{
					pAddin->AddDocXml(m_pWorkBook, CComBSTR(m_strDocXml), CComBSTR(L"tangram"));
				}
				CoUninitialize();
			}).then([pAddin,this]()
			{
				CString strKey = _T("<cluster>");
				CString strData = _T("");
				CString strVal = _T("");

				CString _strXml = pAddin->GetXmlData(_T("userforms"), m_strDocXml);
				int nPos = _strXml.Find(strKey);
				while (nPos != -1)
				{
					strData = _strXml.Left(nPos);
					nPos = strData.ReverseFind('<');
					strData = strData.Mid(nPos + 1);
					nPos = strData.ReverseFind('>');
					strData = strData.Left(nPos);
					strData.Trim();
					strVal = pAddin->GetXmlData(strData, _strXml);
					strKey = _T("</cluster>");
					nPos = _strXml.Find(strKey);
					_strXml = _strXml.Mid(nPos + 9);
					nPos = _strXml.Find(_T("<cluster>"));
					m_mapUserFormScript[strData] = strVal;
				}

				::PostMessage(pAddin->m_hHostWnd, WM_OPENDOCUMENT, (WPARAM)m_mapExcelWorkBookWnd.begin()->second, 0);
			});
		}

		//void CExcelWorkBook::ModifySheetForTangram(IDispatch* Sh, CString strSheetXml, CString strTaskPaneXml)
		//{
		//	if (Sh == nullptr)
		//	{
		//		return;
		//	}

		//	CComBSTR szMember(L"CustomProperties");
		//	DISPID dispid = -1;
		//	HRESULT hr = Sh->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &dispid);
		//	if (hr == S_OK)
		//	{
		//		DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
		//		VARIANT result = { 0 };
		//		EXCEPINFO excepInfo;
		//		memset(&excepInfo, 0, sizeof excepInfo);
		//		UINT nArgErr = (UINT)-1;
		//		HRESULT hr = Sh->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispParams, &result, &excepInfo, &nArgErr);
		//		if (S_OK == hr && VT_DISPATCH == result.vt)
		//		{
		//			CString strXml = _T("");
		//			if(strTaskPaneXml!=_T("")&& strSheetXml!=_T(""))
		//				strXml.Format(_T("<sheet><default><sheet><cluster>%s</cluster></sheet><taskpane><cluster>%s</cluster></taskpane></default></sheet>"), strSheetXml, strTaskPaneXml);
		//			else
		//			{
		//				if(strTaskPaneXml == _T("") && strSheetXml != _T(""))
		//					strXml.Format(_T("<sheet><default><sheet><cluster>%s</cluster></sheet></default></sheet>"), strSheetXml);
		//				else
		//				{
		//					strXml.Format(_T("<sheet><default><sheet><cluster><xobj name=\"Start\" objid=\"nucleus\" /></cluster></sheet><taskpane><cluster>%s</cluster></taskpane></default></sheet>"), strTaskPaneXml);
		//				}
		//			}
		//			CComQIPtr<Excel::CustomProperties> pCustomProperties(result.pdispVal);
		//			Excel::ICustomProperties* pProps = (Excel::ICustomProperties*)pCustomProperties.p;
		//			if (pCustomProperties)
		//			{
		//				CComPtr<Excel::CustomProperty> pProp;
		//				long nCount = 0;
		//				CComBSTR bstrName(L"");
		//				pProps->get_Count(&nCount);
		//				for (int i = 1; i <= nCount; i++)
		//				{
		//					CComPtr<Excel::CustomProperty> pProp;
		//					pProps->get_Item(CComVariant(i), &pProp);
		//					Excel::ICustomProperty* _pProp = (ICustomProperty*)pProp.p;
		//					_pProp->get_Name(&bstrName);
		//					CString strName = OLE2T(bstrName);
		//					if (strName == _T("Tangram"))
		//					{
		//						_pProp->put_Value(CComVariant(strXml));
		//						::VariantClear(&result);
		//						return;
		//					}
		//				}
		//			}
		//			CComPtr<Excel::CustomProperty> pProp;
		//			pProps->Add(CComBSTR(L"Tangram"), CComVariant(strXml), &pProp);
		//		}
		//		::VariantClear(&result);
		//	}
		//}

		void CExcelWorkBook::OnNewSheet(IDispatch* Sh)
		{
			CString strSheetXml = _T("");
			CString strSheetTaskPanelXml = _T("");
			CString _strSheetXml = _T("");
			CString strSheetName = _T("");

			if (m_bCreating==false)
			{
				VARIANT_BOOL bDesignState = false;
				strSheetName = g_pCosmos->GetNewGUID();
				strSheetName.Replace(_T("-"), _T(""));
				strSheetName = _T("Sheet") + strSheetName;
				CString strPath = g_pCosmos->m_strExeName;
				strPath += _T("WorkSheet");
				CString strTemplate = g_pCosmos->GetDocTemplateXml(_T("Please select WorkSheet Template:"), strPath, _T(".SheetXml"));
				auto it = g_pCosmos->m_mapValInfo.find(_T("fromvisualstudio"));
				if (it != g_pCosmos->m_mapValInfo.end())
				{
					bDesignState = it->second.boolVal;
				}
				else
				{
					it = g_pCosmos->m_mapValInfo.find(_T("exceldesignstate"));
					if (it != g_pCosmos->m_mapValInfo.end())
					{
						bDesignState = it->second.boolVal;
					}
				}
				if (strTemplate == _T(""))
				{
					if(bDesignState)
						strTemplate = _T("<sheet><cluster><xobj name=\"start\" /></cluster></sheet>");
					else
					{
						strTemplate = _T("<sheet><cluster><xobj name=\"start\" gridtype=\"nucleus\" /></cluster></sheet>");
					}
				}
				CTangramXmlParse m_Parse;
				if (m_Parse.LoadXml(strTemplate) || m_Parse.LoadFile(strTemplate))
				{
					strTemplate = m_Parse.xml();
					CString strName = _T("<")+m_Parse.name();
					int nPos = strTemplate.ReverseFind('<');
					CString str = strTemplate.Left(nPos);
					nPos = str.Find(strName);
					str = str.Mid(nPos + strName.GetLength());
					strName = _T("sheet");
					strSheetXml = _T("<");
					strSheetXml += strName;
					strSheetXml += str;
					strSheetXml += _T("</");
					strSheetXml += strName;
					strSheetXml += _T(">");
					if (bDesignState&&::MessageBox(::GetActiveWindow(), _T("Do you want to create a taskpane Object with this sheet?"), _T("New Sheet"), MB_YESNO) == IDYES)
					{
						strPath = g_pCosmos->m_strExeName;
						strPath += _T("worksheetTaskPanel");
						strTemplate = g_pCosmos->GetDocTemplateXml(_T("Please select WorkSheet TaskPanel Template:"), strPath, _T(".TaskPanelXml"));
						if (strTemplate == _T(""))
						{
							if (bDesignState)
								strTemplate = _T("<sheet><cluster><xobj name=\"start\" /></cluster></sheet>");
							else
							{
								strTemplate = _T("<sheet><cluster><xobj name=\"start\" objid=\"nucleus\" /></cluster></sheet>");
							}
						}
						CTangramXmlParse m_Parse2;
						if (m_Parse2.LoadXml(strTemplate) || m_Parse2.LoadFile(strTemplate))
						{
							strTemplate = m_Parse2.xml();
							CString strName = _T("<")+m_Parse.name();
							int nPos = strTemplate.ReverseFind('<');
							CString str = strTemplate.Left(nPos);
							nPos = str.Find(strName);
							str = str.Mid(nPos + strName.GetLength());
							strName = _T("taskpane");
							strSheetTaskPanelXml = _T("<");
							strSheetTaskPanelXml += strName;
							strSheetTaskPanelXml += str;
							strSheetTaskPanelXml += _T("</");
							strSheetTaskPanelXml += strName;
							strSheetTaskPanelXml += _T(">");
						}
					}
				}
				
				if(strSheetTaskPanelXml!=_T(""))
					_strSheetXml.Format(_T("<%s><default>%s%s</default></%s>"), strSheetName, strSheetXml, strSheetTaskPanelXml, strSheetName);
				else
					_strSheetXml.Format(_T("<%s><default>%s</default></%s>"), strSheetName, strSheetXml, strSheetName);
			}
			CComQIPtr<_Worksheet> pSheet(Sh);
			if(pSheet)
			{
				auto it = find(pSheet.p);
				if (it == end())
				{
					CComBSTR bstrName(L"");
					pSheet->get_Name(&bstrName);
					CExcelWorkSheet* pExcelWorkSheet = new CExcelWorkSheet();
					pExcelWorkSheet->m_strSheetName = strSheetName;// OLE2T(bstrName);
					pExcelWorkSheet->m_pSheet = pSheet.p;
					pExcelWorkSheet->m_pSheet->AddRef();
					(*this)[pExcelWorkSheet->m_pSheet] = pExcelWorkSheet;
					if (_strSheetXml != _T(""))
					{
						CComPtr<Excel::CustomProperties> _pProps;
						pSheet->get_CustomProperties(&_pProps);
						ICustomProperties* pProps = (ICustomProperties*)_pProps.p;
						CComPtr<Excel::CustomProperty> pProp;
						pProps->Add(CComBSTR(L"Tangram"), CComVariant(_strSheetXml), &pProp);
						(*pExcelWorkSheet)[strSheetName] = _strSheetXml;
					}
				}
			}
		}

		void CExcelWorkBook::OnSheetBeforeDelete(IDispatch* Sh)
		{
			CComQIPtr<_Worksheet> pSheet(Sh);
			if(pSheet)
			{
				auto it = find(pSheet.p);
				if(it!=end())
				{
					delete it->second;
					erase(it);
				}
			}
		}

		void CExcelWorkBook::OnBeforeSave(VARIANT_BOOL SaveAsUI, VARIANT_BOOL* Cancel)
		{
			CExcelAddin* pAddin = (CExcelAddin*)g_pCosmos;
			if (m_pTaskPaneGalaxy)
			{
				m_pTaskPaneGalaxy->UpdateXobj();
			}
			if (m_pGalaxy)
			{
				m_pGalaxy->UpdateXobj();
				HWND hPWnd = m_pGalaxy->GetTopLevelParent().m_hWnd;
				auto it1 = pAddin->m_mapTaskPaneMap.find((long)hPWnd);
				if (it1 != pAddin->m_mapTaskPaneMap.end())
				{
					CString strXml = pAddin->GetDocXmlByKey(m_pWorkBook, CComBSTR(L"tangram"));
					CTangramXmlParse m_Parse;
					bool b = m_Parse.LoadXml(strXml);
					if (b)
					{
						CComBSTR bstrXml(L"");
						it1->second->get_Title(&bstrXml);
						strXml = OLE2T(bstrXml);
						m_Parse.put_attr(_T(""), strXml);
						int nValue = 0;
						it1->second->get_Width(&nValue);
						m_Parse.put_attr(_T("width"), nValue);
						it1->second->get_Height(&nValue);
						m_Parse.put_attr(_T("height"), nValue);
						Office::MsoCTPDockPosition m_Pos;
						it1->second->get_DockPosition(&m_Pos);
						m_Parse.put_attr(_T("dockposition"), m_Pos);
						Office::MsoCTPDockPositionRestrict m_MsoCTPDockPositionRestrict;
						it1->second->get_DockPositionRestrict(&m_MsoCTPDockPositionRestrict);
						m_Parse.put_attr(_T("dockpositionrestrict"), m_MsoCTPDockPositionRestrict);
						pAddin->AddDocXml(m_pWorkBook, CComBSTR(m_Parse.xml()), CComBSTR(L"tangram"));
					}
				}
			}
		}

		void CExcelWorkBook::OnSheetActivate(IDispatch* Sh)
		{
			g_pCosmos->m_pDesignXobj = nullptr;
			if (m_pGalaxy == nullptr)
				return;

			CExcelAddin* pAddin = (CExcelAddin*)g_pCosmos;
			CComQIPtr<Excel::_Worksheet> pSheet(Sh);
			CExcelObject* pExcelWorkBookWnd = pAddin->m_pActiveExcelObject;

			CString strName = _T("");
			CString strXml = m_strDefaultSheetXml;
			if (pSheet)
			{
				CComPtr<Excel::CustomProperties> pCustomProperties;
				pSheet->get_CustomProperties(&pCustomProperties);
				Excel::ICustomProperties* pProps = (Excel::ICustomProperties*)pCustomProperties.p;
				long nCount = 0;
				CComBSTR bstrName(L"");
				pProps->get_Count(&nCount);
				for (int i = 1; i <= nCount; i++)
				{
					CComPtr<Excel::CustomProperty> pProp;
					pProps->get_Item(CComVariant(i), &pProp);
					Excel::ICustomProperty* _pProp = (ICustomProperty*)pProp.p;
					_pProp->get_Name(&bstrName);
					strName = OLE2T(bstrName);
					if (strName == _T("Tangram"))
					{
						CComVariant var;
						_pProp->get_Value(&var);
						strXml = OLE2T(var.bstrVal);
						::VariantClear(&var);
						break;
					}
				}
			}
			else
			{
				CComQIPtr<Excel::_Chart> pChart(Sh);
			}
			if (strXml == _T(""))
				return;
			CTangramXmlParse m_Parse;
			bool b = m_Parse.LoadXml(strXml);
			if (b)
			{
				CTangramXmlParse* pParse = m_Parse.GetChild(_T("default"));
				if (pParse)
				{
					strName = m_Parse.name();
					CString strKey = strName;
					strName += _T("_default");
					CTangramXmlParse* pParse2 = pParse->GetChild(_T("sheet"));
					if (pParse2)
					{
						IXobj* pXobj = nullptr;
						m_pGalaxy->Observe(strName.AllocSysString(), CComBSTR(pParse2->xml()), &pXobj);
						CXobj* _pXobj = (CXobj*)pXobj;
						if (_pXobj->m_pXobjShareData->m_pOfficeObj == nullptr)
						{
							_pXobj->m_pXobjShareData->m_pOfficeObj = Sh;
							_pXobj->m_pXobjShareData->m_pOfficeObj->AddRef();
						}
					}
				}
				pParse = pParse->GetChild(_T("taskpane"));
				if (pParse)
				{
					strName += _T("_taskpane");

					Office::_CustomTaskPane* _pCustomTaskPane = nullptr;
					CString strSheetName = _T("");
					auto it = pAddin->m_mapTaskPaneMap.find((long)pExcelWorkBookWnd->m_hForm);
					if (it == pAddin->m_mapTaskPaneMap.end())
					{
						VARIANT vWindow;
						vWindow.vt = VT_DISPATCH;
						vWindow.pdispVal = nullptr;
						HRESULT hr = pAddin->m_pCTPFactory->CreateCTP(L"Tangram.Ctrl.1", m_strTaskPaneTitle.AllocSysString(), vWindow, &_pCustomTaskPane);
						_pCustomTaskPane->AddRef();
						pAddin->m_mapTaskPaneMap[(long)pAddin->m_pActiveExcelObject->m_hForm] = _pCustomTaskPane;
						_pCustomTaskPane->put_Visible(true);
						CComPtr<ICosmosCtrl> pCtrlDisp;
						_pCustomTaskPane->get_ContentControl((IDispatch**)&pCtrlDisp);
						if (pCtrlDisp)
						{
							LONGLONG hWnd = 0;
							pCtrlDisp->get_HWND(&hWnd);
							pExcelWorkBookWnd->m_hTaskPane = (HWND)hWnd;
							if (m_pTaskPaneGalaxyCluster == nullptr)
							{
								HWND hPWnd = ::GetParent((HWND)hWnd);
								CWindow m_Wnd;
								m_Wnd.Attach(hPWnd);
								m_Wnd.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
								m_Wnd.Detach();

								m_pTaskPaneGalaxyCluster = new CComObject<CGalaxyCluster>();
								m_pTaskPaneGalaxyCluster->m_hWnd = hPWnd;
								g_pCosmos->m_mapWindowPage[hPWnd] = m_pTaskPaneGalaxyCluster;

								IGalaxy* pTaskPaneFrame = nullptr;
								m_pTaskPaneGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)hWnd), CComBSTR(L"TaskPane"), &pTaskPaneFrame);
								m_pTaskPaneGalaxy = (CGalaxy*)pTaskPaneFrame;
								if (m_pTaskPaneGalaxy)
								{
									IXobj* pXobj = nullptr;
									m_pTaskPaneGalaxy->Observe(CComBSTR(strName), pParse->xml().AllocSysString(), &pXobj);
									CXobj* _pXobj = (CXobj*)pXobj;
									if (_pXobj->m_pXobjShareData->m_pOfficeObj == nullptr)
									{
										_pXobj->m_pXobjShareData->m_pOfficeObj = Sh;
										_pXobj->m_pXobjShareData->m_pOfficeObj->AddRef();
									}
								}
							}
							else
								m_pTaskPaneGalaxy->ModifyHost(hWnd);
						}
					}
					else
					{
						_pCustomTaskPane = it->second;
						_pCustomTaskPane->put_Visible(true);
						if (m_pTaskPaneGalaxy)
						{
							IXobj* pXobj = nullptr;
							m_pTaskPaneGalaxy->Observe(CComBSTR(strName), pParse->xml().AllocSysString(), &pXobj);
						}
					}
				}
			}
		}

		CExcelWorkSheet::CExcelWorkSheet(void)
		{
#ifdef _DEBUG
			g_pCosmos->m_nOfficeDocsSheet++;
#endif	
			m_pSheet			= nullptr;
			m_strKey			= _T("");
			m_strSheetName		= _T("");
		}

		CExcelWorkSheet::~CExcelWorkSheet(void)
		{
#ifdef _DEBUG
			g_pCosmos->m_nOfficeDocsSheet--;
#endif	
			m_pSheet			= nullptr;
			m_strKey			= _T("");
			m_strSheetName		= _T("");
		}
	}
}
