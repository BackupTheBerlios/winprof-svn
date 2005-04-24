#pragma once
#include "afxcmn.h"

#include "CommonDefs.h"

// CStatisticsDialog dialog
class CWinProfDoc;
class CStatManager;
class CSymbolManager;

class CStatisticsDialog : public CDialog
{
	DECLARE_DYNAMIC(CStatisticsDialog)

public:
	CStatisticsDialog(CWinProfDoc* doc, DWORD address, CWnd* pParent = NULL); // standard constructor
	CStatisticsDialog(CWinProfDoc* doc, const filtered_list_t& filtered_list, CWnd* pParent = NULL); // standard constructor
	virtual ~CStatisticsDialog();

// Dialog Data
	enum { IDD = IDD_STATISTICS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	DWORD address;
	CWinProfDoc *document;
	const filtered_list_t* list;
	CListCtrl m_StatList;

public:
	virtual BOOL OnInitDialog();
};
