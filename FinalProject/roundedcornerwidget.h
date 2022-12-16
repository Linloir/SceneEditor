#pragma once

#include <qwidget.h>
#include <qevent.h>
#include <qboxlayout.h>

class RoundedCornerWidget : public QWidget {
    
    Q_OBJECT

public:
    RoundedCornerWidget(QWidget* parent = 0);
    ~RoundedCornerWidget();

protected:
    // UI settings
    int _cornerRadius = 15;
    QColor _borderColor = QColor(255, 255, 255);

    // UI elements
    QWidget* _mainWidget = nullptr;
    QVBoxLayout* _mainLayout = nullptr;
    QWidget* _borderWidget = nullptr;
    
    // Initialization mark
    bool _initialized = false;
    void initializeWidgetUI();

    // UI events
    virtual void showEvent(QShowEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

public:
    // Public APIs
    void setCornerRadius(int radius);
    void setBorderColor(QColor color);
    QWidget* mainWidget() const;
};
