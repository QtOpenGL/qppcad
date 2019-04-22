#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/node_book/qnode.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/sflow/sflow_base_nodes.hpp>

using namespace qpp;
using namespace qpp::cad;

node_book_t::node_book_t() {

  set_default_flags(ws_item_flags_default);
  m_scene = new node_book_graphics_scene_t(nullptr);

  auto new_node1 = std::make_shared<sf_i_prop_node_t>();
  auto qnode1 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode1.get());
  qnode1->set_sflow_node(new_node1);
  qnode1->setPos(QPoint(0,0));
  m_qnodes.push_back(qnode1);

  auto new_node2 = std::make_shared<sf_i_final_node_t>();
  auto qnode2 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode2.get());
  qnode2->set_sflow_node(new_node2);
  qnode2->setPos(QPoint(300,0));
  m_qnodes.push_back(qnode2);

  auto new_node3 = std::make_shared<sf_i_p_const_node_t>();
  auto qnode3 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode3.get());
  qnode3->setPos(QPoint(600,0));
  qnode3->set_sflow_node(new_node3);
  m_qnodes.push_back(qnode3);

  auto new_node4 = std::make_shared<sf_i_sum_i_node_t>();
  auto qnode4 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode4.get());
  qnode4->setPos(QPoint(0,300));
  qnode4->set_sflow_node(new_node4);
  m_qnodes.push_back(qnode4);

  auto new_node5 = std::make_shared<sf_f_prop_node_t>();
  auto qnode5 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode5.get());
  qnode5->setPos(QPoint(300,300));
  qnode5->set_sflow_node(new_node5);
  m_qnodes.push_back(qnode5);

  auto new_node6 = std::make_shared<sf_f_p_const_node_t>();
  auto qnode6 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode6.get());
  qnode6->setPos(QPoint(600,300));
  qnode6->set_sflow_node(new_node6);
  m_qnodes.push_back(qnode6);

}

void node_book_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                        vector3<float> &out_look_at) {
  //do nothing
}

void node_book_t::render() {
  //do nothing
}

bool node_book_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string node_book_t::compose_type_descr() {
  return "node_book";
}

void node_book_t::update(float delta_time) {
  //do nothing
}

float node_book_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t node_book_t::get_num_cnt_selected() {
  return 0;
}

size_t node_book_t::get_content_count() {
  return 0;
}

void node_book_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);

}

void node_book_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

}

void node_book_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

}

