#include <string.h>
#include "srb.in.h"

#include "Node.h"
#include "iBus.h"
#include "SrbMaster.h"
#include "cLogger.h"
#include "Json.h"

#include "BaseCluster.h"
#include "InfoCluster.h"
#include "ErrorCluster.h"
#include "MappingCluster.h"

#include "./Nodes/MotorX2/NodeMotorX2.h"
#include "./Nodes/LiBatX2/NodeLiBatX2.h"
#include "./Nodes/Ps2Handle/NodePs2Handle.h"


using namespace lee8871_support;
namespace srb {

	//TODO: how to throw error
	Node::Node(uint8 address, SrbMaster* master){
		this->master = master;
		baseCLU() = new BaseCluster(this, address);
		iAccess* acs = Bus()->newAccess(this);
		baseCLU()->loadReadPkg(acs);
		Bus()->loadAccess(acs);
		Bus()->doAccess();
		to_json = new Json();
		if (Exsist) {
			initCluster();
			expand();
		}
	}	
	int Node::expand() {
		if (strcmp(Node_type(), NodeMotorX2::NODE_TYPE) == 0) {
			to_json->cloneTransport(*static_cast<NodeMotorX2*>(this)->finalToJson(), this);
			static_cast<NodeMotorX2*>(this)->initFormNode();
			writeAllNode = NodeMotorX2::writeAllNode;
			return done;
		}
		else if (strcmp(Node_type(), NodeLibatx2::NODE_TYPE) == 0) {
			to_json->cloneTransport(*static_cast<NodeLibatx2*>(this)->finalToJson(), this);
			static_cast<NodeLibatx2*>(this)->initFormNode();
			writeAllNode = NodeLibatx2::writeAllNode;
			return done;
		}
		else if (strcmp(Node_type(), NodePs2Handle::NODE_TYPE) == 0) {
			to_json->cloneTransport(*static_cast<NodePs2Handle*>(this)->finalToJson(), this);
			static_cast<NodePs2Handle*>(this)->initFormNode();
			writeAllNode = NodePs2Handle::writeAllNode;
			return done;
		}
		else {
			to_json->cloneTransport(*static_cast<UnknowNode*>(this)->finalToJson(), this);
			static_cast<UnknowNode*>(this)->initFormNode();
			writeAllNode = UnknowNode::writeAllNode;
			return done;

		}
	}


	int Node::initCluster() {
		infoCLU() = new InfoCluster(this);
		errorCLU() = new ErrorCluster(this);
		mapping0CLU() = new MappingCluster(this, 3);

		iAccess* acs;
		acs = Bus()->newAccess(this);
		infoCLU()->loadReadPkg(acs);
		Bus()->loadAccess(acs);
		acs = Bus()->newAccess(this);
		errorCLU()->loadReadPkg(acs);
		Bus()->loadAccess(acs);
		acs = Bus()->newAccess(this);
		mapping0CLU()->loadReadPkg(acs);
		Bus()->loadAccess(acs);

		Bus()->doAccess();
		return done;
	}



	Node::~Node(){
		delete to_json;
		for (int i = 0;i < 4;i++) {
			if (mapping[i] != nullptr) {
				delete (mapping[i]);
				mapping[i] = nullptr;
			}
		}
		for (int i = 0;i < MAX_CLUSTER_NUMBER;i++) {
			if (clusters[i] != nullptr) {
				delete (clusters[i]);
				clusters[i] = nullptr;
			}
		}
	}
	int Node::setMapping(const uint8* map, int port){
		if ((port < 0) || (port > 3)) {
			return argument_error;
		}
		sMapping* map_to_set = (sMapping*)map;
		int len = map_to_set->up_len + map_to_set->down_len + 2;
		uint8* map_copy = new uint8[len];
		if(map_copy==nullptr){
			return new_fail;
		}
		for (int i = 0; i < len;i++){
			map_copy[i] = map[i];
		}
		mapping[port] = (sMapping*)map_copy;
		return done;
	}

