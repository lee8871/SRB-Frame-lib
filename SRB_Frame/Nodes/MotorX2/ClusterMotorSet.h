#pragma once

#include "iCluster.h"
#include "SRB-app-dumotor-share.h"
namespace srb {
	class ClusterMotorSet: public iCluster {
	private:
		using csThis = MotorX2::csMotorSet;
	public:
		constexpr static uint8 DEFAULT_CID = 10;
		ClusterMotorSet(Node* node);
		~ClusterMotorSet();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static  lee8871_support::Json to_json;
	};
};
