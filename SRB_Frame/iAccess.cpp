#include "iAccess.h"
#include "iJsonWriter.h"
#include "OsSupport.h"

using namespace lee8871_support;
namespace srb {
	static inline int srbPkgToJson(const sSrbPkg* pkg, const char* name, iJsonWriter &recordJW) {
		if (pkg != nullptr) {
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
	const char * iAccess::getType(){
		return "iAccess";
	}
	int iAccess::cancle() {
		if (_status <= eAccessStatus::WaitSend) {
			_status = eAccessStatus::Cancel;
			return done;
		}
		return fail;
	};
	bool iAccess::isStatusFinish() {
		return (_status >= eAccessStatus::RecvedDone);
	};


	int iAccess::recordSendTime(void) {
		if (_status != eAccessStatus::WaitSend) {
			return fail;
		}
		_send_time = OsSupport::getTimesUs();
		_status = eAccessStatus::SendWaitRecv;
		return done;
	}
	int iAccess::initDone() {
		if (Status != eAccessStatus::Initing) {
			return fail;
		}
		_status = eAccessStatus::WaitSend;
		return done;

	}
	int iAccess::timeoutAccess() {
		if (_status != eAccessStatus::SendWaitRecv) {
			return fail;
		}
		_status = eAccessStatus::SoftwareTimeout;
		return done;
	}




};