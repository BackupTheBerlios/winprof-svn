// FunctionTreeView.cpp : implementation of the CFunctionTreeView class
//

#include "stdafx.h"
#include "WinProf.h"

#include "WinProfDoc.h"
#include "FunctionTreeView.h"
#include "..\CallMon\CallMon.h"
#include "MainFrm.h"
#include "StatisticListView.h"
#include "SymbolManager.h"
#include ".\functiontreeview.h"
#include <vector>

using namespace std;
using namespace stdext;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFunctionTreeView

IMPLEMENT_DYNCREATE(CFunctionTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CFunctionTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnTvnDeleteitem)
END_MESSAGE_MAP()

CSymbolManager* LIST_ITEM_DATA::man = NULL;
CStatManager* LIST_ITEM_DATA::stats = NULL;

// used in CFunctionTreeView::FillTheTree only
class INVOC_INFO_EX {
public:
	INVOC_INFO_EX(DWORD a, DWORD64 t) : address(a), time(t) {}
	DWORD address;
	DWORD64 time;
}; // class INVOC_INFO_EX

// CFunctionTreeView construction/destruction

CFunctionTreeView::CFunctionTreeView()
{
	// TODO: add construction code here
}

CFunctionTreeView::~CFunctionTreeView()
{
}

BOOL CFunctionTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CFunctionTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	
	GetTreeCtrl().ModifyStyle(NULL,	TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS);

	// TODO: You may populate your TreeView with items by directly accessing
	// its tree control through a call to GetTreeCtrl().
	FillTheTree();
}


// CFunctionTreeView diagnostics

#ifdef _DEBUG
void CFunctionTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CFunctionTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CWinProfDoc* CFunctionTreeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinProfDoc)));
	return (CWinProfDoc*)m_pDocument;
}
#endif //_DEBUG

void CFunctionTreeView::FillTheTree()
{
	// I cannot find a better place to initialize this... shouldn't be here
	LIST_ITEM_DATA::man = &GetDocument()->symbol_manager;
	LIST_ITEM_DATA::stats = &GetDocument()->stat_manager;

	// the tree to be built and displayed, initialized to be empty
	CTreeCtrl& ctrl = GetTreeCtrl(); 
	ctrl.DeleteAllItems();
	static_cast<CMainFrame*>(AfxGetMainWnd())->GetRightPane()->GetListCtrl().DeleteAllItems();
	if (GetDocument()->call_info.empty()) return;

	HTREEITEM root = ctrl.InsertItem("root", TVI_ROOT), current = root;
	vector<INVOC_INFO_EX> stack;
	INVOC_INFO* invoc_info;

	// build the infrastructure
	
	for (list<CALL_INFO>::const_iterator iter = GetDocument()->call_info.begin(); iter != GetDocument()->call_info.end(); ++iter)
	{
		const CALL_INFO& call_info = *iter;

		// begin parsing of the read data
		if (call_info.type == CALL_INFO_START) 
		{
			// insert an entry to the stack
			stack.push_back(INVOC_INFO_EX(call_info.address, call_info.time));

			invoc_info = new INVOC_INFO(call_info.address);
			// update the stat manager and the invoc_info
			// stat man must be updated on info_start to preserve the calls order
			int invocation = GetDocument()->stat_manager.UpdateStatsWith(*invoc_info);
			invoc_info->invocation = invocation;

			// insert into the tree a new node
			CString name = GetDocument()->symbol_manager.GetSymName(call_info.address);
			if (name.IsEmpty()) name.Format("0x%x", call_info.address);
			name.AppendFormat(" (%d)", invocation);
			current = ctrl.InsertItem(name, current);

			// attach the data to the tree node
			ctrl.SetItemData(current, (DWORD_PTR)invoc_info);
		} else {
			INVOC_INFO_EX& invoc_info_ex = stack.back();
			ASSERT(invoc_info_ex.address == call_info.address);

			// back references
			DWORD64 runtime = call_info.time - invoc_info_ex.time;
			invoc_info = reinterpret_cast<INVOC_INFO*>(ctrl.GetItemData(current));
			GetDocument()->stat_manager.UpdateRunTime(*invoc_info, current, runtime);

			stack.pop_back();
			current = ctrl.GetParentItem(current);
		}
	}
	
	ctrl.SelectItem(root);
	ctrl.Expand(root, TVE_EXPAND);
	OnTvnSelchanged(NULL, NULL);
}

// CFunctionTreeView message handlers

void CFunctionTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTreeCtrl& ctrl = GetTreeCtrl(); 
	HTREEITEM hItem = ctrl.GetSelectedItem();
	CStatisticListView *RightPane = static_cast<CMainFrame*>(AfxGetMainWnd())->GetRightPane();
	RightPane->GetListCtrl().DeleteAllItems();
	int counter = 0;

	if (hItem != NULL)
	{

		if (hItem != ctrl.GetRootItem())
			RightPane->InsertLine(++counter, *reinterpret_cast<INVOC_INFO*>(ctrl.GetItemData(hItem)));

		if (ctrl.ItemHasChildren(hItem))
		{
			ctrl.EnsureVisible(hItem);

			HTREEITEM hChildItem = ctrl.GetChildItem(hItem);
			while (hChildItem != NULL)
			{
				RightPane->InsertLine(++counter, *reinterpret_cast<INVOC_INFO*>(ctrl.GetItemData(hChildItem)));
			    hChildItem = ctrl.GetNextItem(hChildItem, TVGN_NEXT);
			}
		}
	}
	if (pResult) *pResult = 0;
}

void CFunctionTreeView::OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTreeCtrl& ctrl = GetTreeCtrl();
	HTREEITEM hItem = pNMTreeView->itemOld.hItem;
	if (hItem != ctrl.GetRootItem())
		delete reinterpret_cast<INVOC_INFO*>(pNMTreeView->itemOld.lParam);
	*pResult = 0;
}
