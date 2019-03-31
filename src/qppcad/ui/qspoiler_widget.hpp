#ifndef QPP_CAD_QSPOILER_WIDGET
#define QPP_CAD_QSPOILER_WIDGET

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <qppcad/qppcad.hpp>

namespace qpp {

  namespace cad {

    class qspoiler_widget_t : public QFrame {

        Q_OBJECT

      public:

        QVBoxLayout *main_lt;
        QFrame *top_frm;
        QHBoxLayout *hbox_frm;
        QLabel *lbl_frm;
        QPushButton *action_btn;
        QWidget *widget_list;
        QVBoxLayout *widget_list_lt;

        bool m_state{true};
        bool m_collapsable{false};

        explicit qspoiler_widget_t(const QString & title = "",
                                   QWidget *parent = 0,
                                   bool _collapsable = false,
                                   int _spacing = 6,
                                   int _max_width = 360);

        void add_content_layout(QLayout *new_lt);
        void process_state();

      public slots:

        void collapse_button_pressed();

    };

  }

}

#endif
