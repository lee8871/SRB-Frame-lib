#pragma once
#include "srb_heads.h"
#include "iCluster.h"

namespace srb {
	class BaseCluster: public iCluster {
	public:
		const csBase * get_data();
		BaseCluster(BaseNode* node);

	};
};
