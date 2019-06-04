#pragma once
#include "srb_heads.h"
namespace srb {
	class BaseNode{
	private:
	
	protected:
		sMapping* mapping[4] = {null};
		iCluster* clu[MAX_CLUSTER_NUMBER] = { null };
		Master* master = null;
		BaseCluster * baseCLU = null;
	public:
		uint8 addr;
		uint8 rs_data[128] = {0};
		BaseNode(uint8 a, Master* m);
		~BaseNode();
		uint8 getAddr();
		void sendDone(iAccess* a);
		//void sendFail(Access* a);
		bool BaseNode::setMapping(const uint8* map, int map_num);
		bool BaseNode::sendAccess(int port);
		iBus* readonly Bus;
	};
	
}