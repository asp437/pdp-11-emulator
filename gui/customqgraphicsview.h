//
// Created by Aleksandr Parfenov on 19.10.16.
//

#ifndef PDP_11_EMULATOR_CUSTOMQGRAPHICSVIEW_H
#define PDP_11_EMULATOR_CUSTOMQGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

class PDPMachine;

class CustomQGraphicsView : public QGraphicsView {
Q_OBJECT
public:
    CustomQGraphicsView(QWidget *parent = 0);
    void set_pdp_machine(PDPMachine *pdp_machine) { _pdp_machine = pdp_machine; }
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    PDPMachine *_pdp_machine;
};
#endif //PDP_11_EMULATOR_CUSTOMQGRAPHICSVIEW_H
