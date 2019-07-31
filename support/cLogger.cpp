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
		switch(type){
		case -1:
			str_len_inc += snprintf((last_error_string + str_len_inc), BUF_LEN - str_len_inc, "[%s][", head);
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




	

FILE *fp = nullptr;
int writeToLog(char* str);
int enalbeLog(const char* pathname) {
	if (fp != nullptr) {
		fclose(fp);
		fp = nullptr;
	}
	if (pathname[0] == '~') {
		char expandedPathName[256];
		snprintf(expandedPathName, 256, "%s%s", getenv("HOME"), pathname + 1);
		fp = fopen(expandedPathName, "a");
	}
	else {
		fp = fopen(pathname, "a");
	}

	if (fp == nullptr) {
		return fail;
	}
	else {
		logger.setReportCallback(writeToLog);
	}
	return done;
	//TODO:: close file
}
int enalbeLogToEnv() {
	if (fp != nullptr) {
		return redo;
	}
	char expandedPathName[256];
	int str_len_inc = 0;
	char * srb_base_ENV = getenv("SRB_BASE");
	if (srb_base_ENV == nullptr) {
		return fail;
	}	
	str_len_inc += snprintf((expandedPathName + str_len_inc), 256 - str_len_inc, "%s/log", srb_base_ENV);
	str_len_inc += trans::usTotimestr((expandedPathName + str_len_inc), 256 - str_len_inc, OsSupport::getTimesUs());
	str_len_inc += snprintf((expandedPathName + str_len_inc), 256 - str_len_inc, ".log");
	fp = fopen(expandedPathName, "a");
	if (fp == nullptr) {
		return fail;
	}
	else {
		logger.setReportCallback(writeToLog);
	}
	return done;
}
int writeToLog(char* str) {
	fprintf(fp, "%s", str);
	fflush(fp);
	return done;
}


}