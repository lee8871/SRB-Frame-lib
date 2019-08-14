#include <cstdlib>
#include <cmath>

#include "JsonGenerateString.h"
#define NOT_JSON_USER
#include "json.h"

namespace lee8871_support {

	void JsonGenerateString::inputString(const char * string)	{
		append('"');
		while (*string != 0){
			switch (*string) {
			case '"':
				append("\\\"");
				break;
			case '\\':
				append("\\\\");
				break;
			case '/':
				append("\\/");
				break;
			case '\b':
				append("\\\b");
				break;
			case '\f':
				append("\\\f");
				break;
			case '\n':
				append("\\\n");
				break;
			case '\r':
				append("\\\r");
				break;
			case '\t':
				append("\\\t");
				break;
				//TODO /u is not support now
			default:
				append(*string);
			}
			string++;
		}
		append('"');
	}

	void JsonGenerateString::inputNumber(int value) {
		print("%d", value);
	}
	void JsonGenerateString::inputNumber(unsigned int value) {
		print("%u", value);
	}
	void JsonGenerateString::inputNumber(double value) {
		print("%lf", value);
	}
	void JsonGenerateString::inputNumber(float value) {
		print("%f", value);
	}
	void JsonGenerateString::inputBool(bool value) {
		if (value) {
			append("true");
		}
		else {
			append("false");
		}
	}
	void JsonGenerateString::addBgn() {
		append('{');
		tab_level++;
		addTab();
	}
	void JsonGenerateString::addGap() {
		append(',');
		addTab();
	}
	void JsonGenerateString::addEnd() {
		tab_level--;
		addTab();
		append('}');
	}
	void JsonGenerateString::arrayBgn() {
		append('[');
	}
	void JsonGenerateString::arrayGap() {
		append(',');
	}
	void JsonGenerateString::arrayEnd() {
		append(']');
	}

	inline void JsonGenerateString::addTab() {
		if (isExpanded) {
			append('\n');
			for (int i = 0;i < tab_level;i++) {
				append(tab_string);
			}
		}
	}


	JsonGenerateString::JsonGenerateString(char *_buf, int _size)
		:LString(_buf,_size){
		tab_level = 0;
	}

	JsonGenerateString::JsonGenerateString(int size)
		: LString(size) {
		tab_level = 0;
	}

	JsonGenerateString::~JsonGenerateString(){

	}




};