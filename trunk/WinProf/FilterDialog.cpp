// FilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinProf.h"
#include "FilterDialog.h"
#include ".\filterdialog.h"
#include "Filter.h"
#include "AtomFilter.h"
#include "CompositeFilter.h"
#include "AtomFilterDialog.h"
#include "CompositeFilterDialog.h"
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
	ON_BN_CLICKED(IDC_ATOM_ADD_BUTTON, OnBnClickedAtomAddButton)
	ON_BN_CLICKED(IDC_ATOM_EDIT_BUTTON, OnBnClickedAtomEditButton)
	ON_NOTIFY(NM_DBLCLK, IDC_ATOM_FILTERS_LIST, OnNMDblclkAtomFiltersList)
	ON_BN_CLICKED(IDC_ATOM_REMOVE_BUTTON, OnBnClickedAtomRemoveButton)
	ON_BN_CLICKED(IDC_COMPOSITE_ADD_BUTTON, OnBnClickedCompositeAddButton)
	ON_BN_CLICKED(IDC_COMPOSITE_EDIT_BUTTON, OnBnClickedCompositeEditButton)
	ON_BN_CLICKED(IDC_COMPOSITE_REMOVE_BUTTON, OnBnClickedCompositeRemoveButton)
	ON_NOTIFY(NM_DBLCLK, IDC_COMPOSITE_FILTERS_LIST, OnNMDblclkCompositeFiltersList)
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

	FillControls();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CFilterDialog::FillControls()
{
	m_FilterBy.ResetContent();
	m_AtomFiltersList.DeleteAllItems();
	m_CompositeFiltersList.DeleteAllItems();

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
				m_AtomFiltersList.SetItemText(atom_counter, 2, document->stat_manager.GetStats()[st]->GetStatCaption());
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
			m_CompositeFiltersList.SetItemText(composite_counter, 1, f->GetExpr());
			composite_counter++;
		}
		m_FilterBy.AddString(*iter);
	}
	int x = m_FilterBy.FindStringExact(-1, document->m_ActiveFilter);
	m_FilterBy.SetCurSel(x==CB_ERR ? 0 : x);
}

void CFilterDialog::OnBnClickedAtomAddButton()
{
	// TODO: Add your control notification handler code here
	CAtomFilterDialog dlg(document);
	if (dlg.DoModal() == IDCANCEL) return;
	FillControls();
}

void CFilterDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_FilterBy.GetLBText(m_FilterBy.GetCurSel(), m_ActiveFilter);
	CDialog::OnOK();
}

void CFilterDialog::OnBnClickedAtomEditButton()
{
	// TODO: Add your control notification handler code here
	int sel = m_AtomFiltersList.GetSelectionMark();
	if (sel == -1) return;
	CAtomFilterDialog dlg(document, m_AtomFiltersList.GetItemText(sel, 0));
	if (dlg.DoModal() == IDCANCEL) return;
	FillControls();
}

void CFilterDialog::OnNMDblclkAtomFiltersList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	OnBnClickedAtomEditButton();
	*pResult = 0;
}

void CFilterDialog::OnBnClickedAtomRemoveButton()
{
	// TODO: Add your control notification handler code here
	int sel = m_AtomFiltersList.GetSelectionMark();
	if (sel == -1) return;
	if (!document->filter_manager.Destroy(m_AtomFiltersList.GetItemText(sel, 0)))
	{
		MessageBox("Couldn't delete atom filter because other filters depend on it.", NULL, MB_ICONERROR | MB_OK);
		return;
	}
	FillControls();
}

void CFilterDialog::OnBnClickedCompositeAddButton()
{
	// TODO: Add your control notification handler code here
	CCompositeFilterDialog dlg(document);
	if (dlg.DoModal() == IDCANCEL) return;
	FillControls();
}

void CFilterDialog::OnBnClickedCompositeEditButton()
{
	// TODO: Add your control notification handler code here
	int sel = m_CompositeFiltersList.GetSelectionMark();
	if (sel == -1) return;
	CCompositeFilterDialog dlg(document, m_CompositeFiltersList.GetItemText(sel, 0));
	if (dlg.DoModal() == IDCANCEL) return;
	FillControls();
}

void CFilterDialog::OnBnClickedCompositeRemoveButton()
{
	// TODO: Add your control notification handler code here
	int sel = m_CompositeFiltersList.GetSelectionMark();
	if (sel == -1) return;
	if (!document->filter_manager.Destroy(m_CompositeFiltersList.GetItemText(sel, 0)))
	{
		MessageBox("Couldn't delete composite filter because other filters depend on it.", NULL, MB_ICONERROR | MB_OK);
		return;
	}
	FillControls();
}

void CFilterDialog::OnNMDblclkCompositeFiltersList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	OnBnClickedCompositeEditButton();
	*pResult = 0;
}
