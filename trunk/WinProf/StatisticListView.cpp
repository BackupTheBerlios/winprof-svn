// StatisticListView.cpp : implementation of the CStatisticListView class
//

#include "stdafx.h"
#include "WinProf.h"

#include "WinProfDoc.h"
#include "StatisticListView.h"
#include ".\statisticlistview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStatisticListView

IMPLEMENT_DYNCREATE(CStatisticListView, CListView)

BEGIN_MESSAGE_MAP(CStatisticListView, CListView)
	ON_WM_STYLECHANGED()
	ON_WM_CREATE()
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
{_T("#"), _T("Function Name"), _T("Run Time")};

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

void CStatisticListView::InsertLine(int lineNumber, CString str[NUM_COLUMNS-1])
{
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = lineNumber;
	lvi.iSubItem = 0;

	char buf[100];
	itoa(lineNumber, buf, 10);
	lvi.pszText =  buf;

	lvi.iImage = lineNumber;
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = INDEXTOSTATEIMAGEMASK(1);
	GetListCtrl().InsertItem(&lvi);

	for(int j = 1; j<NUM_COLUMNS; j++)
		GetListCtrl().SetItemText(lineNumber-1, j, str[j-1]);
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
//	GetListCtrl().DeleteAllItems();
}
