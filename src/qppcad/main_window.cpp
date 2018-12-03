#include <qppcad/main_window.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

main_window::main_window(QWidget *parent) {

  main_widget = new QWidget;
  app_state_t* astate = app_state_t::get_inst();
  //tool_panel_widget->setSizePolicy(QSizePolicy::)
  setCentralWidget(main_widget);
  setMinimumHeight(300);
  setMinimumWidth(600);
  init_base_shortcuts();
  init_menus();
  init_widgets();
  init_layouts();
  bool connect_st1 = QObject::connect(astate->astate_evd,
                                      SIGNAL(workspaces_changed_signal()),
                                      this, SLOT(workspaces_changed_slot()));

  bool connect_st2 = QObject::connect(astate->astate_evd,
                                      SIGNAL(current_workspace_changed_signal()),
                                      this, SLOT(current_workspace_changed()));

  bool connect_st3 = QObject::connect(astate->astate_evd,
                                      SIGNAL(current_workspace_selected_item_changed_signal()),
                                      this, SLOT(current_workspace_selected_item_changed())
                                      );
  workspaces_changed_slot();
  current_workspace_changed();
  //astate->log(fmt::format("Connection status: {}", connect_st));

  setStyleSheet("QPushButton:checked{"
                "background-color: rgb(150, 150, 150);}");
}

main_window::~main_window() {

}

void main_window::init_base_shortcuts() {
  //  sc_terminate_app = new QShortcut(this);
  //  sc_terminate_app->setKey(Qt::Key_Escape);
  //  connect(sc_terminate_app, SIGNAL(activated()), this, SLOT(slot_shortcut_terminate_app()));
}

