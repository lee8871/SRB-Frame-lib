#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class BaseCluster: public iCluster {
	public:
		static const uint8 DEFAULT_CID = 0;
		BaseCluster(Node* node, uint8 addr);
		~BaseCluster();
		const csBase* Data();
		csBase* Buffer();
		int addressLedSet(iAccess* acs, uint8 cmd);
		virtual int toJson(iJsonWriter & json_printer)override;
		static json to_json;
	};
};

