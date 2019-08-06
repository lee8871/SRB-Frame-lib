#pragma once
#include "srb_heads.h"
#include "iAccess.h"

namespace srb {
	class BaseCluster;
	class InfoCluster;
	class ErrorCluster;
	class MappingCluster;
	class iExpandNode;
	class Node:public iAccesser {
	private:
		friend class iExpandNode;
	protected:
		sMapping* mapping[4] = {nullptr};
		iCluster* clusters[MAX_CLUSTER_NUMBER] = { nullptr };
		SrbMaster* master = nullptr;
		BaseCluster* baseCLU = nullptr;
		InfoCluster* infoCLU = nullptr;
		ErrorCluster* errorCLU = nullptr;
		MappingCluster* mapping0CLU = nullptr;
		bool _exsist = false;
		int initCluster();
		int expand();
		iExpandNode* _expand_node = nullptr;


	public:
		bool const& Exsist = _exsist;
		iBus* Bus();
		const char* Node_name();
		const char* Node_type();		
		uint8 Addr() override;
		iExpandNode* const& Expand_node = _expand_node;

	public:
		uint8 data_rs[128] = {0};
		Node(uint8 a, SrbMaster* m);
		~Node();
		int setMapping(const uint8* map, int map_num);
		int sendAccess(int port);
		void accessDoneReply(iAccess*) override;
		int toJsonAll(iJsonWriter & json_printer);
	};	
}