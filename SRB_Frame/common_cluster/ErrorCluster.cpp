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
		{"file",&relTo(file)},
		{"line",&relTo(line)},
		{"description",buildJsonStr((char*)relTo(description),24)},
	};



}
