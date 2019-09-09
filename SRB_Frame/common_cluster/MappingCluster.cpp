#include "srb.in.h"
#include "MappingCluster.h"
#include "iAccess.h"
#include "Json.h"
namespace srb {

	MappingCluster::MappingCluster(Node* node, uint8 cid)
		:iCluster(node)
	{
		_cluster_id = cid;
		data_len = sizeof(csThis);
	}
	MappingCluster::~MappingCluster(){	}


	Json MappingCluster::to_json{
#define relTo(value) (((csThis*)((MappingCluster*)0)->_data_u8)->value)
		{"up_len",&relTo(m.up_len)},
		{"down_len",&relTo(m.down_len)},
		{"table",buildUint8Array(relTo(m.table),28)}
	};





}
