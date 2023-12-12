#include "application.h"

#include "defines.h"
#include "platform/platform.h"
#include "core/assert.h"
#include "core/logger.h"
#include "attached.h"

typedef struct {
	attached_instance* instance;
	platform_state platform;
	b8 running;
	b8 suspended;
	i16 width;
	i16 height;
	f64 lastTime;
} application_state;

static b8 applicationInitialized = false;
static application_state applicationState;

b8 application_create(attached_instance* instance) {

	if (applicationInitialized) {
		log_error("application already initialized!");
		return false;
	}

	applicationState.instance = instance;
	applicationState.running      = true;
	applicationState.suspended    = false;
	applicationState.width        = instance->applicationConfig.initialWidth;
	applicationState.height       = instance->applicationConfig.initialHeight;
	applicationState.lastTime     = 0.0f;

	if (!platform_create(
		&applicationState.platform,
		instance->applicationConfig.name,
		instance->applicationConfig.initialX,
		instance->applicationConfig.initialY,
		instance->applicationConfig.initialWidth,
		instance->applicationConfig.initialHeight)) {
		log_error("unable to create platform!");
		return false;
	}

	if (!instance->setup(instance)) {
		log_error("attached_instance failed to setup!");
		return false;
	}

	instance->resize(instance, applicationState.width, applicationState.height);

	applicationInitialized = true;

	return true;

}

u32 application_run() {

	while (applicationState.running) {

		if (!platform_poll_events(&applicationState.platform))
			applicationState.running = false;

		if (applicationState.suspended) break;

		if (!applicationState.instance->update(applicationState.instance, 0)) {
			log_error("attached_instance failed to update!");
			applicationState.running = false;
		}

		if (!applicationState.instance->render(applicationState.instance, 0)) {
			log_error("attached_instance failed to render!");
			applicationState.running = false;
		}

	}

	// explicitly set to false just in case
	applicationState.running = false;
	platform_destroy(&applicationState.platform);

	// TODO(zgayford): exit codes
	return 0;

}
