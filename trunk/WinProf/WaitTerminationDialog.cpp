// WaitTerminationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinProf.h"
#include "WaitTerminationDialog.h"
#include ".\waitterminationdialog.h"


// CWaitTerminationDialog dialog

IMPLEMENT_DYNAMIC(CWaitTerminationDialog, CDialog)
CWaitTerminationDialog::CWaitTerminationDialog(HANDLE hProcess_, CWnd* pParent /*=NULL*/)
	: CDialog(CWaitTerminationDialog::IDD, pParent), hProcess(hProcess_)
{
}

CWaitTerminationDialog::~CWaitTerminationDialog()
{
}

void CWaitTerminationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitTerminationDialog, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CWaitTerminationDialog message handlers

UINT ThreadFunc(LPVOID pParam)
{
	CWaitTerminationDialog *dlg = (CWaitTerminationDialog *)pParam;
	WaitForSingleObject(dlg->GetProcess(), INFINITE);
	dlg->OnCancel();
	return 0;
}

int CWaitTerminationDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	AfxBeginThread(ThreadFunc, this);

	return 0;
}

HANDLE CWaitTerminationDialog::GetProcess(void)
{
	return hProcess;
}
