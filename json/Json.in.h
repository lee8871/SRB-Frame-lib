#pragma once

#define NOT_JSON_USER
#include "Json.h"
#include "cLogger.h"
namespace lee8871_support {
	extern ModuleLog* JsonLog;
};
#ifdef WIN32
#ifdef _DEBUG
#define INFO(format,...) JsonLog->addLog(eLogLevel::info,format, __VA_ARGS__)
#else
#define INFO(format,...)
#endif

#define ERROR(format,...) JsonLog->addLog(eLogLevel::erro,format,  __VA_ARGS__ )
#define WARNING(format,...) JsonLog->addLog(eLogLevel::warn,format,  __VA_ARGS__ )
#endif

#ifdef UNIX
#ifdef _DEBUG
#define INFO(arg...) JsonLog->addLog(eLogLevel::info, arg)
#else
#define INFO(arg...)
#endif

#define ERROR(arg...) JsonLog->addLog(eLogLevel::erro,  arg )
#define WARNING(arg...) JsonLog->addLog(eLogLevel::warn,  arg )
#endif