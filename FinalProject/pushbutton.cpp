#include <qanimationgroup.h>
#include <qparallelanimationgroup.h>
#include <qsequentialanimationgroup.h>
#include <qregularexpression.h>

#include "pushbutton.h"

PushButton::PushButton(QWidget* child, QWidget* parent)
    : QWidget(parent), _childWidget(child)
{
    initializeUI();
}

PushButton::~PushButton()
{
}

void PushButton::initializeUI() {
    generateColor(_defaultColorScheme);
    
    // Add margin for the child widget
    _stretchLayout = new QHBoxLayout(this);
    _stretchLayout->setContentsMargins(_contentMargin);
    _stretchLayout->setSpacing(0);
    _stretchLayout->setAlignment(Qt::AlignCenter);
    setLayout(_stretchLayout);
    if (_childWidget) {
        // prevent adding a null widget
        _stretchLayout->addWidget(_childWidget);
        _childWidget->show();
        _childWidgetOriginalGeometry = _childWidget->geometry();
    }
    
    // Initialize background widget
    _backgroundWidget = new QWidget(this);
    _backgroundWidget->resize(size());
    _backgroundWidget->setObjectName("backgroundWidget");
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    _backgroundWidget->lower();
    _backgroundWidget->show();

    // Initialize indicator
    _indicator = new QWidget(this);
    switch (_indicatorPosition) {
        case LUI_BTN_POS_LEFT:
            _indicator->resize(_indicatorWidth, _activatedLengthRatio * height());
            _indicator->move(_indicatorSpacing, (0.5 - _activatedLengthRatio / 2) * height());
            break;
        case LUI_BTN_POS_RIGHT:
            _indicator->resize(_indicatorWidth, _activatedLengthRatio * height());
            _indicator->move(width() - _indicatorWidth - _indicatorSpacing, (0.5 - _activatedLengthRatio / 2) * height());
            break;
        case LUI_BTN_POS_TOP:
            _indicator->resize(_activatedLengthRatio * width(), _indicatorWidth);
            _indicator->move((0.5 - _activatedLengthRatio / 2) * width(), _indicatorSpacing);
            break;
        case LUI_BTN_POS_BOTTOM:
            _indicator->resize(_activatedLengthRatio * width(), _indicatorWidth);
            _indicator->move((0.5 - _activatedLengthRatio / 2) * width(), height() - _indicatorWidth - _indicatorSpacing);
            break;
    }
    _indicator->setObjectName("indicator");
    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number((float)_indicatorWidth / 2) + "px;}");
    _indicatorEffect = new QGraphicsOpacityEffect(_indicator);
    _indicatorEffect->setOpacity(0);
    _indicator->setGraphicsEffect(_indicatorEffect);
    _indicator->show();

    // Enable mouse tracking
    setMouseTracking(true);
}

void PushButton::generateColor(QColor colorScheme) {
    _backgroundColor = colorScheme.lighter(120);
    _backgroundColor.setAlpha(0);
    _hoverColor = colorScheme.lighter(120);
    _hoverColor.setAlpha(40);
    _pressedColor = colorScheme.lighter(120);
    _pressedColor.setAlpha(50);
    _selectedColor = colorScheme.lighter(120);
    _selectedColor.setAlpha(35);
    _indicatorColor = colorScheme;
}

