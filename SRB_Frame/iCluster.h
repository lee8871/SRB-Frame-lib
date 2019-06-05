#pragma once
#include "srb_heads.h"
namespace srb {
	class iCluster {
	protected:
		uint8 _data_u8[30] = { 0 };
		BaseNode* node = null;
		uint8 cluster_id = 0;
	public:
		const uint8* readonly const data_u8;
		uint8 buffer_u8[30];

		iCluster(BaseNode* n);
		void loadReadPkg(iAccess* a);
		void readDone(iAccess* a);
		void write(iAccess* a);
		void writeDone(iAccess* a);
	};



}
