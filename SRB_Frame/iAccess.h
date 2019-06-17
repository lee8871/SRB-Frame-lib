#pragma once
#include "srb_heads.h"
#include <time.h>  
#include <iostream>
namespace srb {
	class Node;
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
	class Node;
	class iAccesser;
	class ostream;
	class iJsonWriter;

	class iAccess {
	protected:
		eAccessStatus _status = eAccessStatus::NoInit;
		sSrbPkg* _send_pkg = null;
		sSrbPkg* _recv_pkg = null;
		timespec _send_time;

	public:
		sSrbPkg* const& Send_pkg = _send_pkg;
		sSrbPkg* const& Recv_pkg = _recv_pkg;
		eAccessStatus const& Status = _status;
		timespec const& Send_time = _send_time;

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
			int rev = timespec_get(&_send_time,0);
			if (rev == 0) {
				rev = -1;
			}
		}
		int sendJson(iJsonWriter & record);
	};
	//不需要让节点的Mapping或者cluster等实际的接收者成为iAccesser
	//因为节点需要更新存在情况
	class iAccesser {
		public:
		virtual uint8 Addr() = 0;
		virtual void accessDone(iAccess* a) = 0;		
	};
}
