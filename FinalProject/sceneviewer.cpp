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

    _shaderProgram.ensureInitialized();
    Logger::info("Shader Program initialized");

    VertexShader vertexShader("./temp/shaders/vertexshader.vs");
    FragmentShader fragmentShader("./temp/shaders/fragmentshader.fs");
    _shaderProgram.attachShader(vertexShader);

}

void SceneViewer::resizeGL(int w, int h) {
    
}

void SceneViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*
    vector<Vertex> vertices = {
        { { -0.5f, -0.5f, 0.0f } },
        { { 0.5f, -0.5f, 0.0f } },
        { { 0.0f, 0.5f, 0.0f } }
    };
    VertexBufferObject vbo(vertices);
    VertexArrayObject vao(vbo);
    vao.setVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    vao.enableVertexAttribute(0);
*/


    VertexShader vertexShader("D:\\code\\ComputerGraphic\\SceneEditor\\FinalProject\\vertexshader.vs");
    FragmentShader fragmentShader("D:\\code\\ComputerGraphic\\SceneEditor\\FinalProject\\fragmentshader.fs");
    ShaderProgram shaderProgram(vertexShader, fragmentShader);
    shaderProgram.setActive();

    Model test_model("D:\\code\\ComputerGraphic\\SceneEditor\\obj\\nanosuit/nanosuit.obj");
    test_model.render(shaderProgram);
    
    //vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}