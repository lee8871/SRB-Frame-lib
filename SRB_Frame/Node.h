#pragma once
#include "srb_heads.h"
#include "iAccess.h"
namespace srb {
	class Node:public iAccesser {
	private:
	
	protected:
		sMapping* mapping[4] = {null};
		iCluster* clu[MAX_CLUSTER_NUMBER] = { null };
		Master* master = null;
		BaseCluster* baseCLU;
		InfoCluster* infoCLU;
		ErrorCluster* errorCLU;
		bool _exsist = false;
	public:
		bool const& Exsist = _exsist;
		BaseCluster * const& BaseCLU = baseCLU;
		InfoCluster * const& InfoCLU = infoCLU;
		ErrorCluster * const& ErrorCLU = errorCLU;
		iBus*  Bus();
		const char* Node_name();
		uint8 Addr() override;

	public:
		uint8 rs_data[128] = {0};
		Node(uint8 a, Master* m);
		~Node();

		int setMapping(const uint8* map, int map_num);
		int sendAccess(int port);
		void accessDone(iAccess*) override;



		int toJsonAll(iJsonWriter & json_printer);
	};	
}