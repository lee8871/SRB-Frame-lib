#pragma once
#include "srb_heads.h"
#include "iAccess.h"
namespace srb {
	class Broadcaster:public iAccesser {
	private:
	
	protected:
		Master* master = null;
	public:
		Broadcaster(Master* m);
		~Broadcaster();
		iBus*  Bus();
		uint8 Addr() override;
		void accessDone(iAccess*) override;

		int setLedAddress(uint8 cmd);

	};	
}