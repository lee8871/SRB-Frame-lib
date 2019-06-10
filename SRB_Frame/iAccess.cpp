#include "iAccess.h"
#include <iostream>     

namespace srb {
	int iAccess::sendJson(ostream record)	{
		record << "{";
		record << "\"Ts\":" << _send_time << ",";
		record << "\"Addr\":" << owner->Addr << ",";
		return 0;
	}
}
