// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include <hash_map>
//#include "CWinProfStatistics.h"
//#include "StatManager.h"

struct FUNCTION_CALL_INFO {
	unsigned int count;
	DWORD64 time;
};

typedef stdext::hash_map<DWORD, FUNCTION_CALL_INFO> function_call_map_t;

class CStatisticListView;
class CFunctionTreeView;
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:
	function_call_map_t function_call_map;

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
	CStatisticListView* GetRightPane();
	CFunctionTreeView* GetLeftPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnProjectOpenExe();
};


