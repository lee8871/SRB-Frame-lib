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
#define INFO(__VA_ARGS__...) JsonLog->addLog(eLogLevel::info, __VA_ARGS__)
#else
#define INFO(__VA_ARGS__...)
#endif

#define ERROR(__VA_ARGS__...) JsonLog->addLog(eLogLevel::erro,  __VA_ARGS__ )
#define WARNING(__VA_ARGS__...) JsonLog->addLog(eLogLevel::warn,  __VA_ARGS__ )
#endif