#pragma once

#include <qwidget.h>
#include <qevent.h>
#include <qboxlayout.h>
#include <qgraphicseffect.h>
#include <qpropertyanimation.h>

class PushButton : public QWidget {
    
    Q_OBJECT

public:
    enum LUI_BTN_INDICATOR_POS {
        LUI_BTN_POS_LEFT,
        LUI_BTN_POS_RIGHT,
        LUI_BTN_POS_TOP,
        LUI_BTN_POS_BOTTOM
    };
        
public:
    PushButton(QWidget* child, QWidget* parent);
    ~PushButton();

private:
    // Button Layout
    QWidget* _childWidget;
    QRect _childWidgetOriginalGeometry;
    const float _childWidgetShrinkRatio = 0.9;
    QHBoxLayout* _stretchLayout;
    QMargins _contentMargin = QMargins(12, 12, 12, 12);

    // Button ui
    int _radius = 8;
    
    QWidget* _backgroundWidget;
    const QColor _defaultColorScheme = QColor(58, 143, 183);
    QColor _backgroundColor;
    QColor _hoverColor;
    QColor _pressedColor;
    QColor _selectedColor;
    QColor _restoredColor[5];
    
    QWidget* _indicator;
    LUI_BTN_INDICATOR_POS _indicatorPosition = LUI_BTN_POS_LEFT;
    const int _indicatorWidth = 6;
    const int _indicatorSpacing = 4;
    const float _activatedLengthRatio = 0.4;
    const float _hoveredLengthRatio = 0.5;
    const float _pressedLengthRatio = 0.2;
    QColor _indicatorColor;
    QGraphicsOpacityEffect* _indicatorEffect;

    // Button state
    bool _enabled = true;
    bool _hovered = false;
    bool _pressed = false;
    bool _selected = false;
    
private:
    // UI util functions
    void initializeUI();
    void generateColor(QColor colorScheme);

private:
    // Interactions
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

public:
    // Operation APIs
    void select();
    void deselect();
    void setEnabled(bool enabled = true);
    
    // Attribute setter APIs
    void setRadius(int radius);
    
    void setBackgroundColor(QColor color);
    void setHoverColor(QColor color);
    void setPressedColor(QColor color);
    void setSelectedColor(QColor color);
    void setIndicatorColor(QColor color);
    void setColorScheme(QColor primaryColor);
    
    QHBoxLayout* mainLayout() const;
    void setAlignment(Qt::Alignment alignment);
    void setMargin(QMargins margin);
    void setMargin(int left, int top, int right, int bottom);

    void setIndicatorPosition(LUI_BTN_INDICATOR_POS position);
    
    QWidget* childWidget();
    void setChildWidget(QWidget* widget);

    bool isSelected() const;

signals:
    void onClick();
    void onHover();
    void onPressed();
};
