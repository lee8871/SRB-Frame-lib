#include <string.h>
#include <stdio.h>

#include "stringHash.h"

using namespace std;
using namespace lee8871_support;
using namespace lee8871_support;
using namespace srb;
using namespace lee8871_support;
int main(int argc, char *argv[]) {
	char cmd[20];
	char argument_str[256];
	char c;
	while (1)	{
		cmd[0] = 0;
		argument_str[0] = 0;
		printf(">");
		scanf("%20s%[^\n]%c", cmd, argument_str, &c);
		switch (getHashCmd(cmd)) {
		case getHashCmd("hello"):
			printf("Hi, %s\n",argument_str);
			break;
		default:
			printf("input str is [%s],Id is %u\n", cmd, getHashCmd(cmd));
		}
	}
}