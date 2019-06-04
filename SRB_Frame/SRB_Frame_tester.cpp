#include "SRB_Frame_tester.h"
#include "UsbToSrb.h"
#include "Master.h"


#include <time.h>

#include <string>
#define __MASTER__
#define __MAPPING_DECLEAR__
#include "SRB-app-dumotor-share.h"
#undef __MASTER__
#undef __MAPPING_DECLEAR__

using namespace std;
using namespace srb::usb_bus;
using namespace srb;
string timeToString(time_t tim)
{
	char str[64];
	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", localtime(&tim));
	return str;
}
void beginCheck();
#ifdef WINDOW_86
LARGE_INTEGER bgn_time;
LARGE_INTEGER end_time;
LARGE_INTEGER cpu_freq;
void setPriority() {
	return;
}
void beginCheck()
{
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&bgn_time);
}
int check(const char *dscrp, int report_mini_value = 0)
{
	QueryPerformanceCounter(&end_time);
	int diff = (end_time.QuadPart - bgn_time.QuadPart)*1000000.0 / (cpu_freq.QuadPart);
	cout << dscrp << diff << endl;
	QueryPerformanceCounter(&bgn_time);
	return diff;

}
#endif
#if UNIX
struct timeval bgn_time;
struct timeval end_time;

#include <sched.h>
#include <unistd.h>
void setPriority() {
	struct sched_param param;
	int maxpri;
	maxpri = sched_get_priority_max(SCHED_FIFO);
	param.sched_priority = maxpri;
	if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) {
		cout << "priority set fail" << endl;
	}
	else {
		cout << "priority set done" << endl;
	}
}
void beginCheck()
{
	while (0 != gettimeofday(&bgn_time, null));
}
__suseconds_t check(const char *dscrp, int report_mini_value = 0)
{
	while (0 != gettimeofday(&end_time, null));
	__suseconds_t diff = end_time.tv_usec - bgn_time.tv_usec;
	if (diff < 0) {
		diff += 1000000;
	}
	if (diff > report_mini_value) {
		cout << dscrp << diff << endl;
	}
	while (0 != gettimeofday(&bgn_time, null));
	return diff;
}
#endif

UsbToSrb* Main_bus;
const int TEST_PKG_NUM = 10000;
Master* Main_master;

int main(int argc, char *argv[]) {
	setPriority();
	int16 speed = 0;
	Main_bus = new UsbToSrb();
	Main_master = new Master(Main_bus);



	cout << "try Open Port:" << Main_bus->openUsbByName("USB-TEST-BED") << endl;
	time_t begin_time;	time(&begin_time);
	cout << "Test send begin at " << timeToString(begin_time) << endl;
	cout << TEST_PKG_NUM << " accessing is doing. " << endl;

	BaseNode* node = Main_master->getNode(10);
	node->setMapping(Du_Motor::mapping1, 1);

	beginCheck();
	long long int totle_send_time_us = 0;
	Du_Motor::sDataRs* motor_rs = (Du_Motor::sDataRs*)(node->rs_data);
	for (int i = 0;i < TEST_PKG_NUM;i++) {
		motor_rs->ma.brake = no;
		motor_rs->ma.speed++;
		if (motor_rs->ma.speed >= 1000) {
			motor_rs->ma.speed = 0;
		}

		motor_rs->mb.brake = no;
		motor_rs->mb.speed--;
		if (motor_rs->mb.speed <= -1000) {
			motor_rs->mb.speed = 0;
		}

		node->sendAccess(1);
		check("calculate:", 300);
		Main_bus->doAccess();
		totle_send_time_us += check("send time >300 :", 300);
	}
	time_t end_time;	time(&end_time);
	cout << "Test send end at " << timeToString(end_time) << endl;
	cout << "It cost " << end_time - begin_time << "(s) at all." << endl;
	cout << "accessing time avariage is " << totle_send_time_us / (TEST_PKG_NUM) << "(us)." << endl;
	delete Main_bus ;
	delete Main_master ;	
	
	return 0;
}
