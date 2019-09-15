#pragma once
#include "SRB-share-const.h"
#include "iAccess.h"

namespace lee8871_support {
	class Json;
};

namespace srb {
	class BaseCluster;
	class InfoCluster;
	class ErrorCluster;
	class MappingCluster;

	class SrbMaster;
	class Node;
	class iBus;
	class iCluster;
	class iAccess;

	class UnknowNode;

	class Node:public iAccesser {
	private:
		bool _exsist = false;
		int initCluster();
		int expand(); 
	protected:
		sMapping* mapping[4] = { nullptr };
		iCluster* clusters[MAX_CLUSTER_NUMBER] = { nullptr };
		SrbMaster* master = nullptr;
	public:

		inline BaseCluster*& baseCLU() { return (BaseCluster*&)clusters[0]; }
		inline InfoCluster*& infoCLU() { return (InfoCluster*&)clusters[1]; }
		inline ErrorCluster*& errorCLU() { return (ErrorCluster*&)clusters[2]; }
		inline MappingCluster*& mapping0CLU() { return (MappingCluster*&)clusters[3]; }

		bool const& Exsist = _exsist;
		iBus* Bus();
		const char* Node_name();
		const char* Node_type();		
		uint8 Addr() override;
	public:
		uint8 data_rs[128] = {0};
		Node(uint8 a, SrbMaster* m);
		~Node();
		int setMapping(const uint8* map, int map_num);
		int sendAccess(int port);
		int dataAccessDone(iAccess*);
		void accessDoneReply(iAccess*) override;
		lee8871_support::Json* to_json;
		void changeName(const char* name);
		void changeAddr(unsigned char addr);
		int (*writeAllNode)(Node *n);
	};


	class UnknowNode:public Node{
	public:
		UnknowNode() = delete;
		UnknowNode(const UnknowNode&) = delete;
		UnknowNode(UnknowNode&&) = delete;
		constexpr static const char* NODE_TYPE = "Unknow";
		lee8871_support::Json* finalToJson();
		int initFormNode();
		static int writeAllNode(Node*);
	};	
}