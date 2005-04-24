#pragma once

#include "stdafx.h"
#include "WinProfDoc.h"

// CCompositeFilterDialog dialog

class CCompositeFilterDialog : public CDialog
{
	DECLARE_DYNAMIC(CCompositeFilterDialog)

public:
	CCompositeFilterDialog(CWinProfDoc* doc, CString filt = "", CWnd* pParent = NULL);   // standard constructor
	virtual ~CCompositeFilterDialog();

// Dialog Data
	enum { IDD = IDD_COMPOSITE_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
private:
	CWinProfDoc *document;
	CString filter;
private:
	CString m_Name;
	CString m_Expr;
public:
	virtual BOOL OnInitDialog();
};
