﻿#include "PerformanceAnalyzer.h"
#include "OsSupport.h"


using namespace std;
namespace srb {
	PerformanceTimer::PerformanceTimer() {
	}
	PerformanceTimer::~PerformanceTimer(){
	}

	int PerformanceTimer::beginCheck(){
		check_buf = OsSupport::getTimesUs();
		_is_checking = true;
		return done;
	}

	int PerformanceTimer::endCheck(){
		tUs  _end = OsSupport::getTimesUs();
		_is_checking = false;
		_last_time_cost = (int)(_end - check_buf);
		return done;
	}

}