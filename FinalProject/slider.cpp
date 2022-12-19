#include <qregularexpression.h>

#include "slider.h"

Slider::Slider(float min, float max, int step, QWidget* parent) :
    QWidget(parent), _min(min), _max(max), _step(step)
{
    // Set map functions
    _transformFunc = [this](float x) {
        // Map x of [0, step] to [_min, _max]
        float y = x / _step * (_max - _min) + _min;
        return y;
    };
    _inversionFunc = [this](float y) {
        // Map y of [_min, _max] to [0, step]
        float x = (y - _min) / (_max - _min) * _step;
        return x;
    };
    // Generate colors
    generateColor(_defaultSchemeColor);
    // Create main layout
    _mainLayout = new QHBoxLayout(this);
    _mainLayout->setContentsMargins(4, 4, 4, 4);
    _mainLayout->setSpacing(0);
    setLayout(_mainLayout);
    // Create slider
    _slider = new QSlider(Qt::Horizontal, this);
    _slider->setMinimum(0);
    _slider->setMaximum(_step);
    _slider->setSingleStep(1);
    // Set slider style sheet
    QString grooveStyle = "QSlider::groove:horizontal {"
        "height:6px;"
        "border-radius:3px;"
        "}";
    QString sliderStyle = "QSlider::handle:horizontal {"
        "width:12px;"
        "margin-bottom:-3px;"
        "margin-top:-3px;"
        "background:" + _handleColor.name(QColor::HexArgb) + ";"
        "border-radius:6px;"
        "}";
    QString sliderHoverStyle = "QSlider::handle:horizontal:hover {"
        "width:12px;"
        "margin-bottom:-3px;"
        "margin-top:-3px;"
        "background:" + _hoverColor.name(QColor::HexArgb) + ";"
        "border-radius:6px;"
        "}";
    QString sliderPressStyle = "QSlider::handle:horizontal:pressed {"
        "width:12px;"
        "margin-bottom:-3px;"
        "margin-top:-3px;"
        "background:" + _pressColor.name(QColor::HexArgb) + ";"
        "border-radius:6px;"
        "}";
    QString subStyle = "QSlider::sub-page:horizontal {"
        "background:" + _subColor.name(QColor::HexArgb) + ";"
        "border-radius:3px;"
        "}";
    QString addStyle = "QSlider::add-page:horizontal {"
        "background:" + _addColor.name(QColor::HexArgb) + ";"
        "border-radius:3px;"
        "}";
    _slider->setStyleSheet(grooveStyle + sliderStyle + sliderHoverStyle + sliderPressStyle + subStyle + addStyle);
    // Create decrease button
    _decreaseBtn = new PushButton(nullptr, this);
    _decreaseBtn->setColorScheme(_defaultSchemeColor);
    _decreaseBtn->setFixedSize(24, 24);
    _decreaseBtn->setRadius(8);
    _decreaseBtn->setMargin(0, 0, 0, 3);
    _decreaseBtn->setIndicatorColor(QColor(255, 255, 255, 0));
    // Create decrease label
    _decreaseIcon = new QLabel(_decreaseBtn);
    _decreaseIcon->setFont(QFont("Font Awesome 6 Free Solid", 6));
    _decreaseIcon->setText("\uf068");
    _decreaseIcon->setAlignment(Qt::AlignCenter);
    _decreaseBtn->setChildWidget(_decreaseIcon);
    _decreaseIcon->show();
    // Create increase button
    _increaseBtn = new PushButton(nullptr, this);
    _increaseBtn->setColorScheme(_defaultSchemeColor);
    _increaseBtn->setFixedSize(24, 24);
    _increaseBtn->setRadius(8);
    _increaseBtn->setMargin(0, 0, 0, 3);
    _increaseBtn->setIndicatorColor(QColor(255, 255, 255, 0));
    // Create increase label
    _increaseIcon = new QLabel(_increaseBtn);
    _increaseIcon->setFont(QFont("Font Awesome 6 Free Solid", 6));
    _increaseIcon->setText("\uf067");
    _increaseIcon->setAlignment(Qt::AlignCenter);
    _increaseBtn->setChildWidget(_increaseIcon);
    _increaseIcon->show();
    // Add to main layout
    _mainLayout->addWidget(_decreaseBtn);
    _mainLayout->addSpacing(4);
    _mainLayout->addWidget(_slider);
    _mainLayout->addSpacing(4);
    _mainLayout->addWidget(_increaseBtn);
    _decreaseBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _increaseBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _decreaseBtn->show();
    _slider->show();
    _increaseBtn->show();
    // Connect signals and slots
    connect(_decreaseBtn, &PushButton::onClick, this, [this]() {
        // Set current value
        _slider->setValue(_slider->value() - 1);
        emit onChanged(_transformFunc(_slider->value()));
        emit onSetValue(_transformFunc(_slider->value()));
        emit onDragEnd();
    });
    connect(_increaseBtn, &PushButton::onClick, this, [this]() {
        // Set current value
        _slider->setValue(_slider->value() + 1);
        emit onChanged(_transformFunc(_slider->value()));
        emit onSetValue(_transformFunc(_slider->value()));
        emit onDragEnd();
    });
    connect(_slider, &QSlider::valueChanged, this, [this](int value) {
        // Judge whether the slider is changed by dragging or function
        if (_slider->isSliderDown()) {
            // Value changed by user
            emit onChanged(_transformFunc(value));
        }
        emit onSetValue(_transformFunc(value));
    });
    connect(_slider, &QSlider::sliderPressed, this, &Slider::onDragStart);
    connect(_slider, &QSlider::sliderReleased, this, &Slider::onDragEnd);
}

