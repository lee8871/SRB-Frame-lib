#include <string.h>
#include <stdio.h>

#include "OsSupport.h" 
#include "UsbToSrb.h"
#include "Node.h"
#include "SrbMaster.h"
#include "StreamJsonWriter.h"
#include "cLogger.h"

using namespace std;
using namespace srb::usb_bus;
using namespace srb;


FILE *fp = nullptr;
int writeToLog(char* str);
int enalbeLog(const char* pathname) {
	if (fp != nullptr) {
		fclose(fp);
		fp = nullptr;
	}
	if (pathname[0] == '~') {
		char expandedPathName[256];
		snprintf(expandedPathName, 256, "%s%s", getenv("HOME"), pathname + 1);
		fp = fopen(expandedPathName, "a");
	}
	else {
		fp = fopen(pathname, "a");
	}


	if (fp == nullptr) {
		return fail;
	}
	else {
		logger.setReportCallback(writeToLog);
	}
	//TODO:: close file
}
int writeToLog(char* str) {
	fprintf(fp, "%s", str);
	fflush(fp);
	return done;
}


char *usb_port_name =nullptr;
int listNode();
int main(int argc, char *argv[]) {
	for(int i = 0;i<argc; i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'B':
					usb_port_name = argv[i] + 2;
				break;
				case 'L':
					if (enalbeLog(argv[i] + 2) != done) {
						printf("Log file open fail <%s>", argv[i]);
					}
					break;
				default:
					printf("Unknow parament '%s'\n",argv[i]);
			}
		}
	}
	listNode();
	return 	0;
}

int listNode() {
	if (usb_port_name == nullptr) {
		printf("test bus name should set by -B<bus_name>\n");
		return -1;
	}
	printf("Scaning nodes on bus <%s>\n", usb_port_name);
	OsSupport::setPriority();
	auto mainbusUB(std::make_unique<UsbToSrb>());
	auto mainSRBM(std::make_unique<SrbMaster>(mainbusUB.get()));

	int rev;
	rev = mainbusUB->openUsbByName(usb_port_name);
	if (rev != done) {
		printf("Try open port: [%s] fail!\n", usb_port_name);
		return -1;
	}
	mainSRBM->scanNodes();
	for (int i = 0;i < mainSRBM->MAX_NODE_NUM;i++) {
		auto node = mainSRBM->getNode(i);
		if (node != nullptr) {
			printf("Addr:%-3d  Name:%-18s  Type:%s\n", node->Addr(), node->Node_name(), node->Node_type());
		}
	}
	return 0;
}

