#include <qppcad/workspace.hpp>
#include <qppcad/app_state.hpp>
#include <data/ptable.hpp>
#include <io/geomio.hpp>
#include <mathf/math.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/ws_comp_chem_data.hpp>
#include <qppcad/ws_volume_data.hpp>

using namespace qpp;
using namespace qpp::cad;

std::shared_ptr<ws_item_t> ws_item_factory::create_object(const std::string &obj_type) {
  if (obj_type == "ws_atoms_list") return std::make_shared<ws_atoms_list_t>();
  if (obj_type == "ws_comp_chem_data") return std::make_shared<ws_comp_chem_data_t>();
  if (obj_type == "ws_volume_data") return std::make_shared<ws_volume_data_t>();
  return nullptr;
}

std::optional<size_t> workspace_t::get_selected_idx () {
  for (size_t i = 0; i < m_ws_items.size(); i++)
    if (m_ws_items[i]->m_selected) return std::optional<size_t>(i);
  return std::nullopt;
}

ws_item_t *workspace_t::get_selected () {
  std::optional<size_t> sel_idx = get_selected_idx();
  if (sel_idx) return m_ws_items[*sel_idx].get();
  else return nullptr;
}

std::shared_ptr<ws_item_t> workspace_t::get_selected_sp(){
  std::optional<size_t> sel_idx = get_selected_idx();
  if (sel_idx) return m_ws_items[*sel_idx];
  else return nullptr;
}

bool workspace_t::set_selected_item (const size_t sel_idx, bool emit_signal) {

  app_state_t* astate = app_state_t::get_inst();

  unselect_all();

  astate->log(fmt::format("workspace_t::set_selected_item ({} {})", sel_idx, emit_signal));

  if (sel_idx < m_ws_items.size() && !m_ws_items.empty()) {
      m_ws_items[sel_idx]->m_selected = true;
      if (m_ws_items[sel_idx]->get_flags() & ws_item_flags_support_translation) {
          m_gizmo->attached_item = m_ws_items[sel_idx].get();
          m_gizmo->update_gizmo(0.1f, true);
          astate->make_viewport_dirty();
        }
      else {
          m_gizmo->attached_item = nullptr;
          m_gizmo->update_gizmo(0.1f, true);
        }
      //astate->make_viewport_dirty();
      if (emit_signal) astate->astate_evd->cur_ws_selected_item_changed();
      return true;
    }

  //astate->make_viewport_dirty();
  if (emit_signal) astate->astate_evd->cur_ws_selected_item_changed();
  return false;
}

void workspace_t::unselect_all (bool emit_signal) {
  for (auto &ws_item : m_ws_items) ws_item->m_selected = false;
  app_state_t* astate = app_state_t::get_inst();
  if (emit_signal) astate->astate_evd->cur_ws_selected_item_changed();
}

void workspace_t::toggle_edit_mode () {

  app_state_t* astate = app_state_t::get_inst();

  if (m_edit_type == ws_edit_t::edit_item) m_edit_type = ws_edit_t::edit_content;
  else m_edit_type = ws_edit_t::edit_item;

  astate->astate_evd->cur_ws_edit_type_changed();

}

void workspace_t::ws_changed () {

}

void workspace_t::reset_camera () {
  m_camera->reset_camera();
  set_best_view();
}

void workspace_t::set_best_view () {

  if (m_ws_items.size() == 0) {
      m_camera->reset_camera();
      m_camera->update_camera();
      return;
    }

  vector3<float> vec_look_at  = vector3<float>(0.0, 0.0, 0.0);
  vector3<float> vec_look_pos = vector3<float>(0.0, 0.0, -3.0);

  for (auto &ws_item : m_ws_items)
    ws_item->vote_for_view_vectors(vec_look_pos, vec_look_at);

  vec_look_at  /= m_ws_items.size();
  vec_look_pos /= m_ws_items.size();

  m_camera->m_look_at = vec_look_at;
  m_camera->m_view_point = vec_look_pos;

  //  std::cout << "set bv " << _vLookAt << std::endl << _vLookPos << std::endl
  //            << "end bv " << std::endl;
  m_camera->orthogonalize_gs();
  m_camera->update_camera();

  if ((m_camera->m_look_at-m_camera->m_view_point).norm() < 0.4f || vec_look_at == vec_look_pos)
    m_camera->reset_camera();
}

