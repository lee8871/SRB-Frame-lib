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
		Du_Motor::sDataRs*const & Data_rs = (Du_Motor::sDataRs*const)data_rs_void;
	public :
		DumotorNode(Node* base_node);
		~DumotorNode();
	};	
}