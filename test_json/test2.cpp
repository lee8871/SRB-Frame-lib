#include <cstdio>
#include <memory>
#include "cLogger.h"




#include "Json.h"


using namespace lee8871_support;
#include "../readonly/SRB-app-dumotor-share.h"

using namespace std;
using namespace srb;




int a = 20;
int b = -42;
int c[] = { 1,2,34,567,-89,0 };
int d[] = { 99,66 };
Json listtest3{
	{"a", &a},
	{"b",&b},
	{"c",{c,c + 1,c + 2,c + 3,c + 4,c + 5}},

	{"d",{
		{"d",d},
		{"d2",d + 1}
	}},
};
Json listtest5{
	{"lt3",listtest3},
	{"ltag4",{
		{"d",d},
		{"d2",d + 1},
	}},
};
int main(int argc, char *argv[]) {
	JsonGenerateString str{ 4096 };
	listtest3.get(&str);	printf("%s\n", str.Buf);str.clear();

}
