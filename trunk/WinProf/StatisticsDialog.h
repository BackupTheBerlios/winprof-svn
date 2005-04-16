#pragma once
#include "afxcmn.h"

// CStatisticsDialog dialog
class CWinProfDoc;
class CStatManager;
class CSymbolManager;

class CStatisticsDialog : public CDialog
{
	DECLARE_DYNAMIC(CStatisticsDialog)

public:
	CStatisticsDialog(CStatManager& stat_mgr, CSymbolManager& symbol_mgr, DWORD address, CWnd* pParent = NULL); // standard constructor
	virtual ~CStatisticsDialog();

// Dialog Data
	enum { IDD = IDD_STATISTICS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	DWORD address;
	CStatManager& stat_manager;
	CSymbolManager& symbol_manager;
	CListCtrl m_StatList;

public:
	virtual BOOL OnInitDialog();
};
