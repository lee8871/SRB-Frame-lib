#pragma once
#include "lee.h"
#include <iostream>
namespace srb {
	constexpr unsigned int getId(const char * str) {
		int i = 0;
		unsigned int id = 0;
		unsigned int id_temp = 1;
		const int mod = 1610612741;
		unsigned char mapped = 0;
		while (1) {
			if (str[i] == 0) { return (id^id_temp); }
			//map char form ASCII to quick mapping table str shold in lower case letters 				
			mapped = (unsigned char)(str[i] - 'a' + 1);
			if (mapped > 0x31) {
				mapped = (27 + mapped % 5);
			}
			id_temp ^= ((unsigned int)mapped) << (1 + (i % 6) * 5);
			i++;
			if ((i % 6) == 0) {
				id ^= id_temp;
				id *= mod;
				id_temp = 1;
			}
		}
	}
};