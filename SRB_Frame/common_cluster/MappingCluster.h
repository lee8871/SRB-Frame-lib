#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class MappingCluster: public iCluster {
	public:
		static const uint8 DEFAULT_CID = 3;
		MappingCluster(Node* node, uint8 cid = DEFAULT_CID);
		~MappingCluster();
		const csMapping* Data();
		csMapping* Buffer();
		virtual int toJson(iJsonWriter & json_printer)override;
	};
};
