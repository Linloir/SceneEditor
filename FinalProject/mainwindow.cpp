#include "mainwindow.h"
#include "logger.h"

#define MAX_MOUSE_MOVEMENT 300

MainWindow::MainWindow(QWidget *parent)
    : FramelessWindow(20, parent)
{
    // Create main layout for page and sidebar
    _mainLayout = new QHBoxLayout(_windowWidget);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(0);
    _windowWidget->setLayout(_mainLayout);

    // Create placeholder widget for pages
    _placeHolderWidget = new QWidget(_windowWidget);
    _placeHolderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _placeHolderWidget->setMouseTracking(true);
    
    // Connect the resize event of the placeholder widget to the resizePages function using event filter
    _placeHolderWidget->installEventFilter(this);
    
    // Create sidebar
    _sideBar = new SideBar(_windowWidget);
    connect(_sideBar, &SideBar::onPageChanged, this, [=](PageWidget* previousPage, PageWidget* currentPage) {
        // Check for input validity
        if (previousPage == currentPage) {
            return;
        }
        if (previousPage != nullptr) {
            previousPage->offStage();
        }
        if (currentPage != nullptr) {
            currentPage->onStage();
        }
    });
    _sideBar->setMouseTracking(true);

    // Add sidebar and placeholder widget to main layout
    _mainLayout->addWidget(_sideBar);
    _mainLayout->addWidget(_placeHolderWidget);
    
    // Create editor page and connect to side bar
    _editorPage = new EditorPage(_placeHolderWidget);
    _editorPage->setMouseTracking(true);
    _sideBar->addPage(_editorPage);

    // Create about page and connect to side bar
    _aboutPage = new AboutPage(_placeHolderWidget);
    _aboutPage->setMouseTracking(true);
    _sideBar->addPage(_aboutPage);
}

MainWindow::~MainWindow() {
}

void MainWindow::resizePages(QResizeEvent* event) {
    // Check for input validity
    if (event == nullptr) {
        return;
    }

    // Get the size of the placeholder widget
    QSize size = event->size();

    // Resize the editor page
    _editorPage->resize(size);
    _aboutPage->resize(size);
}

void MainWindow::showEvent(QShowEvent* event) {
    // Call parent show event
    FramelessWindow::showEvent(event);

    // Resize all the pages based on the placeholder widget
    _editorPage->resize(_placeHolderWidget->size());
    _aboutPage->resize(_placeHolderWidget->size());
}

bool MainWindow::eventFilter(QObject* object, QEvent* event) {
    // Check for input validity
    if (object == nullptr || event == nullptr) {
        return false;
    }

    // Check if the object is the placeholder widget
    if (object == _placeHolderWidget) {
        // Check if the event is a resize event
        if (event->type() == QEvent::Resize) {
            // Resize all the pages
            resizePages(static_cast<QResizeEvent*>(event));
        }
    }

    // Call parent event filter
    return FramelessWindow::eventFilter(object, event);
}