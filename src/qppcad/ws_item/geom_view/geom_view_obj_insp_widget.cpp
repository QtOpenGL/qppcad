#include <qppcad/ws_item/geom_view/geom_view_obj_insp_widget.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

void geom_view_obj_insp_widget_t::construct_general_tab() {

  tg_geom_summary_widget = new qspoiler_widget_t(tr("Geometry summary"));
  tg_geom_summary_lt = new QFormLayout;
  tg_geom_summary_widget->add_content_layout(tg_geom_summary_lt);

  tg_geom_summary_total_atoms = new QLabel;
  tg_geom_summary_total_types = new QLabel;

  tg_geom_summary_lt->addRow(tr("Total atoms"), tg_geom_summary_total_atoms);
  tg_geom_summary_lt->addRow(tr("Total types"), tg_geom_summary_total_types);
  init_form_layout(tg_geom_summary_lt);

  tg_type_summary_widget = new qspoiler_widget_t(tr("Type Summary"));
  tg_type_summary_lt = new QVBoxLayout;
  tg_type_summary_widget->add_content_layout(tg_type_summary_lt);
  tg_type_summary_tbl = new QTableWidget;
  tg_type_summary_tbl->setColumnCount(3);

  QStringList table_hdr_cell;
  table_hdr_cell.push_back("X");
  table_hdr_cell.push_back("Y");
  table_hdr_cell.push_back("Z");

  tg_gb_cell = new qspoiler_widget_t(tr("Lattice parameters"));
  tg_gb_cell_lt = new QVBoxLayout;
  tg_gb_cell->add_content_layout(tg_gb_cell_lt);
  tg_gb_cell_tbl = new QTableWidget;

  tg_gb_cell_tbl->setColumnCount(3);
  tg_gb_cell_tbl->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  tg_gb_cell_tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tg_gb_cell_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tg_gb_cell_tbl->setHorizontalHeaderLabels(table_hdr_cell);
  tg_gb_cell_tbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  tg_gb_cell_lt->addWidget(tg_gb_cell_tbl);

  QStringList table_hdr;
  table_hdr.push_back("Name");
  table_hdr.push_back("Count");
  table_hdr.push_back("Color");

  tg_type_summary_tbl->setHorizontalHeaderLabels(table_hdr);
  tg_type_summary_tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tg_type_summary_tbl->verticalHeader()->setVisible(false);
  tg_type_summary_tbl->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  tg_type_summary_tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tg_type_summary_tbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  tg_type_summary_lt->addWidget(tg_type_summary_tbl);

  tab_general->tab_inner_widget_lt->addWidget(tg_geom_summary_widget);
  tab_general->tab_inner_widget_lt->addWidget(tg_type_summary_widget);
  tab_general->tab_inner_widget_lt->addWidget(tg_gb_cell);

  tab_general->tab_inner_widget_lt->addStretch();

}

