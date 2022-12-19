#pragma once

#include <qwidget.h>
#include <qboxlayout.h>
#include <qlabel.h>

#include "pagewidget.h"
#include "sceneviewer.h"
#include "roundedcornerwidget.h"
#include "modelselector.h"
#include "modelsetter.h"

class EditorPage : public PageWidget {
    
    Q_OBJECT

public:
    EditorPage(QWidget* parent = 0);
    ~EditorPage();

private:
    // Push button icons
    QLabel* _iconButtonLabel = nullptr;
    QWidget* _textButtonWidget = nullptr;
    QHBoxLayout* _textButtonLayout = nullptr;
    QLabel* _textButtonIcon = nullptr;
    QLabel* _textButtonLabel = nullptr;
    
    // UI elements
    QVBoxLayout* _titleLayout = nullptr;
    const QFont _titleFont = QFont("DengXian", 26, QFont::ExtraLight);
    QLabel* _titleLabel = nullptr;
    
    QWidget* _mainWidget = nullptr;
    QHBoxLayout* _mainLayout = nullptr;

    ModelSelector* _modelSelector = nullptr;

    QVBoxLayout* _editingLayout = nullptr;

    RoundedCornerWidget* _sceneViewerContainer = nullptr;
    QVBoxLayout* _sceneViewerContainerLayout = nullptr;
    SceneViewer* _sceneViewer = nullptr;

    ModelSetter* _modelSetter = nullptr;

public:
    virtual PushButton* getPageIconButton(QWidget* context) override;
    virtual PushButton* getPageTextButton(QWidget* context) override;
};
