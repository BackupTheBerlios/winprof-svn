#pragma once

#include "StdAfx.h"
#include "MainFrm.h"
#include "FunctionTreeView.h"
#include "CommonDefs.h"

using namespace std;

class CStatManager
{
public:
	CStatManager(void);
	~CStatManager(void);

	// make these functions non static to ensure that the object is created
	// if so, then all the allocated data is deleted "automatically" in destructor
	INT UpdateStatsWith(const INVOC_INFO& call);
	void UpdateRunTime(const INVOC_INFO& call, HTREEITEM item, DWORD64 time);
	static void Clear(void);

private: // auxiliary functions
	void AddStatClass(const CWinProfStatistics* stat);

public:
	static statistics_t& GetStats(void) {return stats;}
	static func2vect_t& GetDataBaseRef(void) {return func2vect;}
	static stname2index_t& GetStName2Index(void) {return stname2index;}
	static CTreeCtrl& GetTreeCtrl(void) {return static_cast<CMainFrame*>(AfxGetMainWnd())->GetLeftPane()->GetTreeCtrl();}

private:
	static func2vect_t func2vect;
	static statistics_t stats;
	static stname2index_t stname2index;
}; // class CStatManager