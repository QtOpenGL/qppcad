#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ccd_view_t::ccd_view_t() {
  set_default_flags(ws_item_flags_default);
}

void ccd_view_t::manual_step_update(const int dir) {
  auto old_step = m_cur_step;
  auto new_step = m_cur_step + dir;
  if (new_step >= 0 && new_step < m_ccd->m_steps.size()) m_cur_step = new_step;
  if (old_step != m_cur_step) update_joined_atoms_list_animation(m_cur_step);
}

void ccd_view_t::manual_update_vib() {
  for (auto &items : m_connected_items) {
      geom_view_t *al = dynamic_cast<geom_view_t*>(items.get());
      if (al && al->m_anim->get_total_anims() == m_ccd->m_vibs.size() + 1)  {
          al->m_anim->m_cur_anim = m_cur_vib + 1;
          al->m_anim->m_cur_anim_time = 0.0f;
          al->m_anim->m_play_anim = true;
          al->m_anim->m_play_cyclic = true;
          al->m_anim->m_anim_frame_time = 0.1f;
        }
    }
}

void ccd_view_t::fill_custom_colors_of_geom_anim(const std::string color_map_name) {

  app_state_t* astate = app_state_t::get_inst();

}

void ccd_view_t::vote_for_view_vectors(vector3<float> &vOutLookPos,
                                                vector3<float> &vOutLookAt) {
  //do nothing
}

void ccd_view_t::render() {

}

void ccd_view_t::update_joined_atoms_list_animation(size_t step_idx) {

  for (auto &items : m_connected_items) {
      geom_view_t *al = dynamic_cast<geom_view_t*>(items.get());
      if (al && al->m_anim->get_total_anims() > 1
          && al->m_anim->m_anim_data[1].frames.size() == m_ccd->m_steps.size())  {
          //fmt::print(std::cout, "Hallelujiah\n");
          al->m_anim->update_and_set_anim(1, step_idx);
        }
    }

}


bool ccd_view_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}


std::string ccd_view_t::compose_type_descr() {
  return "comp. chem. data";
}

void ccd_view_t::update(float delta_time) {

}

float ccd_view_t::get_bb_prescaller() {
  return 1.0f;
}

void ccd_view_t::updated_internally() {

}

uint32_t ccd_view_t::get_amount_of_selected_content() {
  return 0;
}

size_t ccd_view_t::get_content_count() {
  return 0;
}

void ccd_view_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void ccd_view_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}
