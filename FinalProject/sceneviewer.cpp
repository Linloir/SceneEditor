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
    // Set key tracking
    setFocusPolicy(Qt::StrongFocus);
    // Set the focus
    setFocus();

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
    extractShaderResource("vertexshader.glsl");
    extractShaderResource("fragmentshader.glsl");
    extractShaderResource("skyboxvertexshader.glsl");
    extractShaderResource("skyboxfragmentshader.glsl");
    extractShaderResource("terrainvertexshader.glsl");
    extractShaderResource("terrainfragmentshader.glsl");
    extractShaderResource("boundfragmentshader.glsl");
    extractShaderResource("boundvertexshader.glsl");
}

SceneViewer::~SceneViewer() {
    if (_dirLight) {
        delete _dirLight;
    }

    for (auto obj : _objects) {
        delete obj;
    }
}

void SceneViewer::extractShaderResource(const QString& shaderName) {
    QString shaderResourcePath = ":/shaders/" + shaderName;
    QString shaderTempPath = "./temp/shaders/" + shaderName;
    
    if (QFile::exists(shaderTempPath))
    {
        QFile::remove(shaderTempPath);
    }
    QFile::copy(shaderResourcePath, shaderTempPath);
    QFile::setPermissions(shaderTempPath, QFile::ReadOwner | QFile::WriteOwner);
}

Renderable* SceneViewer::hitTest(const Ray& ray) {
    HitRecord newRecord = HitRecord();
    Renderable* newObject = nullptr;
    for (int i = 0; i < _objects.size(); i++) {
        Logger::debug("Testing object " + std::to_string(i));
        Renderable* obj = _objects[i];
        if (obj == _operatingObject) {
            // Ignore current operating Object
            continue;
        }
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
        }
    }
    _hitRecord = newRecord;
    return newObject;
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

    _boundShader.ensureInitialized();
    Logger::info("Bound Shader initialized");
    VertexShader boundVertexShader("./temp/shaders/boundvertexshader.glsl");
    FragmentShader boundFragmentShader("./temp/shaders/boundfragmentshader.glsl");
    _boundShader.attachShader(boundVertexShader);
    _boundShader.attachShader(boundFragmentShader);
    boundVertexShader.dispose();
    boundFragmentShader.dispose();

    _skyShader.ensureInitialized();
    Logger::info("Sky Shader initialized");
    
    VertexShader skyVertexShader("./temp/shaders/skyboxvertexshader.glsl");
    FragmentShader skyFragmentShader("./temp/shaders/skyboxfragmentshader.glsl");
    _skyShader.attachShader(skyVertexShader);
    _skyShader.attachShader(skyFragmentShader);
    skyVertexShader.dispose();
    skyFragmentShader.dispose();

    _terrainShader.ensureInitialized();
    Logger::info("Terrain Shader initialized");
    
    VertexShader terrainVertexShader("./temp/shaders/terrainvertexshader.glsl");
    FragmentShader terrainFragmentShader("./temp/shaders/terrainfragmentshader.glsl");
    _terrainShader.attachShader(terrainVertexShader);
    _terrainShader.attachShader(terrainFragmentShader);
    terrainVertexShader.dispose();
    terrainFragmentShader.dispose();

    // Test Code Start
    _sky = new SkyBox("E:\\Repositories\\CollegeProjects\\CGAssignments\\FinalProject\\SkyBoxes");
    _terrain = new Terrain("E:\\Repositories\\CollegeProjects\\CGAssignments\\FinalProject\\Terrains");
    
    _dirLight = new DirLight();

    //Model* model = new Model("E:\\Repositories\\CollegeProjects\\CGAssignments\\FinalProject\\Models\\backpack\\backpack.obj");
    //Renderable* backpack = new Renderable(model);
    //backpack->move(glm::vec3(-5.0f, -2.0f, -2.0f));
    //backpack->updateBoundary();
    //_objects.push_back(backpack);
    //
    //Renderable* backpack2 = new Renderable(model);
    //backpack2->makeLight();
    //backpack2->originalLight()->setIdealDistance(500);
    //_objects.push_back(backpack2);
    // Test Code End
    
    _camera.setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
}

void SceneViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SceneViewer::paintGL() {
    Logger::debug("Repainting");
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set view and projection matrices
    glm::mat4 view = _camera.viewMatrix();
    glm::mat4 projection = _camera.projectionMatrix((float)width() / (float)height());

    // Sky Box Render ---------------------------------------------------
    if (_sky != nullptr) {
        _skyShader.bind();
        _skyShader.setUniform("view", glm::mat4(glm::mat3(view)));
        _skyShader.setUniform("projection", projection);
        _sky->render();
        _skyShader.unbind();
    }
    // ------------------------------------------------------------------

    // Terrain Render ---------------------------------------------------
    if (_terrain != nullptr) {
        _terrainShader.bind();
        _terrainShader.setUniform("view", view);
        _terrainShader.setUniform("projection", projection);
        _terrainShader.setUniform("model", _terrain->modelMatrix());
        _terrainShader.setUniform("texture1", 2);
        _terrain->render();
        _terrainShader.unbind();
    }
    // ------------------------------------------------------------------

    // Renderable Render ------------------------------------------------
    _shaderProgram.bind();

    _shaderProgram.setUniform("view", view);
    _shaderProgram.setUniform("projection", projection);
    _shaderProgram.setUniform("viewPos", _camera.position());

    int pointLights = 0;
    int spotLights = 0;

    // Update lights
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

    // Render objects
    for (auto object : _objects) {
        if (object == _pressedObject) {
            _shaderProgram.setUniform("selColor", glm::vec3(0.22f));
        }
        else if (object == _operatingObject) {
            _shaderProgram.setUniform("selColor", glm::vec3(0.1f));
        }
        else if (object == _hoveredObject) {
            _shaderProgram.setUniform("selColor", glm::vec3(0.2f));
        }
        else {
            _shaderProgram.setUniform("selColor", glm::vec3(0.0f));
        }
        object->render(_shaderProgram);
    }

    _shaderProgram.unbind();
    // ------------------------------------------------------------------

    // Bound box render -------------------------------------------------
    if (_selectedObject != nullptr && !_hideBound) {
        _boundShader.bind();
        _boundShader.setUniform("view", view);
        _boundShader.setUniform("projection", projection);
        _selectedObject->boundary().render();
        _boundShader.unbind();
    }
    if (_hoveredObject != nullptr && _hoveredObject != _selectedObject) {
        _boundShader.bind();
        _boundShader.setUniform("view", view);
        _boundShader.setUniform("projection", projection);
        _hoveredObject->boundary().render();
        _boundShader.unbind();
    }
    // ------------------------------------------------------------------
}

void SceneViewer::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        _pressedObject = _hoveredObject;
    }
    else {
        _lastMousePosition = event->pos();
    }

    parentWidget()->update();
    setFocus();
}

void SceneViewer::mouseReleaseEvent(QMouseEvent* event) {
    // State transfer
    bool startOperatingObject = false;
    if (_operatingObject != nullptr) {
        // Click when having an operating object
        _operatingObject->updateBoundary();
        if (!_dragged) {
            // if haven't changed since last mouse press, it's a submission click
            _operatingObject = nullptr;
            _hideBound = false;
        }
        else {
            // dragged, keep it operational
            _dragged = false;
            _hideBound = true;
            _operatingObject = _operatingObject;
        }
    }
    else if (_pressedObject != nullptr && _pressedObject == _selectedObject) {
        // Double select on an object, set in operating mode
        _operatingObject = _selectedObject;
        _hideBound = true;
        startOperatingObject = true;
    }
    else if (_dragged) {
        _dragged = false;
        _hideBound = false;
        if (_selectedObject != nullptr) {
            _selectedObject->updateBoundary();
        }
    }
    else {
        _selectedObject = _pressedObject;
        _hideBound = false;
        emit onSelect(_selectedObject);
    }
    
    // Reset pressed object
    _pressedObject = nullptr;

    // Update hover object
    float relX = (float)event->x() / (float)width();
    float relY = 1 - (float)event->y() / (float)height();
    Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
    _hoveredObject = hitTest(ray);
    
    if (startOperatingObject) {
        // If just setted to operating mode, move the object
        moveOperatingObject(ray);
    }

    // Update the view
    parentWidget()->update();
}

