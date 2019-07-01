#pragma once
#include "lee.h"
namespace srb {
	class OsSupport {
	public :
		static tUs getTimesUs();
		static void msSleep(int ms);
		static void setPriority(void);
	};
};