Slider::~Slider()
{}

void Slider::generateColor(QColor schemeColor) {
    _subColor = schemeColor;
    _addColor = QColor(216, 216, 216);
    _handleColor = QColor(194, 194, 194);
    _hoverColor = schemeColor.lighter(20);
    float hoverBlendRatio = 0.2;
    _hoverColor = QColor(
        _hoverColor.red() * hoverBlendRatio + _handleColor.red() * (1 - hoverBlendRatio),
        _hoverColor.green() * hoverBlendRatio + _handleColor.green() * (1 - hoverBlendRatio),
        _hoverColor.blue() * hoverBlendRatio + _handleColor.blue() * (1 - hoverBlendRatio)
    );
    _pressColor = schemeColor.lighter(20);
    float pressBlendRatio = 0.5;
    _pressColor = QColor(
        _pressColor.red() * pressBlendRatio + _handleColor.red() * (1 - pressBlendRatio),
        _pressColor.green() * pressBlendRatio + _handleColor.green() * (1 - pressBlendRatio),
        _pressColor.blue() * pressBlendRatio + _handleColor.blue() * (1 - pressBlendRatio)
    );
}

void Slider::setColorScheme(QColor color) {
    generateColor(color);
    // Change style sheet
    QString grooveStyle = "QSlider::groove:horizontal {"
        "height:6px;"
        "border-radius:3px;"
        "}";
    QString sliderStyle = "QSlider::handle:horizontal {"
        "width:12px;"
        "margin-bottom:-3px;"
        "margin-top:-3px;"
        "background:" + _handleColor.name(QColor::HexArgb) + ";"
        "border-radius:6px;"
        "}";
    QString sliderHoverStyle = "QSlider::handle:horizontal:hover {"
        "width:12px;"
        "margin-bottom:-3px;"
        "margin-top:-3px;"
        "background:" + _hoverColor.name(QColor::HexArgb) + ";"
        "border-radius:6px;"
        "}";
    QString sliderPressStyle = "QSlider::handle:horizontal:pressed {"
        "width:12px;"
        "margin-bottom:-3px;"
        "margin-top:-3px;"
        "background:" + _pressColor.name(QColor::HexArgb) + ";"
        "border-radius:6px;"
        "}";
    QString subStyle = "QSlider::sub-page:horizontal {"
        "background:" + _subColor.name(QColor::HexArgb) + ";"
        "border-radius:3px;"
        "}";
    QString addStyle = "QSlider::add-page:horizontal {"
        "background:" + _addColor.name(QColor::HexArgb) + ";"
        "border-radius:3px;"
        "}";
    _slider->setStyleSheet(grooveStyle + sliderStyle + sliderHoverStyle + sliderPressStyle + subStyle + addStyle);
    // Change button color
    _decreaseBtn->setColorScheme(color);
    _decreaseBtn->setIndicatorColor(QColor(255, 255, 255, 0));
    _increaseBtn->setColorScheme(color);
    _increaseBtn->setIndicatorColor(QColor(255, 255, 255, 0));
}

