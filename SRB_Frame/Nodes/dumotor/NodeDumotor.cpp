#include "srb.in.h"
#include "Node.h"

#include "NodeDumotor.h"

#include "BaseCluster.h"
#include "InfoCluster.h"
#include "ErrorCluster.h"
#include "MappingCluster.h"
#include "iCluster.h"
#include "iBus.h"
#include "ClusterMotorAdj.h"
#include "ClusterMotorSet.h"
#include "Json.h"

using namespace lee8871_support;
namespace srb {

	int DumotorNode::initFormNode()
	{
		if (setCLU() != nullptr) {
			Srb_log.addLog(eLogLevel::erro, "Cluster[ID=%d] is reinitializing to setCLU", setCLU()->DEFAULT_CID);
			delete setCLU();
		}		
		setCLU() = new MotorSetCluster(this);		
		auto acs = Bus()->newAccess(this);
		setCLU()->loadReadPkg(acs);
		Bus()->loadAccess(acs);
		if (adjCLU() != nullptr) {
			Srb_log.addLog(eLogLevel::erro, "Cluster[ID=%d] is reinitializing to adjCLU", adjCLU()->DEFAULT_CID);
			delete adjCLU();
		}
		adjCLU() = new MotorAdjCluster(this);

		acs = Bus()->newAccess(this);
		adjCLU()->loadReadPkg(acs);
		Bus()->loadAccess(acs);
		
		Bus()->doAccess();
		setMapping(mapping0CLU()->Data()->u8, 0);
		setMapping(Du_Motor::Mapping1, 1);
		setMapping(Du_Motor::Mapping2, 2);
		setMapping(Du_Motor::Mapping2, 2);
		setMapping(Du_Motor::Mapping3, 1);
		return done;
	}

	int DumotorNode::writeAllNode(Node *n)	{
		return static_cast<DumotorNode*>(n)->__writeAllNode();
	}

	int DumotorNode::__writeAllNode()
	{
		iAccess* acs = Bus()->newAccess(this);
		mapping0CLU()->loadWritePkg(acs);
		Bus()->loadAccess(acs);
		acs = Bus()->newAccess(this);
		adjCLU()->loadWritePkg(acs);
		Bus()->loadAccess(acs);
		acs = Bus()->newAccess(this);
		setCLU()->loadWritePkg(acs);
		Bus()->loadAccess(acs);
		Bus()->doAccess();
		return 0;
	}



	static Json* local_to_json = nullptr;
#define relTo(value) (((DumotorNode*)0)->value)
	Json* DumotorNode::finalToJson() {
		if (local_to_json == nullptr) {
			local_to_json = new Json{
				//{"baseCLU",buildJsonPtr(BaseCluster::to_json, &relTo(baseCLU()))},
				//{"infoCLU",buildJsonPtr(InfoCluster::to_json, &relTo(infoCLU()))},
				//{"errorCLU",buildJsonPtr(ErrorCluster::to_json, &relTo(errorCLU()))},
				{"mapping0CLU",buildJsonPtr(MappingCluster::to_json, &relTo(mapping0CLU()))},
				{"MotorAdj",buildJsonPtr(MotorAdjCluster::to_json, &relTo(adjCLU()))},
				{"MotorSet",buildJsonPtr(MotorSetCluster::to_json, &relTo(setCLU()))}
			};
		}
		return local_to_json;
	}


}
