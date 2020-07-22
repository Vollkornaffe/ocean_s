#ifndef GDBOIDS_H
#define GDBOIDS_H

#include <iostream>
#include <Godot.hpp>
#include <Node.hpp>
#include <PassiveParticles2D.hpp>
#include <PoolArrays.hpp>

#include "sdf.h"

#define RAND_RANGE(LO,HI) LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)))

namespace godot {

class GDBoids : public Node {
  GODOT_CLASS(GDBoids, Node)

public:
  static void _register_methods() {
    register_method("physics_process", &godot::GDBoids::physics_process);
    register_method("write_to_particles", &godot::GDBoids::write_to_particles);


    register_property<GDBoids, int>("amount", &GDBoids::set_amount, &GDBoids::get_amount, 0);
    register_property<GDBoids, float>("animation_speed", &GDBoids::_animation_speed, 1.0);
    register_property<GDBoids, float>("to_mouse", &GDBoids::_to_mouse, 1.0);
    register_property<GDBoids, float>("damping", &GDBoids::_damping, 1.0);
    register_property<GDBoids, float>("boundary_range", &GDBoids::_boundary_range, 20.0);
    register_property<GDBoids, float>("boundary_force", &GDBoids::_boundary_range, 1.0);
  }

  enum Species {
    SPECIES_FISH_RED,
    SPECIES_FISH_GREEN,
    SPECIES_FISH_BLUE,
    SPECIES_MAX,
  };

  GDBoids() {}
  ~GDBoids() {}


  void _init() {

    std::cout << "Initializing GDBoids" << std::endl; 

    _amount = 0;
    _animation_speed = 1.0;
    _to_mouse = 1.0;
    _damping = 1.0;
    _boundary_range = 20.0;
    _boundary_force = 1.0;
    initialized = true;

  }

  void set_amount(int new_amount) {

    if (!initialized) return;

    if (_amount == new_amount) return;

    selects.resize(new_amount);
    species.resize(new_amount);
    animation_phases.resize(new_amount);
    velocities.resize(new_amount);
    actives.resize(new_amount);
    custom_data_xy.resize(new_amount);
    custom_data_zw.resize(new_amount);
    positions.resize(new_amount);
    directions.resize(new_amount);

    auto w_selects = selects.write();
    auto w_species = species.write();
    auto w_animation_phases = animation_phases.write();
    auto w_velocities = velocities.write();
    auto w_actives = actives.write();
    auto w_custom_data_xy = custom_data_xy.write();
    auto w_custom_data_zw = custom_data_zw.write();
    auto w_positions = positions.write();
    auto w_directions = directions.write();

    for (int i = _amount; i < new_amount; i++) {

      w_actives[i] = 1;
      w_selects[i] = 0;
      w_species[i] = rand() % SPECIES_MAX;
      w_animation_phases[i] = 0.0;
      w_custom_data_zw[i].x = (float) w_species[i] / (float) SPECIES_MAX;
      w_positions[i] = Vector2(
        RAND_RANGE(0.0, 1000.0),
        RAND_RANGE(0.0, 1000.0)
      );
      w_velocities[i] = Vector2(0.0, 0.0);
      w_directions[i] = Vector2(1.0, 0.0);
    }

    _amount = new_amount;
  }

  int get_amount() {
    return _amount;
  }

  void physics_process(Vector2 goal, SDF * sdf, float delta) {

    if (!initialized) return;

    auto w_selects = selects.write();
    auto w_species = species.write();
    auto w_animation_phases = animation_phases.write();
    auto w_velocities = velocities.write();
    auto w_actives = actives.write();
    auto w_custom_data_xy = custom_data_xy.write();
    auto w_custom_data_zw = custom_data_zw.write();
    auto w_positions = positions.write();
    auto w_directions = directions.write();

    auto sdf_scale = sdf->get_scale();

    for (int i = 0; i < _amount; i++) {

      if (!w_actives[i]) continue;

      auto position = w_positions[i];
      auto velocity = w_velocities[i];
      auto force = Vector2(0.0, 0.0);

      // boundary
      auto sdf_index = sdf->idx(position.x / sdf_scale.x, position.y / sdf_scale.y);
      auto sdf_value = sdf->values[sdf_index];
      auto sdf_gradient = sdf->gradients[sdf_index];
      if (sdf_value < 0.0) {
        position -= sdf_value * sdf_gradient;
        velocity -= sdf_gradient * sdf_gradient.dot(velocity);
      } else if (sdf_value < _boundary_range) {
        force -= (sdf_value - _boundary_range)  * sdf_gradient * _boundary_force;
      }

      force += _to_mouse * (goal - position);
      force -= _damping * velocity;

      velocity += force * delta;
      position += velocity * delta;

      auto vel_norm = velocity.length();
      w_animation_phases[i] = fmod(w_animation_phases[i] + 0.001 * vel_norm * _animation_speed, 1.0);
      w_custom_data_zw[i].x = ((float) w_species[i] + w_animation_phases[i]) / (float) SPECIES_MAX;
      w_directions[i] = vel_norm == 0.0 ? w_directions[i] : (velocity / vel_norm).tangent();

      // writing
      w_positions[i] = position;
      w_velocities[i] = velocity;
    }

  }

  void write_to_particles(PassiveParticles2D* p) {

    if (!initialized) return;

    if (p->get_amount() != _amount) {
      set_amount(p->get_amount());
    }

    p->write_data(actives, custom_data_xy, custom_data_zw, positions, directions);

  }

  int _amount;
  float _animation_speed;
  float _to_mouse;
  float _damping;
  float _boundary_range;
  float _boundary_force;

private:

  bool initialized = false;

  PoolIntArray selects;
  PoolIntArray species;
  PoolRealArray animation_phases;
  PoolVector2Array velocities;

  // these are passed to the passive particles
  PoolIntArray actives;
  PoolVector2Array custom_data_xy;
  PoolVector2Array custom_data_zw;
  PoolVector2Array positions;
  PoolVector2Array directions;

};

}

#endif
