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
    QHBoxLayout* _mainLayout;
    
    QWidget* _objectSettingPanel;
    QVBoxLayout* _objectSettingLayout;

    QWidget* _lightSettingButtons;
    QVBoxLayout* _lightSettingsButtonsLayout;
    
    QWidget* _lightSettingPanel;
    QVBoxLayout* _lightSettingLayout;
    
    QWidget* _lightColorSettingPanel;
    QVBoxLayout* _lightColorSettingPanelLayout;

    ModelAttributeSlide* _scale;
    ModelAttributeSlide* _rotateX;
    ModelAttributeSlide* _rotateY;
    ModelAttributeSlide* _rotateZ;
    ModelAttributeSlide* _lightDistance;
    ModelAttributeSlide* _lightRotateTheta;
    ModelAttributeSlide* _lightRotatePhi;
    ModelAttributeSlide* _lightCutoffAngle;
    ModelAttributeSlide* _lightR;
    ModelAttributeSlide* _lightG;
    ModelAttributeSlide* _lightB;
    QWidget* _lightSwitchPanel;
    QVBoxLayout* _lightSwitchLayout;
    PushButton* _lightSwitch;
    QLabel* _lightSwitchIcon;
    PushButton* _lightColorPanel;
    QLabel* _lightColorPanelIcon;
    PushButton* _deleteBtn;
    QLabel* _deleteIcon;

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
