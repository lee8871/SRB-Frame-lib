#pragma once
#include "lee.h"
namespace lee8871_support {
	using tUs = long long int;
	tUs getTimesUs();
	void msSleep(int ms);
	void setPriority(void);	
	
	enum class eTerminalColor {
		normal, highlight, warning, error
	};
	void setTerminalColor(eTerminalColor color);
	int getProcessName(char* ptr, int length);
};