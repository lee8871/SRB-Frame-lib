#include "argh.h"
#include <string.h>
#include <iostream>  
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


char *usb_port_name =nullptr;
int listNode();

int main(int argc, char *argv[]) {
	for(int i = 0;i<argc; i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'B':
					usb_port_name = argv[i] + 2;
				break;
				default:
					printf("Unknow parament '%s'\n",argv[i]);
					return -1;
			}
		}
	}
	return listNode();	
}



int listNode(){	
	if(usb_port_name[0] == '\0'){
		printf("test bus name should set by -B<bus_name>\n");
		return -1;			
	}
	OsSupport::setPriority();	
	auto mainbusUB(std::make_unique<UsbToSrb>());
	auto mainSRBM(std::make_unique<SrbMaster>(mainbusUB.get()));

	int rev;	
	rev = mainbusUB->openUsbByName(usb_port_name);
	if (rev != done) {
		printf("Try open port: [%s] fail!\n", usb_port_name );
		return -1;
	}	
	mainSRBM->scanNodes();
	auto coutSJW(std::make_unique<StreamJsonWriter>(&cout));
	coutSJW->is_expand_mode = true;
	for (int i = 0;i < mainSRBM->MAX_NODE_NUM;i++) {
		if (mainSRBM->getNode(i) != nullptr) {
			mainSRBM->getNode(i)->toJsonAll(*coutSJW);
		}
	}
	return 0;
}
