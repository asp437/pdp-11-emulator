#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "cpustatetableview.h"
#include <QGraphicsPixmapItem>
#include "disasmtableview.h"
#include "memoryexplorertableview.h"
#include "../core/io_devices/PDPDisplayAdapter.h"

class PDPMachine;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void set_pdp_machine(PDPMachine *pdp_machine);
    void render_display(std::vector<std::vector<int> > &buffer);
    void render_display(PDPDisplayAdapter *display_adapter);
public slots:
    void on_load_rom_button_clicked();
    void on_run_button_clicked();
    void on_step_button_clicked();
    void on_reset_button_clicked();

    void clock_update();
    void display_update();

    void on_jump_to_disasm_edit_returnPressed();
    void on_jump_to_mem_edit_returnPressed();

private:
    void update_cpu_registers_view();

    QGraphicsPixmapItem *_screen_graphics_item;
    DisasmTableView *_diasm_table_view;
    Ui::MainWindow *_ui;
    QTimer *_clock_timer;
    QTimer *_display_timer;
    PDPMachine *_pdp_machine;
};

#endif // MAINWINDOW_H
