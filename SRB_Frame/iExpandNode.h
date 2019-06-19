#pragma once
#include "srb_heads.h"
#include "iAccess.h"
namespace srb {
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
	};
}