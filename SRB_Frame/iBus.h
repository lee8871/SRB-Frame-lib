#pragma once
#include "UsbAccess.h"
namespace srb {
	class iBus{//����SRB�ӿ���
	private:

	public:
		//
		virtual bool addAccess(UsbAccess * access_pa) = 0;
		virtual int doAccess() = 0;
		virtual int getAccessQueueLen() = 0;
	};
}