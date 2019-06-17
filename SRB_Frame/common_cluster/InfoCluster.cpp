#include "InfoCluster.h"
#include "iAccess.h"
#include "iJsonWriter.h"
namespace srb {
	//TODO:整理全部的初始化簇ID
	InfoCluster::InfoCluster(Node* node, uint8 cid)
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


	int InfoCluster::toJson(iJsonWriter & json_printer) {
		json_printer.beginObj("Info_clu");
		json_printer.writeNum("Id", Cluster_id);
		json_printer.writeEndLine();
		json_printer.writeStr("node_type", (const char*)Data()->node_type);
		json_printer.writeEndLine();
		json_printer.writeNumArray("node_version_BCD", Data()->node_version_BCD, 2);
		json_printer.writeNumArray("srb_version_BCD", Data()->srb_version_BCD, 2);
		json_printer.writeNum("time_stamp", Data()->time_stamp);
		json_printer.endObj();
		return done;
	}

}
