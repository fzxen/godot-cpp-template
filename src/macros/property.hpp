#ifndef PROPERTY

namespace godot
{
#define READONLY_PROPERTY(TYPE, NAME, DEFAULT_VALUE) \
private:                                             \
  TYPE NAME = DEFAULT_VALUE;                         \
                                                     \
public:                                              \
  TYPE get_##NAME() const { return NAME; }

#define WRITEABLE_PROPERTY(TYPE, NAME, DEFAULT_VALUE) \
private:                                              \
  TYPE NAME = DEFAULT_VALUE;                          \
                                                      \
public:                                               \
  void set_##NAME(TYPE p_value) { NAME = p_value; }   \
  TYPE get_##NAME() const { return NAME; }
}
#endif