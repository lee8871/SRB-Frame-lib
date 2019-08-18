#pragma once 

namespace lee8871_support {


	struct reportNum {
		int rev;
		const char * name;
	};
	class subSystem {
	private:
		bool isRecordOpen;
		int id;
		char* const name;
		reportNum *rev_table;
	};

	class cLogger {
	private:
		static const int BUF_LEN = 4096;
		char last_error_string[BUF_LEN];
		static const int LOG_TYPE_MAX = 127;
		char is_log_enable[LOG_TYPE_MAX] = {0};
		int(*srbErrorReportCB)(char *) = nullptr;
		int setLogHead(const char* head, int type);
		int sendFileHead();
	public:
		int crashPrint(const char *format, ...);
		int errPrint(const char *format, ...);
		int infoPrint(int info_type, const char *format, ...);
		int setReportCallback(int(*srbErrorReportCB)(char *));
		int onLogType(int info_type, const char *format, ...);
		int offLogType(int info_type, const char *format, ...);
		
		const char * const& Last_error_str = last_error_string;
	};
	extern cLogger logger;
	int enalbeLog(const char* pathname);
	int enalbeLogToEnv();

}