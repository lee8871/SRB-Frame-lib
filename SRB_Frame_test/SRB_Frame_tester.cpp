#include "SRB_Frame_tester.h"
#include "UsbToSrb.h"
#include "Master.h"
#include "Node.h"
#include "BaseCluster.h"
#include "Broadcaster.h"

#include <time.h>  
#include <string>
#include <iostream>     

#include "PerformanceAnalyzer.h"


#include "./Nodes/dumotor/DumotorNode.h"

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

UsbToSrb* mainbusUB;
const int TEST_PKG_NUM = 10000;
Master* mainSRBM;
PerformanceTimer access_PT;

bool ctrlhandler(DWORD fdwctrltype);



int main(int argc, char *argv[]) {
	if (!(SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, true))){
		return 0;
	}	
	setPriority();
	mainbusUB = new UsbToSrb();
	mainSRBM = new Master(mainbusUB);
	char usb_port_name[] = "USB-TEST-BED";
	char test_node_name[] = "key ctrl";
	char test_node2_name[] = "key ctrl 2";
	int rev;
	rev = mainbusUB->openUsbByName(usb_port_name);
	if (rev != done) {
		cout << "Try open port: [" << usb_port_name << "] fail!"<<endl;
		goto main_error_exit;
	}
	cout << "Open port: [" << usb_port_name << "]" << endl << endl;

	time_t begin_time;	time(&begin_time);
	cout << "Test send begin at " << timeToString(begin_time) << endl;

	cout << TEST_PKG_NUM << " accessing is doing. " << endl;
	mainSRBM->scanNodes();
	Node * node = mainSRBM->getNode(test_node_name);
	Node * node2 = mainSRBM->getNode(test_node2_name);
	if(node ==null){
		cout << "get node fail, no node is named " << '"'<< test_node_name << '"' << endl;
		goto main_error_exit;
	}
	if (node == null) {
		cout << "get node2 fail, no node is named " << '"' << test_node2_name << '"' << endl;
		goto main_error_exit;
	}
	DumotorNode * key_ctrl_DUMOTOR = DumotorNode::expand(node);
	DumotorNode * key_ctrl_2_DUMOTOR = DumotorNode::expand(node2);

	if (key_ctrl_DUMOTOR == null) {
		cout << "Node expand error, Node type is " << '"' << node->Node_type() << '"' << endl;
		goto main_error_exit;
	}
	if (key_ctrl_2_DUMOTOR == null) {
		cout << "Node2 expand error, Node type is " << '"' << node->Node_type() << '"' << endl;
		goto main_error_exit;
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
			node->sendAccess(0);
			node2->sendAccess(0);
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
	
	
	main_error_exit:
	delete mainbusUB ;
	delete mainSRBM ;	
	system("pause");
	return 0;
}


bool ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)	{
	case CTRL_C_EVENT:
		cout<<"ctrl-c event\n\n";
		break;
	case CTRL_CLOSE_EVENT:
		cout << "ctrl-close event\n\n";
		break;
	case CTRL_BREAK_EVENT:
		cout << "ctrl-break event\n\n";
		break;
	case CTRL_LOGOFF_EVENT:
		cout << "ctrl-logoff event\n\n";
		break;
	case CTRL_SHUTDOWN_EVENT:
		cout << "ctrl-shutdown event\n\n";
		break;
	default:
		break;
	}
	delete mainbusUB;
	delete mainSRBM;
	return false;
}