#include "Json.h"

using namespace std;
namespace lee8871_support {


	void json::copyValueFrom(const json& from) {
		size = from.size;
		transform = from.transform;

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
	struct named_json {
		unsigned int hash;
		const char* name;
		json j;
	};


	int asObject(transformCBArgumenrt) {
		if (is_get) {
			auto table = (named_json*)obj->value_prt;
			int i = 0;
			if (0 == obj->size) { return str->append("{}"); }
			checkFailReturn(str->objectBgn());
			while (1) {
				checkFailReturn(str->print("\"%s\":",table[i].name));
				checkFailReturn(table[i++].j.get(str, diff));
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
	json::json(initializer_list<pair<const char*, json>> v) : transform(asObject) {
		size = v.size();
		if (size != 0) {
			auto table = new named_json[size]();
			int i = 0;
			for each (auto var in v) {
				table[i].j.moveFrom(var.second);
				table[i].name = var.first;
				table[i].hash = getHashString(var.first);
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
		auto table = (json*)obj->value_prt;
		int i = 0;
		if (is_get) {
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
			if (str->confirm('[') == false) {
				str->reportError("[ErrDse]")->print("need a '['\n");
				return fail;
			}
			if (0 == obj->size) {
				if (str->confirm(']') == false) {
					str->reportError("[ErrDse]")->print("need a ']'\n");
					return fail;
				}
				return done;
			}
			else{
				while (1) {
					checkFailReturn(table[i++].set(str, diff));
					if (i < obj->size) {
						if (str->confirm(',') == false) {
							if (str->confirm(']') == false) {
								str->reportError("[ErrDse]")->print("need a ','\n");
								return fail;
							}
							str->reportError("[WarningDse]")->print("Array short than need\n");
							return done;
						}
					}
					else {
						if (str->confirm(']') == false) {
							if (str->confirm(',') == true) {
								str->reportError("[ErrDse]")->print("Array Long than need, values are aborded.\n");
								return fail;//TODO, aborded, other data and return done
							}
							str->reportError("[WarningDse]")->print("need a ']'\n");
							return fail;
						}
					return done;
					}
				}
			}
		}
	}
};


