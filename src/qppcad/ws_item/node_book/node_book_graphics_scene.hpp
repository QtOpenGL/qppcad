#ifndef QPP_CAD_NODE_BOOK_GRAPHICS_SCENE
#define QPP_CAD_NODE_BOOK_GRAPHICS_SCENE

#include <qppcad/qppcad.hpp>
#include <QRectF>
#include <QGraphicsScene>
#include <QPainter>

namespace qpp {

  namespace cad {

    //https://github.com/rochus/qt5-node-editor
    class node_book_graphics_scene_t : public QGraphicsScene {

      Q_OBJECT

      public:

        node_book_graphics_scene_t(QObject *parent);

      protected:

        virtual void drawBackground(QPainter *painter, const QRectF &rect) override;

      private:

        QColor p_clr_bckgr;
        QColor p_clr_light;
        QColor p_clr_dark;

        QPen p_pen_light;
        QPen p_pen_dark;

        QBrush p_brush_bckgr;

    };

  }

}

#endif
