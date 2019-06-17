#include "iCluster.h"
#include "iAccess.h"
#include "BaseNode.h"
#include "iJsonWriter.h"
namespace srb {

	iCluster::iCluster(BaseNode* n)	{
		_node = n;
	}
	void iCluster::loadReadPkg(iAccess* acs){
		acs->Send_pkg->data[0] = _cluster_id;
		acs->Send_pkg->bfc.length = 1;
		acs->Send_pkg->bfc.port = SC_PORT_CFG;
	}
	void iCluster::readDone(iAccess * acs){
		if (acs->Status == eAccessStatus::RecvedDone) {
			for (int i = 0; i < acs->Recv_pkg->bfc.length;i++){
				_data_u8[i] = acs->Recv_pkg->data[i];
			}
		}
	}
	const uint8 * iCluster::Data_u8() {
		return _data_u8;
	}

	int iCluster::toJson(iJsonWriter & json_printer) {
		json_printer.beginObj("unknow_clu");
		json_printer.writeNum("Id", Cluster_id);
		json_printer.writeEndLine();
		json_printer.writeNumArray("datas", _data_u8, 30);
		json_printer.endObj();
		return done;
	}

}
