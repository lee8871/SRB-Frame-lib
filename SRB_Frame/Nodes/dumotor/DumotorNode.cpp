#include "Node.h"



#define __MASTER__
#define __MAPPING_DECLEAR__
#include "SRB-app-dumotor-share.h"
#undef __MASTER__
#undef __MAPPING_DECLEAR__

#include "DumotorNode.h"
#include "MappingCluster.h"
#include <string.h>

namespace srb {
	Du_Motor::sDataRs * DumotorNode::Data()
	{
		return  (Du_Motor::sDataRs*const)(base_node->data_rs);
	}
	DumotorNode::DumotorNode(Node * base_node):
		iExpandNode(base_node)
	{
		setCLU = new MotorSetCluster(base_node);
		adjCLU = new MotorAdjCluster(base_node);
		addCluster(setCLU);
		addCluster(adjCLU);
		base_node->setMapping(base_node->MappingCLU->Data()->u8, 0);
		base_node->setMapping(Du_Motor::mapping1, 1);
		base_node->setMapping(Du_Motor::mapping2, 2);
		base_node->setMapping(Du_Motor::mapping3, 1);
		
	}
	DumotorNode::~DumotorNode()	{	

	}

	DumotorNode * DumotorNode::expand(Node * n){
		if (n == null) {
			return null;
		}
		if (strcmp(n->Node_type(), Node_type) == 0) {
			return (DumotorNode *)(n->Expand_node);
		}
		else {
			return null;
		}
	}

	const char DumotorNode::Node_type[] = "Du_Motor";


}
