#pragma once
#include "srb_heads.h"
#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class BaseCluster: public iCluster {
	private:
		using csThis = csBase;
	public:
		static const uint8 DEFAULT_CID = 0;
		BaseCluster(Node* node, uint8 addr);
		~BaseCluster();
		inline csThis* Data() {
			return (csThis*)Data_u8();
		}
		int addressLedSet(iAccess* acs, uint8 cmd);
		static Json to_json;
	};
};