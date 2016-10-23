#include "core/pdp_machine.h"
#include "gui/mainwindow.h"
#include <QApplication>

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    // TODO: Test this on Windows system. Used in order to accommodate labels and info in window on Linux system
//    QFont font = a.font();
//    font.setPointSize(font.pointSize() - 2);
//    a.setFont(font);

    MainWindow w;
    w.show();
    return a.exec();
}