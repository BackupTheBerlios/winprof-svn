// StatisticListView.h : interface of the CStatisticListView class
//


#pragma once


class CStatisticListView : public CListView
{
protected: // create from serialization only
	CStatisticListView();
	DECLARE_DYNCREATE(CStatisticListView)

// Attributes
public:
	CWinProfDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CStatisticListView();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // debug version in StatisticListView.cpp
inline CWinProfDoc* CStatisticListView::GetDocument() const
   { return reinterpret_cast<CWinProfDoc*>(m_pDocument); }
#endif

