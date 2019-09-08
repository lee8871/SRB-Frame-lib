#pragma once
#include "srb_heads.h"
#include "iAccess.h"

namespace srb {
	class iCluster{
	protected:
		uint8 _data_u8[30] = { 0 };
		Node* _parent_node = nullptr;
		uint8 _cluster_id;
		int data_len = -1;
	public :
		Node* const& Parent_node = _parent_node;
		uint8 const& Cluster_id = _cluster_id;
	public:
		inline uint8* Data_u8() { return _data_u8; }
		iCluster(Node* n);
		void loadReadPkg(iAccess* a);
		void accessDone(iAccess* a);
		void loadWritePkg(iAccess* a);
	};
}
