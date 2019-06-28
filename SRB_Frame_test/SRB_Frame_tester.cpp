#include <time.h>  
#include <string>
#include <iostream>     


#include "UsbToSrb.h"
#include "SrbMaster.h"
#include "Broadcaster.h"
#include "./Nodes/dumotor/DumotorNode.h"


#include "PerformanceAnalyzer.h"

using namespace std;
using namespace srb::usb_bus;
using namespace srb;
string timeToString(time_t tim)
{
	char str[64];
	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", localtime(&tim));
	return str;
}
#ifdef WINDOW_86
#include <windows.h>

#define __Sleep(ms) Sleep(ms)
void setPriority() {
	return;
}

#endif
#if UNIX
#include <sched.h>
#include <unistd.h>
#define __Sleep(ms) usleep(ms*1000)
void setPriority() {
	struct sched_param param;
	int maxpri;
	maxpri = sched_get_priority_max(SCHED_FIFO);
	param.sched_priority = maxpri;
	if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) {
		cout << "Set priority fail" << endl;
	}
	else {
		cout << "Set priority done" << endl;
	}
}
#endif

const int TEST_PKG_NUM = 100000;
PerformanceTimer access_PT;

int main(int argc, char *argv[]) {
	try {
		setPriority();
		auto mainbusUB(std::make_unique<UsbToSrb>());
		auto mainSRBM(std::make_unique<SrbMaster>(mainbusUB.get()));
		char usb_port_name[] = "USB-TEST-BED";
		char test_node_name[] = "key ctrl";
		char test_node2_name[] = "key ctrl 2";
		int rev;
		time_t begin_time;
		
		DumotorNode * key_ctrl_DUMOTOR;
		DumotorNode * key_ctrl_2_DUMOTOR;

		rev = mainbusUB->openUsbByName(usb_port_name);
		if (rev != done) {
			cout << "Try open port: [" << usb_port_name << "] fail!" << endl;
			return -1;
		}
		cout << "Open port: [" << usb_port_name << "]" << endl << endl;
		time(&begin_time);
		cout << "Test send begin at " << timeToString(begin_time) << endl;

		cout << TEST_PKG_NUM << " accessing is doing. " << endl;
		mainSRBM->scanNodes();

		key_ctrl_DUMOTOR = DumotorNode::expand((*mainSRBM)[test_node_name]);
		key_ctrl_2_DUMOTOR = DumotorNode::expand((*mainSRBM)[test_node2_name]);

		if (key_ctrl_DUMOTOR == nullptr) {
			cout << "Node expand error,"  << endl;
			return -1;
		}
		if (key_ctrl_2_DUMOTOR == nullptr) {
			cout << "Node2 expand error, " << endl;
			return -1;
		}
		long long int totle_send_time_us = 0;
		for (int i = 0;i < TEST_PKG_NUM;i++) {
			key_ctrl_DUMOTOR->Data()->ma.brake = no;
			key_ctrl_DUMOTOR->Data()->ma.speed++;
			if (key_ctrl_DUMOTOR->Data()->ma.speed >= 1000) {
				key_ctrl_DUMOTOR->Data()->ma.speed = -1000;
			}
			key_ctrl_DUMOTOR->Data()->mb.brake = no;
			key_ctrl_DUMOTOR->Data()->mb.speed--;
			if (key_ctrl_DUMOTOR->Data()->mb.speed <= -1000) {
				key_ctrl_DUMOTOR->Data()->mb.speed = 1000;
			}

			key_ctrl_2_DUMOTOR->Data()->ma.brake = no;
			key_ctrl_2_DUMOTOR->Data()->ma.speed++;
			if (key_ctrl_2_DUMOTOR->Data()->ma.speed >= 1000) {
				key_ctrl_2_DUMOTOR->Data()->ma.speed = -1000;
			}
			key_ctrl_2_DUMOTOR->Data()->mb.brake = no;
			key_ctrl_2_DUMOTOR->Data()->mb.speed--;
			if (key_ctrl_2_DUMOTOR->Data()->mb.speed <= -1000) {
				key_ctrl_2_DUMOTOR->Data()->mb.speed = 1000;
			}

			for (access_PT.beginCheck();access_PT.Is_checking;access_PT.endCheck()) {
				key_ctrl_DUMOTOR->sendAccess(0);
				key_ctrl_2_DUMOTOR->sendAccess(0);
				mainbusUB->doAccess();
			}
			totle_send_time_us += access_PT.Last_time_cost;
		}

		for (int i = 0;i < 5;i++) {
			mainSRBM->commonBC->setLedAddress(BCC_SHOW_HIGH_ADDR);
			__Sleep(100);
			mainSRBM->commonBC->setLedAddress(BCC_SHOW_LOW_ADDR);
			__Sleep(100);
			mainSRBM->commonBC->setLedAddress(BCC_SHOW_CLOSE);
			__Sleep(100);
		}

		time_t end_time;	time(&end_time);
		cout << "Test send end at " << timeToString(end_time) << endl;
		cout << "It cost " << end_time - begin_time << "(s) at all." << endl;
		cout << "accessing time avariage is " << totle_send_time_us / (TEST_PKG_NUM) << "(us)." << endl;
		return 0;
	}
	catch(const char * str) {
		cout << "topic catch error:"<<str;
		return -2;
	}
}



