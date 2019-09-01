#define NOT_JSON_USER
#include "Json.in.h"

namespace lee8871_support {
	ModuleLog* JsonLog = nullptr;
};
using namespace std;
namespace lee8871_support {


	Json::Json() :_transform(nullptr) {}	
	void Json::cloneTransport(const Json&from, void* new_value_ptr) {
		value_prt = new_value_ptr;
		if (_transform != nullptr) {
			_transform->freeQuote();
		}
		_transform = from._transform;
		_transform->quote();

	}

	void Json::moveFrom(Json &from){
		value_prt = from.value_prt;
		_transform = from._transform;
		from._transform = nullptr;
		from.value_prt = nullptr;

	}
	void Json::copyFrom(const Json &from) {
		value_prt = from.value_prt;
		if (_transform != nullptr) {
			_transform->freeQuote();
		}
		_transform = from._transform;
		_transform->quote();
	}

	Json::~Json() {
		if (_transform != nullptr) {
			_transform->freeQuote();
		}
	}
	Json Json::operator= (const Json & right)	{
		copyFrom(right);
		return right;
	}
	Json::Json(const Json& old) {
		copyFrom(old);
	}
	Json::Json(Json&& old) {
		moveFrom(old);
	}
	Json::Json(iJsonTransformer* _transform, void * value_prt) :
		_transform(_transform->quote()),
		value_prt(value_prt) {}


	class asArray :public iJsonTransformer {
	private:
		Json * table;
		int size;

	public:
		asArray(initializer_list<Json> v){
			size = v.size();
			if (size != 0) {
				table = new Json[size]();
				int i = 0;
				for (auto var : v) {
					table[i++].moveFrom(var);
				}
				INFO("Json array with %d length is create - %x.", size, (size_t)this);
			}
			else {
				table = nullptr;
				WARNING("Json array with 0 length is create.");
			}
		}
		asArray(const asArray& form) = delete;
		asArray(asArray&& form) = delete;

		~asArray() {

			if (table != nullptr) {
				delete[] table;
			}
			INFO("Json array with %d length is delete - %x.", size, (size_t)this);
		}
		int get(JsonGenerateString* str, const void *diff) {
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
			str->recordBeginPtr();
			int i = 0;
			if (str->isBgnHasNext(true)) {
				do {
					if (i < size) {
						checkFailReturn(table[i].set(str, diff));
					}
					else {
						str->captureAndPrintError("Json array too long");
						return fail;
					}
					i++;
				} while (str->isGapHasNext(true));
			}
			if (i != size) {
				str->captureAndPrintError("Json array length no match  %d->%d",i,size);
			}
			return done;
		}
	};
	Json::Json(initializer_list<Json> v) :
		_transform((new asArray(v))),
		value_prt(nullptr){}

	class asObject:public iJsonTransformer {
	private:
		struct named_json {
			unsigned int hash;
			const char* name;
			Json j;
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
				next_ch = *(str->Ptr);
				str->forward();
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
		asObject(initializer_list<pair<const char*, Json>> v) {
			size = v.size();
			if (size != 0) {
				table = new named_json[size]();
				int i = 0;
				for(auto var : v) {
					table[i].j.moveFrom(var.second);
					table[i].name = var.first;
					table[i].hash = getHashString(var.first);
					i++;
				}
				INFO("Json object with %d length is create - %x.", size, (size_t)this);
			}
			else {
				table = nullptr;
				WARNING("Json object with 0 length is create.");
			}
		}
		asObject(const asObject& form) = delete;
		asObject(asObject&& from) = delete;
		~asObject() {
			if (table != nullptr) {
				delete[] table;
			}
			INFO("Json object with %d length is delete - %x.", size, (size_t)this);
		}
		int get(JsonGenerateString* str, const void *diff) {
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
			str->recordBeginPtr();
			str->removeSpace();
			unsigned int hash = getHashByDoubleQuotes(str);
			if (str->checkCh(':') == false) {
				str->captureAndPrintError("get '%c' need a ':'", *str->Ptr);
				return fail;
			}
			if (hash == NO_HASH) {
				str->captureAndPrintError("get attribute hash fail.");
				return fail;
			}
			int i = 0;
			while (table[i].hash != hash) {
				i++;
				if (i >= size) {
					str->captureAndPrintError("attribute no match.");
					return fail;
				}
			}
			return table[i].j.set(str, diff);
		}
		int set(JsonParseString* str, void *diff) {
			str->recordBeginPtr();
			if (str->isBgnHasNext(false)){
				do {
					checkFailReturn(setOneAttribute(str, diff));
				}
				while (str->isGapHasNext(false));
			}
			return done;
		}
	};

	Json::Json(initializer_list<pair<const char*, Json>> v) :
		_transform(new asObject(v)) ,
		value_prt(nullptr) {}




	int Json::get(JsonGenerateString* str, const void *diff) {
		return _transform->get(str, (void*)((size_t)diff+ (size_t)value_prt));
	}

	int Json::set(JsonParseString * str, void * diff){
		return _transform->set( str, (void*)((size_t)diff + (size_t)value_prt));
	}

};


















/*{
	int __get(JsonGenerateString* str, const void *diff) {
		ERROR("get() call by an uninitialized json object");
		return fail;
	};
	int __set(JsonParseString* str, void *diff) {
		ERROR("set() call by an uninitialized json object");
		return fail;
	};
	size = form.size;
	if (size != 0) {
		table = new json[size]();
		for (int i = 0;i < size;i++) {
			table[i].copyFrom(form.table[i]);
		}
		INFO("Json array with %d length is copy - %x.", size, (size_t)this);
	}
	else {
		table = nullptr;
		WARNING("Json array with 0 length is copy - %x.", (size_t)this);
	}
}*/
/* {
			size = form.size;
			if (size != 0) {
				table = new named_json[size]();
				for (int i = 0;i < size;i++) {
					table[i].j.copyFrom(form.table[i].j);
					table[i].name = form.table[i].name;
					table[i].hash = form.table[i].hash;
				}
				INFO("Json object with %d length is copy - %x.", size, (size_t)this);
			}
			else {
				table = nullptr;
				WARNING("Json object with 0 length is copy - %x.", (size_t)this);
			}
		}*/