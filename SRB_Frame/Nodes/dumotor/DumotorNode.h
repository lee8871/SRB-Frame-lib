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
	

	//TODO 下面再这个里面将Node分化成有特定功能的节点
	//但是需要从Node中取出这个节点,这个应该怎么写?



}