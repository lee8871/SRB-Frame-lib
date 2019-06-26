#include "PerformanceAnalyzer.h"


using namespace std;
namespace srb {

#ifdef WINDOW_86
#include <time.h>  
#include <windows.h>
	void PerformanceTimer::initTimes() {
		if (cpu_freq == -1) {
			LARGE_INTEGER fcpu;
			QueryPerformanceFrequency(&fcpu);
			cpu_freq = fcpu.QuadPart;
		}
	}
	__int64 PerformanceTimer::getTimesUs()	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return time.QuadPart*1000000 / cpu_freq;
	}
#endif

#if UNIX
#include <sys/time.h> 
#include <sched.h>
#include <unistd.h>

	void PerformanceTimer::initTimes() {
	}
	long long int PerformanceTimer::getTimesUs() {
		struct timeval time;
		while ( 0 != gettimeofday(&time, nullptr) );
		long long int rev = time.tv_usec+time.tv_sec*1000*1000;
		return rev;
	}
#endif





PerformanceTimer::PerformanceTimer( ){
	initTimes();
}

PerformanceTimer::~PerformanceTimer(){
}

int PerformanceTimer::beginCheck(){
	check_buf = getTimesUs();
	_is_checking = true;
	return done;
}

int PerformanceTimer::endCheck(){
	long long int  end = getTimesUs();
	_is_checking = false;
	_last_time_cost = (int)(end - check_buf);
	return done;
}

}