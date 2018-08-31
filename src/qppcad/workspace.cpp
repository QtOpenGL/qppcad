#include <qppcad/workspace.hpp>
#include <qppcad/app.hpp>
#include <data/ptable.hpp>
#include <io/geomio.hpp>
#include <mathf/math.hpp>

using namespace qpp;

int16_t workspace_t::get_selected_item(){
  for (uint16_t i = 0; i < ws_items.size(); i++)
    if (ws_items[i]->bSelected) return i;
  return -1;
}

ws_item_t *workspace_t::get_selected(){
  int16_t sel_idx = get_selected_item();
  if (sel_idx != -1) return ws_items.at(sel_idx);
  else return nullptr;
}

void workspace_t::set_selected_item(const int16_t sel_idx){
  unselect_all();
  if (sel_idx >= 0 && sel_idx < ws_items.size() && ws_items.size() > 0)
      ws_items[sel_idx]->bSelected = true;
}

void workspace_t::unselect_all(){
  for (uint16_t i = 0; i < ws_items.size(); i++) ws_items[i]->bSelected = false;
}

void workspace_t::workspace_changed(){
  //rebuild ws items
  ws_names_c.clear();
  for(uint8_t i = 0; i < ws_items.size(); i++)
    ws_names_c.push_back(fmt::format("[{}] {}", i, ws_items.at(i)->name));

}

void workspace_t::reset_camera(){
  camera->reset_camera();
  set_best_view();
}

void workspace_t::set_best_view(){

  if (ws_items.size() == 0) {
      camera->reset_camera();
      return;
    }

  vector3<float> _vLookAt = vector3<float>(0.0, 0.0, 0.0);
  vector3<float> _vLookPos = vector3<float>(0.0, 0.0, 0.0);

  for (ws_item_t* _ws_item : ws_items)
    _ws_item->vote_for_view_vectors(_vLookPos, _vLookAt);

  _vLookAt  /= ws_items.size();
  _vLookPos /= ws_items.size();

  camera->vLookAt = _vLookAt;
  camera->vViewPoint = _vLookPos;

  //  std::cout << "set bv " << _vLookAt << std::endl << _vLookPos << std::endl
  //            << "end bv " << std::endl;

  if ((camera->vLookAt-camera->vViewPoint).norm() < 0.4f)
    camera->reset_camera();
}

