#define NOT_JSON_USER
#include "Json.h"

using namespace std;
namespace lee8871_support {
	class asError: public JsonTransformer {
	public:
		int get(JsonGenerateString* str, void *diff) {
			//TODO add errorLog report
			return fail;
		};
		int set(JsonParseString* str, void *diff) {
			//TODO add errorLog report
			return fail;
		};
		~asError() {};
		JsonTransformer* copy() {
			return new asError();
		}
	};

	json::json() :transform(new asError) {}

	void json::moveFrom(json &from){
		value_prt = from.value_prt;
		transform = from.transform;
		from.transform = new asError();
		from.value_prt = nullptr;
		if (value_prt == nullptr)
		{
			value_prt = (void*)0;
		}
	}
	void json::copyFrom(const json &from) {
		value_prt = from.value_prt;
		transform = from.transform->copy();
		if (value_prt == nullptr)
		{
			value_prt = (void*)0;
		}
	}
	json::~json() {
		if (transform->isCommon == false) {
			delete transform;
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


	class asArray :public JsonTransformer {
	private:
		json * table;
		int size;	

	public:
		asArray(initializer_list<json> v){
			isCommon = false;
			size = v.size();
			if (size != 0) {
				table = new json[size]();
				int i = 0;
				for each (auto var in v) {
					table[i++].moveFrom(var);
				}
			}
			else {
				table = nullptr;
			}
		}	
		asArray(const asArray& form) {
			isCommon = false;
			size = form.size;
			if (size != 0) {
				table = new json[size]();
				for (int i = 0;i < size;i++) {
					table[i].copyFrom(form.table[i]);
				}
			}
			else {
				table = nullptr;
			}
		}
		asArray(asArray&& form) = delete;
		JsonTransformer*  copy() {
			return new asArray(*this);
		}
		~asArray() {
			if (table != nullptr) {
				delete[] table;
			}
		}
		int get(JsonGenerateString* str, void *diff) {
			int i = 0;
			if (0 == size) { return str->append("[]"); }
			str->arrayBgn();
			while (1) {
				checkFailReturn(table[i++].get(str, diff));
				if (i < size) {
					str->arrayGap();
				}
				else {
					str->arrayEnd();
					return str->checkOverflow();
				}
			}
		}
		int set(JsonParseString* str, void *diff) {
			int i = 0;
			if (str->isBgnHasNext(true)) {
				do {
					if (i < size) {
						checkFailReturn(table[i].set(str, diff));
					}
					else {
						str->captureError("EJ2A")->print("Json array too long");
						return fail;
					}
					i++;
				} while (str->isGapHasNext(true));
			}
			if (i != size) {
				str->captureError("WJ2A")->print("Json array lengthno match  %d->%d",i,size);
			}
			return done;
		}
	};
	json::json(initializer_list<json> v) : transform(new asArray(v)) {
		value_prt = nullptr;
	}

	class asObject:public JsonTransformer {
	private:
		struct named_json {
			unsigned int hash;
			const char* name;
			json j;
		};
		named_json * table;
		int size;
		static constexpr unsigned int NO_HASH = 0;
		static constexpr int HASH_MOD = 1610612741;
		unsigned int getHashString(const char * str) {
			if (str == nullptr) {
				return NO_HASH;
			}
			int i = 0;
			unsigned int id = 0;
			unsigned int id_temp = 1;
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
		unsigned int getHashByDoubleQuotes(JsonParseString* str) {
			unsigned int id = 0;
			unsigned int id_temp = 1;
			unsigned char mapped = 0;
			if (!str->checkCh('"')) {
				return NO_HASH;
			}
			int i = 0;
			char next_ch;
			while (i < 255) {
				next_ch = str->nextChar();
				if (next_ch == '"') {return (id^id_temp); }
				id_temp ^= ((unsigned int)next_ch) << (1 + (i % 4) * 7);
				i++;
				if ((i % 4) == 0) {
					id ^= id_temp;
					id *= HASH_MOD;
					id_temp = 1;
				}
			}
			return NO_HASH;
		}
	public :
		asObject(initializer_list<pair<const char*, json>> v) {
			isCommon = false;
			size = v.size();
			if (size != 0) {
				table = new named_json[size]();
				int i = 0;
				for each (auto var in v) {
					table[i].j.moveFrom(var.second);
					table[i].name = var.first;
					table[i].hash = getHashString(var.first);
					i++;
				}
			}
			else {
				table = nullptr;
			}
		}
		asObject(const asObject& form) {
			isCommon = false;
			size = form.size;			
			if (size != 0) {
				table = new named_json[size]();
				for (int i = 0;i < size;i++) {
					table[i].j.copyFrom(form.table[i].j);
					table[i].name = form.table[i].name;
					table[i].hash = form.table[i].hash;
				}
			}
			else {
				table = nullptr;
			}
		}
		asObject(asObject&& from) = delete;
		~asObject() {
			if (table != nullptr) {
				delete[] table;
			}
		}

		JsonTransformer* copy() {
			return new asObject(*this);
		}
		int get(JsonGenerateString* str, void *diff) {
			int i = 0;
			if (0 == size) { return str->append("{}"); }
			str->addBgn();
			while (1) {
				str->print("\"%s\":", table[i].name);
				checkFailReturn(table[i++].j.get(str, diff));
				if (i < size) {
					str->addGap();
				}
				else {
					str->addEnd();
					return str->checkOverflow();
				}
			}
		}
		int setOneAttribute(JsonParseString* str, void *diff) {
			str->outputRemoveSpace();
			unsigned int hash = getHashByDoubleQuotes(str);
			if (str->checkCh(':') == false) {
				str->captureError("J2O")->print("get '%c' need a ':'", *str->Ptr);
				return fail;
			}
			if (hash == NO_HASH) {
				str->captureError("J2O")->print("get attribute hash fail.");
				return fail;
			}
			int i = 0;
			while (table[i].hash != hash) {
				i++;
				if (i >= size) {
					str->captureError("J2O")->print("attribute no match.");
					return fail;
				}
			}
			checkFailReturn(table[i++].j.set(str, diff));
		}
		int set(JsonParseString* str, void *diff) {
			if (str->isBgnHasNext(false)){
				do {
					checkFailReturn(setOneAttribute(str, diff));
				}
				while (str->isGapHasNext(false));
			}
		}
	};

	json::json(initializer_list<pair<const char*, json>> v) 
		: transform(new asObject(v)) {
		value_prt = nullptr;
	}

	json::json(JsonTransformer* transform, void * value_prt) : transform(transform), value_prt(value_prt) {}

	int json::get(JsonGenerateString* str, void *diff) {
		return transform->get(str, (void*)((size_t)diff+ (size_t)value_prt));
	}

	int json::set(JsonParseString * str, void * diff){
		return transform->set( str, (void*)((size_t)diff + (size_t)value_prt));
	}

};


