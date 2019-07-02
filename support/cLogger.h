﻿#pragma once 

namespace srb {
	class cLogger {
	private:
		static const int BUF_LEN = 4096;
		char last_error_string[BUF_LEN];
		int(*srbErrorReportCB)(char *);
	public:
		int errPrint(const char *format, ...);
		int setReportCallback(int(*srbErrorReportCB)(char *));
	};
	extern cLogger logger;

}