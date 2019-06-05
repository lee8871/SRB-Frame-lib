#include "BaseCluster.h"
namespace srb {

	BaseCluster::BaseCluster(BaseNode* node, uint8 addr) 
		:iCluster(node)
	{
		((csBase*)data_u8)->addr = addr;
	}
	BaseCluster::~BaseCluster(){	}



}
