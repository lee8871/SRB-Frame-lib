#include <string.h>
#include <stdio.h>

#include "lee.h"
#include "cLogger.h"
#include "Json.h"


#include "OsSupport.h" 
#include "UsbToSrb.h"
#include "Node.h"
#include "SrbMaster.h"
#include "StreamJsonWriter.h"
#include "cLogger.h"

using namespace std;
using namespace srb::usb_bus;
using namespace srb;
using namespace lee8871_support;
/*     _(:з」∠)_
现在终于决定不做能写入日志的log了。
原因很简单，c++的库文件太烂，太乱或者体量比本程序还大，或者又要依赖别的库比如boost。
c++流也是比较愚蠢的选择，不过可以保留。
json的方法还是可以沿用流的但是要移动到Master里面，以便控制记录是否打开。
日志系统全部搞在内存里面。使用内存搞一个字符串指针来存放出现的问题，然后通过调用方的日志系统记录错误
*/

char usb_port_name[64] = "";//like "USB-TEST-BED";
int listNode();

int main(int argc, char *argv[]) {
	for(int i = 0;i<argc; i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'B':
				strcpy(usb_port_name,argv[i]+2);
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
	setPriority();	
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
		Node* node = mainSRBM->getNode(i);
		if (node != nullptr) {

			printf("Addr:%-3d  Name:%-18s  Type:%s\n", node->Addr(), node->Node_name(), node->Node_type());
		}
	}
	return 0;
}
