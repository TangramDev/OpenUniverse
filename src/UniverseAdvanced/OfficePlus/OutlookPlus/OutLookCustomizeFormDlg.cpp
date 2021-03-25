/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103250054
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

// TangramOutLookCustomizeFormDlg.cpp : COutLookCustomizeFormDlg implementation
#include "../../stdafx.h"
#include "../../UniverseApp.h"
#include "../../Galaxy.h"
#include "OutLookAddin.h"
#include "OutLookCustomizeFormDlg.h"

// COutLookCustomizeFormDlg
namespace OfficePlus
{
	namespace OutLookPlus
	{
		COutLookCustomizeFormDlg::COutLookCustomizeFormDlg()
			: m_strName(_T(""))
		{
			m_pOutLookInspector = NULL;
		}

		COutLookCustomizeFormDlg::~COutLookCustomizeFormDlg()
		{
		}

		LRESULT COutLookCustomizeFormDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			CAxDialogImpl<COutLookCustomizeFormDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);
			bHandled = true;
			return 1;  
		}

		LRESULT COutLookCustomizeFormDlg::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			if (m_pOutLookInspector)
			{
				OutLook::Pages* pPages = NULL;
				m_pOutLookInspector->m_pInspector->get_ModifiedFormPages((IDispatch**)&pPages);
				HWND hwnd = ::GetDlgItem(m_hWnd, IDC_EDIT_OUTLOOKPAGE);
				::GetWindowText(hwnd, g_pCosmos->m_szBuffer, 255);
				m_strName = CString(g_pCosmos->m_szBuffer);
				m_strName.Trim();
				if (m_strName != _T(""))
				{
					CComPtr<IDispatch> pItem;
					pPages->Item(CComVariant(L""), &pItem);
					if (pItem)
					{
						::MessageBox(m_hWnd, _T("Page is exists, please input a new name!"), _T("Customize OutLook Form"), MB_OK);
						return 0;
					}
					else
					{
						pPages->Add(CComVariant(m_strName), &pItem);
						if (pItem)
						{
							BSTR bstrName = m_strName.AllocSysString();
							m_pOutLookInspector->m_pInspector->ShowFormPage(bstrName);
							CComQIPtr<IOleWindow> pWnd(pItem);
							if (pWnd)
							{
								HWND hWnd = 0;
								pWnd->GetWindow(&hWnd);
								if (hWnd)
								{
									COutLookPageWnd* pOutLookPageWnd = new COutLookPageWnd();
									pOutLookPageWnd->m_pOutLookInspector = m_pOutLookInspector;
									pOutLookPageWnd->m_hFrameWnd = ::GetParent(hWnd);
									HWND hParent = ::GetParent(pOutLookPageWnd->m_hFrameWnd);
									::ShowWindow(hParent, SW_HIDE);
									pOutLookPageWnd->m_strName = m_strName;
									CString strXml = _T("");
									CString strName = m_strName;
									strName.Replace(_T(" "),_T("_"));
									strXml.Format(_T("<%s><cluster><xobj name=\"Start\" /></cluster></%s>"), strName, strName);
									pOutLookPageWnd->m_strXml =strXml;
									CComQIPtr<MSForm::_UserForm> pForm(pItem);
									pOutLookPageWnd->m_pForm = pForm.p;
									pOutLookPageWnd->m_pForm->AddRef();
									pOutLookPageWnd->SubclassWindow(hParent);
									m_pOutLookInspector->m_mapOutLookPage[m_strName] = pOutLookPageWnd;
									pOutLookPageWnd->DesignPage();
								}
								m_pOutLookInspector->m_pInspector->SetCurrentFormPage(bstrName);
								::SysFreeString(bstrName);
							}
						}
					}
				}
				else
				{
					return 0;
				}
			}
			EndDialog(wID);
			return 0;
		}

		LRESULT COutLookCustomizeFormDlg::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			EndDialog(wID);
			return 0;
		}
	}
}