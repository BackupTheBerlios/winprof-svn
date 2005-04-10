// FunctionTreeView.h : interface of the CFunctionTreeView class
//

#pragma once

#include <hash_map>
#include "SymbolManager.h"

struct STATISTIC_LIST_INFO
{
	DWORD address;
	CString name;
	CString time;
	STATISTIC_LIST_INFO(DWORD address_, CString name_, CString time_)
		: address(address_), name(name_), time(time_) {}
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
public:
	static CString dword64tostr(DWORD64 x);
	
// Implementation
public:
	virtual ~CFunctionTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCommandsStart();
protected:
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // debug version in FunctionTreeView.cpp
inline CWinProfDoc* CFunctionTreeView::GetDocument()
   { return reinterpret_cast<CWinProfDoc*>(m_pDocument); }
#endif

