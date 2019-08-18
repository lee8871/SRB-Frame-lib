#include "iBus.h"
#include "iAccess.h"
#include "AccessRecorder.h"

using namespace lee8871_support;
namespace srb {
	int iBus::accessDone(iAccess* acs){
		if(access_recoeder!=nullptr){
			access_recoeder->record(acs);
		}	
		acs->owner->accessDoneReply(acs);
		return done;
	}

};