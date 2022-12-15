#pragma once

#include <qwidget.h>
#include <qevent.h>
#include <qopenglfunctions.h>
#include <QtOpenGLWidgets/qopenglwidget.h>

#include <vector>

#include "camera.h"
#include "shader.h"
#include "renderable.h"
#include "vao.h"
#include "utils.h"
#include "lightCaster.h"

class SceneViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

private:
    // OpenGL section-------------------------------------
    // List of objects currently in the scene
    std::vector<Renderable> _objects;
    // List of light casters in the scene
    std::vector<Illuminant> _illuminants;
    // Shader program for objects
    ShaderProgram _shaderProgram = ShaderProgram::empty();
    // Main camera
    Camera _camera;
    float _cameraMovementSpeed = 0.02f;
    float _cameraRotationSpeed = 0.3f;
    float _cameraPushSpeed = 0.02f;
    // Rotate center
    glm::vec3 _rotateCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    // User Interaction flags section---------------------
    QPoint _lastMousePosition;
    Renderable* _selectedObject = nullptr;
	
public:
	SceneViewer(QWidget* parent = 0);
	~SceneViewer();
    void update_light();

protected:
    // OpenGL functions
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;

    // Mouse events
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    
};