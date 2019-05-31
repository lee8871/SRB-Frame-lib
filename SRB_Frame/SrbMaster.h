#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "SRB-base-cluster-share.h"
#include "iBus.h"

namespace srb {
	class iSrbMaster {
	protected:
		iBus* srb_bus;
		BaseNode* nodes[256] = {null};
	public:
		BaseNode* getNode(uint8 addr);
		BaseNode* getNode(const char* name);
		void scanNodes();


	
		

	};

}
