// WinProf++Doc.cpp : implementation of the CWinProfDoc class
//

#include "stdafx.h"
#include "WinProf.h"

#include "WinProfDoc.h"
#include ".\winprofdoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

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
	m_ExeFileName.Empty();
	symbol_manager.Flush();
	call_info.clear();

	return TRUE;
}




// CWinProfDoc serialization

void CWinProfDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_ExeFileName;
		symbol_manager.Serialize(ar);
		ar << (unsigned int)call_info.size();
		for (list<CALL_INFO>::const_iterator iter = call_info.begin(); iter != call_info.end(); ++iter)
			ar.Write(&*iter, sizeof(CALL_INFO));
	}
	else
	{
		ar >> m_ExeFileName;
		symbol_manager.Serialize(ar);
		unsigned int size;
		CALL_INFO ci;
		ar >> size;
		call_info.clear();
		while (size--)
		{
			ar.Read(&ci, sizeof(CALL_INFO));
			call_info.push_back(ci);
		}
//		UpdateAllViews(NULL, 1);
	}
}

void CWinProfDoc::ReadCallLog(CString filename)
{
	call_info.clear();
	CFile f(filename, CFile::modeRead);
	CALL_INFO ci;
	while (f.Read(&ci, sizeof(CALL_INFO)))
		call_info.push_back(ci);
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