void main_window::init_menus() {

  file_menu  = menuBar()->addMenu(tr("&File"));

  act_new_ws = new QAction(this);
  act_new_ws->setText(tr("New workspace"));
  act_new_ws->setShortcut(QKeySequence(tr("Ctrl+n")));
  file_menu->addAction(act_new_ws);
  connect(act_new_ws, &QAction::triggered, this, &main_window::create_new_workspace);

  act_open_ws = new QAction(this);
  act_open_ws->setText(tr("Open workspace"));
  act_open_ws->setShortcut(QKeySequence(tr("Ctrl+o")));
  file_menu->addAction(act_open_ws);
  connect(act_open_ws, &QAction::triggered, this, &main_window::open_workspace);

  menu_import = file_menu->addMenu(tr("Import"));
  menu_import_xyz = new QAction(this);
  menu_import_xyz->setText("XYZ");
  menu_import->addAction(menu_import_xyz);
  connect(menu_import_xyz, &QAction::triggered, this,
          [this](){this->import_file("Import XYZ file", "*", qc_file_fmt::standart_xyz);});

  menu_import_cp2k = menu_import->addMenu("CP2K");
  menu_import_cp2k_output = new QAction(this);
  menu_import_cp2k_output->setText(tr("OUTPUT"));
  menu_import_cp2k->addAction(menu_import_cp2k_output);
  connect(menu_import_cp2k_output, &QAction::triggered, this,
          [this](){this->import_file("Import CP2K output", "*", qc_file_fmt::cp2k_output);});

  menu_import_firefly = menu_import->addMenu("Firefly");
  menu_import_firefly_output = new QAction(this);
  menu_import_firefly_output->setText(tr("OUTPUT"));
  menu_import_firefly->addAction(menu_import_firefly_output);
  connect(menu_import_firefly_output, &QAction::triggered, this,
          [this](){this->import_file("Import Firefly output", "*", qc_file_fmt::firefly_output);});

  menu_import_vasp = menu_import->addMenu("VASP");
  menu_import_vasp_poscar = new QAction(this);
  menu_import_vasp_poscar->setText(tr("POSCAR"));
  connect(menu_import_vasp_poscar,&QAction::triggered, this,
          [this](){this->import_file("Import VASP POSCAR", "*", qc_file_fmt::vasp_poscar);});

  menu_import_vasp->addAction(menu_import_vasp_poscar);
  menu_import_vasp_outcar = new QAction(this);
  menu_import_vasp_outcar->setText(tr("OUTCAR"));
  menu_import_vasp->addAction(menu_import_vasp_outcar);
  connect(menu_import_vasp_outcar,&QAction::triggered, this,
          [this](){this->import_file("Import VASP OUTCAR", "*", qc_file_fmt::vasp_outcar_md);});

  act_save_ws = new QAction(this);
  act_save_ws->setText(tr("Save workspace"));
  act_save_ws->setShortcut(QKeySequence(tr("Ctrl+s")));
  file_menu->addSeparator();
  file_menu->addAction(act_save_ws);
  connect(act_save_ws, &QAction::triggered, this, &main_window::save_workspace);

  act_save_ws_as = new QAction(this);
  act_save_ws_as->setText(tr("Save workspace as"));
  file_menu->addAction(act_save_ws_as);
  connect(act_save_ws_as, &QAction::triggered, this, &main_window::save_workspace_as);

  act_close_app = new QAction(this);
  act_close_app->setText(tr("Close"));
  act_close_app->setShortcut(QKeySequence(tr("Ctrl+q")));
  file_menu->addSeparator();
  file_menu->addAction(act_close_app);
  connect(act_close_app, &QAction::triggered, this, &main_window::slot_shortcut_terminate_app);

  edit_menu  = menuBar()->addMenu(tr("&Edit"));
  act_undo = new QAction(this);
  act_undo->setText(tr("Undo"));
  act_undo->setShortcut(QKeySequence(tr("Ctrl+u")));
  edit_menu->addAction(act_undo);

  act_redo = new QAction(this);
  act_redo->setText(tr("Redo"));
  act_redo->setShortcut(QKeySequence(tr("Ctrl+r")));
  edit_menu->addAction(act_redo);

  edit_menu->addSeparator();

  //Selection menu
  edit_selection_menu = edit_menu->addMenu(tr("Content selection"));
  edit_selection_select_all = new QAction(this);
  edit_selection_select_all->setText(tr("Select all"));
  edit_selection_select_all->setShortcut(QKeySequence(tr("ctrl+a")));
  edit_selection_menu->addAction(edit_selection_select_all);
  connect(edit_selection_select_all, &QAction::triggered,
          this, &main_window::action_select_all_content);

  edit_selection_unselect_all = new QAction(this);
  edit_selection_unselect_all->setText(tr("Unselect all"));
  edit_selection_unselect_all->setShortcut(QKeySequence(tr("ctrl+u")));
  edit_selection_menu->addAction(edit_selection_unselect_all);
  connect(edit_selection_unselect_all, &QAction::triggered,
          this, &main_window::action_unselect_all_content);

  edit_selection_invert = new QAction(this);
  edit_selection_invert->setText(tr("Invert selection"));
  edit_selection_invert->setShortcut(QKeySequence(tr("ctrl+i")));
  edit_selection_menu->addAction(edit_selection_invert);
  connect(edit_selection_invert, &QAction::triggered,
          this, &main_window::action_invert_selected_content);
  //End of selection menu

  switch_between_ws_edit_mode = new QAction(this);
  switch_between_ws_edit_mode->setText(tr("Switch edit mode"));
  switch_between_ws_edit_mode->setShortcut(Qt::Key::Key_Tab);
  connect(switch_between_ws_edit_mode, SIGNAL(triggered()), this, SLOT(toggle_ws_edit_mode()));
  edit_menu->addAction(switch_between_ws_edit_mode);

  edit_menu->addSeparator();

  act_settings = new QAction(this);
  act_settings->setText(tr("Settings"));
  edit_menu->addSeparator();
  edit_menu->addAction(act_settings);

  tools_menu = menuBar()->addMenu(tr("&Tools"));
  tools_menu_generators = tools_menu->addMenu(tr("Generators"));

  act_axial_scale = new QAction(this);
  act_axial_scale->setText(tr("Axial scale"));
  tools_menu->addAction(act_axial_scale);
  connect(act_axial_scale, &QAction::triggered, this, &main_window::dialog_axial_scale);

  act_sc_generator = new QAction(this);
  act_sc_generator->setText(tr("Supercell generator"));
  tools_menu_generators->addAction(act_sc_generator);
  connect(act_sc_generator, &QAction::triggered, this, &main_window::dialog_supercell_generation);

  tools_quick_geom_export = tools_menu->addMenu(tr("Export selected geometry"));
  tools_quick_geom_export_xyz = new QAction(this);
  tools_quick_geom_export_xyz->setText("XYZ");
  tools_quick_geom_export->addAction(tools_quick_geom_export_xyz);
  connect(tools_quick_geom_export_xyz, &QAction::triggered, this,
          [this](){this->export_selected_geometry("Export[xyz]", qc_file_fmt::standart_xyz);});

  tools_quick_geom_export_cp2k_coord = new QAction(this);
  tools_quick_geom_export_cp2k_coord->setText("CP2K coord section");
  tools_quick_geom_export->addAction(tools_quick_geom_export_cp2k_coord);
  connect(tools_quick_geom_export_cp2k_coord, &QAction::triggered, this,
          [this]() { this->export_selected_geometry("Export[CP2K coord. section]",
                                                    qc_file_fmt::cp2k_coord_cell_section);});

  tools_quick_geom_export_vasp_poscar = new QAction(this);
  tools_quick_geom_export_vasp_poscar->setText("VASP POSCAR");
  tools_quick_geom_export->addAction(tools_quick_geom_export_vasp_poscar);
  connect(tools_quick_geom_export_vasp_poscar, &QAction::triggered, this,
          [this]() { this->export_selected_geometry("Export[VASP POSCAR]",
                                                    qc_file_fmt::vasp_poscar);});

  help_menu  = menuBar()->addMenu(tr("&Help"));
  act_about = new QAction(this);
  act_about->setText(tr("About"));
  help_menu->addAction(act_about);
}

