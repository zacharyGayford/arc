#include "logger.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "defines.h"
#include "core/memory.h"
#include "core/string.h"

static log_level logLevel = LOG_DEBUG;
static log_mode logMode = LOG_MODE_FILE_ONLY;

// TODO(zgayford): implement
// void log_set_file(const char* path)

void _log_message(log_level level, const char* fmt, ...) {

	if (level < logLevel) return;

	printf("%s", logColors[level]);

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	printf(TERM_COLOR_RESET "\n");
	va_end(args);

}

void _log_message_with_location(log_level level, const char* filepath, const u32 line, const char *fmt, ...) {

	if (level < logLevel) return;

	arena* scratch = arena_create(strlen(filepath) * 8);

	string filepathString = stringify(scratch, filepath);
	string delim = stringify(scratch, "\\");

	u64 count = 0;
	slice* dices = string_dice(scratch, filepathString, delim, &count);
	string filename = stringify(scratch, dices[count - 1]);

	printf("%s%s file: %s line: %d\n\t", 
			logColors[level],
			logStrings[level],
			filename,
			line
		 );

	arena_destroy(scratch);

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	printf(TERM_COLOR_RESET "\n");
	va_end(args);

}

void _log_assert_failure_with_location(const char* filepath, const u32 line, const char* message) {

	arena* scratch = arena_create(strlen(filepath) * 8);

	string filepathString = stringify(scratch, filepath);
	string delim = stringify(scratch, "\\");

	u64 count = 0;
	slice* dices = string_dice(scratch, filepathString, delim, &count);
	string filename = stringify(scratch, dices[count - 1]);

	printf("%sassertion failure file: %s line: %d\n", 
			logColors[LOG_ERROR],
			filename,
			line
		 );

	arena_destroy(scratch);

	printf("\t%s", message);
	printf(TERM_COLOR_RESET "\n");

}

void log_set_level(log_level level) {
	logLevel = level;
}

void log_set_mode(log_mode mode) {
	logMode = mode;
}
