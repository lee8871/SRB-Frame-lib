#pragma once
#include "srb_heads.h"
#include "iCluster.h"

namespace srb {
	class BaseCluster: public iCluster {
	public:
		BaseCluster(BaseNode* node, uint8 addr);
		~BaseCluster();
		const csBase* readonly Data = (const csBase* const)data_u8;
		csBase* readonly Buffer = (csBase* const)buffer_u8;
	};
};
