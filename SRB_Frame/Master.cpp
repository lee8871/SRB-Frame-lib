#include "Master.h"
#include "BaseNode.h"
#include <string>

namespace srb {
	int Master::addNode(uint8 addr){
		if (nodes[addr] == null) {
			nodes[addr] = new BaseNode(addr,this);
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
	BaseNode * Master::getNode(uint8 addr){
		return nodes[addr];
	}
	BaseNode * Master::getNode(const char * name)	{
		for (int i = 0;i < MAX_NODE_NUM; i++) {
			if (0 == strcmp(nodes[i] ->Node_name, name)) {
				return nodes[i];
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

	Master::Master(iBus* b)	{
		_bus = b;
	}

	Master::~Master(){
		for (int addr = 0;addr < MAX_NODE_NUM;addr++){
			removeNode(addr);
		}
	}


}

