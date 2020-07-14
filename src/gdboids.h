#ifndef GDBOIDS_H
#define GDBOIDS_H

#include <iostream>
#include <Godot.hpp>
#include <Node.hpp>
#include <PassiveParticles2D.hpp>

namespace godot {

class GDBoids : public Node {
  GODOT_CLASS(GDBoids, Node)

public:
  static void _register_methods() {
    register_method("_physics_process", &godot::GDBoids::_physics_process);
    register_method("set_passive_particles", &godot::GDBoids::set_passive_particles);
  }

  GDBoids() {}
  ~GDBoids() {}

  void _init() {
    p_passive_particles = NULL;
  }

  void _physics_process(float delta) {
    if (p_passive_particles == NULL) {
      std::cout << "no passive particles set" << std::endl;
      return;
    } 

    std::cout << p_passive_particles->get_amount() << std::endl;

  }

  void set_passive_particles(PassiveParticles2D* p) {
    p_passive_particles = p;
  }

private:
  PassiveParticles2D * p_passive_particles;
};

}

#endif
