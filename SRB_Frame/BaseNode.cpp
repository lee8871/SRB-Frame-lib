#include "BaseNode.h"
namespace srb {
	BaseNode::BaseNode(){


	}
	BaseNode::~BaseNode(){

	}
	uint8 BaseNode::getAddr(){
		return addr;
	}
	bool BaseNode::setMapping(const uint8* map, int port){
		if ((port < 0) || (port > 3)) {
			throw "port should be int in [0,3]";
		}
		sMapping* map_to_set = (sMapping*)map;
		int len = map_to_set->up_len + map_to_set->down_len + 2;
		uint8* map_copy = new uint8[len];
		if(map_copy==null){
			throw "New Map has No memory";
		}
		for (int i = 0; i < len;i++){
			map_copy[i] = map[i];
		}
		mapping[port] = (sMapping*)map_copy;
		return true;

	}

	bool BaseNode::sendAccess(int port) {
		if ((port < 0) || (port > 3)) {
			throw "port should be int in [0,3]";
		}
		UsbAccess* a = new UsbAccess(this);
		sSrbPkg * pkg = a->getSendPkg();
		int i;
		for (i = 0;i < mapping[port]->down_len;i++) {
			pkg->data[i] = rs_data[mapping[port]->table[i]];
		}
		pkg->bfc.length = i;
		pkg->bfc.port = port;
		bus->addAccess(a);
	}
	void BaseNode::sendDone(Access * a)
	{
		delete a;
	}

	//uint8 BaseNode::
}
