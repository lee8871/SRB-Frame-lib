#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "SRB-app-dumotor-share.h"
namespace srb {
	class MotorAdjCluster: public iCluster {
	public:
		constexpr static uint8 DEFAULT_CID = 11;
		MotorAdjCluster(Node* node);
		~MotorAdjCluster();
		const Du_Motor::csMotorAdj* Data();
		Du_Motor::csMotorAdj* Buffer();
		virtual int toJson(iJsonWriter & json_printer)override;
		static Json to_json;
	};
};
