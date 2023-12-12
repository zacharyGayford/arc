#include "defines.h"
#include "memory.h"

#ifndef STRING_H
#define STRING_H

typedef char* string;

typedef struct {
	char* start;
	char* end;
} slice;

#define SLICE(str, startIndex, endIndex)				\
	((slice) { 									\
		.start = str + startIndex, 					\
		.end = str + endIndex 						\
	}) 											\

string string_create(arena* arn, u32 length);

#define stringify(arn, x) _Generic((x),					\
	const char*: stringify_cstr,						\
	char*: stringify_cstr,							\
	slice: stringify_slice) 							\
	((arn), (x))

string stringify_cstr(arena* arn, const char* cstr);
string stringify_slice(arena* arn, slice s);

#define string_put(str, x) _Generic((x),				\
	const char*: string_put_cstr,						\
	char*: string_put_cstr,							\
	slice: string_put_slice) 						\
	((str), (x))
 
void string_put_cstr(string str, const char* cstr);
void string_put_slice(string str, slice s);

u64 string_length(const string str);
u64 string_allocated(const string str);

string string_concatenate(arena* arn, const string stra, const string strb);
string string_insert(arena* arn, const string stra, const string strb, u64 index);
void string_insert_inplace(string stra, const string strb, u64 index);

void slice_replace(slice s, const string replacement);
void slice_replace_cstr(slice s, const char* cstr);

slice string_match(const string str, const string find);
slice string_match_cstr(const string str, const char* find);
slice slice_match(slice s, const string find);
slice slice_match_cstr(slice s, const char* find);

//TODO(zgayford): implement
slice* string_match_all(arena* arn, const string str, const string find, u64* count);
//TODO(zgayford): implement
void string_replace_all(arena* arn, string str, const string replacement);

slice* string_dice(arena* arn, const string str, const string delim, u64* count);

#endif
