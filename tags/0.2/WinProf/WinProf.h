// WinProf++.h : main header file for the WinProf++ application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CWinProfApp:
// See WinProf++.cpp for the implementation of this class
//

class CWinProfApp : public CWinApp
{
public:
	CWinProfApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWinProfApp theApp;