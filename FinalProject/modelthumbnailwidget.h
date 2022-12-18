#pragma once

#include <QtOpenGLWidgets/qopenglwidget.h>
#include <qopenglfunctions.h>
#include <qevent.h>

#include "model.h"
#include "camera.h"
#include "shader.h"
#include "renderable.h"

class ModelThumbnailWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    ModelThumbnailWidget(Model* model, QWidget* parent = 0);
    ~ModelThumbnailWidget();

private:
    Model* _model = nullptr;
    Renderable _object = Renderable::empty();
    ShaderProgram _shaderProgram = ShaderProgram::empty();
    Camera _camera;

private:
    void extractShaderResource(const QString& shaderName);
    
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
};
