#ifndef SDF_H
#define SDF_H

#include <vector>
#include <limits>

#include <Godot.hpp>
#include <Node.hpp>
#include <Polygon2D.hpp>

#include "util.h"

namespace godot {

class SDF : public Node {
  GODOT_CLASS(SDF, Node)

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

    _scale = 1.0;
    _width = 0;
    _height = 0;

  }

  inline int idx(Vector2 p) const {
    return idx(p.x / _scale, p.y / _scale);
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
    float scale,
    int width,
    int height,
    Polygon2D * p_poly
  ) {

    _scale = scale;
    _width = width;
    _height = height;

    values.reserve(width * height);
    gradients.reserve(width * height);

    const auto v = p_poly->get_polygon().read();
    const auto N = p_poly->get_polygon().size();

    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {

        const auto p = Vector2(i, j) * scale;

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

  float _scale;
  int _width;
  int _height;

  std::vector<float> values;
  std::vector<Vector2> gradients;

};

}

#endif