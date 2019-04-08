#include <qppcad/ws_item/arrow_array/arrow_array.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

arrow_array_t::arrow_array_t() {
  set_default_flags(ws_item_flags_default | ws_item_flags_support_rendering);
}

void arrow_array_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void arrow_array_t::render() {

  app_state_t* astate = app_state_t::get_inst();

  ws_item_t::render();

  if (!m_is_visible) return;
  if (!m_binded_gv) return;
  if (!m_binded_gv->m_anim->animable()) return;

  auto cur_anim = m_binded_gv->m_anim->get_current_anim();

  if (cur_anim && m_binded_gv->m_anim->get_current_anim()->m_anim_type == geom_anim_t::anim_static)
    return;

  if (cur_anim->frames.size() == 0) return;

  int total_frames    = cur_anim->frames.size();
  float start_frame_0 = int(m_binded_gv->m_anim->m_cur_anim_time);
  float end_frame_0   = std::ceil(m_binded_gv->m_anim->m_cur_anim_time);
  float frame_delta_0 = 1 - (m_binded_gv->m_anim->m_cur_anim_time - start_frame_0);
  int start_frame_n_0 = std::clamp(int(start_frame_0), 0, total_frames-1);
  int end_frame_n_0   = std::clamp(int(end_frame_0), 0, total_frames-1);

  float integrated_dt = 1.01 / (m_binded_gv->m_anim->m_anim_frame_time * 60);
  float start_frame_1 = int(m_binded_gv->m_anim->m_cur_anim_time + integrated_dt);
  float end_frame_1   = std::ceil(m_binded_gv->m_anim->m_cur_anim_time + integrated_dt);
  float frame_delta_1 = 1 - (m_binded_gv->m_anim->m_cur_anim_time + integrated_dt - start_frame_1);
  int start_frame_n_1 = std::clamp(int(start_frame_1), 0, total_frames-1);
  int end_frame_n_1   = std::clamp(int(end_frame_1), 0, total_frames-1);

  if (start_frame_0 >= cur_anim->frames.size() || end_frame_n_0 >= cur_anim->frames.size()) return;
  if (cur_anim->frames[start_frame_n_0].atom_pos.size() != m_binded_gv->m_geom->nat()) return;
  if (cur_anim->frames[end_frame_n_0].atom_pos.size() != m_binded_gv->m_geom->nat()) return;
  if (cur_anim->frames[start_frame_n_1].atom_pos.size() != m_binded_gv->m_geom->nat()) return;
  if (cur_anim->frames[end_frame_n_1].atom_pos.size() != m_binded_gv->m_geom->nat()) return;

  astate->log(fmt::format("ENTER AA RENDERING s0={} e0={} d0={} s1={} e1={} d1={}",
                          start_frame_0, end_frame_0, frame_delta_0,
                          start_frame_1, end_frame_1, frame_delta_1));

  astate->dp->begin_render_general_mesh();
  for (size_t i = 0; i < m_binded_gv->m_geom->nat(); i++) {

      vector3<float> start_pos = cur_anim->frames[start_frame_n_0].atom_pos[i] * (frame_delta_0) +
                                 cur_anim->frames[end_frame_n_0].atom_pos[i] * (1-frame_delta_0) +
                                 m_binded_gv->m_pos;

      vector3<float> end_pos = cur_anim->frames[start_frame_n_1].atom_pos[i] * (frame_delta_1) +
                               cur_anim->frames[end_frame_n_1].atom_pos[i] * (1-frame_delta_1) +
                               m_binded_gv->m_pos;

      float dnodem = (end_pos - start_pos).norm();
      if (dnodem < 0.001f) continue;

      vector3<float> dir = (end_pos - start_pos).normalized();

      //astate->log(fmt::format("{}", dir));

      vector3<float> end = start_pos + dir * m_unf_arrow_len;

      matrix4<float> mat_body = matrix4<float>::Identity();
      mat_body.block<3,1>(0,3) = start_pos;
      mat_body.block<3,1>(0,2) = end - start_pos;

      vector3<float> vec_axis_norm = mat_body.block<3,1>(0,2).normalized();
      mat_body.block<3,1>(0,0) = vec_axis_norm.unitOrthogonal() * m_unf_arrow_scale;
      mat_body.block<3,1>(0,1) = vec_axis_norm.cross(mat_body.block<3,1>(0,0));
      mat_body.block<3,1>(0,3) = start_pos ;

      vector3<float> start_ar = end;
      vector3<float> end_ar = end + dir * m_unf_arrow_cap_len;

      matrix4<float> mat_arrow = matrix4<float>::Identity();
      mat_arrow.block<3,1>(0,3) = start_ar;
      mat_arrow.block<3,1>(0,2) = end_ar - start_ar;

      vector3<float> veca_axis_norm = mat_arrow.block<3,1>(0,2).normalized();
      mat_arrow.block<3,1>(0,0) = veca_axis_norm.unitOrthogonal() * m_unf_arrow_cap_scale;
      mat_arrow.block<3,1>(0,1) = veca_axis_norm.cross(mat_arrow.block<3,1>(0,0));
      mat_arrow.block<3,1>(0,3) = start_ar ;

      astate->dp->render_general_mesh(mat_body, m_color, astate->mesh_cylinder);
      astate->dp->render_general_mesh(mat_arrow, m_color, astate->mesh_unit_cone);

    }

  astate->dp->end_render_general_mesh();

}

bool arrow_array_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string arrow_array_t::compose_type_descr() {
  return "arrow_array";
}

void arrow_array_t::update(float delta_time) {
  //do nothing
}

float arrow_array_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t arrow_array_t::get_amount_of_selected_content() {
  return 0;
}

size_t arrow_array_t::get_content_count() {
  return 0;
}

void arrow_array_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void arrow_array_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

void arrow_array_t::updated_internally() {

  app_state_t *astate = app_state_t::get_inst();
  astate->log(fmt::format("AARRAY[{}]::updated internally", m_name));

  if (m_src) {
      auto _as_gv = m_src->cast_as<geom_view_t>();
      if (_as_gv) m_binded_gv = _as_gv;
      else m_binded_gv = nullptr;
    } else {
      m_binded_gv = nullptr;
    }

}