void geom_view_obj_insp_widget_t::construct_display_tab() {

  gb_disp_s = new qspoiler_widget_t(tr("Draw settings"));
  gb_disp_s_lt = new QFormLayout;
  gb_disp_s->add_content_layout(gb_disp_s_lt);

  disp_s_draw_atoms = new qbinded_checkbox;
  disp_s_draw_bonds = new qbinded_checkbox;
  disp_s_draw_img_atoms = new qbinded_checkbox;
  disp_s_draw_img_bonds = new qbinded_checkbox;

  disp_s_atom_scale = new qbinded_float_spinbox;
  disp_s_atom_scale->set_min_max_step(0.01, 3.0, 0.01);
  disp_s_bond_scale = new qbinded_float_spinbox;
  disp_s_bond_scale->set_min_max_step(0.01, 3.0, 0.01);

  disp_s_render_style = new qbinded_combobox;
  disp_s_render_style->addItem("Balls and sticks");
  disp_s_render_style->addItem("Dynamic lines");
  disp_s_render_style->addItem("X-atom lines");
  disp_s_render_style->addItem("Billboards");
  disp_s_render_style->addItem("Buffered billboards");

  disp_s_color_mode = new qbinded_combobox;
  disp_s_color_mode->addItem(tr("Color from ptable"));
  disp_s_color_mode->addItem(tr("Color from xgeom"));

  disp_draw_subcells = new qbinded_checkbox;
  disp_draw_subcells_label = new QLabel(tr("Draw subcells"));
  disp_subcells_idx = new qbinded_int3_input;
  disp_subcells_idx_label = new QLabel(tr("Subcells range"));

  disp_subcells_idx->set_min_max_step(1, 10, 1);
  connect(disp_draw_subcells, &qbinded_checkbox::stateChanged,
          this, &geom_view_obj_insp_widget_t::draw_subcells_changed);

  disp_s_sel_vis = new qbinded_checkbox;
  disp_s_sel_vis_affect_bonds = new qbinded_checkbox;

  disp_draw_cell = new qbinded_checkbox;
  disp_draw_cell_label = new QLabel(tr("Draw cell"));

  disp_cell_color = new qbinded_color3_input;
  disp_cell_color_label = new QLabel(tr("Cell color"));

  disp_subcell_color = new qbinded_color3_input;
  disp_subcell_color_label = new QLabel(tr("Sub cell color"));

  gb_disp_s_lt->addRow(tr("Draw style"), disp_s_render_style);
  gb_disp_s_lt->addRow(tr("Color style"), disp_s_color_mode);
  gb_disp_s_lt->addRow(tr("Draw atoms"), disp_s_draw_atoms);
  gb_disp_s_lt->addRow(tr("Draw bonds"), disp_s_draw_bonds);
  gb_disp_s_lt->addRow(tr("Draw im. atoms"), disp_s_draw_img_atoms);
  gb_disp_s_lt->addRow(tr("Draw im. bonds"), disp_s_draw_img_bonds);
  gb_disp_s_lt->addRow(tr("Atom scale"), disp_s_atom_scale);
  gb_disp_s_lt->addRow(tr("Bond scale"), disp_s_bond_scale);

  gb_disp_s_lt->addRow(tr("Sel. vis."), disp_s_sel_vis);
  gb_disp_s_lt->addRow(tr("Sel. vis. bonds"), disp_s_sel_vis_affect_bonds);
  gb_disp_s_lt->addRow(disp_draw_cell_label, disp_draw_cell);
  gb_disp_s_lt->addRow(disp_cell_color_label, disp_cell_color);
  gb_disp_s_lt->addRow(disp_draw_subcells_label, disp_draw_subcells);
  gb_disp_s_lt->addRow(disp_subcells_idx_label, disp_subcells_idx);
  gb_disp_s_lt->addRow(disp_subcell_color_label, disp_subcell_color);

  init_form_layout(gb_disp_s_lt);

  gb_disp_labels = new qspoiler_widget_t(tr("Labels"));
  gb_disp_labels_lt = new QFormLayout;
  gb_disp_labels->add_content_layout(gb_disp_labels_lt);

  disp_s_labels_style = new qbinded_combobox;
  disp_s_labels_style->addItem("None");
  disp_s_labels_style->addItem("Id");
  disp_s_labels_style->addItem("Type");
  disp_s_labels_style->addItem("Type and Id");
  disp_s_labels_style->addItem("Charge");
  disp_s_labels_style->addItem("Custom");
  disp_labels_size = new qbinded_int_spinbox;
  disp_labels_size->set_min_max_step(5, 35, 1);
  disp_s_inplace_labels = new qbinded_checkbox;
  disp_sl_labels = new qbinded_checkbox;
  gb_disp_labels_lt->addRow(tr("Labels style"), disp_s_labels_style);
  gb_disp_labels_lt->addRow(tr("Labels size"), disp_labels_size);
  gb_disp_labels_lt->addRow(tr("Inplace labels"), disp_s_inplace_labels);
  gb_disp_labels_lt->addRow(tr("Selective vis."), disp_sl_labels);
  init_form_layout(gb_disp_labels_lt);

  //display - shading tab initialization
  gb_disp_shading = new qspoiler_widget_t(tr("Shading settings"));
  gb_disp_shading_lt = new QFormLayout;
  gb_disp_shading->add_content_layout(gb_disp_shading_lt);

  disp_shading_spec = new qbinded_checkbox;
  disp_shading_spec_value = new qbinded_float_spinbox;
  disp_shading_spec_value->set_min_max_step(0.25, 50, 0.25);
  gb_disp_shading_lt->addRow(tr("Enable specular"), disp_shading_spec);
  gb_disp_shading_lt->addRow(tr("Specular power"), disp_shading_spec_value);
  init_form_layout(gb_disp_shading_lt);

  //display - type specific rendering
  gb_disp_type_spec_rend = new qspoiler_widget_t(tr("Type specific rendering"));
  gb_disp_type_spec_rend_lt = new QFormLayout;
  gb_disp_type_spec_rend->add_content_layout(gb_disp_type_spec_rend_lt);

  disp_type_spec_tv = new QTableView;
  gb_disp_type_spec_rend->setLayout(gb_disp_type_spec_rend_lt);
  gb_disp_type_spec_rend_lt->addWidget(disp_type_spec_tv);

  disp_type_spec_tv->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  disp_type_spec_tv->verticalHeader()->hide();
  disp_type_spec_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  disp_type_spec_tv->setFocusPolicy(Qt::NoFocus);
  disp_type_spec_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  disp_type_spec_mdl = new qtype_specific_rendering_model_t;
  disp_type_spec_tv->setModel(disp_type_spec_mdl);

  //display - bonding table
  gb_disp_bt = new qspoiler_widget_t(tr("Bonding table"));
  gb_disp_bt->setContentsMargins(0,0,0,0);
  disp_bt_lt = new QVBoxLayout;
  gb_disp_bt->add_content_layout(disp_bt_lt);
  bt_mdl = new qbonding_table_model_t;
  disp_bt = new QTableView;

  disp_bt->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  disp_bt->verticalHeader()->hide();
  disp_bt->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
  disp_bt->setFocusPolicy(Qt::NoFocus);
  disp_bt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  disp_bt->setModel(bt_mdl);
  disp_bt_lt->addWidget(disp_bt);
  bt_dist_delegate = new qrealspinbox_delegate_t;
  bt_dist_delegate->set_min_max_step(0.1, 5.0, 0.01);
  disp_bt->setItemDelegateForColumn(2, bt_dist_delegate);

  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_s);
  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_shading);
  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_labels);
  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_type_spec_rend);
  tab_disp->tab_inner_widget_lt->addWidget(gb_disp_bt);

  tab_disp->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_anim_tab() {

  gb_anim_summary = new qspoiler_widget_t(tr("Summary"));
  gb_anim_summary_lt = new QFormLayout;
  gb_anim_summary->add_content_layout(gb_anim_summary_lt);

  gb_anim_summary->setMaximumHeight(250);
  gb_anim_total_anims = new QLabel;
  gb_rebuild_bonds = new qbinded_checkbox;
  gb_play_cyclic = new qbinded_checkbox;
  gb_anim_speed = new qbinded_float_spinbox;
  gb_anim_speed->set_min_max_step(0.01,1.0,0.01);
  gb_anim_total_frames_in_anim = new QLabel;
  gb_anim_cur_frame = new QLabel;
  gb_current_anim = new QComboBox;
  gb_current_anim->setMaximumWidth(def_gen_control_width);

  connect(gb_current_anim,
          SIGNAL(currentIndexChanged(int)),
          this,
          SLOT(cur_anim_index_changed(int)));

  gb_anim_summary_lt->addRow(tr("Num. of anim."), gb_anim_total_anims);
  gb_anim_summary_lt->addRow(tr("Rebuild bonds"), gb_rebuild_bonds);
  gb_anim_summary_lt->addRow(tr("Play in cycle"), gb_play_cyclic);
  gb_anim_summary_lt->addRow(tr("Current anim."), gb_current_anim);
  gb_anim_summary_lt->addRow(tr("Frame time "), gb_anim_speed);
  gb_anim_summary_lt->addRow(tr("Num frm."), gb_anim_total_frames_in_anim);
  gb_anim_summary_lt->addRow(tr("Current frame:"), gb_anim_cur_frame);
  init_form_layout(gb_anim_summary_lt);

  gb_anim_timeline = new qspoiler_widget_t(tr("Timeline"));
  gb_anim_timeline->setMaximumWidth(400);
  gb_anim_timeline_lt = new QHBoxLayout;
  gb_anim_timeline->add_content_layout(gb_anim_timeline_lt);

  gb_anim_timeline_slider = new QSlider(Qt::Orientation::Horizontal);
  gb_anim_timeline_slider->setTickPosition(QSlider::TicksBothSides);
  gb_anim_timeline_slider->setTickInterval(10);
  connect(gb_anim_timeline_slider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(anim_timeline_slider_value_changed(int)));

  //gb_anim_timeline_layout->addWidget(gb_anim_timeline_cur_frame, 1);
  gb_anim_timeline_lt->addWidget(gb_anim_timeline_slider, 1);

  gb_anim_buttons = new qspoiler_widget_t(tr("Control"));
  gb_anim_buttons_lt = new QHBoxLayout;
  gb_anim_buttons->add_content_layout(gb_anim_buttons_lt);
  gb_anim_buttons->setMaximumWidth(400);
  gb_anim_buttons->setMaximumHeight(90);

  anim_play = new QPushButton(tr("PLAY"));
  anim_play->setCheckable(true);
  connect(anim_play,
          SIGNAL(toggled(bool)),
          this,
          SLOT(play_anim_button_toggle(bool)));

  anim_to_start = new QPushButton(tr("<<"));
  connect(anim_to_start,
          SIGNAL(clicked()),
          this,
          SLOT(anim_button_begin_clicked()));

  anim_to_end = new QPushButton(tr(">>"));
  connect(anim_to_end,
          SIGNAL(clicked()),
          this,
          SLOT(anim_button_end_clicked()));

  anim_frame_forward = new QPushButton(tr("+F"));
  connect(anim_frame_forward,
          SIGNAL(clicked()),
          this,
          SLOT(anim_button_frame_move_forward_clicked()));

  anim_frame_backward = new QPushButton(tr("-F"));
  connect(anim_frame_backward,
          SIGNAL(clicked()),
          this,
          SLOT(anim_button_frame_move_backward_clicked()));

  gb_anim_buttons_lt->addWidget(anim_play, 1);
  gb_anim_buttons_lt->addWidget(anim_to_start, 1);
  gb_anim_buttons_lt->addWidget(anim_to_end, 1);
  gb_anim_buttons_lt->addWidget(anim_frame_forward, 1);
  gb_anim_buttons_lt->addWidget(anim_frame_backward, 1);

  tab_anim->tab_inner_widget_lt->addWidget(gb_anim_summary);
  tab_anim->tab_inner_widget_lt->addWidget(gb_anim_timeline);
  tab_anim->tab_inner_widget_lt->addWidget(gb_anim_buttons);

  tab_anim->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_measure_tab() {

  tms_pair_dist_gb = new qspoiler_widget_t(tr("Interatomic distances"));
  tms_pair_dist_gb_lt = new QFormLayout;
  tms_pair_dist_gb->add_content_layout(tms_pair_dist_gb_lt);

  tms_pair_cur_msr = new QComboBox;

  connect(tms_pair_cur_msr,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::measurement_current_index_changed);

  tms_pair_at1_info = new QLabel();
  tms_pair_at2_info = new QLabel();
  tms_pair_dist = new QLabel();

  tms_pair_dist_color = new qbinded_color3_input;
  tms_pair_enabled = new qbinded_checkbox;

  tms_pair_line_size = new qbinded_int_spinbox;
  tms_pair_line_size->set_min_max_step(1, 20, 1);

  tms_font_screen_size = new qbinded_int_spinbox;
  tms_font_screen_size->set_min_max_step(5, 40, 1);

  tms_pair_line_style = new qbinded_combobox;
  tms_pair_line_style->addItem("Solid");
  tms_pair_line_style->addItem("Dotted");
  tms_pair_line_style->addItem("Dashed");
  tms_pair_line_style->addItem("Dash-Dot");
  tms_pair_line_style->addItem("Dash-Dot-Dot");

  tms_pair_label_style = new qbinded_combobox;
  tms_pair_label_style->addItem("Standart");
  tms_pair_label_style->addItem("Outline");
  tms_pair_label_style->addItem("Border");

  tms_pair_dist_gb_lt->addRow(tr("Current"), tms_pair_cur_msr);
  tms_pair_dist_gb_lt->addRow(tr("Atom №1"), tms_pair_at1_info);
  tms_pair_dist_gb_lt->addRow(tr("Atom №2"), tms_pair_at2_info);
  tms_pair_dist_gb_lt->addRow(tr("Line style"), tms_pair_line_style);
  tms_pair_dist_gb_lt->addRow(tr("Line size"), tms_pair_line_size);
  tms_pair_dist_gb_lt->addRow(tr("Color"), tms_pair_dist_color);
  tms_pair_dist_gb_lt->addRow(tr("Enabled"), tms_pair_enabled);
  tms_pair_dist_gb_lt->addRow(tr("Label style"), tms_pair_label_style);
  tms_pair_dist_gb_lt->addRow(tr("Font size(pt)"), tms_font_screen_size);

  init_form_layout(tms_pair_dist_gb_lt);

  tab_measurement->tab_inner_widget_lt->addWidget(tms_pair_dist_gb);
  tab_measurement->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_modify_tab() {

  tm_gb_add_atom = new qspoiler_widget_t(tr("Add atom"));
  tm_gb_add_atom_lt = new QFormLayout;
  tm_gb_add_atom->add_content_layout(tm_gb_add_atom_lt);

  tm_gb_single_atom = new qspoiler_widget_t(tr("Modify single atom"));
  tm_gb_single_atom_lt = new QFormLayout;
  tm_gb_single_atom_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_single_atom->add_content_layout(tm_gb_single_atom_lt);
  tm_single_atom_combo = new QComboBox;
  tm_single_atom_combo->setMaximumWidth(def_gen_control_width);
  tm_single_atom_combo->setEditable(true);
  tm_single_atom_vec3 = new qbinded_float3_input;
  tm_single_atom_vec3->set_min_max_step(-10000, 10000, 0.01);
  tm_single_atom_vec3->set_default_suffix();
  tm_single_atom_idx = new QLabel;
  tm_single_atom_num = new QLabel;

  tm_single_atom_commit = new QPushButton(tr("Commit changes"));
  tm_single_atom_commit->setMaximumWidth(tab_modify_op_button_width);
  connect(tm_single_atom_commit, &QPushButton::pressed,
          this, &geom_view_obj_insp_widget_t::modify_single_atom_button_clicked);

  tm_single_atom_delete = new QPushButton(tr("Delete atom"));
  tm_single_atom_delete->setMaximumWidth(tab_modify_op_button_width);
  connect(tm_single_atom_delete, &QPushButton::pressed,
          this, &geom_view_obj_insp_widget_t::modify_single_atom_delete_button_clicked);

  tm_gb_single_atom_lt->addRow(tr("Atom name"), tm_single_atom_combo);
  tm_gb_single_atom_lt->addRow(tr("Atom idx."), tm_single_atom_idx);
  tm_gb_single_atom_lt->addRow(tr("Atom num."), tm_single_atom_num);
  tm_gb_single_atom_lt->addRow(tr("Atom pos."), tm_single_atom_vec3);
  tm_gb_single_atom_lt->addRow("", tm_single_atom_commit);
  tm_gb_single_atom_lt->addRow("", tm_single_atom_delete);
  qt_helpers::resize_form_lt_labels(tm_gb_single_atom_lt, tab_modify_label_width);

  tm_add_atom_combo = new QComboBox;
  tm_add_atom_combo->setEditable(true);
  tm_add_atom_combo->setMaximumWidth(tab_modify_op_button_width);
  tm_add_atom_vec3 = new qbinded_float3_input;
  tm_add_atom_vec3->set_min_max_step(-1000, 1000, 0.01);
  tm_add_atom_vec3->set_default_suffix();

  tm_add_atom_button = new QPushButton(tr("Add atom"));
  tm_add_atom_button->setMaximumWidth(tab_modify_op_button_width);
  connect(tm_add_atom_button, &QPushButton::pressed,
          this, &geom_view_obj_insp_widget_t::modify_add_atom_button_clicked);

  tm_gb_add_atom_lt->addRow(tr("Atom name"), tm_add_atom_combo);
  tm_gb_add_atom_lt->addRow(tr("Atom pos."), tm_add_atom_vec3);
  tm_gb_add_atom_lt->addRow("", tm_add_atom_button);
  qt_helpers::resize_form_lt_labels(tm_gb_add_atom_lt, tab_modify_label_width);

  tm_gb_pair_dist = new qspoiler_widget_t(tr("Pair distance"));
  tm_gb_pair_dist_lt = new QFormLayout;
  tm_gb_pair_dist_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_pair_dist->add_content_layout(tm_gb_pair_dist_lt);
  tm_pair_dist_atom1 = new QLabel;
  tm_pair_dist_atom2 = new QLabel;
  tm_pair_dist_atom1_idx = new QLabel;
  tm_pair_dist_atom2_idx = new QLabel;
  tm_pair_dist_spinbox = new QDoubleSpinBox;
  tm_pair_dist_note_label = new QLabel;
  tm_pair_dist_t_mode = new QComboBox;
  tm_pair_dist_t_mode->addItem("Transform both");
  tm_pair_dist_t_mode->addItem("Fix first");
  tm_pair_dist_t_mode->addItem("Fix second");
  tm_pair_dist_t_mode->setMaximumWidth(def_gen_control_width);

  tm_pair_dist_note_label->setText(tr("Distance"));
  tm_pair_dist_spinbox->setMinimum(0.0);
  tm_pair_dist_spinbox->setMaximum(10);
  tm_pair_dist_spinbox->setSingleStep(0.01);
  tm_pair_dist_spinbox->setMaximumWidth(def_gen_control_width);

  tm_gb_pair_dist_lt->addRow(tr("Atom №1"), tm_pair_dist_atom1);
  tm_gb_pair_dist_lt->addRow(tr("Atom №2"), tm_pair_dist_atom2);
  tm_gb_pair_dist_lt->addRow(tr("Atom №1 idx"), tm_pair_dist_atom1_idx);
  tm_gb_pair_dist_lt->addRow(tr("Atom №2 idx"), tm_pair_dist_atom2_idx);
  tm_gb_pair_dist_lt->addRow(tr("Trnsf. mode"), tm_pair_dist_t_mode);
  tm_gb_pair_dist_lt->addRow(tm_pair_dist_note_label, tm_pair_dist_spinbox);
  qt_helpers::resize_form_lt_labels(tm_gb_pair_dist_lt, tab_modify_label_width);

  tm_gb_pair_creation = new qspoiler_widget_t(tr("Insert atom between"));
  tm_gb_pair_creation_lt = new QFormLayout;
  tm_gb_pair_creation_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_pair_creation->add_content_layout(tm_gb_pair_creation_lt);
  tm_pair_creation_combo = new QComboBox;
  tm_pair_creation_combo->setEditable(true);
  tm_pair_creation_combo->setMaximumWidth(def_gen_control_width);

  tm_pair_creation_button = new QPushButton("Append");
  tm_pair_creation_button->setMaximumWidth(tab_modify_op_button_width);
  tm_gb_pair_creation_lt->addRow(tr("New atom type"), tm_pair_creation_combo);
  tm_gb_pair_creation_lt->addRow("", tm_pair_creation_button);
  qt_helpers::resize_form_lt_labels(tm_gb_pair_creation_lt, tab_modify_label_width);

  connect(tm_pair_creation_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_add_atom_between_pair);

  connect(tm_pair_dist_spinbox,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &geom_view_obj_insp_widget_t::modify_pair_dist_spinbox_value_changed);

  tm_gb_u_scale = new qspoiler_widget_t(tr("Barycentric scaling"));
  tm_gb_u_scale_lt = new QFormLayout;
  tm_gb_u_scale_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_u_scale->add_content_layout(tm_gb_u_scale_lt);

  tm_u_scale_sb_x = new QDoubleSpinBox;
  tm_u_scale_sb_x->setMinimum(0.01);
  tm_u_scale_sb_x->setMaximum(2.0);
  tm_u_scale_sb_x->setSingleStep(0.01);
  tm_u_scale_sb_x->setValue(1.0);

  tm_u_scale_sb_y = new QDoubleSpinBox;
  tm_u_scale_sb_y->setMinimum(0.01);
  tm_u_scale_sb_y->setMaximum(2.0);
  tm_u_scale_sb_y->setSingleStep(0.01);
  tm_u_scale_sb_y->setValue(1.0);

  tm_u_scale_sb_z = new QDoubleSpinBox;
  tm_u_scale_sb_z->setMinimum(0.01);
  tm_u_scale_sb_z->setMaximum(2.0);
  tm_u_scale_sb_z->setSingleStep(0.01);
  tm_u_scale_sb_z->setValue(1.0);

  tm_u_scale_x_enabled = new QCheckBox;
  tm_u_scale_x_enabled->setChecked(true);

  tm_u_scale_y_enabled = new QCheckBox;
  tm_u_scale_y_enabled->setChecked(true);

  tm_u_scale_z_enabled = new QCheckBox;
  tm_u_scale_z_enabled->setChecked(true);

  tm_u_apply_scale_button = new QPushButton(tr("Apply scale"));
  tm_u_apply_scale_button->setMaximumWidth(tab_modify_op_button_width);
  connect(tm_u_apply_scale_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_barycentric_scale_button_clicked);

  tm_gb_u_scale_lt->addRow("Scale X", tm_u_scale_sb_x);
  tm_gb_u_scale_lt->addRow("Scale Y", tm_u_scale_sb_y);
  tm_gb_u_scale_lt->addRow("Scale Z", tm_u_scale_sb_z);
  tm_gb_u_scale_lt->addRow("Enable::X", tm_u_scale_x_enabled);
  tm_gb_u_scale_lt->addRow("Enable::Y", tm_u_scale_y_enabled);
  tm_gb_u_scale_lt->addRow("Enable::Z", tm_u_scale_z_enabled);
  tm_gb_u_scale_lt->addRow("", tm_u_apply_scale_button);
  qt_helpers::resize_form_lt_labels(tm_gb_u_scale_lt, tab_modify_label_width);

  tm_gb_translate = new qspoiler_widget_t(tr("Translate selected atoms"));
  tm_gb_translate_lt = new QFormLayout;
  tm_gb_translate_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_translate->add_content_layout(tm_gb_translate_lt);
  tm_translate_vec3 = new qbinded_float3_input;
  tm_translate_vec3->set_min_max_step(-10000, 10000, 0.01);
  tm_translate_vec3->set_default_suffix();

  tm_translate_apply_button = new QPushButton(tr("Apply translate"));
  tm_translate_apply_button->setMaximumWidth(tab_modify_op_button_width);

  tm_translate_coord_type_label = new QLabel("Coord. type");
  tm_translate_coord_type = new QComboBox;
  tm_translate_coord_type->setMaximumWidth(def_gen_control_width);

  tm_translate_coord_type->addItem("Cart.");
  tm_translate_coord_type->addItem("Frac.");

  tm_gb_translate_lt->addRow(tm_translate_coord_type_label, tm_translate_coord_type);
  tm_gb_translate_lt->addRow(tr("Tr. vector"), tm_translate_vec3);
  tm_gb_translate_lt->addRow("", tm_translate_apply_button);
  qt_helpers::resize_form_lt_labels(tm_gb_translate_lt, tab_modify_label_width);

  connect(tm_translate_coord_type,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::modify_translate_coord_type_changed);

  connect(tm_translate_apply_button,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_translate_selected_atoms_clicked);

  tm_gb_bc_rot = new qspoiler_widget_t(tr("Rotate selected atoms"));
  tm_gb_bc_rot_lt = new QFormLayout;
  tm_gb_bc_rot_lt->setLabelAlignment(Qt::AlignRight);
  tm_gb_bc_rot->add_content_layout(tm_gb_bc_rot_lt);

  tm_bc_rot_axis = new QComboBox;
  tm_bc_rot_axis->setMaximumWidth(def_gen_control_width);
  tm_bc_rot_axis->addItem("X");
  tm_bc_rot_axis->addItem("Y");
  tm_bc_rot_axis->addItem("Z");
  tm_bc_rot_axis->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);
  tm_bc_rot_axis->setItemData(1, QBrush(Qt::green), Qt::TextColorRole);
  tm_bc_rot_axis->setItemData(2, QBrush(Qt::blue), Qt::TextColorRole);

  tm_bc_rot_angle = new QDoubleSpinBox;
  tm_bc_rot_angle->setMaximumWidth(def_gen_control_width);
  tm_bc_rot_angle->setMinimum(-1000);
  tm_bc_rot_angle->setMaximum(1000);

  tm_bc_rot_angle_type = new QComboBox;
  tm_bc_rot_angle_type->setMaximumWidth(def_gen_control_width);

  connect(tm_bc_rot_angle_type,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &geom_view_obj_insp_widget_t::modify_bc_rot_angle_type_change);

  tm_bc_rot_angle_type->addItem("Degrees");
  tm_bc_rot_angle_type->addItem("Radians");
  tm_bc_rot_apply = new QPushButton(tr("Apply rotation"));
  tm_bc_rot_apply->setMaximumWidth(tab_modify_op_button_width);
  connect(tm_bc_rot_apply,
          &QPushButton::pressed,
          this,
          &geom_view_obj_insp_widget_t::modify_bc_rot_apply);

  tm_gb_bc_rot_lt->addRow(tr("Rotation axis"), tm_bc_rot_axis);
  tm_gb_bc_rot_lt->addRow(tr("Angle type"), tm_bc_rot_angle_type);
  tm_gb_bc_rot_lt->addRow(tr("Angle"), tm_bc_rot_angle);
  tm_gb_bc_rot_lt->addRow("", tm_bc_rot_apply);
  qt_helpers::resize_form_lt_labels(tm_gb_bc_rot_lt, tab_modify_label_width);

  tm_gb_group_op = new qspoiler_widget_t(tr("Group operations"));
  tm_group_op_lt = new QGridLayout;
  tm_gb_group_op->add_content_layout(tm_group_op_lt);
  tm_group_op_sv_show = new QPushButton(tr("SV:SHOW"));
  tm_group_op_sv_hide = new QPushButton(tr("SV:HIDE"));
  tm_group_op_sv_show_all = new QPushButton(tr("SV:SHOW ALL"));

  connect(tm_group_op_sv_show, &QPushButton::pressed,
          this, &geom_view_obj_insp_widget_t::modify_group_op_sv_show);
  connect(tm_group_op_sv_hide, &QPushButton::pressed,
          this, &geom_view_obj_insp_widget_t::modify_group_op_sv_hide);
  connect(tm_group_op_sv_show_all, &QPushButton::pressed,
          this, &geom_view_obj_insp_widget_t::modify_group_op_sv_show_all);

  tm_group_op_sel_ngbs = new QPushButton(tr("SEL:NGB"));
  connect(tm_group_op_sel_ngbs, &QPushButton::pressed,
          this, &geom_view_obj_insp_widget_t::modify_group_op_sel_ngbs);

  tm_group_op_del_sel = new QPushButton(tr("SEL:DEL"));
  connect(tm_group_op_del_sel, &QPushButton::pressed,
          this, &geom_view_obj_insp_widget_t::modify_group_op_del_sel);

  tm_group_op_lt->addWidget(tm_group_op_sv_show,     0, 0, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_sv_hide,     0, 1, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_sv_show_all, 0, 2, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_sel_ngbs,    1, 0, 1, 1);
  tm_group_op_lt->addWidget(tm_group_op_del_sel,    1, 1, 1, 1);

  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_add_atom);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_single_atom);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_pair_dist);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_pair_creation);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_u_scale);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_translate);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_bc_rot);
  tab_modify->tab_inner_widget_lt->addWidget(tm_gb_group_op);

  tab_modify->tab_inner_widget_lt->addStretch(1);

}

