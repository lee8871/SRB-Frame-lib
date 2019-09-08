#include "MotorAdjCluster.h"
#include "Json.h"

namespace srb {
	using namespace Du_Motor;
	MotorAdjCluster::MotorAdjCluster(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	MotorAdjCluster::~MotorAdjCluster(){	}



	Json MotorAdjCluster::to_json{
#define relTo(value) (((csThis*)((MotorAdjCluster*)0)->_data_u8)->value)
		{"Adj",&relTo(Adj)},
		{"TogMotorA",&relTo(TogMotorA)},
		{"TogMotorB",&relTo(TogMotorB)}
	};

}
