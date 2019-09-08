#include "MotorSetCluster.h"
#include "iAccess.h"
#include "iJsonWriter.h"
#include "Json.h"

namespace srb {
	using namespace Du_Motor;
	MotorSetCluster::MotorSetCluster(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	MotorSetCluster::~MotorSetCluster(){	}

	Json MotorSetCluster::to_json{
#define relTo(value) (((csThis*)((MotorSetCluster*)0)->_data_u8)->value)
		{"min_pwm_a",&relTo(min_pwm_a)},
		{"min_pwm_b",&relTo(min_pwm_b)},
		{"period",&relTo(period)},
		{"lose_control_ms",&relTo(lose_control_ms)},
		{"lose_behavior",&relTo(lose_behavior)}
	};

}
