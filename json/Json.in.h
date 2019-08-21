#pragma once

#define NOT_JSON_USER
#include "Json.h"
#include "cLogger.h"
namespace lee8871_support {
	extern ModuleLog JsonLog;
};
#ifdef _DEBUG
#define INFO(format, ...) JsonLog.addLog(eLogLevel::info, format,__VA_ARGS__)
#else
#define INFO(format, ...)
#endif