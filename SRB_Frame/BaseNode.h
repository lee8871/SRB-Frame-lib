#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "UsbAccess.h"
#include "iBus.h"
namespace srb {
	class Access;
	class iBus;
	class BaseNode{
	private:
	
	protected:
		sMapping* mapping[4] = {null};
		//iCluster* clu[100];

	public:
		uint8 rs_data[128] = {0};
		BaseNode();
		~BaseNode();
		uint8 getAddr();
		uint8 addr;
		void sendDone(Access* a);
		//void sendFail(Access* a);
		bool BaseNode::setMapping(const uint8* map, int map_num);
		bool BaseNode::sendAccess(int port);
		iBus* bus;
	};
}