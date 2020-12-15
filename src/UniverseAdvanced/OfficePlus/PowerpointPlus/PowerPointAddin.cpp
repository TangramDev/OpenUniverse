/********************************************************************************
*					Open Universe - version 1.1.5.29							*
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
#include "../../GalaxyCluster.h"
#include "../../TangramHtmlTreeWnd.h"
#include "fm20.h"
#include "PowerPointAddin.h"
#include "PowerpointPlusEvents.cpp"

using namespace OfficePlus::PowerPointPlus;

namespace OfficePlus
{
	namespace PowerPointPlus
	{
		CPowerPntObject::CPowerPntObject(void)
		{
			m_hTaskPane = NULL;
			m_hTaskPaneWnd = NULL;
			m_hTaskPaneChildWnd = NULL;
			m_pPresentation = nullptr;
		}

		CPowerPntObject::~CPowerPntObject(void)
		{
		}

		void CPowerPntObject::OnObjDestory()
		{
			CPowerPntAddin* pAddin = ((CPowerPntAddin*)g_pHubble);

			if (pAddin->m_pActivePowerPntObject == this)
			{
				pAddin->m_pActivePowerPntObject = nullptr;
				pAddin->m_pActiveGrid = nullptr;
			}
			if (m_pPresentation)
			{
				CString strKey = m_pPresentation->m_strKey;
				auto it2 = m_pPresentation->find(m_hChildClient);
				if (it2 != m_pPresentation->end())
				{
					m_pPresentation->erase(it2);
				}
				size_t nCount = m_pPresentation->size();
				if (nCount > 0 && m_pPresentation->m_pGalaxy)
				{
					auto it2 = m_pPresentation->begin();
					m_pPresentation->m_pGalaxy->ModifyHost((LONGLONG)it2->second->m_hClient);
					if (m_hTaskPane)
					{
						m_hTaskPane = NULL;
						m_pPresentation->m_pTaskPaneGalaxy->ModifyHost((LONGLONG)it2->second->m_hTaskPaneChildWnd);
						::DestroyWindow(m_hTaskPaneWnd);
					}
				}
				if (nCount == 0)
				{
					CCloudAddinPresentation* pHubblePresentation = NULL;
					auto it = ((CPowerPntAddin*)g_pHubble)->m_mapHubblePres.find(strKey);
					if (it != ((CPowerPntAddin*)g_pHubble)->m_mapHubblePres.end())
						((CPowerPntAddin*)g_pHubble)->m_mapHubblePres.erase(it);
					delete m_pPresentation;
				}
			}

			auto it = pAddin->m_mapTaskPaneMap.find((long)m_hForm);
			if (it != pAddin->m_mapTaskPaneMap.end())
			{
				it->second->Delete();
				pAddin->m_mapTaskPaneMap.erase(it);
			}
			auto it3 = pAddin->m_mapOfficeObjects2.find(m_hForm);
			if (it3 != pAddin->m_mapOfficeObjects2.end())
				pAddin->m_mapOfficeObjects2.erase(it3);
		}

		CCloudAddinPresentation::CCloudAddinPresentation()
		{
			m_bDesignState = false;
			m_strKey = _T("");
			m_strTaskPaneXml = _T("");
			m_pDocGalaxyCluster = nullptr;
			m_pGalaxy = nullptr;
			m_pTaskPaneGalaxyCluster = nullptr;
			m_pTaskPaneGalaxy = nullptr;
		}

		CCloudAddinPresentation::~CCloudAddinPresentation()
		{

		}

		CPowerPntAddin::CPowerPntAddin()
		{
			m_pActivePowerPntObject = nullptr;
			m_pCurrentSavingPresentation = nullptr;
			m_strDesignerToolBarCaption = _T("PowerPoint Designer");
		}

		void CPowerPntAddin::AddDocXml(IDispatch* pDocdisp, BSTR bstrXml, BSTR bstrKey)
		{
			CComQIPtr<PowerPoint::_Presentation> pDoc(pDocdisp);
			if (pDoc)
			{
				CComPtr<Office::_CustomXMLParts> pCustomXMLParts;
				pDoc->get_CustomXMLParts(&pCustomXMLParts);
				_AddDocXml(pCustomXMLParts.p, bstrXml, bstrKey);
			}
		}

		CString CPowerPntAddin::GetDocXmlByKey(IDispatch* pDocdisp, BSTR bstrKey)
		{
			CComQIPtr<PowerPoint::_Presentation> pDoc(pDocdisp);
			if (pDoc)
			{
				CComPtr<Office::_CustomXMLParts> pCustomXMLParts;
				pDoc->get_CustomXMLParts(&pCustomXMLParts);
				CString strRet = _GetDocXmlByKey(pCustomXMLParts.p, bstrKey);
				return strRet;
			}
			return _T("");
		}

		void CPowerPntAddin::OnPresentationBeforeSave(/*[in]*/ _Presentation * Pres,	/*[in,out]*/ VARIANT_BOOL * Cancel) 
		{
			CComBSTR bstrName(L"");
			Pres->get_FullName(&bstrName);
			CString strKey = OLE2T(bstrName);
			m_pCurrentSavingPresentation = nullptr;
			auto it = m_mapHubblePres.find(strKey);
			if (it != m_mapHubblePres.end())
				m_pCurrentSavingPresentation = it->second;
			CString strXml = _T("");
			CTangramXmlParse* pParse = nullptr;
			CTangramXmlParse* pParse2 = nullptr;
			if (m_pCurrentSavingPresentation)
			{
				CComPtr<Office::_CustomXMLParts> pCustomXMLParts;
				Pres->get_CustomXMLParts(&pCustomXMLParts);
				CString strGUID = GetNewGUID();
				CGalaxy* pGalaxy = m_pCurrentSavingPresentation->m_pGalaxy;
				if (pGalaxy)
				{
					pParse = pGalaxy->UpdateGrid();
					strXml = _GetDocXmlByKey(pCustomXMLParts.p, CComBSTR(L"tangram"));
					CTangramXmlParse m_Parse;
					if (m_Parse.LoadXml(strXml))
					{
						pParse2 = m_Parse.GetChild(pParse->name());
						if (pParse2)
						{
							CTangramXmlParse* pParse3 = pParse2->GetChild(TGM_CLUSTER);
							pParse2->RemoveNode(pParse3);
							pParse2->put_text(strGUID);
							strXml = m_Parse.xml();
							strXml.Replace(strGUID, pParse->GetChild(TGM_CLUSTER)->xml());
							AddDocXml(Pres, CComBSTR(strXml), CComBSTR(L"tangram"));
						}
					}
				}
				pGalaxy = m_pCurrentSavingPresentation->m_pTaskPaneGalaxy;
				if (pGalaxy)
				{
					pParse = pGalaxy->UpdateGrid();
					strXml = _GetDocXmlByKey(pCustomXMLParts.p, CComBSTR(L"tangram"));
					CTangramXmlParse m_Parse;
					if (m_Parse.LoadXml(strXml))
					{
						pParse2 = m_Parse.GetChild(pParse->name());
						if (pParse2)
						{
							CTangramXmlParse* pParse3 = pParse2->GetChild(TGM_CLUSTER);
							pParse2->RemoveNode(pParse3);
							pParse2->put_text(strGUID);
							strXml = m_Parse.xml();
							strXml.Replace(strGUID, pParse->GetChild(TGM_CLUSTER)->xml());
							AddDocXml(Pres, CComBSTR(strXml), CComBSTR(L"tangram"));
						}
					}
				}
			}
		}

		void CPowerPntAddin::OnPresentationSave(/*[in]*/ _Presentation * Pres)
		{
			CComBSTR bstrName(L"");
			Pres->get_FullName(&bstrName);
			CString strNewKey = OLE2T(bstrName);
			if (m_pCurrentSavingPresentation&&m_pCurrentSavingPresentation->m_strKey != strNewKey)
			{
				auto it = m_mapHubblePres.find(strNewKey);
				if (it != m_mapHubblePres.end())
				{
					m_mapHubblePres.erase(it);
					m_mapHubblePres[strNewKey] = m_pCurrentSavingPresentation;
					m_pCurrentSavingPresentation->m_strKey = strNewKey;
					m_pCurrentSavingPresentation = nullptr;
				}
			}
		}

		STDMETHODIMP CPowerPntAddin::StartApplication(BSTR bstrAppID, BSTR bstrXml)
		{
			CString strID = OLE2T(bstrAppID);
			strID.MakeLower();
			if (strID.Find(_T("powerpoint.application")) == 0)
				return put_AppKeyValue(CComBSTR(L"doctemplate"), CComVariant(bstrXml));
			return CHubble::StartApplication(bstrAppID, bstrXml);
		}

		STDMETHODIMP CPowerPntAddin::put_AppKeyValue(BSTR bstrKey, VARIANT newVal)
		{
			CString strKey = OLE2T(bstrKey);

			if (strKey == _T(""))
				return S_OK;
			strKey.Trim();
			strKey.MakeLower();
			if (strKey == _T("doctemplate"))
			{
				auto it = g_pHubble->m_mapValInfo.find(_T("doctemplate"));
				if (it != g_pHubble->m_mapValInfo.end())
				{
					::VariantClear(&it->second);
					g_pHubble->m_mapValInfo.erase(it);
				}
				g_pHubble->m_mapValInfo[strKey] = newVal;
				CComPtr<_Presentation> pDoc;
				CComVariant varTemplate(L"");
				CComPtr<PowerPoint::Presentations> pPresentations;
				m_pApplication->get_Presentations(&pPresentations);
				pPresentations->Add(Office::MsoTriState::msoTrue, &pDoc);
			}
			return CHubble::put_AppKeyValue(bstrKey, newVal);
		}

		STDMETHODIMP CPowerPntAddin::HubbleCommand(IDispatch* RibbonControl)
		{
			if (m_spRibbonUI)
				m_spRibbonUI->Invalidate();
			CComBSTR bstrTag(L"");
			CComBSTR bstrID(L"");
			CComQIPtr<IRibbonControl> pIRibbonControl(RibbonControl);
			if (pIRibbonControl)
			{
				pIRibbonControl->get_Id(&bstrID);
				pIRibbonControl->get_Tag(&bstrTag);
				CString strTag = OLE2T(bstrTag);
				if (strTag.CompareNoCase(_T("opentangramfile")) == 0)
				{
					CComPtr<IHubbleDoc> pDoc;
					return this->OpenTangramFile(&pDoc);
				}
				int nPos = strTag.Find(_T("@"));
				CString strPath = m_strAppCommonDocPath + strTag;
				if (::PathFileExists(strPath))
				{
					CComPtr<IHubbleDoc> pDoc;
					return this->OpenHubbleDocFile(strPath.AllocSysString(), &pDoc);
				}
			}
			CString strTag = OLE2T(bstrTag);
			int nPos = strTag.Find(_T("TangramButton.Cmd"));
			strTag.Replace(_T("TangramButton.Cmd."), _T(""));
			int nCmdIndex = _wtoi(strTag);
			CComPtr<DocumentWindow> pWindow;
			m_pApplication->get_ActiveWindow(&pWindow);
			CPowerPntObject* pPntObject = nullptr;
			LONG hMainWnd = 0;
			pWindow->get_HWND(&hMainWnd);
			HWND hWnd = (HWND)hMainWnd;
			auto it = m_mapOfficeObjects.find(hWnd);
			if (it != m_mapOfficeObjects.end())
			{
				pPntObject = (CPowerPntObject*)it->second;
			}
			else
				return S_FALSE;
			CCloudAddinPresentation* pHubblePresentation = pPntObject->m_pPresentation;
			switch (nCmdIndex)
			{
			case 100:
			{
				CGalaxy* pGalaxy = pHubblePresentation->m_pGalaxy;
				if (pGalaxy == nullptr)
					break;
				if (pHubblePresentation->m_bDesignState == false)
				{
					pGalaxy->m_bDesignerState = true;
					pHubblePresentation->m_bDesignState = true;
					CreateCommonDesignerToolBar();
					CGrid* pGrid = pGalaxy->m_pWorkGrid;
					if (pGrid->m_strID.CompareNoCase(TGM_NUCLEUS) == 0)
					{
						CString strXml = _T("<documentui><layout><grid name=\"Start\" /></layout></documentui>");
						IGrid* pDesignNode = nullptr;
						pGalaxy->Observe(CComBSTR(L"default-inDesigning"), CComBSTR(strXml), &pDesignNode);
					}

					m_pDesigningFrame = pGalaxy;
					m_pDesigningFrame->UpdateDesignerTreeInfo();
					break;
				}
				else
				{
					pGalaxy->m_bDesignerState = false;
					pHubblePresentation->m_bDesignState = false;
				}
			}
			break;
			case 101:
			{
				auto iter = m_mapTaskPaneMap.find((LONG)pPntObject->m_hForm);
				if (iter != m_mapTaskPaneMap.end())
				{
					iter->second->put_Visible(true);
				}
				else
				{
					CString strXml = pHubblePresentation->m_strTaskPaneXml;
					if (strXml != _T(""))
					{
						CTangramXmlParse m_Parse;
						if (m_Parse.LoadXml(strXml))
						{
							VARIANT vWindow;
							vWindow.vt = VT_DISPATCH;
							vWindow.pdispVal = nullptr;
							Office::_CustomTaskPane* _pCustomTaskPane;
							CString strCap = _T("");
							strCap = m_Parse.attr(_T("Title"), _T(""));
							if (strCap == _T(""))
								strCap = _T("Tangram for PowerPoint");
							CComBSTR bstrCap(strCap);
							HRESULT hr = m_pCTPFactory->CreateCTP(L"Tangram.Ctrl.1", bstrCap, vWindow, &_pCustomTaskPane);
							_pCustomTaskPane->AddRef();
							_pCustomTaskPane->put_Visible(true);
							m_mapTaskPaneMap[(LONG)pPntObject->m_hForm] = _pCustomTaskPane;
							CComPtr<IHubbleCtrl> pCtrlDisp;
							_pCustomTaskPane->get_ContentControl((IDispatch**)&pCtrlDisp);
							if (pCtrlDisp)
							{
								LONGLONG hWnd = 0;
								pCtrlDisp->get_HWND(&hWnd);
								HWND hPWnd = ::GetParent((HWND)hWnd);
								pPntObject->m_hTaskPane = (HWND)hWnd;
								if (pHubblePresentation->m_pTaskPaneGalaxyCluster == nullptr)
								{
									auto it = g_pHubble->m_mapWindowPage.find(hPWnd);
									if (it != g_pHubble->m_mapWindowPage.end())
										pHubblePresentation->m_pTaskPaneGalaxyCluster = (CGalaxyCluster*)it->second;
									else
									{
										pHubblePresentation->m_pTaskPaneGalaxyCluster = new CComObject<CGalaxyCluster>();
										pHubblePresentation->m_pTaskPaneGalaxyCluster->m_hWnd = hPWnd;
										g_pHubble->m_mapWindowPage[hPWnd] = pHubblePresentation->m_pTaskPaneGalaxyCluster;
									}

									if (pHubblePresentation->m_pTaskPaneGalaxyCluster)
									{
										IGalaxy* pGalaxy = nullptr;
										pHubblePresentation->m_pTaskPaneGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant(hWnd), CComBSTR(L"TaskPane"), &pGalaxy);
										pHubblePresentation->m_pTaskPaneGalaxy = (CGalaxy*)pGalaxy;
										if (pHubblePresentation->m_pTaskPaneGalaxy)
										{
											IGrid* pGrid = nullptr;
											pHubblePresentation->m_pTaskPaneGalaxy->Observe(CComBSTR("Default"), strXml.AllocSysString(), &pGrid);
										}
									}
								}
								else
									pHubblePresentation->m_pTaskPaneGalaxy->ModifyHost(hWnd);
							}
						}
					}
				}
			}
			break;
			case 102:
			{
			}
			break;
			}

			CComPtr<PowerPoint::_Presentation> pPres;
			m_pApplication->get_ActivePresentation(&pPres);
			CCosmosEvent* pObj = new CComObject<CCosmosEvent>;
			pObj->Init(_T("PowerpntPlus"), 0, pPres.p);
			pObj->m_mapVar[0] = CComVariant(bstrID);
			pObj->m_mapVar[1] = CComVariant(bstrTag);

			FireAppEvent(pObj);
			return S_OK;
		}

		HRESULT CPowerPntAddin::OnConnection(IDispatch* pHostApp, int ConnectMode)
		{
			COfficeAddin::OnConnection(pHostApp, ConnectMode);
			if (m_strRibbonXml == _T(""))
			{
				//CTangramXmlParse m_Parse;
				//if (m_Parse.LoadXml(m_strConfigFile) || m_Parse.LoadFile(m_strConfigFile))
				//{
				//	m_strRibbonXml = m_Parse[_T("RibbonUI")][_T("customUI")].xml();
				//}
			}
			pHostApp->QueryInterface(__uuidof(IDispatch), (LPVOID*)&m_pApplication);
			BSTR bstrCap = ::SysAllocString(L"");
			m_pApplication->get_Caption(&bstrCap);
			CString strCaption = OLE2T(bstrCap);
			strCaption += _T(" Plus");
			m_pApplication->put_Caption(strCaption.AllocSysString());
			::SysFreeString(bstrCap);
			HRESULT hr = ((CPowerpointPlusEApplication*)this)->DispEventAdvise(m_pApplication);
			return S_OK;
		}

		HRESULT CPowerPntAddin::OnDisconnection(int DisConnectMode)
		{
			HRESULT hr = ((CPowerpointPlusEApplication*)this)->DispEventUnadvise(m_pApplication);
			m_pApplication.Detach();
			return S_OK;
		}

		STDMETHODIMP CPowerPntAddin::GetCustomUI(BSTR RibbonID, BSTR* RibbonXml)
		{
			if (!RibbonXml)
				return E_POINTER;
			*RibbonXml = m_strRibbonXml.AllocSysString();
			return (*RibbonXml ? S_OK : E_OUTOFMEMORY);
		}

		bool CPowerPntAddin::OnActiveOfficeObj(HWND hWnd)
		{
			auto it = m_mapOfficeObjects2.find(hWnd);
			if (it != m_mapOfficeObjects2.end())
			{
				if (m_pActivePowerPntObject != it->second)
				{
					m_pActivePowerPntObject = (CPowerPntObject*)it->second;

					CCloudAddinPresentation* pHubblePresentation = m_pActivePowerPntObject->m_pPresentation;

					if (pHubblePresentation->m_pGalaxy)
						pHubblePresentation->m_pGalaxy->ModifyHost((LONGLONG)m_pActivePowerPntObject->m_hClient);

					if (pHubblePresentation->m_pTaskPaneGalaxy)
					{
						if (::IsWindow(m_pActivePowerPntObject->m_hTaskPane))
							pHubblePresentation->m_pTaskPaneGalaxy->ModifyHost((LONGLONG)m_pActivePowerPntObject->m_hTaskPane);
						else
							pHubblePresentation->m_pTaskPaneGalaxy->ModifyHost((LONGLONG)m_pActivePowerPntObject->m_hTaskPaneChildWnd);
					}
					CGalaxy* pGalaxy = m_pActivePowerPntObject->m_pPresentation->m_pGalaxy;
					if (pGalaxy)
					{
						if (pGalaxy->m_bDesignerState&&m_pDesignerFrame&&m_pDesigningFrame != pGalaxy)
						{
							m_pDesigningFrame = pGalaxy;
							pGalaxy->UpdateDesignerTreeInfo();
						}
					}
				}
				return true;
			}
			return false;
		}

		void CPowerPntAddin::WindowDestroy(HWND hWnd)
		{
			::GetClassName(hWnd, m_szBuffer, MAX_PATH);
			CString strClassName = CString(m_szBuffer);
			if (strClassName == _T("ThunderDFrame"))
			{
				//OnDestroyVbaForm(hWnd);
				auto it = m_mapVBAForm.find(hWnd);
				if (it != m_mapVBAForm.end())
					m_mapVBAForm.erase(it);
			}
			else if (strClassName == _T("mdiClass"))
			{
				OnCloseOfficeObj(strClassName, hWnd);
			}
		}

		void CPowerPntAddin::WindowCreated(CString strClassName, LPCTSTR strName, HWND hPWnd, HWND hWnd)
		{
			if (strClassName == _T("mdiClass"))
			{
				m_pActivePowerPntObject = new CComObject<CPowerPntObject>;
				m_pActivePowerPntObject->m_hForm = ::GetParent(hPWnd);
				m_pActivePowerPntObject->m_hClient = hPWnd;
				m_pActivePowerPntObject->m_hChildClient = hWnd;
				m_mapOfficeObjects2[m_pActivePowerPntObject->m_hForm] = m_pActivePowerPntObject;
				m_mapOfficeObjects[hWnd] = m_pActivePowerPntObject;
				::PostMessage(m_hHostWnd, WM_OFFICEOBJECTCREATED, (WPARAM)hWnd, 0);
			}
		}

		void CPowerPntAddin::ConnectOfficeObj(HWND hWnd)
		{
			if (m_pApplication == nullptr)
				return;
			auto it = m_mapOfficeObjects.find(hWnd);
			CPowerPntObject* pPowerPntWndObj = (CPowerPntObject*)it->second;
			CComPtr<_Presentation> pDoc;
			m_pApplication->get_ActivePresentation(&pDoc);
			if (pDoc)
			{
				CComBSTR bstrName(L"");
				pDoc->get_Path(&bstrName);
				CString strPath = OLE2T(bstrName);
				pDoc->get_FullName(&bstrName);
				CString strKey = OLE2T(bstrName);

				bool bNewWindow = false;
				CCloudAddinPresentation* pHubblePresentation = nullptr;// 
				auto it = m_mapHubblePres.find(strKey);
				if (it != m_mapHubblePres.end())
				{
					pHubblePresentation = it->second;
					bNewWindow = true;
				}
				else
				{
					pHubblePresentation = new CCloudAddinPresentation();
					m_mapHubblePres[strKey] = pHubblePresentation;
				}
				pHubblePresentation->m_strKey = strKey;
				(*pHubblePresentation)[hWnd] = pPowerPntWndObj;
				pPowerPntWndObj->m_pPresentation = pHubblePresentation;
				if (bNewWindow)
					return;

				if (strPath == _T(""))
				{
					CString strTemplate = GetDocTemplateXml(_T("Please select Slild Template:"), m_strExeName, _T(""));
					CTangramXmlParse m_Parse;
					bool bLoad = m_Parse.LoadXml(strTemplate);
					if (bLoad == false)
						bLoad = m_Parse.LoadFile(strTemplate);
					if (bLoad == false)
						return;
					CString strNewDocInfo = _T("");
					pHubblePresentation->m_strTaskPaneTitle = m_Parse.attr(_T("title"), _T("TaskPane"));
					pHubblePresentation->m_nWidth = m_Parse.attrInt(_T("width"), 200);
					pHubblePresentation->m_nHeight = m_Parse.attrInt(_T("height"), 300);
					pHubblePresentation->m_nMsoCTPDockPosition = (MsoCTPDockPosition)m_Parse.attrInt(_T("dockposition"), 4);
					pHubblePresentation->m_nMsoCTPDockPositionRestrict = (MsoCTPDockPositionRestrict)m_Parse.attrInt(_T("dockpositionrestrict"), 3);
					pPowerPntWndObj->m_pPresentation->m_strDocXml = m_Parse.xml();
					AddDocXml(pDoc, CComBSTR(pPowerPntWndObj->m_pPresentation->m_strDocXml), CComBSTR(L"tangram"));
					CTangramXmlParse* pParse = m_Parse.GetChild(_T("taskpaneui"));
					if (pParse)
					{
						CString strXml = pParse->xml();
						if (strXml != _T(""))
						{
							pPowerPntWndObj->m_pPresentation->m_strTaskPaneXml = strXml;
							auto it = m_mapTaskPaneMap.find((LONG)pPowerPntWndObj->m_hForm);
							if (it != m_mapTaskPaneMap.end())
							{
								it->second->put_Visible(false);
								m_mapTaskPaneMap.erase(it);
							}
							VARIANT vWindow;
							vWindow.vt = VT_DISPATCH;
							vWindow.pdispVal = nullptr;
							Office::_CustomTaskPane* _pCustomTaskPane;
							CString strCap = _T("");
							strCap = pParse->attr(_T("Title"), _T(""));
							if (strCap == _T(""))
								strCap = _T("Tangram for PowerPoint");
							CComBSTR bstrCap(strCap);
							HRESULT hr = m_pCTPFactory->CreateCTP(L"Tangram.Ctrl.1", bstrCap, vWindow, &_pCustomTaskPane);
							_pCustomTaskPane->AddRef();
							_pCustomTaskPane->put_Visible(true);
							m_mapTaskPaneMap[(LONG)pPowerPntWndObj->m_hForm] = _pCustomTaskPane;
							CComPtr<IHubbleCtrl> pCtrlDisp;
							_pCustomTaskPane->get_ContentControl((IDispatch**)&pCtrlDisp);
							if (pCtrlDisp)
							{
								LONGLONG hWnd = 0;
								pCtrlDisp->get_HWND(&hWnd);
								HWND hPWnd = ::GetParent((HWND)hWnd);
								pPowerPntWndObj->m_hTaskPane = (HWND)hWnd;
								if (pHubblePresentation->m_pTaskPaneGalaxyCluster == nullptr)
								{
									auto it = g_pHubble->m_mapWindowPage.find(hPWnd);
									if (it != g_pHubble->m_mapWindowPage.end())
										pHubblePresentation->m_pTaskPaneGalaxyCluster = (CGalaxyCluster*)it->second;
									else
									{
										pHubblePresentation->m_pTaskPaneGalaxyCluster = new CComObject<CGalaxyCluster>();
										pHubblePresentation->m_pTaskPaneGalaxyCluster->m_hWnd = hPWnd;
										g_pHubble->m_mapWindowPage[hPWnd] = pHubblePresentation->m_pTaskPaneGalaxyCluster;
									}

									if (pHubblePresentation->m_pTaskPaneGalaxyCluster)
									{
										IGalaxy* pGalaxy = nullptr;
										pHubblePresentation->m_pTaskPaneGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant(hWnd), CComBSTR(L"TaskPane"), &pGalaxy);
										pHubblePresentation->m_pTaskPaneGalaxy = (CGalaxy*)pGalaxy;
										if (pHubblePresentation->m_pTaskPaneGalaxy)
										{
											IGrid* pGrid = nullptr;
											pHubblePresentation->m_pTaskPaneGalaxy->Observe(CComBSTR("Default"), strXml.AllocSysString(), &pGrid);
										}
									}
								}
								else
									pHubblePresentation->m_pTaskPaneGalaxy->ModifyHost(hWnd);
							}
						}
					}
					pParse = m_Parse.GetChild(_T("documentui"));
					if (pParse)
					{
						IGalaxyCluster* pGalaxyCluster = nullptr;
						CreateGalaxyCluster((LONGLONG)pPowerPntWndObj->m_hForm, &pGalaxyCluster);
						pPowerPntWndObj->m_pPresentation->m_pDocGalaxyCluster = (CGalaxyCluster*)pGalaxyCluster;
						IGalaxy* pGalaxy = nullptr;
						pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)pPowerPntWndObj->m_hClient), CComBSTR(L"Document"), &pGalaxy);
						pPowerPntWndObj->m_pPresentation->m_pGalaxy = (CGalaxy*)pGalaxy;
						IGrid* pGrid = nullptr;
						pGalaxy->Observe(CComBSTR(L"Default"), CComBSTR(pParse->xml()), &pGrid);
					}
				}
				else
				{
					CString strXML = GetDocXmlByKey(pDoc, CComBSTR(L"tangram"));
					pPowerPntWndObj->m_pPresentation->m_strDocXml = strXML;

					if (strXML != _T(""))
					{
						CTangramXmlParse m_Parse;
						if (m_Parse.LoadXml(strXML) || m_Parse.LoadFile(strXML))
						{
							CTangramXmlParse* pParse = m_Parse.GetChild(_T("taskpaneui"));
							if (pParse)
							{
								CString strXml = pParse->xml();
								if (pPowerPntWndObj != nullptr)
								{
									pPowerPntWndObj->m_pPresentation->m_strTaskPaneXml = strXml;
									auto it = m_mapTaskPaneMap.find((LONG)pPowerPntWndObj->m_hForm);
									if (it != m_mapTaskPaneMap.end())
									{
										it->second->put_Visible(false);
										m_mapTaskPaneMap.erase(it);
									}
									VARIANT vWindow;
									vWindow.vt = VT_DISPATCH;
									vWindow.pdispVal = nullptr;
									Office::_CustomTaskPane* _pCustomTaskPane;
									CString strCap = _T("");
									strCap = pParse->attr(_T("Title"), _T(""));
									if (strCap == _T(""))
										strCap = _T("Tangram for PowerPoint");
									CComBSTR bstrCap(strCap);
									HRESULT hr = m_pCTPFactory->CreateCTP(L"Tangram.Ctrl.1", bstrCap, vWindow, &_pCustomTaskPane);
									_pCustomTaskPane->AddRef();
									_pCustomTaskPane->put_Visible(true);
									m_mapTaskPaneMap[(LONG)pPowerPntWndObj->m_hForm] = _pCustomTaskPane;
									CComPtr<IHubbleCtrl> pCtrlDisp;
									_pCustomTaskPane->get_ContentControl((IDispatch**)&pCtrlDisp);
									if (pCtrlDisp)
									{
										LONGLONG hWnd = 0;
										pCtrlDisp->get_HWND(&hWnd);
										HWND hPWnd = ::GetParent((HWND)hWnd);
										pPowerPntWndObj->m_hTaskPane = (HWND)hWnd;
										if (pHubblePresentation->m_pTaskPaneGalaxyCluster == nullptr)
										{
											auto it = g_pHubble->m_mapWindowPage.find(hPWnd);
											if (it != g_pHubble->m_mapWindowPage.end())
												pHubblePresentation->m_pTaskPaneGalaxyCluster = (CGalaxyCluster*)it->second;
											else
											{
												pHubblePresentation->m_pTaskPaneGalaxyCluster = new CComObject<CGalaxyCluster>();
												pHubblePresentation->m_pTaskPaneGalaxyCluster->m_hWnd = hPWnd;
												g_pHubble->m_mapWindowPage[hPWnd] = pHubblePresentation->m_pTaskPaneGalaxyCluster;
											}

											if (pHubblePresentation->m_pTaskPaneGalaxyCluster)
											{
												IGalaxy* pGalaxy = nullptr;
												pHubblePresentation->m_pTaskPaneGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant(hWnd), CComBSTR(L"TaskPane"), &pGalaxy);
												pHubblePresentation->m_pTaskPaneGalaxy = (CGalaxy*)pGalaxy;
												if (pHubblePresentation->m_pTaskPaneGalaxy)
												{
													IGrid* pGrid = nullptr;
													pHubblePresentation->m_pTaskPaneGalaxy->Observe(CComBSTR("Default"), strXml.AllocSysString(), &pGrid);
												}
											}
										}
										else
											pHubblePresentation->m_pTaskPaneGalaxy->ModifyHost(hWnd);
									}
								}
							}
							pParse = m_Parse.GetChild(_T("documentui"));
							if (pParse)
							{
								IGalaxyCluster* pGalaxyCluster = nullptr;
								CreateGalaxyCluster((LONGLONG)pPowerPntWndObj->m_hForm, &pGalaxyCluster);
								pPowerPntWndObj->m_pPresentation->m_pDocGalaxyCluster = (CGalaxyCluster*)pGalaxyCluster;
								IGalaxy* pGalaxy = nullptr;
								pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((long)pPowerPntWndObj->m_hClient), CComBSTR(L"Document"), &pGalaxy);
								pPowerPntWndObj->m_pPresentation->m_pGalaxy = (CGalaxy*)pGalaxy;
								IGrid* pGrid = nullptr;
								pGalaxy->Observe(CComBSTR(L"Default"), CComBSTR(pParse->xml()), &pGrid);
							}
						}
					}
				}
			}
		}

		HRESULT CPowerPntAddin::CreateHubbleCtrl(void* pv, REFIID riid, LPVOID* ppv)
		{
			return CPowerPntAppCtrl::_CreatorClass::CreateInstance(pv, riid, ppv);
		}

		CPowerPntAppCtrl::CPowerPntAppCtrl()
		{

		}

		STDMETHODIMP CPowerPntAppCtrl::put_AppCtrl(VARIANT_BOOL newVal)
		{
			g_pHubble->m_pHubbleAppCtrl = this;
			return S_OK;
		}
		void CPowerPntAppCtrl::OnFinalMessage(HWND hWnd)
		{
			__super::OnFinalMessage(hWnd);
		}

		STDMETHODIMP CPowerPntAppCtrl::get_tag(VARIANT* pVal)
		{
			return S_OK;
		}


		STDMETHODIMP CPowerPntAppCtrl::put_tag(VARIANT newVal)
		{
			return S_OK;
		}

		STDMETHODIMP CPowerPntAppCtrl::get_HWND(LONGLONG* pVal)
		{
			*pVal = (long)m_hWnd;
			return S_OK;
		}

		STDMETHODIMP CPowerPntAppCtrl::get_Hubble(IHubble** pVal)
		{
			*pVal = g_pHubble;
			return S_OK;
		}
	}
}// namespace OfficePlus

