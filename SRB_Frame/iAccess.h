#pragma once
#include "srb_heads.h"
#include <time.h>     
namespace srb {
	class BaseNode;
/***********************************************************************
	NoInit  +->	NoSend	    +->	SendWaitRecv		+-> RecvedDone
			+->	SendFail    +-> SoftwareTimeout		+->	BusTimeOut
			+->	Cancle  							+->	RecvedBadPkg
*************************************************************************/
	enum class eAccessStatus {
		NoInit,
		Cancel,
		WaitSend, 
		SendWaitRecv, 
		RecvedDone, 
		BusTimeOut, 
		RecvedBadPkg,
		SendFail,
		SoftwareTimeout,
	};
	class BaseNode;
	class iAccesser;
	class ostream;

	class iAccess {
	protected:
		eAccessStatus _status = eAccessStatus::NoInit;
		sSrbPkg* _send_pkg = null;
		sSrbPkg* _recv_pkg = null;
		time_t _send_time;

	public:
		sSrbPkg* const& Send_pkg = _send_pkg;
		sSrbPkg* const& Recv_pkg = _recv_pkg;
		eAccessStatus const& Status = _status;
		time_t const& Send_time = _send_time;

	public:

		iAccesser* owner = null;
		inline bool cancle() {
			if (_status <= eAccessStatus::WaitSend) {
				_status = eAccessStatus::Cancel;
			}
		};
		inline bool isStatusFinish() {
			return (_status >= eAccessStatus::RecvedDone);
		};
		inline void recordSendTime(void) {
			time(&_send_time);
		}
		int sendJson(ostream record);
		

	};
	//TODO: 让节点的Mapping或者cluster等实际的接收者成为iAccesser
	class iAccesser {
		public:
		virtual uint8 Addr() = 0;
		virtual void accessDone(iAccess* a) = 0;		
	};
}
