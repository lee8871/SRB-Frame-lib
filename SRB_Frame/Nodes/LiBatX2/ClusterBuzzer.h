#pragma once

#include "iCluster.h"
#include "SRB-app-charger-share.h"
namespace srb {
	class ClusterBuzzer : public iCluster {
	private:
		using csThis = Charger_2LiB::csBuzzer;
	public:
		constexpr static uint8 DEFAULT_CID = 12;
		ClusterBuzzer(Node* node);
		~ClusterBuzzer();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static  lee8871_support::Json to_json;
	};
};
