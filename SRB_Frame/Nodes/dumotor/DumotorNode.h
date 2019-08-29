#pragma once
#include "srb_heads.h"
#include "SRB-app-dumotor-share.h"
#include "Node.h"
#include "MotorAdjCluster.h"
#include "MotorSetCluster.h"


namespace srb {
	class MotorSetCluster;
	class MotorAdjCluster;
	class DumotorNode:public Node {
	public :

		DumotorNode() = delete;
		DumotorNode(const DumotorNode&) = delete;
		DumotorNode(DumotorNode&&) = delete;


		inline MotorSetCluster*& setCLU() {
			return  (MotorSetCluster *&)clusters[MotorSetCluster::DEFAULT_CID];
		}
		inline MotorAdjCluster*& adjCLU() {
			return  (MotorAdjCluster *&)clusters[MotorAdjCluster::DEFAULT_CID];
		}
		inline Du_Motor::sDataRs* Data(){
			return  (Du_Motor::sDataRs* const)(data_rs);
		}



		constexpr static const char* NODE_TYPE = "Du_Motor";
		lee8871_support::Json* finalToJson();
		int initFormNode();
	};	



}