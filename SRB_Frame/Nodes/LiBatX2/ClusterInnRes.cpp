#include "ClusterInnRes.h"

namespace srb {
	ClusterInnRes::ClusterInnRes(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		data_len = sizeof(csThis);
	}
	ClusterInnRes::~ClusterInnRes(){	}

	Json ClusterInnRes::to_json{
#define relTo(value) (((csThis*)((ClusterInnRes*)0)->_data_u8)->value)
		{"r0",&relTo(mOhm_a[0])},
		{"r1",&relTo(mOhm_a[1])},
		{"r2",&relTo(mOhm_a[2])},
		{"r3",&relTo(mOhm_a[3])},
		{"r4",&relTo(mOhm_a[4])}
	};

}