	int Node::sendAccess(int port) {
		if ((port < 0) || (port > 3)) {
			return argument_error;
		}
		iAccess* acs = Bus()->newAccess(this);
		int i,map;
		int max_len = mapping[port]->down_len;
		for (i = 0;i < max_len;i++) {
			map = mapping[port]->table[i + mapping[port]->up_len];
			acs->Send_pkg->data[i] = data_rs[map];
		}
		acs->Send_pkg->bfc.length = i;
		acs->Send_pkg->bfc.port = port;
		Bus()->loadAccess(acs);
		return done;
	}
	int Node::dataAccessDone(iAccess* acs) {
		int port = acs->Send_pkg->bfc.port;
		int i, map;
		int max_len = mapping[port]->up_len;
		if (max_len > acs->Recv_pkg->bfc.length) {
			max_len = acs->Recv_pkg->bfc.length;
		}
		for (i = 0;i < max_len;i++) {
			map = mapping[port]->table[i];
			data_rs[map] = acs->Recv_pkg->data[i];
		}
		return 0;
	}


	void Node::accessDoneReply(iAccess * acs)	{	
		if (acs->Status == eAccessStatus::RecvedDone) {
			_exsist = true;
			if (acs->Recv_pkg->bfc.error == yes) {
				Srb_log.addLog(eLogLevel::erro, "access recv bfc.error IS yes, may node error or send data error.");
			}
			else {
				switch (acs->Send_pkg->bfc.port) {
				case SC_PORT_D0:
				case SC_PORT_D1:
				case SC_PORT_D2:
				case SC_PORT_D3:
					dataAccessDone(acs);
					break;
				case SC_PORT_CFG:
					//TODO Note this if cluster is not exsist
					clusters[acs->Send_pkg->data[0]]->accessDone(acs);
					break;
				default:
					break;
				}
			}
		}
		else if (acs->Status == eAccessStatus::BusTimeOut) {
			_exsist = false;
		}
		else {
			Srb_log.addLog(eLogLevel::erro, "accessDoneReply shold call when status is ReceiveDone or BusTimeOut ");
		}
		delete acs;
	}

	void Node::changeName(const char * name) {
		char * to = (char*)(baseCLU()->Data()->name);
		for (int i = 0;i < 27;i++) {
			to[i] = name[i];
			if (to[i] == 0) {
				break;
			}
		}
		iAccess* acs = Bus()->newAccess(this);
		baseCLU()->loadWritePkg(acs);
		Bus()->loadAccess(acs);
		Bus()->doAccess();
	}

	void Node::changeAddr(unsigned char new_addr)	{
		baseCLU()->Data()->addr = new_addr;
		iAccess* acs = Bus()->newAccess(this);
		baseCLU()->loadWritePkg(acs);
		Bus()->loadAccess(acs);
		Bus()->doAccess();
	}



	iBus * Node::Bus() {
		return this->master->Bus();
	}

	uint8 Node::Addr() {
		return this->baseCLU()->Data()->addr;
	}

	const char * Node::Node_name() {
		return (const char *)this->baseCLU()->Data()->name;
	}

	const char * Node::Node_type()	{
		return (const char *)this->infoCLU()->Data()->node_type;
	}
	
	int UnknowNode::initFormNode() {
		return done;
	}

	int UnknowNode::writeAllNode(Node *)
	{
		return 0;
	}

	static Json* local_to_json = nullptr;
#define relTo(value) (((UnknowNode*)0)->value)
	Json* UnknowNode::finalToJson() {
		if (local_to_json == nullptr) {
			local_to_json = new Json{
				{"baseCLU",buildJsonPtr(BaseCluster::to_json, &relTo(baseCLU()))},
				{"infoCLU",buildJsonPtr(InfoCluster::to_json, &relTo(infoCLU()))},
				{"errorCLU",buildJsonPtr(ErrorCluster::to_json, &relTo(errorCLU()))},
				{"mapping0CLU",buildJsonPtr(MappingCluster::to_json, &relTo(mapping0CLU()))}
			};
		}
		return local_to_json;
	}

}

