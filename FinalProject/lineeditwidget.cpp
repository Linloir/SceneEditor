#include <qpropertyanimation.h>
#include <qparallelanimationgroup.h>

#include "lineeditwidget.h"

LineEditWidget::LineEditWidget(QWidget* parent) :
    QWidget(parent)
{
    // Generate colors
    generateColor(_defaultColorScheme);

    // Initialize ui
    initializeUI();

    // Connect line edit events
    connect(_editor, &QLineEdit::returnPressed, this, &LineEditWidget::endEdit);
    connect(_editor, &QLineEdit::editingFinished, this, &LineEditWidget::endEdit);
    connect(_editor, &QLineEdit::textChanged, this, &LineEditWidget::onTextChanged);
}

LineEditWidget::~LineEditWidget() {
}

void LineEditWidget::initializeUI() {
    // Construct and set main layout
    _mainLayout = new QHBoxLayout(this);
    _mainLayout->setContentsMargins(12, 0, 12, 0);
    _mainLayout->setSpacing(8);
    setLayout(_mainLayout);

    // Construct editor container widget
    _editorWidget = new QWidget(this);
    _mainLayout->addWidget(_editorWidget);
    _editorWidget->show();

    // Construct editor layout to stretch editor widget
    _editorWidgetLayout = new QHBoxLayout(_editorWidget);
    _editorWidgetLayout->setContentsMargins(0, 10, 0, 10);
    _editorWidgetLayout->setSpacing(0);
    _editorWidget->setLayout(_editorWidgetLayout);

    // Install event filter to editor widget to resize indicator
    _editorWidget->installEventFilter(this);

    // Construct real text editor
    _editor = new QLineEdit(_editorWidget);
    _editor->setText("");
    _editor->setFont(_defaultFont);
    _editor->setAlignment(Qt::AlignRight);
    _editor->setReadOnly(true);
    _editor->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    _editor->setStyleSheet("QLineEdit{color:#2c2c2c;background-color:#00ffffff;border-style:none;}");
    //_editor->setFixedHeight(_editor->fontMetrics().lineSpacing());  // Restrict one line
    _editor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    _editorWidgetLayout->addWidget(_editor);
    _editor->show();

    // Construct indicator widget
    _indicator = new QWidget(_editorWidget);
    _indicator->setObjectName("indicator");
    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number((float)_indicatorWidth / 2) + "px;}");
    _indicator->resize(_indicatorWidth, _indicatorWidth);
    _indicator->move(_editorWidget->width() - _indicatorWidth, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
    _indicatorEffect = new QGraphicsOpacityEffect(_indicator);
    _indicatorEffect->setOpacity(0);
    _indicator->setGraphicsEffect(_indicatorEffect);
    _indicator->show();

    // Construct background widget
    _backgroundWidget = new QWidget(this);
    _backgroundWidget->resize(size());
    _backgroundWidget->setObjectName("backgroundWidget");
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");
    _backgroundWidget->lower();
    _backgroundWidget->show();

    // Set size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void LineEditWidget::generateColor(QColor colorScheme) {
    _backgroundColor = colorScheme.lighter(120);
    _backgroundColor.setAlpha(5);
    _hoverColor = colorScheme.lighter(120);
    _hoverColor.setAlpha(40);
    _pressedColor = colorScheme.lighter(120);
    _pressedColor.setAlpha(50);
    _indicatorColor = colorScheme;
}

void LineEditWidget::startEdit() {
    if (_editing) {
        return;
    }

    // Set editing flag
    _editing = true;
    
    // Enable qlineedit widget
    _editor->setReadOnly(false);
    _editor->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    _editor->setFocus();
    _editor->setCursorPosition(_editor->text().length());

    // Minorly move cursor to update cursor icon
    QCursor::setPos(QCursor::pos() + QPoint(1, 0));
    QCursor::setPos(QCursor::pos() + QPoint(-1, 0));

    // Add grow and fade in animation for indicator
    QParallelAnimationGroup* startEditAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation* growAnimation = new QPropertyAnimation(_indicator, "geometry");
    QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(_indicatorEffect, "opacity");
    growAnimation->setDuration(200);
    growAnimation->setEasingCurve(QEasingCurve::OutQuad);
    fadeInAnimation->setDuration(200);
    fadeInAnimation->setEasingCurve(QEasingCurve::OutQuad);
    growAnimation->setStartValue(_indicator->geometry());
    growAnimation->setEndValue(QRect(
        0,
        _editorWidget->height() - _indicatorWidth - _indicatorSpacing,
        _editorWidget->width(),
        _indicatorWidth
    ));
    fadeInAnimation->setStartValue(_indicatorEffect->opacity());
    fadeInAnimation->setEndValue(0.999);
    startEditAnimation->addAnimation(growAnimation);
    startEditAnimation->addAnimation(fadeInAnimation);
    startEditAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    
    // Emit signal
    emit onStartEditing(_editor->text());
}

