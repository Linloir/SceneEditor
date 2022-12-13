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
    QFile::copy(":/shaders/vertexshader.vs", "./temp/shaders/vertexshader.vs");
    QFile::copy(":/shaders/fragmentshader.fs", "./temp/shaders/fragmentshader.fs");
}

SceneViewer::~SceneViewer() {

}

void SceneViewer::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    Model* backpackModel = new Model("E:\\Repositories\\CollegeProjects\\CGAssignments\\FinalProject\\Models\\backpack\\backpack.obj");
    Logger::info("Model loaded");
    Renderable renderable(backpackModel);
    _objects.push_back(backpackModel);
}

void SceneViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SceneViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    _shaderProgram.bind();
    
    for (auto object : _objects) {
        object.render(_shaderProgram);
    }
}