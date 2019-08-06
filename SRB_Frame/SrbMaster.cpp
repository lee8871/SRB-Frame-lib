#include "SrbMaster.h"
#include "Node.h"
#include "Broadcaster.h"
#include <string.h>

using namespace lee8871_support;
namespace srb {

	SrbMaster::SrbMaster(iBus* b) {
		_bus = b;
		commonBC = new Broadcaster(this);
	}

	SrbMaster::~SrbMaster() {
		for (int addr = 0;addr < MAX_NODE_NUM;addr++) {
			removeNode(addr);
		}
		delete commonBC;
	}

	int SrbMaster::addNode(uint8 addr){
		if (nodes[addr] == nullptr) {
			nodes[addr] = new Node(addr,this);
			if (nodes[addr] == nullptr) {
				return new_fail;
			}
			else {
				return done;
			}
		}
		else {
			return node_exsist;
		}
	}
	int SrbMaster::removeNode(uint8 addr){
		if (nodes[addr] == nullptr) {
			return node_no_exsist;
		}
		else {
			delete (nodes[addr]);
			nodes[addr] = nullptr;
			return done;
		}
	}

	iExpandNode*  SrbMaster::operator[] (uint8 addr) {
		Node * n = getNode(addr);
		if(n == nullptr){
			return nullptr;
		}
		return n->Expand_node;
	}

	iExpandNode* SrbMaster::operator[] (const char* name) {

		Node * n = getNode(name);
		if (n == nullptr) {
			return nullptr;
		}
		return n->Expand_node;
	}


	Node * SrbMaster::getNode(uint8 addr){
		return nodes[addr];
	}

	Node * SrbMaster::getNode(const char * name)	{
		for (int i = 0;i < MAX_NODE_NUM; i++) {
			if (nodes[i] != nullptr) {
				if (0 == strcmp(nodes[i]->Node_name(), name)) {
					return nodes[i];
				}
			}
		}
		return nullptr;
	}

	void SrbMaster::scanNodes(){
		for (int addr = 0; addr < MAX_NODE_NUM; addr++) {
			if (done == addNode(addr)) {
				if (nodes[addr]->Exsist == false) {
					removeNode(addr);
				}
			}			
		}
	}

	iBus * SrbMaster::Bus()	{
		return _bus;
	}


}

