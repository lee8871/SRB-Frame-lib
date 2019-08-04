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

json json_array_0[]{ c,c + 1,c + 2,c + 3,c + 4,c + 5 };
json jsonobj_0[]{ {"a",&a},{"b",&b},{ "c",json_array_0,6,true} };
json jsonobj_1[]{ {"d",d},{"jsonobj_0",jsonobj_0,3}};
json top{ "top",jsonobj_1, sizeof(jsonobj_1)/sizeof(json) };


json jsonobj_2[]{ {"d",d},{"jsonobj_0",jsonobj_0,3,true} };
json top2{ "top",jsonobj_2, sizeof(jsonobj_2)/sizeof(json) };



json jsontest2_2{"top", new json[2]{
	{"d",d},
	{"jsontest2_2",new json[3]{
		&a,
		{"b",&b},
		{ "c",new json[6]{
			c,
			c + 1,
			c + 2,
			c + 3,
			c + 4,
			c + 5
		},6,true}
	}, 3}
}, 2 };

json listtest{ "list_test",{&a,{"b",&b}} };

int main(int argc, char *argv[]) {
	jsontest2_2.getJson(bank, 4096);	printf("%s\n", bank);
	listtest.getJson(bank, 4096);	printf("%s\n", bank);

	string ht = "hashtest";
	top.getJson(bank, 4096);	printf("%s\n", bank);
	top2.getJson(bank, 4096);	printf("%s\n", bank);
	c[3] = 2009;
	a = 223;
	top.getJson(bank, 4096);	printf("%s\n", bank);
	top.getJson(bank, 5);	printf("%s\n", bank);



	printf("test for print\n");
	void * value;
	value = &((json*)0)->hash;
	printf(" hash num address difference is %d\n", (size_t)value);
	printf(" Added address is %d\n", (size_t)((size_t)value + (size_t)(&top)));
	printf(" Reale address is %d\n", (size_t)(&(top.hash)));


	while (1)	{

	}
}