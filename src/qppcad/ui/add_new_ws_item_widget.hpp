#ifndef QPP_CAD_ADD_NEW_WS_ITEM_WIDGET
#define QPP_CAD_ADD_NEW_WS_ITEM_WIDGET

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

namespace qpp {

  namespace cad {

    class add_new_ws_item_widget_t : public QDialog {

      public:

        QVBoxLayout *main_lt;
        QHBoxLayout *data_lt;
        QHBoxLayout *button_lt;
        QPushButton *ok_button;
        QPushButton *cancel_button;

        QGroupBox *gb_ctor;
        QVBoxLayout *gb_ctor_lt;
        QRadioButton *gb_ctor_geom0d;
        QRadioButton *gb_ctor_geom1d;
        QRadioButton *gb_ctor_geom2d;
        QRadioButton *gb_ctor_geom3d;
        QRadioButton *gb_ctor_psg;
        QRadioButton *gb_ctor_pgf_prod;
        QRadioButton *gb_ctor_cube;
        QRadioButton *gb_ctor_arrow;

        QGroupBox *gb_type_descr;
        QVBoxLayout *gb_type_descr_lt;
        QLabel *type_descr_lbl;

        QGroupBox *gb_type_param;
        QFormLayout *gb_type_param_lt;

        QLabel *type_param_name_lbl;
        QLineEdit *type_param_name;
        QLabel *type_param_ag_lbl;
        QComboBox *type_param_ag;

        QStringList descr_list;

        add_new_ws_item_widget_t();

      public slots:

        void ok_button_clicked();
        void cancel_button_clicked();
        void react_gb_ctor_geom0d_checked(bool checked);
        void react_gb_ctor_geom1d_checked(bool checked);
        void react_gb_ctor_geom2d_checked(bool checked);
        void react_gb_ctor_geom3d_checked(bool checked);
        void react_gb_ctor_psg_checked(bool checked);
        void react_gb_ctor_pgf_prod_checked(bool checked);
        void react_gb_ctor_cube_checked(bool checked);
        void react_gb_ctor_arrow_checked(bool checked);


    };

  }

}


#endif