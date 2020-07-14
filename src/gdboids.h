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
    register_method("_process", &godot::GDBoids::_process);
  }

  GDBoids() {}
  ~GDBoids() {}

  void _init() {}

  void _process(float delta) {
    std::cout << "asdf" << std::endl;
  }
};

}

#endif
