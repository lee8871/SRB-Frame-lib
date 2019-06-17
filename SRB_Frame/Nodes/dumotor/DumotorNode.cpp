#include "Node.h"
#include "DumotorNode.h"

namespace srb {
	DumotorNode::DumotorNode(Node * base_node):
		iExpandNode(base_node)
	{
		setCLU = new MotorSetCluster(base_node);
		adjCLU = new MotorAdjCluster(base_node);
		addCluster(setCLU);
		addCluster(adjCLU);
		
	}

	DumotorNode::~DumotorNode()	{
		

	}



}
