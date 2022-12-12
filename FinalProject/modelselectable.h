#pragma once

#include "model.h"

#include <qwidget.h>

class ModelSelectable : public QWidget
{
    Q_OBJECT

private:
    Model* _model = nullptr;

public:
    ModelSelectable(QWidget* parent = 0);
    ~ModelSelectable();
};