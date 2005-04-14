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
#include "StatManager.h"
#include "WinProfStatistics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

void CStatisticListView::InsertLine(int lineNumber, INVOC_INFO* invoc_info)
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
	ctrl.SetItemData(ctrl.InsertItem(&lvi), (DWORD_PTR)new LIST_ITEM_DATA(lineNumber, invoc_info->address));

	// the function name is set manually
	CWinProfDoc* doc = GetDocument();
	ctrl.SetItemText(lineNumber-1, 1, doc->symbol_manager.GetSymName(invoc_info->address));

	// the rest of information
	statistics_t& stats = GetDocument()->stat_manager.GetStats();
	for(int j = 0; j < (int)stats.size(); j++)
	{
		stats_type st = (stats_type)j;
		if(stats[st]->IsVisible() == false) continue;
		ctrl.SetItemText(lineNumber-1, j+2, stats[st]->GetString(*invoc_info));
	}
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
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// the first 2 columns must always be the number and the function name
	for(int i = 0; i < 2; ++i) {
		lvc.iSubItem = i;
		lvc.pszText = _ColumnName[i];
		lvc.cx = _ColumnWidth[i];
		lvc.fmt = _ColumnPlace[i];
		ListCtrl.InsertColumn(i, &lvc);
	}
	
	// build all the rest of columns
	statistics_t& stats = GetDocument()->stat_manager.GetStats();
	for(int i = 0; i < (int)stats.size(); i++) 
	{
		stats_type st = (stats_type)i;
		if(stats[st]->IsVisible() == false) continue;
		lvc.iSubItem = i+2;
		char buf[100];
		strcpy(buf, stats[st]->GetStatName().GetString());
		lvc.pszText = buf;
		lvc.cx = stats[st]->GetWidth();
		lvc.fmt = stats[st]->GetColumnPlace();
		ListCtrl.InsertColumn(i+2, &lvc);
	}

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
	LIST_ITEM_DATA* data = (LIST_ITEM_DATA*)ctrl.GetItemData(item);
	CStatisticsDialog dlg(GetDocument(), data->address);
	dlg.DoModal();
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int column = (int)lParamSort & 0xFF;
	int direction = ((int)lParamSort >> 8) & 0xFF;
	LIST_ITEM_DATA *d1, *d2;
	CSymbolManager* man = LIST_ITEM_DATA::man;
	CStatManager* stats = LIST_ITEM_DATA::stats;
	if (direction) {
		d1 = (LIST_ITEM_DATA*)lParam2;
		d2 = (LIST_ITEM_DATA*)lParam1;
	} else {
		d1 = (LIST_ITEM_DATA*)lParam1;
		d2 = (LIST_ITEM_DATA*)lParam2;
	}
	switch (column)
	{
	case 0:		// Line#
		return d1->line - d2->line;
	case 1:		// Function Name
		return man->GetSymName(d1->address).Compare(man->GetSymName(d2->address));
	default:	// A statistic
		return stats->GetStats()[stats_type(column-2)]->StatCompare(INVOC_INFO(d1->address, 0), INVOC_INFO(d2->address, 0));
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
	delete (LIST_ITEM_DATA*)pNMLV->lParam;
	*pResult = 0;
}
