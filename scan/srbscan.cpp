#include <string.h>
#include <stdio.h>

#include "OsSupport.h" 
#include "UsbToSrb.h"
#include "Node.h"
#include "SrbMaster.h"
#include "Broadcaster.h"
#include "StreamJsonWriter.h"
#include "cLogger.h"


#include "stringHash.h"
#include "BaseCluster.h"
#include "ErrorCluster.h"
#include "infoCluster.h"

using namespace std;
using namespace lee8871_support;
using namespace srb::usb_bus;
using namespace srb;
using namespace lee8871_support;
using namespace lee8871_support;

int readCmd();
int readArgument(char* arg);
int main(int argc, char *argv[]) {
	setPriority();
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
	mainSRBM->scanNodes();
	return done;
}
int listNode(char* sarg) {
	char* file_name = new char[64];
	JsonGenerateString* jg = new JsonGenerateString(4096);
	for (int i = 0;i < mainSRBM->MAX_NODE_NUM;i++) {
		auto node = mainSRBM->getNode(i);
		if (node != nullptr) {
			printf("Addr:%-3d  Name:%-18s  Type:%s\n", node->Addr(), node->Node_name(), node->Node_type());
			jg->clear();
			Node::to_json.get(jg, node);
			snprintf(file_name, 64, "%s.json", node->Node_name());
			jg->writeToFile(file_name);
			puts(jg->Buf);
		}
	}
	delete jg;
	delete[] file_name;
	return done;
}
int addrLed(char* sarg) {
	char cmd = 'c';
	sscanf(sarg, " -%c", &cmd);
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
int showInfo(char* sarg) {
	int addr;
	char name[20];
	if (1 == sscanf(sarg, " %d", &addr)) {
		auto node = mainSRBM->getNode(addr);
		if (node != nullptr) {
			printf("Addr:%-3d  Name:%-18s  Type:%s\n", node->Addr(), node->Node_name(), node->Node_type());
			return done;
		}
		else {
			printf("Node which address = %d is not exist.\n", addr);
			return done;
		}
	}
	else if (1 == sscanf(sarg, " %s", &name)) {
		auto node = mainSRBM->getNode(name);
		if (node != nullptr) {
			printf("Addr:%-3d  Name:%-18s  Type:%s\n", node->Addr(), node->Node_name(), node->Node_type());
			return done;
		}
		else {
			printf("Node which name = %s is not exist.\n", name);
			return done;
		}
	}
	else {
		printf("Cmd 'info' should followed node name or addr.");
		return fail;
	}
}




//----------------------------------------------------------------------
int readArgument(char* arg) {
	if (arg[0] == '-') {
		switch (arg[1]) {
		case 'B':
			return connectToBus(arg + 2);
		default:
			printf("Unknow argument '%s' ignored. _(:з」∠)_\n", arg);
			return fail;
		}
	}
	return fail;
}	

int readCmd() {
	static char cmd[20];
	static char argument_str[256];
	static char c;
	cmd[0] = 0;
	argument_str[0] = 0;
	printf(">");
	scanf("%20s%[^\n]%c", cmd, argument_str, &c);
	switch (getHashCmd(cmd)) {
	case getHashCmd("ls"):
		return listNode(argument_str);
	case getHashCmd("exit"):
		exit(0);
	case getHashCmd("led"):
		return addrLed(argument_str);
	case getHashCmd("info"):
		return showInfo(argument_str);
	default:
		printf("input str is [%s],Id is %u\n", cmd, getHashCmd(cmd));
		return fail;
	}
}