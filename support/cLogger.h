#pragma once 

#include <cstdio>
namespace lee8871_support {
	class ModuleLog;
	enum class eLogLevel {
		fatal = 0,erro = 1,warn = 2,info = 3,debug = 4
	};
	class iLog {
	public:
	/*	virtual iLog& begin(const char* str) = 0;
		virtual iLog& operator << (const char* str) = 0;
		virtual iLog& operator << (int str) = 0;
		virtual iLog& operator << (long int str) = 0;
		virtual iLog& operator << (float str) = 0;
		virtual iLog& operator << (double str) = 0;
		virtual void end() = 0;
		virtual int appendLogBegin(Module *m, eLogLevel l) = 0;
		*/
		virtual int addLog(ModuleLog * m, eLogLevel l, const char * format, va_list args) = 0;
	};
	class ModuleLog {
	private :
		const char* _name;
		iLog* _log;
	public :
		eLogLevel level;
		ModuleLog(iLog* log, const char * name, eLogLevel lv = eLogLevel::erro);
		iLog* const &  Log = _log;
		const char* const & Name = _name;
		int addLog(eLogLevel l, const char* format, ...);
	};

	class cLogger :public iLog {
	private:
		char* _ptr;
		char* _end;
		static const int BUF_LEN = 4096;
		//char* buf;
		char _buf[BUF_LEN];


		static const int LOG_TYPE_MAX = 127;
		char is_log_enable[LOG_TYPE_MAX] = {0};

		int appendTime();
		void appendName(const char * name);
		void appendLevel(eLogLevel l);
		int append(char a);
		int append(const char* a);

		void reset();
		void clear();
		int checkOverflow();
		
		int sendFileHead();
		int(*srbErrorReportCB)(char *) = nullptr;
	public:
		const char * const& Last_error_str = _buf;
		cLogger();
		int addLog(ModuleLog *m, eLogLevel l, const char *format, va_list args);
		int print(const char *format, ...);
		int vprint(const char *format, va_list args);
		int setReportCallback(int(*srbErrorReportCB)(char *));
		void end() {}



	};


	extern cLogger logger;
	int enalbeLog(const char* pathname);
	int enalbeLogToEnv();

}