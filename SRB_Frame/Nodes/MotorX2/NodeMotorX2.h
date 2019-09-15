#pragma once

#include "SRB-app-dumotor-share.h"
#include "Node.h"
#include "ClusterMotorAdj.h"
#include "ClusterMotorSet.h"

namespace srb {
	class NodeMotorX2:public Node {
	public :

		NodeMotorX2() = delete;
		NodeMotorX2(const NodeMotorX2&) = delete;
		NodeMotorX2(NodeMotorX2&&) = delete;


		inline ClusterMotorSet*& setCLU() {
			return  (ClusterMotorSet *&)clusters[ClusterMotorSet::DEFAULT_CID];
		}
		inline ClusterMotorAdj*& adjCLU() {
			return  (ClusterMotorAdj *&)clusters[ClusterMotorAdj::DEFAULT_CID];
		}
		inline Du_Motor::sDataRs* Data(){
			return  (Du_Motor::sDataRs* const)(data_rs);
		}
			   
		constexpr static const char* NODE_TYPE = "Du_Motor";
		lee8871_support::Json* finalToJson();
		int initFormNode();
		static int writeAllNode(Node *n);


	private:
		int __writeAllNode();
	};	



}