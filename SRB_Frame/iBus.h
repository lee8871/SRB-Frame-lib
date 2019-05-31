#pragma once
#include "UsbAccess.h"
namespace srb {
	class Access;
	class iBus{//这是SRB接口类
	private:

	public:
		//
		virtual bool addAccess(Access * access_pa) = 0;
		virtual int doAccess() = 0;
		virtual int getAccessQueueLen() = 0;
	};
}