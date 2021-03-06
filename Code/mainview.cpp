#include "mainview.h"
#include "model.h"
#include "vertex.h"

#include <math.h>
#include <QDateTime>


/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    makeCurrent();
    debugLogger->stopLogging();
    delete debugLogger;

    qDebug() << "MainView destructor";
    destroyModelBuffers();
    doneCurrent();
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ),
             this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0, 1.0, 0.0, 1.0);

    createShaderProgram();
    loadMesh();

    // Initialize transformations
    updateProjectionTransform();
    updateModelTransforms();
}

void MainView::createShaderProgram()
{
    // Create shader programs
    shaderProgramNormal.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_normal.glsl");
    shaderProgramNormal.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_normal.glsl");
    shaderProgramNormal.link();

    shaderProgramGour.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_gouraud.glsl");
    shaderProgramGour.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_gouraud.glsl");
    shaderProgramGour.link();

    shaderProgramPhong.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_phong.glsl");
    shaderProgramPhong.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_phong.glsl");
    shaderProgramPhong.link();

    shaderProgram = &shaderProgramPhong;

    // Get the uniforms
    uniformModelViewTransform = shaderProgram->uniformLocation("modelViewTransform");
    uniformNormalTransform = shaderProgram->uniformLocation("normalTransform");
    uniformProjectionTransform = shaderProgram->uniformLocation("projectionTransform");
    uniformMaterialColour = shaderProgram->uniformLocation("materialColour");
    uniformLightPosition = shaderProgram->uniformLocation("lightPosition");
    uniformSampler = shaderProgram->uniformLocation("sampler");
}

void MainView::loadMesh()
{
    materialColour = QVector3D(1,0.3,0.75);
    lightPosition = QVector3D(2,2,2);

    Model model(":/models/cat.obj");
    QVector<QVector3D> vertexCoords = model.getVertices();
    QVector<QVector3D> vertexNormals = model.getNormals();
    QVector<QVector2D> vertexTextureCoords = model.getTextureCoords();

    QVector<float> meshData;
    meshData.reserve(vertexCoords.size() + vertexNormals.size() + vertexTextureCoords.size());

    for (int i = 0; i < vertexCoords.length(); i++)
    {
        meshData.append(vertexCoords[i].x());
        meshData.append(vertexCoords[i].y());
        meshData.append(vertexCoords[i].z());
        meshData.append(vertexNormals[i].x());
        meshData.append(vertexNormals[i].y());
        meshData.append(vertexNormals[i].z());
        meshData.append(vertexTextureCoords[i].x());
        meshData.append(vertexTextureCoords[i].y());
    }

    meshSize = meshData.size();

    // Generate VAO
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);

    // Generate VBO
    glGenBuffers(1, &meshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

    // Write the data to the buffer
    glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(float), meshData.data(), GL_STATIC_DRAW);

    // Set vertex coordinates to location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Set colour coordinates to location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set texture coordinates to location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    loadTexture(":/textures/cat_diff.png");
}

void MainView::loadTexture(QString textureString) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLfloat f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&f);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,f);//This and previous 2 lines should enable anisotropic filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    textureImage = imageToBytes(QImage(textureString));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.data());
}

// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    // Clear the screen before rendering
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    (*shaderProgram).bind();

    // Set the projection matrix
    glUniformMatrix4fv(uniformProjectionTransform, 1, GL_FALSE, projectionTransform.data());
    glUniformMatrix4fv(uniformModelViewTransform, 1, GL_FALSE, meshTransform.data());
    glUniformMatrix3fv(uniformNormalTransform, 1, GL_FALSE, normalTransform.data());

    glUniform3f(uniformMaterialColour, materialColour[0], materialColour[1], materialColour[2]);
    glUniform3f(uniformLightPosition, lightPosition[0], lightPosition[1], lightPosition[2]);

    glUniform1i(uniformSampler, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, meshSize);

    (*shaderProgram).release();
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight)
{
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
    updateProjectionTransform();
}

void MainView::updateProjectionTransform()
{
    float aspect_ratio = static_cast<float>(width()) / static_cast<float>(height());
    projectionTransform.setToIdentity();
    projectionTransform.perspective(60, aspect_ratio, 0.2, 20);
}

void MainView::updateModelTransforms()
{
    meshTransform.setToIdentity();
    meshTransform.translate(0, -0.5, -2);
    meshTransform.scale(scale);
    meshTransform.rotate(QQuaternion::fromEulerAngles(rotation));

    normalTransform = meshTransform.normalMatrix();

    update();
}

// --- OpenGL cleanup helpers

void MainView::destroyModelBuffers()
{
    glDeleteBuffers(1, &meshVBO);
    glDeleteVertexArrays(1, &meshVAO);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    rotation = { static_cast<float>(rotateX), static_cast<float>(rotateY), static_cast<float>(rotateZ) };
    updateModelTransforms();
}

void MainView::setScale(int newScale)
{
    scale = static_cast<float>(newScale) / 100.f;
    updateModelTransforms();
}

void MainView::setShadingMode(ShadingMode shading)
{
    switch(shading) {

        case 1:
            shaderProgram = &shaderProgramNormal;
        break;

        case 2:
            shaderProgram = &shaderProgramGour;
        break;

        case 0:
            shaderProgram = &shaderProgramPhong;
        break;
    }
    update();
}

// --- Private helpers

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}

