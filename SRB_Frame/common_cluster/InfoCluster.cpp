#include "InfoCluster.h"
#include "iAccess.h"
namespace srb {
	//TODO:����ȫ���ĳ�ʼ����ID
	InfoCluster::InfoCluster(BaseNode* node, uint8 cid)
		:iCluster(node)
	{
		_cluster_id = cid;
	}
	InfoCluster::~InfoCluster(){	}



	const csInfo * InfoCluster::Data(){
		return ((const csInfo* )(Data_u8()));
	}
	csInfo * InfoCluster::Buffer()	{
		return (csInfo*)buffer_u8;
	}

	int InfoCluster::toJson(char * str, int length) {
		int i;
		return 0;

	}

}
