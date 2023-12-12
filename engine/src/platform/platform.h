#pragma once

#include "defines.h"

typedef struct {
	void* internal;
} platform_state;

b8 platform_create(
		platform_state* state,
		const char* applicationName,
		i32 x,
		i32 y,
		i32 width,
		i32 height);

void platform_destroy(platform_state* state);

b8 platform_poll_events(platform_state* state);

void* platform_memory_allocate(u64 length, b8 aligned);
void platform_memory_free(void* block, b8 aligned);
void platform_memory_zero(void* dst, u64 length);
void platform_memory_copy(void* dst, const void* src, u64 length);
void platform_memory_set(void* dst, i32 value, u64 length);

void platform_console_stdout(const char* message);
void platform_console_stderr(const char* message);

f64 platform_time_get();
void platform_time_sleep(u64 ms);
