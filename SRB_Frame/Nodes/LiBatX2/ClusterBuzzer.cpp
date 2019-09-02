#include "ClusterBuzzer.h"

namespace srb {
	ClusterBuzzer::ClusterBuzzer(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	ClusterBuzzer::~ClusterBuzzer(){	}

	Json ClusterBuzzer::to_json{
#define relTo(value) (((csThis*)((ClusterBuzzer*)0)->_data_u8)->value)
		{"power_on",&relTo(power_on)},
		{"jack_in_vot_low",&relTo(jack_in_vot_low)},
		{"jack_in_charge_close",&relTo(jack_in_charge_close)},
		{"charging",&relTo(charging)},
		{"charge_done",&relTo(charge_done)},
		{"change_done_next",&relTo(change_done_next)},
		{"jack_out",&relTo(jack_out)},
		{"low_power",&relTo(low_power)}
	};

}
