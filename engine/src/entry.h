#include "core/application.h"
#include "core/logger.h"
#include "attached.h"

extern b8 application_attach_instance(attached_instance* instance);

int main(void) {
	
	attached_instance instance;

	if (!application_attach_instance(&instance)) {
		log_error("unable to create game!");
		return -1;
	}

	if (!instance.setup ||
	    !instance.update ||
	    !instance.render ||
	    !instance.resize) {
		log_error("function pointers not assigned!");
		return -2;
	}

	if (!application_create(&instance)) {
		log_error("failed to create application!");
		return 1;
	}

	return application_run();

}
