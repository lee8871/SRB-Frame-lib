#include "BaseNode.h"
namespace srb {
	BaseNode::BaseNode(uint8 address){
		this->addr = address;
		clu[0] = new iCluster(this);
		clu[0]->loadReadPkg(bus->newAccess(this));
		bus->doAccess();


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
		iAccess* a = bus->newAccess(this);
		int i;
		for (i = 0;i < mapping[port]->down_len;i++) {
			a->send_pkg->data[i] = rs_data[mapping[port]->table[i]];
		}
		a->send_pkg->bfc.length = i;
		a->send_pkg->bfc.port = port;
		bus->addAccess(a);
	}


	void BaseNode::sendDone(iAccess * acs)	{		
		switch (acs->send_pkg->bfc.port)
		{
		case SC_PORT_D0:
		case SC_PORT_D1:
		case SC_PORT_D2:
		case SC_PORT_D3:
			delete acs;break;
		case SC_PORT_CFG:
			clu[acs->recv_pkg->data[0]]->readDone(acs);
			delete acs;break;
		default:
			delete acs;break;
		}
		
	}
}