void geom_view_obj_insp_widget_t::construct_select_tab() {

}

void geom_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  geom_view_t* _tmp = dynamic_cast<geom_view_t*>(_binding_item);

  assert(_tmp != nullptr);

  if (_tmp) {
      b_al = _tmp;
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void geom_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_al) {

      //General tab
      tg_geom_summary_total_atoms->setText(
            QString::fromStdString(fmt::format("{}", b_al->m_geom->nat())));
      tg_geom_summary_total_types->setText(
            QString::fromStdString(fmt::format("{}", b_al->m_geom->n_atom_types())));

      //update type table
      tg_type_summary_tbl->clearContents();
      tg_type_summary_tbl->setRowCount(b_al->m_geom->n_atom_types());

      for (int i = 0; i < b_al->m_geom->n_atom_types(); i++) {

          QString n_name_str = QString::fromStdString(b_al->m_geom->atom_of_type(i));
          QTableWidgetItem *n_name = new QTableWidgetItem(n_name_str);
          n_name->setTextAlignment(Qt::AlignCenter);
          tg_type_summary_tbl->setItem(i, 0, n_name);

          QString n_c_str = tr("%1").arg(b_al->m_geom->get_atom_count_by_type(i));
          QTableWidgetItem *n_c = new QTableWidgetItem(n_c_str);
          n_c->setTextAlignment(Qt::AlignCenter);
          tg_type_summary_tbl->setItem(i, 1, n_c);

          QString n_clr_str = "";
          QTableWidgetItem *n_clr = new QTableWidgetItem(n_clr_str);
          n_clr->setTextAlignment(Qt::AlignCenter);

          auto ap_idx = ptable::number_by_symbol(b_al->m_geom->atom_of_type(i));
          vector3<float> bc(0.0, 0.0, 1.0);
          if (ap_idx) {bc = ptable::get_inst()->arecs[*ap_idx-1].m_color_jmol;}
          QColor color_bck;
          color_bck.setRgbF(bc[0], bc[1], bc[2]);
          n_clr->setBackgroundColor(color_bck);
          tg_type_summary_tbl->setItem(i, 2, n_clr);

        }

      //resize type table view
      qt_helpers::vrt_resize_tv_to_content(tg_type_summary_tbl);

      //update cell
      cell_changed();
      qt_helpers::vrt_resize_tv_to_content(tg_gb_cell_tbl);

      //display tab
      disp_s_draw_atoms->bind_value(&b_al->m_draw_atoms);
      disp_s_draw_bonds->bind_value(&b_al->m_draw_bonds);
      disp_s_draw_img_atoms->bind_value(&b_al->m_draw_img_atoms);
      disp_s_draw_img_bonds->bind_value(&b_al->m_draw_img_bonds);
      disp_s_atom_scale->bind_value(&b_al->m_atom_scale_factor);
      disp_s_bond_scale->bind_value(&b_al->m_bond_scale_factor);
      disp_s_render_style->bind_value(reinterpret_cast<int*>(&b_al->m_render_style));
      disp_s_color_mode->bind_value(reinterpret_cast<int*>(&b_al->m_color_mode));

      disp_s_labels_style->bind_value(reinterpret_cast<int*>(&b_al->m_labels->m_style));
      disp_labels_size->bind_value(&b_al->m_labels->m_label_font_size);
      disp_s_inplace_labels->bind_value(&b_al->m_labels->m_render_inplace_hud);
      disp_sl_labels->bind_value(&b_al->m_labels->m_selective_label_render);

      disp_shading_spec->bind_value(&b_al->m_draw_specular);
      disp_shading_spec_value->bind_value(&b_al->m_shading_specular_power);

      disp_s_sel_vis->bind_value(&b_al->m_sel_vis);
      disp_s_sel_vis_affect_bonds->bind_value(&b_al->m_sel_vis_affect_bonds);

      disp_draw_cell->bind_value(&b_al->m_draw_cell);
      disp_draw_subcells->bind_value(&b_al->m_draw_subcells);
      disp_subcells_idx->bind_value(&b_al->m_subcells_range);
      disp_cell_color->bind_value(&b_al->m_cell_color);
      disp_subcell_color->bind_value(&b_al->m_subcell_color);

      draw_subcells_changed(0);
      disp_draw_cell->setVisible(b_al->m_geom->DIM == 3);
      disp_draw_cell_label->setVisible(b_al->m_geom->DIM == 3);
      disp_draw_subcells->setVisible(b_al->m_geom->DIM == 3);
      disp_draw_subcells_label->setVisible(b_al->m_geom->DIM == 3);
      disp_subcells_idx->setVisible(b_al->m_geom->DIM == 3 && b_al->m_draw_subcells);
      disp_subcells_idx_label->setVisible(b_al->m_geom->DIM == 3 && b_al->m_draw_subcells);
      disp_subcell_color->setVisible(b_al->m_geom->DIM == 3 && b_al->m_draw_subcells);
      disp_subcell_color_label->setVisible(b_al->m_geom->DIM == 3 && b_al->m_draw_subcells);
      disp_cell_color->setVisible(b_al->m_geom->DIM == 3);
      disp_cell_color_label->setVisible(b_al->m_geom->DIM == 3);
      //anim bindings
      update_animate_section_status();

      gb_anim_total_anims->setText(tr("%1").arg(b_al->m_anim->get_total_anims()));
      gb_rebuild_bonds->bind_value(&b_al->m_anim->m_rebuild_bonds_in_anim);
      gb_play_cyclic->bind_value(&b_al->m_anim->m_play_cyclic);
      gb_anim_speed->bind_value(&b_al->m_anim->m_anim_frame_time);

      //binding current anim combobox
      gb_current_anim->blockSignals(true);
      gb_current_anim->clear();

      for (size_t i = 0; i < b_al->m_anim->get_total_anims(); i++) {
          gb_current_anim->addItem(
                QString::fromStdString(b_al->m_anim->m_anim_data[i].m_anim_name));
        }

      gb_current_anim->setCurrentIndex(b_al->m_anim->m_cur_anim);
      gb_current_anim->blockSignals(false);
      update_anim_tab();

      anim_play->blockSignals(true);
      anim_play->setChecked(b_al->m_anim->m_play_anim);
      anim_play->blockSignals(false);

      disp_type_spec_tv->setModel(nullptr);
      disp_type_spec_mdl->bind(b_al);
      disp_type_spec_tv->setModel(disp_type_spec_mdl);
      disp_type_spec_tv->update();
      qt_helpers::vrt_resize_tv_to_content(disp_type_spec_tv);

      disp_bt->setModel(nullptr);
      bt_mdl->bind(b_al);
      disp_bt->setModel(bt_mdl);
      disp_bt->update();
      qt_helpers::vrt_resize_tv_to_content(disp_bt);

      //bind tab modify

      //end bind tab modify

      update_modify_tab();
      update_measurement_tab();
      update_select_tab();

    }

}

