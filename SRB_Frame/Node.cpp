#include "Node.h"
#include "BaseCluster.h"
#include "InfoCluster.h"
#include "ErrorCluster.h"
#include "MappingCluster.h"
#include "iBus.h"
#include "Master.h"
#include "iJsonWriter.h"

#include "iExpandNode.h"
#include "./Nodes/dumotor/DumotorNode.h"
#include <string.h>

namespace srb {

	//TODO: how to throw error
	Node::Node(uint8 address, Master* master){
		this->master = master;
		clusters[0] = baseCLU = new BaseCluster(this, address);
		iAccess* acs = Bus()->newAccess(this);
		baseCLU->loadReadPkg(acs);
		Bus()->doAccess();
		if (Exsist) {
			initCluster();
		}
	}


	int Node::initCluster() {
		clusters[1] = infoCLU = new InfoCluster(this);
		clusters[2] = errorCLU = new ErrorCluster(this);
		clusters[3] = mappingCLU = new MappingCluster(this, 3);

		iAccess* acs;
		acs = Bus()->newAccess(this);
		infoCLU->loadReadPkg(acs);
		acs = Bus()->newAccess(this);
		errorCLU->loadReadPkg(acs);
		acs = Bus()->newAccess(this);
		mappingCLU->loadReadPkg(acs);
		Bus()->doAccess();

		if (strcmp((char*)(infoCLU->Data()->node_type), DumotorNode::Node_type) ==0){
			_expand_node = new DumotorNode(this);
		}
		return done;
	}


	Node::~Node(){
		if (_expand_node != null) {
			if (strcmp(Node_type(), DumotorNode::Node_type) == 0) {
				 delete (DumotorNode*)_expand_node;
			}
			else {
				delete _expand_node;
			}
			_expand_node = null;
		}
		for (int i = 0;i < 4;i++) {
			if (mapping[i] != null) {
				delete (mapping[i]);
				mapping[i] = null;
			}
		}
		for (int i = 0;i < MAX_CLUSTER_NUMBER;i++) {
			if (clusters[i] != null) {
				delete (clusters[i]);
				clusters[i] = null;
			}
		}
	}
	int Node::setMapping(const uint8* map, int port){
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

	int Node::sendAccess(int port) {
		if ((port < 0) || (port > 3)) {
			return par_error;
		}
		iAccess* acs = Bus()->newAccess(this);
		int i;
		for (i = 0;i < mapping[port]->down_len;i++) {
			acs->Send_pkg->data[i] = data_rs[mapping[port]->table[i]];
		}
		acs->Send_pkg->bfc.length = i;
		acs->Send_pkg->bfc.port = port;
		return done;
	}


	void Node::accessDone(iAccess * acs)	{	
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
				clusters[acs->Send_pkg->data[0]]->readDone(acs);
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
	int Node::toJsonAll(iJsonWriter & json_printer)	{
		json_printer.beginObj("untyped_node");
		for (int i = 0;i < MAX_CLUSTER_NUMBER;i++) {
			if (clusters[i] != null) {
				clusters[i]->toJson(json_printer);
				json_printer.writeEndLine();
			}
		}
		json_printer.endObj();
		return done;
	}

	

	iBus * Node::Bus() {
		return this->master->Bus();
	}

	uint8 Node::Addr() {
		return this->baseCLU->Data()->addr;
	}

	const char * Node::Node_name() {
		return (const char *)this->baseCLU->Data()->name;
	}

	const char * Node::Node_type()
	{
		return (const char *)this->infoCLU->Data()->node_type;
	}

}
