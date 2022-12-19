#pragma once

#include <qwidget.h>
#include <qboxlayout.h>
#include <qlabel.h>

#include "renderable.h"
#include "pushbutton.h"
#include "modelattrslide.h"

class ModelSetter : public QWidget
{
    Q_OBJECT

public:
    ModelSetter(QWidget* parent = 0);
    ModelSetter(Renderable* object, QWidget* parent = 0);
    ~ModelSetter();

private:
    // UI Elemenets
    QHBoxLayout* _mainLayout = nullptr;
    
    QWidget* _objectSettingPanel = nullptr;
    QVBoxLayout* _objectSettingLayout = nullptr;

    QWidget* _lightSettingButtons = nullptr;
    QVBoxLayout* _lightSettingsButtonsLayout = nullptr;
    
    QWidget* _lightSettingPanel = nullptr;
    QVBoxLayout* _lightSettingLayout = nullptr;
    
    QWidget* _lightColorSettingPanel = nullptr;
    QVBoxLayout* _lightColorSettingPanelLayout = nullptr;

    ModelAttributeSlide* _scale = nullptr;
    ModelAttributeSlide* _rotateX = nullptr;
    ModelAttributeSlide* _rotateY = nullptr;
    ModelAttributeSlide* _rotateZ = nullptr;
    ModelAttributeSlide* _lightDistance = nullptr;
    ModelAttributeSlide* _lightRotateTheta = nullptr;
    ModelAttributeSlide* _lightRotatePhi = nullptr;
    ModelAttributeSlide* _lightCutoffAngle = nullptr;
    ModelAttributeSlide* _lightR = nullptr;
    ModelAttributeSlide* _lightG = nullptr;
    ModelAttributeSlide* _lightB = nullptr;
    QWidget* _lightSwitchPanel = nullptr;
    QVBoxLayout* _lightSwitchLayout = nullptr;
    PushButton* _lightSwitch = nullptr;
    QLabel* _lightSwitchIcon = nullptr;
    PushButton* _lightColorPanel = nullptr;
    QLabel* _lightColorPanelIcon = nullptr;
    PushButton* _deleteBtn = nullptr;
    QLabel* _deleteIcon = nullptr;

    // State
    Renderable* _object = nullptr;
    bool _colorPaletteOn = false;

private:
    void setRotate();
    void setLightDir();
    void setLightColor();

public:
    void update(Renderable* object);

signals:
    void onAdjustStart();
    void onAdjustEnd();
    void onAdjust();
    void onDeleteObject();
};
