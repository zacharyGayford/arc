#pragma once

#include "core/application.h"

typedef struct attached_instance {
	b8 (*setup)(struct attached_instance* instance);
	b8 (*update)(struct attached_instance* instance, f64 deltaTime);
	b8 (*render)(struct attached_instance* instance, f64 deltaTime);
	void (*resize)(struct attached_instance* instance, u32 width, u32 height);
	application_config applicationConfig;
	void* state;
} attached_instance;
