#ifndef QPPCAD_NODE_BOOK_GRAPHICS_SCENE
#define QPPCAD_NODE_BOOK_GRAPHICS_SCENE

#include <qppcad/qppcad.hpp>
#include <QRectF>
#include <QGraphicsScene>
#include <QPainter>
#include <qppcad/ws_item/node_book/qnode.hpp>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

namespace qpp {

  namespace cad {

    //https://github.com/rochus/qt5-node-editor

    class qnode_t;
    class qnode_socket_t;
    class qnode_connection_t;

    class node_book_graphics_scene_t : public QGraphicsScene {

      Q_OBJECT

      public:

        QMenu *gs_global_menu;
        QMenu *gs_qnode_menu;
        QAction *gs_qnode_menu_delete;
        QAction *gs_qnode_menu_unlink_all;

        std::vector<qnode_t*> m_nodes;
        std::vector<qnode_socket_t*> m_sockets;
        std::vector<qnode_connection_t*> m_connections;

        node_book_graphics_scene_t(QObject *parent);
        void add_connection(qnode_connection_t *_con);
        void add_node(qnode_t *_node);
        void update_connections_with_node(qnode_t *_node);
        void delete_node(qnode_t *_node);
        void unlink_node(qnode_t *_node);
        void construct_new_node(QPointF pos, size_t sflow_fbr_hash);

      protected:

        void drawBackground(QPainter *painter, const QRectF &rect) override;
        bool event(QEvent *event) override;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

      private:

        QColor p_clr_bckgr;
        QColor p_clr_light;
        QColor p_clr_dark;

        QPen p_pen_light;
        QPen p_pen_dark;

        QBrush p_brush_bckgr;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
