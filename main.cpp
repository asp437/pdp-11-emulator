#include "core/pdp_machine.h"
#include "gui/mainwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
  QApplication a(argc, argv);
  MainWindow w;
  w.set_pdp_machine(new PDPMachine("new_video_mode_demo.bin"));
  w.show();
  return a.exec();
}