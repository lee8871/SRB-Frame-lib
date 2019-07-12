#include <iostream> 
#include <stdio.h>
#include <stdarg.h>

#include "cLogger.h"
#include "lee.h"
#include "transform.h"
#include "OsSupport.h"

using namespace std;
namespace srb {
	int cLogger::setLogHead(const char* head,int type){	
		int str_len_inc = 0;
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "[%s-%c][",head,type);
		str_len_inc += trans::usTotimestr((last_error_string + str_len_inc), BUF_LEN - str_len_inc, OsSupport::getTimesUs());
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "]:");    
		return str_len_inc;
	}

	int cLogger::sendHead(){
		int str_len_inc = 0;
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "\n========================= SRB LOG =============================\n");
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "[SRBLOG][");
		str_len_inc += trans::usTotimestr((last_error_string + str_len_inc), BUF_LEN - str_len_inc, OsSupport::getTimesUs());
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "]:"); 
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "Log ReportCallback is set.\n");
		srbErrorReportCB(last_error_string);
		return str_len_inc;
		
	}

	int cLogger::errPrint(const char *format, ...){
		int str_len_inc = setLogHead("SRBERROR",0);
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
	int cLogger::crashPrint(const char *format, ...){
		int str_len_inc = setLogHead("SRBCRASH",0);
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
	
	int cLogger::infoPrint(int info_type, const char *format, ...){
		if(is_log_enable[info_type] == 0){
			return 0;
		}
		int str_len_inc = setLogHead("SRBLOG",info_type);
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
	int  cLogger::increaseLogType(int log_type){
		if(log_type>=LOG_TYPE_MAX){
			errPrint("log_type(%d) shold less than LOG_TYPE_MAX(%d).",log_type,LOG_TYPE_MAX);
		}
		if(is_log_enable[log_type] == 100){
			return 100;
		}
		is_log_enable[log_type]++;
		return is_log_enable[log_type];
	}
	int  cLogger::decreaseLogType(int log_type){
		if(log_type>=LOG_TYPE_MAX){
			errPrint("log_type(%d) shold less than LOG_TYPE_MAX(%d).",log_type,LOG_TYPE_MAX);
		}
		if(is_log_enable[log_type] == 0){
			return 0;
		}
		is_log_enable[log_type]--;
		return is_log_enable[log_type];
	}
	int cLogger::setReportCallback(int(*srbErrorReportCB)(char *)){
		this->srbErrorReportCB = srbErrorReportCB;
		if (nullptr != srbErrorReportCB) {
			sendHead();
		}
		return 0;
	}
	cLogger logger;

}