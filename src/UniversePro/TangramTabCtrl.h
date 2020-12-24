/********************************************************************************
 *					DOM Plus for Application - Version 1.1.7.40
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

// CTangramListCtrl

class CTangramListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CTangramListCtrl)

public:
	CTangramListCtrl();
	virtual ~CTangramListCtrl();
	int					m_nListViewIndex;
	int					m_nListViewSelectedIndex;
	CString				m_strDir;
	CString				m_strSubDir;
	CTangramTabCtrl*	m_pCosmosTabCtrl;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

// CTangramTabCtrl

class CTangramTabCtrl : public CTabCtrl
{
	DECLARE_DYNCREATE(CTangramTabCtrl)

public:
	CTangramTabCtrl();
	virtual ~CTangramTabCtrl();
	int					m_nImageIndex;
	CString				m_strFilter;
	CXobj*			m_pXobj;
	CGalaxy*		m_pGalaxy;
	CTangramListView*	m_pCosmosListView;
	CTangramListCtrl	m_ListCtrl;
	void RePosition();
	void FillListCtrl();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg LRESULT OnInitialUpdate(WPARAM wParam, LPARAM lParam);
};


