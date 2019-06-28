#include "UsbToSrb.h"
#include "Node.h"
#include "SrbMaster.h"
#include "StreamJsonWriter.h"

#include <iostream>  
#include <stdio.h>
#include "Log.h"

using namespace std;
using namespace srb::usb_bus;
using namespace srb;
/*     _(:з」∠)_
现在终于决定不做能写入日志的log了。
原因很简单，c++的库文件太烂，太乱或者体量比本程序还大，或者又要依赖别的库比如boost。
c++流也是比较愚蠢的选择，不过可以保留。
json的方法还是可以沿用流的但是要移动到Master里面，以便控制记录是否打开。
日志系统全部搞在内存里面。使用内存搞一个字符串指针来存放出现的问题，然后通过调用方的日志系统记录错误
*/


static UsbToSrb* mainbusUB;
static SrbMaster* mainSRBM;int main(int argc, char *argv[]) {	
	//TODO: 让设置程序优先级的方法修改为 一个公共的类
	//setPriority();
	int16 speed = 0;
	mainbusUB = new UsbToSrb();
	mainSRBM = new SrbMaster(mainbusUB);
	srb::log.errPrint("hello error %d", 12);
	printf("try Open Port:%d\n",mainbusUB->openUsbByName("USB-TEST-BED"));
	mainSRBM->scanNodes();

	StreamJsonWriter * coutSJW; 
	coutSJW = new StreamJsonWriter(&cout);
	coutSJW->is_expand_mode = true;
	for (int i = 0;i < mainSRBM->MAX_NODE_NUM;i++) {
		if (mainSRBM->getNode(i) != nullptr) {
			mainSRBM->getNode(i)->toJsonAll(*coutSJW);
		}
	}
	delete coutSJW;
	delete mainbusUB ;
	delete mainSRBM ;	
	system("pause");	
	return 0;
}
