#include "iAccess.h"
#include "iJsonWriter.h"
#include <chrono>

namespace srb {
	static inline int srbPkgToJson(const sSrbPkg* pkg, const char* name, iJsonWriter &recordJW) {
		if (pkg != null) {
			recordJW.beginObj(name);
			recordJW.writeU8Str("bfc,", pkg->bfc.byte);
			recordJW.writeU8ArrayStr("data", pkg->data, pkg->bfc.length);
			recordJW.endObj();
		}
		else {
			recordJW.writeNull(name);
		}
		return done;
	}

	int iAccess::sendJson(iJsonWriter & recordJW) {
		recordJW.beginObj("");
		recordJW.writeLongLongNum("Ts", _send_time);
		recordJW.writeNum("Addr", owner->Addr());
		recordJW.writeNum("Status", (int)Status);
		recordJW.writeEndLine();
		srbPkgToJson(Send_pkg, "Send", recordJW);
		recordJW.writeEndLine();
		srbPkgToJson(Recv_pkg, "Recv", recordJW);
		recordJW.endObj();
		return done;
	}		
	void iAccess::recordSendTime(void){
		std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			);
		_send_time = us.count();
	}
};