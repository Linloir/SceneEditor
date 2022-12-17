#pragma once

#include <QtWidgets/QMainWindow>
#include <qevent.h>
#include <qboxlayout.h>
#include <qgraphicseffect.h>
#include <qpushbutton.h>

class FramelessWindow : public QWidget {
    
    Q_OBJECT
    
public:
    // Window attributes
    enum LUI_WINDOW_ATTRIBUTES {
        LUI_WINDOW_NO_ATTRIBUTES    = 0,
        LUI_WINDOW_DISABLE_CLOSE    = 1 << 0,
        LUI_WINDOW_DISABLE_MAXIMIZE = 1 << 1,
        LUI_WINDOW_DISABLE_MINIMIZE = 1 << 2,
        LUI_WINDOW_DISABLE_RESIZE   = 1 << 3
    };

public:
    FramelessWindow(QWidget* parent = 0);
    FramelessWindow(int cornerRadius, QWidget* parent = 0);
    FramelessWindow(unsigned int attributes, QWidget* parent = 0);
    FramelessWindow(int cornerRadius, unsigned int attributes, QWidget* parent = 0);
    ~FramelessWindow();

protected:
    // UI control variables
    const int _cornerRadius = 0;
    const QColor _backgroundColor = QColor(251, 251, 251);
    const QColor _borderColor = QColor(104, 104, 104);

    // Window initialize
    bool _initialized = false;  // prevent double initialization when restore from minimized state
    void initializeWindowUI();
    virtual void showEvent(QShowEvent* event) override;

    // Widget list
    QVBoxLayout* _stretchLayout = nullptr;

    QWidget* _windowWidget = nullptr;
    QWidget* _windowBorder = nullptr;
    QGraphicsDropShadowEffect* _windowShadow = nullptr;

    QWidget* _windowBtnWidget = nullptr;
    QHBoxLayout* _windowBtnLayout = nullptr;
    QPushButton* _minimizeBtn = nullptr;
    QPushButton* _maximizeBtn = nullptr;
    QPushButton* _closeBtn = nullptr;

    // Window size control
    LUI_WINDOW_ATTRIBUTES _attributes = LUI_WINDOW_NO_ATTRIBUTES;
    bool _maximized = false;
    QRect _lastWindowGeometry;

    virtual void resizeEvent(QResizeEvent* event) override;
    void controlWindowScale();

    // User interaction control
    enum MOUSE_STATE {
        MOUSE_STATE_NONE = 0,
        MOUSE_STATE_RESIZE_LEFT = 1 << 0,
        MOUSE_STATE_RESIZE_TOP = 1 << 1,
        MOUSE_STATE_RESIZE_RIGHT = 1 << 2,
        MOUSE_STATE_RESIZE_BOTTOM = 1 << 3
    };
    bool _mousePressed = false;
    int _mouseState = MOUSE_STATE_NONE;
    QPointF _lastMousePosition;
    void updateMouseState(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    
public:
    QWidget* windowWidget() const { return _windowWidget; }
    LUI_WINDOW_ATTRIBUTES getWindowAttributes();
    void setWindowAttributes(unsigned int attributes);
};