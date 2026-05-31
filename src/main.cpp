#include <QtLogging>
#include <QtTranslation>
#include <QtWidgets>

#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
  qInfo("Hello World!");

  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}