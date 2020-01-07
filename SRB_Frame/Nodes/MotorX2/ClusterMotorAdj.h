#pragma once

#include "iCluster.h"
#include "SRB-app-dumotor-share.h"
namespace srb {
	class ClusterMotorAdj: public iCluster {
	private:
		using csThis = MotorX2::csMotorAdj;
	public:
		constexpr static uint8 DEFAULT_CID = 11;
		ClusterMotorAdj(Node* node);
		~ClusterMotorAdj();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static lee8871_support::Json to_json;
	};
};
