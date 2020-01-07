#include "srb.in.h"
#include "Node.h"
#include "Json.h"
#include "iCluster.h"
#include "iBus.h"

#include "BaseCluster.h"
#include "InfoCluster.h"
#include "ErrorCluster.h"
#include "MappingCluster.h"

#include "NodePs2Handle.h"

using namespace lee8871_support;
namespace srb {

	int NodePs2Handle::initFormNode()
	{
		if (configCLU() != nullptr) {
			Srb_log.addLog(eLogLevel::erro, "Cluster[ID=%d] is reinitializing to configCLU", configCLU()->DEFAULT_CID);
			delete configCLU();
		}
		configCLU() = new ClusterHandleConfig(this);
		auto acs = Bus()->newAccess(this);
		configCLU()->loadReadPkg(acs);
		Bus()->loadAccess(acs);

		Bus()->doAccess();
		setMapping(mapping0CLU()->Data()->u8, 0);
		setMapping(Joystick::Mapping1, 1);
		setMapping(Joystick::Mapping2, 2);
		setMapping(Joystick::Mapping3, 1);
		return done;
	}

	int NodePs2Handle::writeAllNode(Node *n)	{
		return static_cast<NodePs2Handle*>(n)->__writeAllNode();
	}

	int NodePs2Handle::__writeAllNode()
	{
		iAccess* acs = Bus()->newAccess(this);
		mapping0CLU()->loadWritePkg(acs);
		Bus()->loadAccess(acs);
		acs = Bus()->newAccess(this);
		configCLU()->loadWritePkg(acs);
		Bus()->loadAccess(acs);
		Bus()->doAccess();
		return 0;
	}



	static Json* local_to_json = nullptr;
#define relTo(value) (((NodePs2Handle*)0)->value)
	Json* NodePs2Handle::finalToJson() {
		if (local_to_json == nullptr) {
			local_to_json = new Json{
				{"mapping0CLU",buildJsonPtr(MappingCluster::to_json, &relTo(mapping0CLU()))},
				{"ClusterHandleConfig",buildJsonPtr(ClusterHandleConfig::to_json, &relTo(configCLU()))}
			};
		}
		return local_to_json;
	}


}
