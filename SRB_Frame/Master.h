#pragma once
#include "srb_heads.h"

namespace srb {
	class iBus;

	class Master {
	protected:
		static const int MAX_NODE_NUM = 200;
		iBus* _bus = null;
		BaseNode* nodes[MAX_NODE_NUM] = { null };
	public:
		BaseNode* getNode(uint8 addr);
		BaseNode* getNode(const char* name);
		void scanNodes();


		int addNode(uint8 addr);
		int removeNode(uint8 addr);
		int changeAddressNode(uint8 addr,uint8 new_addr);



		Master(iBus* b);
		~Master();
		iBus* readonly Bus = _bus;

	};



}
