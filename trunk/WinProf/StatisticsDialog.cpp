// StatisticsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinProf.h"
#include "StatisticsDialog.h"
#include ".\statisticsdialog.h"


// CStatisticsDialog dialog

IMPLEMENT_DYNAMIC(CStatisticsDialog, CDialog)
CStatisticsDialog::CStatisticsDialog(DWORD address, CString name, DWORD call_count, double avg_runtime,
									 CWnd* pParent /*=NULL*/)
	: CDialog(CStatisticsDialog::IDD, pParent)
	, m_Address(_T(""))
	, m_Name(name)
	, m_CallCount(call_count)
	, m_AvgRunTime(_T(""))
{
	m_Address.Format("0x%08x", address);
	m_AvgRunTime.Format("%1.3f ms", avg_runtime);
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