void main_window::init_widgets() {

  tool_panel_widget = new QWidget;
  //tool_panel_widget->setStyleSheet("background-color:black;");
  tool_panel_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  tool_panel_widget->setMaximumHeight(45);
  //tool_panel_widget->setStyleSheet("border-bottom: 1px solid gray;margin-bottom:0px;");
  tool_panel_widget->setStyleSheet("padding-bottom:-5px; padding-top:-5px;"
                                   "QLabel {color:white;}");

  tp_ws_selector = new QComboBox;
  QObject::connect(tp_ws_selector,
                   SIGNAL(currentIndexChanged(int)),
                   this,
                   SLOT(ws_selector_selection_changed(int)));
  //tp_ws_selector->setParent(tool_panel_widget);
  tp_ws_selector->setStyleSheet("padding:4px;");
  tp_ws_selector->setMinimumWidth(150);
  tp_ws_selector->setMinimumHeight(tp_button_height);
  //  tp_ws_selector_label = new QLabel;
  //  tp_ws_selector_label->setText(tr("Current workspace:"));

  tp_add_ws = new QPushButton;
  tp_add_ws->setMinimumWidth(30);
  tp_add_ws->setMinimumHeight(tp_button_height);
  tp_add_ws->setText("+");
  connect(tp_add_ws, &QPushButton::pressed, this, &main_window::create_new_workspace);

  tp_rm_ws = new QPushButton;
  tp_rm_ws->setText("-");
  tp_rm_ws->setMinimumWidth(30);
  tp_rm_ws->setMinimumHeight(tp_button_height);
  connect(tp_rm_ws, &QPushButton::pressed, this, &main_window::close_current_workspace);

  tp_rnm_ws = new QPushButton;
  tp_rnm_ws->setText("RN");
  tp_rnm_ws->setMinimumWidth(30);
  tp_rnm_ws->setMinimumHeight(tp_button_height);
  connect(tp_rnm_ws, &QPushButton::pressed, this, &main_window::rename_current_workspace);

  tp_show_obj_insp = new QCheckBox;
  tp_show_obj_insp->setCheckState(Qt::Checked);
  tp_show_obj_insp->setText("Inspector");
  tp_show_obj_insp->setMinimumHeight(tp_button_height);
  QObject::connect(tp_show_obj_insp, SIGNAL(stateChanged(int)),
                   this, SLOT(tp_show_obj_insp_state_changed(int)));
  tp_show_obj_insp->setStyleSheet("border:1px solid gray; border-radius:2px; padding-left:5px; "
                                  "padding-right:5px; "
                                  "QCheckBox::indicator { width: 21px;height: 21px;}");

  tp_show_gizmo = new QCheckBox;
  tp_show_gizmo->setCheckState(Qt::Checked);
  tp_show_gizmo->setText("Gizmo");
  tp_show_gizmo->setMinimumHeight(tp_button_height);
  tp_show_gizmo->setStyleSheet("border:1px solid gray; border-radius:2px; padding-left:5px; "
                               "padding-right:5px; "
                               "QCheckBox::indicator { width: 21px;height: 21px;}");
  QObject::connect(tp_show_gizmo, SIGNAL(stateChanged(int)),
                   this, SLOT(tp_show_gizmo_state_changed(int)));

  tp_edit_mode = new QButtonGroup;
  tp_edit_mode->setExclusive(true);
  QObject::connect(tp_edit_mode, SIGNAL(buttonClicked(int)),
                   this, SLOT(ws_edit_mode_selector_button_clicked(int)));

  tp_edit_mode_item = new QPushButton;
  tp_edit_mode_item->setText(tr("ITM"));
  tp_edit_mode_item->setMinimumWidth(40);
  tp_edit_mode_item->setMinimumHeight(tp_button_height);
  tp_edit_mode_item->setCheckable(true);

  tp_edit_mode_content= new QPushButton;
  tp_edit_mode_content->setText(tr("CNT"));
  tp_edit_mode_content->setMinimumWidth(40);
  tp_edit_mode_content->setMinimumHeight(tp_button_height);
  tp_edit_mode_content->setCheckable(true);

  tp_edit_mode->addButton(tp_edit_mode_item, 0);
  tp_edit_mode->addButton(tp_edit_mode_content, 1);

  tp_edit_mode_start = new QFrame;
  tp_edit_mode_start->setFrameShape(QFrame::VLine);
  tp_edit_mode_start->setFrameShadow(QFrame::Sunken);
  tp_edit_mode_start->setFixedWidth(2);
  tp_edit_mode_start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

  tp_edit_mode_end = new QFrame;
  tp_edit_mode_end->setFrameShape(QFrame::VLine);
  tp_edit_mode_end->setFrameShadow(QFrame::Sunken);
  tp_edit_mode_end->setFixedWidth(2);
  tp_edit_mode_end->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);


  tp_camera_x = new QPushButton(tr("C:X"));
  tp_camera_x->setMinimumWidth(34);
  tp_camera_x->setMinimumHeight(tp_button_height);
  connect(tp_camera_x, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view::tv_x_axis);});

  tp_camera_y = new QPushButton(tr("C:Y"));
  tp_camera_y->setMinimumWidth(34);
  tp_camera_y->setMinimumHeight(tp_button_height);
  connect(tp_camera_y, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view::tv_y_axis);});

  tp_camera_z = new QPushButton(tr("C:Z"));
  tp_camera_z->setMinimumWidth(34);
  tp_camera_z->setMinimumHeight(tp_button_height);
  connect(tp_camera_z, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view::tv_z_axis);});

  tp_camera_a = new QPushButton(tr("C:a"));
  tp_camera_a->setMinimumWidth(34);
  tp_camera_a->setMinimumHeight(tp_button_height);
  connect(tp_camera_a, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view::tv_a_axis);});

  tp_camera_b = new QPushButton(tr("C:b"));
  tp_camera_b->setMinimumWidth(34);
  tp_camera_b->setMinimumHeight(tp_button_height);
  connect(tp_camera_b, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view::tv_b_axis);});

  tp_camera_c = new QPushButton(tr("C:c"));
  tp_camera_c->setMinimumWidth(34);
  tp_camera_c->setMinimumHeight(tp_button_height);
  connect(tp_camera_c, &QPushButton::pressed,
          this, [this](){this->apply_camera_view_change(cam_target_view::tv_c_axis);});

  change_camera_buttons_visible(false, false);

  tp_measure_dist = new QPushButton(tr("DIST"));
  tp_measure_dist->setMinimumWidth(40);
  tp_measure_dist->setMinimumHeight(tp_button_height);

  ws_viewer_widget = new ws_viewer_widget_t(this);
  ws_viewer_widget->setStyleSheet("margin-top:-15px;");

  obj_insp_widget = new object_inspector_widget_t();
  obj_insp_widget->setMaximumWidth(400);
  obj_insp_widget->setMinimumWidth(200);
  //obj_insp_widget->setStyleSheet("margin-top:-15px;");
}

