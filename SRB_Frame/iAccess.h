#pragma once
#include "srb_heads.h"
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
	class iAccesser;

	class iAccess {
	protected:
		eAccessStatus _status = eAccessStatus::NoInit;
		sSrbPkg* _send_pkg = null;
		sSrbPkg* _recv_pkg = null;
	public:
		sSrbPkg* const& Send_pkg = _send_pkg;
		sSrbPkg* const& Recv_pkg = _recv_pkg;
		eAccessStatus const& Status = _status;

	public:

		iAccesser* owner = null;
		inline bool isStatusFinish() { return (_status >= eAccessStatus::RecvedDone); };

	};
	//TODO: 让节点的Mapping或者cluster等实际的接收者成为iAccesser
	class iAccesser {
		public:
		virtual uint8 Addr() = 0;
		virtual void accessDone(iAccess* a) = 0;		
	};
}
