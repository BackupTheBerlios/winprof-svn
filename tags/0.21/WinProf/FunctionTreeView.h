// FunctionTreeView.h : interface of the CFunctionTreeView class
//

#pragma once

#include <hash_map>
#include "SymbolManager.h"

struct FUNCTION_CALL_INFO {
	unsigned int count;
	DWORD64 time;
};

class CWinProfDoc;

typedef struct {
	DWORD address;
	DWORD64 start, finish, diff;
} RUN_INFO;

class CFunctionTreeView : public CTreeView
{
protected: // create from serialization only
	CFunctionTreeView();
	DECLARE_DYNCREATE(CFunctionTreeView)

// Attributes
public:
	CWinProfDoc* GetDocument();

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void FillTheTree();
	static CString dword64tostr(DWORD64 x);
	
// Implementation
public:
	virtual ~CFunctionTreeView();
	void SetCallLogFileName(CString filename);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	typedef stdext::hash_map<DWORD, FUNCTION_CALL_INFO> function_call_map_t;
	function_call_map_t function_call_map;
	CSymbolManager symbol_manager;
	CString calllog_filename;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCommandsStart();
};

#ifndef _DEBUG  // debug version in FunctionTreeView.cpp
inline CWinProfDoc* CFunctionTreeView::GetDocument()
   { return reinterpret_cast<CWinProfDoc*>(m_pDocument); }
#endif

