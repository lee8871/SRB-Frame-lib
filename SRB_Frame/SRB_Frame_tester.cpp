// SRB_Frame.cpp: 定义应用程序的入口点。
//
#include "SRB_Frame_tester.h"
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
__suseconds_t check(const char *dscrp, int report_mini_value);
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
int check(const char *dscrp, int report_mini_value = 0)
{
	QueryPerformanceCounter(&end_time);
	cout << dscrp << (end_time.QuadPart - bgn_time.QuadPart)*1000.0/(cpu_freq.QuadPart) << endl;
	QueryPerformanceCounter(&bgn_time);
	retunr 0;

}
#endif
#if UNIX
struct timeval bgn_time;
struct timeval end_time;

 #include <sched.h>
 #include <unistd.h>
void setPriority(){      
	struct sched_param param; 
	int maxpri; 
	maxpri = sched_get_priority_max(SCHED_FIFO);
    param.sched_priority = maxpri; 
	if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) {
		cout << "priority set fail" << endl;
	}
	else{
		cout << "priority set done" << endl;
	}	
}
void beginCheck()
{
	while(0 != gettimeofday(&bgn_time,null));
}
__suseconds_t check(const char *dscrp, int report_mini_value=0)
{
	while(0 != gettimeofday(&end_time,null));
	__suseconds_t diff = end_time.tv_usec - bgn_time.tv_usec;
	if(diff<0){
		diff+=1000000;
	}
	if(diff>report_mini_value){
		cout << dscrp << diff << endl;
	}
	while(0 != gettimeofday(&bgn_time,null));
	return diff;
}
#endif

UsbToSrb bus;
const int TEST_PKG_NUM = 1000000;


int main(int argc,char *argv[]){
	setPriority();
	BaseNode node;
	node.addr = 10;
	int16 speed = 0;
	int temp;
	cout << "try Open Port:" << bus.openUsbByName("USB-TEST-BED")<< endl;
	time_t begin_time;	time(&begin_time);
	cout << "Test send begin at "<<timeToString(begin_time)<<endl;
	cout << TEST_PKG_NUM << " accessing is doing. "<< endl;
	UsbAccess *access;
	beginCheck();
	long long int totle_send_time_us = 0;
	double last;
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
		check("calculate:",300);
		bus.addAccess(access);
		bus.doAccess();
		totle_send_time_us += check("send time >300 :",300);
		delete access;
	}
	time_t end_time;	time(&end_time);
	cout << "Test send end at " << timeToString(end_time) << endl;
	cout << "It cost " <<end_time-begin_time<<"(s) at all."<< endl;
	cout << "accessing time avariage is " <<totle_send_time_us/(TEST_PKG_NUM)<<"(us)."<< endl;


#ifdef WINDOW_86
	system("PAUSE");
#endif
	return 0;
}
