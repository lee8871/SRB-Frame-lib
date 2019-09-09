#include "srb.in.h"
#include "BaseCluster.h"
#include "iAccess.h"
#include "iJsonWriter.h"
#include "Json.h"

namespace srb {

	BaseCluster::BaseCluster(Node* node, uint8 addr)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
		((csThis*)_data_u8)->addr = addr;
		data_len = sizeof(csThis);
	}
	BaseCluster::~BaseCluster(){	}



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
			return argument_error;
		}
	}
	Json BaseCluster::to_json{
#define relTo(value) (((csThis*)((BaseCluster*)0)->_data_u8)->value)
		{"addr",&relTo(addr)},
		{"name",buildJsonStr((char*)relTo(name),27)},
		{"error_behavior",&relTo(error_behavior)}
	};


}
