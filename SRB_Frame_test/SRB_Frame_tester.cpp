#include <time.h>  
#include <string>
#include <stdio.h>   
#include <conio.h>  


#include "UsbToSrb.h"
#include "SrbMaster.h"
#include "Broadcaster.h"
#include "./Nodes/dumotor/DumotorNode.h"


#include "PerformanceAnalyzer.h" include <conio.h>

using namespace std;
using namespace srb::usb_bus;
using namespace srb;
char time_str_temp[64];
int timeToString(time_t& tim, char *str){
	return strftime(str, 64, "%Y-%m-%d %H:%M:%S", localtime(&tim));
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
		printf("Set priority fail");
	}
	else {
		printf("Set priority done");
	}
}
#endif

const int TEST_PKG_NUM = 10000000;
const int REPORT_RAT = 1000;
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
			printf("Try open port: [%s] fail!\n", usb_port_name );
			return -1;
		}

		printf("Open port: [%s].\n", usb_port_name);
		time(&begin_time);
		timeToString(begin_time, time_str_temp);
		printf("Test send begin at %s .\n", time_str_temp);


		printf("Close address LED.\n");
		mainSRBM->commonBC->setLedAddress(BCC_SHOW_CLOSE);
		mainbusUB->doAccess();


		mainSRBM->scanNodes();

		key_ctrl_DUMOTOR = DumotorNode::expand((*mainSRBM)[test_node_name]);
		key_ctrl_2_DUMOTOR = DumotorNode::expand((*mainSRBM)[test_node2_name]);

		if (key_ctrl_DUMOTOR == nullptr) {
			printf("Node expand error.");
			return -1;
		}
		if (key_ctrl_2_DUMOTOR == nullptr) {
			printf("Node2 expand error.");
			return -1;
		}
		long long int totle_send_time_us = 0;
		int report_counter = 0;


		printf("%d accesses are doing.\n", TEST_PKG_NUM);

		for (int access_group_counter = 0;access_group_counter < TEST_PKG_NUM;) {
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

			access_group_counter++;

			totle_send_time_us += access_PT.Last_time_cost;
			report_counter++;
			if (report_counter == REPORT_RAT){
				printf("[%d]: %d accesses time avariage is %10.1f(us).\n", access_group_counter, REPORT_RAT, (1.0 * totle_send_time_us / report_counter));
				totle_send_time_us = 0;
				report_counter = 0;		
				bool is_get_char_c = false;
				while(kbhit() != 0) {
					if (is_get_char_c = (getche() == 'c')) {
						break;
					}
				}
				if (is_get_char_c){
					printf("Get c and stop test!\n");
					break;
				}
			}
		}
		time_t end_time;	
		time(&end_time);

		printf("\n");
		timeToString(end_time, time_str_temp);
		printf( "Test end at %s\n" , time_str_temp);
		printf("It cost %10.2f(s) at all.\n", end_time - begin_time);
		getchar();
		return 0;
	}
	catch(const char * str) {
		printf("topic catch error: %s",str);
		return -2;
	}
}



