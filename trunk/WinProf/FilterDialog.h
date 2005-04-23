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
	CListCtrl m_AtomFiltersList;
	CListCtrl m_CompositeFiltersList;
	CComboBox m_FilterBy;
private:
	void FillControls();
public:
	CString m_ActiveFilter;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAtomAddButton();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedAtomEditButton();
	afx_msg void OnNMDblclkAtomFiltersList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAtomRemoveButton();
	afx_msg void OnBnClickedCompositeAddButton();
	afx_msg void OnBnClickedCompositeEditButton();
	afx_msg void OnBnClickedCompositeRemoveButton();
	afx_msg void OnNMDblclkCompositeFiltersList(NMHDR *pNMHDR, LRESULT *pResult);
};
