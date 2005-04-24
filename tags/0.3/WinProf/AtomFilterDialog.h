#pragma once

#include "WinProfDoc.h"
#include "afxwin.h"

// CAtomFilterDialog dialog

class CAtomFilterDialog : public CDialog
{
	DECLARE_DYNAMIC(CAtomFilterDialog)

public:
	CAtomFilterDialog(CWinProfDoc* doc, CString filt = "", CWnd* pParent = NULL);   // standard constructor
	virtual ~CAtomFilterDialog();

// Dialog Data
	enum { IDD = IDD_ATOM_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CWinProfDoc *document;
	CString filter;
public:
	virtual BOOL OnInitDialog();
private:
	void FillFunctions(DWORD fn);
	void FillStats(int stat);
	void FillStatOp(int op);
private:
	CComboBox m_Functions;
	CComboBox m_Stats;
	CComboBox m_FunctionOp;
	CComboBox m_StatOp;
protected:
	virtual void OnOK();
private:
	CString m_Name;
	CString m_Value;
};
