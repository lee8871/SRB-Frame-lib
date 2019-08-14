#include <cstdarg>
#include <cstdio>

#include "LString.h"


namespace lee8871_support {
#define addAndCheck(a) *(_ptr++)= (a);if (_ptr == _end) { *(--_ptr) = 0;return buf_use_up; }
#define endString() *_ptr = 0; return done;
	LString::LString(char *buf, int size) :
		_end(buf + size), _buf(buf), _ptr(buf) {}

	LString::LString(int size){
		_buf = _owned_buf = new char[size];
		if (_owned_buf == nullptr) {
			_ptr = _end = _buf;
		}
		else{
			_end = _buf + size;
			_ptr = _buf;
		}
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
	void LString::reset() {
		_ptr = _buf;
	}

	void LString::clear()	{
		_ptr = _buf;
		*_ptr = 0;
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

	int LString::foward()	{
		_ptr++;		
		if (_ptr >= _end) {
			_ptr = _end - 1;
			return fail;
		}
		return done;
	}

	int LString::jump(int inc)	{
		_ptr += inc;
		if(_ptr >= _end){
			_ptr = _end - 1;
			return fail;
		}
		else if(_ptr<_buf){
			_ptr = _buf;
			return fail;
		}
	}

	bool LString::checkCh(char c) {
		outputRemoveSpace();
		if (*_ptr == c) {
			_ptr++;
			return true;
		}
		return false;
	}
	char LString::nextChar(){
		char rev = *_ptr;
		_ptr++;
		if (_ptr >= _end) {
			_ptr= _end-1;
			return 0;
		}
		else{
			return rev;
		}
	}	
	void LString::outputRemoveSpace() {
		while (_ptr != End) {
			if ((*_ptr == ' ') || (*_ptr == '\r') ||
				(*_ptr == '\t') || (*_ptr == '\n')) {
				_ptr++;
			}
			else {
				return;
			}
		}
	}

};