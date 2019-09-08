#include "ClusterBattery.h"
#include "Json.h"

namespace srb {


	ClusterBattery::ClusterBattery(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	ClusterBattery::~ClusterBattery(){	}



	Json ClusterBattery::to_json{
#define relTo(value) (((csThis*)((ClusterBattery*)0)->_data_u8)->value)
		{"low_power_alram_mV",&relTo(low_power_alram_mV)},
		{"charger_current_mA",&relTo(charger_current_mA)},
		{"capacity_mAh",&relTo(capacity_mAh)},
		{"inn_res_mOhm",&relTo(inn_res_mOhm)},
		{"sc",&relTo(sc.u8)}
	};
}
