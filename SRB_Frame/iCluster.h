#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "SRB-base-cluster-share.h"
#include "BaseNode.h"
#include "iAccess.h"


namespace srb {
	class BaseNode;
	class iAccess;
	class iCluster {
	protected:
		BaseNode* node;
		uint8 cluster_id;
		uint8 buffer[30];
	public:
		iCluster(BaseNode* n);
		void loadReadPkg(iAccess* a);
		void readDone(iAccess* a);
		void write(iAccess* a);
		void writeDone(iAccess* a);

	};



}
