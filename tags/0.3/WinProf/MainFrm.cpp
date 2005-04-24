// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WinProf.h"

#include "MainFrm.h"
#include "FunctionTreeView.h"
#include "StatisticListView.h"
#include "WinProfDoc.h"
#include "WaitTerminationDialog.h"
#include ".\mainfrm.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "FilterManager.h"

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_PROJECT_OPENEXE, OnProjectOpenExe)
	ON_COMMAND(ID_PROJECT_FILTER, OnProjectFilter)
	ON_COMMAND(ID_PROJECT_STATISTICS, OnProjectStatistics)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//sdoc******************************************************
	
	//CString expr("x & y");
	//CString expr("x & y | z");
/*	CString expr("(x | y) & ~ z | t");

	CFilterManager man;
	vector<CString> infix, postfix;
	man.BuildInfixFormStack(expr, infix);
	man.BuildPostfix(infix, postfix);
	CString out;
	for(int i = 0; i < (int)postfix.size(); i++) {
		CString tmp = out;
		out.Format("%s%s%s", (const char*)tmp, " ", (const char*)postfix[i]);
	}
	MessageBox(out); */
//**********************************************************
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CFunctionTreeView), CSize(250, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE | WS_BORDER, m_wndSplitter.IdFromRowCol(0, 1)))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	if (!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CStatisticListView), CSize(100, 300), pContext) ||
		!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CStatisticListView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		m_wndSplitter2.DestroyWindow();
		return FALSE;
	}
	
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

CStatisticListView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter2.GetPane(0, 0);
	CStatisticListView* pView = DYNAMIC_DOWNCAST(CStatisticListView, pWnd);
	return pView;
}

CStatisticListView* CMainFrame::GetFilterPane()
{
	CWnd* pWnd = m_wndSplitter2.GetPane(1, 0);
	CStatisticListView* pView = DYNAMIC_DOWNCAST(CStatisticListView, pWnd);
	return pView;
}

CFunctionTreeView* CMainFrame::GetLeftPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CFunctionTreeView* pView = DYNAMIC_DOWNCAST(CFunctionTreeView, pWnd);
	return pView;
}

void CMainFrame::OnProjectOpenExe()
{
	// TODO: Add your command handler code here
	CFileDialog fd(TRUE, _T("exe"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("Programs (*.exe)|*.exe|All Files (*.*)|*.*||"));
	if (fd.DoModal() == IDCANCEL) return;
	CString filename = fd.GetPathName();
	GetLeftPane()->GetDocument()->m_ExeFileName = filename;

	PROCESS_INFORMATION info;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	if (!CreateProcess(filename, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &info))
	{
		MessageBox("Couldn't create process.", NULL, MB_ICONERROR | MB_OK);
		return;
	}

	CWaitTerminationDialog dlg(info.hProcess);
	dlg.DoModal();
	CloseHandle(info.hThread);
	CloseHandle(info.hProcess);

	TCHAR temp[MAX_PATH];
	GetTempPath(MAX_PATH, temp);
	CString calllog_filename;
	calllog_filename.Format(_T("%scalllog.%d"), temp, info.dwProcessId);
	if (!GetDocument()->ReadCallLog(calllog_filename))
	{
		MessageBox("Couldn't read functions call log file.\n"
			"Make sure the application is linked against CallMon.lib", NULL, MB_ICONERROR | MB_OK);
		return;
	}
	CFile::Remove(calllog_filename);
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	CWinProfDoc::m_Frequency = (DWORD64)freq.QuadPart;

	ReadSymbols();
	GetDocument()->stat_manager.Clear();
	GetDocument()->SetModifiedFlag();
	GetLeftPane()->FillTheTree();
}

void CMainFrame::ReadSymbols(void)
{
	PROCESS_INFORMATION info;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	if (!CreateProcess(GetDocument()->m_ExeFileName, NULL, NULL, NULL, FALSE, DEBUG_PROCESS|DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &si, &info))
	{
		MessageBox("Couldn't create process.", NULL, MB_ICONERROR | MB_OK);
		return;
	}
	BOOL stop = FALSE;
	HANDLE hProcess = info.hProcess;
	DEBUG_EVENT event;

	while (!stop)
	{
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

	TerminateProcess(hProcess, 0);
	CloseHandle(info.hThread);
	CloseHandle(info.hProcess);
	while (true)
	{
		WaitForDebugEvent(&event, INFINITE);
		ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE); 
		if (event.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) break;
	}
}

CWinProfDoc* CMainFrame::GetDocument(void)
{
	return GetLeftPane()->GetDocument();
}

void CMainFrame::OnProjectFilter()
{
	GetFilterPane()->OnProjectFilter();
}

void CMainFrame::OnProjectStatistics()
{
	GetFilterPane()->OnProjectStatistics();
}
