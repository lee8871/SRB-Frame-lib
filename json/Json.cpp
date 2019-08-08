#include "Json.h"

using namespace std;
namespace lee8871_support {


	void json::copyValueFrom(const json& from) {
		size = from.size;
		transform = from.transform;
		name = from.name;
		hash = from.hash;

	}
	void json::moveFrom(json &from){
		copyValueFrom(from);
		value_prt = from.value_prt;
		from.transform = asError;
		from.size = 0;
		from.value_prt = nullptr;
	}
	void json::copyFrom(const json &from) {
		copyValueFrom(from);
		if (size != 0) {
			json* new_table = new json[size]();
			json* old_table = (json *)from.value_prt;
			for (int i = 0;i < size;i++) {
				new_table[i].copyFrom(old_table[i]);
			}
			value_prt = new_table;
		}
		else{
			value_prt = from.value_prt;
		}
	}
	json::~json()	{
		if (size != 0) {
			delete[]((json*)value_prt);
		}
	}
	json json::operator= (const json & right)	{
		copyFrom(right);
		return right;
	}
	json::json(const json& old) {
		copyFrom(old);
	}
	json::json(json&& old) {
		moveFrom(old);
	}


	json::json(initializer_list<json> v) : transform(asArray) {
		size = v.size();
		if (size != 0) {
			json* table = new json[size]();
			int i = 0;
			for each (auto var in v) {
				table[i++].moveFrom(var);
			}
			value_prt = table;
		}
	}
	constexpr unsigned int NO_HASH = 0;
	constexpr int HASH_MOD = 1610612741;
	unsigned int getHashString(const char * str) {
		if (str == nullptr) {
			return NO_HASH;
		}
		int i = 0;
		unsigned int id = 0;
		unsigned int id_temp = 1;
		const int HASH_MOD = 1610612741;
		unsigned char mapped = 0;
		while (i < 255) {
			if (str[i] == 0) { return (id^id_temp); }
			id_temp ^= ((unsigned int)str[i]) << (1 + (i % 4) * 7);
			i++;
			if ((i % 4) == 0) {
				id ^= id_temp;
				id *= HASH_MOD;
				id_temp = 1;
			}
		}
		return NO_HASH;
	}
	unsigned int getHashByDoubleQuotes(const char * str) {
		int i = 0;
		unsigned int id = 0;
		unsigned int id_temp = 1;
		const int HASH_MOD = 1610612741;
		unsigned char mapped = 0;
		if (str[i] != '"') {
			return NO_HASH;
		}
		i++;
		while (i < 255) {
			if (str[i] == '"') { return (id^id_temp); }
			id_temp ^= ((unsigned int)str[i]) << (1 + (i % 4) * 7);
			i++;
			if ((i % 4) == 0) {
				id ^= id_temp;
				id *= HASH_MOD;
				id_temp = 1;
			}
		}
		return NO_HASH;
	}


	json::json(initializer_list<pair<const char*, json>> v) : transform(asObject) {
		size = v.size();
		if (size != 0) {
			json* table = new json[size]();
			int i = 0;
			for each (auto var in v) {
				table[i].moveFrom(var.second);
				table[i].name = var.first;
				table[i].hash = getHashString(name);
				i++;
			}
			value_prt = table;
		}
	}


	int json::get(JsonString* str, void *diff) {
		return transform(this,str, diff, true);
	}

	int json::set(JsonString * str, void * diff){
		return transform(this, str, diff, false);
	}



	int asArray(transformCBArgumenrt) {
		if (is_get) {
			auto table = (json*)obj->value_prt;
			int i = 0;
			if (0 == obj->size) { return str->append("[]"); }
			checkFailReturn(str->arrayBgn());
			while (1) {
				checkFailReturn(table[i++].get(str, diff));
				if (i < obj->size) {
					checkFailReturn(str->arrayGasket());
				}
				else {
					checkFailReturn(str->arrayEnd());
					return done;
				}
			}
		}
		else {


		}
	}

	int asObject(transformCBArgumenrt) {
		if (is_get) {
			auto table = (json*)obj->value_prt;
			int i = 0;
			if (0 == obj->size) { return str->append("{}"); }
			checkFailReturn(str->objectBgn());
			while (1) {
				checkFailReturn(str->append(table[i].name));
				checkFailReturn(str->append(':'));
				checkFailReturn(table[i++].get(str, diff));
				if (i < obj->size) {
					checkFailReturn(str->objectGasket());
				}
				else {
					checkFailReturn(str->objectEnd());
					return done;
				}
			}
		}
		else {

		}
	}
};


