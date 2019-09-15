#include "srb.in.h"
#include "ClusterMotorAdj.h"
#include "Json.h"

namespace srb {
	using namespace Du_Motor;
	ClusterMotorAdj::ClusterMotorAdj(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	ClusterMotorAdj::~ClusterMotorAdj(){	}



	Json ClusterMotorAdj::to_json{
#define relTo(value) (((csThis*)((ClusterMotorAdj*)0)->_data_u8)->value)
		{"Adj",&relTo(Adj)},
		{"TogMotorA",&relTo(TogMotorA)},
		{"TogMotorB",&relTo(TogMotorB)}
	};

}
