#pragma once
#include "srb_heads.h"
#include "iAccess.h"
namespace srb {
	class BaseCluster;
	class InfoCluster;
	class ErrorCluster;
	class MappingCluster;
	class Node:public iAccesser {
	private:
		friend class iExpandNode;
	protected:
		sMapping* mapping[4] = {null};
		iCluster* clusters[MAX_CLUSTER_NUMBER] = { null };
		Master* master = null;
		BaseCluster* baseCLU = null;
		InfoCluster* infoCLU = null;
		ErrorCluster* errorCLU = null;
		MappingCluster* mappingCLU = null;
		bool _exsist = false;


		int initCluster();
		iExpandNode* _expand_node = null;


	public:
		bool const& Exsist = _exsist;
		BaseCluster * const& BaseCLU = baseCLU;
		InfoCluster * const& InfoCLU = infoCLU;
		ErrorCluster * const& ErrorCLU = errorCLU;
		MappingCluster * const& MappingCLU = mappingCLU;
		iBus*  Bus();
		const char* Node_name();
		uint8 Addr() override;
		iExpandNode* const& Expand_node = _expand_node;

	public:
		uint8 data_rs[128] = {0};
		Node(uint8 a, Master* m);
		~Node();
		int setMapping(const uint8* map, int map_num);
		int sendAccess(int port);
		void accessDone(iAccess*) override;



		int toJsonAll(iJsonWriter & json_printer);
	};	
}