void SceneViewer::mouseMoveEvent(QMouseEvent* event) {
    // Check the type of button pressed
    switch (event->buttons()) {
        case Qt::LeftButton: {
            if (_selectedObject != nullptr) {
                // Set dragged
                _dragged = true;
                // Hide boundary
                _hideBound = true;
                // Rotate around camera up
                glm::vec2 delta = glm::vec2(event->x() - _lastMousePosition.x(), event->y() - _lastMousePosition.y());
                _selectedObject->rotate(_camera.up(), delta.x * 0.01f);
                // Rotate around camera right
                _selectedObject->rotate(_camera.right(), delta.y * 0.01f);
                emit onUpdate(_selectedObject);
            }
            break;
        }
        case Qt::RightButton: {
            // Set dragged
            _dragged = true;
            moveCamera(event);
            break;
        }
        case Qt::MiddleButton: {
            if (_controlPressed && _selectedObject != nullptr) {
                // Set dragged
                _dragged = true;
                // Hide boundary
                _hideBound = true;
                // Scale object
                glm::vec2 delta = glm::vec2(event->x() - _lastMousePosition.x(), event->y() - _lastMousePosition.y());
                _selectedObject->scale(-delta.y * 0.01f);
                emit onUpdate(_selectedObject);
            }
            else {
                // Set dragged
                _dragged = true;
                rotateCamera(event);
            }
            break;
        }
        case Qt::NoButton: {
            float relX = (float)event->x() / (float)width();
            float relY = 1 - (float)event->y() / (float)height();
            Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
            if (_operatingObject == nullptr) {
                // If no button pressed, do hit test and move the current object if selected
                _hoveredObject = hitTest(ray);
                if (_hoveredObject != nullptr) {
                    setCursor(Qt::PointingHandCursor);
                }
                else {
                    setCursor(Qt::ArrowCursor);
                }
            }
            else {
                moveOperatingObject(ray);
                emit onUpdate(_selectedObject);
            }
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

void SceneViewer::moveCamera(QMouseEvent* event) {
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
    if (_operatingObject != nullptr) {
        float relX = (float)event->x() / (float)width();
        float relY = 1 - (float)event->y() / (float)height();
        Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
        moveOperatingObject(ray);
    }
}

void SceneViewer::rotateCamera(QMouseEvent* event) {
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
    if (_operatingObject != nullptr) {
        float relX = (float)event->x() / (float)width();
        float relY = 1 - (float)event->y() / (float)height();
        Ray ray = _camera.generateRay(glm::vec2(relX, relY), (float)width() / (float)height());
        moveOperatingObject(ray);
    }
}

void SceneViewer::keyPressEvent(QKeyEvent* event) {
    Logger::debug("Detect keypress " + std::to_string(event->key()));
    // If ctrl pressed
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        Logger::debug("Control pressed");
        _controlPressed = true;
    }
}

void SceneViewer::keyReleaseEvent(QKeyEvent* event) {
    // If no control pressed
    if (!(event->modifiers().testFlag(Qt::ControlModifier))) {
        Logger::debug("Control released");
        _controlPressed = false;
    }
}

void SceneViewer::moveOperatingObject(const Ray& ray) {
    // Current moving object
    hitTest(ray);
    if (!_hitRecord.hitted()) {
        // Move to the direction of current ray
        glm::vec3 target = _camera.position() + ray.direction() * 15.0f;
        _operatingObject->setPosition(target);
        _operatingObject->updateBoundary();
    }
    // Move the object so that the bottom center of the object is at the hit point
    else if (_stickToSurface) {
        // Stick the bottom center of the model to the surface

        // Clear current translation and rotation while keeping scale
        _operatingObject->setPosition(glm::vec3(0.0f));
        _operatingObject->setRotation(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
        _operatingObject->updateBoundary();

        // Set the bottom center of the model at local origin
        glm::vec3 bottomCenter = _operatingObject->boundary().bottomCenterPoint();
        _operatingObject->move(-bottomCenter);

        // Rotate the model to align with the surface normal
        glm::vec3 normal = _hitRecord.normal();
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 axis = glm::cross(up, normal);
        float angle = glm::acos(glm::dot(up, normal));
        _operatingObject->rotate(axis, angle);

        // Move the model to the hit point
        glm::vec3 hitPoint = _hitRecord.position();
        _operatingObject->move(hitPoint);

        // Update boundary
        _operatingObject->updateBoundary();
    }
    else {
        // Move the object to the hit point
        _operatingObject->setPosition(_hitRecord.position());
    }
}

void SceneViewer::addObject(Model* model) {
    makeCurrent();
    Model* newModel = model->copyToCurrentContext();
    Renderable* newObject = new Renderable(newModel);
    _selectedObject = newObject;
    _operatingObject = newObject;
    _objects.push_back(newObject);
    parentWidget()->update();
    emit onSelect(_selectedObject);
}

void SceneViewer::deleteObject() {
    if (_selectedObject == nullptr) {
        return;
    }
    makeCurrent();
    for (auto it = _objects.begin(); it != _objects.end(); ++it) {
        if (*it == _selectedObject) {
            _objects.erase(it);
            break;
        }
    }
    delete _selectedObject;
    if (_hoveredObject == _selectedObject) {
        _hoveredObject = nullptr;
    }
    if (_pressedObject == _selectedObject) {
        _pressedObject = nullptr;
    }
    _selectedObject = nullptr;
    _operatingObject = nullptr;
    emit onSelect(nullptr);
    parentWidget()->update();
}
