#include "BaseCluster.h"
namespace srb {

	const csBase * BaseCluster::get_data()
	{
		return (const csBase *)buffer;
	}
	BaseCluster::BaseCluster(BaseNode* node) 
		:iCluster(node)
	{

	}
}
