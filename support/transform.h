#pragma once
#include "lee.h"
#include <iostream>
using namespace lee8871_support;
namespace srb {
	namespace trans {
		char u8ToLow(uint8 u8);
		int usTotimestr(char* out_str, int len, tUs us_time);	
	}
};