#include "srb.in.h"
#include "ClusterHandleConfig.h"
#include "iAccess.h"
#include "iJsonWriter.h"
#include "Json.h"

namespace srb {
	using namespace Joystick;
	ClusterHandleConfig::ClusterHandleConfig(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	ClusterHandleConfig::~ClusterHandleConfig(){	}

	Json ClusterHandleConfig::to_json{
#define relTo(value) (((csThis*)((ClusterHandleConfig*)0)->_data_u8)->value)
		{"online_rumble_10ms",&relTo(online_rumble_10ms)},
		{"lose_rumble_10ms",&relTo(lose_rumble_10ms)},
		{"rumble_L_strength",&relTo(rumble_L_strength)}
	};
}
