#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class ErrorCluster: public iCluster {
	public:
		static const uint8 DEFAULT_CID = 2;
		ErrorCluster(Node* node);
		~ErrorCluster();
		const csError* Data();
		csError* Buffer();
		virtual int toJson(iJsonWriter & json_printer)override;
		static json to_json;
	
	};
};
