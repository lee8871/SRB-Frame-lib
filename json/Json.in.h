#pragma once

#define NOT_JSON_USER
#include "Json.h"
#include "cLogger.h"
namespace lee8871_support {
	extern ModuleLog* JsonLog;
};
#ifdef _DEBUG
#define INFO(format, ...) JsonLog->addLog(eLogLevel::info, format,__VA_ARGS__)
#else
#define INFO(format, ...)
#endif


#define ERROR(format, ...) JsonLog->addLog(eLogLevel::erro, format,__VA_ARGS__)
#define WARNING(format, ...) JsonLog->addLog(eLogLevel::warn, format,__VA_ARGS__)