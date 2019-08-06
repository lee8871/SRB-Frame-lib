#pragma once
#include "lee.h"
using namespace lee8871_support;
namespace srb {
	class OsSupport {
	public :
		static tUs getTimesUs();
		static void msSleep(int ms);
		static void setPriority(void);
	};
};