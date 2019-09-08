#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "SRB-app-dumotor-share.h"
namespace srb {
	class MotorSetCluster: public iCluster {
	private:
		using csThis = Du_Motor::csMotorSet;
	public:
		constexpr static uint8 DEFAULT_CID = 10;
		MotorSetCluster(Node* node);
		~MotorSetCluster();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static  lee8871_support::Json to_json;
	};
};
