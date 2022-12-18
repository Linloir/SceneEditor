#include "sceneviewer.h"

#include <vector>
#include <string>
#include <qresource.h>
#include <qurl.h>
#include <qdir.h>

#include "vbo.h"
#include "vao.h"
#include "shader.h"
#include "logger.h"
#include "model.h"
#include "skybox.h"

using std::vector;

SceneViewer::SceneViewer(QWidget* parent)
	: QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 3);
    setFormat(format);
    
    // Create a folder
    QDir dir("./temp/shaders");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // Copy the shaders to the folder
    if (QFile::exists("./temp/shaders/vertexshader.vs")) {
        QFile::remove("./temp/shaders/vertexshader.vs");
    }
    QFile::copy(":/shaders/vertexshader.vs", "./temp/shaders/vertexshader.vs");
    QFile::setPermissions("./temp/shaders/vertexshader.vs", QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    if (QFile::exists("./temp/shaders/fragmentshader.fs")) {
        QFile::remove("./temp/shaders/fragmentshader.fs");
    }
    QFile::copy(":/shaders/fragmentshader.fs", "./temp/shaders/fragmentshader.fs");
    QFile::setPermissions("./temp/shaders/fragmentshader.fs", QFile::ReadOwner | QFile::WriteOwner);
}

SceneViewer::~SceneViewer() {

}

void SceneViewer::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    Logger::info("Currently running on OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));

    _shaderProgram.ensureInitialized();
    Logger::info("Shader Program initialized");

    VertexShader vertexShader("./temp/shaders/vertexshader.vs");
    FragmentShader fragmentShader("./temp/shaders/fragmentshader.fs");
    _shaderProgram.attachShader(vertexShader);
    _shaderProgram.attachShader(fragmentShader);
    vertexShader.dispose();
    fragmentShader.dispose();

    skyShader.ensureInitialized();

    VertexShader vertexShader_sky("./temp/shaders/skyboxShader.vs");
    FragmentShader fragmentShader_sky("./temp/shaders/skyboxShader.fs");
    skyShader.attachShader(vertexShader_sky);
    skyShader.attachShader(fragmentShader_sky);
    vertexShader_sky.dispose();
    fragmentShader_sky.dispose();


    terrainShader.ensureInitialized();
    VertexShader vertexShader_ter("./temp/shaders/terrainShader.vs");
    FragmentShader fragmentShader_ter("./temp/shaders/terrainShader.fs");
    terrainShader.attachShader(vertexShader_ter);
    terrainShader.attachShader(fragmentShader_ter);
    vertexShader_ter.dispose();
    fragmentShader_ter.dispose();


    Model* backpackModel = new Model("../Models/backpack/backpack.obj");
    Logger::info("Model loaded");
    Renderable backpack(backpackModel);
    _objects.push_back(backpack);

    sky = new skybox("../skybox");

    ter = new Terrain("../terrain");
    
    _camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
}

void SceneViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SceneViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = _camera.viewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(_camera.zoomVal()), (float)width() / (float)height(), 0.1f, 100.0f);

    terrainShader.bind();
    glm::mat4 Model = glm::mat4(1.0f);
   
    terrainShader.setUniform("view", view);
    terrainShader.setUniform("projection", projection);
    terrainShader.setUniform("model",ter->Model);
    terrainShader.setUniform("texture1", 2);
    ter->render();
    terrainShader.unbind();
    
    _shaderProgram.bind();

    // Set view and projection matrices
    _shaderProgram.setUniform("view", view);
    _shaderProgram.setUniform("projection", projection);

    for (auto object : _objects) {
        object.render(_shaderProgram);
    }

    _shaderProgram.unbind();

    


    skyShader.bind();
    view = glm::mat4(glm::mat3(view));
    skyShader.setUniform("view", view);
    skyShader.setUniform("projection", projection);
    sky->render();
    skyShader.unbind();


}

void SceneViewer::mousePressEvent(QMouseEvent* event) {
    Logger::debug("Mouse pressed at: " + std::to_string(event->x()) + ", " + std::to_string(event->y()));
    if (event->button() == Qt::LeftButton) {
        // TODO: Hit test on objects
    }
    else {
        _lastMousePosition = event->pos();
    }
}

void SceneViewer::mouseMoveEvent(QMouseEvent* event) {
    Logger::debug("Mouse moved with offset: " + std::to_string(event->x() - _lastMousePosition.x()) + ", " + std::to_string(event->y() - _lastMousePosition.y()));
    // Check the type of button pressed
    switch (event->buttons()) {
        case Qt::LeftButton: {
            // Move the selected object
            if (_selectedObject != nullptr) {
                // TODO: move the selected object
            }
            break;
        }
        case Qt::RightButton: {
            // Move the camera
            float xoffset = event->x() - _lastMousePosition.x();
            float yoffset = _lastMousePosition.y() - event->y();    // reversed since y-coordinates go from bottom to top
            float xmovement = xoffset * _cameraMovementSpeed;
            float ymovement = yoffset * _cameraMovementSpeed;
            glm::vec3 cameraPrevPos = _camera.position();
            _camera.move({ -xmovement, -ymovement });
            glm::vec3 cameraNewPos = _camera.position();
            _rotateCenter += cameraNewPos - cameraPrevPos;
            Logger::debug("Camera moved to: " + std::to_string(_camera.position().x) + ", " + std::to_string(_camera.position().y) + ", " + std::to_string(_camera.position().z));
            Logger::debug("New center: " + std::to_string(_rotateCenter.x) + ", " + std::to_string(_rotateCenter.y) + ", " + std::to_string(_rotateCenter.z));
            break;
        }
        case Qt::MiddleButton: {
            // Rotate the camera
            float xoffset = event->x() - _lastMousePosition.x();
            float yoffset = _lastMousePosition.y() - event->y();    // reversed since y-coordinates go from bottom to top
            // Calculate pitch angle
            float pitch = yoffset * _cameraRotationSpeed;
            // Calculate yaw angle
            float yaw = xoffset * _cameraRotationSpeed;
            _camera.rotate(_rotateCenter, pitch, -yaw);
            Logger::debug("Camera rotated to: " + std::to_string(_camera.position().x) + ", " + std::to_string(_camera.position().y) + ", " + std::to_string(_camera.position().z));
            Logger::debug("Center at: " + std::to_string(_rotateCenter.x) + ", " + std::to_string(_rotateCenter.y) + ", " + std::to_string(_rotateCenter.z));
            break;
        }
        default: {
            Logger::warning("Unknown mouse button input");
            Logger::warning("Mouse button: " + std::to_string(event->buttons()));
            break;
        }
    }
    // Update the last mouse position
    _lastMousePosition = event->pos();
    // Update the view
    update();
}

void SceneViewer::wheelEvent(QWheelEvent* event) {
    // Zoom in or out
    float wheelOffset = event->angleDelta().y();
    Logger::debug("Wheel offset: " + std::to_string(wheelOffset));
    _camera.push(wheelOffset * _cameraPushSpeed);
    glm::vec3 cameraFront = _camera.front();
    _rotateCenter += wheelOffset * _cameraPushSpeed * cameraFront;
    Logger::debug("New camera position: " + std::to_string(_camera.position().x) + ", " + std::to_string(_camera.position().y) + ", " + std::to_string(_camera.position().z));
    Logger::debug("New center position: " + std::to_string(_rotateCenter.x) + ", " + std::to_string(_rotateCenter.y) + ", " + std::to_string(_rotateCenter.z));
    // Update the view
    update();
}
