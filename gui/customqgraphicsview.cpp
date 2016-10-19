//
// Created by Aleksandr Parfenov on 19.10.16.
//

#include "customqgraphicsview.h"
#include "../common.h"
#include <QKeyEvent>
#include "../core/pdp_machine.h"

CustomQGraphicsView::CustomQGraphicsView(QWidget *parent) : QGraphicsView(parent) {
    _pdp_machine = nullptr;
}

void CustomQGraphicsView::keyPressEvent(QKeyEvent *event) {
    if (_pdp_machine != nullptr)
        _pdp_machine->key_pressed((uint) event->key(), true);
}

void CustomQGraphicsView::keyReleaseEvent(QKeyEvent *event) {
    if (_pdp_machine != nullptr)
        _pdp_machine->key_pressed((uint) event->key(), false);
}