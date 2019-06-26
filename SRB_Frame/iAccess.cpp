#include "iAccess.h"
#include "iJsonWriter.h"

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





#ifdef WINDOW_86
#include <time.h>
#include <windows.h>
	static int cpu_freq = -1;
	static void initTimes() {
		if (cpu_freq == -1) {
			LARGE_INTEGER f;
			QueryPerformanceFrequency(&f);
			cpu_freq =(int)( f.QuadPart);
		}
	}
	void iAccess::recordSendTime(void) {
		if(cpu_freq==-1){
			initTimes();
		}
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		_send_time = time.QuadPart*1000000 / cpu_freq;
	}
#endif

#if UNIX
#include <sys/time.h> 
	void iAccess::recordSendTime(void) {
		struct timeval t;
		if (gettimeofday(&t,null) != 0) {
			_send_time = -1;
		}
		_send_time = t.tv_usec + t.tv_sec*1000*1000;
	}
#endif


};