void PushButton::enterEvent(QEnterEvent* event) {
    setCursor(Qt::PointingHandCursor);

    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    
    QParallelAnimationGroup* indicatorEnterAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation* indicatorGrowLength = new QPropertyAnimation(_indicator, "geometry", this);
    QPropertyAnimation* indicatorFadeIn = new QPropertyAnimation(_indicatorEffect, "opacity", this);
    indicatorGrowLength->setDuration(150);
    indicatorGrowLength->setEasingCurve(QEasingCurve::OutQuad);
    indicatorFadeIn->setDuration(100);
    indicatorFadeIn->setEasingCurve(QEasingCurve::OutQuad);
    indicatorGrowLength->setStartValue(_indicator->geometry());
    switch (_indicatorPosition) {
        case LUI_BTN_POS_LEFT:
            indicatorGrowLength->setEndValue(QRect(
                _indicatorSpacing,
                (0.5 - _hoveredLengthRatio / 2) * height(),
                _indicatorWidth,
                _hoveredLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_RIGHT:
            indicatorGrowLength->setEndValue(QRect(
                width() - _indicatorWidth - _indicatorSpacing,
                (0.5 - _hoveredLengthRatio / 2) * height(),
                _indicatorWidth,
                _hoveredLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_TOP:
            indicatorGrowLength->setEndValue(QRect(
                (0.5 - _hoveredLengthRatio / 2) * width(),
                _indicatorSpacing,
                _hoveredLengthRatio * width(),
                _indicatorWidth
            ));
            break;
        case LUI_BTN_POS_BOTTOM:
            indicatorGrowLength->setEndValue(QRect(
                (0.5 - _hoveredLengthRatio / 2) * width(),
                height() - _indicatorWidth - _indicatorSpacing,
                _hoveredLengthRatio * width(),
                _indicatorWidth
            ));
            break;
    }
    indicatorFadeIn->setStartValue(_indicatorEffect->opacity());
    indicatorFadeIn->setEndValue(0.999);
    indicatorEnterAnimation->addAnimation(indicatorGrowLength);
    indicatorEnterAnimation->addAnimation(indicatorFadeIn);
    indicatorEnterAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    _hovered = true;
    emit onHover();
}

void PushButton::leaveEvent(QEvent* event) {
    setCursor(Qt::ArrowCursor);

    if (_selected) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _selectedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    else {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    
    QParallelAnimationGroup* indicatorLeaveAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation* indicatorShrinkLength = new QPropertyAnimation(_indicator, "geometry", this);
    QPropertyAnimation* indicatorFadeOut = new QPropertyAnimation(_indicatorEffect, "opacity", this);
    indicatorShrinkLength->setDuration(150);
    indicatorShrinkLength->setEasingCurve(QEasingCurve::OutQuad);
    indicatorFadeOut->setDuration(100);
    indicatorFadeOut->setEasingCurve(QEasingCurve::OutQuad);
    indicatorShrinkLength->setStartValue(_indicator->geometry());
    switch (_indicatorPosition) {
        case LUI_BTN_POS_LEFT:
            indicatorShrinkLength->setEndValue(QRect(
                _indicatorSpacing,
                (0.5 - _activatedLengthRatio / 2) * height(),
                _indicatorWidth,
                _activatedLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_RIGHT:
            indicatorShrinkLength->setEndValue(QRect(
                width() - _indicatorWidth - _indicatorSpacing,
                (0.5 - _activatedLengthRatio / 2) * height(),
                _indicatorWidth,
                _activatedLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_TOP:
            indicatorShrinkLength->setEndValue(QRect(
                (0.5 - _activatedLengthRatio / 2) * width(),
                _indicatorSpacing,
                _activatedLengthRatio * width(),
                _indicatorWidth
            ));
            break;
        case LUI_BTN_POS_BOTTOM:
            indicatorShrinkLength->setEndValue(QRect(
                (0.5 - _activatedLengthRatio / 2) * width(),
                height() - _indicatorWidth - _indicatorSpacing,
                _activatedLengthRatio * width(),
                _indicatorWidth
            ));
            break;
    }
    indicatorFadeOut->setStartValue(_indicatorEffect->opacity());
    indicatorFadeOut->setEndValue(_selected ? 0.999 : 0);
    indicatorLeaveAnimation->addAnimation(indicatorShrinkLength);
    indicatorLeaveAnimation->addAnimation(indicatorFadeOut);
    indicatorLeaveAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    _pressed = false;
    _hovered = false;
}

void PushButton::mousePressEvent(QMouseEvent* event) {
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    
    QPropertyAnimation* indicatorShrinkLength = new QPropertyAnimation(_indicator, "geometry", this);
    indicatorShrinkLength->setDuration(100);
    indicatorShrinkLength->setEasingCurve(QEasingCurve::OutQuad);
    indicatorShrinkLength->setStartValue(_indicator->geometry());
    switch (_indicatorPosition) {
        case LUI_BTN_POS_LEFT:
            indicatorShrinkLength->setEndValue(QRect(
                _indicatorSpacing,
                (0.5 - _pressedLengthRatio / 2) * height(),
                _indicatorWidth,
                _pressedLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_RIGHT:
            indicatorShrinkLength->setEndValue(QRect(
                width() - _indicatorWidth - _indicatorSpacing,
                (0.5 - _pressedLengthRatio / 2) * height(),
                _indicatorWidth,
                _pressedLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_TOP:
            indicatorShrinkLength->setEndValue(QRect(
                (0.5 - _pressedLengthRatio / 2) * width(),
                _indicatorSpacing,
                _pressedLengthRatio * width(),
                _indicatorWidth
            ));
            break;
        case LUI_BTN_POS_BOTTOM:
            indicatorShrinkLength->setEndValue(QRect(
                (0.5 - _pressedLengthRatio / 2) * width(),
                height() - _indicatorWidth - _indicatorSpacing,
                _pressedLengthRatio * width(),
                _indicatorWidth
            ));
            break;
    }
    indicatorShrinkLength->start(QAbstractAnimation::DeleteWhenStopped);

    _pressed = true;
    
    emit onPressed();
}

void PushButton::mouseReleaseEvent(QMouseEvent* event) {
    if (!_pressed) {
        return;
    }
    
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");

    QPropertyAnimation* indicatorGrowLength = new QPropertyAnimation(_indicator, "geometry", this);
    indicatorGrowLength->setDuration(100);
    indicatorGrowLength->setEasingCurve(QEasingCurve::OutQuad);
    indicatorGrowLength->setStartValue(_indicator->geometry());
    switch (_indicatorPosition) {
        case LUI_BTN_POS_LEFT:
            indicatorGrowLength->setEndValue(QRect(
                _indicatorSpacing,
                (0.5 - _hoveredLengthRatio / 2) * height(),
                _indicatorWidth,
                _hoveredLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_RIGHT:
            indicatorGrowLength->setEndValue(QRect(
                width() - _indicatorWidth - _indicatorSpacing,
                (0.5 - _hoveredLengthRatio / 2) * height(),
                _indicatorWidth,
                _hoveredLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_TOP:
            indicatorGrowLength->setEndValue(QRect(
                (0.5 - _hoveredLengthRatio / 2) * width(),
                _indicatorSpacing,
                _hoveredLengthRatio * width(),
                _indicatorWidth
            ));
            break;
        case LUI_BTN_POS_BOTTOM:
            indicatorGrowLength->setEndValue(QRect(
                (0.5 - _hoveredLengthRatio / 2) * width(),
                height() - _indicatorWidth - _indicatorSpacing,
                _hoveredLengthRatio * width(),
                _indicatorWidth
            ));
            break;
    }
    indicatorGrowLength->start(QAbstractAnimation::DeleteWhenStopped);

    if (_pressed) {
        // prevent double trigger if mouse is first left and then released
        emit onClick();
    }

    _pressed = false;
}

void PushButton::resizeEvent(QResizeEvent* event) {
    _backgroundWidget->setGeometry(0, 0, width(), height());
    switch (_indicatorPosition) {
        case LUI_BTN_POS_LEFT:
            if (_pressed) {
                _indicator->setGeometry(QRect(
                    _indicatorSpacing,
                    (0.5 - _pressedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _pressedLengthRatio * height()
                ));
            }
            else if (_hovered) {
                _indicator->setGeometry(QRect(
                    _indicatorSpacing,
                    (0.5 - _hoveredLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _hoveredLengthRatio * height()
                ));
            }
            else {
                _indicator->setGeometry(QRect(
                    _indicatorSpacing,
                    (0.5 - _activatedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _activatedLengthRatio * height()
                ));
            }
            break;
        case LUI_BTN_POS_RIGHT:
            if (_pressed) {
                _indicator->setGeometry(QRect(
                    width() - _indicatorWidth - _indicatorSpacing,
                    (0.5 - _pressedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _pressedLengthRatio * height()
                ));
            }
            else if (_hovered) {
                _indicator->setGeometry(QRect(
                    width() - _indicatorWidth - _indicatorSpacing,
                    (0.5 - _hoveredLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _hoveredLengthRatio * height()
                ));
            }
            else {
                _indicator->setGeometry(QRect(
                    width() - _indicatorWidth - _indicatorSpacing,
                    (0.5 - _activatedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _activatedLengthRatio * height()
                ));
            }
            break;
        case LUI_BTN_POS_TOP:
            if (_pressed) {
                _indicator->setGeometry(QRect(
                    (0.5 - _pressedLengthRatio / 2) * width(),
                    _indicatorSpacing,
                    _pressedLengthRatio * width(),
                    _indicatorWidth
                ));
            }
            else if (_hovered) {
                _indicator->setGeometry(QRect(
                    (0.5 - _hoveredLengthRatio / 2) * width(),
                    _indicatorSpacing,
                    _hoveredLengthRatio * width(),
                    _indicatorWidth
                ));
            }
            else {
                _indicator->setGeometry(QRect(
                    (0.5 - _activatedLengthRatio / 2) * width(),
                    _indicatorSpacing,
                    _activatedLengthRatio * width(),
                    _indicatorWidth
                ));
            }
            break;
        case LUI_BTN_POS_BOTTOM:
            if (_pressed) {
                _indicator->setGeometry(QRect(
                    (0.5 - _pressedLengthRatio / 2) * width(),
                    height() - _indicatorWidth - _indicatorSpacing,
                    _pressedLengthRatio * width(),
                    _indicatorWidth
                ));
            }
            else if (_hovered) {
                _indicator->setGeometry(QRect(
                    (0.5 - _hoveredLengthRatio / 2) * width(),
                    height() - _indicatorWidth - _indicatorSpacing,
                    _hoveredLengthRatio * width(),
                    _indicatorWidth
                ));
            }
            else {
                _indicator->setGeometry(QRect(
                    (0.5 - _activatedLengthRatio / 2) * width(),
                    height() - _indicatorWidth - _indicatorSpacing,
                    _activatedLengthRatio * width(),
                    _indicatorWidth
                ));
            }
            break;
    }
}

void PushButton::select() {
    if (_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    else if (_hovered) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    else {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _selectedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    
    // First shrink then length the indicator, also fade in
    QSequentialAnimationGroup* indicatorSelectAnimation = new QSequentialAnimationGroup(this);
    QParallelAnimationGroup* indicatorShrinkAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation* indicatorFadeIn = new QPropertyAnimation(_indicatorEffect, "opacity", this);
    QPropertyAnimation* indicatorShrinkLength = new QPropertyAnimation(_indicator, "geometry", this);
    QPropertyAnimation* indicatorGrowLength = new QPropertyAnimation(_indicator, "geometry", this);
    indicatorFadeIn->setDuration(100);
    indicatorFadeIn->setEasingCurve(QEasingCurve::OutQuad);
    indicatorShrinkLength->setDuration(100);
    indicatorShrinkLength->setEasingCurve(QEasingCurve::OutQuad);
    indicatorGrowLength->setDuration(100);
    indicatorGrowLength->setEasingCurve(QEasingCurve::OutQuad);
    indicatorFadeIn->setStartValue(_indicatorEffect->opacity());
    indicatorFadeIn->setEndValue(0.999);
    indicatorShrinkLength->setStartValue(_indicator->geometry());
    switch (_indicatorPosition) {
        case LUI_BTN_POS_LEFT:
            indicatorShrinkLength->setEndValue(QRect(
                _indicatorSpacing,
                (0.5 - _pressedLengthRatio / 2) * height(),
                _indicatorWidth,
                _pressedLengthRatio * height()
            ));
            indicatorGrowLength->setStartValue(QRect(
                _indicatorSpacing,
                (0.5 - _pressedLengthRatio / 2) * height(),
                _indicatorWidth,
                _pressedLengthRatio * height()
            ));
            indicatorGrowLength->setEndValue(QRect(
                _indicatorSpacing,
                (0.5 - _activatedLengthRatio / 2) * height(),
                _indicatorWidth,
                _activatedLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_RIGHT:
            indicatorShrinkLength->setEndValue(QRect(
                width() - _indicatorWidth - _indicatorSpacing,
                (0.5 - _pressedLengthRatio / 2) * height(),
                _indicatorWidth,
                _pressedLengthRatio * height()
            ));
            indicatorGrowLength->setStartValue(QRect(
                width() - _indicatorWidth - _indicatorSpacing,
                (0.5 - _pressedLengthRatio / 2) * height(),
                _indicatorWidth,
                _pressedLengthRatio * height()
            ));
            indicatorGrowLength->setEndValue(QRect(
                width() - _indicatorWidth - _indicatorSpacing,
                (0.5 - _activatedLengthRatio / 2) * height(),
                _indicatorWidth,
                _activatedLengthRatio * height()
            ));
            break;
        case LUI_BTN_POS_TOP:
            indicatorShrinkLength->setEndValue(QRect(
                (0.5 - _pressedLengthRatio / 2) * width(),
                _indicatorSpacing,
                _pressedLengthRatio * width(),
                _indicatorWidth
            ));
            indicatorGrowLength->setStartValue(QRect(
                (0.5 - _pressedLengthRatio / 2) * width(),
                _indicatorSpacing,
                _pressedLengthRatio * width(),
                _indicatorWidth
            ));
            indicatorGrowLength->setEndValue(QRect(
                (0.5 - _activatedLengthRatio / 2) * width(),
                _indicatorSpacing,
                _activatedLengthRatio * width(),
                _indicatorWidth
            ));
            break;
        case LUI_BTN_POS_BOTTOM:
            indicatorShrinkLength->setEndValue(QRect(
                (0.5 - _pressedLengthRatio / 2) * width(),
                height() - _indicatorWidth - _indicatorSpacing,
                _pressedLengthRatio * width(),
                _indicatorWidth
            ));
            indicatorGrowLength->setStartValue(QRect(
                (0.5 - _pressedLengthRatio / 2) * width(),
                height() - _indicatorWidth - _indicatorSpacing,
                _pressedLengthRatio * width(),
                _indicatorWidth
            ));
            indicatorGrowLength->setEndValue(QRect(
                (0.5 - _activatedLengthRatio / 2) * width(),
                height() - _indicatorWidth - _indicatorSpacing,
                _activatedLengthRatio * width(),
                _indicatorWidth
            ));
            break;
    }
    indicatorShrinkAnimation->addAnimation(indicatorShrinkLength);
    indicatorShrinkAnimation->addAnimation(indicatorFadeIn);
    indicatorSelectAnimation->addAnimation(indicatorShrinkAnimation);
    indicatorSelectAnimation->addAnimation(indicatorGrowLength);
    indicatorSelectAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    
    // Set selected state
    _selected = true;
}

void PushButton::deselect() {
    if (!_selected) {
        return;
    }
    
    if (_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    else if (_hovered) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    else {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");

        // Cursor is currently not in the button, shrink and fade out the indicator
        QParallelAnimationGroup* indicatorDeselectAnimation = new QParallelAnimationGroup(this);
        QPropertyAnimation* indicatorFadeOut = new QPropertyAnimation(_indicatorEffect, "opacity", this);
        QPropertyAnimation* indicatorShrinkLength = new QPropertyAnimation(_indicator, "geometry", this);
        indicatorFadeOut->setDuration(100);
        indicatorFadeOut->setEasingCurve(QEasingCurve::OutQuad);
        indicatorShrinkLength->setDuration(100);
        indicatorShrinkLength->setEasingCurve(QEasingCurve::OutQuad);
        indicatorFadeOut->setStartValue(_indicatorEffect->opacity());
        indicatorFadeOut->setEndValue(0);
        indicatorShrinkLength->setStartValue(_indicator->geometry());
        switch (_indicatorPosition) {
            case LUI_BTN_POS_LEFT:
                indicatorShrinkLength->setEndValue(QRect(
                    _indicatorSpacing,
                    (0.5 - _pressedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _pressedLengthRatio * height()
                ));
                break;
            case LUI_BTN_POS_RIGHT:
                indicatorShrinkLength->setEndValue(QRect(
                    width() - _indicatorWidth - _indicatorSpacing,
                    (0.5 - _pressedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _pressedLengthRatio * height()
                ));
                break;
            case LUI_BTN_POS_TOP:
                indicatorShrinkLength->setEndValue(QRect(
                    (0.5 - _pressedLengthRatio / 2) * width(),
                    _indicatorSpacing,
                    _pressedLengthRatio * width(),
                    _indicatorWidth
                ));
                break;
            case LUI_BTN_POS_BOTTOM:
                indicatorShrinkLength->setEndValue(QRect(
                    (0.5 - _pressedLengthRatio / 2) * width(),
                    height() - _indicatorWidth - _indicatorSpacing,
                    _pressedLengthRatio * width(),
                    _indicatorWidth
                ));
                break;
        }
        indicatorDeselectAnimation->addAnimation(indicatorFadeOut);
        indicatorDeselectAnimation->addAnimation(indicatorShrinkLength);
        indicatorDeselectAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    // Set selected state
    _selected = false;
}

void PushButton::setRadius(int radius) {
    // get current style sheet
    QString styleSheet = _backgroundWidget->styleSheet();
    // remove old border radius
    styleSheet.remove(QRegularExpression("border-radius:\\d+px;"));
    // add new border radius
    styleSheet.append("border-radius: " + QString::number(radius) + "px;");
    // set new style sheet
    _backgroundWidget->setStyleSheet(styleSheet);
}

void PushButton::setBackgroundColor(QColor color) {
    _backgroundColor = color;
    if (!_selected && !_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
}

void PushButton::setHoverColor(QColor color) {
    _hoverColor = color;
    if (_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
}

void PushButton::setPressedColor(QColor color) {
    _pressedColor = color;
    if (_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
}

void PushButton::setSelectedColor(QColor color) {
    _selectedColor = color;
    if (_selected && !_pressed && !_hovered) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _selectedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
}

void PushButton::setIndicatorColor(QColor color) {
    _indicatorColor = color;
    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number((float)_indicatorWidth / 2) + "px;}");
}

void PushButton::setColorScheme(QColor color) {
    generateColor(color);
    if (_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    else if (_hovered) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    else if (_selected) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _selectedColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    else {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";border-radius:" + QString::number(_radius) + "px;}");
    }
    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number((float)_indicatorWidth / 2) + "px;}");
}

QHBoxLayout* PushButton::mainLayout() const {
    return _stretchLayout;
}

void PushButton::setAlignment(Qt::Alignment alignment) {
    _stretchLayout->setAlignment(alignment);
}

void PushButton::setMargin(QMargins margin) {
    _stretchLayout->setContentsMargins(margin);
}

void PushButton::setMargin(int left, int top, int right, int bottom) {
    _stretchLayout->setContentsMargins(left, top, right, bottom);
}

void PushButton::setIndicatorPosition(LUI_BTN_INDICATOR_POS pos) {
    _indicatorPosition = pos;
    switch (pos) {
        case LUI_BTN_POS_LEFT:
            if (_pressed) {
                _indicator->setGeometry(
                    _indicatorSpacing,
                    (0.5 - _pressedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _pressedLengthRatio * height()
                );
            }
            else if (_hovered) {
                _indicator->setGeometry(
                    _indicatorSpacing,
                    (0.5 - _hoveredLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _hoveredLengthRatio * height()
                );
            }
            else {
                _indicator->setGeometry(
                    _indicatorSpacing,
                    (0.5 - _activatedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _activatedLengthRatio * height()
                );
            }
            break;
        case LUI_BTN_POS_RIGHT:
            if (_pressed) {
                _indicator->setGeometry(
                    width() - _indicatorWidth - _indicatorSpacing,
                    (0.5 - _pressedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _pressedLengthRatio * height()
                );
            }
            else if (_hovered) {
                _indicator->setGeometry(
                    width() - _indicatorWidth - _indicatorSpacing,
                    (0.5 - _hoveredLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _hoveredLengthRatio * height()
                );
            }
            else {
                _indicator->setGeometry(
                    width() - _indicatorWidth - _indicatorSpacing,
                    (0.5 - _activatedLengthRatio / 2) * height(),
                    _indicatorWidth,
                    _activatedLengthRatio * height()
                );
            }
            break;
        case LUI_BTN_POS_TOP:
            if (_pressed) {
                _indicator->setGeometry(
                    (0.5 - _pressedLengthRatio / 2) * width(),
                    _indicatorSpacing,
                    _pressedLengthRatio * width(),
                    _indicatorWidth
                );
            }
            else if (_hovered) {
                _indicator->setGeometry(
                    (0.5 - _hoveredLengthRatio / 2) * width(),
                    _indicatorSpacing,
                    _hoveredLengthRatio * width(),
                    _indicatorWidth
                );
            }
            else {
                _indicator->setGeometry(
                    (0.5 - _activatedLengthRatio / 2) * width(),
                    _indicatorSpacing,
                    _activatedLengthRatio * width(),
                    _indicatorWidth
                );
            }
            break;
        case LUI_BTN_POS_BOTTOM:
            if (_pressed) {
                _indicator->setGeometry(
                    (0.5 - _pressedLengthRatio / 2) * width(),
                    height() - _indicatorWidth - _indicatorSpacing,
                    _pressedLengthRatio * width(),
                    _indicatorWidth
                );
            }
            else if (_hovered) {
                _indicator->setGeometry(
                    (0.5 - _hoveredLengthRatio / 2) * width(),
                    height() - _indicatorWidth - _indicatorSpacing,
                    _hoveredLengthRatio * width(),
                    _indicatorWidth
                );
            }
            else {
                _indicator->setGeometry(
                    (0.5 - _activatedLengthRatio / 2) * width(),
                    height() - _indicatorWidth - _indicatorSpacing,
                    _activatedLengthRatio * width(),
                    _indicatorWidth
                );
            }
            break;
    }
}

QWidget* PushButton::childWidget() {
    return _childWidget;
}

void PushButton::setChildWidget(QWidget* widget) {
    _childWidget = widget;
    for (int i = 0; i < _stretchLayout->count(); i++) {
        _stretchLayout->removeItem(_stretchLayout->itemAt(i));
    }
    if (_childWidget != nullptr) {
        _stretchLayout->addWidget(_childWidget);
        _childWidget->show();
        _childWidgetOriginalGeometry = _childWidget->geometry();
    }
}

bool PushButton::isSelected() const {
    return _selected;
}
