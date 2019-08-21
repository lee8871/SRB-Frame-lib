#include "PerformanceAnalyzer.h"
#include "OsSupport.h"


using namespace std;
namespace lee8871_support {
	PerformanceTimer::PerformanceTimer() {
	}
	PerformanceTimer::~PerformanceTimer(){
	}

	int PerformanceTimer::beginCheck(){
		check_buf = getTimesUs();
		_is_checking = true;
		return done;
	}

	int PerformanceTimer::endCheck(){
		tUs  _end = getTimesUs();
		_is_checking = false;
		_last_time_cost = (int)(_end - check_buf);
		return done;
	}

}