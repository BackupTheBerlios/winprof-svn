// WinProf++Doc.h : interface of the CWinProfDoc class
//


#pragma once

class CWinProfDoc : public CDocument
{
protected: // create from serialization only
	CWinProfDoc();
	DECLARE_DYNCREATE(CWinProfDoc)

// Attributes
public:

// Operations
public:

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
	CString m_ExeFileName;
};


