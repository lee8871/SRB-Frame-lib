// SRB_Frame.cpp: 定义应用程序的入口点。
//
#include "SRB-Frame.h"
#include "UsbToSrb.h"
#include <time.h>

#include <string>
using namespace std;
using namespace srb;
BaseNode bn;
string timeToString(time_t tim)
{
	char str[64];
	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", localtime(&tim));
	return str;
}
void check(const char *dscrp, int report_mini_value);
void beginCheck();
#ifdef WINDOW_86
LARGE_INTEGER bgn_time;
LARGE_INTEGER end_time;
LARGE_INTEGER cpu_freq;

void beginCheck()
{
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&bgn_time);
}
void check(const char *dscrp, int report_mini_value = 0)
{
	QueryPerformanceCounter(&end_time);
	cout << dscrp << (end_time.QuadPart - bgn_time.QuadPart)*1000.0/(cpu_freq.QuadPart) << endl;
	QueryPerformanceCounter(&bgn_time);

}
#endif
#if UNIX
struct timeval bgn_time;
struct timeval end_time;

void beginCheck()
{
	gettimeofday(&bgn_time,null);
}
void check(const char *dscrp, int report_mini_value=0)
{
	__suseconds_t diff = end_time.tv_usec - bgn_time.tv_usec;
	gettimeofday(&end_time,null);
	if(diff>report_mini_value){
		cout << dscrp << end_time.tv_usec - bgn_time.tv_usec << endl;
	}
	gettimeofday(&bgn_time,null);
}
#endif

UsbToSrb bus;
const int TEST_PKG_NUM = 2000*100;


int main()
{
	BaseNode node;
	node.addr = 10;
	int16 speed = 0;
	int temp;
	cout << "try Open Port:" << bus.openUsbByName("USB-TEST-BED")<< endl;
	time_t begin_time;	time(&begin_time);





	cout << "Test send begin at "<<timeToString(begin_time)<<endl;
	cout << TEST_PKG_NUM << " access is doing. "<< endl;
	UsbAccess *access;
	beginCheck();
	for (int i = 0;i < TEST_PKG_NUM;i++) {
		access = new UsbAccess(&node);
		sSrbPkg* send_pkg = access->getSendPkg(); {
			int data_p = 0;
			send_pkg->data[data_p++] = (uint8)speed;
			send_pkg->data[data_p++] = (uint8)(speed>>8);
			send_pkg->data[data_p++] = (uint8)speed;
			send_pkg->data[data_p++] = (uint8)(speed>>8);
			send_pkg->bfc.length = data_p;
			send_pkg->bfc.port = SC_PORT_D0;
		}
		speed++;
		if (speed >= 0x1000) {
			speed = 0;
		}
		check("\t\t\t\tcalculte time: ",-1);

		bus.addAccess(access);
		bus.doAccess();
		check("send time: ",-1);



		delete access;
	}

	time_t end_time;	time(&end_time);
	cout << "Test send end at " << timeToString(end_time) << endl;
	cout << "Cost " <<(end_time-begin_time)<<" S"<< endl;
	cout << "Cost " <<(end_time-begin_time)*1000.0 /(TEST_PKG_NUM)<<" mS / Access"<< endl;


	system("PAUSE");
	return 0;
}