void main_window::init_layouts() {

  main_layout = new QVBoxLayout;
  main_widget->setLayout(main_layout);
  main_layout->addWidget(tool_panel_widget);
  main_layout->setContentsMargins(0,0,0,0);
  main_layout->setSpacing(0);

  layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
  layout_ws_viewer_obj_insp->addWidget(ws_viewer_widget);
  layout_ws_viewer_obj_insp->addWidget(obj_insp_widget);
  layout_ws_viewer_obj_insp->setContentsMargins(0,0,0,0);
  //layout_ws_viewer_obj_insp->setStyleSheet("margin-top:0px;");
  layout_ws_viewer_obj_insp->setHandleWidth(15);
  main_layout->addWidget(layout_ws_viewer_obj_insp);

  tool_panel_layout = new QHBoxLayout;
  tool_panel_widget->setLayout(tool_panel_layout);
  //tool_panel_layout->addWidget(tp_ws_selector_label, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_ws_selector, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_add_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_rm_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_rnm_ws, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_show_obj_insp, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_show_gizmo, 0, Qt::AlignLeft);

  tool_panel_layout->addWidget(tp_edit_mode_start, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_edit_mode_item, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_edit_mode_content, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_edit_mode_end, 0, Qt::AlignLeft);

  tool_panel_layout->addWidget(tp_camera_x, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_y, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_z, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_a, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_b, 0, Qt::AlignLeft);
  tool_panel_layout->addWidget(tp_camera_c, 0, Qt::AlignLeft);

  tool_panel_layout->addWidget(tp_measure_dist, 0, Qt::AlignLeft);

  tool_panel_layout->addStretch(1);
  //  layout_tools_main_window = new QGridLayout;
  //  layout_tools_main_window->setContentsMargins(0,0,0,0);
  //  main_widget->setLayout(layout_tools_main_window);
  //  layout_tools_main_window->addWidget(tool_panel_widget, 0, 0, 1, 2);

  //  layout_ws_viewer_obj_insp = new QSplitter(Qt::Horizontal);
  //  layout_tools_main_window->addLayout()
  //  layout_tools_main_window->addWidget(ws_viewer_placeholder, 1, 0, 1, 1);
  //  layout_tools_main_window->addWidget(obj_inst_placeholder, 1, 1, 1, 1);
}

