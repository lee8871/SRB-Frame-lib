#include "MotorAdjCluster.h"
#include "iJsonWriter.h"

namespace srb {
	using namespace Du_Motor;
	MotorAdjCluster::MotorAdjCluster(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
	}
	MotorAdjCluster::~MotorAdjCluster(){	}



	const csMotorAdj * MotorAdjCluster::Data(){
		return (const csMotorAdj* )Data_u8();
	}
	csMotorAdj * MotorAdjCluster::Buffer()	{
		return (csMotorAdj*)buffer_u8;
	}
	int MotorAdjCluster::toJson(iJsonWriter & json_printer) {
		json_printer.beginObj("adj_clu");
		json_printer.writeNum("Id", Cluster_id);
		json_printer.writeEndLine();
		json_printer.writeBool("TogMotorA", (no == Data()->TogMotorA));
		json_printer.writeBool("TogMotorB", (no == Data()->TogMotorB));
		json_printer.writeNum("Adj", Data()->Adj);
		json_printer.endObj();
		return done;
	}

}
