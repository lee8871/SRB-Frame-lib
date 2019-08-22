#pragma once

#define NOT_JSON_USER
#include "Json.h"
#include "cLogger.h"
namespace lee8871_support {
	extern ModuleLog* JsonLog;
};
#ifdef _DEBUG
#define INFO(args...) JsonLog->addLog(eLogLevel::info, args)
#else
#define INFO(args...)
#endif

#define ERROR( args...) JsonLog->addLog(eLogLevel::erro,  args )
#define WARNING( args...) JsonLog->addLog(eLogLevel::warn,  args )