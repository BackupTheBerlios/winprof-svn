#pragma once

#include "WinProfDoc.h"
#include "afxcmn.h"
#include "afxwin.h"

// CFilterDialog dialog

class CFilterDialog : public CDialog
{
	DECLARE_DYNAMIC(CFilterDialog)

public:
	CFilterDialog(CWinProfDoc* doc, CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterDialog();

// Dialog Data
	enum { IDD = IDD_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CWinProfDoc *document;
public:
	CListCtrl m_AtomFiltersList;
	CListCtrl m_CompositeFiltersList;
	virtual BOOL OnInitDialog();
	CComboBox m_FilterBy;
};
