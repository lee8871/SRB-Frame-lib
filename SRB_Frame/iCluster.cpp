#include "srb.in.h"
#include "iCluster.h"
#include "iAccess.h"
#include "Node.h"
#include "Json.h"

using namespace lee8871_support;
namespace srb {
	iCluster::iCluster(Node* n)	{
		_parent_node = n;
	}
	void iCluster::loadReadPkg(iAccess* acs){
		acs->Send_pkg->data[0] = _cluster_id;
		acs->Send_pkg->bfc.length = 1;
		acs->Send_pkg->bfc.port = SC_PORT_CFG;
	}
	void iCluster::loadWritePkg(iAccess* acs) {
		acs->Send_pkg->bfc.length = 1+ data_len;
		acs->Send_pkg->data[0] = _cluster_id;
		for (int i = 1; i < acs->Send_pkg->bfc.length;i++) {
			acs->Send_pkg->data[i] = _data_u8[i-1];
		}
		acs->Send_pkg->bfc.port = SC_PORT_CFG;
	}
	void iCluster::accessDone(iAccess * acs){
		if (acs->Status == eAccessStatus::RecvedDone) {
			if (acs->Send_pkg->bfc.length == 1) {
				for (int i = 0; i < acs->Recv_pkg->bfc.length;i++) {
					_data_u8[i] = acs->Recv_pkg->data[i];
				}
			}
		}
	}

}