void geom_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

  disp_s_draw_atoms->unbind_value();
  disp_s_draw_bonds->unbind_value();
  disp_s_draw_img_atoms->unbind_value();
  disp_s_draw_img_bonds->unbind_value();
  disp_s_atom_scale->unbind_value();
  disp_s_bond_scale->unbind_value();
  disp_s_render_style->unbind_value();
  disp_s_color_mode->unbind_value();
  disp_s_labels_style->unbind_value();
  disp_labels_size->unbind_value();
  disp_s_inplace_labels->unbind_value();
  disp_sl_labels->unbind_value();
  disp_shading_spec->unbind_value();
  disp_shading_spec_value->unbind_value();
  disp_s_sel_vis->unbind_value();
  disp_s_sel_vis_affect_bonds->unbind_value();
  disp_draw_cell->unbind_value();
  disp_cell_color->unbind_value();
  disp_draw_subcells->unbind_value();
  disp_subcells_idx->unbind_value();
  disp_subcell_color->unbind_value();
  gb_rebuild_bonds->unbind_value();
  gb_play_cyclic->unbind_value();
  gb_anim_speed->unbind_value();

  disp_type_spec_mdl->unbind();
  bt_mdl->unbind();

  unbind_measure_tab();

  b_al = nullptr;

}

