#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"

namespace srb {
	class ErrorCluster: public iCluster {
	public:
		static const uint8 DEFAULT_CID = 2;
		ErrorCluster(Node* node, uint8 cid = DEFAULT_CID);
		~ErrorCluster();
		const csError* Data();
		csError* Buffer();
		virtual int toJson(iJsonWriter & json_printer)override;
	};
};
