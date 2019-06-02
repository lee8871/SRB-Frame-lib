#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "iAccess.h"
#include "iBus.h"
#include "iCluster.h"
namespace srb {
	class iAccess;
	class iBus;
	class iCluster;
	class BaseNode{
	private:
	
	protected:
		sMapping* mapping[4] = {null};
		iCluster* clu[MAX_CLUSTER_NUMBER];

	public:
		uint8 addr;
		uint8 rs_data[128] = {0};
		BaseNode(uint8 address);
		~BaseNode();
		uint8 getAddr();
		void sendDone(iAccess* a);
		//void sendFail(Access* a);
		bool BaseNode::setMapping(const uint8* map, int map_num);
		bool BaseNode::sendAccess(int port);

		iBus* bus;
		iBus* getBus() { return bus; }
	};
}