#include "MotorAdjCluster.h"
#include "iJsonWriter.h"
using namespace Du_Motor;

namespace srb {
	//TODO:整理全部的初始化簇ID
	MotorAdjCluster::MotorAdjCluster(Node* node, uint8 cid)
		:iCluster(node)
	{
		_cluster_id = cid;
	}
	MotorAdjCluster::~MotorAdjCluster(){	}



	const csMotorAdj * MotorAdjCluster::Data(){
		return ((const csMotorAdj* )(Data_u8()));
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
