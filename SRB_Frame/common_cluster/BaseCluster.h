#pragma once
#include "srb_heads.h"
#include "iCluster.h"

namespace srb {
	class BaseCluster: public iCluster {
	public:
		BaseCluster(BaseNode* node);
		~BaseCluster();
		const csBase * get_data();

	};
};
