
#include "pch.h"
#include "framework.h"
#include "ViewTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree() noexcept
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_MESSAGE(WM_COSMOSMSG, OnCosmosMsg)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

LRESULT CViewTree::OnCosmosMsg(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CWnd::DefWindowProc(WM_COSMOSMSG, wParam, lParam);
	if (lParam == 10000)
	{
		HANDLE hData = ::GetProp(m_hWnd, _T("CosmosData"));
		if (hData)
		{
			CTangramXmlParse* pParse = (CTangramXmlParse*)hData;
			if (pParse)
			{
				int nID = pParse->attrInt(_T("clientid"), 0);
				if (nID == ::GetDlgCtrlID(m_hWnd))
				{
					CString xml = pParse->xml();
				}
			}
		}
	}
	return lRes;
}
void CViewTree::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString strText = _T("");
	GetParent()->GetWindowText(strText);
	CString strXml = _T("");
	CString strKey = _T("default");
	CosmosUIItemData* pTreeItemData = (CosmosUIItemData*)pNMTreeView->itemNew.lParam;
	if (pNMTreeView->itemNew.hItem != GetRootItem())
	{
		if (pTreeItemData && pTreeItemData->m_strKey != _T(""))
			strKey = strText + _T("_") + pTreeItemData->m_strKey;
	}
	strKey.Replace(_T(" "), _T("_"));
	*pResult = 0;
}

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != nullptr);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}
