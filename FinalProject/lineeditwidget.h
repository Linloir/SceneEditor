#pragma once

#include <qwidget.h>
#include <qlineedit.h>
#include <qboxlayout.h>
#include <qgraphicseffect.h>
#include <qevent.h>

class LineEditWidget : public QWidget {
    
    Q_OBJECT
       
public:
    LineEditWidget(QWidget* parent = 0);
    ~LineEditWidget();

private:
    // UI control variables
    int _cornerRadius = 8;

    // UI elements
    QHBoxLayout* _mainLayout = nullptr; // For layout the user added components and the real editor component
    
    QWidget* _backgroundWidget = nullptr;
    const QColor _defaultColorScheme = QColor(58, 143, 183);
    QColor _backgroundColor;
    QColor _hoverColor;
    QColor _pressedColor;
    
    QWidget* _editorWidget = nullptr;   // Container widget for the real editor component
    QHBoxLayout* _editorWidgetLayout = nullptr;
    
    QLineEdit* _editor = nullptr;
    const QFont _defaultFont = QFont("DengXian", 10, QFont::Normal);

    QWidget* _indicator = nullptr;
    const int _indicatorWidth = 4;
    const int _indicatorSpacing = 4;
    QColor _indicatorColor;
    QGraphicsOpacityEffect* _indicatorEffect;
    
    // Editor state
    bool _hovered = false;
    bool _pressed = false;
    bool _enabled = true;
    bool _editing = false;
    bool _initialized = false;

private:
    // UI Util functions
    void initializeUI();
    void generateColor(QColor colorScheme);
    
    // Interaction Util functions
    void startEdit();
    void endEdit();

private:
    // Interactions
    virtual void showEvent(QShowEvent* event) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void focusInEvent(QFocusEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual bool eventFilter(QObject* obj, QEvent* event) override;

public:
    // Public setter APIs for text
    void setText(const QString& text);
    void setPlaceholderText(const QString& text);
    void setValidator(const QValidator* validator);

    // Publics setter APIs for UI
    void setEnabled(bool enabled = true);
    void setMargins(QMargins margins);
    void setMargins(int left, int top, int right, int bottom);
    void setBackgroundColor(QColor color);
    void setHoverColor(QColor color);
    void setPressedColor(QColor color);
    void setIndicatorColor(QColor color);
    void setColorScheme(QColor primaryColor);

    // Public getter APIs
    QString text() const;
    QHBoxLayout* mainLayout() const;

signals:
    // Public signal functions
    void onStartEditing(const QString& text);
    void onTextChanged(const QString& text);
    void onEndEditing(const QString& text);
};