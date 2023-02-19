#pragma once
#ifdef DEBUG
	#ifdef PLATFORM_WINDOWS
		#define DEBUGBREAK() __debugbreak()
	#elif defined PLATFORM_LINUX
		#include <signal.h>
		#define DEBUGBREAK() raise(SIGTRAP)
	#else
		#define DEBUGBREAK() 
	#endif
	#define ASSERT(x) if (!(x)) { DEBUGBREAK(); }
#else
	#define ASSERT(x)
#endif // DEBUG