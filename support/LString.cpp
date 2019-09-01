#include <cstdarg>
#include <cstdio>

#include "LString.h"


namespace lee8871_support {
#define addAndCheck(a) *(_ptr++)= (a);if (_ptr == _end) { *(--_ptr) = 0;return buf_use_up; }
#define endString() *_ptr = 0; return done;


	LString::LString(char *buf, int size) :
		_end(buf + size), _buf(buf), _ptr(buf) {}

	LString::LString(int size){
		_buf = _owned_buf = new(std::nothrow) char[size];
		if (_owned_buf == nullptr) {
			_ptr = _end = _buf;
		}
		else{
			_end = _buf + size;
			_ptr = _buf;
		}
	}

	LString::LString(const char * path) {
		FILE *fp = fopen(path,"r");
		fpos_t fp_0;
		fgetpos(fp,&fp_0);
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);
		_buf = _owned_buf = new(std::nothrow) char[len];
		if (_owned_buf == nullptr) {
			_ptr = _end = _buf;
		}
		else {
			_end = _buf + len;
			_ptr = _buf;
			//TODO read file
		}
		fsetpos(fp, &fp_0);
		char * p = _buf;
		while (1) {
			*p = fgetc(fp);
			p++;
			if (p == _end-1) {
				*p = 0;
				return;
			}
		}

	}
	int LString::writeToFile(const char * path) {
		FILE *fp = fopen(path, "w");
		if (fp == nullptr) {
			return fail;
		}
		fputs(_buf, fp);
		if (fclose(fp) != 0) {
			printf("fclose return error");
			exit(-1);
		}
		return done;
	}





	LString::~LString(){
		if (_owned_buf != nullptr) {
			delete[] _owned_buf;
		}
	}

	int LString::append(char a) {
		addAndCheck(a);
		endString();
	}

	int LString::append(const char* a) {
		while (*a != 0) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}
	int LString::append(const char * a, int length){
		for(int i =0;((i<length)&&(*a!=0));i++) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}
	int LString::print(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int increase = vsnprintf(_ptr, _end - _ptr, format, args);
		va_end(args);
		if (increase < 0) {
			return increase;
		}
		_ptr += increase;
		return checkOverflow();
	}
	int LString::checkOverflow(){
		if (_ptr == (_end - 1)) {
			return buf_use_up;
		}
		else {
			return done;
		}
	}


	int LString::forward(int inc)	{
		_ptr += inc;
		if(_ptr >= _end){
			_ptr = _end - 1;
			return fail;
		}
		else if(_ptr<_buf){
			_ptr = _buf;
			return fail;
		}
		return done;
	}

	bool LString::checkCh(char c) {
		removeSpace();
		if (*_ptr == c) {
			_ptr++;
			return true;
		}
		return false;
	}

	int LString::forward() {
		_ptr++;
		if (_ptr >= _end) {
			_ptr = _end - 1;
			return fail;
		}
		return done;
	}


	bool LString::checkStr(const char * check) {
		removeSpace();
		char* p = _ptr;
		while (*p == *check) {
			p++;check++;
			if (check == 0) {
				_ptr = p;
				return true;
			}
		}
		return false;
	}




};