// WinProf++Doc.cpp : implementation of the CWinProfDoc class
//

#include "stdafx.h"
#include "WinProf.h"

#include "WinProfDoc.h"
#include ".\winprofdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinProfDoc

IMPLEMENT_DYNCREATE(CWinProfDoc, CDocument)

BEGIN_MESSAGE_MAP(CWinProfDoc, CDocument)
END_MESSAGE_MAP()


// CWinProfDoc construction/destruction

CWinProfDoc::CWinProfDoc()
: m_ExeFileName(_T(""))
{
	// TODO: add one-time construction code here

}

CWinProfDoc::~CWinProfDoc()
{
}

BOOL CWinProfDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CWinProfDoc serialization

void CWinProfDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CWinProfDoc diagnostics

#ifdef _DEBUG
void CWinProfDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinProfDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWinProfDoc commands
