#include <iostream> 
#include <stdarg.h>
#include "cLogger.h"
#include "lee.h"
#include "transform.h"
#include "OsSupport.h"

using namespace std;
namespace lee8871_support {


	static const char* enum_log_level_name[] = {
		"fatal","error","warn ","info ","debug"
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
	int cLogger::sendFileHead() {
		clear();
		appendTime();
		appendLevel(eLogLevel::info);
		appendName("logger");
		print("cLogger module built at %s %s",__DATE__,__TIME__);
		return checkOverflow();
	}
	int cLogger::append(const char * a) {
		while (*a != 0) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}
	
	cLogger::cLogger() {

		_ptr = _buf;
		_end = _buf + BUF_LEN;
		if (done == openLogToEnv()) {
			sendFileHead();
		}
	}

	cLogger::~cLogger()	{
		closeLog();
	}

	inline int cLogger::openLogToEnv() {
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
		return done;
	}

	int cLogger::closeLog()
	{
		if (fp != nullptr) {
			fclose(fp);
			return done;
		}
		else {
			return redo;
		}
	}

	inline int cLogger::writeToLog() {
		fprintf(fp, "%s", _buf);
		fflush(fp);
		return done;
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
		appendLevel(l);
		appendName(m->Name);
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
		writeToLog();
		return checkOverflow();
	}


	ModuleLog::ModuleLog(iLog* log, const char * name, 
		const char * date, const char * time, 
		eLogLevel lv
	
	){
		_name = name;
		_log = log;
		level = lv;
		addLogFocue(eLogLevel::info, "Module %s loaded, log level %s, built at %s %s.", name,enum_log_level_name[(int)lv], date,time);

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
	int ModuleLog::addLogFocue(eLogLevel l, const char * format, ...) {
		va_list args;
		va_start(args, format);
		int rev = _log->addLog(this, l, format, args);
		va_end(args);
		return rev;
	}





	static bool is_inited = false;
	cLogger* logger = nullptr;

	cLoggerInit::cLoggerInit(){
		if (is_inited == false) {
			is_inited = true;
			logger = new cLogger();
		}
	}

	cLoggerInit::~cLoggerInit()	{
		if (is_inited == true) {
			is_inited = false;
			

			//	delete logger;
		}
	}

}
/*
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
	}*/