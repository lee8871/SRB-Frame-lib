#include <string>
#include <stdio.h>

#include <memory>
#include "json.h"

using namespace std;
using namespace srb;
using namespace atjson;
char bank[4096];	



int a = 20;
int b = -42;
int c[] = { 1,2,34,567,-89,0 };
int d[] = { 99,66 };



json listtest{ c,c + 1,c + 2,c + 3,c + 4,c + 5 };

json listtest2{ {"a", &a},{"b",&b} };

json listtest3{ {"a", &a},{"b",&b},{"c",{c,c + 1,c + 2,c + 3,c + 4,c + 5}},{"d",{{"d",d},{"d2",d + 1}}} };
int main(int argc, char *argv[]) {
	listtest.getJson(bank, 4096);	printf("%s\n", bank);
	listtest2.getJson(bank, 4096);	printf("%s\n", bank);
	listtest3.getJson(bank, 4096);	printf("%s\n", bank);
	while (1)	{

	}
}