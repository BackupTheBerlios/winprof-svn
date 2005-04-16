#pragma once

#include "StatManager.h"

class CWinProfStatistics 
{
public:
	CWinProfStatistics(void) {}
	virtual ~CWinProfStatistics(void) {}

	virtual CString GetString(const INVOC_INFO& call) const {return CString();}
	virtual int StatCompare(const INVOC_INFO &c1, const INVOC_INFO &c2) const {return 0;}
	virtual CString GetStatName(void) const {return CString();}
	virtual int GetWidth(void) const {return 100;}
	virtual int GetColumnPlace(void) const {return LVCFMT_LEFT;}
	virtual bool IsPerInvocation(void) const {return false;}
	virtual void ClearCache(void) const {}
	
protected:
	// a commonly used compare scenario
	template <typename T> 
	static int StatCompare(const T &t1, const T &t2) {
		if (t1 == t2) return 0;
		return (t1 < t2) ? -1 : 1;
	}

	static const func2vect_t& func2vect;
}; // class CWinProfStatistics