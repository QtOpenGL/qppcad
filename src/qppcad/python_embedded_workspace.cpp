#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/ws_item_trampoline.hpp>
#include <qppcad/workspace.hpp>
#include <qppcad/geom_view/geom_view.hpp>
#include <qppcad/ws_point_sym_group/ws_point_sym_group.hpp>

using namespace qpp;
using namespace qpp::cad;

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(workspace_stuff, m) {

  py::class_<workspace_manager_t,  std::shared_ptr<workspace_manager_t> >(m, "workspace_manager_t")
      .def("__len__", [](const workspace_manager_t &wsm){ return wsm.m_ws.size();})
      .def("__getitem__", [](const workspace_manager_t &wsm, size_t i) {
        if (i >= wsm.m_ws.size()) throw py::index_error();
        return wsm.m_ws[i];
      }, py::return_value_policy::reference_internal)
      .def("__getitem__", [](workspace_manager_t &wsm, std::string ws_name) {
        auto retv = wsm.get_by_name(ws_name);
        if (!retv) throw py::key_error();
        return retv;
       }, py::return_value_policy::reference_internal)
      .def("has_wss", &workspace_manager_t::has_wss)
      .def("set_cur", &workspace_manager_t::set_cur_id);

  py::class_<workspace_t,  std::shared_ptr<workspace_t> >(m, "workspace_t")
      .def_readwrite("name", &workspace_t::m_ws_name)
      .def("__len__", [](workspace_t &ws) {return ws.m_ws_items.size();})
      .def("__getitem__", [](workspace_t &ws, size_t i) {
        if (i >= ws.m_ws_items.size()) throw py::index_error();
        return ws.m_ws_items[i];
      }, py::return_value_policy::reference_internal, py::keep_alive<0,2>())
      .def("__getitem__", [](workspace_t &ws, std::string item_name) {
        auto retv = ws.get_by_name(item_name);
        if (!retv) throw py::key_error();
        return retv;
       }, py::return_value_policy::reference_internal, py::keep_alive<0,2>())
      .def("construct_item", &workspace_t::py_construct_item)
      .def("__repr__", &workspace_t::py_get_repr)
      .def("__str__", &workspace_t::py_get_repr);

  py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> py_ws_item_t(m, "ws_item_t");
  py_ws_item_t
          .def_readwrite("name", &ws_item_t::m_name)
          .def("get_cnt_count", &ws_item_t::get_content_count)
          .def("get_parent_ws", [](ws_item_t &wsi){return wsi.m_parent_ws;})
         // .def_readwrite("m_pos", &ws_item_t::get_pos, &ws_item_t::set_pos)
          .def("__repr__", &ws_item_t::py_get_repr)
          .def("__str__", &ws_item_t::py_get_repr);

  py::class_<geom_view_t, std::shared_ptr<geom_view_t> >
  py_atoms_list_t(m, "geom_view_t", py_ws_item_t);
  py_atoms_list_t.def_property_readonly("geom",
                                        [](geom_view_t &self) {return self.m_geom.get();},
                                        py::return_value_policy::reference);

  py::class_<ws_point_sym_group_t, std::shared_ptr<ws_point_sym_group_t> >
  py_point_sym_group_t(m, "ws_point_sym_group_t", py_ws_item_t);
  py_point_sym_group_t.def_readwrite("ag", &ws_point_sym_group_t::m_ag);
  py_point_sym_group_t.def_readwrite("pg_axes", &ws_point_sym_group_t::m_pg_axes);
  py_point_sym_group_t.def("update_view", &ws_point_sym_group_t::py_update);
  py_point_sym_group_t.def_readwrite("center", &ws_point_sym_group_t::m_new_centre);

}