void main_window::change_camera_buttons_visible(bool cart_c, bool cell_c) {
  tp_camera_x->setVisible(cart_c);
  tp_camera_y->setVisible(cart_c);
  tp_camera_z->setVisible(cart_c);
  tp_camera_a->setVisible(cell_c);
  tp_camera_b->setVisible(cell_c);
  tp_camera_c->setVisible(cell_c);
}

void main_window::workspaces_changed_slot() {

  app_state_t* astate = app_state_t::get_inst();
  tp_ws_selector->clear();

  tp_ws_selector->blockSignals(true);

  if (astate->ws_manager->has_wss()) {
      tp_rm_ws->setEnabled(true);
      tp_rnm_ws->setEnabled(true);
      tp_show_gizmo->setEnabled(true);
      for (auto &ws : astate->ws_manager->m_ws) {
          QString dest = QString::fromStdString(ws->m_ws_name);
          tp_ws_selector->addItem(dest);
        }

      tp_ws_selector->setCurrentIndex(*(astate->ws_manager->get_current_id()));

    } else {
      tp_rm_ws->setEnabled(false);
      tp_rnm_ws->setEnabled(false);
      tp_show_gizmo->setEnabled(false);
    }

  tp_ws_selector->blockSignals(false);
  astate->log("main_window::workspaces_changed_slot()");

}

void main_window::ws_selector_selection_changed(int index) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto current = astate->ws_manager->get_current_id();
      astate->log(fmt::format("ws_selector_selection_changed index: {}, ws_cur_id: {}",
                              index, *current));
      if (current) {
          astate->ws_manager->set_current(index);
          astate->make_viewport_dirty();
        }
    }
}

void main_window::tp_show_obj_insp_state_changed(int state) {

  if (state == Qt::Checked) {
      obj_insp_widget->show();
    }

  if (state == Qt::Unchecked) {
      obj_insp_widget->hide();
    }
}

void main_window::tp_show_gizmo_state_changed(int state) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          cur_ws->m_gizmo->m_is_visible = state == Qt::Checked;
          astate->make_viewport_dirty();
        }
    }

}

void main_window::import_file(QString dialog_name,
                              QString file_ext,
                              qc_file_fmt file_fmt) {

  app_state_t* astate = app_state_t::get_inst();

  QString fileName = QFileDialog::getOpenFileName(this, dialog_name, file_ext);

  if (fileName != "") {
      astate->ws_manager->import_file_as_new_workspace(fileName.toStdString(), file_fmt);
      workspaces_changed_slot();
    }
}

