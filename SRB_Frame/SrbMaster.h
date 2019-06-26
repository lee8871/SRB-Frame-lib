#pragma once
#include "srb_heads.h"
namespace srb {
	class iBus;
	class SrbMaster {
	public:
		static const int MAX_NODE_NUM = 200;
	protected:
		iBus* _bus = nullptr;
		Node* nodes[MAX_NODE_NUM] = { nullptr };
	public:
		Broadcaster* commonBC;
		void scanNodes();
		iExpandNode* operator[] (uint8 addr);
		iExpandNode* operator[] (const char* name);

		Node* getNode(uint8 addr);
		Node* getNode(const char* name);
		
		int addNode(uint8 addr);
		int removeNode(uint8 addr);
		int changeAddressNode(uint8 addr,uint8 new_addr);
			   
		SrbMaster(iBus* b);
		~SrbMaster();
		iBus* Bus();

	};



}
