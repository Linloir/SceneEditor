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
    extractShaderResource("thumbnailvertexshader.glsl");
    extractShaderResource("thumbnailfragmentshader.glsl");
}

ModelThumbnailWidget::~ModelThumbnailWidget() {
}

void ModelThumbnailWidget::extractShaderResource(const QString& shaderName) {
    QString shaderResourcePath = ":/shaders/" + shaderName;
    QString shaderTempPath = "./temp/shaders/" + shaderName;

    if (QFile::exists(shaderTempPath))
    {
        QFile::remove(shaderTempPath);
    }
    QFile::copy(shaderResourcePath, shaderTempPath);
    QFile::setPermissions(shaderTempPath, QFile::ReadOwner | QFile::WriteOwner);
}

void ModelThumbnailWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    _shaderProgram.ensureInitialized();
    
    VertexShader vertexShader("./temp/shaders/thumbnailvertexshader.glsl");
    FragmentShader fragmentShader("./temp/shaders/thumbnailfragmentshader.glsl");
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