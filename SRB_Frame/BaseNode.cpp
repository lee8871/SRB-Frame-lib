#include "BaseNode.h"
#include "BaseCluster.h"
namespace srb {
	BaseNode::BaseNode(uint8 address, Master* master){
		this->addr = address;
		this->master = master;

		baseCLU = new BaseCluster(this);
		clu[0] = baseCLU;
		baseCLU->loadReadPkg(get_bus()->newAccess(this));
		get_bus()->doAccess();
	}

	BaseNode::~BaseNode(){
		for (int i = 0;i < 4;i++) {
			if (mapping[i] != null) {
				delete (mapping[i]);
				mapping[i] = null;
			}
		}
		for (int i = 0;i < MAX_CLUSTER_NUMBER;i++) {
			if (clu[i] != null) {
				delete (clu[i]);
				clu[i] = null;
			}
		}
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
		iAccess* a = get_bus()->newAccess(this);
		int i;
		for (i = 0;i < mapping[port]->down_len;i++) {
			a->send_pkg()->data[i] = rs_data[mapping[port]->table[i]];
		}
		a->send_pkg()->bfc.length = i;
		a->send_pkg()->bfc.port = port;
		return true;
	}

	void BaseNode::sendDone(iAccess * acs)	{		
		switch (acs->send_pkg()->bfc.port)
		{
		case SC_PORT_D0:
		case SC_PORT_D1:
		case SC_PORT_D2:
		case SC_PORT_D3:
			break;
		case SC_PORT_CFG:
			clu[acs->send_pkg()->data[0]]->readDone(acs);
			break;
		default:
			break;
		}
	}

	iBus* BaseNode::get_bus() { return master->get_bus(); }
}