void main_window::export_selected_geometry(QString dialog_name, qc_file_fmt file_fmt) {

  app_state_t* astate = app_state_t::get_inst();

  stop_update_cycle();

  auto cur_ws = astate->ws_manager->get_current();
  if (cur_ws) {
      auto cur_idx = cur_ws->get_selected_idx();
      auto cur_it = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

      if (cur_it) {
          QString file_name =
              QFileDialog::getSaveFileName(nullptr, dialog_name, "", "*.*", nullptr,
                                           QFileDialog::ReadOnly);
          if (file_name != QString::null) cur_it->save_to_file(file_fmt, file_name.toStdString());
        }
    }

  start_update_cycle();

}

void main_window::create_new_workspace() {
  app_state_t* astate = app_state_t::get_inst();
  astate->ws_manager->query_create_new_workspace(true);
  workspaces_changed_slot();
  astate->make_viewport_dirty();
}

void main_window::open_workspace() {
  app_state_t* astate = app_state_t::get_inst();
  QString file_name = QFileDialog::getOpenFileName(this, "Open qpp::cad workspace", "*.json");
  if (file_name != "") {
      astate->ws_manager->load_workspace_from_file(file_name.toStdString());
      workspaces_changed_slot();
    }
}

void main_window::save_workspace() {

  app_state_t* astate = app_state_t::get_inst();

  stop_update_cycle();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          QFileInfo check_file(QString::fromStdString(cur_ws->m_fs_path));
          if (check_file.exists() && check_file.isFile() && cur_ws->m_fs_path != "") {
              cur_ws->save_workspace_to_json(cur_ws->m_fs_path);
            } else {
              QString file_name = QFileDialog::getSaveFileName(this, "Save qpp::cad workspace",
                                                               "*.json");
              if (file_name != "") {
                  cur_ws->save_workspace_to_json(file_name.toStdString());
                  cur_ws->m_fs_path = file_name.toStdString();
                }
            }
        }
    }

  start_update_cycle();

  current_workspace_changed();
}

void main_window::save_workspace_as() {

  app_state_t* astate = app_state_t::get_inst();

  stop_update_cycle();


  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          QString file_name = QFileDialog::getSaveFileName(this, "Save qpp::cad workspace",
                                                           "*.json");
          if (file_name != "") {
              cur_ws->save_workspace_to_json(file_name.toStdString());
              cur_ws->m_fs_path = file_name.toStdString();
            }
        }
    }
  current_workspace_changed();

  start_update_cycle();

}

void main_window::close_current_workspace() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, tr("Workspace -> Close"),
                                        tr("Do you really want to close the workspace?"),
                                        QMessageBox::Yes | QMessageBox::No);
          if (reply == QMessageBox::Yes) {

            }
          else if (reply == QMessageBox::No) {

            }
        }
    }
}

void main_window::rename_current_workspace() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          bool ok;
          QString text = QInputDialog::getText(this, tr("Workspace -> Rename"),
                                               tr("User name:"), QLineEdit::Normal,
                                               QString::fromStdString(cur_ws->m_ws_name), &ok);
          if (ok && text != "") {
              cur_ws->m_ws_name = text.toStdString();
              workspaces_changed_slot();
            }
        }
    }
}

void main_window::current_workspace_changed() {

  app_state_t* astate = app_state_t::get_inst();

  change_camera_buttons_visible(false, false);

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          tp_show_gizmo->setChecked(cur_ws->m_gizmo->m_is_visible);
          std::string title_text = fmt::format("qpp::cad [ws_name: {}] - [path: {}]",
                                               cur_ws->m_ws_name, cur_ws->m_fs_path);
          this->setWindowTitle(QString::fromStdString(title_text));
          act_sc_generator->setEnabled(true);
        } else {
          this->setWindowTitle("qpp::cad");
        }
    } else {
      act_sc_generator->setEnabled(false);
      this->setWindowTitle("qpp::cad");
    }

  current_workspace_properties_changed();
  current_workspace_selected_item_changed();
}

