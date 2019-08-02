#include <string>
#include <stdio.h>

#include <memory>
#include "json.h"

using namespace std;
using namespace srb;
using json = atjson::json;
char bank[4096];	


int a = 20;
int b = -42;
int c[] = { 1,2,34,567,-89,0 };
json myconfig[]{ {"a",&a},{"b",&b},{ "c",c,6 } };
json top{ "top",myconfig,sizeof(myconfig)/sizeof(json) };

int main(int argc, char *argv[]) {
	string ht = "hashtest";
	getJson(bank, 4096, &top, 1);	printf("%s\n", bank);

	char temp[20];
	int rev = sscanf(bank, R"({"%[^"]s":)", temp);
	printf("%s--%d\n", temp, rev);

	c[3] = 2009;
	a = 223;
	getJson(bank, 4096, myconfig,3);	printf("%s\n", bank);

	while (1)	{
	}
}