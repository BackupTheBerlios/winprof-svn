// FunctionTreeView.cpp : implementation of the CFunctionTreeView class
//

#include "stdafx.h"
#include "WinProf.h"

#include "WinProfDoc.h"
#include "FunctionTreeView.h"
#include "../CallMon/CallMon.h"
#include "MainFrm.h"
#include "StatisticListView.h"
#include ".\functiontreeview.h"
#include <vector> // alexeyd

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFunctionTreeView

IMPLEMENT_DYNCREATE(CFunctionTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CFunctionTreeView, CTreeView)
	ON_COMMAND(ID_COMMANDS_START, OnCommandsStart)
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
	
	GetTreeCtrl().ModifyStyle(NULL,	TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);

	// TODO: You may populate your TreeView with items by directly accessing
	// its tree control through a call to GetTreeCtrl().
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

// this function is already defined in CallMon, didn't succeed to use the version
// alexeyd
CString CFunctionTreeView::dword64tostr(DWORD64 x) 
{
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
	HTREEITEM root = ctrl.InsertItem("root",TVI_ROOT), current = root;
	
	// place holders, line counter
	CALL_INFO call_info;
	RUN_INFO run_info;
	INT counter = 0;

	// open the intermediate log file
	CFile f;
	if(!f.Open("calllog2.prof", CFile::modeRead | CFile::shareDenyWrite)) 
		return;

	// function_id -> (number of calls, average run time)
	//map_type map();
	std::vector<RUN_INFO> stack;

	// build the infrastructure
	while(true) 
	{
		// read line from the intermediate call log file
		int bytes_read = f.Read(&call_info, sizeof(CALL_INFO));
		if (bytes_read == 0) return;
		if (bytes_read < sizeof(CALL_INFO) && bytes_read > 0) {
			OutputDebugString("unexpected end of file\n");
			return;
		}

		// begin parsing of the read data
		if (call_info.type == CALL_INFO_START) 
		{
			// update run_info
			run_info.address = call_info.address;
			run_info.start = call_info.time;
			run_info.finish = 0; // stub values, to be updated on the 
			run_info.diff = 0;
			stack.push_back(run_info);

			// insert into the tree a new node
			CString s;
			s.Format("%ld", call_info.address);
			current = ctrl.InsertItem(s, current);
		} else {
			#ifdef _DEBUG
			// if the call log file is built properly this if is never entered
			if(stack.back().address != call_info.address){
				CString s;
				s.Format("improper input file: %ld %ld\n", stack.back().address, call_info.address);
				OutputDebugString(s);
			}
			#endif

			// update the stack values
			stack.back().finish = call_info.time;
			stack.back().diff = stack.back().finish - stack.back().start;

			// collect statistics information about functions call
			/* unsigned long addr = (unsigned long)call_info.address;
			map_type::const_iterator iter = map.find(addr);
			if (iter.begin() == iter.end()) {
				map[addr] = info_type(0, 0);
			}
			map[addr].first++;
			map[addr].second += stack.back().diff; */

			// prepare information to be transferred to the table
			CString ad; ad.Format("%ld", stack.back().address);
			CString s1(dword64tostr(stack.back().start));
			CString s2(dword64tostr(stack.back().finish));
			CString s3(dword64tostr(stack.back().diff));
			CString* str[4] = {&ad, &s1, &s2, &s3};
			(static_cast<CMainFrame*>(theApp.m_pMainWnd))->GetRightPane()->InsertLine(++counter, str);
			stack.pop_back();

			current = ctrl.GetParentItem(current);
		}
	}
}

// CFunctionTreeView message handlers

void CFunctionTreeView::OnCommandsStart()
{
	// TODO: Add your command handler code here
	FillTheTree();
}
