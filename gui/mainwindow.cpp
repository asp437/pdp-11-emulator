#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../common.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QGraphicsScene *scene = new QGraphicsScene();
  ui->graphicsView->setScene(scene);
  screen_graphics_item = new QGraphicsPixmapItem();
  ui->graphicsView->scene()->addItem(screen_graphics_item);
  ui->graphicsView->show();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::renderScreen(std::vector<std::vector<int> > screen_buffer) {
  QImage image((int) screen_buffer.size(), (int) screen_buffer[0].size(), QImage::Format_ARGB32);
  image = image.convertToFormat(QImage::Format_RGB16);
  for (int x = 0; x < screen_buffer.size(); x++) {
    for (int y = 0; y < screen_buffer[x].size(); y++) {
      uint8 r, g, b;
//      image.setPixel(x, y, (x+y) % 2 == 0 ? 0xFFFFFFFF : 0x000000);
      image.setPixel(x, y, 0xFF000000 | screen_buffer[x][y]);
    }
  }
  screen_graphics_item->setPixmap(QPixmap::fromImage(image));
  screen_graphics_item->update(0, 0, image.width(), image.height());
  ui->graphicsView->update();
}
