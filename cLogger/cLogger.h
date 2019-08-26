#pragma once 
#include <cstdio>

namespace lee8871_support {
	class ModuleLog;
	class cLogger;


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
	private :
		const char* _name;
	public :
		eLogLevel level;
		ModuleLog(const char * name, const char* data, const char* time);
		ModuleLog(const char * name, eLogLevel lv, const char* data, const char* time);
		const char* const & Name = _name;
		int addLog(eLogLevel l, const char* format, ...);
		int addLogFocue(eLogLevel l, const char * format, ...);
	};

	class cLogger :public iLog {
	private:
		class Impl;
		Impl* pimpl;

	public:
		cLogger();
		~cLogger();
		//const char * const& Last_error_str = _buf;
		int addLog(ModuleLog *m, eLogLevel l, const char *format, va_list args);
		void end() {}
	};

	class CLOGGER_INITIELAZATION_CLASS {
	public:
		CLOGGER_INITIELAZATION_CLASS();
		~CLOGGER_INITIELAZATION_CLASS();
	};
	static CLOGGER_INITIELAZATION_CLASS __CLOGGER_INITIELAZATION_OBJECT;
}