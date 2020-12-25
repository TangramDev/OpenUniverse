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
