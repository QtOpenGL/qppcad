#include <qppcad/app_state_event_disp.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


void app_state_event_disp_t::wss_changed() {
  app_state_t* astate = app_state_t::get_inst();
  //astate->log("DEBUG: app_state_event_disp_t::workspaces_changed()");
  emit(wss_changed_signal());
}

void app_state_event_disp_t::cur_ws_changed() {
  app_state_t* astate = app_state_t::get_inst();
  //astate->log("DEBUG: app_state_event_disp_t::current_workspace_changed()");
  emit(cur_ws_changed_signal());
}

void app_state_event_disp_t::cur_ws_edit_type_changed(){
   emit(cur_ws_edit_type_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_changed() {
  app_state_t* astate = app_state_t::get_inst();
  //astate->log("DEBUG: app_state_event_disp_t::current_workspace_selected_item_changed()");
  emit(cur_ws_selected_item_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_content_changed() {
  app_state_t* astate = app_state_t::get_inst();
  //astate->log("DEBUG: app_state_event_disp_t::current_workspace_selected_item_content_changed()");
  emit(cur_ws_selected_item_content_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_frame_changed() {
  app_state_t* astate = app_state_t::get_inst();
  //astate->log("DEBUG: app_state_event_disp_t::current_workspace_selected_item_frame_changed()");
  emit(cur_ws_selected_item_frame_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_atoms_list_selection_changed() {
   emit(cur_ws_selected_atoms_list_selection_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_atoms_list_cell_changed() {
  emit(cur_ws_selected_atoms_list_cell_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_item_position_changed(){
  emit(cur_ws_selected_item_position_changed_signal());
}

void app_state_event_disp_t::cur_ws_selected_atoms_list_selected_content_changed() {
  emit(cur_ws_selected_atoms_list_selected_content_changed_signal());
}

void app_state_event_disp_t::python_console_clear_requested() {
  emit(python_console_clear_requested_signal());
}

app_state_event_disp_t::app_state_event_disp_t(QObject *parent) : QObject (parent) {

}
