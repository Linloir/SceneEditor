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

    _vao.ensureInitialized();
    Logger::info("Vertex Array Object initialized");
    
    vector<Vertex> vertices = {
        { { -0.5f, -0.5f, 0.0f } },
        { { 0.5f, -0.5f, 0.0f } },
        { { 0.0f, 0.5f, 0.0f } }
    };
    VertexBufferObject vbo(vertices);
    Logger::info("Vertex Buffer Object initialized");
    
    _vao.bindVertexBufferObject(vbo);
    _vao.setVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    _vao.enableVertexAttribute(0);
    Logger::info("Vertex Buffer Object bound to Vertex Array Object");

    _shaderProgram.ensureInitialized();
    Logger::info("Shader Program initialized");

    VertexShader vertexShader("./temp/shaders/vertexshader.vs");
    FragmentShader fragmentShader("./temp/shaders/fragmentshader.fs");
    _shaderProgram.attachShader(vertexShader);
    _shaderProgram.attachShader(fragmentShader);
    vertexShader.dispose();
    fragmentShader.dispose();

    Model* backpackModel = new Model("D:/code/ComputerGraphic/SceneEditor/obj/nanosuit/nanosuit.obj");
    Logger::info("Model loaded");
    Renderable renderable(backpackModel);
    _objects.push_back(backpackModel);
    
    //renderable.scale(1);
    //renderable.rotate(glm::vec3(1, 0, 0), 2);
    //renderable.check_boundary();
    //auto t1 = renderable.get_lower_boundary();
    //auto t2 = renderable.get_upper_boundary();

    _camera.setPosition(glm::vec3(0.0f, 15.0f, 8.0f));
    _camera.setYaw(-90.0f);
    _camera.setPitch(-30.0f);



}

void SceneViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SceneViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    _shaderProgram.bind();

    // Set view and projection matrices
    glm::mat4 view = _camera.viewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(_camera.zoom()), (float)width() / (float)height(), 0.1f, 100.0f);
    _shaderProgram.setUniform("view", view);
    _shaderProgram.setUniform("projection", projection);

    for (auto object : _objects) {
        object.render(_shaderProgram);
    }
}