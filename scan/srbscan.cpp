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

#include "LString.h"



using namespace std;
using namespace lee8871_support;
using namespace srb::usb_bus;
using namespace srb;
using namespace lee8871_support;
using namespace lee8871_support;

int readCmd();

int connectToBus(const char * usb_port_name);
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


static int scanNode(LString* sarg,Node** node) {
	int addr;
	char name[27];
	*node = nullptr;
	sarg->removeSpace();
	if (1 == sscanf(sarg->Ptr, "%d", &addr)) {
		*node = mainSRBM->getNode(addr);
		sarg->nextGroup();
		return done;
	}
	else if (1 == sscanf(sarg->Ptr, "%s", name)) {
		*node = mainSRBM->getNode(name);
		sarg->nextGroup();
		return done;
	}
	return false;
}





int listNode(LString* sarg) {
	for (int i = 0;i < mainSRBM->MAX_NODE_NUM;i++) {
		auto node = mainSRBM->getNode(i);
		if (node != nullptr) {
			printf("Addr:%-3d  Name:%-18s  Type:%s\n", node->Addr(), node->Node_name(), node->Node_type());
		}
	}
	return done;
}

int addrLed(LString* sarg) {
	char cmd = 'c';
	sarg->removeSpace();
	switch (*(sarg->Ptr)){
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

int showInfo(LString* sarg) {
	Node* node = nullptr;
	if (fail == scanNode(sarg, &node)) {
		printf("Cmd 'info' should followed node name or addr.\n");
		return fail;
	}
	if (node == nullptr) {
		printf("Node is not exist.\n");
		return fail;
	}
	char* file_name = new char[64];
	JsonGenerateString* jg = new JsonGenerateString(4096);
	jg->clear();
	node->to_json.get(jg);
	snprintf(file_name, 64, "%s.json", node->Node_name());
	jg->writeToFile(file_name);
	puts(jg->Buf);
	delete jg;
	delete[] file_name;
}

int getCluster(LString* sarg) {
	Node* node = nullptr;
	if (fail == scanNode(sarg, &node)) {
		printf("Cmd 'info' should followed node name or addr.\n");
		return fail;
	}
	if (node == nullptr) {
		printf("Node is not exist.\n");
		return fail;
	}
	char* file_name = new char[64];
	JsonGenerateString* jg = new JsonGenerateString(4096);
	jg->clear();
	node->to_json.get(jg);
	snprintf(file_name, 64, "%s.json", node->Node_name());
	jg->writeToFile(file_name);
	puts(jg->Buf);
	delete jg;
	delete[] file_name;
}


int setCluster(LString* sarg) {
	Node* node = nullptr;
	if (fail == scanNode(sarg, &node)) {
		printf("Cmd 'info' should followed node name or addr.\n");
		return fail;
	}
	if (node == nullptr) {
		printf("Node is not exist.\n");
		return fail;
	}
	char* file_name = new char[64];
	JsonParseString *jp = nullptr;
	if (1 == sscanf(sarg->Ptr, "%s", file_name)) {
		jp = new JsonParseString{ file_name };//TODO  用工厂函数，处理文件打开失败的情况
	}

	delete[] file_name;
	if (jp == nullptr) {
		printf("setclu useage: setclu [addr/name] [file path]\n");
		return fail;
	}
	else {
		int rev_set = node->to_json.set(jp);
		if (nullptr != jp->Error_str) {
			if (rev_set == done) {
				setTerminalColor(eTerminalColor::warning);
			}
			else {
				setTerminalColor(eTerminalColor::error);
			}
			printf("%s\n", jp->Error_str->Buf);
			setTerminalColor(eTerminalColor::normal);
		}
		delete jp;
		node->writeAllNode(node);

		return done;
	}
}


int changeName(LString* sarg) {
	Node* node = nullptr;
	if (fail == scanNode(sarg, &node)) {
		printf("Cmd 'info' should followed node name or addr.\n");
		return fail;
	}
	if (node == nullptr) {
		printf("Node is not exist.\n");
		return fail;
	}
	char name[27];
	sarg->getGroup(name, 27);
	node->changeName(name);
}

int changeAddr(LString* sarg) {
	Node* node = nullptr;
	if (fail == scanNode(sarg, &node)) {
		printf("Cmd 'info' should followed node name or addr.\n");
		return fail;
	}
	if (node == nullptr) {
		printf("Node is not exist.\n");
		return fail;
	}
	int new_addr;
	if (1 != sscanf(sarg->Ptr, "%d", &new_addr)) {
		printf("Need new addr.\n");
		return fail;
	}
	mainSRBM->changeAddressNode(node->Addr(), new_addr);
}




int readCmd() {
	static char cmd[20];
	static LString argument_str{ 256 };
	cmd[0] = 0;
	argument_str.clear();
	printf(">");
	scanf("%20s%[^\n]", cmd, argument_str.Buf);

	switch (getHashCmd(cmd)) {
	case getHashCmd("ls"):
		return listNode(&argument_str);
	case getHashCmd("exit"):
		exit(0);
	case getHashCmd("led"):
		return addrLed(&argument_str);
	case getHashCmd("info"):
		return showInfo(&argument_str);
	case getHashCmd("getclu"):
		return getCluster(&argument_str);
	case getHashCmd("setclu"):
		//setclu left left.json 
		return setCluster(&argument_str);
	case getHashCmd("setname"):
		return changeName(&argument_str);
	case getHashCmd("changeaddr"):
	case getHashCmd("ca"):
		return changeAddr(&argument_str);
	default:
		printf("input str is [%s],Id is %u\n", cmd, getHashCmd(cmd));
		return fail;
	}
	int n = getchar();
}
