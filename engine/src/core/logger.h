#include "defines.h"

#ifndef LOGGER_H
#define LOGGER_H

typedef enum : u32 {
	LOG_TRACE,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_COUNT
} log_level;

// TODO(zgayford): file logging 
typedef enum : u32 {
	LOG_MODE_FILE_ONLY,
	LOG_MODE_TERM_ONLY,
	LOG_MODE_FILE_AND_TERM
} log_mode;

#define TERM_COLOR_RESET "\033[39m\033[49m"
#define TERM_COLOR_FG(r, g, b) "\033[38;2;" #r ";" #g ";" #b "m"
#define TERM_COLOR_BG(r, g, b) "\033[48;2;" #r ";" #g ";" #b "m"

// made it match my editor theme because im geeky like that
// static const char* logColors[LOG_COUNT] = {
// 	TERM_COLOR_FG(18 , 35 , 57 ),
// 	TERM_COLOR_FG(91 , 119, 140),
// 	TERM_COLOR_FG(131, 159, 180),
// 	TERM_COLOR_FG(122, 87 , 89 ),
// 	TERM_COLOR_FG(151, 125, 124)
// };

static const char* logColors[LOG_COUNT] = {
	TERM_COLOR_FG(38 , 35 , 58 ),
	TERM_COLOR_FG(110, 106, 134),
	TERM_COLOR_FG(196, 167, 231),
	TERM_COLOR_FG(246, 193, 119),
	TERM_COLOR_FG(235, 111, 146)
};


static const char* logStrings[LOG_COUNT] = {
	"trace",
	"debug",
	"info",
	"warn",
	"error"
};

#define log_trace(...)				_log_message(LOG_TRACE, __VA_ARGS__);
#define log_debug(...)				_log_message(LOG_DEBUG, __VA_ARGS__);
#define log_info(...)				_log_message(LOG_INFO , __VA_ARGS__);
#define log_warn(...)				_log_message_with_location(LOG_WARN , __FILE__, __LINE__, __VA_ARGS__);
#define log_error(...)				_log_message_with_location(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__);
#define log_assert_failure(message)	_log_assert_failure_with_location(__FILE__, __LINE__, message);

api void log_set_file(const char* path);
api void log_set_level(log_level level);
api void log_set_mode(log_mode mode);
api void _log_message(u32 level, const char* fmt, ...);
api void _log_message_with_location(u32 level, const char* filepath, const u32 line, const char* fmt, ...);
api void _log_assert_failure_with_location(const char* filepath, const u32 line, const char* message);

#endif
