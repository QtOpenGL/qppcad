#include <iostream>
#include <QApplication>
#include <QStyleFactory>
#include <QFontDatabase>
#include <qppcad/main_window.hpp>
#include <qppcad/app_state.hpp>
#include <QTextStream>

using namespace qpp;
using namespace qpp::cad;

int main (int argc, char **argv) {

  QCoreApplication::setOrganizationName("igc");
  QCoreApplication::setOrganizationDomain("igc.irk.ru");
  QCoreApplication::setApplicationName("qppcad");

  app_state_t::init_inst();
  app_state_t::get_inst()->init_managers();
  app_state_t::get_inst()->load_settings();

  QSurfaceFormat format;
  format.setDepthBufferSize(24);

  //format.setStencilBufferSize(8);
  format.setSamples(6);
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication app(argc, argv);
  qApp->setStyle(QStyleFactory::create("Fusion"));
  // increase font size for better reading
  QFont defaultFont = QApplication::font();
  defaultFont.setPointSize(defaultFont.pointSize()+1);
  qApp->setFont(defaultFont);
  // modify palette to dark
  QPalette darkPalette;
  darkPalette.setColor(QPalette::Window,QColor(53,53,53));
  darkPalette.setColor(QPalette::WindowText,Qt::white);
  darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
  darkPalette.setColor(QPalette::Base,QColor(42,42,42));
  darkPalette.setColor(QPalette::AlternateBase,QColor(66,66,66));
  darkPalette.setColor(QPalette::ToolTipBase,Qt::white);
  darkPalette.setColor(QPalette::ToolTipText,Qt::white);
  darkPalette.setColor(QPalette::Text,Qt::white);
  darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
  darkPalette.setColor(QPalette::Dark,QColor(35,35,35));
  darkPalette.setColor(QPalette::Shadow,QColor(20,20,20));
  darkPalette.setColor(QPalette::Button,QColor(53,53,53));
  darkPalette.setColor(QPalette::ButtonText,Qt::white);
  darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
  darkPalette.setColor(QPalette::BrightText,Qt::red);
  darkPalette.setColor(QPalette::Link,QColor(42,130,218));
  darkPalette.setColor(QPalette::Highlight,QColor(42,130,218));
  darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
  darkPalette.setColor(QPalette::HighlightedText,Qt::white);
  darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));

  qApp->setPalette(darkPalette);
  QFontDatabase::addApplicationFont(":/fonts/Hack-Regular.ttf");
  QFont font = QFont("Hack-Regular", 11, 4);
  QIcon icon("://icons8-molecule-40.png");
  app.setWindowIcon(icon);
  app.setFont(font);

  main_window w;
  w.showMaximized();
  int ret_code = app.exec();

  app_state_t::get_inst()->save_settings();

  return ret_code;

}
