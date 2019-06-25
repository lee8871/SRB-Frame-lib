#include "lee.h"
#include "iJsonWriter.h"
#include <iostream> 

namespace srb {
	class Log {
	private:
//		ostream write_to_STM;
		int log_level;
	public:
		Log();
		~Log();
	};
}