#ifndef QPP_WORKSPACE_H
#define QPP_WORKSPACE_H

#include <geom/lace3d.hpp>
#include <geom/geom.hpp>
#include <geom/xgeom.hpp>
#include <qppcad/camera.hpp>
#include <vector>
#include <iostream>
#include <qppcad/ws_item.hpp>
#include <qppcad/ws_atom_list.hpp>

namespace qpp{

  class workspace;
  class ws_item;

  enum ws_periodic {
    ws_periodic_a,
    ws_periodic_b,
    ws_periodic_c
  };

  enum ws_edit_type {
    EDIT_WS_ITEM = 0,
    EDIT_WS_ITEM_CONTENT = 1
  };


  /// workspace manager
  class workspace_manager {
  public:

    int iCurrentWorkSpace;
    std::vector<workspace*> ws;
    workspace_manager(){ iCurrentWorkSpace = -1;}
    workspace* get_current_workspace();

    bool has_wss(){return ws.size()>0;}
    void init_default_workspace();
    void render_current_workspace();
    void mouse_click();
  };

  /// workspace
  class workspace {
  public:
    ws_edit_type cur_edit_type;
    std::vector<ws_item*> ws_items;
    std::string ws_name;
    camera* ws_cam;
    ray<float> debugRay;
    bool bFirstRender;
    std::vector<std::string> vWSNames_c;

    workspace(std::string _ws_name = "default"){
      ws_name = _ws_name;
      bFirstRender = true;
      ws_cam = new camera();
      ws_cam->reset_camera();
      cur_edit_type = ws_edit_type::EDIT_WS_ITEM;

    }

    int16_t get_selected_item();
    void set_selected_item(const int16_t sel_idx);
    void unselect_all();

    void workspace_changed();
    void reset_camera();
    void set_best_view();
    void render();
    void mouse_click(const double fMouseX, const double fMouseY);
    void add_item_to_workspace(ws_item *item_to_add);
  };




}

#endif