void workspace_t::render() {

  app_state_t* astate = app_state_t::get_inst();

  if (m_gizmo->m_is_active && m_gizmo->attached_item) m_gizmo->render();

  if (astate->m_debug_show_selection_ray){
      astate->dp->begin_render_line();
      astate->dp->render_line(vector3<float>(1.0, 1.0, 0.0), m_ray.start,
                              m_ray.start + m_ray.dir * 155.0);
      astate->dp->end_render_line();
    }

  if (astate->dp) {

      ///// Draw axis /////
      if (astate->m_show_axis) {
          vector3<float> vScrTW = astate->camera->unproject(-0.95f, -0.90f);
          float axis_magn = 0.07f *astate->camera->m_stored_dist;
          if (astate->camera->m_cur_proj == cam_proj_t::proj_persp) axis_magn = 0.015f;
          else axis_magn = m_camera->m_ortho_scale * 0.1f;

          astate->dp->begin_render_line();
          astate->dp->
              render_line(vector3<float>(1.0, 0.0, 0.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(axis_magn, 0.0, 0.0) + vScrTW);

          astate->dp->
              render_line(vector3<float>(0.0, 1.0, 0.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(0.0, axis_magn, 0.0) + vScrTW);

          astate->dp->
              render_line(vector3<float>(0.0, 0.0, 1.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(0.0, 0.0, axis_magn) + vScrTW);
          astate->dp->end_render_line();

        }
      ///// Draw axis end /////
    }

  for (auto &ws_item : m_ws_items) ws_item->render();

}

void workspace_t::render_overlay(QPainter &painter) {
  for (auto &ws_item : m_ws_items)
    if (ws_item->m_is_visible) ws_item->render_overlay(painter);
}

void workspace_t::mouse_click (const float mouse_x, const float mouse_y) {

  //if (ImGui::GetIO().WantCaptureMouse) return;
  app_state_t* astate = app_state_t::get_inst();

  if (m_camera->m_cur_proj == cam_proj_t::proj_persp) {
      m_ray.start = m_camera->m_view_point;
      m_ray.dir = (m_camera->unproject(mouse_x, mouse_y) - m_camera->m_view_point).normalized();
    } else {
      float z_p =  (m_camera->m_znear_ortho + m_camera->m_zfar_ortho) /
                   (m_camera->m_znear_ortho - m_camera->m_zfar_ortho);

      m_ray.start = m_camera->unproject(mouse_x, mouse_y, z_p);
      m_ray.dir = (m_camera->m_look_at - m_camera->m_view_point).normalized();

    }

  if (m_gizmo->m_is_visible && m_gizmo->attached_item)
    if (m_gizmo->process_ray(&m_ray)){
        astate->log("gizmo clicked");
        return;
      }

  bool hit_any = false;

  if (m_edit_type != ws_edit_t::edit_content) {
      for (auto &ws_item : m_ws_items) ws_item->m_selected = false;
      m_gizmo->attached_item = nullptr;
    }

  for (auto &ws_item : m_ws_items) {
      bool is_hit = ws_item->mouse_click(&m_ray);
      hit_any = hit_any || is_hit;
      if (is_hit && m_edit_type == ws_edit_t::edit_item && ws_item->m_is_visible &&
          (ws_item->get_flags() & ws_item_flags_support_selection)) {
          m_gizmo->attached_item = ws_item.get();
          auto it = std::find(m_ws_items.begin(), m_ws_items.end(), ws_item);
          if (it != m_ws_items.end()) {
              auto index = std::distance(m_ws_items.begin(), it);
              set_selected_item(index);
              break;
            }
        }
    }

  if (m_edit_type != ws_edit_t::edit_content && !hit_any) {
      m_gizmo->attached_item = nullptr;
      unselect_all();
    }

}

void workspace_t::add_item_to_ws (const std::shared_ptr<ws_item_t> &item_to_add) {

  item_to_add->set_parent_workspace(this);
  m_ws_items.push_back(item_to_add);
  app_state_t::get_inst()->astate_evd->cur_ws_changed();
  //c_app::log(fmt::format("New workspace {} size = {}", m_ws_name, m_ws_items.size()));

}

void workspace_t::clear_connected_items(std::shared_ptr<ws_item_t> item_to_delete) {
  for (auto elem : m_ws_items) {
      auto it = std::find(elem->m_connected_items.begin(),
                          elem->m_connected_items.end(),
                          item_to_delete);
      if (it != elem->m_connected_items.end())
        elem->m_connected_items.erase(it);
    }
}

void workspace_t::save_ws_to_json (const std::string filename) {

  app_state_t* astate = app_state_t::get_inst();

  std::ofstream out_file(filename);
  json data;

  data[JSON_QPPCAD_VERSION] = "1.0-aa";
  data[JSON_WS_NAME] = m_ws_name;
  json j_workspace_background = json::array({m_background_color[0],
                                             m_background_color[1],
                                             m_background_color[2], });
  data[JSON_BG_CLR] = j_workspace_background;

  json ws_objects = json::array({});
  for (const auto &ws_item : m_ws_items)
    if (ws_item->can_be_written_to_json()) {
        json ws_object;
        ws_item->save_to_json(ws_object);
        ws_objects.push_back(ws_object);
      }

  data[JSON_OBJECTS] = ws_objects;

  out_file << data.dump(2);

  astate->astate_evd->new_file_loaded(filename, qc_file_fmt::qppcad_json);

}

void workspace_t::load_ws_from_json (const std::string filename) {

  app_state_t* astate = app_state_t::get_inst();

  std::fstream ifile(filename);
  json data;

  try {
    data = json::parse(ifile);

    if (data.find(JSON_WS_NAME) != data.end()) m_ws_name = data[JSON_WS_NAME];
    if (data.find(JSON_BG_CLR) != data.end())
      for (uint8_t i = 0; i < 3; i++) m_background_color[i] = data[JSON_BG_CLR][i];

    if (data.find(JSON_OBJECTS) != data.end()){
        json objects = data[JSON_OBJECTS];
        for (auto &object : objects)
          if (object.find(JSON_WS_ITEM_TYPE) != object.end()){
              std::string obj_type = object[JSON_WS_ITEM_TYPE];
              std::shared_ptr<ws_item_t> obj = ws_item_factory::create_object(obj_type);
              obj->load_from_json(object);
              add_item_to_ws(obj);
            } else {
              astate->log(fmt::format("WARNING: Cannot find type for object \"{}\" in file \"{}\"!",
                                      object[JSON_WS_ITEM_NAME].get<std::string>(), filename));
            }
      }

    m_fs_path = filename;

  } catch (json::parse_error &e) {
    std::cerr << e.what() << " " << std::endl;
  }

}

void workspace_t::update (float delta_time) {

  if (m_first_render) {
      set_best_view();
      m_first_render = false;
    }

  m_gizmo->update_gizmo(delta_time);

  //handle deletion
  for (auto it = m_ws_items.begin(); it != m_ws_items.end(); ) {
      if ((*it)->m_marked_for_deletion) {

          if (it->get() == m_gizmo->attached_item)
            m_gizmo->attached_item = nullptr;

          clear_connected_items(*it);
          it = m_ws_items.erase(it);
          ws_changed();

          app_state_t* astate = app_state_t::get_inst();
          astate->astate_evd->cur_ws_changed();
        }
      else {
          ++it;
        }
    }

  //update cycle
  for (auto &ws_item : m_ws_items) ws_item->update(delta_time);

}

void workspace_t::set_edit_type (const ws_edit_t new_edit_type) {

  app_state_t* astate = app_state_t::get_inst();
  m_edit_type = new_edit_type;
  astate->astate_evd->cur_ws_edit_type_changed();

}

workspace_manager_t::workspace_manager_t (app_state_t *_astate) {

  m_cur_ws_id = 0;
  cached_astate = _astate;

}

std::shared_ptr<workspace_t> workspace_manager_t::get_cur_ws () {
  if (m_cur_ws_id >= m_ws.size()) return nullptr;
  return m_ws[m_cur_ws_id];
}

std::optional<size_t> workspace_manager_t::get_cur_id () {
  if (!m_ws.empty()) return std::optional<size_t>(m_cur_ws_id);
  return std::nullopt;
}

bool workspace_manager_t::set_cur_id (const size_t ws_index) {

  //c_app::log("set current called");
  app_state_t* astate = app_state_t::get_inst();

  if (ws_index < m_ws.size() && has_wss()) {
      m_cur_ws_id = ws_index;
      //update_window_title();
      cached_astate->camera = m_ws[ws_index]->m_camera.get();
      cached_astate->camera->update_camera();
      astate->astate_evd->cur_ws_changed();
      return true;
    }

  astate->astate_evd->cur_ws_changed();
  return false;

}

std::shared_ptr<workspace_t> workspace_manager_t::get_ws(int id) {
  if (!m_ws.empty() && id >= 0 && id < m_ws.size()) return m_ws[id];
  else return nullptr;
}

void workspace_manager_t::init_default () {

  std::ifstream test_in_dev_env("../data/refs/laf3_p3.vasp");
  if (!test_in_dev_env.good()) return;

  load_from_file("../deps/qpp/examples/io/ref_data/xyz/slab.xyz",
                 qc_file_fmt::standart_xyz);

  load_from_file("../data/refs/POSCAR.mp-558947_SiO2",
                 qc_file_fmt::vasp_poscar);

  load_from_file("../deps/qpp/examples/io/ref_data/xyz/nanotube.xyz",
                 qc_file_fmt::standart_xyz);

  load_from_file("../data/refs/mp-971662_Si.vasp",
                 qc_file_fmt::vasp_poscar);

  load_from_file("../deps/qpp/examples/io/ref_data/firefly/dvb_ir.out",
                 qc_file_fmt::firefly_output);

  //  auto _ws2 = std::make_shared<workspace_t>("d2");
  //  auto _ws3 = std::make_shared<workspace_t>("d1");

  //  auto _wsl2 = std::make_shared<ws_atoms_list_t>();
  //  _ws3->add_item_to_ws(_wsl2);
  //  _wsl2->load_from_file(qc_file_fmt::standart_xyz,
  //                        "../deps/qpp/examples/io/ref_data/xyz/slab.xyz",
  //                        false);

  //  auto _wsl3 = std::make_shared<ws_atoms_list_t>();
  //  _ws2->add_item_to_ws(_wsl3);
  //  _wsl3->load_from_file(qc_file_fmt::vasp_poscar, "../data/refs/POSCAR.mp-558947_SiO2",
  //                        false);

  //  auto _wsl32 = std::make_shared<ws_atoms_list_t>();
  //  _ws2->add_item_to_ws(_wsl32);
  //  _wsl32->load_from_file(qc_file_fmt::standart_xyz,
  //                         "../deps/qpp/examples/io/ref_data/xyz/nanotube.xyz",
  //                         true);

  //  auto _wsl33 = std::make_shared<ws_atoms_list_t>();
  //  _ws2->add_item_to_ws(_wsl33);
  //  _wsl33->load_from_file(qc_file_fmt::vasp_poscar, "../data/refs/mp-971662_Si.vasp",
  //                         false);


  //  auto _ws4 = std::make_shared<workspace_t>();
  //  _ws4->m_ws_name = "animtest1";
  //  auto _ws4_al = std::make_shared<ws_atoms_list_t>();
  //  _ws4->add_item_to_ws(_ws4_al);

  //  _ws4_al->load_from_file(qc_file_fmt::firefly_output,
  //                          "../deps/qpp/examples/io/ref_data/firefly/dvb_ir.out",
  //                          false);

  //  _wsl3->m_name = "zeolite1";
  //  _wsl32->m_name = "nanotube1";
  //  _wsl32->m_pos = vector3<float>(0.0f, 0.0f, 14.0f);
  //  _wsl33->m_name = "ss1";
  //  _wsl33->m_pos = vector3<float>(0.0f, 22.0f, 2.0f);

  //  add_ws(_ws3);
  //  add_ws(_ws2);
  //  add_ws(_ws4);

  //  //  _ws2->save_workspace_to_json("test.json");
  //  //  _ws4->save_workspace_to_json("test_with_anim.json");

  //  set_cur_id(2);

}

void workspace_manager_t::render_cur_ws () {

  app_state_t* astate = app_state_t::get_inst();

  if (has_wss()) {
      if (m_cur_ws_id < m_ws.size()) {
          astate->glapi->glClearColor(m_ws[m_cur_ws_id]->m_background_color[0],
              m_ws[m_cur_ws_id]->m_background_color[1],
              m_ws[m_cur_ws_id]->m_background_color[2], 1);
          astate->glapi->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          m_ws[m_cur_ws_id]->render();
          return ;
        }
    }

  astate->glapi->glClearColor(0.4f, 0.4f, 0.4f, 1);
  astate->glapi->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void workspace_manager_t::render_cur_ws_overlay(QPainter &painter) {

  if (has_wss()) {
      if (m_cur_ws_id < m_ws.size()) {
          m_ws[m_cur_ws_id]->render_overlay(painter);
        }
    }
}


void workspace_manager_t::mouse_click () {

  app_state_t* astate = app_state_t::get_inst();
  astate->log(fmt::format("Mouse click {} {}", astate->mouse_x, astate->mouse_y));

  astate->log(fmt::format("Mouse click in ws {} {}",
                          astate->mouse_x_dc, astate->mouse_y_dc));

  if (has_wss()) {
      get_cur_ws()->mouse_click(astate->mouse_x_dc, astate->mouse_y_dc);
      astate->make_viewport_dirty();
    }

}

void workspace_manager_t::ws_mgr_changed() {
  app_state_t* astate = app_state_t::get_inst();
  astate->astate_evd->wss_changed();
}

void workspace_manager_t::add_ws (const std::shared_ptr<workspace_t> &ws_to_add) {
  ws_to_add->m_owner = this;
  m_ws.push_back(ws_to_add);
  ws_to_add->ws_changed();
  ws_mgr_changed();
}


void workspace_manager_t::load_from_file(const std::string &fname,
                                         qc_file_fmt file_format,
                                         bool override) {

  app_state_t* astate = app_state_t::get_inst();

  if (!override)
    for (int i = 0; i < m_ws.size(); i++)
      if (m_ws[i]->m_fs_path.find(fname) != std::string::npos) {
          set_cur_id(i);
          return;
        }

  auto new_ws = std::make_shared<workspace_t>();

  std::string file_name_extr = qpp::extract_base_name(fname);
  new_ws->m_ws_name = file_name_extr;
  new_ws->m_fs_path = fname;

  if (file_format != qc_file_fmt::qppcad_json) {

      new_ws->m_is_ws_imported = true;

      auto new_atoms_list = std::make_shared<ws_atoms_list_t>();
      new_ws->add_item_to_ws(new_atoms_list);
      new_atoms_list->load_from_file(file_format, fname,
                                     qc_file_fmt_helper::need_to_auto_center(file_format));

    } else {
      new_ws->load_ws_from_json(fname);
    }

  add_ws(new_ws);
  set_cur_id(m_ws.size()-1);

  astate->astate_evd->new_file_loaded(fname, file_format);

}

void workspace_manager_t::load_from_file_autodeduce(const std::string file_name,
                                                    const std::string file_format) {
  qc_file_fmt guess_ff;
  if (file_format.empty())
    guess_ff = qc_file_fmt_helper::file_name_to_file_format(file_name);
  else
    guess_ff = qc_file_fmt_helper::file_format_from_string(file_format);
  load_from_file(file_name, guess_ff);

}


void workspace_manager_t::create_new_ws(bool switch_to_new_workspace) {

  auto new_ws = std::make_shared<workspace_t>();
  new_ws->m_ws_name = fmt::format("new_workspace{}", m_ws.size());
  m_ws.push_back(new_ws);
  if (switch_to_new_workspace) set_cur_id(m_ws.size()-1);
  ws_mgr_changed();

}


