#ifndef QPP_CAD_WS_ATOMS_LIST_MEASURE_S
#define QPP_CAD_WS_ATOMS_LIST_MEASURE_S

#include <geom/geom.hpp>
#include <qppcad/qppcad.hpp>
#include <QPainter>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    class ws_atoms_list_t;

    template<typename AINT = uint32_t>
    struct measurement_bond_record {

        AINT m_at1;
        AINT m_at2;
        index m_idx1;
        index m_idx2;
        bool m_show{true};

        measurement_bond_record (const AINT _at1, const AINT _a2,
                                 const index _idx1, const index _idx2) :
          m_at1(_at1), m_at2(_a2), m_idx1(_idx1), m_idx2(_idx2) {}

    };

    template<typename AINT = uint32_t>
    struct measurement_angle_record {

        AINT m_at1;
        AINT m_at2;
        AINT m_at3;
        index m_idx1;
        index m_idx2;
        index m_idx3;

        int m_order{1};

        bool m_show{true};
        measurement_angle_record (const AINT _at1, const AINT _a2, const AINT _a3,
                                  const index _idx1, const index _idx2, const index _idx3) :
          m_at1(_at1), m_at2(_a2), m_at3(_a3), m_idx1(_idx1), m_idx2(_idx2), m_idx3(_idx3) {}
    };


    class ws_atoms_list_measurement_subsys_t {

      private:

        ws_atoms_list_t* p_owner;

      public:
        std::vector<measurement_bond_record<uint32_t> > m_dist_recs;
        std::vector<measurement_angle_record<uint32_t> > m_angle_recs;

        ws_atoms_list_measurement_subsys_t (ws_atoms_list_t &_p_owner);

        float dist (const size_t idx);

        void add_bond_measurement (const uint32_t atm1,
                                   const uint32_t atm2,
                                   const index idx1,
                                   const index idx2);

        void add_angle_measurement (const uint32_t atm1,
                                    const uint32_t atm2,
                                    const uint32_t atm3,
                                    const index idx1,
                                    const index idx2,
                                    const index idx3);

        void remove_bond_measurement (const size_t measure_idx);
        void remove_angle_measurement (const size_t measure_idx);

        std::optional<size_t> is_bond_measurement_exist (const uint32_t atm1,
                                                         const uint32_t atm2,
                                                         const index idx1,
                                                         const index idx2);

        std::optional<size_t> is_angle_measurement_exist(const uint32_t atm1,
                                                         const uint32_t atm2,
                                                         const uint32_t atm3,
                                                         const index idx1,
                                                         const index idx2,
                                                         const index idx3);
        void render ();
        void render_overlay(QPainter &painter);
        void notify_atom_has_been_deleted(const uint32_t atm);

    };

  }

}

#endif
