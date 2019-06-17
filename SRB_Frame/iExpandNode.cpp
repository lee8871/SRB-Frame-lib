#include "Node.h"
#include "iExpandNode.h"
#include "iCluster.h"
#include "iBus.h"

namespace srb {

	int iExpandNode::addCluster(iCluster* cluster)	{
		if (base_node->clusters[cluster->Cluster_id] != null)
		{
			throw "Cluster is existence";
			return fail;
		}
		base_node->clusters[cluster->Cluster_id] = cluster;
		iAccess* acs = base_node->Bus()->newAccess(base_node);
		cluster->loadReadPkg(acs);
		base_node->Bus()->doAccess();
		return done;
	}

	iExpandNode::iExpandNode(Node * base_node){
		this->base_node = base_node;
		data_rs_void = base_node->data_rs;

	}

	iExpandNode::~iExpandNode()	{

	}

}
