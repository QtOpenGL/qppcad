#ifndef QPP_CAD_MARCHING_CUBES_H
#define QPP_CAD_MARCHING_CUBES_H

#include <qppcad/qppcad.hpp>
#include <qppcad/mesh.hpp>
#include <io/cube.hpp>
#include <symm/cell.hpp>

namespace qpp {

  namespace cad {

    class volume_helper {
      public:
        //based on http://paulbourke.net/geometry/polygonise/
        static void polygonise_volume_mc(mesh_t &mesh,
                                         scalar_volume_t<float> &volume,
                                         float isolevel,
                                         int steps);

        static void polygonise_vol_mc(mesh_t &mesh,
                                               scalar_volume_t<float> &volume,
                                               float isolevel,
                                               int steps);

        static void comp_bounding_cube(vector3<float> &va,
                                       vector3<float> &vc,
                                       vector3<float> &vb,
                                       vector3<float> &start,
                                       float &cube_a);

        static float get_value_from_cube(vector3<float> &pos,
                                         scalar_volume_t<float> &volume);

        static vector3<float> get_vertex_normal_from_cube(const int ix,
                                                          const int iy,
                                                          const int iz,
                                                          scalar_volume_t<float> &volume);

        static float get_value_from_cube_interpolated(vector3<float> &pos,
                                                      scalar_volume_t<float> &volume);

        static vector3<float> vertex_interp(float isolevel,
                                            vector3<float> &p1, vector3<float> &p2,
                                            float val_p1, float val_p2);
    };

  }

}

#endif
