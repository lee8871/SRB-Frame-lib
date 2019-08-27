#include "Node.h"
#include "iExpandNode.h"
#include "iCluster.h"
#include "iBus.h"

#include "BaseCluster.h"
#include "InfoCluster.h"
#include "ErrorCluster.h"
#include "MappingCluster.h"

using namespace lee8871_support;
namespace srb {

	int iExpandNode::addCluster(iCluster* cluster)	{
		if (_base_node->clusters[cluster->Cluster_id] != nullptr)
		{
			Srb_log.addLog(eLogLevel::erro, "Cluster[ID=%d] is existence",cluster->Cluster_id);
			return fail;
		}
		_base_node->clusters[cluster->Cluster_id] = cluster;
		iAccess* acs = _base_node->Bus()->newAccess(_base_node);
		cluster->loadReadPkg(acs);
		_base_node->Bus()->loadAccess(acs);
		_base_node->Bus()->doAccess();
		return done;
	}

	iExpandNode::iExpandNode(Node * base_node){
		_base_node = base_node;
		data_rs_void = _base_node->data_rs;

	}

	iExpandNode::~iExpandNode()	{

	}

	int iExpandNode::sendAccess(int port){
		return _base_node->sendAccess(port);
	}

	const char * iExpandNode::Node_type()	{
		return _base_node->Node_type();
	}

	BaseCluster * iExpandNode::BaseCLU()	{
		return _base_node->baseCLU;
	}

	InfoCluster * iExpandNode::InfoCLU() {
		return _base_node->infoCLU;
	}

	ErrorCluster * iExpandNode::ErrorCLU() {
		return _base_node->errorCLU;
	}

	MappingCluster * iExpandNode::Mapping0CLU() {
		return _base_node->mapping0CLU;
	}


	SrbMaster * iExpandNode::Master()	{
		return _base_node->master;
	}
	Json* iExpandNode::base_to_json = nullptr;
	Json* iExpandNode::ToJson() {
		if (base_to_json == nullptr) {
			base_to_json = new Json{
		#define relTo(value) (((Node*)0)->value)
		{"base",buildJsonPtr(BaseCluster::to_json, &relTo(baseCLU))},
		{"error",buildJsonPtr(ErrorCluster::to_json, &relTo(errorCLU))},
		{"info",buildJsonPtr(InfoCluster::to_json, &relTo(infoCLU))},
		{"mapping",buildJsonPtr(MappingCluster::to_json, &relTo(mapping0CLU))}
			};
		}
		return base_to_json;
	}

}
