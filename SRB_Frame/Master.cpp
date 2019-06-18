#include "Master.h"
#include "Node.h"
#include "Broadcaster.h"
#include <string.h>

namespace srb {

	Master::Master(iBus* b) {
		_bus = b;
		commonBC = new Broadcaster(this);
	}

	Master::~Master() {
		for (int addr = 0;addr < MAX_NODE_NUM;addr++) {
			removeNode(addr);
		}
		delete commonBC;
	}

	int Master::addNode(uint8 addr){
		if (nodes[addr] == null) {
			nodes[addr] = new Node(addr,this);
			if (nodes[addr] == null) {
				return no_memory;
			}
			else {
				return done;
			}
		}
		else {
			return node_exsist;
		}
	}
	int Master::removeNode(uint8 addr){
		if (nodes[addr] == null) {
			return node_no_exsist;
		}
		else {
			delete (nodes[addr]);
			nodes[addr] = null;
			return done;
		}
	}
	Node * Master::getNode(uint8 addr){
		return nodes[addr];
	}
	Node * Master::getNode(const char * name)	{
		for (int i = 0;i < MAX_NODE_NUM; i++) {
			if (nodes[i] != null) {
				if (0 == strcmp(nodes[i]->Node_name(), name)) {
					return nodes[i];
				}
			}
		}
		return null;
	}

	void Master::scanNodes(){
		for (int addr = 0; addr < MAX_NODE_NUM; addr++) {
			if (done == addNode(addr)) {
				if (nodes[addr]->Exsist == false) {
					removeNode(addr);
				}
			}			
		}
	}




	iBus * Master::Bus()	{
		return _bus;
	}


}

