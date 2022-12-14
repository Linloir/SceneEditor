#include <qapplication.h>
#include <qpainterpath.h>
#include <qscreen.h>
#include <qlabel.h>

#include "framelesswindow.h"
#include "logger.h"

#define MAX_MOUSE_MOVEMENT 300

FramelessWidget::FramelessWidget(int cornerRadius, unsigned int attributes, QWidget* parent)
    : _cornerRadius(cornerRadius), _attributes((LUI_WINDOW_ATTRIBUTES)attributes), QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    
    // Create and properly set real displayed window widget
    _stretchLayout = new QVBoxLayout(this);
    _stretchLayout->setContentsMargins(30, 30, 30, 30);
    _windowWidget = new QWidget(this);
    _windowWidget->setObjectName("windowWidget");
    _windowWidget->setMouseTracking(true);
    _stretchLayout->addWidget(_windowWidget);
    _windowWidget->show();
    setLayout(_stretchLayout);

    // Set style sheet for window widget
    QString windowWidgetStyleSheet = "QWidget#windowWidget{background-color:" + _backgroundColor.name() + ";border-radius:" + QString::number(_cornerRadius) + "px;}";
    _windowWidget->setStyleSheet(windowWidgetStyleSheet);

    // Set shadow for window widget
    _windowShadow = new QGraphicsDropShadowEffect(_windowWidget);
    _windowShadow->setBlurRadius(30);
    _windowShadow->setColor(QColor(0, 0, 0));
    _windowShadow->setOffset(0, 0);
    _windowWidget->setGraphicsEffect(_windowShadow);

    // Create window control buttons container widget & its layout
    _windowBtnWidget = new QWidget(_windowWidget);
    _windowBtnWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _windowBtnWidget->setMouseTracking(true);
    _windowBtnLayout = new QHBoxLayout(_windowBtnWidget);
    _windowBtnLayout->setContentsMargins(0, 0, 0, 0);
    _windowBtnLayout->setSpacing(10);
    _windowBtnLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _windowBtnWidget->setLayout(_windowBtnLayout);

    // Create window control buttons
    _minimizeBtn = new QPushButton(_windowBtnWidget);
    _maximizeBtn = new QPushButton(_windowBtnWidget);
    _closeBtn = new QPushButton(_windowBtnWidget);

    _minimizeBtn->setFixedSize(12, 12);
    _maximizeBtn->setFixedSize(12, 12);
    _closeBtn->setFixedSize(12, 12);

    _minimizeBtn->setStyleSheet("QPushButton{border-radius: 6px; background-color: #c2c2c2;}"
        "QPushButton:hover{background-color: #e98b2a;}");
    _maximizeBtn->setStyleSheet("QPushButton{border-radius: 6px; background-color: #c2c2c2;}"
        "QPushButton:hover{background-color: #2d6d4b;}");
    _closeBtn->setStyleSheet("QPushButton{border-radius: 6px; background-color: #c2c2c2;}"
        "QPushButton:hover{background-color: #ab3b3a;}");

    _windowBtnLayout->addWidget(_minimizeBtn);
    _windowBtnLayout->addWidget(_maximizeBtn);
    _windowBtnLayout->addWidget(_closeBtn);
    if ((_attributes & LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_DISABLE_MINIMIZE) == 0) {
        _minimizeBtn->show();
    }
    if ((_attributes & LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_DISABLE_MAXIMIZE) == 0) {
        _maximizeBtn->show();
    }
    if ((_attributes & LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_DISABLE_CLOSE) == 0) {
        _closeBtn->show();
    }

    // Connect window control buttons
    connect(_minimizeBtn, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(_maximizeBtn, &QPushButton::clicked, this, &FramelessWidget::controlWindowScale);
    connect(_closeBtn, &QPushButton::clicked, this, &QWidget::close);
}

FramelessWidget::FramelessWidget(QWidget* parent)
    : FramelessWidget(0, LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_NO_ATTRIBUTES, parent)
{
}

FramelessWidget::FramelessWidget(int cornerRadius, QWidget* parent)
    : FramelessWidget(cornerRadius, LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_NO_ATTRIBUTES, parent)
{
}

FramelessWidget::FramelessWidget(unsigned int attributes, QWidget* parent)
    : FramelessWidget(0, attributes, parent)
{
}

FramelessWidget::~FramelessWidget() {}

void FramelessWidget::showEvent(QShowEvent* event) {
    // Initialize window UI after window is shown
    initializeWindowUI();
}

