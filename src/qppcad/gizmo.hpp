#ifndef QPP_GIZMO_H
#define QPP_GIZMO_H
#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>
#include <geom/primitive_intersections.hpp>
#include <qppcad/ws_item.hpp>

namespace qpp {

  const vector3<float> gizmo_axis[3] = {
    vector3<float>(1, 0, 0),
    vector3<float>(0, 1, 0),
    vector3<float>(0, 0, 1)
  };

  class gizmo_t {
    public:
      vector3<float> pos;
      float gizmo_box_size;
      float gizmo_shift_magnitude;
      bool is_active;
      uint8_t touched_axis;
      std::array<aabb_3d<float>,3> bx;
      std::array<bool, 3> bx_touched;
      ws_item_t *attached_item;

      template<typename REAL>
      bool process_ray(ray<REAL> *ray){
        bool _gizmo_touched = false;
        if (ray) {
            touched_axis = 4;
            for(uint8_t i = 0; i < 3; i++){
                aabb_3d<float> aabb_in_world_frame = bx[i].shifted(pos);
                if (ray_aabb_test(ray, &aabb_in_world_frame)){
                    touched_axis = i;
                    bx_touched[i] = true;
                    _gizmo_touched = true;
                  }
                else
                  bx_touched[i] = false;
              }
          }
        return _gizmo_touched;
      }

      void transform_attached_object(float delta_time);
      void clear_selected_axis();
      void update_gizmo(float delta_time);
      void render();

      gizmo_t();
  };
}
#endif
