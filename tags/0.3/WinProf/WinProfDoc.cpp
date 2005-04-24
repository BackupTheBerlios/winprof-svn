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
DWORD64 CWinProfDoc::m_Frequency = 0;

CWinProfDoc::CWinProfDoc()
: m_ExeFileName(_T(""))
, m_ActiveFilter(_T(""))
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
	m_ActiveFilter.Empty();
	symbol_manager.Flush();
	call_info.clear();
	stat_manager.Clear();
	filter_manager.Destroy();

	return TRUE;
}

BOOL CWinProfDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	stat_manager.Clear();

	return TRUE;
}



// CWinProfDoc serialization

void CWinProfDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_ExeFileName << m_Frequency;
		symbol_manager.Serialize(ar);
		filter_manager.Serialize(ar);
		ar << m_ActiveFilter;
		ar << (unsigned int)call_info.size();
		for (list<CALL_INFO>::const_iterator iter = call_info.begin(); iter != call_info.end(); ++iter)
			ar.Write(&*iter, sizeof(CALL_INFO));
	}
	else
	{
		ar >> m_ExeFileName >> m_Frequency;
		symbol_manager.Serialize(ar);
		filter_manager.Serialize(ar);
		ar >> m_ActiveFilter;
		unsigned int size;
		CALL_INFO ci;
		ar >> size;
		call_info.clear();
		while (size--)
		{
			ar.Read(&ci, sizeof(CALL_INFO));
			call_info.push_back(ci);
		}
	}
}

BOOL CWinProfDoc::ReadCallLog(CString filename)
{
	call_info.clear();
	CFile f;
	if (!f.Open(filename, CFile::modeRead))
		return FALSE;
	CALL_INFO ci;
	while (f.Read(&ci, sizeof(CALL_INFO)))
		call_info.push_back(ci);
	return TRUE;
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
