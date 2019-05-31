#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "UsbAccess.h"
namespace srb {

	class UsbAccess;
	class BaseNode{
	private:
	
	protected:
		uint8 rs_data[128];
		sMapping* mapping[4];
		iCluster* clu[100];

	public:
		BaseNode();
		~BaseNode();
		uint8 getAddr();
		uint8 addr;
		bool updateAccess(int port);
		void sendDone(UsbAccess a);
		void sendFail(UsbAccess a);
	};
}