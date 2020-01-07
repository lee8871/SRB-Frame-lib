#include "srb.in.h"
#include "InfoCluster.h"
#include "iAccess.h"
#include "Json.h"
namespace srb {
	InfoCluster::InfoCluster(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	InfoCluster::~InfoCluster() {	}


	Json InfoCluster::to_json{
#define relTo(value) (((csThis*)((InfoCluster*)0)->_data_u8)->value)
		{"node_version",{&relTo(node_version[0]),&relTo(node_version[1])}},
		{"srb_version",{&relTo(srb_version[0]),&relTo(srb_version[1])}},
		{"time_stamp",&relTo(time_stamp)},
		{"node_type",buildJsonStr((char*)relTo(node_type),17)},
	};
}
