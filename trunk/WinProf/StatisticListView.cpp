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
//	ON_WM_CREATE()
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

#define NUM_COLUMNS 4
#define NUM_ITEMS   7
static _TCHAR *_ColumnName[NUM_COLUMNS] =
{
	_T("FunctionName"), _T("Time"), _T("NetTime"), _T("Num Of Calls")
};
static int _ColumnPlace[NUM_COLUMNS] =
{
	LVCFMT_LEFT, LVCFMT_CENTER,LVCFMT_CENTER, LVCFMT_CENTER
};

static int _ColumnWidth[NUM_COLUMNS] =
{
	150, 75, 75, 100
};

static _TCHAR *_gszItem[NUM_ITEMS][NUM_COLUMNS] =
{
	_T("foo1"),  _T("255"), _T("255"), _T("0"),  
	_T("foo2"),     _T("255"), _T("0"),   _T("0"), 
	_T("foo3"),   _T("0"),   _T("255"), _T("0"),  
	_T("foo4"), _T("255"), _T("0"),   _T("255"),
	_T("foo5"),    _T("0"),   _T("255"), _T("255"),
	_T("foo6"),    _T("0"),   _T("0"),   _T("255"),
	_T("foo7"),    _T("192"), _T("192"), _T("192"), 
};



void CStatisticListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();
	ModifyStyle(0, LVS_REPORT);

// building columns
	int i, j;
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
	OnUpdate(NULL, NULL, NULL);
	//this->Update();
// insert items

}

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
 void  CStatisticListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	GetListCtrl().DeleteAllItems();
	LV_ITEM lvi;
	for(int i = 0; i < 3; i++)
	{
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = _gszItem[i][0];
		lvi.iImage = i;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(1);

		GetListCtrl().InsertItem(&lvi);
	}
	LPCTSTR s = "12";
	for(i = 0; i<3; i++)
	{
		for(int j = 1; j<NUM_COLUMNS; j++)
		{
			GetListCtrl().SetItemText(i,j,_gszItem[i][j]);
		}
	}
	
}
