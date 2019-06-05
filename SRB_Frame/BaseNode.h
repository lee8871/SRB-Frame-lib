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
		bool _is_node_exsist = false;
	public:
		uint8 rs_data[128] = {0};
		BaseNode(uint8 a, Master* m);
		~BaseNode();
		void sendDone(iAccess* a);
		//void sendFail(Access* a);
		int BaseNode::setMapping(const uint8* map, int map_num);
		int BaseNode::sendAccess(int port);
		iBus* readonly Bus;
		uint8 readonly Addr;
		bool readonly Exsist = _is_node_exsist;
		const char* readonly Node_name;
	};	
}