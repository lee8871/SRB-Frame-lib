#pragma once
#include "lee.h"
#include <iostream>
#include "OsSupport.h"
namespace lee8871_support {
	namespace trans {
		char u8ToLow(srb::uint8  u8);
		int usTotimestr(char* out_str, int len, tUs us_time);	
	}
};