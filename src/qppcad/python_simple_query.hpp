#ifndef QPP_CAD_PYTHON_SQ_H
#define QPP_CAD_PYTHON_SQ_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

namespace py = pybind11;

namespace qpp {

  namespace cad {

    class simple_query {

      public:

        static void open_file(std::string file_name);
        static void open_file_query(std::string file_name, qc_file_fmt file_format);
        static void make_super_cell(const int sc_a, const int sc_b, const int sc_c);
        static void select_ws(int ws_idx);
        static void select_itm(int itm_idx);
        static void sel_cnt(int cnt_idx);
        static void sel_cnt_parity();
        static void sel_invert();
        static void sel_cnt_all();
        static void sel_cnt_list(py::list sel_list);
        static void sel_cnt_type(py::str sel_type);
        static void sel_cnt_sphere(vector3<float> sph_center, float sph_rad);
        static void sel_hemisphere(int coord_idx, bool positive);
        static void unsel_cnt_all();
        static void unsel_cnt(int cnt_idx);
        static void unsel_cnt_list(py::list sel_list);
        static void unsel_cnt_type(py::str sel_type);
        static py::list get_sel();

        static vector3<float> gizmo_pos();

        static void edit_mode(int mode);
        static void rebond();
        static void translate_selected(float tx, float ty, float tz);
        static void set_charge(float charge);
        static void set_ws_bg(vector3<float> bg);

        static void ptable_set_color_by_number(int num, float r, float g, float b);
        static void ptable_set_color_by_name(std::string name, float r, float g, float b);
        static void ptable_set_radius_by_number(int num, float r);
        static void ptable_set_radius_by_name(std::string name, float r);

        static vector3<float> ptable_get_color_by_number(int num);
        static vector3<float> ptable_get_color_by_name(std::string name);
        static float ptable_get_radius_by_number(int num);
        static float ptable_get_radius_by_name(std::string name);

        static void camera_move(vector3<float> axis, float magnitude);
        static void camera_rotate_yaw(float magnitude);
        static void camera_rotate_pitch(float magnitude);
        static void camera_zoom(float magnitude);
        static void camera_mode(int mode);
        static void copy_camera_from_ws(int ws_id);

        static py::list sv_get();
        static void sv_edit(int at, bool status);
        static void sv_edit_list(py::list at_list, bool status);
        static void sv_edit_all(bool status);

        static py::list cl_get();
        static void set_cl_state(int at, bool status);
        static void set_cl_text(int at, std::string text);

    };

  }

}


#endif
