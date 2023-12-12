#include "defines.h"

#ifndef MEMORY_H
#define MEMORY_H

typedef struct _arena arena;

arena* arena_create(u64 allocSize);
void arena_destroy(arena* arn);
void* arena_alloc(arena* arn, u64 allocSize);
u64 arena_available(arena *arn);

#endif