void geom_view_obj_insp_widget_t::bind_measure_tab() {

  if (b_al) {

      int cur_msr = b_al->m_measure->m_cur_dist_rec_ui - 1;
      if (cur_msr < b_al->m_measure->m_dist_recs.size() &&
          !b_al->m_measure->m_dist_recs.empty()) {
          auto &rec = b_al->m_measure->m_dist_recs[cur_msr];
          tms_pair_dist_color->bind_value(&rec.m_bond_color);
          tms_pair_enabled->bind_value(&rec.m_show);
          tms_pair_line_size->bind_value(&rec.m_line_size);
          tms_font_screen_size->bind_value(&rec.m_font_size);
          tms_pair_line_style->bind_value(reinterpret_cast<int*>(&rec.m_line_render_style));
          tms_pair_label_style->bind_value(reinterpret_cast<int*>(&rec.m_label_render_style));
          tms_pair_enabled->setEnabled(true);
          tms_pair_dist_color->setEnabled(true);
          tms_pair_line_style->setEnabled(true);
          tms_pair_line_size->setEnabled(true);
          tms_font_screen_size->setEnabled(true);
          tms_pair_label_style->setEnabled(true);
        } else {
          unbind_measure_tab();
        }

    }

}

void geom_view_obj_insp_widget_t::unbind_measure_tab() {

  tms_pair_dist_color->unbind_value();
  tms_pair_enabled->unbind_value();
  tms_pair_line_style->unbind_value();
  tms_pair_line_size->unbind_value();
  tms_font_screen_size->unbind_value();
  tms_pair_label_style->unbind_value();

  tms_pair_line_style->setEnabled(false);
  tms_pair_dist_color->setEnabled(false);
  tms_pair_line_size->setEnabled(false);
  tms_pair_enabled->setEnabled(false);
  tms_pair_enabled->setChecked(false);
  tms_font_screen_size->setEnabled(false);
  tms_pair_label_style->setEnabled(false);

}

