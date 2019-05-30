#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "SRB-base-cluster-share.h"
#include "BaseNode.h"
namespace srb {
	class BaseNode;


/*
	NoInit  ->	NoSend ->	SendWaitRecv		+-> RecvedDone
				|			|					+->	BusTimeOut
				SendFail	SoftwareTimeout		+->	RecvedBadPkg
*/
	enum class eAccessStatus {
		NoInit,
		NoSend, 
		SendWaitRecv, 
		RecvedDone, 
		BusTimeOut, RecvedBadPkg,

		SendFail,
		SoftwareTimeout,
	};
	class BaseNode;
	class UsbAccess {
	protected:
		BaseNode* node =null;
		eAccessStatus status = eAccessStatus::NoInit;
		sUsbToSrbPkg* usb_send_pkg = null;
		sUsbToSrbPkg* usb_recv_pkg = null;
	public:
		sSrbPkg* getSendPkg();
		sSrbPkg* getRecvPkg();
		bool getUsbSendPkg(sUsbToSrbPkg** pkg, int* len);
		bool setUsbRecvPkg(sUsbToSrbPkg* pkg, int len);
		bool timeoutAccess();
		uint8 getAddr();
		uint8 getSno();

		void recvBadPkg();

		eAccessStatus getStatus();

		UsbAccess(BaseNode*);
		~UsbAccess();

		inline bool isStatusFinish() {
			return (status >= eAccessStatus::RecvedDone);
		}
	};

}
