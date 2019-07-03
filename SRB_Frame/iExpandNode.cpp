#include "Node.h"
#include "iExpandNode.h"
#include "iCluster.h"
#include "iBus.h"

#include "BaseCluster.h"
#include "InfoCluster.h"
#include "ErrorCluster.h"
#include "MappingCluster.h"
namespace srb {

	int iExpandNode::addCluster(iCluster* cluster)	{
		if (base_node->clusters[cluster->Cluster_id] != nullptr)
		{
			logger.errPrint("Cluster[ID=%d] is existence",cluster->Cluster_id);
			return fail;
		}
		base_node->clusters[cluster->Cluster_id] = cluster;
		iAccess* acs = base_node->Bus()->newAccess(base_node);
		cluster->loadReadPkg(acs);
		base_node->Bus()->loadAccess(acs);
		base_node->Bus()->doAccess();
		return done;
	}

	iExpandNode::iExpandNode(Node * base_node){
		this->base_node = base_node;
		data_rs_void = base_node->data_rs;

	}

	iExpandNode::~iExpandNode()	{

	}

	int iExpandNode::sendAccess(int port){
		return base_node->sendAccess(port);
	}

	const char * iExpandNode::Node_type()	{
		return base_node->Node_type();
	}

	BaseCluster * iExpandNode::BaseCLU()	{
		return base_node->baseCLU;
	}

	InfoCluster * iExpandNode::InfoCLU() {
		return base_node->infoCLU;
	}

	ErrorCluster * iExpandNode::ErrorCLU() {
		return base_node->errorCLU;
	}

	MappingCluster * iExpandNode::Mapping0CLU() {
		return base_node->mapping0CLU;
	}


	SrbMaster * iExpandNode::Master()	{
		return base_node->master;
	}

}
