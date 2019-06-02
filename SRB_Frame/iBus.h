#pragma once
#include "iAccess.h"
namespace srb {
	class iAccess;
	class BaseNode;
	class iBus{//这是SRB接口类
	private:

	public:
		//
		virtual bool addAccess(iAccess * access_pa) = 0;
		virtual int doAccess() = 0;
		virtual int getAccessQueueLen() = 0;
		virtual iAccess* newAccess(BaseNode * node) = 0;
	};
}