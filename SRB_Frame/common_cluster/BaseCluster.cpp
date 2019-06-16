#include "BaseCluster.h"
#include "iAccess.h"
namespace srb {

	BaseCluster::BaseCluster(BaseNode* node, uint8 addr, uint8 cid)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		((csBase*)_data_u8)->addr = addr;
	}
	BaseCluster::~BaseCluster(){	}



	const csBase * BaseCluster::Data(){
		return ((const csBase* )(Data_u8()));
	}
	csBase * BaseCluster::Buffer()	{
		return (csBase*)buffer_u8;
	}

	int BaseCluster::toJson(char * str, int length) {
		int i;
		return 0;

	}



	//TODO:这个函数的用法有些奇怪,是个静态函数,考虑怎么重新
	int BaseCluster::addressLedSet(iAccess* acs, uint8 cmd)	{
		switch (cmd) {
		case BCC_SHOW_HIGH_ADDR:
		case BCC_SHOW_LOW_ADDR:
		case BCC_SHOW_CLOSE:
			acs->Send_pkg->data[0] = DEFAULT_CID;
			acs->Send_pkg->data[1] = cmd;
			acs->Send_pkg->bfc.length = 2;
			acs->Send_pkg->bfc.port = SC_PORT_CFG;
			return done;
		default:
			return par_error;
		}
	}
}
