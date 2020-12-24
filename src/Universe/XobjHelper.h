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

class CXobjHelper : public CWnd
{
	DECLARE_DYNCREATE(CXobjHelper)
public:
	BOOL					m_bNoMove;
	BOOL					m_bBKWnd;
	BOOL					m_bEraseBkgnd;
	BOOL					m_bCreateExternal;
	HWND					m_hFormWnd;

	CString					m_strKey;
	CString					m_strXml;

	CXobj*					m_pXobj;
	CXobj*					m_pParentXobj;

	IOleInPlaceActiveObject* m_pOleInPlaceActiveObject;

	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CXobjHelper();           // protected constructor used by dynamic creation
	virtual ~CXobjHelper();
	void PostNcDestroy();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnActiveTangramObj(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCosmosMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTabChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetCosmosObj(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSplitterReposition(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
};
