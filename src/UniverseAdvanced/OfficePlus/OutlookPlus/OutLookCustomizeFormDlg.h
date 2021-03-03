/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103040042           *
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
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

// OutLookCustomizeFormDlg.h : COutLookCustomizeFormDlg implementation file

#pragma once

#include "../../resource.h"      
#include <atlhost.h>
#include <atlstr.h>
using namespace ATL;

// COutLookCustomizeFormDlg

namespace OfficePlus
{
	namespace OutLookPlus
	{
		class COutLookInspector;
		class COutLookCustomizeFormDlg :
			public CAxDialogImpl<COutLookCustomizeFormDlg>
		{
		public:
			COutLookCustomizeFormDlg();
			~COutLookCustomizeFormDlg();

			enum { IDD = IDD_TANGRAMOUTLOOKCUSTOMIZEFORMDLG };

			CString				m_strName;
			COutLookInspector*	m_pOutLookInspector;
			BEGIN_MSG_MAP(COutLookCustomizeFormDlg)
				MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
				COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
				COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
				CHAIN_MSG_MAP(CAxDialogImpl<COutLookCustomizeFormDlg>)
			END_MSG_MAP()

			LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
			LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
			LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		};
	}
}
