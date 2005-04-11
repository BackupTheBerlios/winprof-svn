// WinProf++Doc.h : interface of the CWinProfDoc class
//

#pragma once

#include "SymbolManager.h"
#include "StatManager.h"
#include "..\CallMon\CallMon.h"
#include <list>

class CWinProfDoc : public CDocument
{
protected: // create from serialization only
	CWinProfDoc();
	DECLARE_DYNCREATE(CWinProfDoc)

// Attributes
public:
	CSymbolManager symbol_manager;
	CStatManager stat_manager;
	CString m_ExeFileName;
	std::list<CALL_INFO> call_info;
	DWORD64 m_Frequency;

// Operations
public:
	void ReadCallLog(CString filename);

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CWinProfDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
};


