#include <qppcad/ws_item/pdos_view/pdos_view_extended_editor.hpp>
#include <QBarSeries>
#include <QScatterSeries>
#include <QSplineSeries>
#include <QValueAxis>

using namespace qpp;
using namespace qpp::cad;

pdos_view_extended_editor_t::pdos_view_extended_editor_t() {

  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0,0,10,10);
  setLayout(main_lt);


  pdos_gen_chart_view = new QChartView;
  pdos_gen_chart_view->setRubberBand(QChartView::RectangleRubberBand);
 // pdos_gen_chart_view->setChart(pdos_gen_chart);

  main_lt->addWidget(pdos_gen_chart_view);

}

void pdos_view_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {


  if (!_binding_item) {
      m_binded_nx = nullptr;
      pdos_gen_chart_view->setChart(nullptr);
      return;
    }

  auto as_nx = _binding_item->cast_as<pdos_view_t>();
  if (!as_nx) {
      m_binded_nx = nullptr;
      pdos_gen_chart_view->setChart(nullptr);
      return;
    }

  m_binded_nx = as_nx;
  pdos_gen_chart_view->setChart(m_binded_nx->m_pdos_gen_chart);

  ws_item_extended_editor_t::bind_to_item(_binding_item);

}

void pdos_view_extended_editor_t::update_from_ws_item() {

  ws_item_extended_editor_t::update_from_ws_item();

  if (!m_binded_nx) return;

}

void pdos_view_extended_editor_t::unbind_item() {

  ws_item_extended_editor_t::unbind_item();
  m_binded_nx = nullptr;

}

bool pdos_view_extended_editor_t::can_be_binded_to(ws_item_t *item) {
  return true;
}

QString pdos_view_extended_editor_t::header_name_hint() {
  return "pdos_view_extended_editor EDITOR";
}
