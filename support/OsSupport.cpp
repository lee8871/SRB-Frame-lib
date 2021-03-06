﻿#include <stdio.h>
#include "lee.h"
#include "OsSupport.h"


namespace lee8871_support {
#ifdef WINDOW_64
#include <time.h>  
#include <winsock.h>
#include <minwindef.h>
#include <windows.h>

	#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
	#define DELTA_EPOCH_IN_US  11644473600000000Ui64
	#else
	#define DELTA_EPOCH_IN_US  11644473600000000ULL
	#endif
	tUs getTimesUs() {
		FILETIME t;
		GetSystemTimeAsFileTime(&t);
		tUs rev;
		rev = *((tUs*)(&t));
		rev /= 10;
		rev -= DELTA_EPOCH_IN_US;
		return rev;
	}
	void msSleep(int ms) {
		Sleep(ms);
	}
	void setPriority() {
		return;
	}
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	static int color_argument_table[4] = {
		0x07,
		FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED,
		FOREGROUND_INTENSITY | FOREGROUND_RED
	};

	void setTerminalColor(eTerminalColor color) {
		SetConsoleTextAttribute(handle, color_argument_table[(int)color]);
	}

	int getProcessName(char* ptr, int length) {
		char * exe_path = new char[FILENAME_MAX];
		if (exe_path == nullptr) {
			return new_fail;
		}
		GetModuleFileNameA(NULL, exe_path, MAX_PATH);

		char *exe_path_end = (exe_path + FILENAME_MAX);
		char * name = nullptr;
		for (char * scan = exe_path; scan < exe_path_end;scan++) {
			if (*scan == '\\') {
				name = scan + 1;
			}
			else if (*scan == '.') {
				*scan = '_';
			}
			else if (*scan == 0) {
				break;
			}
		}
		if (name == nullptr) {
			return fail;
		}
		char * ptr_base = ptr;
		int i = 0;
		for(; i<(length-1); i++) {
			ptr[i] = name[i];
			if ((name[i] == 0)) {
				break;
			}
		}
		ptr[i] = 0;
		return i;
	}


#endif


#ifdef WINDOW_86
#include <time.h>  
#include <winsock.h>
#include <minwindef.h>
#include <windows.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_US  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_US  11644473600000000ULL
#endif
	tUs getTimesUs() {
		FILETIME t;
		GetSystemTimeAsFileTime(&t);
		tUs rev;
		rev = *((tUs*)(&t));
		rev /= 10;
		rev -= DELTA_EPOCH_IN_US;
		return rev;
	}
	void msSleep(int ms) {
		Sleep(ms);
	}
	void setPriority() {
		return;
	}
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	static int color_argument_table[4] = {
		0x07,
		FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED,
		FOREGROUND_INTENSITY | FOREGROUND_RED
	};

	void setTerminalColor(eTerminalColor color) {
		SetConsoleTextAttribute(handle, color_argument_table[(int)color]);
	}

	int getProcessName(char* ptr, int length) {
		char* exe_path = new char[FILENAME_MAX];
		if (exe_path == nullptr) {
			return new_fail;
		}
		GetModuleFileNameA(NULL, exe_path, MAX_PATH);

		char* exe_path_end = (exe_path + FILENAME_MAX);
		char* name = nullptr;
		for (char* scan = exe_path; scan < exe_path_end; scan++) {
			if (*scan == '\\') {
				name = scan + 1;
			}
			else if (*scan == '.') {
				*scan = '_';
			}
			else if (*scan == 0) {
				break;
			}
		}
		if (name == nullptr) {
			return fail;
		}
		char* ptr_base = ptr;
		int i = 0;
		for (; i < (length - 1); i++) {
			ptr[i] = name[i];
			if ((name[i] == 0)) {
				break;
			}
		}
		ptr[i] = 0;
		return i;
	}


#endif



#if UNIX
#include <sys/time.h> 
#include <sched.h>
#include <unistd.h>
	tUs getTimesUs() {
		struct timeval time;
		while (0 != gettimeofday(&time, nullptr));
		tUs rev = time.tv_usec + time.tv_sec * (1000 * 1000);
		return rev;
	}

	void msSleep(int ms) {
		usleep(ms * 1000);
	}

	void setPriority() {
		struct sched_param param;
		int maxpri;
		maxpri = sched_get_priority_max(SCHED_FIFO);
		param.sched_priority = maxpri;
		if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) {
			printf("Set priority fail\n");
		}
		else {
			printf("Set priority done\n");
		}
	}	
	static const char* color_argument_table[4] = {
		"\033[0m",
		"\033[32m",
		"\033[33m",
		"\033[31m"
	};

	void setTerminalColor(eTerminalColor color) {
		switch(color){
			case eTerminalColor::normal:
			printf("\033[0m");
			break;
			case eTerminalColor::highlight:
			printf("\033[32m");
			break;
			case eTerminalColor::warning:
			printf("\033[33m");
			break;
			case eTerminalColor::error:
			printf("\033[31m");
			break;

		}
		//	printf(  color_argument_table[(int)color]  );
	}



	int getProcessName(char* ptr, int length) {

		char * exe_path = new char[FILENAME_MAX];
		if (exe_path == nullptr) {
			return new_fail;
		}
		readlink("/proc/self/exe", exe_path, FILENAME_MAX);
		for (char * ptr = exe_path; ptr < (exe_path + FILENAME_MAX);ptr++) {
			if (*ptr == '/') {
				exe_path = ptr + 1;
			}
			else if (*ptr == 0) {
				break;
			}
		}
		char *exe_path_end = (exe_path + FILENAME_MAX);
		char * name = nullptr;
		for (char * scan = exe_path; scan < exe_path_end; scan++) {
			if (*scan == '/') {
				name = scan + 1;
			}
			else if (*scan == 0) {
				break;
			}
		}
		if (name == nullptr) {
			return fail;
		}
		char * ptr_base = ptr;
		int i = 0;
		for (; i < (length - 1); i++) {
			ptr[i] = name[i];
			if ((name[i] == 0)) {
				break;
			}
		}
		ptr[i] = 0;
		return i;
	}
#endif
};