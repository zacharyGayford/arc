#include <defines.h>
#include <platform/platform.h>

int main(void) {
	
	platform_state state;
	if (platform_create(&state, "test", 500, 500, 500, 500)) {
		while (true) {
			platform_poll_events(&state);
		}
	}

	return 0;
}
