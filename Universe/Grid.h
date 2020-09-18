/********************************************************************************
*					Open Universe - version 0.8.0								*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
********************************************************************************/

#pragma once

// CGrid

class CGrid : public CSplitterWnd
{
	DECLARE_DYNCREATE(CGrid)
public:
	void Save();
	void RecalcLayout();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CStar* m_pHostNode;
	bool bInited;
	int m_nHostWidth, m_nHostHeight;
	int m_Vmin,m_Vmax,m_Hmin,m_Hmax;
	int m_nNeedRefreshCol;
	int m_nMasterRow;
	int m_nMasterCol;
	COLORREF		rgbLeftTop;
	COLORREF		rgbMiddle;
	COLORREF		rgbRightBottom;
protected:
	CGrid();           // protected constructor used by dynamic creation
	virtual ~CGrid();

	BOOL			m_bCreated;
	bool			m_bNeedRefreh;
	CStar*			m_pStar;

	BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void StartTracking(int ht);
	void StopTracking(BOOL bAccept);
	void TrackRowSize(int y, int row);
	void TrackColumnSize(int x, int col);
	void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	void PostNcDestroy();
	void DrawAllSplitBars(CDC* pDC, int cxInside, int cyInside);
	CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	//void RefreshNode(IStar*);

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnActivePage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSplitterNodeAdd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnActiveTangramObj(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetTangramObj(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSplitterCreated(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void _RecalcLayout();
	void TangramLayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray, int nMax, int nSize, int nSizeSplitter, CStar* pHostNode, bool bCol);
	void TangramDeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout, CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar);
};
