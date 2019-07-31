#include <string.h>
#include <stdio.h>

#include <memory>
#include "Serializer.h"

using namespace std;
using namespace srb;
char bank[4096];

int main(int argc, char *argv[]) {

	int a = 20;
	int b = -42;
	int c[] = { 1,2,34,567,-89,0 };
	Serializer<int, char> a_jo("a", &a);
	Serializer<int, char> b_jo("b", &b);
	Serializer<int, char> c_jo("c", c, 6);
	iSerializer<char>* obj[] = { &a_jo, &b_jo, &c_jo };
	Serializer<iSerializer<char>*, char> obj_jo("test0", obj, 3);
	iSerializer<char>* o1 = obj_jo.get();
	auto top_jo(std::make_unique<Serializer<iSerializer<char>*, char>>("top", &o1, 3));
	
	top_jo->get(bank, 4096);
	printf("%s", bank);
	while (1)	{

	}
}