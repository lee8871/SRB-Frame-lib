#pragma once
#include "lee.h"
namespace lee8871_support {
	using tUs = long long int;
	class OsSupport {
	public :
		static tUs getTimesUs();
		static void msSleep(int ms);
		static void setPriority(void);
	};
};