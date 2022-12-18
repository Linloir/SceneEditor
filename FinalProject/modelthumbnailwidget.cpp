#include <qfile.h>
#include <qdir.h>

#include "modelthumbnailwidget.h"

ModelThumbnailWidget::ModelThumbnailWidget(Model* model, QWidget* parent) : 
    QOpenGLWidget(parent), _model(model)
{
    // OpenGL initialize
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

ModelThumbnailWidget::~ModelThumbnailWidget() {
}

void ModelThumbnailWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    _shaderProgram.ensureInitialized();
    
    VertexShader vertexShader("./temp/shaders/vertexshader.vs");
    FragmentShader fragmentShader("./temp/shaders/fragmentshader.fs");
    _shaderProgram.attachShader(vertexShader);
    _shaderProgram.attachShader(fragmentShader);
    vertexShader.dispose();
    fragmentShader.dispose();

    _object.setModel(_model->copyToCurrentContext());
    _camera.setPosition(glm::vec3(0.0f, 0.0f, 7.0f));
}

void ModelThumbnailWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shaderProgram.bind();
    
    glm::mat4 view = _camera.viewMatrix();
    glm::mat4 projection = _camera.projectionMatrix((float)width() / (float)height());
    _shaderProgram.setUniform("view", view);
    _shaderProgram.setUniform("projection", projection);
    
    _object.render(_shaderProgram);
    
    _shaderProgram.unbind();
}

void ModelThumbnailWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}