void main_window::current_workspace_selected_item_changed() {

  app_state_t* astate = app_state_t::get_inst();


  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto cur_it_as_al = dynamic_cast<ws_atoms_list_t*>(cur_it);
          if (cur_it_as_al) {
              tp_measure_dist->show();
              if (cur_it_as_al->m_geom->DIM == 3) change_camera_buttons_visible(false, true);
              else change_camera_buttons_visible(true, false);
            }
          else {
              change_camera_buttons_visible(false, false);
              tp_measure_dist->hide();
            }
        }
    }
}

void main_window::current_workspace_properties_changed() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          bool check_t = cur_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM;
          tp_edit_mode_item->blockSignals(true);
          tp_edit_mode_content->blockSignals(true);
          tp_edit_mode_item->setChecked(check_t);
          tp_edit_mode_content->setChecked(!check_t);
          tp_edit_mode_item->blockSignals(false);
          tp_edit_mode_content->blockSignals(false);
        }
    }

  astate->make_viewport_dirty();
}

void main_window::ws_edit_mode_selector_button_clicked(int id) {
  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          if (id == 0) cur_ws->m_edit_type = ws_edit_type::EDIT_WS_ITEM;
          else cur_ws->m_edit_type = ws_edit_type::EDIT_WS_ITEM_CONTENT;
        }
    }

  current_workspace_properties_changed();
}

void main_window::apply_camera_view_change(cam_target_view target_view) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (al) {

              vector3<float> look_from;
              vector3<float> look_to;
              vector3<float> look_up{0.0, 1.0, 0.0};
              bool need_to_update_camera{false};

              switch (target_view) {

                case cam_target_view::tv_x_axis : {
                    float axis_size =
                        std::max(2.0f, al->m_ext_obs->aabb.max[0] - al->m_ext_obs->aabb.min[0]);
                    look_from = al->m_pos + 2.0f*vector3<float>(axis_size, 0.0, 0.0);
                    look_to = al->m_pos;
                    look_up = {0.0 , 0.0 , 1.0};
                    need_to_update_camera = true;
                    break;
                  }

                case cam_target_view::tv_y_axis : {
                    float axis_size =
                        std::max(2.0f, al->m_ext_obs->aabb.max[1] - al->m_ext_obs->aabb.min[1]);
                    look_from = al->m_pos + 2.0f*vector3<float>(0.0, axis_size, 0.0);
                    look_to = al->m_pos;
                    look_up = {0.0, 0.0, 1.0};
                    need_to_update_camera = true;
                    break;
                  }

                case cam_target_view::tv_z_axis : {
                    float axis_size =
                        std::max(2.0f,al->m_ext_obs->aabb.max[2] - al->m_ext_obs->aabb.min[2]);
                    look_from = al->m_pos + 2.0f*vector3<float>(0.0, 0.0, axis_size);
                    look_to = al->m_pos;
                    look_up = {0.0, 1.0, 0.0};
                    need_to_update_camera = true;
                    break;
                  }

                case cam_target_view::tv_a_axis : {
                    vector3<float> center =
                        0.5*(al->m_geom->cell.v[0] +al-> m_geom->cell.v[1] + al->m_geom->cell.v[2]);
                    look_from = al->m_pos + center - 2.0f*al->m_geom->cell.v[0];
                    look_to = al->m_pos  + center;
                    look_up = {0.0 , 0.0 , 1.0};
                    need_to_update_camera = true;
                    break;
                  }

                case cam_target_view::tv_b_axis : {
                    vector3<float> center =
                        0.5*(al->m_geom->cell.v[0] + al->m_geom->cell.v[1] + al->m_geom->cell.v[2]);
                    look_from = al->m_pos + center - 2.0f*al->m_geom->cell.v[1];
                    look_to = al->m_pos  + center;
                    look_up = {0.0, 0.0, 1.0};
                    need_to_update_camera = true;
                    break;
                  }

                case cam_target_view::tv_c_axis : {
                    vector3<float> center =
                        0.5*(al->m_geom->cell.v[0] + al->m_geom->cell.v[1] + al->m_geom->cell.v[2]);
                    look_from = al->m_pos + center - 2.0f*al->m_geom->cell.v[2];
                    look_to = al->m_pos  + center;
                    look_up = {0.0, 1.0, 0.0};
                    need_to_update_camera = true;
                    break;
                  }

                }

              if (need_to_update_camera) {
                  astate->camera->m_view_point = look_from;
                  astate->camera->m_look_at = look_to;
                  astate->camera->m_look_up = look_up;
                  astate->camera->orthogonalize_gs();
                  astate->camera->update_camera();
                  //astate->make_viewport_dirty();
                }

            } // end of if (cur_it_as_al)

        }
    }

  astate->make_viewport_dirty();
}

