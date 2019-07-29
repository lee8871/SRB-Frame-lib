﻿#include <string.h>
#include <stdio.h>

#include "OsSupport.h" 
#include "UsbToSrb.h"
#include "Node.h"
#include "SrbMaster.h"
#include "Broadcaster.h"
#include "StreamJsonWriter.h"
#include "cLogger.h"


#include "stringHash.h"

using namespace std;
using namespace srb::usb_bus;
using namespace srb;

int readCmd();
int readArgument(char* arg);
int main(int argc, char *argv[]) {
	OsSupport::setPriority();
	for(int i = 0;i<argc; i++){
		readArgument(argv[i]);
	}
	while (1){ 
		readCmd();
	}
	return 	0;
}
auto mainbusUB = std::make_unique<UsbToSrb>();
auto mainSRBM = std::make_unique<SrbMaster>(mainbusUB.get());

int connectToBus(const char * usb_port_name) {
	printf("Scaning nodes on bus <%s>.\n", usb_port_name);
	int rev;
	rev = mainbusUB->openUsbByName(usb_port_name);
	if (rev != done) {
		printf("Try open port: [%s] fail!\n", usb_port_name);
		return fail;
	}
	return done;
}
int listNode(char* sarg) {
	mainSRBM->scanNodes();
	for (int i = 0;i < mainSRBM->MAX_NODE_NUM;i++) {
		auto node = mainSRBM->getNode(i);
		if (node != nullptr) {
			printf("Addr:%-3d  Name:%-18s  Type:%s\n", node->Addr(), node->Node_name(), node->Node_type());
		}
	}
	return done;
}
int addrLed(char* sarg) {
	char cmd = 'c';
	sscanf(sarg, " %c", &cmd);
	switch (cmd) {
	case 'h':
		mainSRBM->commonBC->setLedAddress(BCC_SHOW_HIGH_ADDR);
		break;
	case 'l':
		mainSRBM->commonBC->setLedAddress(BCC_SHOW_LOW_ADDR);
		break;
	case 'c':
		mainSRBM->commonBC->setLedAddress(BCC_SHOW_CLOSE);
		break;
	default:
		return fail;
	}
	mainbusUB->doAccess();
	return done;
}


//--------------------------log---------------------------------------
int customizeLogFile(const char* file_namepath) {
	switch (enalbeLog(file_namepath)) {
	case done:
		return done;
	default:
		printf("Log file open fail in argument '%s'.\n", file_namepath);
		return fail;
	}
}
int defaultLogFile() {
	switch (enalbeLogToEnv()) {
	case done:
		printf("Log file is set by ENV.\n");
		return done;
	case fail:
		printf("Log file is nether set by -L, nor set by ENV.\n");
		return fail;
	case redo:
		return done;
	default:
		throw "case error enalbeLogToEnv rev.\n";
	}
}


//----------------------------------------------------------------------
int readArgument(char* arg) {
	if (arg[0] == '-') {
		switch (arg[1]) {
		case 'B':
			return connectToBus(arg + 2);
		case 'L':
			return customizeLogFile(arg + 2);
		default:
			printf("Unknow argument '%s' ignored. _(:з」∠)_\n", arg);
			return fail;
		}
	}
}	

int readCmd() {
	static char cmd[20];
	static char argument_str[256];
	static char c;
	cmd[0] = 0;
	argument_str[0] = 0;
	printf(">");
	scanf("%20s%[^\n]%c", cmd, argument_str, &c);
	switch (getId(cmd)) {
	case getId("ls"):
		return listNode(argument_str);
	case getId("exit"):
		exit(0);
	case getId("led"):
		return addrLed(argument_str);
	case: 
	default:
		printf("input str is [%s],Id is %u\n", cmd, getId(cmd));
		return fail;
	}
}