#pragma once

#include "StatManager.h"

class CWinProfStatistics 
{
public:
	CWinProfStatistics(void) {}
	virtual ~CWinProfStatistics(void) {}

	virtual CString GetString(const INVOC_INFO& call) const = 0;
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const = 0;
	virtual CString GetStatName(void) const = 0;
	
protected:
	// a commonly used compare scenario
	template <typename T> 
	static int StatCompare(const T &t1, const T &t2) {
		if (t1 == t2) return 0;
		return (t1 < t2) ? -1 : 1;
	}

	static func2vect_t& func2vect;
}; // class CWinProfStatistics

func2vect_t& CWinProfStatistics::func2vect(CStatManager::GetDataBaseRef());