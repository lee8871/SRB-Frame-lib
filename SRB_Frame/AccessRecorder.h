#pragma once
#include <memory>
namespace srb {
	class iAccess;
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
