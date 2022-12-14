#pragma once

#include <QtWidgets/QMainWindow>
#include <qboxlayout.h>
#include <qgraphicseffect.h>
#include <qpushbutton.h>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;

private:
    // UI control variables
    const int _cornerRadius = 20;
    const QColor _backgroundColor = QColor(251, 251, 251);
    const QColor _borderColor = QColor(104, 104, 104);

    // Window initialize
    void initializeWindowUI();
    virtual void showEvent(QShowEvent* event) override;
    
    // Widget list
    QVBoxLayout* _stretchLayout = nullptr;
    QWidget* _windowWidget = nullptr;
    QWidget* _windowBorder = nullptr;
    QGraphicsDropShadowEffect* _windowShadow = nullptr;
    QVBoxLayout* _windowLayout = nullptr;
    QWidget* _titleBar = nullptr;
    QHBoxLayout* _titleBarLayout = nullptr;
    QWidget* _windowBtnWidget = nullptr;
    QHBoxLayout* _windowBtnLayout = nullptr;
    QPushButton* _minimizeBtn = nullptr;
    QPushButton* _maximizeBtn = nullptr;
    QPushButton* _closeBtn = nullptr;
    QWidget* _objectList = nullptr;
    QWidget* _settingsPannel = nullptr;
    
    // Window size control
    bool _maximized = false;
    QRect _lastWindowGeometry;
    
    virtual void resizeEvent(QResizeEvent* event) override;
    void controlWindowScale();
    
    // User interaction control
    enum MOUSE_STATE {
        MOUSE_STATE_NONE            = 0,
        MOUSE_STATE_RESIZE_LEFT     = 1 << 0,
        MOUSE_STATE_RESIZE_TOP      = 1 << 1,
        MOUSE_STATE_RESIZE_RIGHT    = 1 << 2,
        MOUSE_STATE_RESIZE_BOTTOM   = 1 << 3
    };
    bool _mousePressed = false;
    int _mouseState = MOUSE_STATE_NONE;
    QPointF _lastMousePosition;
    void updateMouseState(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
};
