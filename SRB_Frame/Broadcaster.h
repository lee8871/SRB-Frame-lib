#pragma once
#include "srb_heads.h"
#include "iAccess.h"
namespace srb {
	class Broadcaster:public iAccesser {
	private:
	
	protected:
		SrbMaster* master = nullptr;
	public:
		Broadcaster(SrbMaster* m);
		~Broadcaster();
		iBus*  Bus();
		uint8 Addr() override;
		void accessDoneReply(iAccess*) override;

		int setLedAddress(uint8 cmd);

	};	
}