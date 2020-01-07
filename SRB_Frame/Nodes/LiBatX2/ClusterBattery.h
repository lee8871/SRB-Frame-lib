#pragma once

#include "iCluster.h"
#include "SRB-app-charger-share.h"
namespace srb {
	class ClusterBattery : public iCluster {
	private:
		using csThis = LiBatT2::csBattery;
	public:
		constexpr static uint8 DEFAULT_CID = 11;
		ClusterBattery(Node* node);
		~ClusterBattery();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static  lee8871_support::Json to_json;
	};
};
