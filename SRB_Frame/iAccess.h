#pragma once
#include "srb_heads.h"
#include "SRB-share-const.h"
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
	class iJsonWriter;

	class iAccess {
	protected:
		eAccessStatus _status = eAccessStatus::Initing;
		sSrbPkg* _send_pkg = nullptr;
		sSrbPkg* _recv_pkg = nullptr;
		long long _send_time;

	public:
		sSrbPkg* const& Send_pkg = _send_pkg;
		sSrbPkg* const& Recv_pkg = _recv_pkg;
		eAccessStatus const& Status = _status;
		long long const& Send_time = _send_time;


	public:
		iAccesser* owner = nullptr;
		int cancle();
		bool isStatusFinish();
		void recordSendTime(void);
		int sendJson(iJsonWriter & record);
		virtual const char* getType();
		
	};
	//����Ҫ�ýڵ��Mapping����cluster��ʵ�ʵĽ����߳�ΪiAccesser
	//��Ϊ�ڵ���Ҫ���´������
	class iAccesser {
		public:
		virtual uint8 Addr() = 0;
		virtual void accessDone(iAccess* a) = 0;		
	};
}
