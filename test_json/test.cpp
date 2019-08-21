#include <cstdio>
#include <memory>
#include "cLogger.h"




#include "json.h"
#include "OsSupport.h"


using namespace lee8871_support;
#include "../readonly/SRB-app-dumotor-share.h"

using namespace std;
using namespace srb;




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
	{"name",buildJsonConstStr("csMotorSet") },
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
class writeIntAsStr : public JsonTransformer {
public:
	int get(JsonGenerateString* str, void* value)override {
		return str->print("\"%d\"", *(int*) value);
	}
	int set(JsonParseString* str, void * value)override {
		return fail;
	};
};
writeIntAsStr common_intAsStr;
json specialTest{
	{"v0",{&common_intAsStr,_ia6 + 0}},
	{"v1",_ia6 + 1},
	{"array",{_ia6,_ia6 + 1,{&common_intAsStr,_ia6 + 2}, _ia6 + 3}},
	{"object",{
		{"v4",_ia6 + 4},
		{"v5",{&common_intAsStr,_ia6 + 5}}
	}}
};
#undef _ia6



json specialTest2{
	{"a", &a},
	{"b",&b},
	{"special-c",{&difftest, c}},
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



int Step = 0;
#include "Windows.h"
#define PRINGSTEP(descripte) do{\
setTerminalColor(eTerminalColor::highlight);\
printf("\n\nStep %d  Enter to -- %s", Step++, descripte );\
setTerminalColor(eTerminalColor::normal);\
while('\n'!=getchar());\
}while(0)



int testRead(json* j_object, char* ptr, int len,void* diff = 0) {
	auto serialze_jstr = std::make_unique<JsonGenerateString>(1024);
	serialze_jstr->isExpanded = false;
	j_object->get(serialze_jstr.get(), diff);
	printf("Before read -- %s\n", serialze_jstr->Buf);serialze_jstr->clear();
	auto read_jstr = std::make_unique<JsonParseString>(ptr, len);
	printf("Read string -- %s\n", read_jstr->Buf);
	int rev_set = j_object->set(read_jstr.get(), diff);
	j_object->get(serialze_jstr.get(), diff);
	printf("After read  -- %s\n", serialze_jstr->Buf);serialze_jstr->clear();

	if (nullptr != read_jstr->Error_str) {
		if (rev_set == done) {
			setTerminalColor(eTerminalColor::warning);
		}
		else {
			setTerminalColor(eTerminalColor::error);
		}
		printf("%s\n", read_jstr->Error_str->Buf);
		setTerminalColor(eTerminalColor::normal);
	}
	printf("\n\n");
	return done;
}



