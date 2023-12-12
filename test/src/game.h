#pragma once

#include <defines.h>
#include <attached.h>

b8 game_setup(attached_instance* instance);
b8 game_update(attached_instance* instance, f64 deltaTime);
b8 game_render(attached_instance* instance, f64 deltaTime);
void game_resize(attached_instance* instance, u32 width, u32 height);
