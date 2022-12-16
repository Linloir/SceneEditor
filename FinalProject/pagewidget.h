#pragma once

#include <qwidget.h>
#include <qgraphicseffect.h>

#include "pushbutton.h"

class PageWidget : public QWidget
{
    
    Q_OBJECT
        
public:
    PageWidget(QWidget* parent = 0);
    ~PageWidget();

protected:
    // Page layout and content widget (which holds the actual page elements)
    QVBoxLayout* _stretchLayout = nullptr;
    QWidget* _contentWidget = nullptr;
    
    // Button widgets for side bar
    PushButton* _iconButton = nullptr;
    PushButton* _textButton = nullptr;

    // Opacity effects
    QGraphicsOpacityEffect* _pageOpacityEffect = nullptr;

    // Page position memory
    QPoint _originPagePosition = QPoint(0, 0);
    
public:
    virtual PushButton* getPageIconButton(QWidget* context) = 0;  // provide a push button with only an icon
    virtual PushButton* getPageTextButton(QWidget* context) = 0;  // provide a push button with an icon and a description text
    
public:
    void onStage();     // provide an on stage animation when the page is selected
    void offStage();    // provide an off stage animation when the page is deselected
};