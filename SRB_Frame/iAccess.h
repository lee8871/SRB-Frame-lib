#pragma once
#include "srb_heads.h"
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
		eAccessStatus _status = eAccessStatus::NoInit;
		sSrbPkg* _send_pkg = null;
		sSrbPkg* _recv_pkg = null;
	public:
		BaseNode* node = null;

		sSrbPkg* readonly Send_pkg = _send_pkg;
		sSrbPkg* readonly Recv_pkg = _recv_pkg;
		eAccessStatus readonly Status = _status;

		inline bool isStatusFinish() { return (_status >= eAccessStatus::RecvedDone); };
	};
}
