#include "iCluster.h"

namespace srb {
	iCluster::iCluster(BaseNode* n)	{
		node = n;
	}
	void iCluster::loadReadPkg(iAccess* acs){
		acs->send_pkg()->data[0] = cluster_id;
		acs->send_pkg()->bfc.length = 1;
		acs->send_pkg()->bfc.port = SC_PORT_CFG;
	}
	void iCluster::readDone(iAccess * acs){
		if (acs->getStatus() == eAccessStatus::RecvedDone) {
			for (int i = 0; i < acs->recv_pkg()->bfc.length;i++){
				buffer[i] = acs->recv_pkg()->data[i];
			}
		}
	}

}
