// FilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinProf.h"
#include "FilterDialog.h"
#include ".\filterdialog.h"
#include "Filter.h"
#include "AtomFilter.h"
#include "CompositeFilter.h"
#include "CommonDefs.h"
#include <vector>

using namespace std;


// CFilterDialog dialog

IMPLEMENT_DYNAMIC(CFilterDialog, CDialog)
CFilterDialog::CFilterDialog(CWinProfDoc* doc, CWnd* pParent /*=NULL*/)
	: CDialog(CFilterDialog::IDD, pParent)
	, document(doc)
{
}

CFilterDialog::~CFilterDialog()
{
}

void CFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ATOM_FILTERS_LIST, m_AtomFiltersList);
	DDX_Control(pDX, IDC_COMPOSITE_FILTERS_LIST, m_CompositeFiltersList);
	DDX_Control(pDX, IDC_MAIN_FILTER_COMBO, m_FilterBy);
}


BEGIN_MESSAGE_MAP(CFilterDialog, CDialog)
END_MESSAGE_MAP()


// CFilterDialog message handlers

BOOL CFilterDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_AtomFiltersList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	m_AtomFiltersList.SetExtendedStyle(m_AtomFiltersList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_AtomFiltersList.InsertColumn(0, "Name", LVCFMT_LEFT, 70, 0);
	m_AtomFiltersList.InsertColumn(1, "Function", LVCFMT_LEFT, 150, 1);
	m_AtomFiltersList.InsertColumn(2, "Statistic", LVCFMT_LEFT, 150, 2);
	m_AtomFiltersList.InsertColumn(3, "Op", LVCFMT_LEFT, 30, 3);
	m_AtomFiltersList.InsertColumn(4, "Value", LVCFMT_LEFT, 50, 4);

	m_CompositeFiltersList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	m_CompositeFiltersList.SetExtendedStyle(m_CompositeFiltersList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_CompositeFiltersList.InsertColumn(0, "Name", LVCFMT_LEFT, 70, 0);
	m_CompositeFiltersList.InsertColumn(1, "Expression", LVCFMT_LEFT, 300, 1);


	m_FilterBy.AddString("");
	vector<CString> filters;
	document->filter_manager.GetFilterNames(filters);
	int atom_counter = 0, composite_counter = 0;
	for (vector<CString>::const_iterator iter = filters.begin(); iter != filters.end(); ++iter)
	{
		CFilter* f = document->filter_manager.GetFilter(*iter);
		if (f->IsAtom())
		{
			DWORD fn;
			bool it;
			int st;
			stat_val_t bnd;
			cmp_oper op;
			static_cast<CAtomFilter*>(f)->GetAttrs(fn, it, st, bnd, op);
			m_AtomFiltersList.InsertItem(atom_counter, *iter);
			m_AtomFiltersList.SetItemText(atom_counter, 1, fn ? (it?"":"!=") + document->symbol_manager.GetSymName(fn) : "");
			if (st!=-1)
			{
				m_AtomFiltersList.SetItemText(atom_counter, 2, (it?"":"!=") + document->stat_manager.GetStats()[st]->GetStatCaption());
				m_AtomFiltersList.SetItemText(atom_counter, 3, CmpOper::caption[op]);
				m_AtomFiltersList.SetItemText(atom_counter, 4, document->stat_manager.GetStats()[st]->ToString(bnd));
			}
			else
			{
				m_AtomFiltersList.SetItemText(atom_counter, 2, "");
				m_AtomFiltersList.SetItemText(atom_counter, 3, "");
				m_AtomFiltersList.SetItemText(atom_counter, 4, "");
			}
			atom_counter++;
		}
		else
		{
			m_CompositeFiltersList.InsertItem(composite_counter, *iter);
			m_CompositeFiltersList.SetItemText(composite_counter, 1, static_cast<CCompositeFilter*>(f)->GetExpr());
			composite_counter++;
		}
		m_FilterBy.AddString(*iter);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}
