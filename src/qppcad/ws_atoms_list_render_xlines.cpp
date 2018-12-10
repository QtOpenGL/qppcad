#include <qppcad/ws_atoms_list_render_xlines.hpp>
#include <qppcad/app_state.hpp>

namespace qpp {

  namespace cad {

    void ws_atoms_list_render_xlines::render(ws_atoms_list_t &al) {

      app_state_t* astate = app_state_t::get_inst();
      index all_null = index::D(al.m_geom->DIM).all(0);
      astate->sp_line_mesh->begin_shader_program();
      astate->mesh_xline_mesh->begin_render_batch();

      /*
        sp->u_on(sp_u_name::m_model_view_proj);
        sp->u_on(sp_u_name::v_translate);
        sp->u_on(sp_u_name::v_color);
         */
      astate->sp_line_mesh->set_u(sp_u_name::m_model_view_proj,
                                       astate->camera->m_proj_view.data());
      glLineWidth(al.m_atom_scale_factor*3);

      for (uint32_t i = 0; i < al.m_geom->nat(); i++)
        if (al.m_draw_atoms &&
            al.m_atom_type_to_hide.find(al.m_geom->type_table(i)) ==
            al.m_atom_type_to_hide.end()) {
            vector3<float> color(0.0, 0.0, 1.0);
            auto ap_idx = ptable::number_by_symbol(al.m_geom->atom(i));
            float dr_rad = 0.4f;
            if (ap_idx) {
                dr_rad = ptable::get_inst()->arecs[*ap_idx - 1].m_radius *
                         al.m_atom_scale_factor;
                color = ptable::get_inst()->arecs[*ap_idx - 1].m_color_jmol;
              }
            astate->sp_line_mesh->set_u(sp_u_name::v_translate,
                                             al.m_geom->pos(i).data());
            astate->sp_line_mesh->set_u(sp_u_name::v_color, color.data());
            astate->mesh_xline_mesh->render_batch();
          }

      astate->mesh_xline_mesh->end_render_batch();
      astate->sp_line_mesh->end_shader_program();

    }


  }

}
