#include "MotorSetCluster.h"
#include "iAccess.h"
#include "iJsonWriter.h"

namespace srb {
	using namespace Du_Motor;
	MotorSetCluster::MotorSetCluster(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
	}
	MotorSetCluster::~MotorSetCluster(){	}



	const csMotorSet * MotorSetCluster::Data(){
		return (const csMotorSet* )Data_u8;
	}
	csMotorSet * MotorSetCluster::Buffer()	{
		return (csMotorSet*)buffer_u8;
	}
	int MotorSetCluster::toJson(iJsonWriter & json_printer) {
		json_printer.beginObj("set_adj");
		json_printer.writeNum("Id", Cluster_id);
		json_printer.writeEndLine();
		json_printer.writeNum("min_pwm_a", Data()->min_pwm_a);
		json_printer.writeNum("min_pwm_b", Data()->min_pwm_b);
		json_printer.writeNum("period", Data()->period);
		json_printer.writeNum("lose_control_ms", Data()->lose_control_ms);
		json_printer.writeNum("lose_behavior", Data()->lose_behavior);
		json_printer.endObj();
		return done;
	}

}
