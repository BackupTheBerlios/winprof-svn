// FunctionTreeView.h : interface of the CFunctionTreeView class
//

#pragma once

class CWinProfDoc;

typedef struct {
	DWORD address;
	DWORD64 start, finish, diff;
} RUN_INFO;

/*******************************
* global statistics information
* hash : function_id(32 bits = unsigned long) -> (number of calls, average run time)
*******************************/
/*using namespace std;
typedef pair<unsigned long, DWORD64> info_type;
typedef hash_map<unsigned long, info_type> map_type; // alexeyd */

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
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

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

