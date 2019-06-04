#pragma once
#include "srb_heads.h"


namespace srb {
	class iCluster {
	protected:
		BaseNode* node = null;
		uint8 cluster_id = 0;
		uint8 buffer[30];
	public:
		iCluster(BaseNode* n);
		void loadReadPkg(iAccess* a);
		void readDone(iAccess* a);
		void write(iAccess* a);
		void writeDone(iAccess* a);
	};



}
