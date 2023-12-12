#include "string.h"

#include <string.h>

#include "defines.h"
#include "core/assert.h"

#define string_get_header(str) (((string_header*)str) - 1)

typedef struct {
	u32 allocated;
	u32 length;
	char data[];
} string_header;

//
// internal functions
//

static inline void string_header_put(string_header* header, const char* cstr) {

	u32 length = 0;

	while (cstr[length] && length < header->allocated) {
		header->data[length] = cstr[length];
		length++;
	}

	header->data[header->allocated] = 0;
	header->length = length;

}

static inline u64 cstr_length(const char* cstr) {
	u64 length = 0;
	while (cstr[length++]);
	return length;
}

static inline u64 string_match_count(string str, string find) {

	u64 index = 0;
	u64 matchIndex = 0;
	u64 matchCount = 0;
	u64 strLength = string_get_header(str)->length;
	u64 findLength = string_get_header(find)->length;

	while (index <= strLength) {
		if (matchIndex == findLength) {
			matchCount++;
			matchIndex = 0;
		}
		if (str[index] != find[matchIndex++])
			matchIndex = 0;
		index++;
	}
	
	return matchCount;

}

//
// external functions
//

string string_create(arena* arn, u32 length) {

	string_header* header = arena_alloc(arn, sizeof(string_header) + length + 1);
	header->allocated = length + 1;
	header->length = 0;

	return header->data;

}

string stringify_cstr(arena* arn, const char* cstr) {

	u32 length = cstr_length(cstr);

	string_header* header = arena_alloc(arn, sizeof(string_header) + length + 1);
	header->allocated = length + 1;

	string_header_put(header, cstr);
	
	return header->data;

}

string stringify_slice(arena* arn, slice s) {

	u32 length = s.end - s.start;

	string_header* header = arena_alloc(arn, sizeof(string_header) + length + 1);
	header->allocated = length + 1;
	header->length = length;
	header->data[length] = 0;

	while (length--)
		header->data[length] = s.start[length];

	return header->data;

}

void string_put_cstr(string str, const char* cstr) {

	u32 length = 0;
	string_header* header = string_get_header(str);

	while (cstr[length] && length < header->allocated) {
		header->data[length] = cstr[length];
		length++;
	}

	header->length = length;
	header->data[length] = 0;

}

void string_put_slice(string str, slice s) {

	string_header* header = string_get_header(str);
	u32 length = s.end - s.start;
	length = MIN(length, header->allocated - 1);

	header->length = length;
	header->data[length] = 0;

	while (length--)
		header->data[length] = s.start[length];


}

u64 string_length(const string str) {
	return string_get_header(str)->length;
}

u64 string_allocated(const string str) {
	return string_get_header(str)->allocated;
}

string string_concatenate(arena* arn, const string stra, const string strb) {

	u64 alength = string_length(stra);
	u64 blength = string_length(strb);

	string compound = string_create(arn, alength + blength);

	memcpy(compound, stra, alength);
	memcpy(compound + alength, strb, blength);

	return compound;

}

string string_insert(arena* arn, const string stra, const string strb, u64 index) {

	u64 lengtha = string_length(stra);
	u64 lengthb = string_length(strb);

	assert(index < lengtha, "invalid index!");

	string compound = string_create(arn, lengtha + lengthb);

	memcpy(compound, stra, lengtha);
	memcpy(compound + index, strb, lengthb);

	return compound;

}

void string_insert_inplace(string stra, const string strb, u64 index) {

	u64 allocated = string_allocated(stra);
	u64 lengtha = string_length(stra);
	u64 lengthb = string_length(strb);

	assert(index < lengtha, "invalid index!");
	assert(index + lengthb < allocated, "not enought space allocated!");

	memcpy(stra + index, strb, lengthb);

}

void slice_replace(slice s, const string replacement) {

	while (s.start <= s.end) {
		(*s.start) = replacement[s.end - s.start];
		s.start++;
	}

}

void slice_replace_cstr(slice s, const char* replacement) {

	while (s.start <= s.end) {
		(*s.start) = replacement[s.end - s.start];
		s.start++;
	}

}

slice string_match(const string str, const string find) {

	u64 index = 0;
	u64 matchIndex = 0;
	u64 strLength = string_get_header(str)->length;
	u64 findLength = string_get_header(find)->length;

	while (index <= strLength) {
		if (matchIndex == findLength) break;
		if (str[index] != find[matchIndex++])
			matchIndex = 0;
		index++;
	}

	if (matchIndex == findLength)
		return (slice) {
			.start = str + index - findLength,
			.end = str + index,
		};
	else
		return (slice) {
			.start = null,
			.end = null,
		};

}

slice string_match_cstr(const string str, const char* find) {

	u64 index = 0;
	u64 matchIndex = 0;
	u64 strLength = string_get_header(str)->length;
	u64 findLength = cstr_length(find);

	while (index <= strLength) {
		if (matchIndex == findLength) break;
		if (str[index] != find[matchIndex++])
			matchIndex = 0;
		index++;
	}

	if (matchIndex == findLength)
		return (slice) {
			.start = str + index - findLength,
			.end = str + index,
		};
	else
		return (slice) {
			.start = null,
			.end = null,
		};

}

slice slice_match(slice s, const string find) {

	u64 index = 0;
	u64 matchIndex = 0;
	u64 strLength = s.end - s.start;
	u64 findLength = string_get_header(find)->length;

	while (index <= strLength) {
		if (matchIndex == findLength) break;
		if (s.start[index] != find[matchIndex++])
			matchIndex = 0;
		index++;
	}

	if (matchIndex == findLength)
		return (slice) {
			.start = s.start + index - findLength,
			.end = s.start + index,
		};
	else
		return (slice) {
			.start = null,
			.end = null,
		};

}

slice slice_match_cstr(slice s, const char* find) {

	u64 index = 0;
	u64 matchIndex = 0;
	u64 strLength = s.end - s.start;
	u64 findLength = string_get_header(find)->length;

	while (index <= strLength) {
		if (matchIndex == findLength) break;
		if (s.start[index] != find[matchIndex++])
			matchIndex = 0;
		index++;
	}

	if (matchIndex == findLength)
		return (slice) {
			.start = s.start + index - findLength,
			.end = s.start + index,
		};
	else
		return (slice) {
			.start = null,
			.end = null,
		};

}

//TODO(zgayford): implement
slice* string_match_all(arena* arn, const string str, const string find, u64* count) {
	return null;
}
//TODO(zgayford): implement
void string_replace_all(arena* arn, string str, const string replacement) {

}

//TODO(zgayford): do this with string_match_all instead
slice* string_dice(arena* arn, const string str, const string delim, u64* count) {

	u64 matchCount = string_match_count(str, delim) + 1;
	(*count) = matchCount;

	slice* sliceList = arena_alloc(arn, matchCount * sizeof(slice));

	slice match;
	slice window = {
		.start = str,
		.end = str + string_length(str)
	};

	u64 index = 0;
	char* start = str;

	while (--matchCount) {
		match = slice_match(window, delim);
		sliceList[index++] = (slice) {
			.start = start,
			.end = match.start
		};
		start = match.end;
		window.start = match.end;
	}

	// add the last inverted slice
	sliceList[index] = (slice) {
		.start = start,
		.end = str + string_length(str)
	};

	return sliceList;

}

#undef string_get_header
