#ifndef QPP_XGEOM_TABLE_MODEL
#define QPP_XGEOM_TABLE_MODEL

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <QAbstractTableModel>

namespace qpp {

  namespace cad {

    class xgeom_table_model_t : public QAbstractTableModel {

        Q_OBJECT

      private:

        xgeometry<float, periodic_cell<float>> *m_xgeom{nullptr};

        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

      public:

        void bind(xgeometry<float, periodic_cell<float>> *_xgeom);
        void unbind();

    };

  }

}


#endif
