#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "SRB-app-dumotor-share.h"
namespace srb {
	class MotorSetCluster: public iCluster {
	public:
		constexpr static uint8 DEFAULT_CID = 10;
		MotorSetCluster(Node* node);
		~MotorSetCluster();
		const Du_Motor::csMotorSet* Data();
		Du_Motor::csMotorSet* Buffer();
		virtual int toJson(iJsonWriter & json_printer)override;
		static Json to_json;
	};
};
