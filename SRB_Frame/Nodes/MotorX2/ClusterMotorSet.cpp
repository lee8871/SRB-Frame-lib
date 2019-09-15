#include "srb.in.h"
#include "ClusterMotorSet.h"
#include "iAccess.h"
#include "iJsonWriter.h"
#include "Json.h"

namespace srb {
	using namespace Du_Motor;
	ClusterMotorSet::ClusterMotorSet(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	ClusterMotorSet::~ClusterMotorSet(){	}

	Json ClusterMotorSet::to_json{
#define relTo(value) (((csThis*)((ClusterMotorSet*)0)->_data_u8)->value)
		{"min_pwm_a",&relTo(min_pwm_a)},
		{"min_pwm_b",&relTo(min_pwm_b)},
		{"period",&relTo(period)},
		{"lose_control_ms",&relTo(lose_control_ms)},
		{"lose_behavior",&relTo(lose_behavior)}
	};

}
