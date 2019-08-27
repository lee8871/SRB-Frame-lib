#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class MappingCluster: public iCluster {
	public:
		MappingCluster(Node* node, uint8 cid);
		~MappingCluster();
		const csMapping* Data();
		csMapping* Buffer();
		virtual int toJson(iJsonWriter & json_printer)override;
		static Json to_json;
	};
};
