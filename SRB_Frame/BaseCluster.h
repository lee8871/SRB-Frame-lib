#pragma once
#include "lee.h"
#include "iCluster.h"
#include "SRB-share-const.h"
#include "SRB-base-cluster-share.h"
#include "BaseNode.h"
#include "Master.h"


namespace srb {

	class BaseNode;
	class BaseCluster: public iCluster {
	public:
		const csBase * get_data();
		BaseCluster(BaseNode* node);

	};
};
