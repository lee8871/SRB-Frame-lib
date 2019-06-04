#pragma once
#include "srb_heads.h"

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
		iBus* readonly Bus = _bus;

	};

}
