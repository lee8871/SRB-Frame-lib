#include "InfoCluster.h"
#include "iAccess.h"
#include "iJsonWriter.h"
namespace srb {
	InfoCluster::InfoCluster(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
	}
	InfoCluster::~InfoCluster() {	}



	const csInfo * InfoCluster::Data() {
		return (const csInfo*)Data_u8();
	}
	csInfo * InfoCluster::Buffer() {
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
	Json InfoCluster::to_json{
#define relTo(value) (((csInfo*)((InfoCluster*)0)->_data_u8)->value)
		{"node_version",{&relTo(node_version_BCD[0]),&relTo(node_version_BCD[1])}},
		{"srb_version",{&relTo(srb_version_BCD[0]),&relTo(srb_version_BCD[1])}},
		{"time_stamp",&relTo(time_stamp)},
		{"node_type",buildJsonStr((char*)relTo(node_type),17)},
	};
}
