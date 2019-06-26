#pragma once
#include "srb_heads.h"

namespace srb {
	class BaseCluster;
	class InfoCluster;
	class ErrorCluster;
	class MappingCluster;
	class iExpandNode {
	private:
	protected:
		friend class Node;
		Node* base_node = null;
		int addCluster(iCluster* cluster);
		void * data_rs_void;
	public:
		iExpandNode(Node* base_node);
		~iExpandNode();
		int sendAccess(int port);
		const char* Node_type();


		BaseCluster *  BaseCLU();
		InfoCluster *  InfoCLU();
		ErrorCluster *  ErrorCLU();
		MappingCluster *  Mapping0CLU();

	};
}