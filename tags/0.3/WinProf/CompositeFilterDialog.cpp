// CompositeFilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinProf.h"
#include "CompositeFilter.h"
#include "CompositeFilterDialog.h"
#include ".\compositefilterdialog.h"


// CCompositeFilterDialog dialog

IMPLEMENT_DYNAMIC(CCompositeFilterDialog, CDialog)
CCompositeFilterDialog::CCompositeFilterDialog(CWinProfDoc* doc, CString filt, CWnd* pParent /*=NULL*/)
	: CDialog(CCompositeFilterDialog::IDD, pParent)
	, document(doc)
	, filter(filt)
	, m_Name(_T(""))
	, m_Expr(_T(""))
{
}

CCompositeFilterDialog::~CCompositeFilterDialog()
{
}

void CCompositeFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME_EDIT, m_Name);
	DDX_Text(pDX, IDC_EXPR_EDIT, m_Expr);
}


BEGIN_MESSAGE_MAP(CCompositeFilterDialog, CDialog)
END_MESSAGE_MAP()


// CCompositeFilterDialog message handlers

void CCompositeFilterDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if ((filter.IsEmpty() && !document->filter_manager.AddFilter(m_Name, m_Expr)) ||
		(!filter.IsEmpty() && !document->filter_manager.EditFilter(filter, m_Name, m_Expr)))
	{
		MessageBox("Couldn't add/edit composite filter.\nCheck that expression is entered correctly "
			"and that there are no other filters with the same name.", NULL, MB_ICONSTOP | MB_OK);
		return;
	}

	CDialog::OnOK();
}

BOOL CCompositeFilterDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString text;
	GetWindowText(text);
	if (filter.IsEmpty())
		text = "Add" + text;
	else
		text = "Edit" + text;
	SetWindowText(text);

	if (!filter.IsEmpty())
	{
		m_Name = filter;
		m_Expr = static_cast<CCompositeFilter*>(document->filter_manager.GetFilter(filter))->GetExpr();
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}
