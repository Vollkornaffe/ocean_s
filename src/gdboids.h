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
    register_method("physics_process", &godot::GDBoids::physics_process);
    register_method("write_to_particles", &godot::GDBoids::write_to_particles);

    register_property<GDBoids, int>("base/amount", &GDBoids::set_amount, &GDBoids::get_amount, 0);
    register_property<GDBoids, float>("base/animation_speed", &GDBoids::_animation_speed, 1.0);
    register_property<GDBoids, float>("base/overall_speed", &GDBoids::_overall_speed, 1.0);
    register_property<GDBoids, float>("base/damping", &GDBoids::_damping, 1.0);
  }

  enum State {
    STATE_FISH_RED,
    STATE_FISH_GREEN,
    STATE_FISH_BLUE,
    STATE_MAX,
  };

  GDBoids() {}
  ~GDBoids() {}

  void _init() {

    std::cout << "Initializing GDBoids" << std::endl; 

    _amount = 0;
    _animation_speed = 1.0;
    initialized = true;

  }

  void set_amount(int new_amount) {

    if (!initialized) return;

    if (_amount == new_amount) return;

    states.resize(new_amount);
    animation_phases.resize(new_amount);
    animation_offsets.resize(new_amount);
    positions.resize(new_amount);
    velocities.resize(new_amount);
    directions.resize(new_amount);

    auto w_states = states.write();
    auto w_animation_phases = animation_phases.write();
    auto w_animation_offsets = animation_offsets.write();
    auto w_positions = positions.write();
    auto w_velocities = velocities.write();
    auto w_directions = directions.write();

    for (int i = _amount; i < new_amount; i++) {

      w_states[i] = rand() % STATE_MAX;
      w_animation_phases[i] = 0.0;
      w_animation_offsets[i] = (float) w_states[i] / (float) STATE_MAX;
      w_positions[i] = Vector2(
        RAND_RANGE(0.0, 1000.0),
        RAND_RANGE(0.0, 1000.0)
      );
      w_velocities[i] = Vector2(
        RAND_RANGE(-100.0, 100.0),
        RAND_RANGE(-100.0, 100.0)
      );
      w_directions[i] = Vector2(1.0, 0.0);
    }

    _amount = new_amount;
  }

  int get_amount() {
    return _amount;
  }

  void physics_process(Vector2 goal, float delta) {

    if (!initialized) return;

    auto w_states = states.write();
    auto w_animation_phases = animation_phases.write();
    auto w_animation_offsets = animation_offsets.write();
    auto w_positions = positions.write();
    auto w_velocities = velocities.write();
    auto w_directions = directions.write();

    for (int i = 0; i < _amount; i++) {
      w_velocities[i] += _overall_speed * (goal - w_positions[i]) * delta;
      w_velocities[i] -= _damping * w_velocities[i] * delta;
      w_positions[i] += w_velocities[i] * delta;

      auto vel_norm = w_velocities[i].length();
      w_animation_phases[i] = fmod(w_animation_phases[i] + 0.001 * vel_norm * _animation_speed, 1.0);
      w_animation_offsets[i] = ((float) w_states[i] + w_animation_phases[i]) / (float) STATE_MAX;
      w_directions[i] = (w_velocities[i] / (vel_norm == 0.0 ? 1.0 : vel_norm)).tangent();
    }

  }

  void write_to_particles(PassiveParticles2D* p) {

    if (!initialized) return;

    if (p->get_amount() != _amount) {
      set_amount(p->get_amount());
    }

    p->write_data(animation_offsets, positions, directions);

  }

  float _animation_speed;
  float _overall_speed;
  float _damping;

private:

  bool initialized = false;

  int _amount;

  PoolIntArray states;
  PoolRealArray animation_phases;
  PoolRealArray animation_offsets;
  PoolVector2Array positions;
  PoolVector2Array velocities;
  PoolVector2Array directions;

};

}

#endif
