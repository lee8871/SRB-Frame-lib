#include "iCluster.h"
#include "iAccess.h"

namespace srb {
	iCluster::iCluster(BaseNode* n)	{
		node = n;
	}
	void iCluster::loadReadPkg(iAccess* acs){
		acs->Send_pkg->data[0] = cluster_id;
		acs->Send_pkg->bfc.length = 1;
		acs->Send_pkg->bfc.port = SC_PORT_CFG;
	}
	void iCluster::readDone(iAccess * acs){
		if (acs->Status == eAccessStatus::RecvedDone) {
			for (int i = 0; i < acs->Recv_pkg->bfc.length;i++){
				_buffer[i] = acs->Recv_pkg->data[i];
			}
		}
	}

}