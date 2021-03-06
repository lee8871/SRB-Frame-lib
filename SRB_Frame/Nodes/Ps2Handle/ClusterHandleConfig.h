#pragma once

#include "iCluster.h"
#include "SRB-app-ps2handle-share.h"
namespace srb {
	class ClusterHandleConfig: public iCluster {
	private:
		using csThis = Joystick::csHandleConfig;
	public:
		constexpr static uint8 DEFAULT_CID = 11;
		ClusterHandleConfig(Node* node);
		~ClusterHandleConfig();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		static  lee8871_support::Json to_json;
	};
};
