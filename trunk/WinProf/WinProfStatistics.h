#ifndef WIN_PROF_STATISTICS
#define WIN_PROF_STATISTICS

namespace WIN_PROF_STATISTICS {

template <typename T> class WinProfStatistics {
public:
	WinProfStatistics(void) {
		// requirements
		T val;
		T val1;
		val = val1;
		if (val == val1) { ; }
		if (val < val1) { ; }
	}
	virtual ~WinProfStatistics(void);

	virtual T GetStatValue(const RUN_INFO& ri);
	virtual BOOL UpdateWith(const RUN_INFO& ri);

	typedef INT (*comp_func_t)(const T&, const T&);

	// we could implement this as a public field ???
	virtual comp_func GetCompareFunc(void);
}; // class WinProfStatistics

// a template "default" function
INT StatCompare(const T &t1, const T &d2) {
	if (t1 == t2) return 0;
	return (t1 < t2) ? -1 : 1;
}

}; // namespace

#endif // WIN_PROF_STATISTICS