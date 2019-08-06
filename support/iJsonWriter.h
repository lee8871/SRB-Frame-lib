#pragma once
#include "lee.h"

using namespace lee8871_support;
namespace srb {
	class iJsonWriter {
	public :
		virtual int writeNum(const char* name, int value) = 0;
		virtual int writeLongLongNum(const char * name, long long value) = 0;
		virtual int writeU8Str(const char* name, uint8 value) = 0;
		virtual int writeU8ArrayStr(const char * name, const uint8 * data, int len) = 0;
		virtual int writeStr(const char * name, const char * value) = 0;
		virtual int writeNull(const char * name) = 0;
		virtual int writeBool(const char * name,bool value) = 0;
		virtual int writeNumArray(const char * name, const int * value, int len) = 0;
		virtual int writeNumArray(const char * name, const uint8 * value, int len) = 0;
		
		virtual int writeEndLine() = 0;
		virtual int beginObj(const char* name ) = 0;
		virtual int endObj() = 0;
		
		bool is_expand_mode=false;

	};
};