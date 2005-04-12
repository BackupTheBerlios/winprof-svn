// StatisticsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinProf.h"
#include "StatisticsDialog.h"
#include "FunctionTreeView.h"
#include ".\statisticsdialog.h"
#include "WinProfStatistics.h"
#include "WinProfDoc.h"

// CStatisticsDialog dialog

IMPLEMENT_DYNAMIC(CStatisticsDialog, CDialog)

CStatisticsDialog::CStatisticsDialog(CWinProfDoc* pDoc, DWORD address, CWnd* pParent)
	: CDialog(CStatisticsDialog::IDD, pParent)
	, m_Address(CFunctionTreeView::dword64tostr(address))
	, m_Name(pDoc->symbol_manager.GetSymName(address))
	, m_CallCount(pDoc->stat_manager.GetStats()[COUNT_CALLS]->GetString(INVOC_INFO(address, 0, 0)))
	, m_AvgRunTime(pDoc->stat_manager.GetStats()[AVG_TIME]->GetString(INVOC_INFO(address, 0, 0)))
{
}

CStatisticsDialog::~CStatisticsDialog()
{
}

void CStatisticsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDRESS_EDIT, m_Address);
	DDX_Text(pDX, IDC_NAME_EDIT, m_Name);
	DDX_Text(pDX, IDC_CALL_COUNT_EDIT, m_CallCount);
	DDX_Text(pDX, IDC_AVG_RUNTIME_EDIT4, m_AvgRunTime);
}


BEGIN_MESSAGE_MAP(CStatisticsDialog, CDialog)
END_MESSAGE_MAP()


// CStatisticsDialog message handlers

