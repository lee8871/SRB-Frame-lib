#include "Master.h"
#include "BaseNode.h"

namespace srb {
	BaseNode * Master::getNode(uint8 addr){
		if (nodes[addr] == null) {
			nodes[addr] = new BaseNode(addr,this);
		}
		return nodes[addr];
	}
	Master::Master(iBus* b)	{
		_bus = b;
	}

	Master::~Master(){
		for (int i = 0;i < 256;i++){
			if (nodes[i] != null) {
				delete (nodes[i]);
				nodes[i] = null;
			}
		}
	}


}

