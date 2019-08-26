#pragma once
#include "srb_heads.h"
#include "iAccess.h"
#include "json.h"

namespace srb {
	class iCluster{
	protected:
		uint8 _data_u8[30] = { 0 };
		Node* _parent_node = nullptr;
		uint8 _cluster_id;
	public :


		Node* const& Parent_node = _parent_node;
		uint8 const& Cluster_id = _cluster_id;
	public:
		inline const uint8* Data_u8() { return _data_u8; }
		uint8 buffer_u8[30];

		iCluster(Node* n);
		void loadReadPkg(iAccess* a);
		void readDone(iAccess* a);
		void write(iAccess* a);
		void writeDone(iAccess* a);

		virtual int toJson(iJsonWriter & json_printer);
	};



}
