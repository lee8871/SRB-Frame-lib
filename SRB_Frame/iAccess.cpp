#include "iAccess.h"
#include <iostream>
using namespace std;
namespace srb {
	static inline char uint8ToLow(uint8 u8) {
		char c = '0' + (u8 & 0x0f);
		if (c > '9') { c = c - '9' - 1 + 'A'; }
		return c;
	}
	static inline std::ostream& intToJson(int value, const char* name, std::ostream&  record) {
		record << "\"" << name << "\":";
		record << value ;
		return record;
	}
	static inline std::ostream& uint8ToJson(uint8 value,const char* name, std::ostream&  record) {
		record << "\"" << name << "\":";
		record << "\"" << uint8ToLow(value>> 4) << uint8ToLow(value) << "\"";
		return record;
	}
	static inline std::ostream& uint8ArrayToJson(const uint8* data, int len, const char* name, std::ostream&  record) {
		record << "\"" << name << "\":";
		record << "\"";
		for (int i = 0;i < len; i++) {
			record << uint8ToLow(data[i] >> 4) << uint8ToLow(data[i]) << ' ';
		}
		record << "\"";
		return record;
	}
	static inline std::ostream& srbPkgToJson(const sSrbPkg* pkg, const char* name, std::ostream&  record) {
		record << "\"" << name << "\":";
		if (pkg != null) {
			record << "{";
			record << "\"bfc\":" << (int)(pkg->bfc.byte) << ",";
			uint8ArrayToJson(pkg->data, pkg->bfc.length, "data", record);
			record << "}";
		}
		else {
			record << "null";
		}
		return record;
	}

	int iAccess::sendJson(std::ostream&  record) {
		record << "{";
		intToJson(_send_time.tv_nsec, "Ts", record)<<",";
		intToJson(owner->Addr(), "Addr", record) << ",";
		intToJson((int)Status, "Stat", record) << ","<< endl;

		srbPkgToJson(Send_pkg, "Send", record) << "," << endl;
		srbPkgToJson(Recv_pkg, "Recv", record) << "}" << endl;
		return 0;
	}
};