#include <qppcad/register_all_things.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list_io.hpp>
#include <qppcad/ws_comp_chem_data/ws_comp_chem_data.hpp>
#include <qppcad/ws_volume_data/ws_volume_data.hpp>
#include <qppcad/ws_point_sym_group/ws_point_sym_group.hpp>
#include <qppcad/ws_traj_highlight/ws_traj_highlight.hpp>

#include <qppcad/ws_atoms_list/ws_atoms_list_obj_insp_widget.hpp>
#include <qppcad/ws_volume_data/ws_volume_data_obj_insp_widget.hpp>
#include <qppcad/ws_comp_chem_data/ws_comp_chem_data_obj_insp_widget.hpp>
#include <qppcad/ws_point_sym_group/ws_point_sym_group_obj_insp_widget.hpp>
#include <qppcad/ws_traj_highlight/ws_traj_highlight_obj_insp_widget.hpp>

#include <qppcad/tools/supercell/supercell.hpp>
#include <qppcad/tools/axial_scale/axial_scale.hpp>
#include <qppcad/tools/clamp_atoms_to_cell/clamp_atoms_to_cell.hpp>

using namespace qpp;
using namespace qpp::cad;

void registration_helper_t::reg_ws_item_fbr(ws_item_behaviour_manager_t *bhv_mgr) {
  reg_ws_item_fbr_t<ws_atoms_list_t>(bhv_mgr);
  reg_ws_item_fbr_t<ws_comp_chem_data_t>(bhv_mgr);
  reg_ws_item_fbr_t<ws_volume_data_t>(bhv_mgr);
  reg_ws_item_fbr_t<ws_point_sym_group_t>(bhv_mgr);
  reg_ws_item_fbr_t<ws_traj_highlight_t>(bhv_mgr);
}

void registration_helper_t::reg_ws_item_obj_insp(ws_item_behaviour_manager_t *bhv_mgr) {
  reg_ws_item_obj_insp_t<ws_atoms_list_t, ws_atoms_list_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_t<ws_volume_data_t, ws_volume_data_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_t<ws_comp_chem_data_t, ws_comp_chem_data_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_t<ws_point_sym_group_t, ws_point_sym_group_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_t<ws_traj_highlight_t, ws_traj_highlight_obj_insp_widget_t>(bhv_mgr);
}

