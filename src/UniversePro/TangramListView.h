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

// CTangramListView view
class CTangramTabCtrl;
class CTangramListView : public CListView
{
	DECLARE_DYNCREATE(CTangramListView)

protected:
	CTangramListView();           // protected constructor used by dynamic creation
	virtual ~CTangramListView();

public:
	IGalaxy* m_pGalaxy;
	CTangramTabCtrl* m_pCosmosTabCtrl;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL			m_bCreated;
	BOOL			m_bFilling;
	int				m_nStyle;
	int				m_nActiveIndex;
	CXobj*		m_pXobj;
	CImageList		m_ImageList;

	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnActiveTangramPage(WPARAM wParam, LPARAM lParam);
public:
	void InitTabCtrl(CTangramTabCtrl* pTabCtrl);
	void ChangeTemplate(int nItem);
};