void workspace_t::render(){


  if (first_render) {
      set_best_view();
      first_render = false;
    }

  app_state_t* astate = &(c_app::get_state());

  if (gizmo->is_active){
      gizmo->render();
    }

  if (astate->bDebugDrawSelectionRay){
      astate->dp->begin_render_line();
      astate->dp->render_line(vector3<float>(1.0, 1.0, 0.0), ray_debug.start,
                      ray_debug.start+ray_debug.dir*55.0);
      astate->dp->end_render_line();
    }

  if (astate->dp != nullptr){


      ///// Draw grid /////
      if (astate->bDrawGrid){
          astate->line_mesh_program->begin_shader_program();
          vector3<float> color(0.75, 0.75, 0.75);
          astate->line_mesh_program->set_u(sp_u_name::m_model_view_proj,
                                        astate->camera->mViewProjection.data());
          astate->line_mesh_program->set_u(sp_u_name::m_model_view, astate->camera->mView.data());
          astate->line_mesh_program->set_u(sp_u_name::v_color, (GLfloat*)color.data());

          for (int dx = -4; dx <= 4; dx++)
            for (int dz = -4; dz <= 4; dz++){
                vector3<float> vTr(dx * (20.0f * 0.5f), dz * (20.0f * 0.5f), 0.0f );
                astate->line_mesh_program->set_u(sp_u_name::v_translate, (GLfloat*)vTr.data());
                astate->gridXZ->render();

              }
          astate->line_mesh_program->end_shader_program();
        }
      ///// Draw grid /////

      ///// Draw axis /////
      if (astate->bDrawAxis){
          vector3<float> vScrTW = astate->camera->unproject(-0.95f, -0.90f);
          float fAxisLen = 0.07f *astate->camera->fStoredDist;
          if (astate->camera->cur_proj ==
              app_camera_proj_type::CAMERA_PROJ_PERSP)
            fAxisLen = 0.015f;

          astate->dp->begin_render_line();
          astate->dp->
              render_line(vector3<float>(1.0, 0.0, 0.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(fAxisLen, 0.0, 0.0) + vScrTW);

          astate->dp->
              render_line(vector3<float>(0.0, 1.0, 0.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(0.0, fAxisLen, 0.0) + vScrTW);

          astate->dp->
              render_line(vector3<float>(0.0, 0.0, 1.0),
                          vector3<float>(0.0, 0.0, 0.0) + vScrTW,
                          vector3<float>(0.0, 0.0, fAxisLen) + vScrTW);
          astate->dp->end_render_line();

        }
      ///// Draw axis end /////
    }

  for (ws_item_t* _ws_item : ws_items) _ws_item->render();

}

void workspace_t::mouse_click(const double fMouseX, const double fMouseY){
  ray_debug.dir = (camera->unproject(fMouseX, fMouseY) - camera->vViewPoint).normalized();
  ray_debug.start = camera->vViewPoint;

  bool bHitAny = false;

  if (cur_edit_type != ws_edit_type::EDIT_WS_ITEM_CONTENT)
    for (ws_item_t* ws_it : ws_items) ws_it->bSelected = false;

  for (ws_item_t* ws_it : ws_items){
      bool bWsHit = ws_it->mouse_click(&ray_debug);
      bHitAny = bHitAny || bWsHit;
      if ((bWsHit) && (cur_edit_type == ws_edit_type::EDIT_WS_ITEM)
          && ws_it->support_selection()) ws_it->bSelected = true;
    }

  if ((cur_edit_type != ws_edit_type::EDIT_WS_ITEM_CONTENT) && (!bHitAny))
    for (ws_item_t* ws_it : ws_items) ws_it->bSelected = false;

}

void workspace_t::add_item_to_workspace(ws_item_t *item_to_add){
  item_to_add->set_parent_workspace(this);
  ws_items.push_back(item_to_add);
  workspace_changed();
  c_app::log(fmt::format("New workspace {} size = {}", ws_name, ws_items.size()));
}

workspace_t *workspace_manager_t::get_current_workspace(){
  if ((iCurrentWorkSpace <= 0) && (iCurrentWorkSpace > ws.size()))
    return nullptr;
  return ws[iCurrentWorkSpace];
}

void workspace_manager_t::init_default_workspace(){

  //  ws_atom_list* _wsl1 = new ws_atom_list();
  //  _wsl1->load_from_file(qc_file_format::format_standart_xyz, "../examples/io/ref_data/dna.xyz",
  //                        true);

  //  ws_atom_list* _wsl2 = new ws_atom_list();
  //  _wsl2->load_from_file(qc_file_format::format_standart_xyz, "../examples/io/ref_data/ddt.xyz",
  //                        true);

  //  ws_atom_list* _wsl3 = new ws_atom_list();
  //  _wsl3->load_from_file(qc_file_format::format_standart_xyz, "../examples/io/ref_data/nanotube.xyz",          true);
  workspace_t* _ws2 = new workspace_t();
  _ws2->ws_name = "d2";
  ws_atom_list_t* _wsl2 = new ws_atom_list_t(_ws2);
  _wsl2->load_from_file(qc_file_format::format_vasp_poscar, "../data/refs/mp-971662_Si.vasp",
                        false);

  workspace_t* _ws3 = new workspace_t();
  _ws3->ws_name = "d1";
  ws_atom_list_t* _wsl3 = new ws_atom_list_t(_ws3);
  _wsl3->load_from_file(qc_file_format::format_vasp_poscar, "../data/refs/POSCAR.mp-558947_SiO2",
                        false);
  ws_atom_list_t* _wsl32 = new ws_atom_list_t(_ws3);
  _wsl32->load_from_file(qc_file_format::format_standart_xyz, "../deps/qpp/examples/io/ref_data/nanotube.xyz",
                         true);
  _wsl3->name = "zeolite1";
  _wsl32->name = "nanotube1";
  _wsl32->shift(vector3<float>(0.0f, 0.0f, 15.0f));

  //  workspace* _ws1 = new workspace();
  //  _ws1->ws_name = "d1";



  //  workspace* _ws3 = new workspace();
  //  _ws3->ws_name = "nanotube";

  //  _ws1->add_item_to_workspace(_wsl1);
  //  _ws1->add_item_to_workspace(_wsl2);
  _ws2->add_item_to_workspace(_wsl2);
  _ws3->add_item_to_workspace(_wsl3);
  _ws3->add_item_to_workspace(_wsl32);
  ws.push_back(_ws2);
  ws.push_back(_ws3);
  //  ws.push_back(_ws1);

  iCurrentWorkSpace = ws.size() - 1;
}

void workspace_manager_t::render_current_workspace(){

  if (has_wss())
    if ((iCurrentWorkSpace >= 0) && (iCurrentWorkSpace < ws.size())){
        c_app::get_state().camera = ws[iCurrentWorkSpace]->camera;
        ws[iCurrentWorkSpace]->render();
      }
}

void workspace_manager_t::mouse_click(){
  app_state_t* astate =  &(c_app::get_state());
  double newMouseX = astate->MouseX;
  double newMouseY = astate->MouseY - astate->ui_manager->iWorkPanelHeight
                     - astate->ui_manager->iWorkPanelYOffset;
  c_app::log(fmt::format("Mouse click {} {}", astate->MouseX, astate->MouseY));

  if ((newMouseX > 0) && (newMouseX < astate->vViewportWidthHeight(0)) &&
      (newMouseY > 0) && (newMouseY < astate->vViewportWidthHeight(1))){

      newMouseX = (newMouseX/astate->vViewportWidthHeight(0)-0.5)*2.0;
      newMouseY = (newMouseY/astate->vViewportWidthHeight(1)-0.5)*-2.0;

      c_app::log(fmt::format("Mouse click in ws {} {}", newMouseX, newMouseY));


      if(has_wss()){
          this->get_current_workspace()->mouse_click(newMouseX, newMouseY);
        }

    }
}
