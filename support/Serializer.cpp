
#include <stdio.h>

#include "Serializer.h"
using namespace std;
namespace srb {


#define writeToBuf(format, ...) do{\
	counter+= snprintf(buf+ counter, buf_size - counter, format,__VA_ARGS__);\
	if(counter<0)return counter;\
	}while(0);

	int Serializer<int, char>::get(char*buf, int buf_size)	{
		int counter = 0;
		writeToBuf("\"%s\":", name);
		switch(len) {
		case 0:
			writeToBuf("null");
			break;
		case 1:
			writeToBuf("%d",value_prt[0]);
			break;
		default:
			int i = 0;
			writeToBuf("[%d", value_prt[i++]);
			while(i < len) {
				writeToBuf(",%d", value_prt[i++]);
			}
			writeToBuf("]");
			break;
		}
		return counter;
	}	

#define writeObject(object) do{\
	counter += object->get(buf+ counter, buf_size - counter);\
	if(counter<0)return counter;\
	}while(0);

	int Serializer<iSerializer<char>*, char>::get(char*buf, int buf_size) {
		int counter = 0;
		writeToBuf("\"%s\":", name);
		switch (len) {
		case 0:
			writeToBuf("null");
			break;
		default:
			int i = 0;
			writeToBuf("{");
			writeObject(value_prt[i++]);
			while (i < len) {
				writeToBuf(",");
				writeObject(value_prt[i++]);
			}
			writeToBuf("}");
			break;
		}
		return counter;
	}

	int Serializer<int, char>::set(const char * buf)
	{
		return 0;
	}	
	int Serializer<iSerializer<char>*, char>::set(const char * buf)
	{
		return 0;
	}


};