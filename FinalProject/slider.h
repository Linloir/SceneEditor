#pragma once

#include <qwidget.h>
#include <qboxlayout.h>
#include <qslider.h>
#include <qlabel.h>

#include "pushbutton.h"

class Slider : public QWidget
{
    Q_OBJECT
        
public:
    Slider(float min, float max, int step, QWidget* parent = 0);
    ~Slider();

private:
    // Settings
    float _min;
    float _max;
    int _step;    // step count of slider value

    // State
    bool _enabled = true;

    // UI Settings
    const QColor _defaultSchemeColor = QColor(58, 143, 183);
    QColor _subColor;
    QColor _addColor;
    QColor _handleColor;
    QColor _hoverColor;
    QColor _pressColor;
    QColor _restoredColor[5];

    // UI Layout
    QHBoxLayout* _mainLayout;
    QSlider* _slider;
    PushButton* _decreaseBtn;
    QLabel* _decreaseIcon;
    PushButton* _increaseBtn;
    QLabel* _increaseIcon;

    // Transformation function
    std::function<float(float)> _transformFunc; // Transform the slider value to actual value
    std::function<float(float)> _inversionFunc; // Transform the actual value to slider value

private:
    void generateColor(QColor schemeColor);

public:
    // Getter APIs
    QHBoxLayout* mainLayout() const { return _mainLayout; }
    float val() const { return _transformFunc(_slider->value()); }
    float lev() const { return _slider->value(); }

    // Setter APIs
    void setColorScheme(QColor color);
    void setMin(float min);
    void setMax(float max);
    void setStep(float max);
    void setValue(float val);   // Set the actual value
    void setTransformation(std::function<float(float)> transform, std::function<float(float)> inverse);

    void setEnabled(bool enabled = true);
    
signals:
    void onChanged(float newVal);   // Triggers only when user changes the value
    void onSetValue(float newVal);  // Triggers when user changes the value or setValue() is called
    void onDragStart();
    void onDragEnd();
};
