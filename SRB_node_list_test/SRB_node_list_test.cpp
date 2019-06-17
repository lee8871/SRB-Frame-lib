#include "UsbToSrb.h"
#include "Master.h"
#include "Node.h"
#include "BaseCluster.h"
#include "Broadcaster.h"
#include "StreamJsonWriter.h"

#include <iostream>     

using namespace std;
using namespace srb::usb_bus;
using namespace srb;



static UsbToSrb* mainbusUB;
static Master* mainSRBM;

int main(int argc, char *argv[]) {	
	//TODO: 让设置程序优先级的方法修改为 一个公共的类
	//setPriority();
	int16 speed = 0;
	mainbusUB = new UsbToSrb();
	mainSRBM = new Master(mainbusUB);

	cout << "try Open Port:" << mainbusUB->openUsbByName("USB-TEST-BED") << endl;
	mainSRBM->scanNodes();

	StreamJsonWriter * coutSJW; 
	coutSJW = new StreamJsonWriter(&cout);
	coutSJW->is_expand_mode = true;
	for (int i = 0;i < mainSRBM->MAX_NODE_NUM;i++) {
		if (mainSRBM->getNode(i) != null) {
			mainSRBM->getNode(i)->toJsonAll(*coutSJW);
		}
	}
	delete coutSJW;
	delete mainbusUB ;
	delete mainSRBM ;	
	system("pause");	
	return 0;
}
