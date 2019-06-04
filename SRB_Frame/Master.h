#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "SRB-base-cluster-share.h"
#include "iBus.h"
#include "BaseNode.h"

namespace srb {
	class iBus;

	class Master {
	protected:
		iBus* _bus = null;
		BaseNode* nodes[256] = {null};
	public:
		BaseNode* getNode(uint8 addr);
		BaseNode* getNode(const char* name);
		void scanNodes();
		Master(iBus* b);
		~Master();

		inline iBus* get_bus() { return _bus;}
	
		

	};

}
