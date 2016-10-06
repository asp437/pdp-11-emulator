#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "../core/pdp_machine.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  _ui(new Ui::MainWindow) {
  _ui->setupUi(this);


  QGraphicsScene *scene = new QGraphicsScene();
  _ui->display_view->setScene(scene);
  _screen_graphics_item = new QGraphicsPixmapItem();
  _ui->display_view->scene()->addItem(_screen_graphics_item);
  _ui->display_view->show();

  _clock_timer = new QTimer();
  _display_timer = new QTimer();
  _clock_timer->setInterval(0);
  _display_timer->setInterval(1000.0 / 20.0);
  connect(_clock_timer, SIGNAL(timeout()), this, SLOT(clock_update()));
  connect(_display_timer, SIGNAL(timeout()), this, SLOT(display_update()));
  // _clock_timer->start();
  _display_timer->start();

  CPUStateTableView *cpu_state_view = new CPUStateTableView();
  cpu_state_view->addObject(std::make_pair("R0", 0));
  cpu_state_view->addObject(std::make_pair("R1", 1));
  cpu_state_view->addObject(std::make_pair("R2", 2));
  cpu_state_view->addObject(std::make_pair("R3", 3));
  cpu_state_view->addObject(std::make_pair("R4", 4));
  cpu_state_view->addObject(std::make_pair("R5", 5));
  cpu_state_view->addObject(std::make_pair("R6/SP", 1));
  cpu_state_view->addObject(std::make_pair("R7/PC", 2));
  cpu_state_view->addObject(std::make_pair("PSW", 3));
  cpu_state_view->addObject(std::make_pair("Int Pri", 3));
  cpu_state_view->addObject(std::make_pair("N", 1));
  cpu_state_view->addObject(std::make_pair("V", 1));
  cpu_state_view->addObject(std::make_pair("Z", 1));
  cpu_state_view->addObject(std::make_pair("C", 1));

  _ui->cpu_state_table->setModel(cpu_state_view);
  _ui->cpu_state_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);


  DisasmTableView *disasm_view = new DisasmTableView();
  disasm_view->addObject(std::make_pair(0140000, "CLR R0"));
  disasm_view->addObject(std::make_pair(0140002, "CLR R1"));
  disasm_view->addObject(std::make_pair(0140004, "CLR R2"));
  disasm_view->addObject(std::make_pair(0140006, "CLR R3"));
  disasm_view->addObject(std::make_pair(0140010, "MOV (01000), R4"));
  disasm_view->addObject(std::make_pair(0140012, "MOV (01002), R5"));
  disasm_view->addObject(std::make_pair(0140014, "ADD R4, R3"));
  disasm_view->addObject(std::make_pair(0140016, "ADD R5, R3"));
  _ui->disasm_table->setModel(disasm_view);
  _ui->disasm_table->setColumnWidth(1, 300);
  _ui->disasm_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  _ui->disasm_table->horizontalHeader()->setSectionsClickable(false);
  disasm_view->select_row(3);


  MemoryExplorerTableView *memory_model = new MemoryExplorerTableView();
  std::vector<uint16_t> data;
  data.push_back(0000000);
  data.push_back(0000002);
  data.push_back(0000004);
  data.push_back(0000006);
  data.push_back(0000010);
  data.push_back(0000012);
  data.push_back(0000014);

  memory_model->setObjects(data);
  _ui->memory_explorer_table->setModel(memory_model);
  _ui->memory_explorer_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  _ui->memory_explorer_table->horizontalHeader()->setSectionsClickable(false);
}

MainWindow::~MainWindow() {
  delete _screen_graphics_item;
  delete _clock_timer;
  delete _display_timer;
  delete _ui;
}

void MainWindow::render_display(std::vector<std::vector<int> > &buffer) {
  QImage image((int)buffer.size(), (int)buffer[0].size(), QImage::Format_RGB888);
  int width = buffer.size(), height = buffer[0].size();
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      image.setPixel(x, y, buffer[x][y]);
    }
  }
  _screen_graphics_item->setPixmap(QPixmap::fromImage(image));
  _ui->display_view->update();
}

void MainWindow::render_display(PDPDisplayAdapter *display_adapter) {
  int width = display_adapter->get_width(), height = display_adapter->get_height();
  QImage image(width, height, QImage::Format_RGB888);

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      image.setPixel(x, y, 0xFF000000 | display_adapter->get_pixel(x, y));
    }
  }
  _screen_graphics_item->setPixmap(QPixmap::fromImage(image));
  _ui->display_view->update();
}

void MainWindow::on_load_rom_button_clicked() {

}

void MainWindow::on_run_button_clicked() {
  _clock_timer->start();
}

void MainWindow::on_step_button_clicked() {
  _clock_timer->stop();
  _pdp_machine->execute_command();
}

void MainWindow::on_reset_button_clicked() {

}

void MainWindow::clock_update() {
  _pdp_machine->execute_command();
  if (_pdp_machine->is_halted()) {
    std::cout << "PDP Machine halted" << std::endl;
    _clock_timer->stop();
  }
}

void MainWindow::display_update() {
  // _pdp_machine->get_video_buffer();
  // render_display(_pdp_machine->get_video_buffer());
  render_display(_pdp_machine->get_display_adapter());
}

void MainWindow::on_jump_to_disasm_edit_returnPressed() {

}

void MainWindow::on_jump_to_mem_edit_returnPressed() {

}
