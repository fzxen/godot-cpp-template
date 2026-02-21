#ifndef SKIP_IN_EDITOR

#include <godot_cpp/classes/engine.hpp>

namespace godot
{
#define SKIP_IN_EDITOR()                         \
  if (Engine::get_singleton()->is_editor_hint()) \
  {                                              \
    return;                                      \
  }
}

#endif