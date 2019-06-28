#include "Log.h"
#include "lee.h"
#include <iostream> 
#include <stdio.h> 

using namespace std;
namespace srb {
	int cLog::errPrint(const char *format, ...){
		int str_len_inc = 0;
		str_len_inc += sprintf_s((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "[SRBERR][]:");
		va_list args;
		__crt_va_start(args, format);
		str_len_inc += vsprintf_s((last_error_string + str_len_inc), BUF_LEN - str_len_inc, format, args);
		__crt_va_end(args);
		if (nullptr != srbErrorReportCB) {
			srbErrorReportCB(last_error_string);
		}
		return str_len_inc;
	}
	int cLog::setReportCallback(int(*srbErrorReportCB)(char *)){
		this->srbErrorReportCB = srbErrorReportCB;
		return 0;
	}
	cLog log;

}