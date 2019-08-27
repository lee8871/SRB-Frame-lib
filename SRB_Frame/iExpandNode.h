#pragma once
#include "srb_heads.h"
#include "Json.h"

namespace srb {
	class BaseCluster;
	class InfoCluster;
	class ErrorCluster;
	class MappingCluster;
	class iExpandNode {
	private:
	protected:
		friend class Node;
		Node* _base_node;
		int addCluster(iCluster* cluster);
		void * data_rs_void;
		static lee8871_support::Json* base_to_json;
	public:
		iExpandNode(Node* _base_node);
		~iExpandNode();
		int sendAccess(int port);
		const char* Node_type();


		BaseCluster *  BaseCLU();
		InfoCluster *  InfoCLU();
		ErrorCluster *  ErrorCLU();
		MappingCluster *  Mapping0CLU();
		SrbMaster * Master();
		virtual lee8871_support::Json* ToJson();

	};
}