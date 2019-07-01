#pragma once
#include "srb_heads.h"
namespace srb {
	class AccessRecorder {
	private:
		class Impl;
		std::unique_ptr<Impl> pimpl;		
	public:
		AccessRecorder();
		~AccessRecorder();
		int setPathname(const char* pathname);
		int record(iAccess* acs);		
	};
}
