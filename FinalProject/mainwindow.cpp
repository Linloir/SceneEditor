#include <qpainterpath.h>
#include <qscreen.h>
#include <qlabel.h>

#include "mainwindow.h"
#include "pushbutton.h"
#include "logger.h"

#define MAX_MOUSE_MOVEMENT 300

MainWindow::MainWindow(QWidget *parent)
    : FramelessWidget(20, parent)
{
    PushButton* btn = new PushButton(new QLabel("Button"), this);
    btn->setFixedSize(100, 100);
    btn->move(100, 100);
    btn->show();
    connect(btn, &PushButton::onClick, this, [=]() {
        if (btn->isSelected()) {
            btn->deselect();
        }
        else {
            btn->select();
        }
    });
}

MainWindow::~MainWindow() {
}