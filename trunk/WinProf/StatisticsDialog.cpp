// StatisticsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinProf.h"
#include "StatisticsDialog.h"
#include "FunctionTreeView.h"
#include ".\statisticsdialog.h"
#include "WinProfStatistics.h"
#include "WinProfDoc.h"
#include "MainFrm.h"

// CStatisticsDialog dialog

IMPLEMENT_DYNAMIC(CStatisticsDialog, CDialog)

CStatisticsDialog::CStatisticsDialog(CStatManager& stat_mgr, CSymbolManager& symbol_mgr, DWORD address, CWnd* pParent)
	: CDialog(CStatisticsDialog::IDD, pParent)
	, address(address)
	, stat_manager(stat_mgr)
	, symbol_manager(symbol_mgr)
{
}

CStatisticsDialog::~CStatisticsDialog()
{
}

void CStatisticsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATISTICS_LIST, m_StatList);
}


BEGIN_MESSAGE_MAP(CStatisticsDialog, CDialog)
END_MESSAGE_MAP()


// CStatisticsDialog message handlers


BOOL CStatisticsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_StatList.SetExtendedStyle(m_StatList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	// building columns
	m_StatList.InsertColumn(1, "Name", LVCFMT_LEFT, 150, 1);
	m_StatList.InsertColumn(2, "Value", LVCFMT_LEFT, 200, 2);

	// fill statistics
	m_StatList.SetItemText(m_StatList.InsertItem(0, "Address"), 1, Format("0x%x", address));
	m_StatList.SetItemText(m_StatList.InsertItem(1, "Name"), 1, symbol_manager.GetSymName(address));

	int counter = 2;
	statistics_t& stats = stat_manager.GetStats();
	INVOC_INFO invoc_info(address);
	for (statistics_t::const_iterator iter = stats.begin(); iter != stats.end(); ++iter)
	{
		const CWinProfStatistics *p = *iter;
		if (!p->IsPerInvocation())
			m_StatList.SetItemText(m_StatList.InsertItem(counter++, p->GetStatName()), 1, p->GetString(invoc_info));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}
