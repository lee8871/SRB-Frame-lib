#include <string>
#include <stdio.h>   
#include <conio.h>


#include "UsbToSrb.h"
#include "SrbMaster.h"
#include "Broadcaster.h"
#include "./Nodes/dumotor/DumotorNode.h"
#include "PerformanceAnalyzer.h" 
#include "OsSupport.h" 
#include "transform.h"

using namespace std;
using namespace srb::usb_bus;
using namespace srb;

const int TEST_PKG_NUM = 10000000;
const int REPORT_RAT = 1000;

PerformanceTimer access_PT;
int main(int argc, char *argv[]) {
	try {
		OsSupport::setPriority();
		auto mainbusUB(std::make_unique<UsbToSrb>());
		auto mainSRBM(std::make_unique<SrbMaster>(mainbusUB.get()));
		char usb_port_name[] = "USB-TEST-BED";
		char test_node_name[] = "key ctrl";
		char test_node2_name[] = "key ctrl 2";
		int rev;		

		DumotorNode * key_ctrl_DUMOTOR;
		DumotorNode * key_ctrl_2_DUMOTOR;

		rev = mainbusUB->openUsbByName(usb_port_name);
		if (rev != done) {
			printf("Try open port: [%s] fail!\n", usb_port_name );
			return -1;
		}

		printf("Open port: [%s].\n", usb_port_name);
		char time_str_temp[64];
		trans::usTotimestr(time_str_temp,64, OsSupport::getTimesUs());
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
				while(_kbhit() != 0) {
					if (is_get_char_c = (_getche() == 'c')) {
						break;
					}
				}
				if (is_get_char_c){
					printf("Get c and stop test!\n");
					break;
				}
			}
		}
		printf("\n");
		trans::usTotimestr(time_str_temp, 64, OsSupport::getTimesUs());
		printf("Test end at %s\n", time_str_temp);
		getchar();
		return 0;
	}
	catch(const char * str) {
		printf("topic catch error: %s",str);
		return -2;
	}
}



