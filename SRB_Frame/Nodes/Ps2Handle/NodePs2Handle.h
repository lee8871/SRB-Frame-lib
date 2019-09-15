#pragma once

#include "SRB-app-dumotor-share.h"
#include "Node.h"
#include "ClusterHandleConfig.h"

namespace srb {
	class NodePs2Handle:public Node {
	public :

		NodePs2Handle() = delete;
		NodePs2Handle(const NodePs2Handle&) = delete;
		NodePs2Handle(NodePs2Handle&&) = delete;


		inline ClusterHandleConfig*& configCLU() {
			return  (ClusterHandleConfig *&)clusters[ClusterHandleConfig::DEFAULT_CID];
		}
		inline Ps2_Handle::sDataRs* Data(){
			return  (Ps2_Handle::sDataRs* const)(data_rs);
		}

		constexpr static const char* NODE_TYPE = "Ps2_Handle";
		lee8871_support::Json* finalToJson();
		int initFormNode();
		static int writeAllNode(Node *n);


	private:
		int __writeAllNode();
	};



}
