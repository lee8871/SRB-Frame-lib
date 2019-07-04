#include <string.h>
#include <stdio.h>   
//#include <conio.h>


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

int TEST_PKG_NUM = -1;
const int REPORT_RAT = 1000;

char usb_port_name[64] = "";//like "USB-TEST-BED";
char test_node_name[64] = "key ctrl";
char test_node2_name[64] = "key ctrl 2";

char node_name[64] = "";


int testOneNodeDUMOTOR();	
int testNode();	
int lsBus();
int enalbeLog(const char* pathname);
PerformanceTimer access_PT;

int main(int argc, char *argv[]) {
	for(int i = 0;i<argc; i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'B':
				strcpy(usb_port_name,argv[i]+2);
				break;
				case 'A':
				sscanf(argv[i],"-A%d",&TEST_PKG_NUM);
				if(TEST_PKG_NUM <=0){
					printf("Error parament '%s' ,use '-A2000' to set access 2000 times.\nAccess will set to 5000.\n",argv[i]);
				}
				break;
				case 'E':
				return lsBus();
				break;
				case 'L':
					if(enalbeLog(argv[i]+2)!=done){
						printf("Log file open fail <%s>",argv[i]);
					}
				break;
				case 'S':
				strcpy(node_name,argv[i]+2);
				break;
				default:
				printf("Unknow parament '%s'\n",argv[i]);
				return -1;
			}
		}
	}
	if(TEST_PKG_NUM <=0){
		TEST_PKG_NUM =5000;
	}
	if(node_name[0] == '\0'){
		return testNode();	
	}
	else{
		return testOneNodeDUMOTOR();
	}
}
FILE *fp = nullptr;

int writeToLog(char* str);
int enalbeLog(const char* pathname){
	if(fp!=nullptr){
		fclose(fp);
		fp = nullptr;
	}
	if(pathname[0] == '~'){
	char expandedPathName[256];
		snprintf(expandedPathName,256,"%s%s",getenv("HOME"),pathname+1);
		fp = fopen(expandedPathName,"a");
	}
	else{
		fp = fopen(pathname,"a");
	}


    if( fp == nullptr){
       return fail;		
    }
	else{
		logger.setReportCallback(writeToLog);	
	}
	//TODO:: close file
}
int writeToLog(char* str){
	fprintf(fp,"%s",str);
	fflush(fp);
	return done;
}

int lsBus(){
	char SRB_bus_name[10][64];
	auto mainbusUB(std::make_unique<UsbToSrb>());
	int counter = mainbusUB->lsUsbByName(SRB_bus_name,10);
	printf("Listing all USB-SRB name:\n");
	for(int i = 0;i<counter;i++){
		printf("\t%s\n", SRB_bus_name[i]);
	}
	
}	
	
	
int testNode(){
	try {
		char time_str_temp[64];
		if(usb_port_name[0] == '\0'){
			printf("test bus name should set by -B<bus_name>\n");
			return -1;			
		}
		OsSupport::setPriority();
		auto mainbusUB(std::make_unique<UsbToSrb>());
		auto mainSRBM(std::make_unique<SrbMaster>(mainbusUB.get()));
		DumotorNode * key_ctrl_DUMOTOR;
		DumotorNode * key_ctrl_2_DUMOTOR;

		int rev;	
		rev = mainbusUB->openUsbByName(usb_port_name);
		if (rev != done) {
			printf("Try open port: [%s] fail!\n", usb_port_name );
			return -1;
		}

		printf("Open port: [%s].\n", usb_port_name);
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
				if(done!=mainbusUB->doAccess()){
					printf("%s",logger.Last_error_str);
				}
			}

			access_group_counter++;

			totle_send_time_us += access_PT.Last_time_cost;
			report_counter++;
			if (report_counter == REPORT_RAT){
				printf("[%d]: %d accesses time avariage is %10.1f(us).\n", access_group_counter, REPORT_RAT, (1.0 * totle_send_time_us / report_counter));
				totle_send_time_us = 0;
				report_counter = 0;		
				bool is_get_char_c = false;
				/****************************************** 
				while(_kbhit() != 0) {
					if (is_get_char_c = (_getche() == 'c')) {
						break;
					}
				}
				if (is_get_char_c){
					printf("Get c and stop test!\n");
					break;
				}
				//********************************/
			}
		}
		printf("\n");
		trans::usTotimestr(time_str_temp, 64, OsSupport::getTimesUs());
		printf("Test end at %s\n", time_str_temp);
		//getchar();
		return 0;
	}
	catch(const char * str) {
		printf("topic catch error: %s",str);
		return -2;
	}
}

	
int testOneNodeDUMOTOR(){
	try {
		if(usb_port_name[0] == '\0'){
			printf("test bus name should set by -B<bus_name>\n");
			return -1;			
		}
		OsSupport::setPriority();
		auto mainbusUB(std::make_unique<UsbToSrb>());
		auto mainSRBM(std::make_unique<SrbMaster>(mainbusUB.get()));
		DumotorNode * node_DUMOTOR;

		int rev;	
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

		node_DUMOTOR = DumotorNode::expand((*mainSRBM)[node_name]);
		if (node_DUMOTOR == nullptr) {
			printf("Node expand error.");
			return -1;
		}

		long long int totle_send_time_us = 0;
		int report_counter = 0;
		printf("%d accesses are doing.\n", TEST_PKG_NUM);
		double speed = 0;
		double Acceleration = 0.2;
		for (int access_group_counter = 0;access_group_counter < TEST_PKG_NUM;) {
			node_DUMOTOR->Data()->ma.brake = no;
			node_DUMOTOR->Data()->ma.speed=speed;
			node_DUMOTOR->Data()->mb.brake = no;
			node_DUMOTOR->Data()->mb.speed=speed;
			speed+=Acceleration;
			if(speed>200){
				Acceleration = -0.2;
			}
			if(speed<-200){
				Acceleration = 0.2;
			}

			for (access_PT.beginCheck();access_PT.Is_checking;access_PT.endCheck()) {
				node_DUMOTOR->sendAccess(0);
				mainbusUB->doAccess();
			}
			OsSupport::msSleep(1);
			access_group_counter++;
			totle_send_time_us += access_PT.Last_time_cost;
			report_counter++;
			if (report_counter == REPORT_RAT){
				printf("[%d]: %d accesses time avariage is %10.1f(us).\n", access_group_counter, REPORT_RAT, (1.0 * totle_send_time_us / report_counter));
				totle_send_time_us = 0;
				report_counter = 0;		
				bool is_get_char_c = false;
			}
		}
		printf("\n");
		trans::usTotimestr(time_str_temp, 64, OsSupport::getTimesUs());
		printf("Test end at %s\n", time_str_temp);
		//getchar();
		return 0;
	}
	catch(const char * str) {
		printf("topic catch error: %s",str);
		return -2;
	}
}



