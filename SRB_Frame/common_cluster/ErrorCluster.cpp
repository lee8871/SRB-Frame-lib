#include "ErrorCluster.h"
#include "iAccess.h"
#include "iJsonWriter.h"
namespace srb {

	ErrorCluster::ErrorCluster(Node* node)
		:iCluster(node)
	{
		_cluster_id = DEFAULT_CID;
	}
	ErrorCluster::~ErrorCluster(){	}



	const csError * ErrorCluster::Data(){
		return (const csError* )Data_u8();
	}
	csError * ErrorCluster::Buffer()	{
		return (csError*)buffer_u8;
	}




	int ErrorCluster::toJson(iJsonWriter & json_printer) {
		json_printer.beginObj("Error_clu");
		json_printer.writeNum("Id", Cluster_id);
		json_printer.writeEndLine();
		json_printer.writeStr("description", (const char *)(Data()->description));
		json_printer.writeNum("file", Data()->file);
		json_printer.writeNum("line", Data()->line);
		json_printer.endObj();
		return done;
	}


	Json ErrorCluster::to_json{
#define relTo(value) (((csError*)((ErrorCluster*)0)->_data_u8)->value)
		{"file",&relTo(file)},
		{"line",&relTo(line)},
		{"description",buildJsonStr((char*)relTo(description),24)},
	};



}
