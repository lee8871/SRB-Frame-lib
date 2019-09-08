#include <cstdio> 
#include <stdarg.h>

/*
#include <unistd.h> 
unistd.h in windows shold be following: 
#include <io.h>
#include <process.h>
*/

#include "cLogger.h"
#include "lee.h"
#include "transform.h"
#include "OsSupport.h"

using namespace std;
namespace lee8871_support {
#define addAndCheck(a) *(_ptr++)= (a);if (_ptr >= _end) { *(--_ptr) = 0;return buf_use_up; }
#define endString() *_ptr = 0; return done;

	static cLogger* logger = nullptr;
	static constexpr char* enum_log_level_name[] = {
	"fatal","error","warn ","info ","debug"
	
	};

	class cLogger::Impl {
	public:
		char* _ptr;
		char* _end;
		static const int BUF_LEN = 4096;
		//char* buf;
		char _buf[BUF_LEN];
		static const int LOG_TYPE_MAX = 127;
		char is_log_enable[LOG_TYPE_MAX] = { 0 };

		int appendTime() {
			append('[');
			size_t inc = trans::usTotimestr(_ptr, _end - _ptr, getTimesUs());
			if (inc < 0) {
				*_ptr = 0;
				return fail;
			}
			_ptr += inc;
			append(']');
			return checkOverflow();
		}
		void appendName(const char * name) {
			append('[');
			append(name);
			append(']');
		}
		void appendLevel(eLogLevel l) {
			append('[');
			append(enum_log_level_name[(int)l]);
			append(']');
		}

		int append(char a) {
			addAndCheck(a);
			endString();
		}
		int append(const char * a) {
			while (*a != 0) {
				addAndCheck(*a);
				a++;
			}
			endString();
		}
		int print(const char *format, ...) {
			va_list args;
			va_start(args, format);
			int rev = vprint(format, args);
			va_end(args);
			return rev;
		}
		int vprint(const char * format, va_list args) {
			int inc = vsnprintf(_ptr, _end - _ptr, format, args);
			if (inc < 0) {
				*_ptr = 0;
				return inc;
			}
			_ptr += inc;
			return checkOverflow();
		}
		/*
		void reset() {
			_ptr = _buf;
		}
		*/
		void clear() {
			_ptr = _buf;
			*_ptr = 0;
		}

		int checkOverflow() {
			if (_ptr == (_end - 1)) {
				return buf_use_up;
			}
			else {
				return done;
			}
		}
		
		
		
		void sendFileHead() {
			clear();
			appendTime();
			appendLevel(eLogLevel::info);
			print("logger to file success! Module built at %s %s.", __DATE__, __TIME__);
			writeToLog();
			return;
		}

		inline int addLog(ModuleLog * m, eLogLevel l, const char * format, va_list args) {
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
			int rev = vprint(format, args);
			writeToLog();
			return done;
		}


		Impl(cLogger *) {
			_ptr = _buf;
			_end = _buf + BUF_LEN;
			openLogToEnv();
			sendFileHead();
		}
		~Impl() {
			closeLog();
		}


		FILE *fp = nullptr;
		#define log_path_remain_len (log_path + FILENAME_MAX - log_ptr)
		void openLogToEnv() {
			if (fp != nullptr) {
				return;
			}
			char log_path[FILENAME_MAX];
			char * log_ptr = log_path;
			char * srb_base_ENV = getenv("SRB_BASE");
			if (srb_base_ENV == nullptr) {
				log_ptr += snprintf(log_ptr, log_path_remain_len, "./log_");
			}
			else {
				if (srb_base_ENV[0] == '~') {
					log_ptr += snprintf(log_ptr, log_path_remain_len, "%s%s/log_", getenv("HOME"), srb_base_ENV + 1);
				}
				else {
					log_ptr += snprintf(log_ptr, log_path_remain_len, "%s/log_", srb_base_ENV);
				}
			}

			int rev = getProcessName(log_ptr, log_path_remain_len);
			if (rev > 0) {
				log_ptr += rev;
			}
			log_ptr += snprintf(log_ptr, log_path_remain_len, "_");
			log_ptr += trans::usTotimestr(log_ptr, log_path_remain_len, getTimesUs());
			log_ptr += snprintf(log_ptr, log_path_remain_len, ".log");

			fp = fopen(log_path, "a");
			if (fp == nullptr) {
				setTerminalColor(eTerminalColor::error);
				printf("==== LOG ERROR ====\n Log file can not open or create log file <%s>\n", log_path);
				setTerminalColor(eTerminalColor::normal);
				printf("Enter to exit");
				while ('\n' != getchar());
				exit(-1);
			}
			return;
		}
		void closeLog(){
			if (fp != nullptr) {
				fclose(fp);
				fp = nullptr;
			}
		}
		void writeToLog() {
			if (fp != nullptr) {
				fprintf(fp, "%s", _buf);
				fflush(fp);
				return;
			}
			else {
				setTerminalColor(eTerminalColor::error);
				printf("==== LOG ERROR ====\n Log no initialization but write to file.\n");
				printf("%s\n", _buf);
				setTerminalColor(eTerminalColor::normal);
				printf("Enter to exit");
				while ('\n' != getchar());
				exit(-1);
			}
		}
	};


	cLogger::cLogger() {
		pimpl = new Impl(this);
	}
	cLogger::~cLogger() {
		delete pimpl;
	}

	int cLogger::addLog(ModuleLog * m, eLogLevel l, const char * format, va_list args)	{
		return pimpl->addLog(m,l,format,args);
	}
	
	ModuleLog::ModuleLog(const char * name, const char* data, const char* time){
		_name = name;
		level = eLogLevel::erro;
		addLogFocue(eLogLevel::info, "Module %s loaded, log level %s. Module built at %s %s.",
			name, enum_log_level_name[(int)level], data,time);
		return;
	}

	ModuleLog::ModuleLog(const char * name, eLogLevel lv, const char* data, const char* time) {
		_name = name;
		level = lv;
		addLogFocue(eLogLevel::info, "Module %s loaded, log level %s. Module built at %s %s.",
			name, enum_log_level_name[(int)level], data,time);
		return;
	}

	int ModuleLog::addLog(eLogLevel l, const char * format, ...) {
		if (l <= level) {
			va_list args;
			va_start(args, format);
			int rev = logger->addLog(this, l, format, args);
			va_end(args);
			return rev;
		}
		return done;
	}
	int ModuleLog::addLogFocue(eLogLevel l, const char * format, ...) {
		va_list args;
		va_start(args, format);
		int rev = logger->addLog(this, l, format, args);
		va_end(args);
		return rev;
	}


	static int is_init_counter = 0;
	CLOGGER_INITIELAZATION_CLASS::CLOGGER_INITIELAZATION_CLASS(){
		if (is_init_counter == 0) {
			logger = new cLogger();
		}
		is_init_counter++;
	}
	CLOGGER_INITIELAZATION_CLASS::~CLOGGER_INITIELAZATION_CLASS()	{
		is_init_counter--;
		if (is_init_counter == 0) {			
			delete logger;
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