#include "sceneviewer.h"

#include <string>
#include <qresource.h>
#include <qurl.h>
#include <qdir.h>

#include "illuminer.h"


SceneViewer::SceneViewer(QWidget* parent)
	: QOpenGLWidget(parent)
{
    // Set mouse tracking
    setMouseTracking(true);
    // OpenGL initialize
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 3);
    setFormat(format);

    // Create temp folder
    QDir dir;
    if (!dir.exists("./temp"))
    {
        dir.mkdir("./temp");
    }
    if (!dir.exists("./temp/shaders"))
    {
        dir.mkdir("./temp/shaders");
    }
    
    // Copy the shaders to the temp folder
    extractShaderResorce("vertexshader.glsl");
    extractShaderResorce("fragmentshader.glsl");
    extractShaderResorce("skyboxvertexshader.glsl");
    extractShaderResorce("skyboxfragmentshader.glsl");
}

SceneViewer::~SceneViewer() {
    if (_dirLight) {
        delete _dirLight;
    }

    for (auto obj : _objects) {
        delete obj;
    }
}

void SceneViewer::extractShaderResorce(const QString& shaderName) {
    QString shaderResourcePath = ":/shaders/" + shaderName;
    QString shaderTempPath = "./temp/shaders/" + shaderName;
    
    if (QFile::exists(shaderTempPath))
    {
        QFile::remove(shaderTempPath);
    }
    QFile::copy(shaderResourcePath, shaderTempPath);
    QFile::setPermissions(shaderTempPath, QFile::ReadOwner | QFile::WriteOwner);
}

void SceneViewer::hitTest(const Ray& ray) {
    HitRecord newRecord = HitRecord();
    Renderable* newObject = nullptr;
    int newObjectIndex = -1;
    for (int i = 0; i < _objects.size(); i++) {
        Logger::debug("Testing object " + std::to_string(i));
        Renderable* obj = _objects[i];
        HitRecord hitRecord = obj->hit(ray);
        if (hitRecord.hitted()) {
            Logger::debug("Hitted object " + std::to_string(i));
        }
        else {
            Logger::debug("Missed object " + std::to_string(i));
        }
        if (hitRecord.hitted() && hitRecord.t() < newRecord.t()) {
            newRecord = hitRecord;
            newObject = obj;
            newObjectIndex = i;
        }
    }
    if (newRecord.hitted()) {
        Logger::debug("Hit test hitted object with index " + std::to_string(newObjectIndex));
    }
    _hitRecord = newRecord;
    _hoveredObject = newObject;
}

void SceneViewer::initializeGL() {
    initializeOpenGLFunctions();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
	
    Logger::info("Currently running on OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));

    _shaderProgram.ensureInitialized();
    Logger::info("Shader Program initialized");

    VertexShader vertexShader("./temp/shaders/vertexshader.glsl");
    FragmentShader fragmentShader("./temp/shaders/fragmentshader.glsl");
    _shaderProgram.attachShader(vertexShader);
    _shaderProgram.attachShader(fragmentShader);
    vertexShader.dispose();
    fragmentShader.dispose();

    _skyShader.ensureInitialized();
    Logger::info("Sky Shader initialized");
    
    VertexShader skyVertexShader("./temp/shaders/skyboxvertexshader.glsl");
    FragmentShader skyFragmentShader("./temp/shaders/skyboxfragmentshader.glsl");
    _skyShader.attachShader(skyVertexShader);
    _skyShader.attachShader(skyFragmentShader);
    skyVertexShader.dispose();
    skyFragmentShader.dispose();

    // Test Code Start
    _sky = new SkyBox("E:\\Repositories\\CollegeProjects\\CGAssignments\\FinalProject\\SkyBoxes");
    
    _dirLight = new DirLight();

    Model* model = new Model("E:\\Repositories\\CollegeProjects\\CGAssignments\\FinalProject\\Models\\backpack\\backpack.obj");
    Renderable* backpack = new Renderable(model);
    backpack->move(glm::vec3(-5.0f, -2.0f, -2.0f));
    backpack->updateBoundary();
    _objects.push_back(backpack);
    
    Renderable* backpack2 = new Renderable(model);
    backpack2->makeLight();
    backpack2->originalLight()->setIdealDistance(500);
    _objects.push_back(backpack2);
    // Test Code End
    
    _camera.setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
}

void SceneViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SceneViewer::paintGL() {
    Logger::debug("Repainting");
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    _shaderProgram.bind();

    // Set view and projection matrices
    glm::mat4 view = _camera.viewMatrix();
    glm::mat4 projection = _camera.projectionMatrix((float)width() / (float)height());
    _shaderProgram.setUniform("view", view);
    _shaderProgram.setUniform("projection", projection);
    _shaderProgram.setUniform("viewPos", _camera.position());

    int pointLights = 0;
    int spotLights = 0;

    for (auto object : _objects) {
        if (object->hasLight()) {
            ScopedLight light = object->transformedLight();
            if (light.isPointLight()) {
                light.updateShader(_shaderProgram, pointLights++);
            }
            else {
                light.updateShader(_shaderProgram, spotLights++);
            }
        }
    }
    
    _shaderProgram.setUniform("pointlightnr", pointLights);
    _shaderProgram.setUniform("spotlightnr", spotLights);

    if (_dirLight != nullptr) {
        _dirLight->updateShader(_shaderProgram, 0);
    }

    _shaderProgram.setUniform("dirlightnr", _dirLight != nullptr ? 1 : 0);

    for (auto object : _objects) {
        object->render(_shaderProgram);
    }

    _shaderProgram.unbind();
    
    _skyShader.bind();
    _skyShader.setUniform("view", glm::mat4(glm::mat3(view)));
    _skyShader.setUniform("projection", projection);
    _sky->render();
    _skyShader.unbind();
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
        case Qt::NoButton: {
            // If no button pressed, do hit test and move the current object if selected
            float relX = (float)event->x() / (float)width();
            float relY = 1 - (float)event->y() / (float)height();
            Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
            hitTest(ray);
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
    parentWidget()->update();
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
    parentWidget()->update();
}
