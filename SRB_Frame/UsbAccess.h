#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "SRB-base-cluster-share.h"
#include "BaseNode.h"
namespace srb {
	class BaseNode;



//	NoInit  +->	NoSend	    +->	SendWaitRecv		+-> RecvedDone
//			+->	SendFail    +-> SoftwareTimeout		+->	BusTimeOut
//													+->	RecvedBadPkg
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
	class Access {
	protected:
		eAccessStatus status = eAccessStatus::NoInit;
		
	public :
		BaseNode* node = null;
		virtual sSrbPkg* getSendPkg() = 0;
		virtual sSrbPkg* getRecvPkg() = 0;

	};


	class UsbAccess: public Access {
	protected:
		sUsbToSrbPkg* usb_send_pkg = null;
		sUsbToSrbPkg* usb_recv_pkg = null;
	public:
		bool getUsbSendPkg(sUsbToSrbPkg** pkg, int* len);
		bool setUsbRecvPkg(sUsbToSrbPkg* pkg, int len);
		bool timeoutAccess();
		sSrbPkg* getSendPkg();
		sSrbPkg* getRecvPkg();

		eAccessStatus getStatus();

		UsbAccess(BaseNode*);
		~UsbAccess();

		inline bool isStatusFinish() {
			return (status >= eAccessStatus::RecvedDone);
		}
	};

}
