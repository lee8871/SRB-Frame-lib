#pragma once
#include "lee.h"
#include "iJsonWriter.h"
#include <iostream> 
namespace srb {
	class StreamJsonWriter: public iJsonWriter {
	private:
		const static int MAX_ITERATION_NUM = 16;
		int iteration;
		bool is_first[MAX_ITERATION_NUM] = { true };
		bool is_next_obj_in_new_line = false;
		std::ostream* record;


		int preWrite(const char * name);



	public:
		StreamJsonWriter(std::ostream* record_ostream);
		int writeNum(const char* name, int value) override;
		int writeLongLongNum(const char * name, long long value)override;
		int writeU8Str(const char* name, uint8 value) override;
		int writeU8ArrayStr(const char * name, const uint8 * data, int len)	 override;
		int writeStr(const char * name, const char * value)override;
		int writeNull(const char * name)override;
		int writeBool(const char * name, bool value)override;

		int writeNumArray(const char* name, const int* value, int len) override;
		int writeNumArray(const char * name, const uint8 * value, int len)override;


		
		int writeEndLine() override;
		int beginObj(const char* name) override;
		int endObj() override;
	};
};