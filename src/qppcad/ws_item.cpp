#include <qppcad/ws_item.hpp>
#include <qppcad/app_state.hpp>
#include <data/color.hpp>
#include <qppcad/workspace.hpp>

using namespace qpp;
using namespace qpp::cad;

void ws_item_t::set_parent_workspace(workspace_t *_parent_ws){
  m_parent_ws = _parent_ws;
}

const std::string ws_item_t::get_name(){
  return m_name;
}

void ws_item_t::set_name(const std::string &_name){
  if (m_name != _name){
      m_name = _name;
      m_parent_ws->ws_changed();
    }
}

void ws_item_t::set_name(const char *_name){
  if (m_name != _name){
      m_name = std::string(_name);
      m_parent_ws->ws_changed();
    }
}

void ws_item_t::add_connected_item(std::shared_ptr<ws_item_t> new_item) {
  if (!is_connected(new_item)) m_connected_items.push_back(new_item);
}

void ws_item_t::rm_connected_item(std::shared_ptr<ws_item_t> item_to_remove) {
  auto idx = get_connected_idx(item_to_remove);
  if (idx) m_connected_items.erase(m_connected_items.begin() + *idx);
}

std::optional<size_t> ws_item_t::get_connected_idx(std::shared_ptr<ws_item_t> item_to_find) {
  for (size_t i = 0; i < m_connected_items.size(); i++)
    if (m_connected_items[i].get() == item_to_find.get()) return std::optional<size_t>(i);
  return std::nullopt;
}

bool ws_item_t::is_connected(std::shared_ptr<ws_item_t> item_to_find) {
  return (get_connected_idx(item_to_find) != std::nullopt);
}

void ws_item_t::add_follower(std::shared_ptr<ws_item_t> new_item) {
 if (!is_follower(new_item)) m_followers.push_back(new_item);
 new_item->m_leader = shared_from_this();
 new_item->on_leader_changed();
}

std::optional<size_t> ws_item_t::get_follower_idx(std::shared_ptr<ws_item_t> item_to_find) {
  for (size_t i = 0; i < m_followers.size(); i++)
    if (m_followers[i].get() == item_to_find.get()) return std::optional<size_t>(i);
  return std::nullopt;
}

bool ws_item_t::is_follower(std::shared_ptr<ws_item_t> item_to_find) {
  return (get_follower_idx(item_to_find) != std::nullopt);
}

void ws_item_t::rm_follower(std::shared_ptr<ws_item_t> item_to_remove) {
  auto idx = get_follower_idx(item_to_remove);
  if (idx) {
      m_followers.erase(m_followers.begin() + *idx);
      item_to_remove->m_leader = nullptr;
      item_to_remove->on_leader_changed();
    }
}

bool ws_item_t::is_selected() {

  if (m_parent_ws) {
      return m_parent_ws->get_selected() == this;
    }
  else {
      return false;
    }

}

void ws_item_t::render () {

  app_state_t* astate = app_state_t::get_inst();

  if (m_selected && (get_flags() & ws_item_flags_support_selection) &&
      (get_flags() & ws_item_flags_support_rendering_bb) && is_bb_visible()) {
      astate->dp->begin_render_aabb();
      if (m_parent_ws->m_edit_type == ws_edit_t::edit_item)
        astate->dp->render_aabb(clr_fuchsia,
                                     m_pos + m_aabb.min,
                                     m_pos + m_aabb.max  );
      else
        astate->dp->render_aabb_segmented(clr_olive,
                                               m_pos + m_aabb.min,
                                               m_pos + m_aabb.max);
      astate->dp->end_render_aabb();
    }
}

void ws_item_t::render_overlay(QPainter &painter) {

}


void ws_item_t::set_default_flags(uint32_t flags){
  p_flags = flags;
}

uint32_t ws_item_t::get_flags() const {
  return p_flags;
}

void ws_item_t::on_leader_changed() {

}


void ws_item_t::update (float delta_time) {

}

float ws_item_t::get_bb_prescaller(){
  return 1.0f;
}

bool ws_item_t::is_bb_visible() {
  return true;
}

const vector3<float> ws_item_t::get_gizmo_content_barycenter() {
  return vector3<float>::Zero();
}

//ws_item_t::~ws_item_t(){

//}

void ws_item_t::on_begin_node_gizmo_translate(){
  m_pos_old = m_pos;
//  c_app::log(fmt::format("Start of translation of node [{}], pos = {}",
//                         m_name, m_pos.to_string_vec()));
}

void ws_item_t::on_end_node_gizmo_translate(){
//  c_app::log(fmt::format("End of translation of node [{}], pos = {}",
//                         m_name, m_pos.to_string_vec()));
}

void ws_item_t::on_begin_content_gizmo_translate() {

}

void ws_item_t::apply_intermediate_translate_content(const vector3<float> &new_pos){

}

void ws_item_t::on_end_content_gizmo_translate() {

}

void ws_item_t::translate(vector3<float> tr_vec) {
  app_state_t* astate = app_state_t::get_inst();
  if (get_flags() & ws_item_flags_support_translation) m_pos += tr_vec;
  astate->make_viewport_dirty();
}

void ws_item_t::save_to_json(json &data) {
  data[JSON_WS_ITEM_NAME] = m_name;
  data[JSON_WS_ITEM_TYPE] = get_type_name();
  data[JSON_IS_VISIBLE] = m_is_visible;
  json coord = json::array({m_pos[0], m_pos[1], m_pos[2]});
  data[JSON_POS] = coord;
}

void ws_item_t::load_from_json(json &data) {
  //read m_name & is_visible & pos
  if (data.find(JSON_WS_ITEM_NAME) != data.end()) m_name = data[JSON_WS_ITEM_NAME];
  if (data.find(JSON_IS_VISIBLE) != data.end()) m_is_visible = data[JSON_IS_VISIBLE];

  if (get_flags() | ws_item_flags_support_translation)
    if (data.find(JSON_POS) != data.end()) {
        vector3<float> pos_rd = vector3<float>::Zero();
        for(uint8_t i = 0; i < 3; i++)
          pos_rd[i] = data[JSON_POS][i].get<float>();
        m_pos = pos_rd;
      }
}

void ws_item_t::load_from_stream(std::basic_istream<char, TRAITS> &stream) {

}

void ws_item_t::save_to_stream(std::basic_istream<char, TRAITS> &stream) {

}

void ws_item_t::load_from_file(std::string &file_name) {

}

void ws_item_t::save_from_file(std::string &file_name) {

}

bool ws_item_t::can_be_written_to_json() {
  return false;
}
