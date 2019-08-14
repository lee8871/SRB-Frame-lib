#pragma once
#include "lee.h"
#include "LString.h"


namespace lee8871_support {

	class JsonGenerateString : public LString {
	protected:
		int tab_level = 0;
		void addTab();

	public:
		bool isExpanded = true;
		char* tab_string = "  ";
		JsonGenerateString(char *buf, int size);
		JsonGenerateString(int size);
		~JsonGenerateString();
		inline void reset() {
			LString::reset();
			tab_level = 0;	
		}


		void addBgn();
		void addGap();
		void addEnd();
		void arrayBgn();
		void arrayGap();
		void arrayEnd();

		void inputBool(bool value);

		void inputString(const char * string);

		void inputNumber(int value);
		void inputNumber(unsigned int value);
		void inputNumber(double value);
		void inputNumber(float value);

	};


};