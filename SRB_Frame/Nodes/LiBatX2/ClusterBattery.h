#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "SRB-app-Charger-share.h"
namespace srb {
	class ClusterBattery : public iCluster {
	private:
		using csThis = Charger_2LiB::csBattery;
	public:
		constexpr static uint8 DEFAULT_CID = 11;
		ClusterBattery(Node* node);
		~ClusterBattery();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static Json to_json;
	};
};
