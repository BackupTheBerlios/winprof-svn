// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "stdafx.h"
#include "resource.h"       // main symbols

class CStatisticListView;
class CFunctionTreeView;
class CWinProfDoc;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
protected:
	CSplitterWnd m_wndSplitter, m_wndSplitter2;

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
	CStatisticListView* GetFilterPane();
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnProjectOpenExe();
private:
	void ReadSymbols(void);
public:
	CWinProfDoc* GetDocument(void);
	afx_msg void OnProjectFilter();
};