void FramelessWidget::initializeWindowUI() {
    if (_initialized) {
        return;
    }

    // Create a round cornered mask for window widget
    QPainterPath path;
    path.addRoundedRect(_windowWidget->rect(), _cornerRadius - 1, _cornerRadius - 1);
    QRegion region(path.toFillPolygon().toPolygon());
    _windowWidget->setMask(region);

    // Create a border for window widget (in order to hide zigzagged edges)
    _windowBorder = new QWidget(this);
    _windowBorder->setObjectName("windowBorder");
    QString windowBorderStyleSheet =
        "QWidget#windowBorder{background-color:#00FFFFFF;border:1.5px solid " + _borderColor.name() + ";border-radius:" + QString::number(_cornerRadius) + "px;}";
    _windowBorder->setStyleSheet(windowBorderStyleSheet);
    _windowBorder->setAttribute(Qt::WA_TransparentForMouseEvents);
    _windowBorder->move(_windowWidget->pos() - QPoint(1, 1));
    _windowBorder->resize(_windowWidget->size() + QSize(2, 2));
    _windowBorder->show();

    // Move button widget to the top right of the window widget
    _windowBtnWidget->move(_windowWidget->width() - _windowBtnWidget->width() - 18, 18);
    _windowBtnWidget->show();

    // Set initialized state
    _initialized = true;
}

void FramelessWidget::resizeEvent(QResizeEvent* event) {
    // Resize window border
    if (_windowBorder != nullptr) {
        _windowBorder->move(_windowWidget->pos() - QPoint(1, 1));
        _windowBorder->resize(_windowWidget->size() + QSize(2, 2));
    }

    // Resize window mask
    QPainterPath path;
    path.addRoundedRect(_windowWidget->rect(), _cornerRadius - 1, _cornerRadius - 1);
    QRegion region(path.toFillPolygon().toPolygon());
    _windowWidget->setMask(region);

    // Move button widget to the top right of the window widget
    _windowBtnWidget->move(_windowWidget->width() - _windowBtnWidget->width() - 18, 18);
}

void FramelessWidget::controlWindowScale() {
    if ((_attributes & LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_DISABLE_MAXIMIZE) != 0) {
        return;
    }
    if (!_maximized) {
        _lastWindowGeometry = frameGeometry();

        Logger::debug("Maximizing window:");
        Logger::debug("[+] current window position: " + std::to_string(x()) + ", " + std::to_string(y()));
        Logger::debug("[+] current window size: " + std::to_string(width()) + ", " + std::to_string(height()));
        Logger::debug("[+] current geometry: " + std::to_string(_lastWindowGeometry.x()) + ", " + std::to_string(_lastWindowGeometry.y()) + ", " + std::to_string(_lastWindowGeometry.width()) + ", " + std::to_string(_lastWindowGeometry.height()));
        Logger::debug("[+] current window widget position: " + std::to_string(_windowWidget->x()) + ", " + std::to_string(_windowWidget->y()));
        Logger::debug("[+] current window widget size: " + std::to_string(_windowWidget->width()) + ", " + std::to_string(_windowWidget->height()));
        Logger::debug("[+] current window border position: " + std::to_string(_windowBorder->x()) + ", " + std::to_string(_windowBorder->y()));
        Logger::debug("[+] current window border size: " + std::to_string(_windowBorder->width()) + ", " + std::to_string(_windowBorder->height()));

        _windowShadow->setEnabled(false);
        _windowBorder->hide();
        QString windowWidgetStyleSheet = "QWidget#windowWidget{background-color:" + _backgroundColor.name() + ";}";
        _windowWidget->setStyleSheet(windowWidgetStyleSheet);

        _stretchLayout->setContentsMargins(0, 0, 0, 0);

        showMaximized();

        QPainterPath path;
        path.addRect(_windowWidget->rect());
        QRegion mask(path.toFillPolygon().toPolygon());
        _windowWidget->setMask(mask);

        _maximized = true;
    }
    else {
        _stretchLayout->setContentsMargins(30, 30, 30, 30);

        showNormal();

        resize(_lastWindowGeometry.size());
        move(_lastWindowGeometry.topLeft());

        _windowShadow->setEnabled(true);
        _windowBorder->show();
        QString windowWidgetStyleSheet = "QWidget#windowWidget{background-color:" + _backgroundColor.name() + ";border-radius:" + QString::number(_cornerRadius) + "px;}";
        _windowWidget->setStyleSheet(windowWidgetStyleSheet);

        QPainterPath path;
        path.addRoundedRect(_windowWidget->rect(), _cornerRadius - 1, _cornerRadius - 1);
        QRegion mask(path.toFillPolygon().toPolygon());
        _windowWidget->setMask(mask);

        Logger::debug("Restoring window:");
        Logger::debug("[+] current window position: " + std::to_string(x()) + ", " + std::to_string(y()));
        Logger::debug("[+] current window size: " + std::to_string(width()) + ", " + std::to_string(height()));
        Logger::debug("[+] current geometry: " + std::to_string(frameGeometry().x()) + ", " + std::to_string(frameGeometry().y()) + ", " + std::to_string(frameGeometry().width()) + ", " + std::to_string(frameGeometry().height()));
        Logger::debug("[+] current window widget position: " + std::to_string(_windowWidget->x()) + ", " + std::to_string(_windowWidget->y()));
        Logger::debug("[+] current window widget size: " + std::to_string(_windowWidget->width()) + ", " + std::to_string(_windowWidget->height()));

        _maximized = false;
    }
}