void main_window::toggle_ws_edit_mode() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          cur_ws->toggle_edit_mode();
          current_workspace_changed();
        }
    }
}

void main_window::start_update_cycle() {
  if (ws_viewer_widget && ws_viewer_widget->m_update_timer) {
      ws_viewer_widget->m_update_timer->start();
    }
}

void main_window::stop_update_cycle() {
  if (ws_viewer_widget && ws_viewer_widget->m_update_timer) {
      p_elapsed_time_in_event_loop =  ws_viewer_widget->m_update_timer->remainingTime();
      ws_viewer_widget->m_update_timer->stop();
      ws_viewer_widget->m_update_timer->setInterval(p_elapsed_time_in_event_loop);
    }
}

void main_window::dialog_supercell_generation() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (al) {
              if (al->m_geom->DIM == 3) {
                  super_cell_widget_t scw;
                  int ret_code = scw.exec();
                  int rep_a = scw.get_replication_coeff(0);
                  int rep_b = scw.get_replication_coeff(1);
                  int rep_c = scw.get_replication_coeff(2);

                  if (ret_code == QDialog::Accepted && (rep_a + rep_b + rep_c >= 1)) {
                      al->make_super_cell(rep_a + 1, rep_b + 1, rep_c + 1);
                      astate->make_viewport_dirty();
                    }
                } else QMessageBox::warning(this, tr("Supercell generation"), tr("m_geom.DIM !=3"));
            }
          else { // is not an atoms list
              QMessageBox::warning(this, tr("Supercell generation"),
                                   tr("ws_item.type != ws_atoms_list"));
            }

        } else {
          QMessageBox::warning(this, tr("Supercell generation"),
                               tr("Workspace not select"));
        }
    }

}

void main_window::dialog_axial_scale() {
  app_state_t* astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      if (cur_ws) {
          auto cur_it = cur_ws->get_selected();
          auto al = dynamic_cast<ws_atoms_list_t*>(cur_it);

          if (al) {
              if (al->m_geom->DIM == 3) {
                  axial_scale_widget_t asw;
                  int ret_code = asw.exec();
                  double sc_a = asw.get_scale_value(0);
                  double sc_b = asw.get_scale_value(1);
                  double sc_c = asw.get_scale_value(2);

                  if (ret_code == QDialog::Accepted) {
                    //  al->make_super_cell(rep_a + 1, rep_b + 1, rep_c + 1);
                      al->apply_axial_scale(sc_a, sc_b, sc_c);
                      astate->make_viewport_dirty();
                    }
                } else QMessageBox::warning(this, tr("Axial scale"), tr("m_geom.DIM !=3"));
            }
          else { // is not an atoms list
              QMessageBox::warning(this, tr("Axial scale"),
                                   tr("ws_item.type != ws_atoms_list"));
            }

        } else {
          QMessageBox::warning(this, tr("Axial scale"),
                               tr("Workspace not select"));
        }
    }
}

void main_window::action_select_all_content() {

  app_state_t* astate = app_state_t::get_inst();

  auto cur_ws = astate->ws_manager->get_current();
  if (cur_ws && cur_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) {

      auto cur_it = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

      if (cur_it) {
          cur_it->select_atoms(true);
        }
    }

}

void main_window::action_unselect_all_content() {

  app_state_t* astate = app_state_t::get_inst();

  auto cur_ws = astate->ws_manager->get_current();
  if (cur_ws && cur_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) {

      auto cur_it = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

      if (cur_it) {
          cur_it->select_atoms(false);
        }
    }
}

void main_window::action_invert_selected_content() {
  app_state_t* astate = app_state_t::get_inst();

  auto cur_ws = astate->ws_manager->get_current();
  if (cur_ws && cur_ws->m_edit_type == ws_edit_type::EDIT_WS_ITEM_CONTENT) {

      auto cur_it = dynamic_cast<ws_atoms_list_t*>(cur_ws->get_selected());

      if (cur_it) {
          cur_it->invert_selected_atoms();
        }
    }
}

void main_window::slot_shortcut_terminate_app() {
  QApplication::quit();
}
