#include <iostream> 
#include <stdio.h>
#include <stdarg.h>

#include "cLogger.h"
#include "lee.h"
#include "transform.h"
#include "OsSupport.h"

using namespace std;
namespace srb {
	int cLogger::errPrint(const char *format, ...){
		int str_len_inc = 0;
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "\t[SRBERR][]:");
		va_list args;
		va_start(args, format);
		str_len_inc += vsnprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, format, args);
		va_end(args);
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "\n");
		if (nullptr != srbErrorReportCB) {
			srbErrorReportCB(last_error_string);
		}
		return str_len_inc;
	}
	int cLogger::infoPrint(const char *format, ...){
		return 0;
	}
	int cLogger::setReportCallback(int(*srbErrorReportCB)(char *)){
		this->srbErrorReportCB = srbErrorReportCB;
		if (nullptr != srbErrorReportCB) {
			char time_str_temp[32];
			trans::usTotimestr(time_str_temp,32, OsSupport::getTimesUs());
			int str_len_inc = 0;
			str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "[SRB LOG][%s]:",time_str_temp);
			str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "\n");
			srbErrorReportCB(last_error_string);
		}
		return 0;
	}
	cLogger logger;

}