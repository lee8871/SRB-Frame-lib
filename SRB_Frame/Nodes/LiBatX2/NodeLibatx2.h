#pragma once
#include "srb_heads.h"
#include "SRB-app-Charger-share.h"
#include "Node.h"
#include "ClusterBattery.h"
#include "ClusterBuzzer.h"
#include "ClusterInnRes.h"

namespace srb {
	class NodeLibatx2:public Node {
	public :

		NodeLibatx2() = delete;
		NodeLibatx2(const NodeLibatx2&) = delete;
		NodeLibatx2(NodeLibatx2&&) = delete;


		inline ClusterBattery*& c_battery() {
			return  (ClusterBattery *&)clusters[ClusterBattery::DEFAULT_CID];
		}
		inline ClusterBuzzer*& c_buzzer() {
			return  (ClusterBuzzer *&)clusters[ClusterBuzzer::DEFAULT_CID];
		}
		inline ClusterInnRes*& c_inn_res() {
			return  (ClusterInnRes *&)clusters[ClusterInnRes::DEFAULT_CID];
		}
		inline Charger_2LiB::sDataRs* Data(){
			return  (Charger_2LiB::sDataRs* const)(data_rs);
		}			   
		constexpr static const char* NODE_TYPE = "Charger_2LiB";
		lee8871_support::Json* finalToJson();
		int initFormNode();
		static int writeAllNode(Node *n);
	private:
		int __writeAllNode();
	};	



}