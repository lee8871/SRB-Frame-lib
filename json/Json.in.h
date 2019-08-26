#pragma once

#define NOT_JSON_USER
#include "Json.h"
#include "cLogger.h"
namespace lee8871_support {
	extern ModuleLog* JsonLog;
};
#ifdef WIN32
#ifdef _DEBUG
#define INFO(...) JsonLog->addLog(eLogLevel::info, __VA_ARGS__)
#else
#define INFO(...)
#endif

#define ERROR(...) JsonLog->addLog(eLogLevel::erro,  __VA_ARGS__ )
#define WARNING(...) JsonLog->addLog(eLogLevel::warn,  __VA_ARGS__ )
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