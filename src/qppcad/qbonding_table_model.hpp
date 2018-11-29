#ifndef QPP_CAD_QBONDING_TABLE_MODEL
#define QPP_CAD_QBONDING_TABLE_MODEL

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <QAbstractTableModel>

namespace qpp {

  namespace cad {

    class qbonding_table_model_t : public QAbstractTableModel {

        Q_OBJECT

      private:
        ws_atoms_list_t *m_al{nullptr};
        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

      public:
        void bind(ws_atoms_list_t *_al);
        void unbind();
    };

  }

}


#endif
