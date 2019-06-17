#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class InfoCluster: public iCluster {
	public:
		static const uint8 DEFAULT_CID = 1;
		InfoCluster(Node* node, uint8 cid = DEFAULT_CID);
		~InfoCluster();
		const csInfo* Data();
		csInfo* Buffer();
		virtual int toJson(iJsonWriter & json_printer)override;
	};
};