void geom_view_obj_insp_widget_t::update_anim_tab() {

  if (b_al) {

      update_anim_tab_visibility();
      cur_anim_index_changed(b_al->m_anim->m_cur_anim);

    }

}

void geom_view_obj_insp_widget_t::update_anim_tab_visibility() {

  if (b_al) {

      auto cur_anim = b_al->m_anim->get_current_anim();
      if (cur_anim && cur_anim->m_anim_type == geom_anim_t::anim_static) {
          gb_anim_buttons->hide();
          gb_anim_timeline->hide();
        }
      else {
          gb_anim_timeline->show();
          gb_anim_buttons->show();
        }
    }

}

void geom_view_obj_insp_widget_t::update_modify_tab() {

  if (b_al) {

      if (b_al->m_parent_ws &&
          b_al->m_parent_ws->m_edit_type == ws_edit_t::edit_content) {

          set_tab_enabled(tab_modify, true);

          if (b_al->m_atom_idx_sel.empty()) {
              tm_gb_single_atom->hide();
              tm_gb_add_atom->show();
              tm_gb_pair_dist->hide();
              tm_gb_u_scale->hide();
              tm_gb_pair_creation->hide();
              tm_gb_translate->hide();
              tm_gb_bc_rot->hide();
              tm_gb_group_op->hide();

              //update atom names combobox
              fill_combo_with_atom_types(tm_add_atom_combo, b_al);
            }

          if (b_al->m_atom_idx_sel.size() == 1) {
              tm_gb_single_atom->show();
              tm_gb_add_atom->hide();
              tm_gb_pair_dist->hide();
              tm_gb_u_scale->hide();
              tm_gb_pair_creation->hide();
              tm_gb_translate->show();
              tm_gb_bc_rot->hide();
              tm_gb_group_op->show();

              fill_combo_with_atom_types(tm_single_atom_combo, b_al);

              auto it = b_al->m_atom_idx_sel.begin();
              if (it != b_al->m_atom_idx_sel.end()) {

                  tm_single_atom_idx->setText(
                        QString::fromStdString(fmt::format("{}", it->m_idx))
                        );

                  tm_single_atom_num->setText(
                        QString::fromStdString(fmt::format("{}", it->m_atm))
                        );

                  tm_single_atom_combo->setCurrentText(
                        QString::fromStdString(b_al->m_geom->atom_name(it->m_atm)));

                  tm_single_atom_vec3->sb_x->setValue(double(b_al->m_geom->pos(it->m_atm)[0]));
                  tm_single_atom_vec3->sb_y->setValue(double(b_al->m_geom->pos(it->m_atm)[1]));
                  tm_single_atom_vec3->sb_z->setValue(double(b_al->m_geom->pos(it->m_atm)[2]));

                } else {

                }

            }

          if (b_al->m_atom_idx_sel.size() == 2) {
              tm_gb_add_atom->hide();
              tm_gb_single_atom->hide();
              tm_gb_pair_dist->show();
              tm_gb_u_scale->hide();
              tm_gb_pair_creation->show();
              tm_gb_translate->show();
              tm_gb_bc_rot->show();
              tm_gb_group_op->show();

              auto it1 = b_al->m_atom_idx_sel.begin();
              auto it2 = it1++;
              if (it2 != b_al->m_atom_idx_sel.end()) {

                  tm_pair_dist_atom1->setText(
                        QString::fromStdString(fmt::format("{}{}",
                                                           b_al->m_geom->atom_name(it1->m_atm),
                                                           it1->m_atm)));
                  tm_pair_dist_atom2->setText(
                        QString::fromStdString(fmt::format("{}{}",
                                                           b_al->m_geom->atom_name(it2->m_atm),
                                                           it2->m_atm)));
                  tm_pair_dist_atom1_idx->setText(
                        QString::fromStdString(fmt::format("{}", it1->m_idx)));
                  tm_pair_dist_atom2_idx->setText(
                        QString::fromStdString(fmt::format("{}", it2->m_idx)));

                  if (it1->m_idx == index::D(b_al->m_geom->DIM).all(0) &&
                      it2->m_idx == index::D(b_al->m_geom->DIM).all(0)) {
                      tm_pair_dist_spinbox->show();
                      float dist_btw = (b_al->m_geom->pos(it1->m_atm, it1->m_idx) -
                                        b_al->m_geom->pos(it2->m_atm, it2->m_idx)).norm();

                      tm_pair_dist_spinbox->blockSignals(true);
                      tm_pair_dist_spinbox->setValue(double(dist_btw));
                      tm_pair_dist_spinbox->blockSignals(false);
                      tm_pair_dist_note_label->show();
                      tm_pair_dist_spinbox->show();
                    } else {
                      tm_pair_dist_note_label->hide();
                      tm_pair_dist_spinbox->hide();
                    }

                  //atom between
                  fill_combo_with_atom_types(tm_pair_creation_combo, b_al);
                }
            }

          if (b_al->m_atom_idx_sel.size() > 2) {
              tm_gb_add_atom->hide();
              tm_gb_single_atom->hide();
              tm_gb_pair_creation->hide();
              tm_gb_pair_dist->hide();
              tm_gb_u_scale->show();
              tm_gb_translate->show();
              tm_gb_bc_rot->show();
              tm_gb_group_op->show();

            }

          if (b_al->m_atom_idx_sel.size() > 0) {
              if (b_al->m_geom->DIM == 3) {
                  tm_translate_coord_type_label->show();
                  tm_translate_coord_type->show();
                } else {
                  tm_translate_coord_type->setCurrentIndex(0);
                  tm_translate_coord_type->hide();
                  tm_translate_coord_type_label->hide();
                }
            }

        } else {

          set_tab_enabled(tab_modify, false);

          tm_gb_add_atom->hide();
          tm_gb_pair_creation->hide();
          tm_gb_single_atom->hide();
          tm_gb_pair_dist->hide();
          tm_gb_u_scale->hide();
          tm_gb_translate->hide();
          tm_gb_bc_rot->hide();
          tm_gb_group_op->hide();

        }

    }

}

void geom_view_obj_insp_widget_t::update_measurement_tab() {

  if (b_al) {

      tms_pair_cur_msr->blockSignals(true);
      tms_pair_cur_msr->clear();

      // check msr pair idx boundary
      //if (tms_pair_cur_msr->currentIndex() )

      tms_pair_cur_msr->addItem(tr("None"));

      index zero = index::D(b_al->m_geom->DIM).all(0);

      for (size_t i = 0; i < b_al->m_measure->m_dist_recs.size(); i++) {

          //compose dist msr name
          int at1 = b_al->m_measure->m_dist_recs[i].m_at1;
          int at2 = b_al->m_measure->m_dist_recs[i].m_at2;
          bool at1_img = b_al->m_measure->m_dist_recs[i].m_idx1 != zero;
          bool at2_img = b_al->m_measure->m_dist_recs[i].m_idx2 != zero;

          std::string item_name = fmt::format("[{}] {}{}{} - {}{}{}",
                                              i,
                                              b_al->m_geom->atom_name(at1),
                                              at1,
                                              at1_img ? "*" : "",
                                              b_al->m_geom->atom_name(at2),
                                              at2,
                                              at2_img ? "*" : "");

          tms_pair_cur_msr->addItem(QString::fromStdString(item_name));

        }

      tms_pair_cur_msr->setCurrentIndex(b_al->m_measure->m_cur_dist_rec_ui);
      update_measurement_tab_info();
      tms_pair_cur_msr->blockSignals(false);

    }

}

