#ifndef SDF_H
#define SDF_H

#include <iostream>
#include <vector>
#include <limits>

#include <Godot.hpp>
#include <ReferenceRect.hpp>
#include <Polygon2D.hpp>

#include "util.h"

namespace godot {

class SDF : public ReferenceRect {
  GODOT_CLASS(SDF, ReferenceRect)

public:
  static void _register_methods() {
    register_method("calculate_from_polygon", &godot::SDF::calculate_from_polygon);
    register_method("get_value", &godot::SDF::get_value);
    register_method("get_gradient", &godot::SDF::get_gradient);
  }

  SDF() {}
  ~SDF() {}

  void _init() {

    std::cout << "Initializing SDF" << std::endl; 

    _scale = Vector2(1.0, 1.0);
    _width = 1;
    _height = 1;
    values.push_back(0.0);
    gradients.push_back(Vector2(0.0,0.0));
  }

  inline int idx(Vector2 p) const {
    return idx(p.x / _scale.x, p.y / _scale.y);
  }
  inline int idx(int i, int j) const {
    return clamp<int>(i, 0, _width - 1) * _height
          + clamp<int>(j, 0, _height - 1);
  }
  inline float get_value(Vector2 pos) {
    return values[idx(pos)];
  }
  inline Vector2 get_gradient(Vector2 pos) {
    return gradients[idx(pos)];
  }

  void calculate_from_polygon(
    Polygon2D * p_poly
  ) {

    _scale = get_scale();
    _width = get_size().x;
    _height = get_size().y;

    std::cout << "calculating with" << std::endl;
    std::cout << _scale.x << " " << _scale.y << std::endl;
    std::cout << _width << std::endl;
    std::cout << _height << std::endl;

    values.clear();
    gradients.clear();
    values.reserve(_width * _height);
    gradients.reserve(_width * _height);

    const auto v = p_poly->get_polygon().read();
    const auto N = p_poly->get_polygon().size();

    for (int i = 0; i < _width; i++) {
      for (int j = 0; j < _height; j++) {

        const auto p = Vector2(i * _scale.x, j * _scale.y);

        Vector2 r;
        float s = 1.0;
        //float d = (p-v[0]).length_squared();
        float d = std::numeric_limits<float>::max();
        int l = N-1;
        for (int k = 0; k < N; k++) {

            // distance
            auto e = v[l] - v[k];
            auto w = p - v[k];
            auto b = w - e * clamp<float>(w.dot(e) / e.length_squared(), 0.0, 1.0);
            auto b_ls = b.length_squared();

            if (d > b_ls) {
                d = b_ls;
                r = b;
            }
            if ((p.y>=v[k].y && p.y<v[l].y && e.x*w.y>e.y*w.x)
            ||  (p.y<v[k].y && p.y>=v[l].y && e.x*w.y<=e.y*w.x)
            ) {
                s *= -1.0;
            } 

            l = k;

        }

        d = r.length();
        values.push_back(d * s);
        gradients.push_back(s * r/(d == 0 ? 1.0 : d));
      }
    }
  }

  Vector2 _scale;
  int _width;
  int _height;

  std::vector<float> values;
  std::vector<Vector2> gradients;

};

}

#endif