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
		sSrbPkg* _send_pkg = null;
		sSrbPkg* _recv_pkg = null;
	public:
		BaseNode* node = null;

		inline sSrbPkg* send_pkg() { return _send_pkg; }
		inline sSrbPkg* recv_pkg() { return _recv_pkg; }
		inline eAccessStatus getStatus() { return status; }
		inline bool isStatusFinish() { return (status >= eAccessStatus::RecvedDone); };
	};
}
