#pragma once
#include "srb_heads.h"
#include "iAccess.h"
namespace srb {
	class iCluster{
	protected:
		uint8 _data_u8[30] = { 0 };
		BaseNode* _node = null;
		uint8 _cluster_id;
	public :


		BaseNode* const& Node = _node;
		uint8 const& Cluster_id = _cluster_id;
	public:
		const uint8* Data_u8();
		uint8 buffer_u8[30];

		iCluster(BaseNode* n);
		void loadReadPkg(iAccess* a);
		void readDone(iAccess* a);
		void write(iAccess* a);
		void writeDone(iAccess* a);

		virtual int toJson(iJsonWriter & json_printer);
	};



}
