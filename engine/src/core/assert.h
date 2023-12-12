#pragma once

// TODO(zgayford): remove
#include <stdlib.h>

#include "core/logger.h"

#ifdef DEBUG

#define assert(condition, message) ({														\
		if (!(condition)) {																\
			log_assert_failure(message);													\
			exit(EXIT_FAILURE);															\
		}})																			\

#define error(errno, ...) ({																\
		if (errno) { 																	\
			log_error(__VA_ARGS__); 														\
			exit(errno); 																\
		}}) 																			\

#define sdl_error(errno, ...) ({															\
		if (errno < 0) { 																\
			log_error(__VA_ARGS__); 														\
			exit(errno); 																\
		}}) 																			\

#elifdef RELEASE

#define assert(condition, message)
#define error(errno, ...)
#define sdl_error(errno, ...)

#else
#endif
