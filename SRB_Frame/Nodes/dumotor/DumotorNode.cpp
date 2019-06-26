#include "Node.h"
#include "DumotorNode.h"
#include "MappingCluster.h"
#include <string.h>
#include "MotorAdjCluster.h"
#include "MotorSetCluster.h"

namespace srb {
	MotorSetCluster * DumotorNode::SetCLU()
	{
		return setCLU;
	}
	MotorAdjCluster * DumotorNode::AdjCLU()
	{
		return adjCLU;
	}
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
		base_node->setMapping(Mapping0CLU()->Data()->u8, 0);
		base_node->setMapping(Du_Motor::Mapping1, 1);
		base_node->setMapping(Du_Motor::Mapping2, 2);
		base_node->setMapping(Du_Motor::Mapping3, 1);
		
	}
	DumotorNode::~DumotorNode()	{	

	}

	DumotorNode * DumotorNode::expand(iExpandNode * n){
		if (n == null) {
			return null;
		}
		if (strcmp(n->Node_type(), Node_type) == 0) {
			return (DumotorNode *)n;
		}
		else {
			return null;
		}
	}

	const char DumotorNode::Node_type[] = "Du_Motor";


}
