#pragma once
#include "lee.h"
#include "SRB-share-const.h"
#include "BaseNode.h"
namespace srb {
	class BaseNode;
/***********************************************************************
	NoInit  +->	NoSend	    +->	SendWaitRecv		+-> RecvedDone
			+->	SendFail    +-> SoftwareTimeout		+->	BusTimeOut
													+->	RecvedBadPkg
*************************************************************************/
	enum class eAccessStatus {
		NoInit,
		NoSend, 
		SendWaitRecv, 
		RecvedDone, 
		BusTimeOut, 
		RecvedBadPkg,
		SendFail,
		SoftwareTimeout,
	};
	class BaseNode;

	class iAccess {
	protected:
		eAccessStatus status = eAccessStatus::NoInit;
	public:
		BaseNode* node = null;
		sSrbPkg* send_pkg = null;
		sSrbPkg* recv_pkg = null;
		inline eAccessStatus getStatus() { return status; }
		inline bool isStatusFinish() { return (status >= eAccessStatus::RecvedDone); };
	};
}
