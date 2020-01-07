#include "srb.in.h"
#include "ErrorCluster.h"
#include "iAccess.h"
#include "Json.h"
namespace srb {

	ErrorCluster::ErrorCluster(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	ErrorCluster::~ErrorCluster(){	}

	Json ErrorCluster::to_json{
#define relTo(value) (((csThis*)((ErrorCluster*)0)->_data_u8)->value)
		{"description",buildJsonStr((char*)relTo(description),24)},
		{"v0",&relTo(v[0])},
		{"v1",&relTo(v[1])},
		{"v2",&relTo(v[2])},
		{"v3",&relTo(v[3])},
		{"v4",&relTo(v[4])}
	};



}
