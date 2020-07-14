#ifndef GDBOIDS_H
#define GDBOIDS_H

#include <iostream>
#include <Godot.hpp>
#include <Node.hpp>

namespace godot {

class GDBoids : public Node {
  GODOT_CLASS(GDBoids, Node)

public:
  static void _register_methods() {
    register_method("_physics_process", &godot::GDBoids::_physics_process);
  }

  GDBoids() {}
  ~GDBoids() {}

  void _init() {}

  void _physics_process(float delta) {
    std::cout << "qwer" << std::endl;
  }
};

}

#endif