void LineEditWidget::endEdit() {
    if (!_editing) {
        return;
    }

    // Set editing flag
    _editing = false;
    
    // Disable qlineedit widget
    _editor->setReadOnly(true);
    _editor->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    _editor->setSelection(0, 0);

    // Add shrink and fade out animation for indicator
    QParallelAnimationGroup* endEditAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation* shrinkAnimation = new QPropertyAnimation(_indicator, "geometry");
    QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(_indicatorEffect, "opacity");
    shrinkAnimation->setDuration(200);
    shrinkAnimation->setEasingCurve(QEasingCurve::OutQuad);
    fadeOutAnimation->setDuration(200);
    fadeOutAnimation->setEasingCurve(QEasingCurve::OutQuad);
    shrinkAnimation->setStartValue(_indicator->geometry());
    shrinkAnimation->setEndValue(QRect(
        _editorWidget->width() - _indicatorWidth,
        _editorWidget->height() - _indicatorWidth - _indicatorSpacing,
        _indicatorWidth,
        _indicatorWidth
    ));
    fadeOutAnimation->setStartValue(_indicatorEffect->opacity());
    fadeOutAnimation->setEndValue(0);
    endEditAnimation->addAnimation(shrinkAnimation);
    endEditAnimation->addAnimation(fadeOutAnimation);
    endEditAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    
    // Emit signal
    emit onEndEditing(_editor->text());
}

void LineEditWidget::showEvent(QShowEvent* event) {
    // Call on parent
    QWidget::showEvent(event);
    
    // Check initialize state
    if (_initialized) {
        return;
    }
    
    // Initialize size dependent widgets
    _backgroundWidget->resize(size());
    if (_editing) {
        _indicator->move(0, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
        _indicator->resize(_editorWidget->width(), _indicatorWidth);
    }
    else {
        _indicator->move(_editorWidget->width() - _indicatorWidth, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
        _indicator->resize(_indicatorWidth, _indicatorWidth);
    }
    
    // Set initialized flag
    _initialized = true;
}

void LineEditWidget::enterEvent(QEnterEvent* event) {
    // Check on enabled
    if (_enabled) {
        setCursor(Qt::PointingHandCursor);
    }
    
    // Change background color
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // Set hover flag
    _hovered = true;
}

void LineEditWidget::leaveEvent(QEvent* event) {
    setCursor(Qt::ArrowCursor);

    // Change background color
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // Set hover flag
    _hovered = false;
    _pressed = false;
}

void LineEditWidget::mousePressEvent(QMouseEvent* event) {
    // Change background color
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // Set pressed flag
    _pressed = true;
}

void LineEditWidget::mouseReleaseEvent(QMouseEvent* event) {
    // Change background color
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // Trigger on click
    if (_pressed) {
        if (_enabled) {
            if (_editing) {
                endEdit();
            }
            else {
                startEdit();
            }
        }
        else {
            if (_editing) {
                endEdit();
            }
        }
    }

    // Set pressed flag
    _pressed = false;
}

void LineEditWidget::focusInEvent(QFocusEvent* event) {
    // Call on parent
    QWidget::focusInEvent(event);

    // Check on enabled
    if (!_enabled) {
        return;
    }

    // Change background color
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");
    
    // Start edit
    if (!_editing) {
        startEdit();
    }
}

void LineEditWidget::focusOutEvent(QFocusEvent* event) {
    // Call on parent
    QWidget::focusOutEvent(event);

    // Change background color
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // End edit
    if (_editing) {
        endEdit();
    }
}

void LineEditWidget::resizeEvent(QResizeEvent* event) {
    // Check initialize state
    if (!_initialized) {
        return;
    }

    // Resize background widget
    _backgroundWidget->resize(size());
}

bool LineEditWidget::eventFilter(QObject* object, QEvent* event) {
    // Resize indicator when editor widget size changed
    if (object == _editorWidget) {
        if (event->type() == QEvent::Resize) {
            if (_editing) {
                _indicator->move(0, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
                _indicator->resize(_editorWidget->width(), _indicatorWidth);
            }
            else {
                _indicator->move(_editorWidget->width() - _indicatorWidth, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
                _indicator->resize(_indicatorWidth, _indicatorWidth);
            }
        }
    }

    // Call on parent
    return QWidget::eventFilter(object, event);
}

void LineEditWidget::setText(const QString& text) {
    // Set text
    _editor->setText(text);
}

void LineEditWidget::setPlaceholderText(const QString& text) {
    // Set placeholder text
    _editor->setPlaceholderText(text);
}

void LineEditWidget::setValidator(const QValidator* validator) {
    // Set validator
    _editor->setValidator(validator);
}

void LineEditWidget::setEnabled(bool enabled) {
    // Set enabled
    _enabled = enabled;

    // Check for current state
    if (_editing && !_enabled) {
        endEdit();
    }
}

void LineEditWidget::setMargins(QMargins margins) {
    // Set margins
    _mainLayout->setContentsMargins(margins);
}

void LineEditWidget::setMargins(int left, int top, int right, int bottom) {
    // Set margins
    _mainLayout->setContentsMargins(left, top, right, bottom);
}

void LineEditWidget::setBackgroundColor(QColor color) {
    // Set background color
    _backgroundColor = color;

    // Check for current state
    if (!_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
}

void LineEditWidget::setHoverColor(QColor color) {
    // Set hover color
    _hoverColor = color;

    // Check for current state
    if (_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
}

void LineEditWidget::setPressedColor(QColor color) {
    // Set pressed color
    _pressedColor = color;

    // Check for current state
    if (_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
}

void LineEditWidget::setIndicatorColor(QColor color) {
    // Set indicator color
    _indicatorColor = color;
    
    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_indicatorWidth) + "px;}");
}

void LineEditWidget::setColorScheme(QColor primaryColor) {
    // Generate colors
    generateColor(primaryColor);

    // Check for current state
    if (!_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
    else if (_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
    else if (_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }

    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_indicatorWidth) + "px;}");
}

QString LineEditWidget::text() const {
    // Return text
    return _editor->text();
}

QHBoxLayout* LineEditWidget::mainLayout() const {
    // Return main layout
    return _mainLayout;
}