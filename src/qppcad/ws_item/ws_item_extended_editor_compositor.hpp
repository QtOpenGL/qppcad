#ifndef QPPCAD_WS_ITEM_EXTENDED_EDITOR_COMPOSITOR
#define QPPCAD_WS_ITEM_EXTENDED_EDITOR_COMPOSITOR

#include <qppcad/qppcad.hpp>
#include <qppcad/ui/qembed_window.hpp>
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace qpp {

  namespace cad {

    class ws_item_extended_editor_compositor_t : public qembed_window_t {

        Q_OBJECT

      public:

        QWidget *ext_edt_plch;
        QPushButton *top_btn_close;
        ws_item_extended_editor_compositor_t(QWidget *parent = nullptr);

      public slots:

        void open_requested();
        void cur_ws_selected_item_changed();
        void cur_ws_changed();

        void close_ext_editor_btn_clicked();

    };

  }

}

#endif
