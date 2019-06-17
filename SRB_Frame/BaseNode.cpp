#include "BaseNode.h"
#include "BaseNode.h"
#include "BaseCluster.h"
#include "infoCluster.h"
#include "ErrorCluster.h"
#include "iBus.h"
#include "Master.h"

#include "iJsonWriter.h"

namespace srb {

	//TODO: how to throw error
	BaseNode::BaseNode(uint8 address, Master* master){
		this->master = master;
		clu[0] = baseCLU = new BaseCluster(this, address);
		clu[1] = infoCLU = new InfoCluster(this);
		clu[2] = errorCLU = new ErrorCluster(this);
		iAccess* acs = Bus()->newAccess(this);
		baseCLU->loadReadPkg(acs);
		Bus()->doAccess();
		if (Exsist) {
			acs = Bus()->newAccess(this);
			infoCLU->loadReadPkg(acs);
			acs = Bus()->newAccess(this);
			errorCLU->loadReadPkg(acs);
			Bus()->doAccess();
		}
	}

	BaseNode::~BaseNode(){
		for (int i = 0;i < 4;i++) {
			if (mapping[i] != null) {
				delete (mapping[i]);
				mapping[i] = null;
			}
		}
		for (int i = 0;i < MAX_CLUSTER_NUMBER;i++) {
			if (clu[i] != null) {
				delete (clu[i]);
				clu[i] = null;
			}
		}
	}
	int BaseNode::setMapping(const uint8* map, int port){
		if ((port < 0) || (port > 3)) {
			return par_error;
		}
		sMapping* map_to_set = (sMapping*)map;
		int len = map_to_set->up_len + map_to_set->down_len + 2;
		uint8* map_copy = new uint8[len];
		if(map_copy==null){
			return no_memory;
		}
		for (int i = 0; i < len;i++){
			map_copy[i] = map[i];
		}
		mapping[port] = (sMapping*)map_copy;
		return done;
	}

	int BaseNode::sendAccess(int port) {
		if ((port < 0) || (port > 3)) {
			return par_error;
		}
		iAccess* acs = Bus()->newAccess(this);
		int i;
		for (i = 0;i < mapping[port]->down_len;i++) {
			acs->Send_pkg->data[i] = rs_data[mapping[port]->table[i]];
		}
		acs->Send_pkg->bfc.length = i;
		acs->Send_pkg->bfc.port = port;
		return done;
	}


	void BaseNode::accessDone(iAccess * acs)	{	
		if (acs->Status == eAccessStatus::RecvedDone) {
			_exsist = true;
			if (acs->Recv_pkg->bfc.error == yes) {
				throw  "receive error package";
			}
			switch (acs->Send_pkg->bfc.port) {
			case SC_PORT_D0:
			case SC_PORT_D1:
			case SC_PORT_D2:
			case SC_PORT_D3:
				break;
			case SC_PORT_CFG:
				clu[acs->Send_pkg->data[0]]->readDone(acs);
				break;
			default:
				break;
			}
		}
		else if (acs->Status == eAccessStatus::BusTimeOut) {
			_exsist = false;
		}
		else {
			throw "receive bad package";
		}
	}
	int BaseNode::toJsonAll(iJsonWriter & json_printer)	{

		json_printer.beginObj("untyped_node");
		for (int i = 0;i < MAX_CLUSTER_NUMBER;i++) {
			if (clu[i] != null) {
				clu[i]->toJson(json_printer);
				json_printer.writeEndLine();
			}
		}
		json_printer.endObj();
		return done;
	}

	
	iBus * BaseNode::Bus() {
		return this->master->Bus();
	}

	uint8 BaseNode::Addr() {
		return this->baseCLU->Data()->addr;
	}

	const char * BaseNode::Node_name() {
		return (const char *)this->baseCLU->Data()->name;
	}

}
