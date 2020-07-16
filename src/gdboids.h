#ifndef GDBOIDS_H
#define GDBOIDS_H

#include <iostream>
#include <Godot.hpp>
#include <Node.hpp>
#include <PassiveParticles2D.hpp>
#include <PoolArrays.hpp>

#define RAND_RANGE(LO,HI) LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)))

namespace godot {

class GDBoids : public Node {
  GODOT_CLASS(GDBoids, Node)

public:
  static void _register_methods() {
    register_method("_physics_process", &godot::GDBoids::_physics_process);
    register_method("_process", &godot::GDBoids::_process);
    register_method("set_passive_particles", &godot::GDBoids::set_passive_particles);
  }

  GDBoids() {}
  ~GDBoids() {}

  void _init() {

    amount = 0;
    p_passive_particles = NULL;
    initialized = true;

  }

  void set_amount(int new_amount) {

    if (!initialized) return;

    if (amount == new_amount) return;

    states.resize(new_amount);
    positions.resize(new_amount);
    velocities.resize(new_amount);
    directions.resize(new_amount);

    auto w_states = states.write();
    auto w_positions = positions.write();
    auto w_directions = directions.write();

    for (int i = amount; i < new_amount; i++) {

      w_states[i] = 0.0;
      w_positions[i] = Vector2(
        RAND_RANGE(0.0, 1000.0),
        RAND_RANGE(0.0, 1000.0)
      );
      w_directions[i] = Vector2(1.0, 0.0);
    }

    amount = new_amount;
  }

  void _physics_process(float delta) {

    if (!initialized) return;

    auto w_positions = positions.write();
    auto w_velocities = velocities.write();

    for (int i = 0; i < amount; i++) {
      w_velocities[i] -= w_positions[i] * delta;
      w_positions[i] += w_velocities[i] * delta;
    }

  }

  void _process(float _delta) {

    if (!initialized) return;

    if (p_passive_particles == NULL) {
      std::cout << "no passive particles set" << std::endl;
      return;
    } 

    if (p_passive_particles->get_amount() != amount) {
      set_amount(p_passive_particles->get_amount());
    }

    p_passive_particles->write_data(states, positions, directions);

  }

  void set_passive_particles(PassiveParticles2D* p) {
    p_passive_particles = p;
  }

private:

  bool initialized = false;

  int amount;

  PoolRealArray states;
  PoolVector2Array positions;
  PoolVector2Array velocities;
  PoolVector2Array directions;

  PassiveParticles2D * p_passive_particles;

};

}

#endif
