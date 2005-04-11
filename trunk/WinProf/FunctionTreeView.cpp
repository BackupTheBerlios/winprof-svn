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
#include "StatManager.h"

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

CString CFunctionTreeView::dword64tostr(DWORD64 x) 
{
	if (x == 0) return "0";

	CString s;
	while (x)
	{
		s += (char)(x%10 + '0');
		x /= 10;
	}
	s.MakeReverse();
	return s;
}

void CFunctionTreeView::FillTheTree()
{
	// the tree to be built and displayed, initialized to be empty
	CTreeCtrl& ctrl = GetTreeCtrl(); 
	ctrl.DeleteAllItems();
	HTREEITEM root = ctrl.InsertItem("root", TVI_ROOT), current = root;
	vector<INVOC_INFO*> stack;
	INVOC_INFO* invoc_info;

	// build the infrastructure
	for (list<CALL_INFO>::const_iterator iter = GetDocument()->call_info.begin(); iter != GetDocument()->call_info.end(); ++iter)
	{
		const CALL_INFO& call_info = *iter;

		// begin parsing of the read data
		if (call_info.type == CALL_INFO_START) 
		{
			// insert into the tree a new node
			CString name = GetDocument()->symbol_manager.GetSymName(call_info.address);
			if (name.IsEmpty()) name.Format("%x", call_info.address);
			current = ctrl.InsertItem(name, current);

			// attach the data to the tree node
			invoc_info = new INVOC_INFO(call_info.address, 0/*invoc_number*/, 0/*run_time*/);
			ctrl.SetItemData(current, (DWORD_PTR)invoc_info);

			// update the stat manager and the invoc_info
			// the stat man must be updated on info_start to ensure the right order of calls
			GetDocument()->stat_manager.UpdateStatsWith(*invoc_info); // invoc_number is updated here
			invoc_info->runtime = call_info.time; // will be updated
			
			// the runtime will be updated, when the end of call is detected
			stack.push_back(invoc_info);
		} else {
			// if the call log file is built properly this if is never entered
			invoc_info = stack.back();
			ASSERT(invoc_info->address == call_info.address);

			// back references
			invoc_info->runtime = call_info.time - invoc_info->runtime;
			GetDocument()->stat_manager.UpdateRunTime(*invoc_info);

			// update controls
			stack.pop_back();
			current = ctrl.GetParentItem(current);
		}
	}
	
	ctrl.SelectItem(root);
	ctrl.Expand(root, TVE_EXPAND);
	OnTvnSelchanged(NULL, NULL);
}

// CFunctionTreeView message handlers

void CFunctionTreeView::OnCommandsStart()
{
	// TODO: Add your command handler code here

	PROCESS_INFORMATION info;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	if (!CreateProcess(GetDocument()->m_ExeFileName, NULL, NULL, NULL, FALSE, DEBUG_PROCESS|DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &si, &info))
	{
		MessageBox("can't create process");
		return;
	}
	BOOL stop = FALSE;
	HANDLE hProcess = info.hProcess;

	while (!stop)
	{
		DEBUG_EVENT event;
		WaitForDebugEvent(&event, INFINITE);
		switch (event.dwDebugEventCode)
		{
			case EXCEPTION_DEBUG_EVENT:
				switch (event.u.Exception.ExceptionRecord.ExceptionCode)
				{
					case EXCEPTION_BREAKPOINT:
						stop=TRUE;
						break;
				}
		}
		ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE); 
	}

	GetDocument()->symbol_manager.SetProcess(hProcess, GetDocument()->call_info);

	FillTheTree();
}

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
		{
			STATISTIC_LIST_INFO *recieve = (STATISTIC_LIST_INFO *)ctrl.GetItemData(hItem);
			OutputDebugString(recieve->name);
			CString str[]  = {recieve->name, recieve->time};
			RightPane->InsertLine(++counter, str, recieve->address);
		}

		if (ctrl.ItemHasChildren(hItem))
		{
			ctrl.EnsureVisible(hItem);

			HTREEITEM hChildItem = ctrl.GetChildItem(hItem);
			while (hChildItem != NULL)
			{
				STATISTIC_LIST_INFO *recieve = (STATISTIC_LIST_INFO *)ctrl.GetItemData(hChildItem);
				OutputDebugString(recieve->name);
				CString str[]  = {recieve->name,recieve->time};
				RightPane->InsertLine(++counter, str, recieve->address);
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
		delete (STATISTIC_LIST_INFO*)pNMTreeView->itemOld.lParam;
	*pResult = 0;
}
