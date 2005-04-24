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

CStatisticsDialog::CStatisticsDialog(CWinProfDoc* doc, DWORD address, CWnd* pParent)
	: CDialog(CStatisticsDialog::IDD, pParent)
	, address(address)
	, document(doc)
	, list(NULL)
{
}

CStatisticsDialog::CStatisticsDialog(CWinProfDoc* doc, const filtered_list_t& filtered_list, CWnd* pParent)
	: CDialog(CStatisticsDialog::IDD, pParent)
	, document(doc)
	, list(&filtered_list)
	, address(0)
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

	int counter = 0;
	// fill statistics
	if (address != 0)
	{
		m_StatList.SetItemText(m_StatList.InsertItem(0, "Address"), 1, Format("0x%x", address));
		m_StatList.SetItemText(m_StatList.InsertItem(1, "Name"), 1, document->symbol_manager.GetSymName(address));
		counter = 2;
	}

	statistics_t& stats = document->stat_manager.GetStats();
	INVOC_INFO invoc_info(address);
	for (statistics_t::const_iterator iter = stats.begin(); iter != stats.end(); ++iter)
	{
		const CWinProfStatistics *p = *iter;
		if (p->IsPerFunction())
			if (address != 0)
				m_StatList.SetItemText(m_StatList.InsertItem(counter++, p->GetStatCaption()), 1, p->GetString(invoc_info));
			else
				m_StatList.SetItemText(m_StatList.InsertItem(counter++, p->GetStatCaption()), 1, p->GetString(*list));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}
