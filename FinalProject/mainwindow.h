#pragma once

#include "framelesswindow.h"

class MainWindow : public FramelessWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
