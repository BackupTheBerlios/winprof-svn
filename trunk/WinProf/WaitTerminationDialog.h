#pragma once


// CWaitTerminationDialog dialog

class CWaitTerminationDialog : public CDialog
{
	DECLARE_DYNAMIC(CWaitTerminationDialog)

public:
	using CDialog::OnCancel;
	CWaitTerminationDialog(HANDLE hProcess, CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaitTerminationDialog();

// Dialog Data
	enum { IDD = IDD_WAIT_TERMINATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	HANDLE hProcess;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	HANDLE GetProcess(void);
};
