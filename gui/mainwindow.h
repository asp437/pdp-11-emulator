#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QGraphicsPixmapItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void renderScreen(std::vector<std::vector<int> > screen_buffer);
private:
  Ui::MainWindow *ui;
  QGraphicsPixmapItem *screen_graphics_item;
};

#endif // MAINWINDOW_H
