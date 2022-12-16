#include <qpainterpath.h>
#include <qregularexpression.h>

#include "roundedcornerwidget.h"

RoundedCornerWidget::RoundedCornerWidget(QWidget* parent) : QWidget(parent) {
    // Create main layout and main widget
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 1, 1, 1);
    _mainLayout->setSpacing(0);
    _mainWidget = new QWidget(this);
    _mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _mainLayout->addWidget(_mainWidget);
    _mainWidget->show();
    setLayout(_mainLayout);
}

RoundedCornerWidget::~RoundedCornerWidget() {
}

void RoundedCornerWidget::showEvent(QShowEvent* event) {
    // Call parent show event
    QWidget::showEvent(event);

    if (_initialized) {
        return;
    }

    // Initialize widget UI
    initializeWidgetUI();

    // Set initialized state
    _initialized = true;
}

void RoundedCornerWidget::initializeWidgetUI() {
    // Set up border widget
    _borderWidget = new QWidget(this);
    _borderWidget->setObjectName("borderWidget");
    QString borderWidgetStyleSheet =
        "QWidget#borderWidget{background-color:#00FFFFFF;border:2px solid " + _borderColor.name() + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}";
    _borderWidget->setStyleSheet(borderWidgetStyleSheet);
    _borderWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    _borderWidget->move(0, 0);
    _borderWidget->resize(width(), height());
    _borderWidget->show();

    // Create a round cornered mask
    QPainterPath path;
    path.addRoundedRect(_mainWidget->rect(), _cornerRadius, _cornerRadius);
    QRegion region(path.toFillPolygon().toPolygon());
    _mainWidget->setMask(region);
}

void RoundedCornerWidget::resizeEvent(QResizeEvent* event) {
    // Resize widget border
    if (_borderWidget != nullptr) {
        _borderWidget->move(0, 0);
        _borderWidget->resize(width(), height());
    }
    
    // Resize window mask
    if (_mainWidget != nullptr) {
        QPainterPath path;
        path.addRoundedRect(_mainWidget->rect(), _cornerRadius, _cornerRadius);
        QRegion region(path.toFillPolygon().toPolygon());
        _mainWidget->setMask(region);
    }
}

void RoundedCornerWidget::setCornerRadius(int radius) {
    // Set corner radius
    _cornerRadius = radius;

    // Modify the corner radius in the current style sheet of the border widget
    QString styleSheet = _borderWidget->styleSheet();
    styleSheet.replace(QRegularExpression("border-radius:\\d+px;"),
        QString("border-radius:%1px;").arg(_cornerRadius));
    _borderWidget->setStyleSheet(styleSheet);
    
    // Reset the mask of the main widget
    QPainterPath path;
    path.addRoundedRect(_mainWidget->rect(), _cornerRadius, _cornerRadius);
    QRegion region(path.toFillPolygon().toPolygon());
    _mainWidget->setMask(region);
}

void RoundedCornerWidget::setBorderColor(QColor color) {
    // Set color
    _borderColor = color;

    // Modify the border color in the current style sheet of the border widget
    QString styleSheet = _borderWidget->styleSheet();
    styleSheet.replace(QRegularExpression("border:2px solid #\\w+;"),
        QString("border:2px solid %1;").arg(_borderColor.name()));
    _borderWidget->setStyleSheet(styleSheet);
}

QWidget* RoundedCornerWidget::mainWidget() const {
    return _mainWidget;
}
