#pragma once

#include "iCluster.h"
#include "SRB-app-dumotor-share.h"
namespace srb {
	class MotorAdjCluster: public iCluster {
	private:
		using csThis = Du_Motor::csMotorAdj;
	public:
		constexpr static uint8 DEFAULT_CID = 11;
		MotorAdjCluster(Node* node);
		~MotorAdjCluster();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static lee8871_support::Json to_json;
	};
};
