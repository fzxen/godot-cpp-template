#ifndef GD_PROPERTY

#include "property.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/string_name.hpp>

namespace godot
{

#define GD_BIND_GETTER_SETTER(CLASS_NAME, NAME)                                      \
  ClassDB::bind_method(D_METHOD("set_" #NAME, "p_" #NAME), &CLASS_NAME::set_##NAME); \
  ClassDB::bind_method(D_METHOD("get_" #NAME), &CLASS_NAME::get_##NAME);

#define EXPORT_PROPERTY(CLASS_NAME, TYPE, NAME, DEFAULT_VALUE, VARIANT_TYPE, HINT, HINT_STRING)     \
  WRITEABLE_PROPERTY(TYPE, NAME, DEFAULT_VALUE)                                                     \
  static void _bind_##NAME##_property()                                                             \
  {                                                                                                 \
    GD_BIND_GETTER_SETTER(CLASS_NAME, NAME)                                                         \
    ADD_PROPERTY(PropertyInfo(VARIANT_TYPE, #NAME, HINT, HINT_STRING), "set_" #NAME, "get_" #NAME); \
  }

#define EXPORT_NODE_PROPERTY(CLASS_NAME, TYPE, NAME, HINT, HINT_STRING)                                \
  WRITEABLE_PROPERTY(TYPE, NAME, nullptr)                                                              \
  static void _bind_##NAME##_property()                                                                \
  {                                                                                                    \
    GD_BIND_GETTER_SETTER(CLASS_NAME, NAME)                                                            \
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #NAME, HINT, HINT_STRING), "set_" #NAME, "get_" #NAME); \
  }

#define EXPORT_PACKED_SCENE_PROPERTY(CLASS_NAME, NAME)                                                                          \
  WRITEABLE_PROPERTY(Ref<PackedScene>, NAME, nullptr)                                                                           \
  static void _bind_##NAME##_property()                                                                                         \
  {                                                                                                                             \
    GD_BIND_GETTER_SETTER(CLASS_NAME, NAME)                                                                                     \
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #NAME, PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_" #NAME, "get_" #NAME); \
  }

#define EXPORT_ARRAY_PROPERTY(CLASS_NAME, ITEM_TYPE, NAME)                                                              \
  WRITEABLE_PROPERTY(TypedArray<ITEM_TYPE>, NAME, {})                                                                   \
  static void _bind_##NAME##_property()                                                                                 \
  {                                                                                                                     \
    GD_BIND_GETTER_SETTER(CLASS_NAME, NAME)                                                                             \
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #NAME, PROPERTY_HINT_ARRAY_TYPE, ITEM_TYPE), "set_" #NAME, "get_" #NAME); \
  }

#define EXPORT_ENUM_PROPERTY(CLASS_NAME, TYPE, NAME, DEFAULT_VALUE, HINT_STRING)                                  \
private:                                                                                                          \
  TYPE NAME = DEFAULT_VALUE;                                                                                      \
  int _NAME = DEFAULT_VALUE;                                                                                      \
                                                                                                                  \
  static void _bind_##NAME##_property()                                                                           \
  {                                                                                                               \
    GD_BIND_GETTER_SETTER(CLASS_NAME, NAME);                                                                      \
    ADD_PROPERTY(PropertyInfo(Variant::INT, #NAME, PROPERTY_HINT_ENUM, HINT_STRING), "set_" #NAME, "get_" #NAME); \
  }                                                                                                               \
                                                                                                                  \
public:                                                                                                           \
  int get_##NAME() const { return NAME; }                                                                         \
  void set_##NAME(int p_##NAME)                                                                                   \
  {                                                                                                               \
    _NAME = p_##NAME;                                                                                             \
    NAME = static_cast<TYPE>(_NAME);                                                                              \
  }
}

#endif