void geom_view_obj_insp_widget_t::update_measurement_tab_info() {

  if (b_al) {

      //transform dist msr id
      int cur_dist_msr = b_al->m_measure->m_cur_dist_rec_ui - 1;

      if (cur_dist_msr < 0 || b_al->m_measure->m_dist_recs.empty()) {

          const QString empty_label = "-";
          tms_pair_at1_info->setText(empty_label);
          tms_pair_at2_info->setText(empty_label);
          unbind_measure_tab();

        } else {

          auto &rec = b_al->m_measure->m_dist_recs[cur_dist_msr];

          QString atom1_info = tr("%1 [%2] %3")
                               .arg(QString::fromStdString(b_al->m_geom->atom_name(rec.m_at1)))
                               .arg(rec.m_at1)
                               .arg(QString::fromStdString(fmt::format("{}",rec.m_idx1)));
          tms_pair_at1_info->setText(atom1_info);

          QString atom2_info = tr("%1 [%2] %3")
                               .arg(QString::fromStdString(b_al->m_geom->atom_name(rec.m_at2)))
                               .arg(rec.m_at2)
                               .arg(QString::fromStdString(fmt::format("{}",rec.m_idx2)));
          tms_pair_at2_info->setText(atom2_info);

          bind_measure_tab();

        }

    }

}

void geom_view_obj_insp_widget_t::update_select_tab() {

  if (b_al) {

      if (b_al->m_parent_ws &&
          b_al->m_parent_ws->m_edit_type == ws_edit_t::edit_content) {
          set_tab_enabled(tab_select, true);
        } else {
          set_tab_enabled(tab_select, false);
        }
    }

}

void geom_view_obj_insp_widget_t::fill_combo_with_atom_types(QComboBox *combo,
                                                             geom_view_t *_al) {
  if (_al && combo) {
      combo->clear();
      for (auto i = 0 ; i < _al->m_geom->n_atom_types(); i++)
        combo->addItem(QString::fromStdString(_al->m_geom->atom_of_type(i)));
    }
}

geom_view_obj_insp_widget_t::geom_view_obj_insp_widget_t() : ws_item_obj_insp_widget_t() {

  tab_disp = define_tab(tr("Display settings"),
                        "://images/monitor.svg");

  tab_anim = define_tab(tr("Animation"),
                        "://images/film.svg",
                        "://images/film_d.svg");

  tab_modify = define_tab(tr("Modify"),
                          "://images/outline-build-24px.svg",
                          "://images/outline-build-24px_d.svg");

  tab_measurement = define_tab(tr("Measurement"),
                               "://images/outline-straighten-24px.svg");

  tab_select = define_tab(tr("Atom selection groups"),
                          "://images/outline-select_all-24px.svg",
                          "://images/outline-select_all-24px_d.svg");

  construct_general_tab();
  construct_display_tab();
  construct_anim_tab();
  construct_measure_tab();
  construct_modify_tab();
  construct_select_tab();

  app_state_t *astate = app_state_t::get_inst();

  connect(astate->astate_evd,
          SIGNAL(cur_ws_selected_item_frame_changed_signal()),
          this,
          SLOT(cur_ws_selected_item_frame_changed()));

  connect(astate->astate_evd,
          SIGNAL(cur_ws_selected_atoms_list_cell_changed_signal()),
          this,
          SLOT(cell_changed()));

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selection_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::cur_it_list_selection_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_edit_type_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::cur_ws_edit_mode_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selected_content_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::cur_it_selected_content_changed);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_item_measurements_changed_signal,
          this,
          &geom_view_obj_insp_widget_t::update_measurement_tab);

}

void geom_view_obj_insp_widget_t::cur_anim_index_changed(int index) {

  if (b_al) {

      if (index < int(b_al->m_anim->get_total_anims())) {

          b_al->m_anim->m_cur_anim = index;
          b_al->m_anim->m_cur_anim_time = 0.0f;
          b_al->m_anim->update_geom_to_anim();

          auto cur_anim = b_al->m_anim->get_current_anim();

          if (cur_anim && cur_anim->m_anim_type != geom_anim_t::anim_static) {
              anim_play->blockSignals(true);
              anim_play->setChecked(b_al->m_anim->m_play_anim);
              anim_play->blockSignals(false);
            } else {
              anim_play->blockSignals(true);
              anim_play->setChecked(false);
              anim_play->blockSignals(false);
            }

          gb_anim_total_frames_in_anim->setText(tr("%1").arg(cur_anim->frames.size()));
          gb_anim_timeline_slider->setMinimum(0);
          gb_anim_timeline_slider->setMaximum(cur_anim->frames.size()-1);

        }

      update_anim_tab_visibility();

    }

}

void geom_view_obj_insp_widget_t::play_anim_button_toggle(bool value) {

  if (b_al) {

      b_al->m_anim->m_play_anim = value;

    }

}

void geom_view_obj_insp_widget_t::anim_updated_external() {

  if (b_al) {
      int current_frame_truncated = std::floor(b_al->m_anim->m_cur_anim_time);

      // remap frame id from 0..size-1 1..size
      gb_anim_cur_frame->setText(tr("%1").arg(current_frame_truncated+1));
      gb_anim_timeline_slider->blockSignals(true);
      gb_anim_timeline_slider->setValue(current_frame_truncated);
      gb_anim_timeline_slider->blockSignals(false);

    }

}

void geom_view_obj_insp_widget_t::cur_ws_selected_item_frame_changed() {
  anim_updated_external();
}

void geom_view_obj_insp_widget_t::anim_timeline_slider_value_changed(int value) {

  if (b_al) {
      auto cur_anim = b_al->m_anim->get_current_anim();
      if (cur_anim && value < cur_anim->frames.size()) {
          b_al->m_anim->update_and_set_anim(b_al->m_anim->m_cur_anim, value);
        }
    }

}

void geom_view_obj_insp_widget_t::anim_button_begin_clicked() {
  if (b_al) b_al->m_anim->update_current_frame_to_begin();
}

void geom_view_obj_insp_widget_t::anim_button_end_clicked() {
  if (b_al) b_al->m_anim->update_current_frame_to_end();
}

void geom_view_obj_insp_widget_t::anim_button_frame_move_forward_clicked() {
  if (b_al) b_al->m_anim->manual_frame_manipulate(1);
}

void geom_view_obj_insp_widget_t::anim_button_frame_move_backward_clicked() {
  if (b_al) b_al->m_anim->manual_frame_manipulate(-1);
}

void geom_view_obj_insp_widget_t::cell_changed() {

  if (b_al) {

      if (b_al->m_geom->DIM > 0) {

          tg_gb_cell_tbl->setRowCount(b_al->m_geom->DIM);
          QStringList table_hdr_cell_v;
          table_hdr_cell_v.push_back("a");
          table_hdr_cell_v.push_back("b");
          table_hdr_cell_v.push_back("c");
          tg_gb_cell_tbl->setVerticalHeaderLabels(table_hdr_cell_v);

          for (int c = 0; c < b_al->m_geom->DIM; c++)
            for (int i = 0; i < 3; i++) {
                float cell_amp = b_al->m_geom->cell.v[c][i];
                QTableWidgetItem *n_ax = new QTableWidgetItem(tr("%1").arg(double(cell_amp)));
                tg_gb_cell_tbl->setItem(c, i, n_ax);
              }

          tg_gb_cell_tbl->resizeRowsToContents();
          tg_gb_cell->show();
        } else {
          tg_gb_cell->hide();
        }
    }

}

void geom_view_obj_insp_widget_t::draw_subcells_changed(int state) {

  if (b_al && b_al->m_draw_subcells ) {
      disp_subcells_idx_label->show();
      disp_subcells_idx->show();
      disp_subcell_color->show();
      disp_subcell_color_label->show();

    } else {
      disp_subcells_idx_label->hide();
      disp_subcells_idx->hide();
      disp_subcell_color->hide();
      disp_subcell_color_label->hide();
    }

}

void geom_view_obj_insp_widget_t::update_animate_section_status() {

  set_tab_enabled(tab_anim, b_al->m_anim->animable());

}

void geom_view_obj_insp_widget_t::cur_it_list_selection_changed() {

  if (b_al && b_al->is_selected()) {
      update_modify_tab();
    }

}

void geom_view_obj_insp_widget_t::modify_add_atom_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      vector3<float> new_atom_pos{
        float(tm_add_atom_vec3->sb_x->value()),
            float(tm_add_atom_vec3->sb_y->value()),
            float(tm_add_atom_vec3->sb_z->value())
      };
      std::string new_atom_name = tm_add_atom_combo->currentText().toStdString();
      b_al->insert_atom(new_atom_name, new_atom_pos);
      update_animate_section_status();
      astate->make_viewport_dirty();
    }
}

