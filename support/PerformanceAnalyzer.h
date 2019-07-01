#pragma once
#include "lee.h"
#include "OsSupport.h"
namespace srb {
	class PerformanceTimer {
	private:
		int cpu_freq = -1;	
		tUs  check_buf;
		bool _is_checking;
		int _last_time_cost;
	public:

		int const & Last_time_cost = _last_time_cost;
		bool const & Is_checking = _is_checking;
	public:
		PerformanceTimer();
		~PerformanceTimer();
		int beginCheck();
		int endCheck();
	};
};