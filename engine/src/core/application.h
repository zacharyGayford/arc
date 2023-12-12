#pragma once

#include "defines.h"

struct attached_instance;

typedef struct {
	i16 initialX;
	i16 initialY;
	i16 initialWidth;
	i16 initialHeight;
	char* name;
} application_config;

api b8 application_create(struct attached_instance* instance);

api u32 application_run();
