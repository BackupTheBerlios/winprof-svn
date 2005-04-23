// AtomFilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinProf.h"
#include "AtomFilter.h"
#include "AtomFilterDialog.h"
#include "SymbolManager.h"
#include "StatManager.h"
#include "WinProfStatistics.h"
#include ".\atomfilterdialog.h"


// CAtomFilterDialog dialog

IMPLEMENT_DYNAMIC(CAtomFilterDialog, CDialog)
CAtomFilterDialog::CAtomFilterDialog(CWinProfDoc* doc, CString filt, CWnd* pParent /*=NULL*/)
	: CDialog(CAtomFilterDialog::IDD, pParent)
	, document(doc)
	, filter(filt)
	, m_Name(_T(""))
	, m_Value(_T(""))
{
}

CAtomFilterDialog::~CAtomFilterDialog()
{
}

void CAtomFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FUNCTION_COMBO, m_Functions);
	DDX_Control(pDX, IDC_STAT_COMBO, m_Stats);
	DDX_Control(pDX, IDC_FUNCTION_OP_COMBO, m_FunctionOp);
	DDX_Control(pDX, IDC_STAT_OP_COMBO, m_StatOp);
	DDX_Text(pDX, IDC_NAME_EDIT, m_Name);
	DDX_Text(pDX, IDC_STAT_BOUND_EDIT, m_Value);
}


BEGIN_MESSAGE_MAP(CAtomFilterDialog, CDialog)
END_MESSAGE_MAP()


// CAtomFilterDialog message handlers


BOOL CAtomFilterDialog::OnInitDialog()
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

	DWORD fn = 0;
	bool this_func = true;
	int stat = -1;
	stat_val_t bound;
	cmp_oper op = (cmp_oper)0;
	if (!filter.IsEmpty())
	{
		m_Name = filter;
		static_cast<CAtomFilter*>(document->filter_manager.GetFilter(filter))->GetAttrs(fn, this_func, stat, bound, op);
		if (stat != -1) m_Value = document->stat_manager.GetStats()[stat]->ToString(bound);
		UpdateData(FALSE);
	}
	m_FunctionOp.SetCurSel(1-(int)this_func);
	FillFunctions(fn);
	FillStats(stat);
	FillStatOp((int)op);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAtomFilterDialog::FillFunctions(DWORD fn)
{
	const CSymbolManager::symbol_map_t& symbols = document->symbol_manager.GetSymbols();
	m_Functions.SetItemData(m_Functions.AddString(""), 0);
	for (CSymbolManager::symbol_map_t::const_iterator iter = symbols.begin(); iter != symbols.end(); ++iter)
		m_Functions.SetItemData(m_Functions.AddString(iter->second), iter->first);
	if (fn)
	{
		CSymbolManager::symbol_map_t::const_iterator iter = symbols.find(fn);
		int pos = m_Functions.FindStringExact(-1, iter->second);
		if (pos != -1) m_Functions.SetCurSel(pos);
		else m_Functions.SetCurSel(0);
	}
	else
		m_Functions.SetCurSel(0);
}

void CAtomFilterDialog::FillStats(int stat)
{
	statistics_t& stats = document->stat_manager.GetStats();
	m_Stats.SetItemData(m_Stats.AddString(""), -1);
	int item = 0;
	for (int i = 0; i < (int)stats.size(); i++)
		if (stats[i]->IsPerFunction() || stats[i]->IsPerInvocation())
		{
			int pos = m_Stats.AddString(stats[i]->GetStatCaption());
			if (i == stat) item = pos;
			m_Stats.SetItemData(pos, i);
		}
	m_Stats.SetCurSel(item);
}

void CAtomFilterDialog::FillStatOp(int op)
{
	for (int i=0; i<CmpOper::CMP_OPER_COUNT; i++)
		m_StatOp.SetItemData(m_StatOp.AddString(CmpOper::caption[i]), i);
	m_StatOp.SetCurSel(op);
}

void CAtomFilterDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	int stat = (int)m_Stats.GetItemData(m_Stats.GetCurSel());
	stat_val_t bound = stat_val_t();
	if (stat != -1)
		if (!document->stat_manager.GetStats()[stat]->GetNumerical(m_Value, bound))
		{
			MessageBox("Improper value entered in statistic value field.", NULL, MB_ICONERROR | MB_OK);
			return;
		}
	bool this_f = (m_FunctionOp.GetCurSel()==0);
	DWORD fn = (DWORD)m_Functions.GetItemData(m_Functions.GetCurSel());
	cmp_oper op = (cmp_oper)m_StatOp.GetItemData(m_StatOp.GetCurSel());
	if ((filter.IsEmpty() && !document->filter_manager.AddFilter(m_Name, this_f, fn, stat, bound, op)) ||
		(!filter.IsEmpty() && !document->filter_manager.EditFilter(filter, m_Name, this_f, fn, stat, bound, op)))
	{
		MessageBox("Couldn't add/edit atom filter.\nCheck that all fields are entered correctly "
			"and that there are no other filters with the same name.", NULL, MB_ICONSTOP | MB_OK);
		return;
	}
	CDialog::OnOK();
}
