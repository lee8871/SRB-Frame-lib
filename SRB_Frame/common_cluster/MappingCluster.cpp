#include "MappingCluster.h"
#include "iAccess.h"
#include "iJsonWriter.h"
namespace srb {

	MappingCluster::MappingCluster(Node* node, uint8 cid)
		:iCluster(node)
	{
		_cluster_id = cid;
	}
	MappingCluster::~MappingCluster(){	}



	const csMapping * MappingCluster::Data(){
		return ((const csMapping* )(Data_u8()));
	}
	csMapping * MappingCluster::Buffer()	{
		return (csMapping*)buffer_u8;
	}
	int MappingCluster::toJson(iJsonWriter & json_printer) {
		json_printer.beginObj("Mapping_clu");
		json_printer.writeNum("Id", Cluster_id);
		json_printer.writeEndLine();
		json_printer.writeNum("up_len", Data()->m.up_len);
		json_printer.writeNumArray("up_mapping", Data()->m.table, Data()->m.up_len);
		json_printer.writeNum("down_len", Data()->m.down_len);
		json_printer.writeNumArray("down_mapping", Data()->m.table+ Data()->m.up_len, Data()->m.down_len);
		json_printer.endObj();
		return done;
	}




}
