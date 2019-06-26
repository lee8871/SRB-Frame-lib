#pragma once
#include "srb_heads.h"
#include "iExpandNode.h"
#include "SRB-app-dumotor-share.h"
namespace srb {
	class MotorSetCluster;
	class MotorAdjCluster;
	class DumotorNode :public iExpandNode{
	private:
		MotorSetCluster* setCLU;
		MotorAdjCluster* adjCLU;
	public :

		MotorSetCluster* SetCLU();
		MotorAdjCluster* AdjCLU();

		static const char Node_type[];
		Du_Motor::sDataRs* Data(); 


	public :
		DumotorNode(Node* base_node);
		~DumotorNode();
		static DumotorNode* expand(iExpandNode* n);
	};	



}