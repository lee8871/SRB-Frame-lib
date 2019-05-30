#include "BaseNode.h"
namespace srb {
	BaseNode::BaseNode(){


	}
	BaseNode::~BaseNode(){

	}
	uint8 BaseNode::getAddr(){
		return addr;
	}
	//bool BaseNode::sendAccess(int port) {
	//	if ((port < 0) || (port > 3)) {
	//		throw "port should be int in [0,3]";
	//	}
	//	UsbAccess* a = new UsbAccess(this);
	//	sSrbPkg * pkg = a->getSendPkg();
	//	for (int i = 0;i < mapping[port]->down_len;i++) {
	//		pkg->data[i] = rs_data[mapping[port]->table[i]];
	//	}

	//}
	//uint8 BaseNode::
}
