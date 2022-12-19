#pragma once

#include <qwidget.h>
#include <qevent.h>
#include <qopenglfunctions.h>
#include <qpair.h>
#include <QtOpenGLWidgets/qopenglwidget.h>

#include <vector>

#include "camera.h"
#include "shader.h"
#include "renderable.h"
#include "illuminer.h"
#include "utils.h"
#include "logger.h"
#include "skybox.h"
#include "terrain.h"

class SceneViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

private:
    // OpenGL section-------------------------------------
    // List of objects currently in the scene
    std::vector<Renderable*> _objects;
    // Sky Box
    SkyBox* _sky = nullptr;
    // Dir light
    bool _dirLightOn = false;
    DirLight* _dirLight = nullptr;
    // Terrain
    Terrain* _terrain;
    // Shader program for objects
    ShaderProgram _shaderProgram = ShaderProgram::empty();
    ShaderProgram _boundShader = ShaderProgram::empty();
    ShaderProgram _skyShader = ShaderProgram::empty();
    ShaderProgram _terrainShader = ShaderProgram::empty();
    // Main camera
    Camera _camera;
    float _cameraMovementSpeed = 0.02f;
    float _cameraRotationSpeed = 0.3f;
    float _cameraPushSpeed = 0.02f;
    // Rotate center
    glm::vec3 _rotateCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    // User preferences
    bool _stickToSurface = false;   // whether to rotate the object to the normal of the hitted surface

    // User Interaction flags section---------------------
    QPoint _lastMousePosition;
    bool _controlPressed = false;
    bool _dragged = false;
    bool _hideBound = false;
    Renderable* _hoveredObject = nullptr;
    Renderable* _pressedObject = nullptr;
    Renderable* _selectedObject = nullptr;
    Renderable* _operatingObject = nullptr;
    HitRecord _hitRecord;

    // UI interface control
    const int _cornerRadius = 10;
    bool _initialized = false;
	
public:
	SceneViewer(QWidget* parent = 0);
	~SceneViewer();

private:
    void extractShaderResource(const QString& shaderName);
    Renderable* hitTest(const Ray& ray);

private:
    void moveCamera(QMouseEvent* event);
    void rotateCamera(QMouseEvent* event);
    void moveOperatingObject(const Ray& ray);

public:
    void addObject(Model* model);

protected:
    // OpenGL functions
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;

    // Mouse events
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

public:
    void setDragFlag() { 
        _hideBound = true;
        parentWidget()->update();
    }
    void clearDragFlag() {
        _hideBound = false;
        if (_selectedObject != nullptr)
            _selectedObject->updateBoundary();
        parentWidget()->update();
    }
    void deleteObject();
    void updateSetting(QPair<QString, QString> setting);

signals:
    void onHover(Renderable* object);
    void onSelect(Renderable* object);
    void onUpdate(Renderable* object);
};