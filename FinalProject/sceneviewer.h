#pragma once

#include <qwidget.h>
#include <qopenglfunctions.h>
#include <QtOpenGLWidgets/qopenglwidget.h>

#include <vector>

#include "renderable.h"
#include "utils.h"

class SceneViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

private:
    std::vector<Renderable> _objects;
	
public:
	SceneViewer(QWidget* parent = 0);
	~SceneViewer();

protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
};