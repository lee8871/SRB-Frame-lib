#pragma once
#include "srb_heads.h"
#include "iExpandNode.h"
#include "MotorAdjCluster.h"
#include "MotorSetCluster.h"
namespace srb {

	class DumotorNode :public iExpandNode{
	private:
		MotorSetCluster* setCLU;
		MotorAdjCluster* adjCLU;
	public :
		static const char Node_type[];
		Du_Motor::sDataRs* Data(); 
	public :
		DumotorNode(Node* base_node);
		~DumotorNode();

		static DumotorNode* expand(Node* n);

	};	



}