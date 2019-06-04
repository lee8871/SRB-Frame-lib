#pragma once
#include "srb_heads.h"
namespace srb {
	class iCluster {
	private:
		uint8 _buffer[30];
	protected:
		BaseNode* node = null;
		uint8 cluster_id = 0;
	public:
		uint8* readonly buffer = _buffer;
		uint8 _write_temp[30];


		iCluster(BaseNode* n);
		void loadReadPkg(iAccess* a);
		void readDone(iAccess* a);
		void write(iAccess* a);
		void writeDone(iAccess* a);
	};



}
