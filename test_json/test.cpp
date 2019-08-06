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
	{"e",{}}
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
	{"e",{}}
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

int writeIntAsStr(JsonString* str, void* value_prt, bool is_get) {
	return str->printf("\"%d\"", *((int*)value_prt));
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
int writeInt6(JsonString* str, void* value_prt, bool is_get) {
	return difftest.get(str, value_prt);
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
	{"e",{}}
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

int main(int argc, char *argv[]) {

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





	printf("\n\nNow test for serialization:\n");
	listtest.get(&str);	printf("%s\n", bank);str.reset();
	listtest2.get(&str);	printf("%s\n", bank);str.reset();
	listtest3.get(&str);	printf("%s\n", bank);str.reset();
	listtest4.get(&str);	printf("%s\n", bank);str.reset();

	printf("\n\nNow test for 1 json transform many Objects:\n");
	difftest.get(&str, c1);	printf("%s\n", bank);str.reset();
	difftest.get(&str, c2);	printf("%s\n", bank);str.reset();
	difftest.get(&str, c3);	printf("%s\n", bank);str.reset();

	printf("\n\nNow test for special output:\n");
	specialTest.get(&str, c1);	printf("%s\n", bank);str.reset();
	specialTest.get(&str, c2);	printf("%s\n", bank);str.reset();
	specialTest2.get(&str);	printf("%s\n", bank);str.reset();

	printf("\n\nNow test for struct<csMotorSet>:\n");
	testForStruct.get(&str, &motor_set_values);	printf("%s\n", bank);str.reset();
	motor_set_values.min_pwm_a = 1229;
	motor_set_values.min_pwm_b = 3242;
	motor_set_values.period = 20;
	motor_set_values.lose_control_ms = 20;
	motor_set_values.lose_behavior = 1;
	testForStruct.get(&str, &motor_set_values);	printf("%s\n", bank);str.reset();

	while (1) {



	}
}