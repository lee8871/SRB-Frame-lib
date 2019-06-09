#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"

namespace srb {
	class BaseCluster: public iCluster {
	public:
		static const uint8 DEFAULT_CID = 0;
		BaseCluster(BaseNode* node, uint8 addr, uint8 cid = DEFAULT_CID);
		~BaseCluster();
		const csBase* Data();
		csBase* Buffer();
		int static addressLedSet(iAccess* acs, uint8 cmd);
	};
};
