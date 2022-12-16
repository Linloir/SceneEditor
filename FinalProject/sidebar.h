#pragma once

#include <qwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qcolor.h>
#include <qmap.h>
#include <qlist.h>

#include "pushbutton.h"
#include "pagewidget.h"

class SideBar : public QWidget
{
    Q_OBJECT
        
public:
    SideBar(QWidget* parent = 0);
    ~SideBar();

private:
    // Initialized flag
    bool _initialized = false;

    // UI control variables
    QColor _backgroundColor = QColor(230, 230, 230);
    int _collapsedWidth = 90;
    int _expandedWidth = 200;

    // UI widgets
    PushButton* _expandButton;
    QLabel* _expandButtonIcon;
    QVBoxLayout* _mainLayout = nullptr; // Layout expand icon and page selection buttons
    QWidget* _pageIconButtonWidget = nullptr;   // Container for page icon buttons
    QVBoxLayout* _pageIconButtonLayout = nullptr;   // Layout page icon buttons
    QWidget* _pageTextButtonWidget = nullptr;   // Container for page text buttons
    QVBoxLayout* _pageTextButtonLayout = nullptr;   // Layout page text buttons
    
    // Interaction controls
    bool _expanded = false;
    PageWidget* _currentPage = nullptr;

    // State storage
    // QMap<PageWidget*, QPair<PushButton*, PushButton*>> _pageButtonMap;
    QList<PageWidget*> _pageList;
    QList<QPair<PushButton*, PushButton*>> _pageButtonList;

private:
    void expand();
    void collapse();
    void selectPage(PageWidget* page);

public:
    void setBackgroundColor(QColor color);
    void setCollapsedWidth(int width);
    void setExpandedWidth(int width);
    void addPage(PageWidget* page);
    void insertPage(PageWidget* page, int index);
    void removePage(PageWidget* page);

signals:
    void onPageChanged(PageWidget* previousPage, PageWidget* currentPage);
};
