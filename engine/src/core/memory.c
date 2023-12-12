#include "memory.h"

#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "core/logger.h"

typedef struct _arena {
	u64 capacity;
	u64 length;
	byte data[];
} arena;

arena* arena_create(u64 allocSize) {
	
	arena* arn = malloc(sizeof(arena) + allocSize);
	memset(arn, 0, allocSize);
	arn->capacity = allocSize;
	arn->length = 0;

	return arn;

}

void arena_destroy(arena* arn) {

	free(arn);

}

void* arena_alloc(arena* arn, u64 allocSize) {

	log_trace("allocation:\t%d", allocSize);

	if (arn->length + allocSize >= arn->capacity)
		return NULL;

	byte* alloc = arn->data + arn->length;
	arn->length += allocSize;

	return alloc;

}

u64 arena_available(arena *arn) {
	return arn->capacity - arn->length;
}
