#ifndef QPPCAD_RENDER_BS
#define QPPCAD_RENDER_BS
#include <qppcad/ws_item/geom_view/geom_view.hpp>

namespace qpp {

  namespace cad {

    struct geom_view_render_bs {

        static void render(geom_view_t &al);
        static void render_atom(geom_view_t &al, const uint32_t at_num,
                                const index &at_index);
        static void render_bond(geom_view_t &al,
                                const uint32_t at_num1, const index &at_index1,
                                const uint32_t at_num2, const index &at_index2);

        static void render_suprematic(geom_view_t &al);
        static void render_atom_suprematic(geom_view_t &al, const uint32_t at_num,
                                           const index &at_index,
                                           bool backpass = false);
        static void render_bond_suprematic(geom_view_t &al,
                                           const uint32_t at_num1, const index &at_index1,
                                           const uint32_t at_num2, const index &at_index2,
                                           bool backpass = false);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
