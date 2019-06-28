#include "Broadcaster.h"
#include "iBus.h"
#include "SrbMaster.h"
#include "BaseCluster.h"

namespace srb {
	Broadcaster::Broadcaster(SrbMaster * m)	{
		this->master = m;
	}
	Broadcaster::~Broadcaster()	{

	}
	iBus * Broadcaster::Bus() {
		return this->master->Bus();
	}
	uint8 Broadcaster::Addr() {
		return 0xff;
	}
	void Broadcaster::accessDone(iAccess * acs) {

	}

	int Broadcaster::setLedAddress(uint8 cmd) {
		iAccess* acs;
		acs = Bus()->newAccess(this);
		BaseCluster* base_cluster = nullptr;
		int rev = base_cluster->addressLedSet(acs, cmd);
		Bus()->loadAccess(acs);
		Bus()->doAccess();
		return rev;
		
	}



}
