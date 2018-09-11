#ifndef QPP_WS_ATOM_LIST_H
#define QPP_WS_ATOM_LIST_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/geom.hpp>
#include <geom/xgeom.hpp>
#include <geom/ngbr.hpp>
#include <geom/extents_observer.hpp>
#include <geom/tws_tree.hpp>
#include <data/color.hpp>
#include <symm/index_set.hpp>
#include <qppcad/ws_item.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/file_formats.hpp>


namespace qpp{

  ///
  /// \brief The ws_atom_list_t class
  ///
  class ws_atoms_list_t : public ws_item_t {
    public:

      bool need_to_rebuild_nbt;

      ///
      /// \brief display_imaginary_atoms
      ///
      bool m_show_imaginary_atoms;
      bool m_show_imaginary_bonds;
      bool m_show_bonds;
      bool m_show_atoms;
      bool m_draw_line_in_dist_measurement;

      unique_ptr<xgeometry<float, periodic_cell<float> > >          m_geom;
      unique_ptr<bonding_table<float> >                             m_bt;
      unique_ptr<neighbours_table<float> >                          m_nt;
      unique_ptr<tws_tree_t<float, periodic_cell<float> > >         m_tws_tr;
      unique_ptr<extents_observer_t<float, periodic_cell<float> > > m_ext_obs;
      unordered_set<uint16_t>                                       m_atom_selection;
      unordered_set<atom_index_set_key, atom_index_set_key_hash>    m_atom_idx_selection;

      vector3<float> m_gizmo_barycenter;

      ws_atoms_list_t(workspace_t* parent);

      void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                 vector3<float> &vOutLookAt) override ;
      ///
      /// \brief geometry_changed
      ///
      void geometry_changed();
      void render() override;

      ///
      /// \brief render_atom
      /// \param atNum
      /// \param atIndex
      ///
      void render_atom(const uint16_t at_num, const index &at_index);

      ///
      /// \brief render_bond
      /// \param atNum1
      /// \param atIndex1
      /// \param atNum2
      /// \param atIndex2
      ///
      void render_bond(const uint16_t at_num1, const index &at_index1,
                       const uint16_t at_num2, const index &at_index2);

      void render_ui() override;
      bool mouse_click(ray_t<float> *click_ray) override;


      bool support_translation() override;
      bool support_rotation() override;
      bool support_scaling() override;
      bool support_content_editing() override;
      bool support_selection() override;
      bool support_rendering_bounding_box() override;
      std::string compose_item_name() override;
      void update(float delta_time) override;
      float get_bb_prescaller() override;

      uint32_t get_amount_of_selected_content() override;

      ///
      void on_begin_content_gizmo_translate() override;
      void apply_intermediate_translate_content(const vector3<float> &new_pos) override;
      void on_end_content_gizmo_translate() override;
      void recalc_gizmo_barycenter();
      const vector3<float> get_gizmo_content_barycenter() override;

      ///
      /// \brief shift
      /// \param vShift
      ///
      void shift(const vector3<float> shift);

      ///
      /// \brief load_from_file
      /// \param eFileFormat
      /// \param sFileName
      /// \param bAutoCenter
      ///
      void load_from_file(qc_file_format file_format, std::string file_name,
                          bool auto_center = false);

      void rebuild_ngbt();

      ~ws_atoms_list_t() override {

      }

  };

}

#endif