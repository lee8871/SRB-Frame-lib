#pragma once
#include "iAccess.h"
#include "SRB-base-cluster-share.h"
namespace srb {
	class SrbMaster;
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