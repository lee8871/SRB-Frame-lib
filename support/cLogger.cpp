﻿#include <iostream> 
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
		switch(type){
		case -1:
			str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "[%s][", head, type);
			break;
		case -2:
			str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "%s[", head);
			break;
		default:
			str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "[%s-%c][", head, type);
			break;
		}
		str_len_inc += trans::usTotimestr((last_error_string + str_len_inc), BUF_LEN - str_len_inc, OsSupport::getTimesUs());
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "]:");    
		return str_len_inc;
	}

	int cLogger::sendFileHead(){
		int str_len_inc = setLogHead("=============================== SRB LOG ===============================\n[LOG]",-2);
		str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "Log ReportCallback is set.\n");
		srbErrorReportCB(last_error_string);
		return str_len_inc;
		
	}

	int cLogger::errPrint(const char *format, ...){
		int str_len_inc = setLogHead("ERROR",-1);
		va_list args;
		va_start(args, format);
		str_len_inc += vsnprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, format, args);
		va_end(args);
		last_error_string[str_len_inc++] = '\n';
		last_error_string[str_len_inc] = '\0';
		if (nullptr != srbErrorReportCB) {
			srbErrorReportCB(last_error_string);
		}
		return str_len_inc;
	}
	int cLogger::crashPrint(const char *format, ...){
		int str_len_inc = setLogHead("CRASH",-1);
		va_list args;
		va_start(args, format);
		str_len_inc += vsnprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, format, args);
		va_end(args);
		last_error_string[str_len_inc++] = '\n';
		last_error_string[str_len_inc] = '\0';
		if (nullptr != srbErrorReportCB) {
			srbErrorReportCB(last_error_string);
		}
		return str_len_inc;
	}
	
	int cLogger::infoPrint(int info_type, const char *format, ...){
		if(is_log_enable[info_type] == 0){
			return 0;
		}
		int str_len_inc = setLogHead("LOG",info_type);
		va_list args;
		va_start(args, format);
		str_len_inc += vsnprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, format, args);
		va_end(args);
		last_error_string[str_len_inc++] = '\n';
		last_error_string[str_len_inc] = '\0';
		if (nullptr != srbErrorReportCB) {
			srbErrorReportCB(last_error_string);
		}
		return str_len_inc;
	}
	int  cLogger::onLogType(int info_type, const char *format, ...) {
		is_log_enable[info_type] = yes;
		int str_len_inc = setLogHead("LOGON", info_type);
		va_list args;
		va_start(args, format);
		str_len_inc += vsnprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, format, args);
		va_end(args);
		last_error_string[str_len_inc++] = '\n';
		last_error_string[str_len_inc] = '\0';
		return done;
	}
	int  cLogger::offLogType(int info_type, const char *format, ...) {
		is_log_enable[info_type] = no;
		int str_len_inc = setLogHead("LOGOFF", info_type);
		va_list args;
		va_start(args, format);
		str_len_inc += vsnprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, format, args);
		va_end(args);
		last_error_string[str_len_inc++] = '\n';
		last_error_string[str_len_inc] = '\0';
		return done;
	}
	int cLogger::setReportCallback(int(*srbErrorReportCB)(char *)){
		this->srbErrorReportCB = srbErrorReportCB;
		if (nullptr != srbErrorReportCB) {
			sendFileHead();
		}
		return 0;
	}
	cLogger logger;

}