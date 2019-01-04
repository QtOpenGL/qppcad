#include <qppcad/ws_atoms_list_colorizer.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void ws_atoms_list_colorizer_helper::colorize_by_distance(ws_atoms_list_t *al,
                                                          float min_dist,
                                                          vector3<float> min_dist_color,
                                                          vector3<float> over_dist_color,
                                                          bool affect_pairs,
                                                          std::string atom_type1,
                                                          std::string atom_type2) {

  if (!al) {
      throw std::runtime_error("colorize_by_distance -> !al");
      return;
    }

  //intermediate geometry
  xgeometry<float, periodic_cell<float> > g(al->m_geom->DIM);

  g.DIM = al->m_geom->DIM;
  g.cell.DIM = al->m_geom->cell.DIM;

  //initialize tws_tree
  tws_tree_t<float, periodic_cell<float> > g_t(g);
  g_t.do_action(act_lock | act_lock_img);

  index zero = index::D(al->m_geom->DIM).all(0);

  if (al->m_anim->animable()) {

      //copy initial geometry
      al->copy_to_xgeometry(g);

      //iterate over all animations
      for (size_t a_id = 0; a_id < al->m_anim->get_total_anims(); a_id++)
        //iterate over all frames
        for (size_t f_id = 0; f_id < al->m_anim->m_anim_data[a_id].frame_data.size(); f_id++) {
            //std::cout << "PROCCESING FRAME " << f_id << " FOR ANIM " << a_id << std::endl;
            //copy anim to temp geometry
            for (int i = 0; i < al->m_geom->nat(); i++)
              g.coord(i) = al->m_anim->m_anim_data[a_id].frame_data[f_id].atom_pos[i];

            g_t.do_action(act_unlock | act_unlock_img);
            g_t.do_action(act_clear_all);
            g_t.do_action(act_build_tree);
            g_t.do_action(act_lock | act_lock_img);

            if (al->m_anim->m_anim_data[a_id].frame_data[f_id].atom_color.size() !=
                al->m_geom->nat())
              al->m_anim->m_anim_data[a_id].frame_data[f_id].atom_color.resize(al->m_geom->nat());

            for (int i = 0; i < g.nat(); i++) {
                std::vector<tws_node_content_t<float> > res;
                g_t.query_sphere(min_dist, g.pos(i), res);

                vector3<float> final_color = over_dist_color;

                int zero_cnt = 0;

                zero_cnt = std::count_if(res.begin(), res.end(),
                           [&zero](tws_node_content_t<float> &elem){
                           return elem.m_idx == zero;});

                if (res.size() > 1 ) {

                    bool pair1_d = al->m_geom->atom(i) == atom_type1;
                    bool pair2_d = al->m_geom->atom(res[1].m_atm) == atom_type2;
                    bool pair1_i = al->m_geom->atom(res[1].m_atm) == atom_type1;
                    bool pair2_i = al->m_geom->atom(i) == atom_type2;
                    bool p1 = pair1_d && pair2_d;
                    bool p2 = pair1_i && pair2_i;

                    if ((!affect_pairs || (affect_pairs && (p1 || p2))) && zero_cnt > 1)
                      final_color = min_dist_color;
                  }

                al->m_anim->m_anim_data[a_id].frame_data[f_id].atom_color[i] = final_color;

              }

          }


    }

}

void ws_atoms_list_colorizer_helper::py_colorize_by_distance(float min_dist,
                                                             vector3<float> min_dist_color,
                                                             vector3<float> over_dist_color) {
  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          //
          std::shared_ptr<ws_atoms_list_t> as_al =
              std::dynamic_pointer_cast<ws_atoms_list_t>(cur_ws->get_selected_sp());

          if (as_al) ws_atoms_list_colorizer_helper::colorize_by_distance(
                as_al.get(), min_dist, min_dist_color, over_dist_color, false, "", "");
        }
    }
}

void ws_atoms_list_colorizer_helper::py_colorize_by_distance_with_pairs(
    float min_dist,
    vector3<float> min_dist_color,
    vector3<float> over_dist_color,
    std::string atom_type1,
    std::string atom_type2) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      auto cur_ws = astate->ws_manager->get_cur_ws();

      if (cur_ws) {
          //
          std::shared_ptr<ws_atoms_list_t> as_al =
              std::dynamic_pointer_cast<ws_atoms_list_t>(cur_ws->get_selected_sp());

          if (as_al) ws_atoms_list_colorizer_helper::colorize_by_distance(
                as_al.get(), min_dist, min_dist_color, over_dist_color,
                true, atom_type1, atom_type2);
        }
    }

}