void FramelessWidget::updateMouseState(QMouseEvent* event) {
    _mouseState = MOUSE_STATE_NONE;
    if ((_attributes & LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_DISABLE_RESIZE) != 0) {
        return;
    }
    if (_maximized) {
        setCursor(Qt::ArrowCursor);
        return;
    }
    // Set mouse state according to abs distance from window border
    if (abs(event->globalPos().x() - (_windowWidget->frameGeometry().left() + frameGeometry().left())) < 5) {
        _mouseState |= MOUSE_STATE_RESIZE_LEFT;
    }
    if (abs(event->globalPos().x() - (_windowWidget->frameGeometry().right() + frameGeometry().left())) < 5) {
        _mouseState |= MOUSE_STATE_RESIZE_RIGHT;
    }
    if (abs(event->globalPos().y() - (_windowWidget->frameGeometry().top() + frameGeometry().top())) < 5) {
        _mouseState |= MOUSE_STATE_RESIZE_TOP;
    }
    if (abs(event->globalPos().y() - (_windowWidget->frameGeometry().bottom() + frameGeometry().top())) < 5) {
        _mouseState |= MOUSE_STATE_RESIZE_BOTTOM;
    }
    // Set cursor shape according to mouse state
    switch (_mouseState) {
        case MOUSE_STATE_RESIZE_LEFT:
        case MOUSE_STATE_RESIZE_RIGHT:
            setCursor(Qt::SizeHorCursor);
            break;
        case MOUSE_STATE_RESIZE_TOP:
        case MOUSE_STATE_RESIZE_BOTTOM:
            setCursor(Qt::SizeVerCursor);
            break;
        case MOUSE_STATE_RESIZE_LEFT | MOUSE_STATE_RESIZE_TOP:
        case MOUSE_STATE_RESIZE_RIGHT | MOUSE_STATE_RESIZE_BOTTOM:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case MOUSE_STATE_RESIZE_LEFT | MOUSE_STATE_RESIZE_BOTTOM:
        case MOUSE_STATE_RESIZE_RIGHT | MOUSE_STATE_RESIZE_TOP:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
    }
}

void FramelessWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        _mousePressed = true;
        _lastMousePosition = event->globalPos().toPointF();
    }
}

void FramelessWidget::mouseReleaseEvent(QMouseEvent* event) {
    _mousePressed = false;
    QScreen* screen = QGuiApplication::screenAt(event->globalPos());
    Logger::debug("Current screen geometry:");
    Logger::debug("[+] screen position: " + std::to_string(screen->geometry().x()) + ", " + std::to_string(screen->geometry().y()));
    Logger::debug("[+] screen size: " + std::to_string(screen->geometry().width()) + ", " + std::to_string(screen->geometry().height()));
    if (abs(event->globalPos().y() - screen->geometry().top()) < 5) {
        controlWindowScale();
    }
    updateMouseState(event);
}

