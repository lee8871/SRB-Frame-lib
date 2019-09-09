#pragma once

#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class InfoCluster: public iCluster {
	private:
		using csThis = csInfo;
	public:
		static const uint8 DEFAULT_CID = 1;
		InfoCluster(Node* node);
		~InfoCluster();
		inline  csThis* Data() {
			return (csThis*)Data_u8();
		}
		static lee8871_support::Json to_json;
	};
};
