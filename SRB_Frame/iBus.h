#pragma once
#include "iAccess.h"
namespace srb {
	class iAccess;
	class BaseNode;
	class iBus{//����SRB�ӿ���
	private:

	public:
		//
		virtual iAccess*  newAccess(BaseNode* sender_node) = 0;
		virtual int doAccess() = 0;
		virtual int getAccessQueueLen() = 0;
	};
}