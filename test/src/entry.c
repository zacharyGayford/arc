#include <defines.h>
#include <entry.h>

#include "game.h"

b8 application_attach_instance(attached_instance* instance) {

	instance->applicationConfig.initialX = 100;
	instance->applicationConfig.initialY = 100;
	instance->applicationConfig.initialWidth = 500;
	instance->applicationConfig.initialHeight = 500;
	instance->applicationConfig.name = "arc-test";

	instance->setup  = &game_setup;
	instance->update = &game_update;
	instance->render = &game_render;
	instance->resize = &game_resize;

	return true;

}
