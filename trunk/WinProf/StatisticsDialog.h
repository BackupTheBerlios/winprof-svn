#pragma once


// CStatisticsDialog dialog

class CStatisticsDialog : public CDialog
{
	DECLARE_DYNAMIC(CStatisticsDialog)

public:
	CStatisticsDialog(DWORD address, CString name, DWORD call_count, double avg_runtime,
		CWnd* pParent = NULL);   // standard constructor
	virtual ~CStatisticsDialog();

// Dialog Data
	enum { IDD = IDD_STATISTICS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_Address;
	CString m_Name;
	DWORD m_CallCount;
	CString m_AvgRunTime;
};
