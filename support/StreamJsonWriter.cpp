﻿#include "StreamJsonWriter.h"
#include <iostream>   
#include "cLogger.h"
using namespace std;
using namespace srb;
namespace lee8871_support {
	namespace trans {
		extern char u8ToLow(uint8 u8);
	}

	lee8871_support::ModuleLog stream_json_writer_log = { "stream_json",eLogLevel::info ,__DATE__,__TIME__ };

	StreamJsonWriter::StreamJsonWriter(std::ostream * record_ostream) {
		setStream(record_ostream);
		iteration = -1;
	}

	int StreamJsonWriter::setStream(std::ostream * record_ostream)	{
		record = record_ostream;
		return done;
	}


	int StreamJsonWriter::preWrite(const char * name)
	{
		if (iteration != -1) {//if not root object
			if (is_first[iteration] == false) {//may need comma if not first object
				*record << ',';
			}
			else {
				is_first[iteration] = false;
			}
			if ((is_next_obj_in_new_line )&&(!is_expand_mode)){
				*record << '\n';
				is_next_obj_in_new_line = false;
			}
			if (name[0] != '\0') {//may write name(in Object) or not(in array)
				if (is_expand_mode) {
					*record << '\n';
					for (int i = 0;i <= iteration;i++) {
						*record << '\t';
					}
				}
				*record << '"' << name << '"' << ':';
			}
		}
		return done;
	}

	int StreamJsonWriter::writeNum(const char * name, int value)	{
		preWrite(name);
		*record << value;
		return done;
	}	
	int StreamJsonWriter::writeLongLongNum(const char * name, long long value) {
		preWrite(name);
		*record << value;
		return done;
	}


	int StreamJsonWriter::writeU8Str(const char * name, uint8 value)	{
		preWrite(name);
		*record << '"' << trans::u8ToLow(value >> 4) << trans::u8ToLow(value) << '"';
		return done;
	}

	int StreamJsonWriter::writeStr(const char * name, const char * value) {
		preWrite(name);
		*record << '"' << value << '"';
		return done;

	}

	int StreamJsonWriter::writeNull(const char * name)	{
		preWrite(name);
		*record << "nullptr" ;
		return done;
	}

	int StreamJsonWriter::writeBool(const char * name, bool value)	{
		preWrite(name);
		if (value) {
			*record << "true";
		}
		else {
			*record << "false";
		}
		return done;
	}

	int StreamJsonWriter::writeNumArray(const char * name, const int * values, int len){
		preWrite(name);
		*record << '[';
		int i = 0;
		if (len > 0) {
			*record << values[i++];
			while(i < len) {
				*record <<','<< values[i++];			
			}
		}
		*record << ']';
		return done;
	}
	int StreamJsonWriter::writeNumArray(const char * name, const uint8 * values, int len){
		preWrite(name);
		*record << '[';
		int i = 0;
		if (len > 0) {
			*record << ((int)values[i++]);
			while(i < len) {
				*record <<','<< ((int)values[i++]);			
			}
		}
		*record << ']';
		return done;
	}

	int StreamJsonWriter::writeU8ArrayStr(const char * name, const uint8 * data, int len )	{
		preWrite(name);			
		*record << '"';
		for (int i = 0;i < len; i++) {
			*record << trans::u8ToLow(data[i] >> 4) << trans::u8ToLow(data[i]) << ' ';
		}
		*record << '"';
		return done;
	}

	int StreamJsonWriter::writeEndLine()	{
		is_next_obj_in_new_line = true;
		return done;
	}

	int StreamJsonWriter::beginObj(const char * name)	{
		preWrite(name);
		iteration++;
		if (iteration >= MAX_ITERATION_NUM) {
			stream_json_writer_log.addLog(eLogLevel::erro, "Json maker enter too much object.");
			return fail;
		}
		is_first[iteration] = true;
		*record << '{';
		return done;
	}

	int StreamJsonWriter::endObj() {
		iteration--;				
		if (is_expand_mode) {
			*record << '\n';
			for (int i = 0;i <= iteration;i++) {
				*record << '\t';
			}
		}
		*record << '}';
		if (iteration == -1){//force new line and flush buffer
			*record  << endl;
		}
		else if (iteration < -1) {
			stream_json_writer_log.addLog(eLogLevel::erro, "Json maker endding more object than begining");
			return fail;
		}
		return done;
	}
	







};