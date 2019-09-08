#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class MappingCluster: public iCluster {
	private:
		using csThis = csMapping;
	public:
		MappingCluster(Node* node, uint8 cid);
		~MappingCluster();
		inline  csThis* Data() {
			return (csThis*)Data_u8();
		}
		static lee8871_support::Json to_json;
	};
};
