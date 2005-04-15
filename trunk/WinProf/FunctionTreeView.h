// FunctionTreeView.h : interface of the CFunctionTreeView class
//

#pragma once

#include <hash_map>
#include "SymbolManager.h"

class CWinProfDoc;

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
public:
	void FillTheTree();
	
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
protected:
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // debug version in FunctionTreeView.cpp
inline CWinProfDoc* CFunctionTreeView::GetDocument()
   { return reinterpret_cast<CWinProfDoc*>(m_pDocument); }
#endif

