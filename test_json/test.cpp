#include <string>
#include <stdio.h>
#include <memory>
#include "json.h"

using namespace lee8871_support;
#include "../readonly/SRB-app-dumotor-share.h"



using namespace std;
char bank[4096];	



int a = 20;
int b = -42;
int c[] = { 1,2,34,567,-89,0 };
int d[] = { 99,66 };

int c1[] = { 1001,1002,10034,100567,-10089,1000 };
int c2[] = { 2001,2002,20034,200567,-20089,2000 };
int c3[] = { 3001,3002,30034,300567,-30089,3000 };

int8 i8value = 19;

json listtest{ c,c + 1,c + 2,c + 3,c + 4,c + 5 };

json listtest1{ {"c",listtest} };//the initializer_list copy listtest, than build listtest1 

json listtest2{
	{"name\t hello",jsonString("csMotorSet \t\n\t\b\f\r\\\'\'\'\"\"\"/// hello") },
	{"I8test", &i8value},
	{"b",&b},
	{"c",listtest}
};

json listtest3{
	{"a", &a},
	{"b",&b},
	{"c",{c,c + 1,c + 2,c + 3,c + 4,c + 5}},
	{"d",{
		{"d",d},
		{"d2",d + 1}
	}},
};

json listtest4{ 
	{"a", &a},
	{"b",&b},
	{"c",{c,c + 1,c + 2,c + 3,c + 4,c + 5}},
	{"d",{
		{"d",d},
		{"d2",d + 1},
		{"d3",{
			{"d31",c},
			{"d32",c + 1},
			{"d33",c + 2},
			{"d34",d + 3}
		}},
	}},
};
json listtest5{
	{"lt1",listtest1},
	{"lt2",listtest2},
	{"lt3",listtest3},
	{"ltag4",{
		{"d",d},
		{"d2",d + 1},
		{"d3",{
			{"lt4",listtest4},
			{"d31",c},
			{"d32",c + 1},
			{"d33",c + 2},
			{"d34",d + 3}
		}},
	}},
};

#define _ia6 ((int*)0)
json difftest{ 
	{"v0",_ia6+0},
	{"v1",_ia6+1},
	{"array",{_ia6,_ia6 + 1,_ia6 + 2,_ia6 + 3}},
	{"object",{
		{"v4",_ia6 + 4},
		{"v5",_ia6 + 5}
	}}
};

int writeIntAsStr(transformCBArgumenrt) {
	return str->printf("\"%d\"", valuePtr(int));
}
json specialTest{
	{"v0",{writeIntAsStr,_ia6 + 0}},
	{"v1",_ia6 + 1},
	{"array",{_ia6,_ia6 + 1,{writeIntAsStr,_ia6 + 2}, _ia6 + 3}},
	{"object",{
		{"v4",_ia6 + 4},
		{"v5",{writeIntAsStr,_ia6 + 5}}
	}}
};

#undef _ia6
int writeInt6(transformCBArgumenrt) {
	return difftest.get(str, valuePtr(void));
}

json specialTest2{
	{"a", &a},
	{"b",&b},
	{"special-c",{writeInt6, c}},
	{"d",{
		{"d",d},
		{"d2",d + 1},
		{"d3",{
			{"d31",c},
			{"d32",c + 1},
			{"d33",c + 2},
			{"d34",d + 3}
		}},
	}},
};
using namespace srb::Du_Motor;
struct csMotorSet motor_set_values={222,124,1600,100,128};


#define _cs_ms(value) {#value,(&(((csMotorSet*)0)->value))}
json testForStruct{
	_cs_ms(min_pwm_a),
	_cs_ms(min_pwm_b),
	_cs_ms(period),
	_cs_ms(lose_control_ms),
	_cs_ms(lose_behavior)
};
#undef _cs_ms
int Step = 0;
#define PRINGSTEP(descripte)  printf("Step %d  Enter to\n%s", Step++, descripte );while('\n'!=getchar());
int main(int argc, char *argv[]) {
	int rev;
	int value;
	PRINGSTEP("Test for string to number");

	char number[32] = "-12937  323";
	JsonString numstr{ number,32 };

	rev = numstr.outputNumber(&value);
	printf("get num1_1   %d, rev = %d\n", value, rev);

	rev = numstr.outputNumber(&value);
	printf("get num1_2   %d, rev = %d\n", value, rev);

	char number2[32] = "29929479876133474519";
	JsonString numstr2{ number2,32 };
	rev = numstr2.outputNumber(&value);
	printf("get num2_1   %d, rev = %d\n", value, rev);

	rev = numstr2.outputNumber(&value);
	printf("get num2_2   %d, rev = %d\n", value, rev);

	char number3[64] = "292 -1254  -223.6 1.646e1293487";
	JsonString numstr3{ number3,64};
	rev = numstr3.outputNumber(&value);
	printf("get num3_1   %d, rev = %d\n", value, rev);
	rev = numstr3.outputNumber(&value);
	printf("get num3_2   %d, rev = %d\n", value, rev);
	rev = numstr3.outputNumber(&value);
	printf("get num3_3   %d, rev = %d\n", value, rev);
	rev = numstr3.outputNumber(&value);
	printf("get num3_4   %d, rev = %d\n", value, rev);
	rev = numstr3.outputNumber(&value);
	printf("get num3_5   %d, rev = %d\n", value, rev);





	PRINGSTEP("print size for types");
	JsonString str{ bank,4096 };
#define print_size(t) printf("Size of %s is %d.\n",#t,sizeof(t))
	print_size(uint8);
	print_size(uint16);
	print_size(uint32);	
	print_size(int8);
	print_size(int16);
	print_size(int32);
	print_size(int);
	print_size(long int);
	print_size(long long int);
	print_size(size_t);
#undef print_size


	PRINGSTEP("Now test for serialization:");
	listtest.get(&str);	    printf("%s\n", bank);str.reset();
	listtest2.get(&str);	printf("%s\n", bank);str.reset();
	listtest3.get(&str);	printf("%s\n", bank);str.reset();
	listtest4.get(&str);	printf("%s\n", bank);str.reset();


	PRINGSTEP("Now test for 1 json transform many Objects:");
	difftest.get(&str, c1);	printf("%s\n", bank);str.reset();
	difftest.get(&str, c2);	printf("%s\n", bank);str.reset();
	difftest.get(&str, c3);	printf("%s\n", bank);str.reset();


	PRINGSTEP("Now test for special output:\n");
	specialTest.get(&str, c1);	printf("%s\n", bank);str.reset();
	specialTest.get(&str, c2);	printf("%s\n", bank);str.reset();
	specialTest2.get(&str); 	printf("%s\n", bank);str.reset();


	PRINGSTEP("Now test for struct<csMotorSet>:");
	testForStruct.get(&str, &motor_set_values);	printf("%s\n", bank);str.reset();
	motor_set_values.min_pwm_a = 1229;
	motor_set_values.min_pwm_b = 3242;
	motor_set_values.period = 20;
	motor_set_values.lose_control_ms = 20;
	motor_set_values.lose_behavior = 1;
	testForStruct.get(&str, &motor_set_values);	printf("%s\n", bank);str.reset();


	PRINGSTEP("Now test a long long serialization:");
	listtest5.get(&str);	printf("%s\n", bank);str.reset();


	PRINGSTEP("press any key to EXIT");
	getchar();
}