void Slider::setMin(float min) {
    _min = min;
}

void Slider::setMax(float max) {
    _max = max;
}

void Slider::setStep(float step) {
    _step = step;
    _slider->setMaximum(step);
}

void Slider::setValue(float value) {
    _slider->setValue((int)_inversionFunc(value));
}

void Slider::setTransformation(std::function<float(float)> transformFunc, std::function<float(float)> inversionFunc) {
    _transformFunc = transformFunc;
    _inversionFunc = inversionFunc;
}

void Slider::setEnabled(bool enabled) {
    if (enabled == _enabled) {
        return;
    }
    _enabled = enabled;
    _slider->setEnabled(enabled);
    _decreaseBtn->setEnabled(enabled);
    _increaseBtn->setEnabled(enabled);
    if (!enabled) {
        // Store colors
        _restoredColor[0] = _subColor;
        _restoredColor[1] = _addColor;
        _restoredColor[2] = _handleColor;
        _restoredColor[3] = _hoverColor;
        _restoredColor[4] = _pressColor;
        // Change colors
        setColorScheme(QColor(200, 200, 200));
    }
    else {
        // Restore colors
        _subColor = _restoredColor[0];
        _addColor = _restoredColor[1];
        _handleColor = _restoredColor[2];
        _hoverColor = _restoredColor[3];
        _pressColor = _restoredColor[4];
        // Change style sheet
        QString grooveStyle = "QSlider::groove:horizontal {"
            "height:6px;"
            "border-radius:3px;"
            "}";
        QString sliderStyle = "QSlider::handle:horizontal {"
            "width:12px;"
            "margin-bottom:-3px;"
            "margin-top:-3px;"
            "background:" + _handleColor.name(QColor::HexArgb) + ";"
            "border-radius:6px;"
            "}";
        QString sliderHoverStyle = "QSlider::handle:horizontal:hover {"
            "width:12px;"
            "margin-bottom:-3px;"
            "margin-top:-3px;"
            "background:" + _hoverColor.name(QColor::HexArgb) + ";"
            "border-radius:6px;"
            "}";
        QString sliderPressStyle = "QSlider::handle:horizontal:pressed {"
            "width:12px;"
            "margin-bottom:-3px;"
            "margin-top:-3px;"
            "background:" + _pressColor.name(QColor::HexArgb) + ";"
            "border-radius:6px;"
            "}";
        QString subStyle = "QSlider::sub-page:horizontal {"
            "background:" + _subColor.name(QColor::HexArgb) + ";"
            "border-radius:3px;"
            "}";
        QString addStyle = "QSlider::add-page:horizontal {"
            "background:" + _addColor.name(QColor::HexArgb) + ";"
            "border-radius:3px;"
            "}";
        _slider->setStyleSheet(grooveStyle + sliderStyle + sliderHoverStyle + sliderPressStyle + subStyle + addStyle);
        // Change button color
        _decreaseBtn->setColorScheme(_subColor);
        _decreaseBtn->setIndicatorColor(QColor(255, 255, 255, 0));
        _increaseBtn->setColorScheme(_subColor);
        _increaseBtn->setIndicatorColor(QColor(255, 255, 255, 0));
    }
}
