// StatisticListView.cpp : implementation of the CStatisticListView class
//

#include "stdafx.h"
#include "WinProf.h"

#include "WinProfDoc.h"
#include "StatisticListView.h"
#include "MainFrm.h"
#include ".\statisticlistview.h"
#include "StatisticsDialog.h"
#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct LIST_ITEM_DATA
{
	int line;
	CString name;
	DWORD address;
	CString runtime;
	LIST_ITEM_DATA(int line_, CString name_, DWORD address_, CString runtime_)
		: line(line_), name(name_), address(address_), runtime(runtime_) {}
};

using namespace std;

// CStatisticListView

IMPLEMENT_DYNCREATE(CStatisticListView, CListView)

BEGIN_MESSAGE_MAP(CStatisticListView, CListView)
	ON_WM_STYLECHANGED()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnLvnColumnclick)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
END_MESSAGE_MAP()

// CStatisticListView construction/destruction

CStatisticListView::CStatisticListView()
{
	// TODO: add construction code here

}

CStatisticListView::~CStatisticListView()
{
}

BOOL CStatisticListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

#define NUM_COLUMNS 3

static _TCHAR *_ColumnName[NUM_COLUMNS] =
{_T("#"), _T("Function Name"), _T("Run Time (ms)")};

static int _ColumnPlace[NUM_COLUMNS] =
{LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT};

static int _ColumnWidth[NUM_COLUMNS] =
{70, 250, 100};

// CStatisticListView diagnostics

#ifdef _DEBUG
void CStatisticListView::AssertValid() const
{
	CListView::AssertValid();
}

void CStatisticListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CWinProfDoc* CStatisticListView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinProfDoc)));
	return (CWinProfDoc*)m_pDocument;
}
#endif //_DEBUG


// CStatisticListView message handlers
void CStatisticListView::OnStyleChanged(int /*nStyleType*/, LPSTYLESTRUCT /*lpStyleStruct*/)
{
	//TODO: add code to react to the user changing the view style of your window
	Default();
}

void CStatisticListView::InsertLine(int lineNumber, CString str[NUM_COLUMNS-1], DWORD address)
{
	CListCtrl& ctrl = GetListCtrl();
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = lineNumber;
	lvi.lParam = lineNumber;
	lvi.iSubItem = 0;

	char buf[100];
	itoa(lineNumber, buf, 10);
	lvi.pszText =  buf;

	lvi.iImage = lineNumber;
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = INDEXTOSTATEIMAGEMASK(1);
	ctrl.SetItemData(ctrl.InsertItem(&lvi), (DWORD_PTR)new LIST_ITEM_DATA(lineNumber, str[0], address, str[1]));

	for(int j = 1; j<NUM_COLUMNS; j++)
		ctrl.SetItemText(lineNumber-1, j, str[j-1]);
}

int CStatisticListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CListCtrl& ListCtrl = GetListCtrl();
	ModifyStyle(LVS_TYPEMASK, LVS_REPORT | LVS_SINGLESEL);
	ListCtrl.SetExtendedStyle(ListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	// building columns
	int i;
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _ColumnName[i];
		lvc.cx = _ColumnWidth[i];
		lvc.fmt = _ColumnPlace[i];
		ListCtrl.InsertColumn(i,&lvc);
	}
	// clean the table for further use

	return 0;
}

void CStatisticListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

void CStatisticListView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int item = reinterpret_cast<NMITEMACTIVATE *>(pNMHDR)->iItem;
	if (item == -1) return;
	CListCtrl& ctrl = GetListCtrl();
	function_call_map_t& function_call_map = static_cast<CMainFrame*>(AfxGetMainWnd())->function_call_map;
	DWORD address = reinterpret_cast<LIST_ITEM_DATA*>(ctrl.GetItemData(item))->address;
	function_call_map_t::const_iterator iter = function_call_map.find(address);
	ASSERT(iter != function_call_map.end());
	double avg = (double)iter->second.time / GetDocument()->m_Frequency * 1000 / iter->second.count;
	CStatisticsDialog dlg(address, ctrl.GetItemText(item, 1), iter->second.count, avg);
	dlg.DoModal();
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int column = (int)lParamSort & 0xFF;
	int direction = ((int)lParamSort >> 8) & 0xFF;
	LIST_ITEM_DATA *l1, *l2;
	if (direction)
	{
		l1 = (LIST_ITEM_DATA *)lParam2;
		l2 = (LIST_ITEM_DATA *)lParam1;
	}
	else
	{
		l1 = (LIST_ITEM_DATA *)lParam1;
		l2 = (LIST_ITEM_DATA *)lParam2;
	}
	switch (column)
	{
	case 0:		// Line#
		return l1->line - l2->line;
	case 1:		// Function Name
		return l1->name.Compare(l2->name);
	case 2:		// RunTime
		if (l1->runtime.GetLength() == l2->runtime.GetLength())
			return l1->runtime.Compare(l2->runtime);
		else
			return l1->runtime.GetLength() - l2->runtime.GetLength();
	}
	return 0;
}

void CStatisticListView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
//	pair<CListCtrl*, int> p(&GetListCtrl(), pNMLV->iSubItem);
	for (int i=0; i<GetListCtrl().GetItemCount(); i++)
	{
		GetListCtrl().GetItemText(i, 1);
	}
	static int last_column = 0;
	static int direction = 0;
	if (last_column == pNMLV->iSubItem)
	{
		direction = ! direction;
	}
	else
	{
		direction = 0;
		last_column = pNMLV->iSubItem;
	}
	GetListCtrl().SortItems(CompareFunc, (direction<<8) + last_column);
}

void CStatisticListView::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	delete (void *)pNMLV->lParam;
	*pResult = 0;
}
