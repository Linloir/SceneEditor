#include "sceneviewer.h"

#include <vector>
#include <string>

#include "vbo.h"
#include "vao.h"
#include "shader.h"
#include "logger.h"

using std::vector;

SceneViewer::SceneViewer(QWidget* parent)
	: QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 3);
    setFormat(format);
}

SceneViewer::~SceneViewer() {

}

void SceneViewer::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Logger::info("Currently running on OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));
}

void SceneViewer::resizeGL(int w, int h) {
    
}

void SceneViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    vector<Vertex> vertices = {
        { { -0.5f, -0.5f, 0.0f } },
        { { 0.5f, -0.5f, 0.0f } },
        { { 0.0f, 0.5f, 0.0f } }
    };
    VertexBufferObject vbo(vertices);
    VertexArrayObject vao(vbo);
    vao.setVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    vao.enableVertexAttribute(0);
    VertexShader vertexShader("E:\\Repositories\\CollegeProjects\\CGAssignments\\FinalProject\\FinalProject\\vertexshader.vs");
    FragmentShader fragmentShader("E:\\Repositories\\CollegeProjects\\CGAssignments\\FinalProject\\FinalProject\\fragmentshader.fs");
    ShaderProgram shaderProgram(vertexShader, fragmentShader);
    shaderProgram.setActive();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}