/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include "customqgraphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow {
public:
    QWidget *centralWidget;
    CustomQGraphicsView *display_view;
    QGroupBox *cpu_state_box;
    QTableView *cpu_state_table;
    QGroupBox *disasm_box;
    QTableView *disasm_table;
    QLabel *label_jump_to_disasm;
    QLineEdit *jump_to_disasm_edit;
    QGroupBox *mem_explorer_box;
    QLineEdit *jump_to_mem_edit;
    QLabel *label_jump_to_mem;
    QTableView *memory_explorer_table;
    QPushButton *load_rom_button;
    QPushButton *run_button;
    QPushButton *step_button;
    QPushButton *reset_button;

    void setupUi(QMainWindow *MainWindow) {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1310, 670);
        MainWindow->setMinimumSize(QSize(1310, 670));
        MainWindow->setMaximumSize(QSize(1310, 670));
        MainWindow->setLayoutDirection(Qt::RightToLeft);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        display_view = new CustomQGraphicsView(centralWidget);
        display_view->setObjectName(QStringLiteral("display_view"));
        display_view->setGeometry(QRect(0, 0, 810, 610));
        cpu_state_box = new QGroupBox(centralWidget);
        cpu_state_box->setObjectName(QStringLiteral("cpu_state_box"));
        cpu_state_box->setGeometry(QRect(815, 0, 140, 440));
        cpu_state_table = new QTableView(cpu_state_box);
        cpu_state_table->setObjectName(QStringLiteral("cpu_state_table"));
        cpu_state_table->setGeometry(QRect(0, 30, 140, 410));
        cpu_state_table->setLayoutDirection(Qt::LeftToRight);
        cpu_state_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        cpu_state_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        cpu_state_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        cpu_state_table->setSelectionMode(QAbstractItemView::NoSelection);
        cpu_state_table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        cpu_state_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        cpu_state_table->horizontalHeader()->setVisible(true);
        cpu_state_table->horizontalHeader()->setDefaultSectionSize(65);
        cpu_state_table->horizontalHeader()->setMinimumSectionSize(65);
        cpu_state_table->horizontalHeader()->setStretchLastSection(true);
        cpu_state_table->verticalHeader()->setDefaultSectionSize(25);
        cpu_state_table->verticalHeader()->setMinimumSectionSize(25);
        disasm_box = new QGroupBox(centralWidget);
        disasm_box->setObjectName(QStringLiteral("disasm_box"));
        disasm_box->setGeometry(QRect(965, 0, 340, 440));
        disasm_table = new QTableView(disasm_box);
        disasm_table->setObjectName(QStringLiteral("disasm_table"));
        disasm_table->setGeometry(QRect(0, 45, 340, 395));
        disasm_table->setLayoutDirection(Qt::LeftToRight);
        disasm_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        disasm_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//        disasm_table->setEditTriggers(QAbstractItemView::AllEditTriggers);
        disasm_table->setSelectionMode(QAbstractItemView::SingleSelection);
        disasm_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        disasm_table->horizontalHeader()->setDefaultSectionSize(80);
        disasm_table->horizontalHeader()->setMinimumSectionSize(80);
        disasm_table->verticalHeader()->setDefaultSectionSize(25);
        disasm_table->verticalHeader()->setMinimumSectionSize(25);
        label_jump_to_disasm = new QLabel(disasm_box);
        label_jump_to_disasm->setObjectName(QStringLiteral("label_jump_to_disasm"));
        label_jump_to_disasm->setGeometry(QRect(5, 20, 150, 20));
        jump_to_disasm_edit = new QLineEdit(disasm_box);
        jump_to_disasm_edit->setObjectName(QStringLiteral("jump_to_disasm_edit"));
        jump_to_disasm_edit->setGeometry(QRect(190, 20, 150, 22));
        jump_to_disasm_edit->setInputMethodHints(Qt::ImhNone);
        mem_explorer_box = new QGroupBox(centralWidget);
        mem_explorer_box->setObjectName(QStringLiteral("mem_explorer_box"));
        mem_explorer_box->setGeometry(QRect(900, 445, 410, 210));
        jump_to_mem_edit = new QLineEdit(mem_explorer_box);
        jump_to_mem_edit->setObjectName(QStringLiteral("jump_to_mem_edit"));
        jump_to_mem_edit->setGeometry(QRect(260, 20, 150, 22));
        jump_to_mem_edit->setInputMethodHints(Qt::ImhNone);
        label_jump_to_mem = new QLabel(mem_explorer_box);
        label_jump_to_mem->setObjectName(QStringLiteral("label_jump_to_mem"));
        label_jump_to_mem->setGeometry(QRect(5, 20, 190, 20));
        memory_explorer_table = new QTableView(mem_explorer_box);
        memory_explorer_table->setObjectName(QStringLiteral("memory_explorer_table"));
        memory_explorer_table->setGeometry(QRect(0, 45, 410, 165));
        memory_explorer_table->setLayoutDirection(Qt::LeftToRight);
        memory_explorer_table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        memory_explorer_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        memory_explorer_table->horizontalHeader()->setDefaultSectionSize(80);
        memory_explorer_table->verticalHeader()->setDefaultSectionSize(25);
        memory_explorer_table->verticalHeader()->setMinimumSectionSize(25);
        load_rom_button = new QPushButton(centralWidget);
        load_rom_button->setObjectName(QStringLiteral("load_rom_button"));
        load_rom_button->setGeometry(QRect(10, 615, 93, 51));
        run_button = new QPushButton(centralWidget);
        run_button->setObjectName(QStringLiteral("run_button"));
        run_button->setGeometry(QRect(120, 615, 93, 51));
        step_button = new QPushButton(centralWidget);
        step_button->setObjectName(QStringLiteral("step_button"));
        step_button->setGeometry(QRect(230, 615, 93, 51));
        reset_button = new QPushButton(centralWidget);
        reset_button->setObjectName(QStringLiteral("reset_button"));
        reset_button->setGeometry(QRect(340, 615, 93, 51));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow) {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PDP-11 Simple Emulator", 0));
        cpu_state_box->setTitle(QApplication::translate("MainWindow", "CPU State", 0));
        disasm_box->setTitle(QApplication::translate("MainWindow", "DisAsm", 0));
        label_jump_to_disasm->setText(QApplication::translate("MainWindow", "Jump to code address", 0));
        jump_to_disasm_edit->setInputMask(QApplication::translate("MainWindow", "B99999", 0));
        jump_to_disasm_edit->setText(QApplication::translate("MainWindow", "000000", 0));
        mem_explorer_box->setTitle(QApplication::translate("MainWindow", "Memory Explorer", 0));
        jump_to_mem_edit->setInputMask(QApplication::translate("MainWindow", "B99999", 0));
        jump_to_mem_edit->setText(QApplication::translate("MainWindow", "000000", 0));
        label_jump_to_mem->setText(QApplication::translate("MainWindow", "Jump to memory address", 0));
        load_rom_button->setText(QApplication::translate("MainWindow", "Load ROM", 0));
        run_button->setText(QApplication::translate("MainWindow", "Run", 0));
        step_button->setText(QApplication::translate("MainWindow", "Step", 0));
        reset_button->setText(QApplication::translate("MainWindow", "Reset", 0));
    } // retranslateUi

};

namespace Ui {
class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
