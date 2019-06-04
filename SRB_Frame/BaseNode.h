#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "iAccess.h"
#include "iBus.h"
#include "iCluster.h"
#include "Master.h"




namespace srb {
	class iAccess;
	class iBus;
	class iCluster;
	class Master;
	extern class BaseCluster;
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

		
		inline iBus* get_bus();
	};
}