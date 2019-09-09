#pragma once
#include "iCluster.h"
#include "iAccess.h"
#include "SRB-base-cluster-share.h"

namespace srb {
	class ErrorCluster: public iCluster {
	private:
		using csThis = csError;
	public:
		static const uint8 DEFAULT_CID = 2;
		ErrorCluster(Node* node);
		~ErrorCluster();
		inline  csThis* Data() {
			return (csThis*)Data_u8();
		}

		static lee8871_support::Json to_json;


	};
};