void registration_helper_t::reg_ws_item_io_bhv(ws_item_behaviour_manager_t *bhv_mgr) {

  size_t xyz_ff_g_hash = bhv_mgr->reg_ffg("XYZ", "xyz");
  size_t vasp_ff_g_hash = bhv_mgr->reg_ffg("VASP", "vasp");
  size_t firefly_ff_g_hash = bhv_mgr->reg_ffg("Firefly", "ff");
  size_t cp2k_ff_g_hash = bhv_mgr->reg_ffg("CP2K", "cp2k");
  size_t generic_ff_g_hash = bhv_mgr->reg_ffg("Generic formats", "generic");

  size_t xyz_ff_hash =
      bhv_mgr->reg_ff("Standart XYZ", "xyz", xyz_ff_g_hash, {".xyz"});

  size_t xyzq_ff_hash =
      bhv_mgr->reg_ff("UC(XYZQ)", "uc", xyz_ff_g_hash, {".uc"});

  size_t poscar_ff_hash =
      bhv_mgr->reg_ff("VASP POSCAR", "poscar", vasp_ff_g_hash, {"POSCAR", ".vasp", ".VASP"} );

  size_t outcar_ff_hash =
      bhv_mgr->reg_ff("VASP OUTCAR", "outcar", vasp_ff_g_hash, {"OUTCAR"} );

  size_t firefly_out_ff_hash =
      bhv_mgr->reg_ff("FF OUTPUT", "ffout", firefly_ff_g_hash, {".out", ".ff"} );

  size_t cp2k_out_ff_hash =
      bhv_mgr->reg_ff("CP2K OUTPUT", "cp2k", cp2k_ff_g_hash, {"cp2k", ".cout"} );

  size_t cp2k_cs_ff_hash =
      bhv_mgr->reg_ff("CP2K Coord.", "cp2kcs", cp2k_ff_g_hash, {".coord", ".cp2k_crd"} );

  size_t generic_cube_ff_hash =
      bhv_mgr->reg_ff("CUBE file", "cube", generic_ff_g_hash, {".cube", ".CUBE"} );

  auto xyz_ff_mgr =
      std::make_shared<
      ws_atoms_list_io_ccd_t<
      read_ccd_from_xyz_file<float>, true, false, true, true, false >
      >();

  auto xyz_s_ff_mgr =
      std::make_shared<
      ws_atoms_list_io_saver_t<
      write_xyz<float, periodic_cell<float> > >
      >();

  auto xyzq_mgf =
      std::make_shared<
      ws_atoms_list_io_loader_t<
      read_xyzq_wrp_def<float, periodic_cell<float> >, ws_atoms_list_role_t::r_uc, 3 >
      >();

  auto xyzq_s_mgf =
      std::make_shared<
      ws_atoms_list_io_saver_t<
      write_xyzq<float, periodic_cell<float> >, true, 3 >
      >();

  auto ff_output_mgf =
      std::make_shared<
      ws_atoms_list_io_ccd_t<
      read_ccd_from_firefly_output<float>, true, true, true, true, false, 0 >
      >();

  auto cp2k_output_mgf =
      std::make_shared<
      ws_atoms_list_io_ccd_t<
      read_ccd_from_cp2k_output<float>, true, true, true, false, true >
      >();

  auto cp2k_cs_mgf =
      std::make_shared<
      ws_atoms_list_io_saver_t<
      write_cp2k_coord_section<float, periodic_cell<float> >, true, 3 >
      >();

  auto vasp_poscar_mgf =
      std::make_shared<
      ws_atoms_list_io_loader_t<
      read_vasp_poscar<float, periodic_cell<float> >, ws_atoms_list_role_t::r_generic, 3 >
      >();

  auto vasp_poscar_s_mgf =
      std::make_shared<
      ws_atoms_list_io_saver_t<
      write_vasp_poscar<
      float, periodic_cell<float> >, true, 3 >
      >();

  auto vasp_outcar_mgf =
      std::make_shared<
      ws_atoms_list_io_anim_loader_t<
      read_vasp_outcar_md_with_frames<float, periodic_cell<float> >, 3 >
      >();

  auto generic_cube_mgf =
      std::make_shared<ws_atoms_list_io_cube_t
      >();


  bhv_mgr->reg_io_bhv(xyz_ff_mgr, xyz_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(xyz_s_ff_mgr, xyz_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(xyzq_mgf, xyzq_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(xyzq_s_mgf, xyzq_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(ff_output_mgf, firefly_out_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(cp2k_output_mgf, cp2k_out_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(cp2k_cs_mgf, cp2k_cs_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(vasp_poscar_mgf, poscar_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(vasp_poscar_s_mgf, poscar_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(vasp_outcar_mgf, outcar_ff_hash, ws_atoms_list_t::get_type_static());
  bhv_mgr->reg_io_bhv(generic_cube_mgf, generic_cube_ff_hash, ws_atoms_list_t::get_type_static());

}

void registration_helper_t::reg_ws_item_tools(ws_item_behaviour_manager_t *bhv_mgr) {

  size_t hash_t_generator = bhv_mgr->reg_tool_grp("Generators");
  size_t hash_t_tr = bhv_mgr->reg_tool_grp("Modify geometry");
  size_t hash_t_generic = bhv_mgr->reg_tool_grp("Generic tools");

  registration_helper_t::reg_ws_item_tool<supercell_tool_t, ws_atoms_list_t>(
        "Supercell",
        hash_t_generator,
        bhv_mgr);
  registration_helper_t::reg_ws_item_tool<axial_scale_tool_t, ws_atoms_list_t>(
        "Axial scale",
        hash_t_tr,
        bhv_mgr);
  registration_helper_t::reg_ws_item_tool<clamp_atoms_to_cell_tool_t, ws_atoms_list_t>(
        "Clamp atoms to cell(3D)",
        hash_t_tr,
        bhv_mgr);
}
