#pragma once

#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>

#include "model.h"
#include "pushbutton.h"
#include "roundedcornerwidget.h"
#include "modelthumbnailwidget.h"
#include "lineeditwidget.h"

class ModelSelectable : public QWidget {
    
    Q_OBJECT

public:
    ModelSelectable(Model* model, QWidget* parent = 0);
    ~ModelSelectable();

private:
    // Data
    Model* _model = nullptr;
    
    // UI control variables
    const QColor _borderColor = QColor(58, 143, 183);
    const int _cornerRadius = 10;

    // UI elements
    QHBoxLayout* _stretchLayout = nullptr;
    PushButton* _mainBtn = nullptr;
    
    QWidget* _mainBtnChild = nullptr;
    QHBoxLayout* _mainLayout = nullptr;
    
    RoundedCornerWidget* _thumbnailContainer = nullptr;
    QHBoxLayout* _thumbnailContainerLayout = nullptr;
    ModelThumbnailWidget* _thumbnailWidget = nullptr;

    PushButton* _deleteButton = nullptr;
    QLabel* _deleteIcon = nullptr;
    
signals:
    void onSelected();
    void onRemoved();
};
