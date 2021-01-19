/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101190013
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

#pragma once

// CDocTemplateDlg dialog

class CDocTemplateDlg : public CDialog
{
	DECLARE_DYNAMIC(CDocTemplateDlg)
public:
	CDocTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDocTemplateDlg();

// Dialog Data
	enum { IDD = IDD_DOCTEMPLATE };

	CString m_strDir;
	CString m_strFilter;
	CString m_strCaption;
	CString strCurTabText;
	CString m_strTemplateDoc;
	CString m_strDocTemplatePath;
	CTabCtrl m_tabCtrl;
	CListCtrl m_TemplateList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnLvnItemchangedTemplatelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTemplatelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeTemplatetab(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};