void geom_view_obj_insp_widget_t::modify_single_atom_button_clicked() {

  if (b_al && b_al->m_atom_idx_sel.size() == 1) {
      auto it = b_al->m_atom_idx_sel.begin();
      if (it != b_al->m_atom_idx_sel.end()) {
          auto itv = *it;
          b_al->update_atom(itv.m_atm, tm_single_atom_combo->currentText().toStdString(),
                            vector3<float>(float(tm_single_atom_vec3->sb_x->value()),
                                           float(tm_single_atom_vec3->sb_y->value()),
                                           float(tm_single_atom_vec3->sb_z->value())));
          update_animate_section_status();
        }
    }
}

void geom_view_obj_insp_widget_t::modify_single_atom_delete_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      b_al->delete_selected_atoms();
      astate->make_viewport_dirty();
      update_animate_section_status();
    }
}

void geom_view_obj_insp_widget_t::modify_pair_dist_spinbox_value_changed(double newval) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      auto it1 = b_al->m_atom_idx_sel.begin();
      auto it2 = it1++;
      if (it2 != b_al->m_atom_idx_sel.end() &&
          it1->m_idx == index::D(b_al->m_geom->DIM).all(0) &&
          it2->m_idx == index::D(b_al->m_geom->DIM).all(0)) {

          pair_dist_mode mode;
          mode = static_cast<pair_dist_mode>(tm_pair_dist_t_mode->currentIndex());
          b_al->update_inter_atomic_dist(float(newval), it1->m_atm, it2->m_atm, mode);

          astate->make_viewport_dirty();
        }
    }
}

void geom_view_obj_insp_widget_t::modify_add_atom_between_pair() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      auto it1 = b_al->m_atom_idx_sel.begin();
      auto it2 = it1++;
      vector3<float> r_btw{0.0, 0.0, 0.0};

      if (it1 != b_al->m_atom_idx_sel.end() && it2 != b_al->m_atom_idx_sel.end())
        r_btw = (b_al->m_geom->pos(it1->m_atm, it1->m_idx) +
                 b_al->m_geom->pos(it2->m_atm, it2->m_idx))*0.5f;

      std::string new_atom_name = tm_pair_creation_combo->currentText().toStdString();
      b_al->insert_atom(new_atom_name, r_btw);
      update_animate_section_status();
      astate->make_viewport_dirty();
    }

}

void geom_view_obj_insp_widget_t::modify_barycentric_scale_button_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      vector3<float> center{0.0f, 0.0f, 0.0f};
      for (auto &rec : b_al->m_atom_idx_sel) center += b_al->m_geom->pos(rec.m_atm);
      center /= b_al->m_atom_idx_sel.size();

      for (auto &rec : b_al->m_atom_idx_sel) {

          float scale_mod_x = 0.0f;
          float scale_mod_y = 0.0f;
          float scale_mod_z = 0.0f;

          if (tm_u_scale_x_enabled->checkState() == Qt::Checked) scale_mod_x = 1.0f;
          if (tm_u_scale_y_enabled->checkState() == Qt::Checked) scale_mod_y = 1.0f;
          if (tm_u_scale_z_enabled->checkState() == Qt::Checked) scale_mod_z = 1.0f;

          vector3<float> new_pos_dist = center - b_al->m_geom->pos(rec.m_atm);
          vector3<float> new_pos = b_al->m_geom->pos(rec.m_atm);

          new_pos[0] +=  (1-float(tm_u_scale_sb_x->value())) * new_pos_dist[0] * scale_mod_x;
          new_pos[1] +=  (1-float(tm_u_scale_sb_y->value())) * new_pos_dist[1] * scale_mod_y;
          new_pos[2] +=  (1-float(tm_u_scale_sb_z->value())) * new_pos_dist[2] * scale_mod_z;

          b_al->update_atom(rec.m_atm, new_pos);
        }

      update_animate_section_status();
      astate->make_viewport_dirty();
    }
}

void geom_view_obj_insp_widget_t::modify_translate_selected_atoms_clicked() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {

      vector3<float> tr_vec(float(tm_translate_vec3->sb_x->value()),
                            float(tm_translate_vec3->sb_y->value()),
                            float(tm_translate_vec3->sb_z->value()));

      if (tm_translate_coord_type->currentIndex() == 1 && b_al->m_geom->DIM == 3) {
          vector3<float> tr_vec_c = tr_vec;
          tr_vec = b_al->m_geom->cell.frac2cart(tr_vec_c);
        }

      for (auto &rec : b_al->m_atom_idx_sel) {
          b_al->update_atom(rec.m_atm, b_al->m_geom->pos(rec.m_atm) + tr_vec);
        }

    }

  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::modify_translate_coord_type_changed(int coord_type) {

  if (coord_type == 0) {
      tm_translate_vec3->set_min_max_step(-10000, 10000, 0.01);
    } else {
      tm_translate_vec3->set_min_max_step(-1.0, 1.0, 0.01);
    }

}

void geom_view_obj_insp_widget_t::modify_bc_rot_angle_type_change(int new_angle_type) {

  if (new_angle_type == 0) tm_bc_rot_angle->setSingleStep(0.5);
  else tm_bc_rot_angle->setSingleStep(0.01);
  tm_bc_rot_angle->setValue(0.0);

}

void geom_view_obj_insp_widget_t::modify_bc_rot_apply() {

  if (b_al && b_al->m_parent_ws && b_al->m_parent_ws->m_edit_type == ws_edit_t::edit_content) {

      float angle;

      if (tm_bc_rot_angle_type->currentIndex() == 0)
        angle = float(tm_bc_rot_angle->value() * qpp::pi) / 180.0f;
      else //bypass input value
        angle = float(tm_bc_rot_angle->value());

      vector3<float> rot_center = b_al->get_gizmo_content_barycenter();

      vector3<float> rot_axis{vector3<float>::Zero()};

      switch (tm_bc_rot_axis->currentIndex()) {
        case 0: {
            rot_axis = {1, 0, 0};
            break;
          }
        case 1: {
            rot_axis = {0, 1, 0};
            break;
          }
        default: {
            rot_axis = {0, 0, 1};
            break;
          }
        }

      Eigen::Affine3f t;
      Eigen::AngleAxisf rot(angle, rot_axis);
      Eigen::Translation<float,3> tb(-rot_center);
      Eigen::Translation<float,3> ta(rot_center);
      t = ta * rot * tb;

      matrix4<float> tm = t.matrix();

      b_al->transform_selected(tm);
      app_state_t *astate = app_state_t::get_inst();
      astate->make_viewport_dirty();

    }

}

void geom_view_obj_insp_widget_t::modify_group_op_sv_show() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      for (auto &rec : b_al->m_atom_idx_sel)
        if (rec.m_idx == index::D(b_al->m_geom->DIM).all(0))
          b_al->m_geom->xfield<bool>(xgeom_sel_vis, rec.m_atm) = false;
    }

  astate->make_viewport_dirty();
}

void geom_view_obj_insp_widget_t::modify_group_op_sv_hide() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      for (auto &rec : b_al->m_atom_idx_sel)
        if (rec.m_idx == index::D(b_al->m_geom->DIM).all(0))
          b_al->m_geom->xfield<bool>(xgeom_sel_vis, rec.m_atm) = true;
    }

  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::modify_group_op_sv_show_all() {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      for (int i = 0; i < b_al->m_geom->nat(); i++)
        b_al->m_geom->xfield<bool>(xgeom_sel_vis, i) = false;
    }

  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::modify_group_op_sel_ngbs() {

  app_state_t *astate = app_state_t::get_inst();
  if (b_al) b_al->select_selected_atoms_ngbs();
  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::modify_group_op_del_sel() {

  app_state_t *astate = app_state_t::get_inst();
  if (b_al) b_al->delete_selected_atoms();
  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::measurement_current_index_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();

  if (b_al) {
      b_al->m_measure->m_cur_dist_rec_ui = index;
      update_measurement_tab_info();
    }

  astate->make_viewport_dirty();

}

void geom_view_obj_insp_widget_t::cur_ws_edit_mode_changed() {

  update_modify_tab();
  update_measurement_tab();
  update_select_tab();

}

void geom_view_obj_insp_widget_t::cur_it_selected_content_changed() {

  update_modify_tab();
  update_measurement_tab();
  update_select_tab();

}
