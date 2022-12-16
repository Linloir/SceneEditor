#pragma once

#include <qevent.h>

#include "framelesswindow.h"
#include "sidebar.h"
#include "editorpage.h"

class MainWindow : public FramelessWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Main ui layout
    QHBoxLayout* _mainLayout = nullptr;
    
    // Side bar and corresponding pages
    SideBar* _sideBar = nullptr;
    EditorPage* _editorPage = nullptr;

    // Place holder widget for resizing pages
    QWidget* _placeHolderWidget = nullptr;

private:
    void resizePages(QResizeEvent* event);

private:
    virtual void showEvent(QShowEvent* event) override;
    virtual bool eventFilter(QObject* object, QEvent* event) override;
};
