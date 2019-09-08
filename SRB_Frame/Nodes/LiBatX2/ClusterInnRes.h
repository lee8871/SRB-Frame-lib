#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "SRB-app-charger-share.h"
namespace srb {
	class ClusterInnRes : public iCluster {
	private:
		using csThis = Charger_2LiB::csInnRes;
	public:
		constexpr static uint8 DEFAULT_CID = 13;
		ClusterInnRes(Node* node);
		~ClusterInnRes();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static Json to_json;
	};
};