int main(int argc, char *argv[]) {
	char num[] = "128";
	auto write_jstr = new JsonGenerateString(1024);
	auto read_jstr = new JsonParseString(num, sizeof(num));	
	PRINGSTEP("Now test for struct<csMotorSet>:"); {
		struct csMotorSet motor_set_values = { 222,124,1600,100,128 };
#define _cs_ms(value) {#value,(&(((csMotorSet*)0)->value))}
		json testForStruct{
			_cs_ms(min_pwm_a),
			_cs_ms(min_pwm_b),
			_cs_ms(period),
			_cs_ms(lose_control_ms),
			_cs_ms(lose_behavior)
		};
#undef _cs_ms
		{
			char json_string[] = R"({"min_pwm_a":144,"min_pwm_b":156,"period":252,"lose_control_ms":25,"lose_behavior":55})";
			testRead(&testForStruct, json_string, sizeof(json_string), &motor_set_values);
		} {
			char json_string[] = R"({"min_pwm_a":90036,"min_pwm_b":9036,"period":-254,"lose_control_ms":455,"lose_behavior":22} )";
			testRead(&testForStruct, json_string, sizeof(json_string), &motor_set_values);
		}

	}
	
	
	PRINGSTEP("test for read objec: "); {
		char str[20] = "test for str";
		int  i = 12;
		json json_object =
		{ {"num",&i },{"name", buildJsonStr(str, 20) }, {"fix",buildJsonConstStr("fix string")} };
		char json_string[] = R"(     {"num":1,"name":"new string"}  )";
		testRead(&json_object, json_string, sizeof(json_string));
		char json_string2[] = R"({"name":"next string","num":2345} )";
		testRead(&json_object, json_string2, sizeof(json_string2));
		char json_string3[] = R"(     {"num":128,"name":"error test",   
									"fix":"fix can not write", "lalala": 12  	}  )";
		testRead(&json_object, json_string3, sizeof(json_string3));
	}


	PRINGSTEP("Test for json String and int "); {
		char str[20] = "old string";
		int  i = 12;
		json json_object = { &i ,buildJsonStr(str,20),buildJsonConstStr("fix string") };
		char json_string[] = R"([1,"new string",  "try overrid fix" ]   )";
		testRead(&json_object, json_string, sizeof(json_string));
		char json_string2[] = R"([1,"new long string so so so so so so so so so so so so so long",  "try overrid fix" ]   )";
		testRead(&json_object, json_string2, sizeof(json_string2));
	}



	PRINGSTEP("Test for json read"); {
		char num_array[] = R"(
				[500 ,11,32,
				12,2,4  ])";
		testRead(&listtest, num_array, sizeof(num_array));
	}{
		char num_array[] = R"([1 ,2,3,12346  ])";
		testRead(&listtest, num_array, sizeof(num_array));
	}{

		char num_array[] = R"([1 ,2,"8764","nihao",12346  ])";
		testRead(&listtest, num_array, sizeof(num_array));
	}{
		char num_array[] = R"([12382387487547598,2.45,8764,"nihao",12346  ])";
		testRead(&listtest, num_array, sizeof(num_array));
	}{
		char num_array[] = R"([12382387487547598,2.45,8764,"nihao",12346  ])";
		testRead(&listtest, num_array, sizeof(num_array));
	}
	






	PRINGSTEP("Test for string to number"); {
		printf("\nFor Int Value\n");
		int rev;
		int value;
		char number[32] = "-12937  323";
		JsonParseString numstr{ number,32 };
		rev = numstr.parseNumber(&value);	printf("get num1_1   %d, rev = %d\n", value, rev);
		rev = numstr.parseNumber(&value);	printf("get num1_2   %d, rev = %d\n", value, rev);

		char number2[32] = "29929479876133474519";
		JsonParseString numstr2{ number2,32 };
		rev = numstr2.parseNumber(&value);	printf("get num2_1   %d, rev = %d\n", value, rev);
		rev = numstr2.parseNumber(&value);	printf("get num2_2   %d, rev = %d\n", value, rev);

		char number3[64] = "292e2 -1254  -223.6 1.646e1293487";
		JsonParseString numstr3{ number3,64 };
		rev = numstr3.parseNumber(&value);	printf("get num3_1   %d, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_2   %d, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_3   %d, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_4   %d, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_5   %d, rev = %d\n", value, rev);
	} {
		printf("\nFor Unsigned Int Value\n");
		int rev;
		unsigned int value;
		char number[32] = "-12937  323";
		JsonParseString numstr{ number,32 };
		rev = numstr.parseNumber(&value);	printf("get num1_1   %u, rev = %d\n", value, rev);
		rev = numstr.parseNumber(&value);	printf("get num1_2   %u, rev = %d\n", value, rev);

		char number2[32] = "29929479876133474519";
		JsonParseString numstr2{ number2,32 };
		rev = numstr2.parseNumber(&value);	printf("get num2_1   %u, rev = %d\n", value, rev);
		rev = numstr2.parseNumber(&value);	printf("get num2_2   %u, rev = %d\n", value, rev);

		char number3[64] = "292e2 -1254  -223.6 1.646e1293487";
		JsonParseString numstr3{ number3,64 };
		rev = numstr3.parseNumber(&value);	printf("get num3_1   %u, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_2   %u, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_3   %u, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_4   %u, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_5   %u, rev = %d\n", value, rev);
	} {
		printf("\nFor Double Value\n");
		int rev;
		double value;
		char number[32] = "-12937  323";
		JsonParseString numstr{ number,32 };
		rev = numstr.parseNumber(&value);	printf("get num1_1   %lf, rev = %d\n", value, rev);
		rev = numstr.parseNumber(&value);	printf("get num1_2   %lf, rev = %d\n", value, rev);

		char number2[32] = "29929479876133474519 didi";
		JsonParseString numstr2{ number2,32 };
		rev = numstr2.parseNumber(&value);	printf("get num2_1   %lf, rev = %d\n", value, rev);
		rev = numstr2.parseNumber(&value);	printf("get num2_2   %lf, rev = %d\n", value, rev);

		char number3[64] = "292e2 -1254  -223.6 1.646e1293487";
		JsonParseString numstr3{ number3,64 };
		rev = numstr3.parseNumber(&value);	printf("get num3_1   %lf, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_2   %lf, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_3   %lf, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_4   %lf, rev = %d\n", value, rev);
		rev = numstr3.parseNumber(&value);	printf("get num3_5   %lf, rev = %d\n", value, rev);
	}



	PRINGSTEP("Now test for serialization:");	
	JsonGenerateString str{ 4096 };
	str.isExpanded = false;
	listtest.get(&str);	    printf("%s\n", str.Buf);str.clear();
	str.isExpanded = true;
	listtest2.get(&str);	printf("%s\n", str.Buf);str.clear();
	listtest3.get(&str);	printf("%s\n", str.Buf);str.clear();
	str.isExpanded = false;
	listtest4.get(&str);	printf("%s\n", str.Buf);str.clear();


	PRINGSTEP("Now test for 1 json transform many Objects:");
	difftest.get(&str, c1);	printf("%s\n", str.Buf);str.clear();
	difftest.get(&str, c2);	printf("%s\n", str.Buf);str.clear();
	difftest.get(&str, c3);	printf("%s\n", str.Buf);str.clear();


	PRINGSTEP("Now test for special output:\n");
	specialTest.get(&str, c1);	printf("%s\n", str.Buf);str.clear();
	specialTest.get(&str, c2);	printf("%s\n", str.Buf);str.clear();
	specialTest2.get(&str); 	printf("%s\n", str.Buf);str.clear();



	PRINGSTEP("Now test a long long serialization:");
	listtest5.get(&str);	printf("%s\n", str.Buf);str.clear();

	PRINGSTEP("print size for types");
#define print_size(t) printf("Size of %s is %d.\n",#t,sizeof(t))
	print_size(writeIntAsStr);
	print_size(json);


	print_size(uint8);
	print_size(uint16);
	print_size(uint32);
	print_size(int8);
	print_size(int16);
	print_size(int32);
	print_size(int);
	print_size(long int);
	print_size(long long int);
	print_size(long double);
	print_size(double);
	print_size(size_t);
	print_size(bool);
#undef print_size


	PRINGSTEP("EXIT TEST");
	getchar();
}


