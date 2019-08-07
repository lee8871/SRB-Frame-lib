
#define transformCBArgumenrt json* obj ,JsonString* str, void* diff, bool is_get
#define valuePtr(T) (   (T*)((size_t)(obj->value_prt) + (size_t)diff)   )
