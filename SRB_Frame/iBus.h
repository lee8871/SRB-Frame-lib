#pragma once
#include "UsbAccess.h"
namespace srb {
	class iBus{//这是SRB接口类
	private:

	public:
		//
		virtual bool addAccess(UsbAccess * access_pa) = 0;
		virtual int doAccess() = 0;
		virtual int getAccessQueueLen() = 0;
	};
}