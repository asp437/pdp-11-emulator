#include "core/pdp_machine.h"
#include "gui/mainwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}