void FramelessWidget::mouseMoveEvent(QMouseEvent* event) {
    Logger::debug("Detected mouse move");
    Logger::debug("[+] mouse global position : " + std::to_string(event->globalPos().x()) + ", " + std::to_string(event->globalPos().y()));
    Logger::debug("[+] window geometry: " + std::to_string(frameGeometry().x()) + ", " + std::to_string(frameGeometry().y()) + ", " + std::to_string(frameGeometry().width()) + ", " + std::to_string(frameGeometry().height()));
    Logger::debug("[+] widget frame geometry: " + std::to_string(_windowWidget->frameGeometry().x()) + ", " + std::to_string(_windowWidget->frameGeometry().y()));
    Logger::debug("[+] widget frame size: " + std::to_string(_windowWidget->frameGeometry().width()) + ", " + std::to_string(_windowWidget->frameGeometry().height()));
    if (event->buttons() == Qt::NoButton) {
        _mousePressed = false;
    }
    if (abs(event->globalPos().x() - _lastMousePosition.x()) > MAX_MOUSE_MOVEMENT) {
        // Maybe moving window across monitors, avoid window disappear
        _lastMousePosition = event->globalPos().toPointF();
    }
    if (abs(event->globalPos().y() - _lastMousePosition.y()) > MAX_MOUSE_MOVEMENT) {
        // Maybe moving window across monitors, avoid window disappear
        _lastMousePosition = event->globalPos().toPointF();
    }
    if (!_mousePressed) {
        updateMouseState(event);
    }
    else {
        // Resize window according to mouse state
        switch (_mouseState) {
            case MOUSE_STATE_RESIZE_LEFT:
                resize(frameGeometry().width() - event->globalPos().x() + frameGeometry().left() + _windowWidget->frameGeometry().left(), frameGeometry().height());
                move(event->globalPos().x() - _windowWidget->frameGeometry().left(), frameGeometry().top());
                break;
            case MOUSE_STATE_RESIZE_RIGHT:
                resize(event->globalPos().x() - frameGeometry().left() + _windowWidget->frameGeometry().left(), frameGeometry().height());
                break;
            case MOUSE_STATE_RESIZE_TOP:
                resize(frameGeometry().width(), frameGeometry().height() - event->globalPos().y() + frameGeometry().top() + _windowWidget->frameGeometry().top());
                move(frameGeometry().left(), event->globalPos().y() - _windowWidget->frameGeometry().top());
                break;
            case MOUSE_STATE_RESIZE_BOTTOM:
                resize(frameGeometry().width(), event->globalPos().y() - frameGeometry().top() + _windowWidget->frameGeometry().top());
                break;
            case MOUSE_STATE_RESIZE_LEFT | MOUSE_STATE_RESIZE_TOP:
                resize(frameGeometry().width() - event->globalPos().x() + frameGeometry().left() + _windowWidget->frameGeometry().left(), frameGeometry().height() - event->globalPos().y() + frameGeometry().top() + _windowWidget->frameGeometry().top());
                move(event->globalPos().x() - _windowWidget->frameGeometry().left(), event->globalPos().y() - _windowWidget->frameGeometry().top());
                break;
            case MOUSE_STATE_RESIZE_LEFT | MOUSE_STATE_RESIZE_BOTTOM:
                resize(frameGeometry().width() - event->globalPos().x() + frameGeometry().left() + _windowWidget->frameGeometry().left(), event->globalPos().y() - frameGeometry().top() + _windowWidget->frameGeometry().top());
                move(event->globalPos().x() - _windowWidget->frameGeometry().left(), frameGeometry().top());
                break;
            case MOUSE_STATE_RESIZE_RIGHT | MOUSE_STATE_RESIZE_TOP:
                resize(event->globalPos().x() - frameGeometry().left() + _windowWidget->frameGeometry().left(), frameGeometry().height() - event->globalPos().y() + frameGeometry().top() + _windowWidget->frameGeometry().top());
                move(frameGeometry().left(), event->globalPos().y() - _windowWidget->frameGeometry().top());
                break;
            case MOUSE_STATE_RESIZE_RIGHT | MOUSE_STATE_RESIZE_BOTTOM:
                resize(event->globalPos().x() - frameGeometry().left() + _windowWidget->frameGeometry().left(), event->globalPos().y() - frameGeometry().top() + _windowWidget->frameGeometry().top());
                break;
            default:
                if (_maximized) {
                    qreal wRatio = (qreal)event->pos().x() / (qreal)_windowWidget->width();
                    controlWindowScale();
                    move(event->globalPos().x() - _windowWidget->width() * wRatio, event->globalPos().y() - 52);
                }
                else {
                    move(frameGeometry().left() + event->globalPos().x() - _lastMousePosition.x(), frameGeometry().top() + event->globalPos().y() - _lastMousePosition.y());
                }
                break;
        }
        _lastMousePosition = event->globalPos().toPointF();
    }
}

FramelessWidget::LUI_WINDOW_ATTRIBUTES FramelessWidget::getWindowAttributes() {
    return _attributes;
}

void FramelessWidget::setWindowAttributes(unsigned int attributes) {
    _attributes = (LUI_WINDOW_ATTRIBUTES)attributes;
    if ((_attributes & LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_DISABLE_MINIMIZE) == 0) {
        _minimizeBtn->show();
    }
    else {
        _minimizeBtn->hide();
    }
    if ((_attributes & LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_DISABLE_MAXIMIZE) == 0) {
        _maximizeBtn->show();
    }
    else {
        _maximizeBtn->hide();
    }
    if ((_attributes & LUI_WINDOW_ATTRIBUTES::LUI_WINDOW_DISABLE_CLOSE) == 0) {
        _closeBtn->show();
    }
    else {
        _closeBtn->hide();
    }
}
