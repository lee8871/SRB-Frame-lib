#include <iostream> 
#include <stdarg.h>
#include "cLogger.h"
#include "lee.h"
#include "transform.h"
#include "OsSupport.h"

using namespace std;
namespace lee8871_support {


	static const char* enum_log_level_name[] = {
		"fatal","erro","warn","info","debug"
	};
#define addAndCheck(a) *(_ptr++)= (a);if (_ptr >= _end) { *(--_ptr) = 0;return buf_use_up; }
#define endString() *_ptr = 0; return done;

	int cLogger::appendTime() {
		append('[');
		size_t inc = trans::usTotimestr(_ptr, _end - _ptr, OsSupport::getTimesUs());
		if (inc < 0) {
			*_ptr = 0;
			return fail;
		}
		_ptr += inc;
		append(']');
		return checkOverflow();
	}
	void cLogger::appendName(const char * name){
		append('[');
		append(name);
		append(']');
	}
	void cLogger::appendLevel(eLogLevel l) {
		append('[');
		append(enum_log_level_name[(int)l]);
		append(']');
	}

	int cLogger::append(char a) {
		addAndCheck(a);
		endString();
	}
	void cLogger::reset() {
		_ptr = _buf;
	}
	void cLogger::clear() {
		_ptr = _buf;
		*_ptr = 0;
	}
	int cLogger::checkOverflow() {
		if (_ptr == (_end - 1)) {
			return buf_use_up;
		}
		else {
			return done;
		}
	}
	inline int cLogger::sendFileHead() {
		append("\n\n\n\n=============================== SRB LOG ===============================\n");
		appendTime();
		appendName("logger");
		appendLevel(eLogLevel::info);
		append("Log ReportCallback is set.\n");
		srbErrorReportCB(_buf);
		return checkOverflow();
	}
	int cLogger::append(const char * a) {
		while (*a != 0) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}
	
	inline cLogger::cLogger() {
		_ptr = _buf;
		_end = _buf + BUF_LEN;
	}

	int cLogger::addLog(ModuleLog * m, eLogLevel l, const char * format, va_list args){
		if (_ptr >= _buf) {
			if (*(_ptr - 1) != '\n') {
				clear();
				append('\n');
			}
			else {
				clear();
			}
		}
		appendTime();
		appendName(m->Name);
		appendLevel(l);
		return vprint(format, args);
	}


	int cLogger::print(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int rev = vprint(format, args);
		va_end(args);
		return rev;
	}

	int cLogger::vprint(const char * format, va_list args){
		int inc = vsnprintf(_ptr, _end - _ptr, format, args);
		if (inc < 0) {
			*_ptr = 0;
			return fail;
		}
		_ptr += inc;
		srbErrorReportCB(_buf);
		return checkOverflow();
	}

	int cLogger::setReportCallback(int(*srbErrorReportCB)(char *)) {
		this->srbErrorReportCB = srbErrorReportCB;
		if (nullptr != srbErrorReportCB) {
			sendFileHead();
		}
		return 0;
	}

	cLogger logger;



#define checkFailReturn(value) do{\
		int rev = (value);\
		if (done != rev){return rev;}\
	}while(0)


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
			checkFailReturn(fopen_s(&fp, expandedPathName, "a"));
		}
		else {
			checkFailReturn(fopen_s(&fp, pathname, "a"));
		}
		logger.setReportCallback(writeToLog);
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
	ModuleLog::ModuleLog(iLog * log, const char * name, eLogLevel lv) {
		_name = name;
		_log = log;
		level = lv;
	}
	int ModuleLog::addLog(eLogLevel l, const char * format, ...) {
		if (l <= level) {
			va_list args;
			va_start(args, format);
			int rev = _log->addLog(this, l, format, args);
			va_end(args);
			return rev;
		}
	}
}
