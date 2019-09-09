#pragma once
#include "SRB-share-const.h"
#include "OsSupport.h"
namespace lee8871_support {
	class iJsonWriter;
}
namespace srb {
	class Node;
/***********************************************************************
	NoInit  +->	NoSend	    +->	SendWaitRecv		+-> RecvedDone
			+->	SendFail    +-> SoftwareTimeout		+->	BusTimeOut
			+->	Cancle  							+->	RecvedBadPkg
*************************************************************************/
	enum class eAccessStatus {
		Initing,
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
	class iAccess;
	using accessDeleteCB_t = int (*)(iAccess*);
	class iAccess {
	protected:
		eAccessStatus _status = eAccessStatus::Initing;
		sSrbPkg* _send_pkg = nullptr;
		sSrbPkg* _recv_pkg = nullptr;
		tUs _send_time = 0;
		iAccess() = default;

	public:
		virtual ~iAccess() = default;
		sSrbPkg* const& Send_pkg = _send_pkg;
		sSrbPkg* const& Recv_pkg = _recv_pkg;
		eAccessStatus const& Status = _status;
		lee8871_support::tUs const& Send_time = _send_time;

		iAccess(const iAccess&) = delete;
		iAccess& operator=(const iAccess&)=delete;

	public:
		iAccesser* owner = nullptr;
		int cancle();
		bool isStatusFinish();


		int initDone();
		int recordSendTime(void);
		int timeoutAccess();

		int sendJson(iJsonWriter & record);
		
	};
	//不需要让节点的Mapping或者cluster等实际的接收者成为iAccesser
	//因为节点需要更新存在情况
	class iAccesser {
		public:
		virtual uint8 Addr() = 0;
		virtual void accessDoneReply(iAccess* a) = 0;		
	};
}
