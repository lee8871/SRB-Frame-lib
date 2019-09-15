#include "srb.in.h"
#include "Node.h"
#include "Json.h"
#include "iCluster.h"
#include "iBus.h"

#include "BaseCluster.h"
#include "InfoCluster.h"
#include "ErrorCluster.h"
#include "MappingCluster.h"

#include "NodeLiBatX2.h"
#include "ClusterBattery.h"
#include "ClusterBuzzer.h"
#include "ClusterInnRes.h"

namespace srb {
	using namespace Charger_2LiB;
using namespace lee8871_support;
	int NodeLibatx2::initFormNode()
	{
		if (c_battery() != nullptr) {
			Srb_log.addLog(eLogLevel::erro, "Cluster[ID=%d] is reinitializing to setCLU", c_battery()->DEFAULT_CID);
			delete c_battery();
		}		
		c_battery() = new ClusterBattery(this);
		auto acs = Bus()->newAccess(this);
		c_battery()->loadReadPkg(acs);
		Bus()->loadAccess(acs);


		if (c_buzzer() != nullptr) {
			Srb_log.addLog(eLogLevel::erro, "Cluster[ID=%d] is reinitializing to adjCLU", c_buzzer()->DEFAULT_CID);
			delete c_buzzer();
		}
		c_buzzer() = new ClusterBuzzer(this);

		acs = Bus()->newAccess(this);
		c_buzzer()->loadReadPkg(acs);
		Bus()->loadAccess(acs);

		if (c_inn_res() != nullptr) {
			Srb_log.addLog(eLogLevel::erro, "Cluster[ID=%d] is reinitializing to adjCLU", c_inn_res()->DEFAULT_CID);
			delete c_inn_res();
		}
		c_inn_res() = new ClusterInnRes(this);

		acs = Bus()->newAccess(this);
		c_inn_res()->loadReadPkg(acs);
		Bus()->loadAccess(acs);


		
		Bus()->doAccess();
		setMapping(mapping0CLU()->Data()->u8, 0);
		setMapping(Mapping1, 1);
		setMapping(Mapping2, 2);
		setMapping(Mapping3, 1);
		return done;
	}

	int NodeLibatx2::writeAllNode(Node *n)	{
		return static_cast<NodeLibatx2*>(n)->__writeAllNode();
	}

	int NodeLibatx2::__writeAllNode()
	{
		iAccess* acs = Bus()->newAccess(this);
		mapping0CLU()->loadWritePkg(acs);
		Bus()->loadAccess(acs);

		acs = Bus()->newAccess(this);
		c_battery()->loadWritePkg(acs);
		Bus()->loadAccess(acs);

		acs = Bus()->newAccess(this);
		c_buzzer()->loadWritePkg(acs);
		Bus()->loadAccess(acs);

		acs = Bus()->newAccess(this);
		c_inn_res()->loadWritePkg(acs);
		Bus()->loadAccess(acs);

		Bus()->doAccess();
		return 0;
	}



	static Json* local_to_json = nullptr;
#define relTo(value) (((NodeLibatx2*)0)->value)
	Json* NodeLibatx2::finalToJson() {
		if (local_to_json == nullptr) {
			local_to_json = new Json{
				{"mapping0CLU",buildJsonPtr(MappingCluster::to_json, &relTo(mapping0CLU()))},
				{"ClusterBuzzer",buildJsonPtr(ClusterBuzzer::to_json, &relTo(c_buzzer()))},
				{"ClusterBattery",buildJsonPtr(ClusterBattery::to_json, &relTo(c_battery()))},
				{"ClusterInnRes",buildJsonPtr(ClusterInnRes::to_json, &relTo(c_inn_res()))}
			};
		}
		return local_to_json;
	}


}
