#ifndef QPP_CAD_QBINDED_INPUTS
#define QPP_CAD_QBINDED_INPUTS

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <qppcad/ws_item.hpp>

namespace qpp {

  namespace cad {

    template<typename T>
    class generic_binded_input_t {

      public:
        bool m_ignore_state_change{false};
        bool m_updated_internally_event{true};
        T *m_binded_value{nullptr};
        ws_item_t *m_binded_ws_item{nullptr};

        void bind_value(T *_binded_value, ws_item_t *item_to_bind = nullptr) {
          m_binded_value = _binded_value;
          m_ignore_state_change = true;
          load_value();
          m_ignore_state_change = false;
        }

        //from model to ui
        void load_value() {
          if (m_binded_value) load_value_ex();
        }

        void unbind_value() {
          m_binded_value = nullptr;
          m_binded_ws_item = nullptr;
        }

        void on_value_changed() {
          if (m_binded_ws_item && m_updated_internally_event)
            m_binded_ws_item->updated_internally();
        }

        virtual void load_value_ex() = 0;
    };

    class qbinded_checkbox : public QCheckBox , public generic_binded_input_t<bool> {

        Q_OBJECT

      public:
        void load_value_ex() override;
        qbinded_checkbox(QWidget *parent = nullptr);

      public slots:
        void check_state_changed(int state);
    };

    class qbinded_float_spinbox : public QDoubleSpinBox, public generic_binded_input_t<float> {

        Q_OBJECT

      public:
        void load_value_ex() override;
        void set_min_max_step(double new_min, double new_max, double new_step);
        qbinded_float_spinbox(QWidget *parent = nullptr);

      public slots:
        void value_changed(double d);

    };

    class qbinded_combobox : public QComboBox, public generic_binded_input_t<int> {

        Q_OBJECT

      public:
        void load_value_ex() override;
        qbinded_combobox(QWidget *parent = nullptr);

      public slots:
        void value_changed(int i);
    };

    class qbinded_int3_input : public QWidget, public generic_binded_input_t<vector3<int> > {

        Q_OBJECT

      public:
        QHBoxLayout *widget_layout;
        QSpinBox *sb_x;
        QSpinBox *sb_y;
        QSpinBox *sb_z;
        void load_value_ex() override;
        void set_min_max_step(int min, int max, int step);
        qbinded_int3_input(QWidget *parent = nullptr);

      private slots:
        void spinbox_value_changed(int newval);

    };

    class qbinded_float3_input : public QWidget, public generic_binded_input_t<vector3<float> > {

        Q_OBJECT

      public:
        QHBoxLayout *widget_layout;
        QDoubleSpinBox *sb_x;
        QDoubleSpinBox *sb_y;
        QDoubleSpinBox *sb_z;
        void load_value_ex() override;
        void set_min_max_step(double min, double max, double step);
        qbinded_float3_input(QWidget *parent = nullptr);

      private slots:
        void spinbox_value_changed(double newval);

    };

    class qbinded_color3_input : public QWidget, public generic_binded_input_t<vector3<float> > {

        Q_OBJECT

      public:
        QColor m_stored_color;
        qbinded_color3_input(QWidget *parent = nullptr);
        void load_value_ex() override;
        void mousePressEvent(QMouseEvent *event) override;

      private slots:

    };


  }

}

#endif
