// StatisticListView.cpp : implementation of the CStatisticListView class
//

#include "stdafx.h"
#include "WinProf.h"

#include "WinProfDoc.h"
#include "StatisticListView.h"
#include "MainFrm.h"
#include ".\statisticlistview.h"
#include "StatisticsDialog.h"
#include "FilterDialog.h"
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
	ON_COMMAND(ID_PROJECT_FILTER, OnProjectFilter)
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

#define NUM_COLUMNS 2

static _TCHAR *_ColumnName[NUM_COLUMNS] =
{_T("#"), _T("Function Name")};

static int _ColumnPlace[NUM_COLUMNS] =
{LVCFMT_LEFT, LVCFMT_LEFT};

static int _ColumnWidth[NUM_COLUMNS] =
{40, 250};

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

void CStatisticListView::InsertLine(int lineNumber, const INVOC_INFO& invoc_info)
{
	CListCtrl& ctrl = GetListCtrl();

	ctrl.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE, lineNumber, 
		Format("%d", lineNumber), INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK, lineNumber, 
		reinterpret_cast<LPARAM>(new LIST_ITEM_DATA(lineNumber, invoc_info)));

	// the function name is set manually
	CString name = GetDocument()->symbol_manager.GetSymName(invoc_info.address);
	if (name.IsEmpty()) name.Format("0x%x", invoc_info.address);
	name.AppendFormat(" (%d)", invoc_info.invocation);

	ctrl.SetItemText(lineNumber-1, 1, name);

	// the rest of information
	statistics_t& stats = GetDocument()->stat_manager.GetStats();
	int counter = 2;
	for (int j = 0; j < (int)stats.size(); j++)
	{
		if (stats[j]->IsPerInvocation())
			ctrl.SetItemText(lineNumber-1, counter++, stats[j]->GetString(invoc_info));
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

	// the first 2 columns must always be the number and the function name
	for(int i = 0; i < 2; ++i)
		ListCtrl.InsertColumn(i, _ColumnName[i], _ColumnPlace[i], _ColumnWidth[i], i);
	
	// build all the rest of columns
	statistics_t& stats = GetDocument()->stat_manager.GetStats();
	int counter = 2;
	for(int i = 0; i < (int)stats.size(); i++) 
		if (stats[i]->IsPerInvocation())
			ListCtrl.InsertColumn(counter++, stats[i]->GetStatCaption(), stats[i]->GetColumnPlace(), stats[i]->GetWidth(), i+2);

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
	CStatisticsDialog dlg(GetDocument()->stat_manager, GetDocument()->symbol_manager, data->invoc_info.address);
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
		return man->GetSymName(d1->invoc_info.address).Compare(man->GetSymName(d2->invoc_info.address));
	default:	// A statistic
		return stats->GetStats()[column-2]->StatCompare(d1->invoc_info, d2->invoc_info);
	}
	return 0; 
} 

void CStatisticListView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
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

void CStatisticListView::BuildFilteredList(void)
{
	filtered_list_t list;
	GetListCtrl().DeleteAllItems();
	GetDocument()->filter_manager.Filter(GetDocument()->m_ActiveFilter, 
		static_cast<CMainFrame*>(AfxGetMainWnd())->GetLeftPane()->GetTreeCtrl(), list);
	int line = 1;
	for (filtered_list_t::const_iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		InsertLine(line++, *(*iter));
	}
}

void CStatisticListView::OnProjectFilter()
{
	CFilterDialog dlg(GetDocument());
	if (dlg.DoModal() == IDCANCEL) return;
	GetDocument()->m_ActiveFilter = dlg.m_ActiveFilter;
	BuildFilteredList();
}
