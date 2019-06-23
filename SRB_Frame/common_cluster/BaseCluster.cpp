#include "BaseCluster.h"
#include "iAccess.h"
#include "iJsonWriter.h"
namespace srb {

	BaseCluster::BaseCluster(Node* node, uint8 addr)
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
	int BaseCluster::toJson(iJsonWriter & json_printer) {
		json_printer.beginObj("Base_clu");
		json_printer.writeNum("Id", Cluster_id);
		json_printer.writeEndLine();
		json_printer.writeNum("addr", Data()->addr);
		json_printer.writeStr("name", (const char *)(Data()->name));
		json_printer.writeNum("error_behavior", Data()->error_behavior);
		json_printer.endObj();
		return done;
	}


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
