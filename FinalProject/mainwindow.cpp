#include "mainwindow.h"
#include "sceneviewer.h"

#include <qboxlayout.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	// Set layout as a horizontal box layout
	QHBoxLayout* horizontalLayout = new QHBoxLayout();
	ui.centralWidget->setLayout(horizontalLayout);
	// Create a placeholder widget on the left
	//QWidget* leftWidget = new QWidget(ui.centralWidget);
	//horizontalLayout->addWidget(leftWidget);
	// Add sceneviewer widget to the main window
	SceneViewer* sceneViewer = new SceneViewer(ui.centralWidget);
	horizontalLayout->addWidget(sceneViewer);
	sceneViewer->resize(400, 400);
	
}

MainWindow::~MainWindow()
{}
